#pragma once

#include "LightEmitter.h"

class DirectionalLightEmitter : public LightEmitter {
public:
	DirectionalLightEmitter() {};
	DirectionalLightEmitter(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	glm::vec3 getDirection() const { return direction; }
	void setDirection(glm::vec3 d) { direction = d; }
private:
	glm::vec3 direction;
};