#pragma once

#include <glm/glm.hpp>
#include "Shader.h"

class Light {
public:
	Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void use(Shader* shader);

	/* ---------- Getters & Setters ---------- */
	glm::vec3 getAmbient()  const { return ambient;  }
	glm::vec3 getDiffuse()  const { return diffuse;  }
	glm::vec3 getSpecular() const { return specular; }

private:
	glm::vec3 position;

	// Properties
	glm::vec3 ambient, diffuse, specular;
};