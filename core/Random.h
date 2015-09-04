#pragma once
namespace mj {
class Random
{
public:
	Random();
	~Random();
	
	template <typename T>
	static T Next();
};
}
