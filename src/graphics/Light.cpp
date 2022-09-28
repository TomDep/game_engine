#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
	position(position),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular) {
}

void Light::use(Shader* shader) {
	shader->use();

	shader->setVector3("light.position", position);

	// light properties
	shader->setVector3("light.ambient", ambient);
	shader->setVector3("light.diffuse", diffuse);
	shader->setVector3("light.specular", specular);
}