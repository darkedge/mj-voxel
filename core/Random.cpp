#include "CorePCH.h"
#include "Random.h"
#include <ctime>

static const float kUint32ToFloat = 1.0f / 4294967296.0f;

static mj::Random s_random;
uint64 mj::Random::s[16];
int32 mj::Random::p = 0;

mj::Random::Random()
{
	std::srand(std::time(0));
	int32 size = sizeof(s);
	unsigned char *a = (unsigned char*) s;
	for (int32 i = 0; i < size; i++)
	{
		a[i] = rand();
	}
}

mj::math::float4 mj::Random::NextFloat4()
{
	uint64 result;
	math::float4 f;
	result = NextUint64();
	f.x = (result >> 32) * kUint32ToFloat;
	f.y = (result & 0x00000000ffffffff) * kUint32ToFloat;
	result = NextUint64();
	f.z = (result >> 32) * kUint32ToFloat;
	f.w = (result & 0x00000000ffffffff) * kUint32ToFloat;
	return f;
}

mj::math::float2 mj::Random::NextFloat2()
{
	uint64 result = NextUint64();
	math::float2 f;
	f.x = (result >> 32) * kUint32ToFloat;
	f.y = (result & 0x00000000ffffffff) * kUint32ToFloat;
	return f;
}

uint64 mj::Random::NextUint64()
{
	uint64 s0 = s[p];
	uint64 s1 = s[p = (p + 1) & 15];
	s1 ^= (s1 << 31);
	s1 ^= (s1 >> 11);
	s0 ^= (s0 >> 30);
	return (s[p] = s0 ^ s1) * 1181783497276652981LL;
}
