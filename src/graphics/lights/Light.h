#pragma once

#include <glm/glm.hpp>
#include "../Shader.h"

class Light {
public:
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	/* ---------- Getters & Setters ---------- */
	glm::vec3 getAmbient()  const { return ambient;  }
	glm::vec3 getDiffuse()  const { return diffuse;  }
	glm::vec3 getSpecular() const { return specular; }

private:
	// Properties
	glm::vec3 ambient, diffuse, specular;
};