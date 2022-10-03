#include "PointLightEmitter.h"

PointLightEmitter::PointLightEmitter(float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
	LightEmitter(ambient, diffuse, specular),
	constant(constant),
	linear(linear),
	quadratic(quadratic) { 

}