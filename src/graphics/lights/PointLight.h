#pragma once

#include "Light.h"

class PointLight : public Light {
public:
	PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	glm::vec3 getPosition() const { return position; }
	float getConstant() const { return constant; }
	float getLinear() const { return linear; }
	float getQuadratic() const {return quadratic; }

private:
	glm::vec3 position;
	float constant, linear, quadratic;
};