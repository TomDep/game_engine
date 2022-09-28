#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <spdlog/spdlog.h>

class Camera {

public:
	Camera(int width, int height, glm::vec3 pos, glm::vec3 front, glm::vec3 up) : width(width), height(height), pos(pos), front(front), up(up) {};

	glm::mat4 getView() {
		return glm::lookAt(pos, pos + front, up);
	}

	/* Getters & Setters */
	float getSpeed() const { return speed; }
	float getSensitivity() const { return sensitivity; }
	glm::vec3 getFront() const { return front; }
	glm::vec3 getUp() const { return up; }

	glm::vec3 getPosition() const { return pos; }
	void setPosition(glm::vec3 newPosition) { pos = newPosition; }

	float getPitch() const { return pitch; }
	void setPitch(float p) {
		pitch = p;
		updateDirection();
	}

	float getYaw() const { return yaw; }
	void setYaw(float y) {
		yaw = y;
		updateDirection();
	}

	void setPitchAndYaw(float p, float y) {
		pitch = p;
		yaw = y;

		updateDirection();
	}
	
	bool getCanMove() {
		return canMove;
	}
	void setCanMove(bool b) {
		canMove = b;
	}

	int getWidth() const { return width; }
	int getHeight() const { return height; }

private:
	int width, height; // Window size

	glm::vec3 pos, front, up;
	float yaw = 0.0f, pitch = -90.0f;

	float speed = 1.0f;
	float sensitivity = 0.1f;

	bool canMove = true;

	void updateDirection() {
		if (canMove) {
			glm::vec3 direction;

			direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		
			front = glm::normalize(direction);
		}
	}
};