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
	std::ifstream strm( file, std::ifstream::in );
	if ( !strm )
	{
		printf( "File not found: %s\n", file );
		return;
	}

	//std::ostringstream txt;

}