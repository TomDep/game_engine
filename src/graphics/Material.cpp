#include "Material.h"

Material::Material(Texture* texture, glm::vec3 specular, float shininess) :
	texture(texture),
	specular(specular),
	shininess(shininess) {
}

void Material::use(Shader* shader) {
	shader->use();
	texture->use();

	shader->setInt("material.diffuse", 0);
	shader->setVector3("material.specular", specular);
	shader->setFloat("material.shininess", shininess);
}