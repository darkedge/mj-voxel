#include "CorePCH.h"
#include "Input.h"
#include "Application.h"

// Keyboard
bool mj::Input::prev[NUM_KEYBOARD_KEYS];
bool mj::Input::keys[NUM_KEYBOARD_KEYS];
bool mj::Input::down[NUM_KEYBOARD_KEYS];
bool mj::Input::up[NUM_KEYBOARD_KEYS];

// Mouse
bool mj::Input::mousePrev[NUM_MOUSE_BUTTONS];
bool mj::Input::mouseButtons[NUM_MOUSE_BUTTONS];
bool mj::Input::mouseDown[NUM_MOUSE_BUTTONS];
bool mj::Input::mouseUp[NUM_MOUSE_BUTTONS];

bool mj::Input::isMouseGrabbed;
mj::math::float2 mj::Input::lastMousePos;
mj::math::float2 mj::Input::mouseDelta;
mj::math::float2 mj::Input::mousePos;

void mj::Input::SetMouseGrabbed( bool grabMouse )
{
	isMouseGrabbed = grabMouse;
	if ( isMouseGrabbed )
	{
		glfwSetInputMode( mj::Application::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );
	}
	else
	{
		glfwSetInputMode( mj::Application::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL );
	}
}