#include "CorePCH.h"
#include "Chunk.h"
#include "Block.h"


mj::Chunk::Chunk() :
m_position( { 0, 0, 0 } )
{
	Init();
}

mj::Chunk::Chunk( const math::int3 &position ) :
m_position( position )
{
	Init();
}

void mj::Chunk::Init()
{
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
	// Delete blocks
	if ( m_blocks )
	{
		for ( int32 x = 0; x < CHUNK_SIZE; x++ ) if ( m_blocks[x] )
		{
			for ( int32 y = 0; y < CHUNK_SIZE; y++ ) if ( m_blocks[x][y] )
			{
				delete[] m_blocks[x][y];
			}
			delete[] m_blocks[x];
		}
		delete[] m_blocks;
	}
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

mj::Chunk& mj::Chunk::operator=(const mj::Chunk &other)
{
	if ( this != &other )
	{
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
