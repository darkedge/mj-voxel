#include "CorePCH.h"
#include "Chunk.h"

#define VOXEL_SIZE 1.0f

mj::Chunk::Chunk()
{
	//members.m_position = 0;
	members.m_vertexBuffer = 0;
	members.m_vertexArray = 0;
	members.m_indexBuffer = 0;
	//members.m_numTris = 0;
}

mj::Chunk::Chunk(const math::float3 &position)
{
	members.m_position = position;
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
	m_blocks = new Block **[CHUNK_WIDTH];
	for (int32 x = 0; x < CHUNK_WIDTH; x++)
	{
		m_blocks[x] = new Block*[CHUNK_HEIGHT];
		for (int32 y = 0; y < CHUNK_HEIGHT; y++)
		{
			m_blocks[x][y] = new Block[CHUNK_DEPTH];
		}
	}

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
	GL_TRY(glDeleteBuffers(1, &members.m_vertexBuffer));
	members.m_vertexBuffer = 0;
	GL_TRY(glDeleteBuffers(1, &members.m_indexBuffer));
	members.m_indexBuffer = 0;
	GL_TRY(glDeleteVertexArrays(1, &members.m_vertexArray));
	members.m_vertexArray = 0;
}

void mj::Chunk::CreateMesh()
{
	/*
	* These are just working variables for the algorithm - almost all taken
	* directly from Mikola Lysenko's javascript implementation.
	*/
	int32 i, j, k, l, w, h, u, v, n, side = 0;

	mj::math::int3 x{ 0, 0, 0 };
	mj::math::int3 q{ 0, 0, 0 };
	mj::math::int3 du{ 0, 0, 0 };
	mj::math::int3 dv{ 0, 0, 0 };

	/*
	* We create a mask - this will contain the groups of matching voxel faces
	* as we proceed through the chunk in 6 directions - once for each face.
	*/
	Block::Face mask[CHUNK_WIDTH * CHUNK_HEIGHT];

	/*
	* These are just working variables to hold two faces during comparison.
	*/
	Block::Face *voxelFace, *voxelFace1;

	/**
	* We start with the lesser-spotted bool for-loop (also known as the old flippy floppy).
	*
	* The variable backFace will be TRUE on the first iteration and FALSE on the second - this allows
	* us to track which direction the indices should run during creation of the quad.
	*
	* This loop runs twice, and the inner loop 3 times - totally 6 iterations - one for each
	* voxel face.
	*/
	for (bool backFace = true, b = false; b != backFace; backFace = backFace && b, b = !b)
	{

		/*
		* We sweep over the 3 dimensions - most of what follows is well described by Mikola Lysenko
		* in his post - and is ported from his Javascript implementation.  Where this implementation
		* diverges, I've added commentary.
		*/
		for (int32 d = 0; d < 3; d++)
		{

			u = (d + 1) % 3;
			v = (d + 2) % 3;

			x[0] = 0;
			x[1] = 0;
			x[2] = 0;

			q[0] = 0;
			q[1] = 0;
			q[2] = 0;
			q[d] = 1;

			/*
			* Here we're keeping track of the side that we're meshing.
			*/
			if (d == 0) { side = backFace ? WEST : EAST; }
			else if (d == 1) { side = backFace ? BOTTOM : TOP; }
			else if (d == 2) { side = backFace ? SOUTH : NORTH; }

			/*
			* We move through the dimension from front to back
			*/
			for (x[d] = -1; x[d] < CHUNK_WIDTH;)
			{

				/*
				* -------------------------------------------------------------------
				*   We compute the mask
				* -------------------------------------------------------------------
				*/
				n = 0;

				for (x[v] = 0; x[v] < CHUNK_HEIGHT; x[v]++)
				{

					for (x[u] = 0; x[u] < CHUNK_WIDTH; x[u]++)
					{

						/*
						* Here we retrieve two voxel faces for comparison.
						*/
						voxelFace = (x[d] >= 0) ? getVoxelFace(x[0], x[1], x[2], side) : nullptr;
						voxelFace1 = (x[d] < CHUNK_WIDTH - 1) ? getVoxelFace(x[0] + q[0], x[1] + q[1], x[2] + q[2], side) : nullptr;

						/*
						* Note that we're using the equals function in the voxel face class here, which lets the faces
						* be compared based on any number of attributes.
						*
						* Also, we choose the face to add to the mask depending on whether we're moving through on a backface or not.
						*/
						mask[n++] = ((voxelFace != nullptr && voxelFace1 != nullptr && voxelFace->equals(voxelFace1)))
							? nullptr
							: backFace ? voxelFace1 : voxelFace;
					}
				}

				x[d]++;

				/*
				* Now we generate the mesh for the mask
				*/
				n = 0;

				for (j = 0; j < CHUNK_HEIGHT; j++)
				{

					for (i = 0; i < CHUNK_WIDTH;)
					{

						if (mask[n] != nullptr)
						{

							/*
							* We compute the width
							*/
							for (w = 1; i + w < CHUNK_WIDTH && mask[n + w] != nullptr && mask[n + w].equals(mask[n]); w++) {}

							/*
							* Then we compute height
							*/
							bool done = false;

							for (h = 1; j + h < CHUNK_HEIGHT; h++)
							{

								for (k = 0; k < w; k++)
								{

									if (mask[n + k + h * CHUNK_WIDTH] == nullptr || !mask[n + k + h * CHUNK_WIDTH].equals(mask[n])) { done = true; break; }
								}

								if (done) { break; }
							}

							/*
							* Here we check the "transparent" attribute in the Block::Face class to ensure that we don't mesh
							* any culled faces.
							*/
							if (!mask[n].transparent)
							{
								/*
								* Add quad
								*/
								x[u] = i;
								x[v] = j;

								du[0] = 0;
								du[1] = 0;
								du[2] = 0;
								du[u] = w;

								dv[0] = 0;
								dv[1] = 0;
								dv[2] = 0;
								dv[v] = h;

								/*
								* And here we call the quad function in order to render a merged quad in the scene.
								*
								* We pass mask[n] to the function, which is an instance of the Block::Face class containing
								* all the attributes of the face - which allows for variables to be passed to shaders - for
								* example lighting values used to create ambient occlusion.
								*/
								quad(math::float3(x[0], x[1], x[2]),
									math::float3(x[0] + du[0], x[1] + du[1], x[2] + du[2]),
									math::float3(x[0] + du[0] + dv[0], x[1] + du[1] + dv[1], x[2] + du[2] + dv[2]),
									math::float3(x[0] + dv[0], x[1] + dv[1], x[2] + dv[2]),
									w,
									h,
									mask[n],
									backFace);
							}

							/*
							* We zero out the mask
							*/
							for (l = 0; l < h; ++l)
							{
								for (k = 0; k < w; ++k) { mask[n + k + l * CHUNK_WIDTH] = nullptr; }
							}

							/*
							* And then finally increment the counters and continue
							*/
							i += w;
							n += w;

						}
						else
						{

							i++;
							n++;
						}
					}
				}
			}
		}
	}

	// Process quads
	GL_TRY(glGenVertexArrays(1, &members.m_vertexArray));
	GL_TRY(glBindVertexArray(members.m_vertexArray));
	GL_TRY(glGenBuffers(1, &members.m_vertexBuffer));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, members.m_vertexBuffer));
	GL_TRY(glBufferData(GL_ARRAY_BUFFER, triangles.Size() * sizeof(Triangle), &triangles[0], GL_STATIC_DRAW));
	GL_TRY(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
	GL_TRY(glEnableVertexAttribArray(0));

	// Index buffer
	mj::Vector<int32> indices(triangles.Size());
	for (int32 i = 0; i < triangles.Size(); i++)
	{
		indices[i] = i;
	}
	GL_TRY(glGenBuffers(1, &members.m_indexBuffer));
	GL_TRY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, members.m_indexBuffer));
	GL_TRY(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.Size() * sizeof(int32), &indices[0], GL_STATIC_DRAW));

	members.m_numTris = triangles.Size();

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
Block::Face *Chunk::getVoxelFace(int x, int y, int z, int side)
{

	Block::Face voxelFace = voxels[x][y][z];

	voxelFace.side = side;

	return &voxelFace;
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
void Chunk::quad(const mj::math::float3 &bottomLeft,
	const mj::math::float3 &topLeft,
	const mj::math::float3 &topRight,
	const mj::math::float3 &bottomRight,
	int width,
	int height,
	const Block::Face &voxel,
	bool backFace)
{

	mj::math::float3 vertices[4];

	vertices[2] = topLeft * VOXEL_SIZE;
	vertices[3] = topRight * VOXEL_SIZE;
	vertices[0] = bottomLeft * VOXEL_SIZE;
	vertices[1] = bottomRight * VOXEL_SIZE;

	int indexes[6];
	{
		if (backFace)
		{
			int a[6] = { 2, 0, 1, 1, 3, 2 };
			memcpy(indexes, a, sizeof(indexes));
		} else {
			int a[6] = { 2, 3, 1, 1, 0, 2 };
			memcpy(indexes, a, sizeof(indexes));
		}
	}

	float colorArray[4 * 4];

	for (int i = 0; i < 4 * 4; i += 4)
	{

		/*
		* Here I set different colors for quads depending on the "type" attribute, just
		* so that the different groups of voxels can be clearly seen.
		*
		*/
		if (voxel.type == 1)
		{
			colorArray[i] = 1.0f;
			colorArray[i + 1] = 0.0f;
			colorArray[i + 2] = 0.0f;
			colorArray[i + 3] = 1.0f;

		}
		else if (voxel.type == 2)
		{
			colorArray[i] = 0.0f;
			colorArray[i + 1] = 1.0f;
			colorArray[i + 2] = 0.0f;
			colorArray[i + 3] = 1.0f;

		}
		else
		{
			colorArray[i] = 0.0f;
			colorArray[i + 1] = 0.0f;
			colorArray[i + 2] = 1.0f;
			colorArray[i + 3] = 1.0f;
		}
	}

	mesh.setBuffer(Type.Position, 3, createFloatBuffer(vertices));
	mesh.setBuffer(Type.Color, 4, colorArray);
	mesh.setBuffer(Type.Index, 3, createIntBuffer(indexes));
}

void mj::Chunk::Render()
{
	GL_TRY( glBindVertexArray( members.m_vertexArray ) );
	GL_TRY( glDrawArrays( GL_TRIANGLES, 0, 3 * members.m_numTris ) );
	GL_TRY( glBindVertexArray( 0 ) );
}

#if 0
mj::Chunk& mj::Chunk::operator=( const mj::Chunk &other )
{
	if ( this != &other )
	{
		Destroy();
		this->m_position = other.m_position;
		if ( other.m_blocks )
		{
			m_blocks = new Block **[CHUNK_WIDTH];
			for ( int32 x = 0; x < CHUNK_WIDTH; x++ )
			{
				m_blocks[x] = new Block*[CHUNK_HEIGHT];
				for ( int32 y = 0; y < CHUNK_HEIGHT; y++ )
				{
					m_blocks[x][y] = new Block[CHUNK_DEPTH];
					memcpy( m_blocks[x][y], other.m_blocks[x][y], CHUNK_DEPTH * sizeof( Block ) );
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
		this->members = other.members;
		this->m_blocks = other.m_blocks;

		other.m_blocks = nullptr;
		other.members.m_vertexBuffer = 0;
		other.members.m_vertexArray = 0;
		other.members.m_indexBuffer = 0;
	}
	return *this;
}

mj::Block &mj::Chunk::GetBlock(const mj::math::int3 &idx)
{
	assert(m_blocks); return m_blocks[idx.x][idx.y][idx.z];
}
