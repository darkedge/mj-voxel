#include "CorePCH.h"
#include "World.h"
#include "Transform.h"
#include "Camera.h"

mj::World::World()
{
	m_player = new mj::Transform();
	m_camera = new mj::gl::Camera();

	// Create world
	for ( int x = 0; x < 10; x++ )
	{
		for ( int z = 0; z < 10; z++ )
		{
			Chunk &chunk = m_chunks.Add();
			chunk.m_position = { x, 0, z };
		}
	}
}

mj::World::~World()
{
}

void mj::World::Tick()
{
	// Update player position

	// Bind camera
	m_camera->Bind();

	// Draw voxels
	for ( int32 i = 0; i < m_chunks.Size(); i++ )
	{
		m_chunks[i].Render();
	}
}
