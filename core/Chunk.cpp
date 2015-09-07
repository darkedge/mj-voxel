#include "CorePCH.h"
#include "Chunk.h"
#include "Block.h"

#define VOXEL_SIZE 1.0f

mj::Chunk::Chunk()
{
	//members.m_position = 0;
	m_vertexBuffer = 0;
	m_vertexArray = 0;
	m_indexBuffer = 0;
	//members.m_numTris = 0;
}

mj::Chunk::Chunk(const math::float3 &position)
{
	m_position = position;
	Init();
}

#if 0
mj::Chunk::Chunk( const Chunk &other )
{
	*this = other;
}
#endif

// TODO: Check if this is the same as default move Ctor
mj::Chunk::Chunk(Chunk &&other)
{
	*this = static_cast<Chunk&&>(other);
}

void mj::Chunk::Init()
{
	// Create blocks
	m_blocks = new Block[WIDTH * HEIGHT * DEPTH];

	// Remove some blocks for testing
	m_blocks[0].m_enabled = false;
	m_blocks[WIDTH * HEIGHT * DEPTH - 1].m_enabled = false;

	m_blocks[8 * HEIGHT * DEPTH + 8 * DEPTH + 15].m_enabled = false;
	m_blocks[8 * HEIGHT * DEPTH + 8 * DEPTH + 0].m_enabled = false;

	m_blocks[8 * HEIGHT * DEPTH + 15 * DEPTH + 8].m_enabled = false;
	m_blocks[8 * HEIGHT * DEPTH + 0 * DEPTH + 8].m_enabled = false;

	m_blocks[15 * HEIGHT * DEPTH + 8 * DEPTH + 8].m_enabled = false;
	m_blocks[0 * HEIGHT * DEPTH + 8 * DEPTH + 8].m_enabled = false;

	CreateMesh();
}

mj::Chunk::~Chunk()
{
	Destroy();
}

void mj::Chunk::Destroy()
{
	delete [] m_blocks;

	// Delete GL buffer (TODO: RAII buffer class?)
	GL_TRY(glDeleteBuffers(1, &m_vertexBuffer));
	m_vertexBuffer = 0;
	GL_TRY(glDeleteBuffers(1, &m_indexBuffer));
	m_indexBuffer = 0;
	GL_TRY(glDeleteVertexArrays(1, &m_vertexArray));
	m_vertexArray = 0;
}

