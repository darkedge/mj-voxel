#include "CorePCH.h"
#include "Macro.h"

#define GL_ERROR_CASE( err )\
	case err:\
	str = #err;\
	break;

// https://www.opengl.org/sdk/docs/man/html/glGetError.xhtml
const char *mjGlErrorString( GLenum err )
{
	const char *str = "";
	switch ( err )
	{
		GL_ERROR_CASE( GL_NO_ERROR );
		GL_ERROR_CASE( GL_INVALID_ENUM );
		GL_ERROR_CASE( GL_INVALID_VALUE );
		GL_ERROR_CASE( GL_INVALID_OPERATION );
		GL_ERROR_CASE( GL_INVALID_FRAMEBUFFER_OPERATION );
		GL_ERROR_CASE( GL_OUT_OF_MEMORY );
		GL_ERROR_CASE( GL_STACK_UNDERFLOW );
		GL_ERROR_CASE( GL_STACK_OVERFLOW );
	}
	return str;
}
