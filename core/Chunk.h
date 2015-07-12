#pragma once

namespace mj {
	class Block;

	// TODO: Serializable
	class Chunk
	{
	public:
		Chunk();
		Chunk( const math::int3 &position );

		Chunk( const Chunk &other ) = delete;				// Copy constructor
		Chunk( Chunk &&other );						// Move constructor
		~Chunk();									// Destructor

		void CreateMesh();
		void Render();

		static const int32 CHUNK_WIDTH = 16;
		static const int32 CHUNK_HEIGHT = 16;
		static const int32 CHUNK_DEPTH = 16;

		Chunk& operator=( const Chunk &other ) = delete;		// Copy assignment operator
		Chunk& operator=( Chunk &&other );			// Move assignment operator

		Block &GetBlock( const mj::math::int3 &idx );

	private:
		void Destroy();
		void Init();

		Block ***m_blocks = nullptr;
		struct 
		{
			math::int3 m_position;
			GLuint m_vertexBuffer;
			GLuint m_vertexArray;
			GLuint m_indexBuffer;
			int32 m_numTris;
		} members;
	};
}
