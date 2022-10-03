#pragma once

#include "LightEmitter.h"

class PointLightEmitter : public LightEmitter {
public:
	PointLightEmitter() {};
	PointLightEmitter(float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	float getConstant() const { return constant; }
	void setConstant(float c) { constant = c; }

	float getLinear() const { return linear; }
	void setLinear(float l) { linear = l; }

	float getQuadratic() const {return quadratic; }
	void setQuadratic(float q) { quadratic = q; }
private:
	float constant, linear, quadratic;
};