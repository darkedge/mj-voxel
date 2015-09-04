#include "Common.h"
#include <GLFW/glfw3.h>
#include "Input.h"
#include "Application.h"

#define NUM_KEYBOARD_KEYS GLFW_KEY_LAST + 1
#define NUM_MOUSE_BUTTONS GLFW_MOUSE_BUTTON_LAST + 1

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

bool mj::Input::GetKey(int key)
{
	return keys[key];
}

bool mj::Input::GetKeyDown(int key)
{
	return down[key];
}

bool mj::Input::GetKeyUp(int key)
{
	return up[key];
}

bool mj::Input::GetMouse(int button)
{
	return mouseButtons[button];
}

bool mj::Input::GetMouseDown(int button)
{
	return mouseDown[button];
}

bool mj::Input::GetMouseUp(int button)
{
	return mouseUp[button];
}

bool mj::Input::IsMouseGrabbed()
{
	return isMouseGrabbed;
}

// In pixels. Origin is top-left
mj::math::float2 mj::Input::GetMouseDelta()
{
	return mouseDelta;
}

// In pixels. Origin is top-left
mj::math::float2 mj::Input::GetMousePosition()
{
	return mousePos;
}

void mj::Input::SetMouseButton(int button, bool pressed)
{
	mouseButtons[button] = pressed;
}

void mj::Input::SetKey(int key, bool pressed)
{
	keys[key] = pressed;
}

void mj::Input::SetMousePosition(const math::float2 pos)
{
	mousePos = pos;
	mouseDelta += pos - lastMousePos;
}

void mj::Input::Init()
{
	// Keyboard
	memset(prev, int(false), NUM_KEYBOARD_KEYS * sizeof(bool));
	memset(keys, int(false), NUM_KEYBOARD_KEYS * sizeof(bool));
	memset(down, int(false), NUM_KEYBOARD_KEYS * sizeof(bool));
	memset(up, int(false), NUM_KEYBOARD_KEYS * sizeof(bool));

	// Mouse
	memset(mousePrev, int(false), NUM_MOUSE_BUTTONS * sizeof(bool));
	memset(mouseButtons, int(false), NUM_MOUSE_BUTTONS * sizeof(bool));
	memset(mouseDown, int(false), NUM_MOUSE_BUTTONS * sizeof(bool));
	memset(mouseUp, int(false), NUM_MOUSE_BUTTONS * sizeof(bool));

	isMouseGrabbed = false;
	mouseDelta = { 0, 0 };
}

void mj::Input::Tick()
{
	// Keyboard
	{
		bool changes[NUM_KEYBOARD_KEYS];
		for (unsigned int i = 0; i < NUM_KEYBOARD_KEYS; i++)
		{
			changes[i] = keys[i] ^ prev[i];
			down[i] = changes[i] & keys[i];
			up[i] = changes[i] & !keys[i];
		}
	}

	// Mouse
		{
			bool changes[NUM_MOUSE_BUTTONS];
			for (unsigned int i = 0; i < NUM_MOUSE_BUTTONS; i++)
			{
				changes[i] = mouseButtons[i] ^ mousePrev[i];
				mouseDown[i] = changes[i] & mouseButtons[i];
				mouseUp[i] = changes[i] & !mouseButtons[i];
			}
		}
}

void mj::Input::PostTick()
{
	memcpy(prev, keys, NUM_KEYBOARD_KEYS * sizeof(bool));
	memcpy(mousePrev, mouseButtons, NUM_MOUSE_BUTTONS * sizeof(bool));
	mouseDelta = { 0, 0 };
	lastMousePos = mousePos;
}

