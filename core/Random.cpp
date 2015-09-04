#include "CorePCH.h"
#include "Random.h"
#include <ctime>

static mj::Random s_random;

mj::Random::Random()
{
	srand(time(NULL));
}


mj::Random::~Random()
{
}

template <typename T>
T mj::Random::Next() {
	return T(0);
}