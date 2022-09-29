#include <spdlog/spdlog.h>
#include "PhysicsManager.h"
#include "RigidBody.h"

RigidBody::RigidBody(glm::vec3 startingPosition) {
	position = startingPosition;
	velocity = glm::vec3(0.0f);
	acceleration = glm::vec3(0.0f);
}

RigidBody::RigidBody(glm::vec3 startingPosition, glm::vec3 startingVelocity) {
	position = startingPosition;
	velocity = startingVelocity;
	acceleration = glm::vec3(0.0f);
}

/* Calculate the velocity and position of the RigidBody */
void RigidBody::update(float dt, float gravityConstant) {
	if (!dynamic) return;

	acceleration = glm::vec3(0.0f,-gravityConstant, 0.0f);

	velocity += acceleration * dt;
	//velocity -= damping * dt;
	position += velocity * dt + acceleration * dt * dt;

	if (position.y < 0) {
		position.y = 0;
		velocity.y = 0;
	}

	//spdlog::debug("POSITION : ({}, {}, {})", position.x, position.y, position.z);
	//spdlog::debug("VELOCITY: ({}, {}, {})", velocity.x, velocity.y, velocity.z);
}