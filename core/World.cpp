#include "CorePCH.h"
#include "World.h"
#include "Transform.h"

mj::World::World() {
	player = new mj::Transform();
}

mj::World::~World() {
}

void mj::World::Tick() {
	// Update player position

	// Draw voxels
}
