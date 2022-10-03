#include "PhysicsManager.h"
#include <spdlog/spdlog.h>

#include "RigidBody.h"

PhysicsManager::PhysicsManager() {
}

void PhysicsManager::update(float dt) {
	std::vector<Entity*>* entities = currentScene->getEntities();
	for (Entity* entity : *entities) {
		glm::vec3 pos = entity->getTransform()->getPosition();
		RigidBody* rigidBody = entity->getRigidBody();
		if (rigidBody != nullptr) {
			if (enabled) {
				rigidBody->update(dt, gravityConstant);
				entity->getTransform()->setPosition(rigidBody->getPosition());
			}
		}
	}
}