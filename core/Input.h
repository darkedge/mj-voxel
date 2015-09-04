#pragma once

namespace mj {
class Input {
public:
	static bool GetKey(int key);

	static bool GetKeyDown(int key);

	static bool GetKeyUp(int key);

	static bool GetMouse(int button);

	static bool GetMouseDown(int button);

	static bool GetMouseUp(int button);

	static bool IsMouseGrabbed();

	// In pixels. Origin is top-left
	static math::float2 GetMouseDelta();

	// In pixels. Origin is top-left
	static math::float2 GetMousePosition();

	static void SetMouseButton(int button, bool pressed);

	static void SetKey(int key, bool pressed);

	static void SetMousePosition(const math::float2 pos);

	static void Init();

	static void Tick();

	static void PostTick();

	static void SetMouseGrabbed(bool grabMouse);

protected:
	// Keyboard
	static bool prev[];
	static bool keys[];
	static bool down[];
	static bool up[];

	// Mouse
	static bool mousePrev[];
	static bool mouseButtons[];
	static bool mouseDown[];
	static bool mouseUp[];

	static bool isMouseGrabbed;
	static math::float2 lastMousePos;
	static math::float2 mouseDelta;
	static math::float2 mousePos;
};

class KeyCode
{
public:
static const int32 UNKNOWN;

static const int32 SPACE;
static const int32 APOSTROPHE;/* ' */
static const int32 COMMA;/* , */
static const int32 MINUS;/* - */
static const int32 PERIOD;/* . */
static const int32 SLASH;/* / */
static const int32 ALPHA_0;
static const int32 ALPHA_1;
static const int32 ALPHA_2;
static const int32 ALPHA_3;
static const int32 ALPHA_4;
static const int32 ALPHA_5;
static const int32 ALPHA_6;
static const int32 ALPHA_7;
static const int32 ALPHA_8;
static const int32 ALPHA_9;
static const int32 SEMICOLON;/*; */
static const int32 EQUAL;/* = */
static const int32 A;
static const int32 B;
static const int32 C;
static const int32 D;
static const int32 E;
static const int32 F;
static const int32 G;
static const int32 H;
static const int32 I;
static const int32 J;
static const int32 K;
static const int32 L;
static const int32 M;
static const int32 N;
static const int32 O;
static const int32 P;
static const int32 Q;
static const int32 R;
static const int32 S;
static const int32 T;
static const int32 U;
static const int32 V;
static const int32 W;
static const int32 X;
static const int32 Y;
static const int32 Z;
static const int32 LEFT_BRACKET;/* [ */
static const int32 BACKSLASH;/* \ */
static const int32 RIGHT_BRACKET;/* ] */
static const int32 GRAVE_ACCENT;/* ` */
static const int32 WORLD_1; /* non-US #1 */
static const int32 WORLD_2; /* non-US #2 */

static const int32 ESCAPE;
static const int32 ENTER;
static const int32 TAB;
static const int32 BACKSPACE;
static const int32 INSERT;
static const int32 DELETE;
static const int32 RIGHT;
static const int32 LEFT;
static const int32 DOWN;
static const int32 UP;
static const int32 PAGE_UP;
static const int32 PAGE_DOWN;
static const int32 HOME;
static const int32 END;
static const int32 CAPS_LOCK;
static const int32 SCROLL_LOCK;
static const int32 NUM_LOCK;
static const int32 PRINT_SCREEN;
static const int32 PAUSE;
static const int32 F1;
static const int32 F2;
static const int32 F3;
static const int32 F4;
static const int32 F5;
static const int32 F6;
static const int32 F7;
static const int32 F8;
static const int32 F9;
static const int32 F10;
static const int32 F11;
static const int32 F12;
static const int32 F13;
static const int32 F14;
static const int32 F15;
static const int32 F16;
static const int32 F17;
static const int32 F18;
static const int32 F19;
static const int32 F20;
static const int32 F21;
static const int32 F22;
static const int32 F23;
static const int32 F24;
static const int32 F25;
static const int32 KP_0;
static const int32 KP_1;
static const int32 KP_2;
static const int32 KP_3;
static const int32 KP_4;
static const int32 KP_5;
static const int32 KP_6;
static const int32 KP_7;
static const int32 KP_8;
static const int32 KP_9;
static const int32 KP_DECIMAL;
static const int32 KP_DIVIDE;
static const int32 KP_MULTIPLY;
static const int32 KP_SUBTRACT;
static const int32 KP_ADD;
static const int32 KP_ENTER;
static const int32 KP_EQUAL;
static const int32 LEFT_SHIFT;
static const int32 LEFT_CONTROL;
static const int32 LEFT_ALT;
static const int32 LEFT_SUPER;
static const int32 RIGHT_SHIFT;
static const int32 RIGHT_CONTROL;
static const int32 RIGHT_ALT;
static const int32 RIGHT_SUPER;
static const int32 MENU;
static const int32 LAST;

#if 0
static const int32 
static const int32 MOD_SHIFT 0x0001
static const int32 MOD_CONTROL 0x0002
static const int32 MOD_ALT0x0004
static const int32 MOD_SUPER 0x0008

static const int32 MOUSE_BUTTON_1;
static const int32 MOUSE_BUTTON_2;
static const int32 MOUSE_BUTTON_3;
static const int32 MOUSE_BUTTON_4;
static const int32 MOUSE_BUTTON_5;
static const int32 MOUSE_BUTTON_6;
static const int32 MOUSE_BUTTON_7;
static const int32 MOUSE_BUTTON_8;
static const int32 MOUSE_BUTTON_LAST GLFW_MOUSE_BUTTON_8
static const int32 MOUSE_BUTTON_LEFT GLFW_MOUSE_BUTTON_1
static const int32 MOUSE_BUTTON_RIGHT GLFW_MOUSE_BUTTON_2
static const int32 MOUSE_BUTTON_MIDDLE GLFW_MOUSE_BUTTON_3

static const int32 JOYSTICK_1;
static const int32 JOYSTICK_2;
static const int32 JOYSTICK_3;
static const int32 JOYSTICK_4;
static const int32 JOYSTICK_5;
static const int32 JOYSTICK_6;
static const int32 JOYSTICK_7;
static const int32 JOYSTICK_8; 
static const int32 JOYSTICK_9; 
static const int32 JOYSTICK_10; 
static const int32 JOYSTICK_11; 
static const int32 JOYSTICK_12; 
static const int32 JOYSTICK_13; 
static const int32 JOYSTICK_14; 
static const int32 JOYSTICK_15;
static const int32 JOYSTICK_16;
static const int32 JOYSTICK_LAST;
#endif
};
} // namespace kl
