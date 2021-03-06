﻿#include "CorePCH.h"
#include "Application.h"
#include "World.h"
#include "Input.h"

GLFWwindow *mj::Application::s_window = nullptr;
int32 mj::Application::s_height = -1;
int32 mj::Application::s_width = -1;
float mj::Application::s_deltaTime = 0.0f;

/************************************************************************/
/* OpenGL Callbacks                                                     */
/************************************************************************/

void CALLBACK debugCallbackARB( GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar *message, GLvoid *userParam )
{
	printf( "%s\n", message );
}

/************************************************************************/
/* GLFW Callbacks                                                       */
/************************************************************************/

void GlfwErrorCallback( int32, const char *description )
{
	fputs( description, stderr );
	fputs( "\n", stderr );
}

void GlfwKeyCallBack( GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods )
{
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, GL_TRUE );

	if ( action == GLFW_PRESS )
	{
		mj::Input::SetKey( key, true );
	}
	if ( action == GLFW_RELEASE )
	{
		mj::Input::SetKey( key, false );
	}
}

void GlfwMouseButtonCallback( GLFWwindow *window, int32 button, int32 action, int32 mods )
{
	if ( action == GLFW_PRESS )
	{
		mj::Input::SetMouseButton( button, true );
	}
	if ( action == GLFW_RELEASE )
	{
		mj::Input::SetMouseButton( button, false );
	}
}

void GlfwScrollCallback( GLFWwindow *window, double xoffset, double yoffset )
{
	// TODO: GUI
}

void GlfwCharCallback( GLFWwindow *window, uint32 codepoint )
{
	// TODO: GUI
}

void GlfwCursorPosCallback( GLFWwindow *window, double xpos, double ypos )
{
	mj::Input::SetMousePosition( mj::math::float2( (float) xpos, (float) ypos ) );
}

void GlfwWindowSizeCallBack( GLFWwindow *window, int32 width, int32 height )
{
	mj::Application::SetWidth( width );
	mj::Application::SetHeight( height );
}

/************************************************************************/
/* Application                                                          */
/************************************************************************/

mj::Application::Application( const char *name /* = "" */, int32 width /* = 1280 */, int32 height /* = 720 */ )
{
	s_width = width;
	s_height = height;
	Init( name );
}

void mj::Application::Init( const char *name )
{
	glfwSetErrorCallback( GlfwErrorCallback );

	if ( !glfwInit() )
		exit( 1 );

	// Window hints
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	s_window = glfwCreateWindow( s_width, s_height, name, nullptr, nullptr );
	glfwMakeContextCurrent( s_window );

	// TODO: Callbacks
	glfwSetKeyCallback( s_window, GlfwKeyCallBack );
	glfwSetMouseButtonCallback( s_window, GlfwMouseButtonCallback );
	glfwSetScrollCallback( s_window, GlfwScrollCallback );
	glfwSetCharCallback( s_window, GlfwCharCallback );
	glfwSetCursorPosCallback( s_window, GlfwCursorPosCallback );
	glfwSetWindowSizeCallback( s_window, GlfwWindowSizeCallBack );

	// Sync to monitor refresh rate
	glfwSwapInterval( 1 );

	/************************************************************************/
	/* OpenGL                                                               */
	/************************************************************************/
	if ( !gladLoadGL() )
	{
		printf( "Failed to init glad!\n" );
		std::exit( EXIT_FAILURE );
	}
	glEnable( GL_BLEND );
	glEnable( GL_MULTISAMPLE );
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
	glClearColor( 0.192156862745098f, 0.3019607843137255f, 0.4745098039215686f, 1.0f );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDepthFunc( GL_LEQUAL );

	// https://www.opengl.org/wiki/Debug_Output
	glDebugMessageCallback( (GLDEBUGPROC) debugCallbackARB, nullptr );
	glEnable( GL_DEBUG_OUTPUT );
	glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );

	world = new World();
}

mj::Application::~Application()
{
	delete world;
}

void mj::Application::Run()
{
	double lastTime = glfwGetTime();
	while ( !glfwWindowShouldClose( s_window ) )
	{
		double now = glfwGetTime();
		SetDeltaTime( (float) ( now - lastTime ) );
		lastTime = now;

		glfwPollEvents();
		mj::Input::Tick();

		world->Tick();
		mj::Input::PostTick();
		glfwSwapBuffers( s_window );
	}

	glfwDestroyWindow( s_window );
	glfwTerminate();

	std::exit( EXIT_SUCCESS );
}
