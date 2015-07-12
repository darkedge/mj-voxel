#pragma once

#define NUM_KEYBOARD_KEYS GLFW_KEY_LAST + 1
#define NUM_MOUSE_BUTTONS GLFW_MOUSE_BUTTON_LAST + 1

namespace mj {
class Input {
public:
	static bool GetKey( int key ) {
		return keys[key];
	}
	static bool GetKeyDown( int key ) {
		return down[key];
	}
	static bool GetKeyUp( int key ) {
		return up[key];
	}

	static bool GetMouse( int button ) {
		return mouseButtons[button];
	}

	static bool GetMouseDown( int button ) {
		return mouseDown[button];
	}

	static bool GetMouseUp( int button ) {
		return mouseUp[button];
	}

	static bool IsMouseGrabbed() {
		return isMouseGrabbed;
	}

	// In pixels. Origin is top-left
	static math::float2 GetMouseDelta() {
		return mouseDelta;
	}

	// In pixels. Origin is top-left
	static math::float2 GetMousePosition() {
		return mousePos;
	}

	static void SetMouseButton( int button, bool pressed ) {
		mouseButtons[button] = pressed;
	}

	static void SetKey( int key, bool pressed ) {
		keys[key] = pressed;
	}

	static void SetMousePosition( const math::float2 pos ) {
		mousePos = pos;
		mouseDelta += pos - lastMousePos;
	}

	static void Init() {
		// Keyboard
		memset( prev, int( false ), NUM_KEYBOARD_KEYS * sizeof( bool ) );
		memset( keys, int( false ), NUM_KEYBOARD_KEYS * sizeof( bool ) );
		memset( down, int( false ), NUM_KEYBOARD_KEYS * sizeof( bool ) );
		memset( up, int( false ), NUM_KEYBOARD_KEYS * sizeof( bool ) );

		// Mouse
		memset( mousePrev, int( false ), NUM_MOUSE_BUTTONS * sizeof( bool ) );
		memset( mouseButtons, int( false ), NUM_MOUSE_BUTTONS * sizeof( bool ) );
		memset( mouseDown, int( false ), NUM_MOUSE_BUTTONS * sizeof( bool ) );
		memset( mouseUp, int( false ), NUM_MOUSE_BUTTONS * sizeof( bool ) );

		isMouseGrabbed = false;
		mouseDelta = {0, 0};
	}

	static void Tick() {
		// Keyboard
		{
			bool changes[NUM_KEYBOARD_KEYS];
			for ( unsigned int i = 0; i < NUM_KEYBOARD_KEYS; i++ ) {
				changes[i]	= keys[i]		^ prev[i];
				down[i]		= changes[i]	& keys[i];
				up[i]		= changes[i]	& !keys[i];
			}
		}

		// Mouse
		{
			bool changes[NUM_MOUSE_BUTTONS];
			for ( unsigned int i = 0; i < NUM_MOUSE_BUTTONS; i++ ) {
				changes[i] = mouseButtons[i] ^ mousePrev[i];
				mouseDown[i] = changes[i] & mouseButtons[i];
				mouseUp[i] = changes[i] & !mouseButtons[i];
			}
		}
	}

	static void PostTick() {
		memcpy( prev, keys, NUM_KEYBOARD_KEYS * sizeof( bool ) );
		memcpy( mousePrev, mouseButtons, NUM_MOUSE_BUTTONS * sizeof( bool ) );
		mouseDelta = {0, 0};
		lastMousePos = mousePos;
	}

	static void SetMouseGrabbed( bool grabMouse );

protected:
	// Keyboard
	static bool prev[NUM_KEYBOARD_KEYS];
	static bool keys[NUM_KEYBOARD_KEYS];
	static bool down[NUM_KEYBOARD_KEYS];
	static bool up[NUM_KEYBOARD_KEYS];

	// Mouse
	static bool mousePrev[NUM_MOUSE_BUTTONS];
	static bool mouseButtons[NUM_MOUSE_BUTTONS];
	static bool mouseDown[NUM_MOUSE_BUTTONS];
	static bool mouseUp[NUM_MOUSE_BUTTONS];

	static bool isMouseGrabbed;
	static math::float2 lastMousePos;
	static math::float2 mouseDelta;
	static math::float2 mousePos;
};
} // namespace kl
