#include "CorePCH.h"
#include "Application.h"
#include "State.h"

// This tells NVIDIA Optimus to use the dedicated video card
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

void TestLua()
{
	mj::lua::State state;
	if(state.LoadFile( "HelloWorld.lua" )) {
		//printf( "%d\n", (int64)state["foo"] );
		//state["foo"].Print();
		//state["foo"] = 5;
		auto ref = state["bar"];
		state["one"]["two"]["three"] = "four";
		ref.Print();
		//ref = ref = "hoi";
		//ref.Print();
		stackDump( state );
	}
}

int main()
{
	//TestLua();
	( new mj::Application( "mj-voxel" ) )->Run();
	return 0;
}
