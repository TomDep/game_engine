#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Entity {
public:
	Entity(glm::vec3 p, glm::vec3 s, glm::vec4 r) {
		position = p;
		scale = s;
		rotation = r;
		
		updateModelMatrix();
	}

	/* ---------- Getters & Setters ---------- */
	void setPosition(glm::vec3 newPosition) {
		position = newPosition;

		updateModelMatrix();
	}

	glm::mat4 getModel() const { return model; }
	glm::vec3 getScale() const { return scale; }
	glm::vec3 getPosition() const { return position; }
private:
	void updateModelMatrix() {
		model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		//model = glm::rotate(model, glm::radians((rotation).w), glm::vec3((rotation).x, (rotation).y, (rotation).z));
	}

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec4 rotation;

	glm::mat4 model;
};

