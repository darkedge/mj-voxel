#pragma once
namespace mj {
	class World;
	class Application
	{
	public:
		Application( const char *name = "", int32 width = 1280, int32 height = 720 );
		~Application();

		void Init( const char *name );
		void Run();

		static int32 GetWidth() { return s_width; }
		static int32 GetHeight() { return s_height; }

	private:
		static int32 s_width, s_height;

		World *world;
	};
} // namespace mj
