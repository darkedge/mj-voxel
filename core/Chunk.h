#pragma once
#include "Block.h"

namespace mj {
	class Block;

	// TODO: Serializable
	class Chunk
	{
	public:
		Chunk();
		Chunk( const math::float3 &position );

		Chunk( const Chunk &other ) = delete;				// Copy constructor
		Chunk( Chunk &&other );						// Move constructor
		~Chunk();									// Destructor

		void CreateMesh();
		void Render();

		static const int32 CHUNK_WIDTH = 16;
		static const int32 CHUNK_HEIGHT = 16;
		static const int32 CHUNK_DEPTH = 16;

		static const int32 SOUTH      = 0;
		static const int32 NORTH      = 1;
		static const int32 EAST       = 2;
		static const int32 WEST       = 3;
		static const int32 TOP        = 4;
		static const int32 BOTTOM     = 5;

		Chunk& operator=( const Chunk &other ) = delete;		// Copy assignment operator
		Chunk& operator=( Chunk &&other );			// Move assignment operator

		Block &GetBlock( const mj::math::int3 &idx );
		math::float3 GetPosition() const { return members.m_position; }

	private:
		void Destroy();
		void Init();
		void quad(const mj::math::float3 &bottomLeft,
			const mj::math::float3 &topLeft,
			const mj::math::float3 &topRight,
			const mj::math::float3 &bottomRight,
			int width,
			int height,
			const Block::Face &voxel,
			bool backFace);
		Block::Face *getVoxelFace(int x, int y, int z, int side);

		Block ***m_blocks = nullptr;
		struct 
		{
			math::float3 m_position;
			GLuint m_vertexBuffer;
			GLuint m_vertexArray;
			GLuint m_indexBuffer;
			int32 m_numTris;
		} members;
	};
}
