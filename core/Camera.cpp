#include "CorePCH.h"
#include "Camera.h"
#include "RenderTexture.h"
#include "Application.h"

mj::gl::Camera::Camera()
{
	m_projectionMatrix = mj::math::Perspective( m_fieldOfView, (float) mj::Application::GetWidth() / mj::Application::GetHeight(), m_zNear, m_zFar );
}

mj::gl::Camera::~Camera()
{
}

void mj::gl::Camera::Bind()
{
	if ( m_renderTexture )
	{
		// Render to texture
		m_renderTexture->Bind();
	}
	else
	{
		// Render to screen
		GL_TRY( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
		GL_TRY( glViewport( 0, 0, mj::Application::GetWidth(), mj::Application::GetHeight() ) );
	}

	// TODO: Check clear flags
	switch ( m_clearFlags )
	{
	default:
		GL_TRY( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
		break;
	}
}
