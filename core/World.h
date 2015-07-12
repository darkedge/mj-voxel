#pragma once
#include "Chunk.h"

namespace mj {
	namespace gl {
		class Camera;
		class Program;
	}
	class Chunk;
	class Transform;
	class World
	{
	public:
		World();
		~World();

		void Tick();
	private:
		gl::Camera *m_camera = nullptr;
		gl::Program *m_program = nullptr;
		Transform *m_player = nullptr;

		Vector<Chunk> m_chunks;
		//std::vector<Chunk> m_chunks;
	};
}
