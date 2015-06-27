#pragma once

namespace mj {
namespace gl {

struct RenderTextureDescriptor;
class RenderTexture {
public:
	enum class WrapMode {
		Clamp,
		Repeat,
	};
	enum class FilterMode {
		Point,
		Bilinear,
		Trilinear,
	};
	enum class AntiAliasing {
		None,
		AA_2x,
		AA_4x,
		AA_8x,
	};

public:
	RenderTexture() = delete;
	RenderTexture( const RenderTextureDescriptor &descriptor );
	~RenderTexture();

#if 0
	int32 GetWidth() const { return m_Width; }
	int32 GetHeight() const { return m_Height; }
	WrapMode GetWrapMode() const { return m_WrapMode; }
	FilterMode GetFilterMode() const { return m_FilterMode; }
	GLenum GetColorFormat() const { return m_ColorFormat; }
#endif
	void Bind();

private:
	int32 m_Width;
	int32 m_Height;
	AntiAliasing m_antiAliasing;
	GLenum m_ColorFormat;
	bool m_DepthBuffer;
	WrapMode m_WrapMode;
	FilterMode m_FilterMode;
	int32 m_AnisoLevel;

	GLuint m_ColorTexture;
	GLuint m_DepthTexture;
	GLuint m_FrameBuffer;
};

struct RenderTextureDescriptor {
	int32 width = 256;
	int32 height = 256;
	RenderTexture::AntiAliasing antiAliasing = RenderTexture::AntiAliasing::None;
	GLenum colorFormat = GL_RGBA;
	bool depthBuffer = true;
	RenderTexture::WrapMode wrapMode = RenderTexture::WrapMode::Clamp;
	RenderTexture::FilterMode filterMode = RenderTexture::FilterMode::Bilinear;
	int32 anisoLevel = 0;
};
}
}
