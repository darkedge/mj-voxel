#include "CorePCH.h"
#include "Application.h"
#include "State.h"

#if 0
void bla()
{
	mono_set_dirs( "C:\\Program Files (x86)\\Mono\\lib", "C:\\Program Files (x86)\\Mono\\etc" );
	MonoDomain *domain = mono_jit_init( "hoi?" ); // Can be anything
	MonoAssembly *assembly = mono_domain_assembly_open( domain, "mod.dll" );
	if ( !assembly )
	{
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
	uint32 GCHandle = mono_gchandle_new( pObject, false );

	// Invoke the constructor.
	MonoObject *pException = nullptr;
	mono_runtime_invoke( pConstructorMethod, pObject, nullptr, &pException );

	mono_jit_cleanup( domain );
}
#endif

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
	TestLua();
	( new mj::Application( "mj-voxel" ) )->Run();
}
