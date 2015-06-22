#include <cstdlib>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

void bla() {
	mono_set_dirs( "C:\\Program Files (x86)\\Mono\\lib", "C:\\Program Files (x86)\\Mono\\etc" );
	MonoDomain *domain = mono_jit_init( "hoi?" ); // Can be anything
	MonoAssembly *assembly = mono_domain_assembly_open( domain, "mod.dll" );

	MonoImage *image = mono_assembly_get_image( assembly );

	MonoClass *pClass = mono_class_from_name( image,
						"mod", "MyClass" );

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

	mono_jit_cleanup (domain);
}

void main() {
	//bla();
}