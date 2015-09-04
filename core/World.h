#pragma once

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
		void MovePlayer();

		gl::Camera *m_camera = nullptr;
		gl::Program *m_program = nullptr;
		Transform *m_player = nullptr;

		Vector<Chunk> m_chunks;
	};
}
