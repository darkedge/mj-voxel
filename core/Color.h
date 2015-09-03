#pragma once
#include "MathUtil.h"

namespace mj {
class Color
{
public:
	Color();
	~Color();

private:
	math::float4 m_value;
};
}
