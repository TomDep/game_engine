#pragma once

#include "Light.h"

class PointLight : public Light {
public:
	PointLight() {};
	PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	glm::vec3 getPosition() const { return position; }
	void setPosition(glm::vec3 p) { position = p; }

	float getConstant() const { return constant; }
	void setConstant(float c) { constant = c; }

	float getLinear() const { return linear; }
	void setLinear(float l) { linear = l; }

	float getQuadratic() const {return quadratic; }
	void setQuadratic(float q) { quadratic = q; }
private:
	glm::vec3 position;
	float constant, linear, quadratic;
};