#ifdef _DEBUG
#define GL_TRY(expr)\
{\
	expr;\
	GLenum err = glGetError();\
	const GLubyte *str = gluErrorString(err);\
	if (err != GL_NO_ERROR)\
	{\
		printf("%s(%d): %s failed, OpenGL error: %d - %s\n", __FILE__, __LINE__, #expr, err, str);\
	}\
}
#else
#define GL_TRY(expr) expr
#endif
