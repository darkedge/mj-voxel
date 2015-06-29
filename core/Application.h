#pragma once
namespace mj {

class World;
class Application {
public:
	Application( const char *name = "", int32 width = 1280, int32 height = 720 );
	~Application();

	void Init( const char *name, int32 width, int32 height );
	void Run();

private:
	World *world;
};
} // namespace mj
