#pragma once
namespace mj {
class Camera;
class Transform;
class World {
public:
	World();
	~World();

	void Tick();
private:
	Camera *camera = nullptr;
	Transform *player = nullptr;
};
}
