#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
	Light(ambient, diffuse, specular),
	position(position),
	constant(constant),
	linear(linear),
	quadratic(quadratic) { 

}