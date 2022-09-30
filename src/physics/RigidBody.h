#pragma once
#include "../gameObjects/Entity.h"

#include <glm/glm.hpp>

class RigidBody {

public:
	RigidBody() {};
	RigidBody(glm::vec3 startingPosition);
	RigidBody(glm::vec3 startingPosition, glm::vec3 startingVelocity);

	void update(float dt, float gravityConstant);

	/* ---------- Getters & Setters ---------- */
	void setDynamic(bool d) { dynamic = d; }
	bool isDynamic() const { return dynamic; }

	glm::vec3 getPosition() const { return position; }
	void setPosition(glm::vec3 newPosition) { position = newPosition; }

	glm::vec3 getVelocity() const { return velocity; }
	glm::vec3 getAcceleration() const { return acceleration; }

	void setAcceleration(glm::vec3 acc) { acceleration = acc; }

private:
	
	// Forces will be applied to a dynamic object
	// A static object can only collision with other RigidBodies but will remain in static position
	bool dynamic = true;

	glm::vec3 position, velocity, acceleration;

};