#include "CorePCH.h"
#include "Program.h"


mj::gl::Program::Program(const char *vs, const char *fs)
{
	handle = glCreateProgram();

	
}


mj::gl::Program::~Program()
{
	if ( handle )
	{
		// TODO: Does this delete shaders?
		glDeleteProgram( handle );
	}
}

bool mj::gl::Program::ReadShaderFile( const char *file, ShaderType type )
{
	int32 size = 0;
#pragma warning(suppress:4996)
	FILE *f = fopen( file, "rb" );
	if ( !f )
	{
		printf( "Could not open file: %s\n", file );
		return false;
	}

	fseek( f, 0, SEEK_END );
	size = ftell( f );
	fseek( f, 0, SEEK_SET );
	char *str = new char[size + 1];
	if ( size != fread( str, sizeof( char ), size, f ) )
	{
		delete[] str;
		printf( "Error while reading file: %s\n", file );
		return false;
	}
	fclose( f );
	str[size] = '\0';


	GLuint shader = 0;
	switch ( type )
	{
	case ShaderType::Vertex:
		shader = glCreateShader( GL_VERTEX_SHADER );
		break;
	case ShaderType::Fragment:
		shader = glCreateShader( GL_FRAGMENT_SHADER );
		break;
	default:
		return false;
	}

	glShaderSource( shader, 1, (const char **) &str, nullptr );
	delete[] str;

	return true;
}