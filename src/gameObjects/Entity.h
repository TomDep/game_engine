#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../physics/RigidBody.h"
#include <string>

class Entity {
public:
	Entity(glm::vec3 p, glm::vec3 s, glm::vec3 r);

	/* ---------- Getters & Setters ---------- */
	glm::mat4 getModelMatrix() const { return modelMatrix; }

	glm::vec3 getPosition() const { return position; }
	void setPosition(glm::vec3 newPosition) {
		position = newPosition;
		updateModelMatrix();
	}

	glm::vec3 getScale() const { return scale; }
	void setScale(glm::vec3 newScale) {
		scale = newScale;
		updateModelMatrix();
	}

	glm::vec3 getRotation() const { return rotation; }
	void setRotation(glm::vec3 newRotation) {
		rotation = newRotation;
		updateModelMatrix();
	}

	/* ---------- Components ---------- */
	void addRigidBody(RigidBody* rb) { rigidBody = rb; }
	RigidBody* getRigidBody() const { return rigidBody; }
private:
	void updateModelMatrix() {
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, scale);
		
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	glm::vec3 position, scale, rotation;
	glm::mat4 modelMatrix;

	/* Components */
	RigidBody* rigidBody = nullptr;
};

