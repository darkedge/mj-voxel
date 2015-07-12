#pragma once

namespace mj {
	class Block;

	// TODO: Serializable
	class Chunk
	{
	public:
		Chunk() = default;
		Chunk( const math::int3 &position );

		Chunk( const Chunk &other ) = delete;				// Copy constructor
		Chunk( Chunk &&other );						// Move constructor
		~Chunk();									// Destructor

		void CreateMesh();
		void Render();

		static const int32 CHUNK_WIDTH = 16;
		static const int32 CHUNK_HEIGHT = 16;
		static const int32 CHUNK_DEPTH = 16;
		math::int3 m_position;

		Chunk& operator=( const Chunk &other ) = delete;		// Copy assignment operator
		Chunk& operator=( Chunk &&other );			// Move assignment operator

		Block &GetBlock( const mj::math::int3 &idx );

	private:
		void Destroy();
		void Init();

		Block ***m_blocks = nullptr;
		struct 
		{
			GLuint m_vertexBuffer = 0;
			GLuint m_vertexArray = 0;
			GLuint m_indexBuffer = 0;
			int32 m_numQuads = 0;
		} members;
	};
}
