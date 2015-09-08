#include "CorePCH.h"
#include "World.h"
#include "Chunk.h"
#include "Transform.h"
#include "Camera.h"
#include "Program.h"
#include "Input.h"
#include "Application.h"

mj::World::World()
{
	m_player = new mj::Transform();
	m_player->SetPosition(0, 0, 32);
	m_player->SetRotation(math::quat(0, 0, 0, 1));

	m_camera = new mj::gl::Camera();
	m_program = new mj::gl::Program("Voxels.vert", "Voxels.frag");

	m_texture = gl::Texture::Texture2D("uv.png", true);

	// Create world
	for (int x = 0; x < 1; x++)
	{
		for (int z = 0; z < 1; z++)
		{
			m_chunks.Add(Chunk({ (float) x, 0, (float) z }));
		}
	}

	// 	m_chunks.Add( Chunk( { 0.0f, 0.0f, 0.0f } ) );
	// 	m_chunks.Add( Chunk( { 5.0f, 0.0f, 0.0f } ) );
	// 	m_chunks.Add( Chunk( { 0.0f, 5.0f, 0.0f } ) );
	// 	m_chunks.Add( Chunk( { 0.0f, 0.0f, 5.0f } ) );
}

mj::World::~World()
{

}

void mj::World::MovePlayer()
{
	static math::float2 lastRotation;
	static math::float2 currentRotation;

	if (mj::Input::GetKeyDown(KeyCode::M))
	{
		mj::Input::SetMouseGrabbed(!mj::Input::IsMouseGrabbed());
	}

	// Reset
	if (mj::Input::GetKeyDown(KeyCode::R))
	{
		currentRotation = lastRotation = { 0, 0 };
		m_player->SetPosition(math::float3(0, 0, 0));
		m_player->SetRotation(math::quat(0, 0, 0, 1));
	}

	if (mj::Input::IsMouseGrabbed())
	{
		// Rotation
		const float ROT_SPEED = 0.0025f;
		currentRotation -= ROT_SPEED * mj::Input::GetMouseDelta();
		if (currentRotation.y < math::Deg2Rad(-89.0f))
		{
			currentRotation.y = math::Deg2Rad(-89.0f);
		}
		if (currentRotation.y > math::Deg2Rad(89.0f))
		{
			currentRotation.y = math::Deg2Rad(89.0f);
		}
		if (currentRotation.x != lastRotation.x || currentRotation.y != lastRotation.y)
		{
			m_player->SetRotation(math::quat(math::float3(currentRotation.y, currentRotation.x, 0)));
			lastRotation = currentRotation;
		}
	}

	// Translation
	const float SPEED = 20.0f;
	math::float3 translation(0, 0, 0);
	math::quat orientation = m_player->GetRotation();
	if (Input::GetKey(KeyCode::W))		translation += m_player->Forward();
	if (Input::GetKey(KeyCode::A))		translation -= m_player->Right();
	if (Input::GetKey(KeyCode::S))		translation -= m_player->Forward();
	if (Input::GetKey(KeyCode::D))		translation += m_player->Right();
	if (Input::GetKey(KeyCode::LEFT_CONTROL) || Input::GetKey(KeyCode::C) || Input::GetKey(KeyCode::LEFT_SHIFT)) translation -= math::float3(0, 1, 0);
	if (Input::GetKey(KeyCode::SPACE)) translation += math::float3(0, 1, 0);
	if (translation != math::float3(0, 0, 0))
	{
		math::float3 pos = m_player->GetPosition();
		pos += math::Normalize(translation) * SPEED * mj::Application::GetDeltaTime();
		m_player->SetPosition(pos);
		//printf("pos: %.1f, %.1f, %.1f\n", m_player->GetPosition().x, m_player->GetPosition().y, m_player->GetPosition().z);
	}
}

void mj::World::Tick()
{
	// Update player position
	MovePlayer();
	math::mat4 translate = math::TranslationMatrix(-m_player->GetPosition());
	math::mat4 rotate = math::Transpose(math::mat4(m_player->GetRotation()));
	math::mat4 view = rotate * translate;

	// Bind camera
	m_camera->Bind();
	m_program->Bind();
	m_program->SetUniform("uMatAlbedo", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture->GetHandle());


	// Draw voxels
	math::mat4 projection = m_camera->GetProjectionMatrix();
	for (int32 i = 0; i < m_chunks.Size(); i++)
	{
		math::mat4 model = math::TranslationMatrix(m_chunks[i].GetPosition());
		math::mat4 mvp = projection * view * model;

		m_program->SetUniform("mvp_matrix", mvp);

		m_chunks[i].Render();
	}

	// Pick voxel
	BlockQuery query;
	if (PickBlock(query))
	{
		DrawBlockHighlight(query.position);
	}
}

void mj::World::DrawBlockHighlight(const math::int3 &position)
{

}

bool mj::World::PickBlock(mj::BlockQuery &query)
{
	query.block = nullptr;

	math::float3 origin = m_player->GetPosition();
	math::float3 direction = m_player->Forward();

	query.position.x = (int32)floor(origin.x);
	query.position.y = (int32)floor(origin.y);
	query.position.z = (int32)floor(origin.z);

	// = (-1/1) signs of vector dir
	int32 stepX = (direction.x < 0) ? -1 : 1;
	int32 stepY = (direction.y < 0) ? -1 : 1;
	int32 stepZ = (direction.z < 0) ? -1 : 1;

	auto IntBound = [](float s, float ds) {
		return (ds > 0 ? ceil(s) - s : s - floor(s)) / abs(ds);
	};

	float tMaxX = IntBound(origin.x, direction.x);
	float tMaxY = IntBound(origin.y, direction.y);
	float tMaxZ = IntBound(origin.z, direction.z);

	float tDeltaX = (float) stepX / direction.x; // length of v between two YZ-boundaries
	float tDeltaY = (float) stepY / direction.y; // length of v between two XZ-boundaries
	float tDeltaZ = (float) stepZ / direction.z; // length of v between two XY-boundaries

	float radius = 10.0f;

	do
	{
		if (tMaxX < tMaxY)
		{
			if (tMaxX < tMaxZ)
			{
				if (tMaxX > radius) break;
				query.position.x += stepX;
				tMaxX += tDeltaX;
			}
			else  
			{
				if (tMaxZ > radius) break;
				query.position.z += stepZ;
				tMaxZ += tDeltaZ;
			}
		}
		else
		{
			if (tMaxY < tMaxZ)
			{
				if (tMaxY > radius) break;
				query.position.y += stepY;
				tMaxY += tDeltaY;
			}
			else
			{
				if (tMaxZ > radius) break;
				query.position.z += stepZ;
				tMaxZ += tDeltaZ;
			}
		}
		query.block = m_chunks[0].GetBlock(query.position.x, query.position.y, query.position.z);
	} while (!query.block);
	//if(blockPtr) printf("FOUND BLOCK: %d %d %d\n", X, Y, Z);
	
	return query.block != nullptr;
}
