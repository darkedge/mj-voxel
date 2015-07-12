#pragma once

namespace mj {
	class Block;

	// TODO: Serializable
	class Chunk
	{
	public:
		Chunk() = default;
		Chunk( const math::int3 &position );

		Chunk( const Chunk &other );				// Copy constructor
		Chunk( Chunk &&other );						// Move constructor
		~Chunk();									// Destructor

		void CreateMesh();
		void Render();

		static const int32 CHUNK_SIZE = 16;
		math::int3 m_position;

		Chunk& operator=( const Chunk &other );		// Copy assignment operator
		Chunk& operator=( Chunk &&other );			// Move assignment operator

	private:
		void Destroy();
		void Init();

		Block ***m_blocks = nullptr;
	};
}
