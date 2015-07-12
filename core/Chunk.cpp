#include "CorePCH.h"
#include "Chunk.h"
#include "Block.h"

mj::Chunk::Chunk( const math::int3 &position ) :
m_position( position )
{
	Init();
}
#if 0
mj::Chunk::Chunk( const Chunk &other )
{
	*this = other;
}
#endif

mj::Chunk::Chunk( Chunk &&other )
{
	this->m_position = other.m_position;
	this->m_blocks = other.m_blocks;
	other.m_blocks = nullptr;
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
	glDeleteBuffers( 1, &members.m_vertexArray );
	glDeleteBuffers( 1, &members.m_indexBuffer );
	glDeleteVertexArrays( 1, &members.m_vertexArray );
}

void mj::Chunk::CreateMesh()
{
	struct Quad
	{
		mj::math::int3 a, b, c, d;
	};

	mj::Vector<Quad> quads;

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
						math::int3 du = { 0, 0, 0 }; du[u] = w;
						math::int3 dv = { 0, 0, 0 }; dv[v] = h;
						quads.Add( { x, x + du, x + du + dv, x + dv } );

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

	// Process quads
	glGenVertexArrays( 1, &members.m_vertexArray );
	glBindVertexArray( members.m_vertexArray );
	glGenBuffers( 1, &members.m_vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, members.m_vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, quads.Size() * sizeof( Quad ), &quads[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_INT, GL_FALSE, 0, nullptr );
	glEnableVertexAttribArray( 0 );

	// Index buffer
	mj::Vector<int32> indices(quads.Size());
	for ( int32 i = 0; i < quads.Size(); i++ )
	{
		indices[i] = i;
	}
	glGenBuffers( 1, &members.m_indexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, members.m_indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.Size() * sizeof( int32 ), &indices[0], GL_STATIC_DRAW );

	members.m_numQuads = quads.Size();

	// Unbind
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void mj::Chunk::Render()
{
	glBindVertexArray( members.m_vertexArray );
	glDrawArrays( GL_QUADS, 0, members.m_numQuads );
	glBindVertexArray( 0 );
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
	}
	return *this;
}

mj::Block &mj::Chunk::GetBlock( const mj::math::int3 &idx )
{
	assert( m_blocks ); return m_blocks[idx.x][idx.y][idx.z];
}
