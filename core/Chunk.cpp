#include "CorePCH.h"
#include "Chunk.h"
#include "Block.h"

mj::Chunk::Chunk()
{
	//members.m_position = 0;
	members.m_vertexBuffer = 0;
	members.m_vertexArray = 0;
	members.m_indexBuffer = 0;
	//members.m_numTris = 0;
}

mj::Chunk::Chunk( const math::float3 &position )
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
mj::Chunk::Chunk( Chunk &&other )
{
	*this = std::forward<Chunk>( other );
}

void mj::Chunk::Init()
{
	// Create blocks
	m_blocks = new Block **[CHUNK_WIDTH];
	for ( int32 x = 0; x < CHUNK_WIDTH; x++ )
	{
		m_blocks[x] = new Block*[CHUNK_HEIGHT];
		for ( int32 y = 0; y < CHUNK_HEIGHT; y++ )
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
	if ( m_blocks )
	{
		for ( int32 x = 0; x < CHUNK_WIDTH; x++ ) if ( m_blocks[x] )
		{
			for ( int32 y = 0; y < CHUNK_HEIGHT; y++ )
			{
				delete[] m_blocks[x][y];
			}
			delete[] m_blocks[x];
		}
	}
	delete[] m_blocks;

	// Delete GL buffer (TODO: RAII buffer class?)
	GL_TRY( glDeleteBuffers( 1, &members.m_vertexBuffer ) );
	members.m_vertexBuffer = 0;
	GL_TRY( glDeleteBuffers( 1, &members.m_indexBuffer ) );
	members.m_indexBuffer = 0;
	GL_TRY( glDeleteVertexArrays( 1, &members.m_vertexArray ) );
	members.m_vertexArray = 0;
}

void mj::Chunk::CreateMesh()
{
	struct Triangle
	{
		mj::math::float3 a, b, c;
	};

	mj::Vector<Triangle> triangles;

	for ( int32 d = 0; d < 3; d++ )
	{
		int32 w, h;
		int32 u = ( d + 1 ) % 3;
		int32 v = ( d + 2 ) % 3;
		mj::math::int3 x = { 0, 0, 0 };
		mj::math::int3 q = { 0, 0, 0 };

		bool mask[CHUNK_WIDTH * CHUNK_HEIGHT];

		q[d] = 1;
		for ( x[d] = -1; x[d] < CHUNK_WIDTH; )
		{
			int32 n = 0;
			for ( x[v] = 0; x[v] < CHUNK_HEIGHT; ++x[v] )
			{
				for ( x[u] = 0; x[u] < CHUNK_DEPTH; ++x[u] )
				{

					Block *aap = ( x[d] >= 0 ) ? &GetBlock( x ) : nullptr;
					Block *beer = ( x[d] < CHUNK_WIDTH - 1 ) ? &GetBlock( x + q ) : nullptr;
					bool cola = aap != beer;
					mask[n++] = cola;
				}
			}
			x[d]++;
			n = 0;
			for ( int32 j = 0; j < CHUNK_HEIGHT; ++j )
			{
				for ( int32 i = 0; i < CHUNK_DEPTH; )
				{
					if ( mask[n] )
					{
						//Compute width
						w = 1;
						while ( mask[n + w] && i + w < CHUNK_DEPTH )
						{
							w++;
						}

						//Compute height (this is slightly awkward)
						bool done = false;
						for ( h = 1; j + h < CHUNK_HEIGHT; ++h )
						{
							for ( int32 k = 0; k < w; ++k )
							{
								if ( !mask[n + k + h * CHUNK_DEPTH] )
								{
									done = true;
									break;
								}
							}
							if ( done )
							{
								break;
							}
						}

						x[u] = i;  x[v] = j;
						math::float3 du = { 0, 0, 0 }; du[u] = w;
						math::float3 dv = { 0, 0, 0 }; dv[v] = h;
						triangles.Add( Triangle{ ( math::float3 )x, ( math::float3 )x + du, ( math::float3 )x + du + dv } );
						triangles.Add( { ( math::float3 )x + du + dv, ( math::float3 )x + dv, x } );

						for ( int32 l = 0; l < h; l++ )
						{
							for ( int32 k = 0; k < w; k++ )
							{
								mask[n + k + l * CHUNK_WIDTH] = false;
							}
						}

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

#if 0
	math::float3 v[] = {
		{ 0, 0, 0 },
		{ 0, 0, 1 },
		{ 0, 1, 0 },
		{ 0, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 0, 1 },
		{ 1, 1, 0 },
		{ 1, 1, 1 },
	};

	triangles.Clear();

	// Front Right Back Left Up Down
	triangles.Add( {
		{ v[1], v[5], v[7] },
		{ v[7], v[3], v[1] },
		{ v[5], v[4], v[6] },
		{ v[6], v[7], v[5] },
		{ v[4], v[0], v[2] },
		{ v[2], v[6], v[4] },
		{ v[0], v[1], v[3] },
		{ v[3], v[2], v[0] },
		{ v[3], v[7], v[6] },
		{ v[6], v[2], v[3] },
		{ v[0], v[4], v[5] },
		{ v[5], v[1], v[0] },
	} );
#endif

	// Process quads
	GL_TRY( glGenVertexArrays( 1, &members.m_vertexArray ) );
	GL_TRY( glBindVertexArray( members.m_vertexArray ) );
	GL_TRY( glGenBuffers( 1, &members.m_vertexBuffer ) );
	GL_TRY( glBindBuffer( GL_ARRAY_BUFFER, members.m_vertexBuffer ) );
	GL_TRY( glBufferData( GL_ARRAY_BUFFER, triangles.Size() * sizeof( Triangle ), &triangles[0], GL_STATIC_DRAW ) );
	GL_TRY( glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr ) );
	GL_TRY( glEnableVertexAttribArray( 0 ) );

	// Index buffer
	mj::Vector<int32> indices( triangles.Size() );
	for ( int32 i = 0; i < triangles.Size(); i++ )
	{
		indices[i] = i;
	}
	GL_TRY( glGenBuffers( 1, &members.m_indexBuffer ) );
	GL_TRY( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, members.m_indexBuffer ) );
	GL_TRY( glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.Size() * sizeof( int32 ), &indices[0], GL_STATIC_DRAW ) );

	members.m_numTris = triangles.Size();

	// Unbind
	GL_TRY( glBindVertexArray( 0 ) );
	GL_TRY( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
	GL_TRY( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) );
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

mj::Chunk& mj::Chunk::operator=( mj::Chunk &&other )
{
	if ( this != &other )
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

mj::Block &mj::Chunk::GetBlock( const mj::math::int3 &idx )
{
	assert( m_blocks ); return m_blocks[idx.x][idx.y][idx.z];
}
