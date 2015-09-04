#include "CorePCH.h"
#include "Application.h"
#include "State.h"

// This tells NVIDIA Optimus to use the dedicated video card
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

// Lua panic catch function
// TODO: Log this to error stream
static int LuaPanic(lua_State *L) {
	// Retrieve error message at the top of the stack
	auto ret = lua_tostring(L, -1);
	lua_pop(L, 1);
	printf("Lua panic - %s\n", ret);

	return 0; // exit(EXIT_FAILURE) is called by Lua
}

void TestLua()
{
	mj::lua::State state;
	lua_atpanic(state, LuaPanic);
	if(state.LoadFile( "HelloWorld.lua" )) {
		// This works
		if(0) {
			printf("%f\n", (double) state["foo"]);
			state["foo"].Print();
			state["foo"] = 5;
			state["foo"].Print();
		}

		// This does not work yet
		if(1) {
			//state["one"]["two"]["three"] = "four";
			auto one = state["one"];
			auto two = one[2];
			auto three = two["three"];
			three = "four";
			three.Print(); // one.two.three = four // yay!
		} // Memory corruption?

		// bar = (null) // ???
		if(0) {
			auto ref = state["bar"];
			ref.Print();
		}

		// This works
		if(0) {
			auto ref = state["this_does_not_exist"];
			ref = "what happens";
			ref.Print();
		}

		mj::lua::stackDump( state );
	}
}

int main()
{
	//TestLua();
	( new mj::Application( "mj-voxel" ) )->Run();
	return 0;
}
