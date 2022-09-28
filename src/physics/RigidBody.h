#pragma once
#include "../gameObjects/Entity.h"

#include <glm/glm.hpp>

class RigidBody {

public:
	RigidBody(glm::vec3 startingPosition);
	RigidBody(glm::vec3 startingPosition, glm::vec3 startingVelocity);

	void update(float dt);

	/* ---------- Getters & Setters ---------- */
	void setStatic() { dynamic = false; }
	void setDynamic() { dynamic = true; }

	glm::vec3 getPosition() const { return position; }
	glm::vec3 getVelocity() const { return velocity; }
	glm::vec3 getAcceleration() const { return acceleration; }
private:
	
	// Forces will be applied to a dynamic object
	// A static object can only collision with other RigidBodies but will remain in static position
	bool dynamic = true;

	glm::vec3 position, velocity, acceleration;

};