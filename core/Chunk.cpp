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
	m_blocks = new Block::Face **[CHUNK_WIDTH];
	for (int32 x = 0; x < CHUNK_WIDTH; x++)
	{
		m_blocks[x] = new Block::Face*[CHUNK_HEIGHT];
		for (int32 y = 0; y < CHUNK_HEIGHT; y++)
		{
			m_blocks[x][y] = new Block::Face[CHUNK_DEPTH];
		}
	}

	m_blocks[0][0][0].m_enabled = false;
	m_blocks[15][15][15].m_enabled = false;

	CreateMesh();
}

mj::Chunk::~Chunk()
{
	Destroy();
}

void mj::Chunk::Destroy()
{
	// Delete blocks
	if (m_blocks)
	{
		for (int32 x = 0; x < CHUNK_WIDTH; x++) if (m_blocks[x])
		{
			for (int32 y = 0; y < CHUNK_HEIGHT; y++)
			{
				delete [] m_blocks[x][y];
			}
			delete [] m_blocks[x];
		}
	}
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
	int32 side = 0;

	mj::math::int3 x{ 0, 0, 0 };
	mj::math::int3 q{ 0, 0, 0 };
	mj::math::int3 du{ 0, 0, 0 };
	mj::math::int3 dv{ 0, 0, 0 };

	/*
	* We create a mask - this will contain the groups of matching voxel faces
	* as we proceed through the chunk in 6 directions - once for each face.
	*/
	Block::Face *mask[CHUNK_WIDTH * CHUNK_HEIGHT] = { 0 };

	/*
	We start with the lesser-spotted bool for-loop (also known as the old flippy floppy).

	The variable backFace will be TRUE on the first iteration and FALSE on the second - this allows
	us to track which direction the indices should run during creation of the quad.

	This loop runs twice, and the inner loop 3 times - totally 6 iterations - one for each
	voxel face.
	*/

	// Which way the quad is facing
	int32 sides[6] = { WEST, BOTTOM, NORTH, EAST, TOP, SOUTH };

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
		for (x[d] = -1; x[d] < CHUNK_WIDTH;)
		{
			// We compute the mask
			int32 n = 0;

			for (x[v] = 0; x[v] < CHUNK_HEIGHT; x[v]++)
			{
				for (x[u] = 0; x[u] < CHUNK_WIDTH; x[u]++)
				{
					// Here we retrieve two voxel faces for comparison.
					Block::Face *voxelFace = nullptr;
					Block::Face *voxelFace1 = nullptr;
					if (x[d] >= 0)
					{
						voxelFace = getVoxelFace(x, side);
					}
					if (x[d] < CHUNK_WIDTH - 1)
					{
						voxelFace1 = getVoxelFace(x + q, side);
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

			for (int32 j = 0; j < CHUNK_HEIGHT; j++)
			{
				for (int32 i = 0; i < CHUNK_WIDTH;)
				{
					if (mask[n])
					{
						int32 width = 0;
						int32 height = 0;
						// Compute width
						for (width = 1; i + width < CHUNK_WIDTH && mask[n + width] != nullptr && mask[n + width]->equals(*mask[n]); width++) {}

						// Compute height
						bool done = false;
						for (height = 1; j + height < CHUNK_HEIGHT; height++)
						{
							for (int32 k = 0; k < width; k++)
							{
								if (mask[n + k + height * CHUNK_WIDTH] == nullptr || !mask[n + k + height * CHUNK_WIDTH]->equals(*mask[n]))
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
						* Here we check the "transparent" attribute in the Block::Face class to ensure that we don't mesh
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
							* We pass mask[n] to the function, which is an instance of the Block::Face class containing
							* all the attributes of the face - which allows for variables to be passed to shaders - for
							* example lighting values used to create ambient occlusion.
							*/
							quad(mesh, math::float3(x[0], x[1], x[2]),
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
								mask[n + k + l * CHUNK_WIDTH] = nullptr;
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
	GL_TRY(glBufferData(GL_ARRAY_BUFFER, mesh.m_positions.Size() * sizeof(math::float3), &mesh.m_positions[0], GL_STATIC_DRAW));
	GL_TRY(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
	GL_TRY(glEnableVertexAttribArray(0));

	// Color buffer
	GL_TRY(glGenBuffers(1, &m_colorBuffer));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer));
	GL_TRY(glBufferData(GL_ARRAY_BUFFER, mesh.m_colors.Size() * sizeof(math::float4), &mesh.m_colors[0], GL_STATIC_DRAW));
	GL_TRY(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr));
	GL_TRY(glEnableVertexAttribArray(1));

	// Index buffer
	GL_TRY(glGenBuffers(1, &m_indexBuffer));
	GL_TRY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer));
	GL_TRY(glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.m_indices.Size() * sizeof(int32), &mesh.m_indices[0], GL_STATIC_DRAW));

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
mj::Block::Face *mj::Chunk::getVoxelFace(math::int3 xyz, int32 side)
{

	Block::Face &voxelFace = m_blocks[xyz.x][xyz.y][xyz.z];

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
void mj::Chunk::quad(
	Mesh &mesh,
	const mj::math::float3 &bottomLeft,
	const mj::math::float3 &topLeft,
	const mj::math::float3 &topRight,
	const mj::math::float3 &bottomRight,
	int32 width,
	int32 height,
	const Block::Face &voxel,
	bool backFace)
{

	mj::math::float3 vertices[4] = {
		bottomLeft * VOXEL_SIZE,
		topLeft * VOXEL_SIZE,
		topRight * VOXEL_SIZE,
		bottomRight * VOXEL_SIZE,
	};

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

	math::float4 colorArray[4];

	for (int32 i = 0; i < 4; i++)
	{
		/*
		* Here I set different colors for quads depending on the "type" attribute, just
		* so that the different groups of voxels can be clearly seen.
		*/
		if (voxel.type == 1)
		{
			colorArray[i] = { 1.0f, 0.0f, 0.0f, 1.0f };
		}
		else if (voxel.type == 2)
		{
			colorArray[i] = { 0.0f, 1.0f, 0.0f, 1.0f };
		}
		else
		{
			colorArray[i] = { 1.0f, 1.0f, 1.0f, 0.8f };
		}
	}

	mesh.m_positions.Add(vertices, 4);
	mesh.m_colors.Add(colorArray, 4);
	mesh.m_indices.Add(indices, 6);
}

void mj::Chunk::Render()
{
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

mj::Block::Face &mj::Chunk::GetBlock(const mj::math::int3 &idx)
{
	assert(m_blocks); return m_blocks[idx.x][idx.y][idx.z];
}
