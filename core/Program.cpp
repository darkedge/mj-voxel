#include "CorePCH.h"
#include "Program.h"


mj::gl::Program::Program( const char *vs, const char *fs )
{
	m_program = glCreateProgram();

	AttachShaderFromFile( &m_vertex, vs, ShaderType::Vertex );
	AttachShaderFromFile( &m_fragment, fs, ShaderType::Fragment );
	Link();
	Validate();
}


mj::gl::Program::~Program()
{
	glDeleteShader( m_vertex );
	glDeleteShader( m_fragment );
	glDeleteProgram( m_program );
}

void mj::gl::Program::AttachShaderFromFile( GLuint *shader, const char *file, ShaderType type )
{
	int32 size = 0;
#pragma warning(suppress:4996)
	FILE *f = fopen( file, "rb" );
	if ( !f )
	{
		printf( "Could not open file: %s\n", file );
		return;
	}

	fseek( f, 0, SEEK_END );
	size = ftell( f );
	fseek( f, 0, SEEK_SET );
	char *str = new char[size + 1];
	if ( size != fread( str, sizeof( char ), size, f ) )
	{
		delete[] str;
		printf( "Error while reading file: %s\n", file );
		return;
	}
	fclose( f );
	str[size] = '\0';


	switch ( type )
	{
	case ShaderType::Vertex:
		*shader = glCreateShader( GL_VERTEX_SHADER );
		break;
	case ShaderType::Fragment:
		*shader = glCreateShader( GL_FRAGMENT_SHADER );
		break;
	default:
		printf( "Unsupported shader type!" );
		return;
	}

	glShaderSource( *shader, 1, (const char **) &str, nullptr );
	delete[] str;

	GL_TRY( glCompileShader( *shader ) );

	// Check compilation log
	GLint status;
	GL_TRY( glGetShaderiv( *shader, GL_COMPILE_STATUS, &status ) );
	bool success = ( status == GL_TRUE );
	if ( success )
	{
		GL_TRY( glAttachShader( m_program, *shader ) );
	}
	else
	{
		GLint length = 0;
		GL_TRY( glGetShaderiv( *shader, GL_INFO_LOG_LENGTH, &length ) );
		if ( length > 1 )
		{
			char *c_log = new char[length];
			GL_TRY( glGetShaderInfoLog( *shader, length, nullptr, c_log ) );
			printf( "%s\n", c_log );
			delete[] c_log;
		}
	}
}

void mj::gl::Program::Bind() const
{
	GL_TRY( glUseProgram( m_program ) );
}

void mj::gl::Program::Link() const
{
	assert( m_program );
	GL_TRY( glLinkProgram( m_program ) );
	FindProgramErrors( GL_LINK_STATUS );
}

void mj::gl::Program::Validate() const
{
	assert( m_program );
	GL_TRY( glValidateProgram( m_program ) );
	FindProgramErrors( GL_VALIDATE_STATUS );
}

// Returns true if errors were found
bool mj::gl::Program::FindProgramErrors( GLenum type ) const
{
	GLint status;
	GL_TRY( glGetProgramiv( m_program, type, &status ) );
	bool success = ( status == GL_TRUE );
	if ( !success )
	{
		GLint length = 0;
		GL_TRY( glGetProgramiv( m_program, GL_INFO_LOG_LENGTH, &length ) );

		if ( length > 0 )
		{
			char *c_log = new char[length];
			GL_TRY( glGetProgramInfoLog( m_program, length, nullptr, c_log ) );
			printf( "%s\n", c_log );
			delete[] c_log;
		}

		return true;
	}

	return false;
}
