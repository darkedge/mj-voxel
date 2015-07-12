#include "CorePCH.h"
#include "World.h"
#include "Transform.h"
#include "Camera.h"
#include "Program.h"

mj::World::World()
{
	m_player = new mj::Transform();
	m_camera = new mj::gl::Camera();
	m_program = new mj::gl::Program( "Voxels.vert", "Voxels.frag" );

	// Create world
	for ( int x = 0; x < 1; x++ )
	{
		for ( int z = 0; z < 1; z++ )
		{
			m_chunks.Add( Chunk( { x, 0, z } ) );
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

	m_program->Bind();

	math::mat4 mat = /* chunk location  * */ m_player->GetLocalToWorldMatrix() * m_camera->GetProjectionMatrix();

	m_program->SetUniform( "mvp_matrix", mat );

	// Draw voxels
	for ( int32 i = 0; i < m_chunks.Size(); i++ )
	{
		m_chunks[i].Render();
	}
}
