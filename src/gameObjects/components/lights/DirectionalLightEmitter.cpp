#include "DirectionalLightEmitter.h"

DirectionalLightEmitter::DirectionalLightEmitter(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
	LightEmitter(ambient, diffuse, specular),
	direction(direction) {

}