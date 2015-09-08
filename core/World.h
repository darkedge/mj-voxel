#pragma once

namespace mj {
namespace gl {
class Camera;
class Program;
class Texture;
}

class Chunk;
class Block;
class Transform;

struct BlockQuery
{
	Block *block;
	math::int3 position;
};

class World
{
public:
	World();
	~World();

	void Tick();

private:
	void MovePlayer();
	bool PickBlock(BlockQuery &query);
	void DrawBlockHighlight(const math::int3 &position);

	gl::Camera *m_camera = nullptr;
	gl::Program *m_program = nullptr;
	Transform *m_player = nullptr;
	gl::Texture *m_texture = nullptr;

	Vector<Chunk> m_chunks;
};
}
