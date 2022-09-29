#include "PhysicsManager.h"
#include <spdlog/spdlog.h>

#include "RigidBody.h"

// Defining physics constants
const static float earthGravity = 9.81f;

PhysicsManager::PhysicsManager() {
	setGravity(earthGravity);
}

void PhysicsManager::update(float dt) {
	
	std::vector<Entity*> entities = currentScene->getEntities();
	for (Entity* entity : entities) {
		RigidBody* rigidBody = entity->getRigidBody();
		if (rigidBody != nullptr) {
			rigidBody->update(dt);
			entity->setPosition(rigidBody->getPosition());
		}
	}
}