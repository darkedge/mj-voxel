#include "CorePCH.h"
#include "Camera.h"
#include "RenderTexture.h"
#include "Application.h"

mj::gl::Camera::Camera()
{
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
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		glViewport( 0, 0, mj::Application::GetWidth(), mj::Application::GetHeight() );
	}

	// TODO: Check clear flags
	switch ( m_clearFlags )
	{
	default:
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		break;
	}
}