void mj::Chunk::CreateMesh()
{
	Mesh mesh;

	/*
	* These are just working variables for the algorithm - almost all taken
	* directly from Mikola Lysenko's javascript implementation.
	*/
	mj::math::int3 x{ 0, 0, 0 };
	mj::math::int3 q{ 0, 0, 0 };
	mj::math::int3 du{ 0, 0, 0 };
	mj::math::int3 dv{ 0, 0, 0 };

	/*
	* We create a mask - this will contain the groups of matching voxel faces
	* as we proceed through the chunk in 6 directions - once for each face.
	*/
	Block *mask[WIDTH * HEIGHT] = { 0 };

	/*
	We start with the lesser-spotted bool for-loop (also known as the old flippy floppy).

	The variable backFace will be TRUE on the first iteration and FALSE on the second - this allows
	us to track which direction the indices should run during creation of the quad.

	This loop runs twice, and the inner loop 3 times - totally 6 iterations - one for each
	voxel face.
	*/

	// Which way the quad is facing
	Block::ESide sides[6] = {
		Block::ESide::West,
		Block::ESide::Bottom,
		Block::ESide::North,
		Block::ESide::East,
		Block::ESide::Top,
		Block::ESide::South,
	};
	Block::ESide side;

	for (int32 p = 0; p < 6; p++)
	{
		int32 d = p % 3;
		bool backFace = p < 3;

		// 0 1 2 3 4 5
		int32 u = (d + 1) % 3; // 1 2 0
		int32 v = (d + 2) % 3; // 2 0 1

		x[0] = 0;
		x[1] = 0;
		x[2] = 0;

		q[0] = 0;
		q[1] = 0;
		q[2] = 0;
		q[d] = 1;

		// Here we're keeping track of the side that we're meshing.
		side = sides[p];

		// We move through the dimension from front to back
		for (x[d] = -1; x[d] < WIDTH;)
		{
			// We compute the mask
			int32 n = 0;

			for (x[v] = 0; x[v] < HEIGHT; x[v]++)
			{
				for (x[u] = 0; x[u] < WIDTH; x[u]++)
				{
					// Here we retrieve two voxel faces for comparison.
					Block *voxelFace = nullptr;
					Block *voxelFace1 = nullptr;
					if (x[d] >= 0)
					{
						voxelFace = GetVoxelFace(x, side);
					}
					if (x[d] < WIDTH - 1)
					{
						voxelFace1 = GetVoxelFace(x + q, side);
					}

					/*
					* Note that we're using the equals function in the voxel face class here, which lets the faces
					* be compared based on any number of attributes.
					*
					* Also, we choose the face to add to the mask depending on whether we're moving through on a backface or not.
					*/
					if (voxelFace && voxelFace1 && voxelFace->equals(*voxelFace1))
					{
						mask[n] = nullptr;
					}
					else
					{
						if (backFace)
						{
							mask[n] = voxelFace1;
						}
						else
						{
							mask[n] = voxelFace;
						}
					}
					n++;
				} // End u loop
			} // End v loop

			x[d]++;

			// Now we generate the mesh for the mask
			n = 0;

			for (int32 j = 0; j < HEIGHT; j++)
			{
				for (int32 i = 0; i < WIDTH;)
				{
					if (mask[n])
					{
						int32 width = 0;
						int32 height = 0;
						// Compute width
						for (width = 1; i + width < WIDTH && mask[n + width] != nullptr && mask[n + width]->equals(*mask[n]); width++) {}

						// Compute height
						bool done = false;
						for (height = 1; j + height < HEIGHT; height++)
						{
							for (int32 k = 0; k < width; k++)
							{
								if (mask[n + k + height * WIDTH] == nullptr || !mask[n + k + height * WIDTH]->equals(*mask[n]))
								{
									done = true;
									break;
								}
							}

							if (done)
							{
								break;
							}
						}

						/*
						* Here we check the "transparent" attribute in the Block class to ensure that we don't mesh
						* any culled faces.
						*/
						if (mask[n]->m_enabled)
						{
							/*
							* Add quad
							*/
							x[u] = i;
							x[v] = j;

							du[0] = 0;
							du[1] = 0;
							du[2] = 0;
							du[u] = width;

							dv[0] = 0;
							dv[1] = 0;
							dv[2] = 0;
							dv[v] = height;

							/*
							* And here we call the quad function in order to render a merged quad in the scene.
							*
							* We pass mask[n] to the function, which is an instance of the Block class containing
							* all the attributes of the face - which allows for variables to be passed to shaders - for
							* example lighting values used to create ambient occlusion.
							*/
							mask[n]->side = side;
							AddQuadToMesh(mesh, math::float3(x[0], x[1], x[2]),
								math::float3(x[0] + du[0], x[1] + du[1], x[2] + du[2]),
								math::float3(x[0] + du[0] + dv[0], x[1] + du[1] + dv[1], x[2] + du[2] + dv[2]),
								math::float3(x[0] + dv[0], x[1] + dv[1], x[2] + dv[2]),
								width,
								height,
								*mask[n],
								backFace);
						}

						// We zero out the mask
						for (int32 l = 0; l < height; l++)
						{
							for (int32 k = 0; k < width; k++)
							{
								mask[n + k + l * WIDTH] = nullptr;
							}
						}


						// And then finally increment the counters and continue
						i += width;
						n += width;
					}
					else
					{
						i++;
						n++;
					}
				}
			} // End mesh loop
		} // End chunk size loop
	} // End axis loop

	// Create VAO
	GL_TRY(glGenVertexArrays(1, &m_vertexArray));
	GL_TRY(glBindVertexArray(m_vertexArray));

	// Position buffer
	GL_TRY(glGenBuffers(1, &m_vertexBuffer));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer));
	if (mesh.m_positions.Size() > 0) GL_TRY(glBufferData(GL_ARRAY_BUFFER, mesh.m_positions.Size() * sizeof(math::float3), &mesh.m_positions[0], GL_STATIC_DRAW));
	GL_TRY(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
	GL_TRY(glEnableVertexAttribArray(0));

	// Color buffer
	GL_TRY(glGenBuffers(1, &m_colorBuffer));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer));
	if (mesh.m_colors.Size() > 0) GL_TRY(glBufferData(GL_ARRAY_BUFFER, mesh.m_colors.Size() * sizeof(math::float4), &mesh.m_colors[0], GL_STATIC_DRAW));
	GL_TRY(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr));
	GL_TRY(glEnableVertexAttribArray(1));

	// Texcoord buffer
	GL_TRY(glGenBuffers(1, &m_texCoordBuffer));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer));
	if (mesh.m_texcoords.Size() > 0) GL_TRY(glBufferData(GL_ARRAY_BUFFER, mesh.m_texcoords.Size() * sizeof(math::float2), &mesh.m_texcoords[0], GL_STATIC_DRAW));
	GL_TRY(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr));
	GL_TRY(glEnableVertexAttribArray(2));

	// Index buffer
	GL_TRY(glGenBuffers(1, &m_indexBuffer));
	GL_TRY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer));
	if (mesh.m_indices.Size() > 0) GL_TRY(glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.m_indices.Size() * sizeof(int32), &mesh.m_indices[0], GL_STATIC_DRAW));

	m_numTris = mesh.m_indices.Size() / 3;

	// Unbind
	GL_TRY(glBindVertexArray(0));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_TRY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

