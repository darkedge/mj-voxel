#define XSTR( X ) STR( X )
#define STR( X ) #X

const char *mjGlErrorString( GLenum err );

#ifdef _DEBUG

#define GL_FLUSH_ERRORS()\
{\
	GLenum err;\
	do\
	{\
		err = glGetError();\
		if ( err != GL_NO_ERROR )\
		{\
			const char *str = mjGlErrorString( err );\
			printf("Flushed error: %s\nAt: %s:%d\n", str, __FILE__, __LINE__ );\
		}\
	} while( err != GL_NO_ERROR );\
}

#define GL_TRY( expr )\
{\
	GLenum err;\
	do\
	{\
		err = glGetError();\
		if ( err != GL_NO_ERROR )\
		{\
			const char *str = mjGlErrorString( err );\
			printf("%s generated before executing %s:\n\tAt: %s:%d\n", str, #expr, __FILE__, __LINE__ );\
		}\
	} while( err != GL_NO_ERROR );\
	expr;\
	do\
	{\
		err = glGetError();\
		if ( err != GL_NO_ERROR )\
		{\
			const char *str = mjGlErrorString( err );\
			printf( "OpenGL error: %s\n\tBy: %s\n\tAt: %s:%d\n", str, #expr, __FILE__, __LINE__ );\
		}\
	} while ( err != GL_NO_ERROR );\
}

#else
#define GL_TRY(expr) expr
#define GL_FLUSH_ERRORS()
#endif
