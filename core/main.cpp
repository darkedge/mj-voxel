#include "CorePCH.h"
#include "Application.h"

void main() {
#if 0
	mj::math::float3 a = {1.0f, 2.0f, 3.0f};
	mj::math::float3 b = {2.0f, 3.0f, 4.0f};
	auto f = mj::math::Cross( a, b );
	printf( "%f %f %f\n", f.x, f.y, f.z );
#endif

	( new mj::Application( "mj-voxel" ) )->Run();
}
