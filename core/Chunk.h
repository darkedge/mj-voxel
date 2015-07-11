#pragma once

namespace mj {
	class Block;

	// TODO: Serializable
	// This class contains dynamic memory
	// SO APPLY RULE OF 3/5
	class Chunk
	{
	public:
		Chunk();
		Chunk( const Chunk &copy )
		{
			*this = copy;
		}
		Chunk( const math::int3 &position );
		~Chunk();

		void CreateMesh();
		void Render();

		static const int32 CHUNK_SIZE = 16;
		math::int3 m_position;

		Chunk& operator=(const Chunk &other);

	private:
		void Init();

		Block ***m_blocks = nullptr;
	};
}
