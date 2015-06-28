#pragma once
namespace mj {
class Application
{
public:
	Application(const char *name = "", int32 width = 1280, int32 height = 720);
	~Application();

	void Init(const char *name, int32 width, int32 height);
	void Run();

private:
	math::Vector3 position;
	math::Quaternion rotation;
};
} // namespace mj
