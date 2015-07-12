#include "CorePCH.h"
#include "Chunk.h"
#include "Block.h"

#if 0
mj::Chunk::Chunk() :
m_position( { 0, 0, 0 } )
{
	Init();
}
#endif

mj::Chunk::Chunk( const math::int3 &position ) :
m_position( position )
{
	Init();
}

mj::Chunk::Chunk( const Chunk &other )
{
	printf( "Cp() " );
	*this = other;
}

mj::Chunk::Chunk( Chunk &&other )
{
	printf( "Mv() \n" );
	this->m_position = other.m_position;
	this->m_blocks = other.m_blocks;
	other.m_blocks = nullptr;
}

void mj::Chunk::Init()
{
	printf( "C() " );
	// Create blocks
	m_blocks = new Block **[CHUNK_SIZE];
	for ( int32 x = 0; x < CHUNK_SIZE; x++ )
	{
		m_blocks[x] = new Block*[CHUNK_SIZE];
		for ( int32 y = 0; y < CHUNK_SIZE; y++ )
		{
			m_blocks[x][y] = new Block[CHUNK_SIZE];
		}
	}
}

mj::Chunk::~Chunk()
{
	Destroy();
}

void mj::Chunk::Destroy()
{
	if ( m_blocks )
	{
		printf( "Dx+ " );
	}
	else
	{
		printf( "Dx- " );
	}
	// Delete blocks
	if ( m_blocks )
	{
		for ( int32 x = 0; x < CHUNK_SIZE; x++ ) if ( m_blocks[x] )
		{
			for ( int32 y = 0; y < CHUNK_SIZE; y++ )
			{
				delete[] m_blocks[x][y];
			}
			delete[] m_blocks[x];
		}
	}
	delete[] m_blocks;
}

void mj::Chunk::CreateMesh()
{
	for ( int32 x = 0; x < CHUNK_SIZE; x++ )
	{
		for ( int32 y = 0; y < CHUNK_SIZE; y++ )
		{
			for ( int32 z = 0; z < CHUNK_SIZE; z++ )
			{
				if ( m_blocks[x][y][z].IsActive() )
				{
					//CreateCube(x, y, z); // ?
				}
			}
		}
	}
}

void mj::Chunk::Render()
{

}

mj::Chunk& mj::Chunk::operator=( const mj::Chunk &other )
{
	if ( this != &other )
	{
		printf( "Cp= " );
		this->m_position = other.m_position;
		m_blocks = new Block **[CHUNK_SIZE];
		for ( int32 x = 0; x < CHUNK_SIZE; x++ )
		{
			m_blocks[x] = new Block*[CHUNK_SIZE];
			for ( int32 y = 0; y < CHUNK_SIZE; y++ )
			{
				m_blocks[x][y] = new Block[CHUNK_SIZE];
				memcpy( m_blocks[x][y], other.m_blocks[x][y], CHUNK_SIZE * sizeof( Block ) );
			}
		}
	}

	return *this;
}

mj::Chunk& mj::Chunk::operator=( mj::Chunk &&other )
{
	if ( this != &other )
	{
		printf( "Mv= " );
		Destroy();
		this->m_position = other.m_position;
		this->m_blocks = other.m_blocks;
		other.m_blocks = nullptr;
	}
	return *this;
}
