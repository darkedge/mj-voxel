#include "CorePCH.h"
#include "Vector.h"

GLFWwindow *g_Window;

void bla() {
	mono_set_dirs( "C:\\Program Files (x86)\\Mono\\lib", "C:\\Program Files (x86)\\Mono\\etc" );
	MonoDomain *domain = mono_jit_init( "hoi?" ); // Can be anything
	MonoAssembly *assembly = mono_domain_assembly_open( domain, "mod.dll" );
	if ( !assembly ) {
		// Assembly not found
		mono_jit_cleanup( domain );
		return;
	}

	MonoImage *image = mono_assembly_get_image( assembly );

	MonoClass *pClass = mono_class_from_name( image, "mod", "MyClass" );

	// Get the constructor of the class.
	MonoMethod *pConstructorMethod = mono_class_get_method_from_name_flags( pClass,
									 ".ctor", -1, 0x0800 ); // METHOD_ATTRIBUTE_SPECIAL_NAME

	// Create a new instance of the class.
	MonoObject *pObject = mono_object_new( domain, pClass );

	// Acquire a GC handle if the object will not be rooted in the CLR universe.
	uint32_t GCHandle = mono_gchandle_new( pObject, false );

	// Invoke the constructor.
	MonoObject *pException = nullptr;
	mono_runtime_invoke( pConstructorMethod, pObject, nullptr, &pException );

	mono_jit_cleanup( domain );
}

void GlfwErrorCallback( int, const char *description ) {
	fputs( description, stderr );
	fputs( "\n", stderr );
}

void InitGLFW( int width, int height, const char *name ) {
	glfwSetErrorCallback( GlfwErrorCallback );

	if ( !glfwInit() )
		exit( 1 );

	// Window hints
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
#ifdef _DEBUG
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
#endif

	g_Window = glfwCreateWindow( width, height, name, nullptr, nullptr );
	glfwMakeContextCurrent( g_Window );

	// TODO: Callbacks
// 	glfwSetKeyCallback(g_Window, GlfwKeyCallBack);
// 	glfwSetMouseButtonCallback(g_Window, GlfwMouseButtonCallback);
// 	glfwSetScrollCallback(g_Window, GlfwScrollCallback);
// 	glfwSetCharCallback(g_Window, GlfwCharCallback);
// 	glfwSetCursorPosCallback(g_Window, GlfwCursorPosCallback);
// 	glfwSetWindowSizeCallback(g_Window, GlfwWindowSizeCallBack);

	// Sync to monitor refresh rate
	glfwSwapInterval( 1 );

	/************************************************************************/
	/* OpenGL                                                               */
	/************************************************************************/
	if ( !gladLoadGL() ) {
		printf( "Failed to init glad!\n" );
		std::exit( EXIT_FAILURE );
	}
	glEnable( GL_BLEND );
	glEnable( GL_MULTISAMPLE );
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
	glClearColor( 0.192156862745098, 0.3019607843137255, 0.4745098039215686, 1.0f );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDepthFunc( GL_LEQUAL );
}

void main() {
	InitGLFW( 1280, 720, "mj-voxel" );

	float dt = 0.0f;
	double lastTime = glfwGetTime();
	while ( !glfwWindowShouldClose( g_Window ) ) {
		double now = glfwGetTime();
		dt = ( float )( now - lastTime );
		lastTime = now;

		glfwPollEvents();

		// TODO Camera's responsibility
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// TODO: Update
		glfwSwapBuffers( g_Window );
	}

	glfwDestroyWindow( g_Window );
	glfwTerminate();

	std::exit( EXIT_SUCCESS );
}
