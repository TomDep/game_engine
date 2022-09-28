#include "PhysicsManager.h"
#include "parea/Particle.h"
#include "parea/core.h"
#include <spdlog/spdlog.h>



PhysicsManager::PhysicsManager() {

	u = new Particle(Vector3(0,0,0), Vector3(0, 0, 15));

}

void PhysicsManager::update(float dt) {
	u->integrate(dt);
	Vector3 pos = u->getPosition();
	spdlog::debug("Position : ({}, {}, {})", pos.x, pos.y, pos.z);
}