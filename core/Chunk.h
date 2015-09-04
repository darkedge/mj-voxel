#pragma once
#include "Block.h"
#include "Color.h"

namespace mj {
	class Block;

	// TODO: Serializable
	class Chunk
	{
	public:
		Chunk();
		Chunk( const math::float3 &position );

		Chunk( const Chunk &other ) = delete;		// Copy constructor
		Chunk( Chunk &&other );						// Move constructor
		~Chunk();									// Destructor

		void CreateMesh();
		void Render();

		static const int32 CHUNK_WIDTH = 16;
		static const int32 CHUNK_HEIGHT = 16;
		static const int32 CHUNK_DEPTH = 16;

		Chunk& operator=( const Chunk &other ) = delete;	// Copy assignment operator
		Chunk& operator=( Chunk &&other );					// Move assignment operator

		Block &GetBlock( const math::int3 &idx );
		math::float3 GetPosition() const { return m_position; }

	private:
		struct Mesh {
			Vector<math::float3> m_positions;
			Vector<math::float4> m_colors;
			Vector<math::float2> m_texcoords;
			Vector<int32> m_indices;
		};
		void Destroy();
		void Init();
		void quad(
			Mesh &mesh,
			const math::float3 &bottomLeft,
			const math::float3 &topLeft,
			const math::float3 &topRight,
			const math::float3 &bottomRight,
			int width,
			int height,
			const Block &voxel,
			bool backFace);
		Block *GetVoxelFace(math::int3 xyz, Block::ESide side);

		Block ***m_blocks = nullptr;
		
		math::float3 m_position;
		GLuint m_vertexBuffer;
		GLuint m_colorBuffer;
		GLuint m_texCoordBuffer;
		GLuint m_vertexArray;
		GLuint m_indexBuffer;
		int32 m_numTris;
	};
}