void mj::Input::SetMouseGrabbed(bool grabMouse)
{
	isMouseGrabbed = grabMouse;
	if (isMouseGrabbed)
	{
		glfwSetInputMode(mj::Application::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(mj::Application::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

const int32 mj::KeyCode::UNKNOWN = GLFW_KEY_UNKNOWN;

const int32 mj::KeyCode::SPACE = GLFW_KEY_SPACE;
const int32 mj::KeyCode::APOSTROPHE = GLFW_KEY_APOSTROPHE  /* ' */;
const int32 mj::KeyCode::COMMA = GLFW_KEY_COMMA  /* , */;
const int32 mj::KeyCode::MINUS = GLFW_KEY_MINUS  /* - */;
const int32 mj::KeyCode::PERIOD = GLFW_KEY_PERIOD  /* . */;
const int32 mj::KeyCode::SLASH = GLFW_KEY_SLASH  /* / */;
const int32 mj::KeyCode::ALPHA_0 = GLFW_KEY_0;
const int32 mj::KeyCode::ALPHA_1 = GLFW_KEY_1;
const int32 mj::KeyCode::ALPHA_2 = GLFW_KEY_2;
const int32 mj::KeyCode::ALPHA_3 = GLFW_KEY_3;
const int32 mj::KeyCode::ALPHA_4 = GLFW_KEY_4;
const int32 mj::KeyCode::ALPHA_5 = GLFW_KEY_5;
const int32 mj::KeyCode::ALPHA_6 = GLFW_KEY_6;
const int32 mj::KeyCode::ALPHA_7 = GLFW_KEY_7;
const int32 mj::KeyCode::ALPHA_8 = GLFW_KEY_8;
const int32 mj::KeyCode::ALPHA_9 = GLFW_KEY_9;
const int32 mj::KeyCode::SEMICOLON = GLFW_KEY_SEMICOLON  /* ; */;
const int32 mj::KeyCode::EQUAL = GLFW_KEY_EQUAL /* = */;
const int32 mj::KeyCode::A = GLFW_KEY_A;
const int32 mj::KeyCode::B = GLFW_KEY_B;
const int32 mj::KeyCode::C = GLFW_KEY_C;
const int32 mj::KeyCode::D = GLFW_KEY_D;
const int32 mj::KeyCode::E = GLFW_KEY_E;
const int32 mj::KeyCode::F = GLFW_KEY_F;
const int32 mj::KeyCode::G = GLFW_KEY_G;
const int32 mj::KeyCode::H = GLFW_KEY_H;
const int32 mj::KeyCode::I = GLFW_KEY_I;
const int32 mj::KeyCode::J = GLFW_KEY_J;
const int32 mj::KeyCode::K = GLFW_KEY_K;
const int32 mj::KeyCode::L = GLFW_KEY_L;
const int32 mj::KeyCode::M = GLFW_KEY_M;
const int32 mj::KeyCode::N = GLFW_KEY_N;
const int32 mj::KeyCode::O = GLFW_KEY_O;
const int32 mj::KeyCode::P = GLFW_KEY_P;
const int32 mj::KeyCode::Q = GLFW_KEY_Q;
const int32 mj::KeyCode::R = GLFW_KEY_R;
const int32 mj::KeyCode::S = GLFW_KEY_S;
const int32 mj::KeyCode::T = GLFW_KEY_T;
const int32 mj::KeyCode::U = GLFW_KEY_U;
const int32 mj::KeyCode::V = GLFW_KEY_V;
const int32 mj::KeyCode::W = GLFW_KEY_W;
const int32 mj::KeyCode::X = GLFW_KEY_X;
const int32 mj::KeyCode::Y = GLFW_KEY_Y;
const int32 mj::KeyCode::Z = GLFW_KEY_Z;
const int32 mj::KeyCode::LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET   /* [ */;
const int32 mj::KeyCode::BACKSLASH = GLFW_KEY_BACKSLASH   /* \ */;
const int32 mj::KeyCode::RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET   /* ] */;
const int32 mj::KeyCode::GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT   /* ` */;
const int32 mj::KeyCode::WORLD_1 = GLFW_KEY_WORLD_1  /* non-US #1 */;
const int32 mj::KeyCode::WORLD_2 = GLFW_KEY_WORLD_2  /* non-US #2 */;

const int32 mj::KeyCode::ESCAPE = GLFW_KEY_ESCAPE;
const int32 mj::KeyCode::ENTER = GLFW_KEY_ENTER;
const int32 mj::KeyCode::TAB = GLFW_KEY_TAB;
const int32 mj::KeyCode::BACKSPACE = GLFW_KEY_BACKSPACE;
const int32 mj::KeyCode::INSERT = GLFW_KEY_INSERT;
const int32 mj::KeyCode::DELETE = GLFW_KEY_DELETE;
const int32 mj::KeyCode::RIGHT = GLFW_KEY_RIGHT;
const int32 mj::KeyCode::LEFT = GLFW_KEY_LEFT;
const int32 mj::KeyCode::DOWN = GLFW_KEY_DOWN;
const int32 mj::KeyCode::UP = GLFW_KEY_UP;
const int32 mj::KeyCode::PAGE_UP = GLFW_KEY_PAGE_UP;
const int32 mj::KeyCode::PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
const int32 mj::KeyCode::HOME = GLFW_KEY_HOME;
const int32 mj::KeyCode::END = GLFW_KEY_END;
const int32 mj::KeyCode::CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
const int32 mj::KeyCode::SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
const int32 mj::KeyCode::NUM_LOCK = GLFW_KEY_NUM_LOCK;
const int32 mj::KeyCode::PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
const int32 mj::KeyCode::PAUSE = GLFW_KEY_PAUSE;
const int32 mj::KeyCode::F1 = GLFW_KEY_F1;
const int32 mj::KeyCode::F2 = GLFW_KEY_F2;
const int32 mj::KeyCode::F3 = GLFW_KEY_F3;
const int32 mj::KeyCode::F4 = GLFW_KEY_F4;
const int32 mj::KeyCode::F5 = GLFW_KEY_F5;
const int32 mj::KeyCode::F6 = GLFW_KEY_F6;
const int32 mj::KeyCode::F7 = GLFW_KEY_F7;
const int32 mj::KeyCode::F8 = GLFW_KEY_F8;
const int32 mj::KeyCode::F9 = GLFW_KEY_F9;
const int32 mj::KeyCode::F10 = GLFW_KEY_F10;
const int32 mj::KeyCode::F11 = GLFW_KEY_F11;
const int32 mj::KeyCode::F12 = GLFW_KEY_F12;
const int32 mj::KeyCode::F13 = GLFW_KEY_F13;
const int32 mj::KeyCode::F14 = GLFW_KEY_F14;
const int32 mj::KeyCode::F15 = GLFW_KEY_F15;
const int32 mj::KeyCode::F16 = GLFW_KEY_F16;
const int32 mj::KeyCode::F17 = GLFW_KEY_F17;
const int32 mj::KeyCode::F18 = GLFW_KEY_F18;
const int32 mj::KeyCode::F19 = GLFW_KEY_F19;
const int32 mj::KeyCode::F20 = GLFW_KEY_F20;
const int32 mj::KeyCode::F21 = GLFW_KEY_F21;
const int32 mj::KeyCode::F22 = GLFW_KEY_F22;
const int32 mj::KeyCode::F23 = GLFW_KEY_F23;
const int32 mj::KeyCode::F24 = GLFW_KEY_F24;
const int32 mj::KeyCode::F25 = GLFW_KEY_F25;
const int32 mj::KeyCode::KP_0 = GLFW_KEY_KP_0;
const int32 mj::KeyCode::KP_1 = GLFW_KEY_KP_1;
const int32 mj::KeyCode::KP_2 = GLFW_KEY_KP_2;
const int32 mj::KeyCode::KP_3 = GLFW_KEY_KP_3;
const int32 mj::KeyCode::KP_4 = GLFW_KEY_KP_4;
const int32 mj::KeyCode::KP_5 = GLFW_KEY_KP_5;
const int32 mj::KeyCode::KP_6 = GLFW_KEY_KP_6;
const int32 mj::KeyCode::KP_7 = GLFW_KEY_KP_7;
const int32 mj::KeyCode::KP_8 = GLFW_KEY_KP_8;
const int32 mj::KeyCode::KP_9 = GLFW_KEY_KP_9;
const int32 mj::KeyCode::KP_DECIMAL = GLFW_KEY_KP_DECIMAL;
const int32 mj::KeyCode::KP_DIVIDE = GLFW_KEY_KP_DIVIDE;
const int32 mj::KeyCode::KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
const int32 mj::KeyCode::KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
const int32 mj::KeyCode::KP_ADD = GLFW_KEY_KP_ADD;
const int32 mj::KeyCode::KP_ENTER = GLFW_KEY_KP_ENTER;
const int32 mj::KeyCode::KP_EQUAL = GLFW_KEY_KP_EQUAL;
const int32 mj::KeyCode::LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
const int32 mj::KeyCode::LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
const int32 mj::KeyCode::LEFT_ALT = GLFW_KEY_LEFT_ALT;
const int32 mj::KeyCode::LEFT_SUPER = GLFW_KEY_LEFT_SUPER;
const int32 mj::KeyCode::RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
const int32 mj::KeyCode::RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
const int32 mj::KeyCode::RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
const int32 mj::KeyCode::RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER;
const int32 mj::KeyCode::MENU = GLFW_KEY_MENU;
const int32 mj::KeyCode::LAST = GLFW_KEY_LAST;
