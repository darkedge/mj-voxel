#pragma once

namespace mj {
namespace gl {
class Texture
{
public:
	Texture() = delete;
	~Texture();

	GLuint GetHandle() const { return handle; }

	static Texture *Random( int width, int height );
	static Texture *Random( int width );
	static Texture *Texture1D( const char *path );
	static Texture *Texture2D( const char *path, bool repeat );

private:
	Texture( GLuint handle );

	struct BinReader
	{
		BinReader( FILE *file );
		~BinReader();
		int w, h, n;
		unsigned char *data = nullptr;
		void* glPtr = nullptr;
		GLenum format;
	};

	struct Image
	{
		Image( const char *path );
		~Image();
		int w, h, n;
		unsigned char *data = nullptr;
		GLenum format;

		void WriteBinary( const char *str );
	};

	GLuint handle;
};
}
}
