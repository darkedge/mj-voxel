#pragma once
namespace mj {
class Random
{
public:
	Random();
	
	static math::float4 NextFloat4();
	static math::float2 NextFloat2();
	static uint64 NextUint64();

private:
	static uint64 s[];
	static int32 p;
};
}
