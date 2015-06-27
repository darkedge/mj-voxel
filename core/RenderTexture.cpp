#include "CorePCH.h"
#include "RenderTexture.h"

mj::gl::RenderTexture::RenderTexture( const RenderTextureDescriptor &desc ) :
	m_Width( desc.width ),
	m_Height( desc.height ),
	m_antiAliasing( desc.antiAliasing ),
	m_ColorFormat( desc.colorFormat ),
	m_DepthBuffer( desc.depthBuffer ),
	m_WrapMode( desc.wrapMode ),
	m_FilterMode( desc.filterMode ),
	m_AnisoLevel( desc.anisoLevel ) {
	glGenFramebuffers( 1, &m_FrameBuffer );
	glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBuffer );
	{
		glGenTextures( 1, &m_ColorTexture );
		glBindTexture( GL_TEXTURE_2D, m_ColorTexture );

		glTexImage2D(
			GL_TEXTURE_2D,		// target
			0,					// level
			m_ColorFormat,		// internal format
			m_Width,			// width
			m_Height,			// height
			0,					// border
			GL_RGBA,			// pixel data format, irrelevant
			GL_FLOAT,			// pixel data type, irrelevant
			nullptr );			// pixels

		// Poor filtering
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		glGenerateMipmap( GL_TEXTURE_2D );

		// Attach texture to frame buffer
		glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ColorTexture, 0 );
	}

	if ( !m_DepthBuffer ) {
		m_DepthTexture = 0;
	} else {
		glGenTextures( 1, &m_DepthTexture );
		glBindTexture( GL_TEXTURE_2D, m_DepthTexture );

		glTexImage2D(
			GL_TEXTURE_2D,		// target
			0,					// level
			GL_DEPTH_COMPONENT,	// internal format
			m_Width,			// width
			m_Height,			// height
			0,					// border
			GL_RGBA,			// pixel data format, irrelevant
			GL_FLOAT,			// pixel data type, irrelevant
			nullptr );			// pixels

		// Poor filtering
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		glGenerateMipmap( GL_TEXTURE_2D );

		// Attach texture to frame buffer
		glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTexture, 0 );
	}

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers( 1, DrawBuffers );

	assert( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE );
}

mj::gl::RenderTexture::~RenderTexture() {
	if ( m_ColorTexture ) {
		glDeleteTextures( 1, &m_ColorTexture );
	}
	if ( m_DepthTexture ) {
		glDeleteTextures( 1, &m_DepthTexture );
	}
	if ( m_FrameBuffer ) {
		glDeleteFramebuffers( 1, &m_FrameBuffer );
	}
}

void mj::gl::RenderTexture::Bind() {
	glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBuffer );
	glViewport( 0, 0, m_Width, m_Height );
}
