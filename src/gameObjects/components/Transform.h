#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform : public Component {
public:
	Transform() : position(glm::vec3()), scale(glm::vec3()), rotation(glm::vec3()) {
		updateModelMatrix();
	}

	Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : position(position), scale(scale), rotation(rotation) {
		updateModelMatrix();
	}

	/* ---------- Getters & Setters ---------- */
	glm::vec3 getPosition() const { return position; }
	void setPosition(glm::vec3 newPosition) {
		position = newPosition;
		updateModelMatrix();

		//if (rigidBody != nullptr) rigidBody->setPosition(position);
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

	glm::mat4 getModelMatrix() const { return modelMatrix; }
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
	glm::mat4 modelMatrix = glm::mat4();
};