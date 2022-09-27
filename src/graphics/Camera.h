#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {

public:
	Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up) : pos(pos), front(front), up(up) {};

	glm::mat4 getView() {
		return glm::lookAt(pos, pos + front, up);
	}

	/* Getters & Setters */
	float getSpeed() const { return speed; }
	glm::vec3 getFront() const { return front; }
	glm::vec3 getUp() const { return up; }

	glm::vec3 getPosition() const { return pos; }
	void setPosition(glm::vec3 newPosition) { pos = newPosition; }
	
private:
	glm::vec3 pos, front, up;
	float speed = 1.0f;
};