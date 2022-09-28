#pragma once

#include "Texture.h"
#include "Shader.h"

#include <glm/glm.hpp>

class Material {
public:
	Material(Texture* texture, glm::vec3 specular, float shininess);
	void use(Shader* shader);
private:
	Texture* texture;
	glm::vec3 specular;
	float shininess;
};