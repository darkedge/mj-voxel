#include "CorePCH.h"
#include "Application.h"
#include "State.h"

void TestLua()
{
	mj::lua::State state;
	state.LoadFile( "HelloWorld.lua" );
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

void main()
{
	//TestLua();
	( new mj::Application( "mj-voxel" ) )->Run();
}