/**
* This function returns an instance of VoxelFace containing the attributes for
* one side of a voxel.  In this simple demo we just return a value from the
* sample data array.  However, in an actual voxel engine, this function would
* check if the voxel face should be culled, and set per-face and per-vertex
* values as well as voxel values in the returned instance.
*/
mj::Block *mj::Chunk::GetVoxelFace(math::int3 xyz, Block::ESide side)
{

	Block &voxelFace = m_blocks[xyz.x * HEIGHT * DEPTH + xyz.y * DEPTH + xyz.z];

	voxelFace.side = side;

	if (voxelFace.m_enabled)
	{
		return &voxelFace;
	}
	else
	{
		return nullptr;
	}
}

/**
* This function renders a single quad in the scene. This quad may represent many adjacent voxel
* faces - so in order to create the illusion of many faces, you might consider using a tiling
* function in your voxel shader. For this reason I've included the quad width and height as parameters.
*
* For example, if your texture coordinates for a single voxel face were 0 - 1 on a given axis, they should now
* be 0 - width or 0 - height. Then you can calculate the correct texture coordinate in your fragement
* shader using coord.xy = fract(coord.xy).
*/
void mj::Chunk::AddQuadToMesh(
	Mesh &mesh,
	const mj::math::float3 &bottomLeft,
	const mj::math::float3 &topLeft,
	const mj::math::float3 &topRight,
	const mj::math::float3 &bottomRight,
	int32 width,
	int32 height,
	const Block &voxel,
	bool backFace)
{

	mj::math::float3 vertices[4] = {
		bottomLeft * VOXEL_SIZE,
		topLeft * VOXEL_SIZE,
		topRight * VOXEL_SIZE,
		bottomRight * VOXEL_SIZE,
	};

	if (voxel.side == Block::ESide::North
		|| voxel.side == Block::ESide::South)
	{
		std::swap(width, height);
	}

	// Rotate by shifting by one
	math::float2 texcoords [] = {
		math::float2(0.0f, float(width)),
		math::float2(float(height), float(width)),
		math::float2(float(height), 0.0f),
		math::float2(0.0f, 0.0f),
		math::float2(0.0f, float(width)),
	};

	math::float2 flipped [] = {
		math::float2(float(height), float(width)),
		math::float2(0.0f, float(width)),
		math::float2(0.0f, 0.0f),
		math::float2(float(height), 0.0f),
		math::float2(float(height), float(width)),
	};

	math::float2 *texcoordptr = texcoords;

	int32 indices[6];
	{
		if (backFace)
		{
			int32 a[6] = { 0, 3, 2, 2, 1, 0 };
			memcpy(indices, a, sizeof(indices));
		}
		else
		{
			int32 a[6] = { 0, 1, 2, 2, 3, 0 };
			memcpy(indices, a, sizeof(indices));
		}
	}

	// Offset with existing data count
	for (int32 i = 0; i < 6; i++)
	{
		indices[i] += mesh.m_positions.Size();
	}

	for (int32 i = 0; i < 4; i++)
	{
		switch (voxel.side)
		{
		case Block::ESide::North:
			texcoordptr = flipped;
			break;
		case Block::ESide::East:
			texcoordptr = texcoords + 1;
			break;
		case Block::ESide::South:
			texcoordptr = texcoords;
			break;
		case Block::ESide::West:
			texcoordptr = flipped + 1;
			break;
		case Block::ESide::Top:
			texcoordptr = texcoords + 1;
			break;
		case Block::ESide::Bottom:
			texcoordptr = flipped + 1;
			break;
		default:
			break;
		}
	}

	mesh.m_texcoords.Add(texcoordptr, 4);
	mesh.m_positions.Add(vertices, 4);
	//mesh.m_colors.Add(colorArray, 4);
	mesh.m_indices.Add(indices, 6);
}

