#include "Common.h"
#include "CoreGL.h"
#include "Texture.h"
#include "Random.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/************************************************************************/
/* TEXTURE::IMAGE                                                       */
/************************************************************************/
mj::gl::Texture::Image::Image( const char *path )
{
	this->data = stbi_load( path, &w, &h, &n, 0 );
	assert( this->data );
	if ( n == 3 )
	{
		format = GL_RGB;
	}
	else if ( n == 4 )
	{
		format = GL_RGBA;
	}
}

mj::gl::Texture::Image::~Image()
{
	stbi_image_free( data );
}

void mj::gl::Texture::Image::WriteBinary( const char *str )
{
#pragma warning(suppress: 4996)
	FILE *dfile = fopen( str, "wb" );
	fwrite( &w, sizeof( w ), 1, dfile );
	fwrite( &h, sizeof( h ), 1, dfile );
	fwrite( &n, sizeof( n ), 1, dfile );
	fwrite( data, w * h * n, 1, dfile );
	fclose( dfile );
}



/************************************************************************/
/* TEXTURE::BINREADER                                                   */
/************************************************************************/
mj::gl::Texture::BinReader::BinReader( FILE *bfile )
{
	// Load bin file
	// obtain file size:
	fseek( bfile, 0, SEEK_END );
	auto lSize = ftell( bfile );
	rewind( bfile );
	data = new unsigned char[lSize];
	fread( data, lSize, 1, bfile );

	w = ( (int32*) ( data ) )[0];
	h = ( (int32*) ( data ) )[1];
	n = ( (int32*) ( data ) )[2];
	glPtr = data + 3 * sizeof( int32 );
	format = n == 3 ? GL_RGB : GL_RGBA;
}

mj::gl::Texture::BinReader::~BinReader()
{
	delete data;
}

/************************************************************************/
/* TEXTURE                                                              */
/************************************************************************/
mj::gl::Texture *mj::gl::Texture::Texture2D( const char *path, bool repeat )
{
	GLuint handle;
	glGenTextures( 1, &handle );
	glBindTexture( GL_TEXTURE_2D, handle );

	if(repeat) {
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	} else {
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	}
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	if(repeat)
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	}
	else
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	}

	// Check if bin file exists
	char binstr[256];
	sprintf(binstr, "%s.bin", path);
	FILE *bfile = fopen( binstr, "rb" );

	if ( bfile )
	{
		BinReader reader( bfile );
		glTexImage2D( GL_TEXTURE_2D, 0, reader.format, reader.w, reader.h, 0, reader.format, GL_UNSIGNED_BYTE, reader.glPtr );
	}
	else
	{
		// Read image from file
		Image image( path );
		glTexImage2D( GL_TEXTURE_2D, 0, image.format, image.w, image.h, 0, image.format, GL_UNSIGNED_BYTE, image.data );
		// Write binary file
		image.WriteBinary( binstr );
	}

	// Generate mipmaps of active texture unit
	if(repeat)
	glGenerateMipmap( GL_TEXTURE_2D );

	return new Texture( handle );
}

mj::gl::Texture *mj::gl::Texture::Texture1D( const char *path )
{
	GLuint handle;
	glGenTextures( 1, &handle );
	glBindTexture( GL_TEXTURE_1D, handle );

	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	// Check if bin file exists
	char binstr[256];
	sprintf(binstr, "%s.bin", path);
#pragma warning(suppress: 4996)
	FILE *bfile = fopen( binstr, "rb" );

	if ( bfile )
	{
		BinReader reader( bfile );
		glTexImage1D( GL_TEXTURE_1D, 0, reader.format, reader.w, 0, reader.format, GL_UNSIGNED_BYTE, reader.glPtr );
	}
	else
	{
		// Read image from file
		Image image( path );
		glTexImage1D( GL_TEXTURE_1D, 0, image.format, image.w, 0, image.format, GL_UNSIGNED_BYTE, image.data );
		// Write binary file
		image.WriteBinary( binstr );
	}

	// Generate mipmaps of active texture unit
	glGenerateMipmap( GL_TEXTURE_1D );

	return new Texture( handle );
}

mj::gl::Texture::~Texture()
{
	glDeleteTextures( 1, &handle );
}

mj::gl::Texture::Texture( GLuint handle ) : handle( handle ) {}

mj::gl::Texture *mj::gl::Texture::Random( int32 width, int32 height )
{
	GLuint texture;
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );

	uint64 s[16];
	for ( int32 i = 0; i < 16; i++ )
	{
		s[i] = Random::Next<uint64>();
	}
	int32 p = 0;

	float *pixels = new float[width * height * 4];
	for ( int32 i = 0; i < width * height * 4; i += 2 )
	{
		uint64 s0 = s[p];
		uint64 s1 = s[p = ( p + 1 ) & 15];
		s1 ^= ( s1 << 31 ); // a
		s1 ^= ( s1 >> 11 ); // b
		s0 ^= ( s0 >> 30 ); // c

		uint64 result = ( s[p] = s0 ^ s1 ) * 1181783497276652981LL;
		pixels[i + 0] = ( result >> 32 ) * ( 1.f / 4294967296.f );
		pixels[i + 1] = ( result & 0x00000000ffffffff )  * ( 1.f / 4294967296.f );
	}

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, pixels );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	delete[] pixels;

	return new Texture( texture );
}

mj::gl::Texture *mj::gl::Texture::Random( int32 width )
{
	GLuint texture;
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_1D, texture );

	uint64 s[16];
	for ( int32 i = 0; i < 16; i++ )
	{
		s[i] = Random::Next<uint64>();
	}
	int32 p = 0;

	float *pixels = new float[width * 4];
	for ( int32 i = 0; i < width * 4; i += 2 )
	{
		uint64 s0 = s[p];
		uint64 s1 = s[p = ( p + 1 ) & 15];
		s1 ^= ( s1 << 31 ); // a
		s1 ^= ( s1 >> 11 ); // b
		s0 ^= ( s0 >> 30 ); // c

		uint64 result = ( s[p] = s0 ^ s1 ) * 1181783497276652981LL;
		pixels[i + 0] = ( result >> 32 ) * ( 1.f / 4294967296.f );
		pixels[i + 1] = ( result & 0x00000000ffffffff )  * ( 1.f / 4294967296.f );
	}

	glTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA, width, 0, GL_RGBA, GL_FLOAT, pixels );

	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	delete[] pixels;

	return new Texture( texture );
}