void mj::Chunk::Render()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GL_TRY(glBindVertexArray(m_vertexArray));
	GL_TRY(glDrawElements(GL_TRIANGLES, m_numTris * 3, GL_UNSIGNED_INT, 0));
	GL_TRY(glBindVertexArray(0));
}

#if 0
mj::Chunk& mj::Chunk::operator=(const mj::Chunk &other)
{
	if (this != &other)
	{
		Destroy();
		this->m_position = other.m_position;
		if (other.m_blocks)
		{
			m_blocks = new Block **[CHUNK_WIDTH];
			for (int32 x = 0; x < CHUNK_WIDTH; x++)
			{
				m_blocks[x] = new Block*[CHUNK_HEIGHT];
				for (int32 y = 0; y < CHUNK_HEIGHT; y++)
				{
					m_blocks[x][y] = new Block[CHUNK_DEPTH];
					memcpy(m_blocks[x][y], other.m_blocks[x][y], CHUNK_DEPTH * sizeof(Block));
				}
			}
		}
	}

	return *this;
}
#endif

mj::Chunk& mj::Chunk::operator=(mj::Chunk &&other)
{
	if (this != &other)
	{
		Destroy();
		this->m_position = other.m_position;
		this->m_vertexBuffer = other.m_vertexBuffer;
		this->m_vertexArray = other.m_vertexArray;
		this->m_indexBuffer = other.m_indexBuffer;
		this->m_numTris = other.m_numTris;
		this->m_blocks = other.m_blocks;

		// Invalidate other
		other.m_blocks = nullptr;
		other.m_vertexBuffer = 0;
		other.m_vertexArray = 0;
		other.m_indexBuffer = 0;
	}
	return *this;
}

mj::Block *mj::Chunk::GetBlock(int32 x, int32 y, int32 z)
{
	if(x < 0 || x >= Chunk::WIDTH) return nullptr;
	if(y < 0 || y >= Chunk::HEIGHT) return nullptr;
	if(z < 0 || z >= Chunk::DEPTH) return nullptr;

	return &m_blocks[x * Chunk::HEIGHT * Chunk::DEPTH + y * Chunk::DEPTH + z];
}