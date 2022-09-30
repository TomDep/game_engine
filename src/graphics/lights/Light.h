#pragma once

#include <glm/glm.hpp>
#include "../Shader.h"

class Light {
public:
	Light() {};
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	/* ---------- Getters & Setters ---------- */
	glm::vec3 getAmbient()  const { return ambient;  }
	void setAmbient(glm::vec3 a) { ambient = a; }

	glm::vec3 getDiffuse()  const { return diffuse;  }
	void setDiffuse(glm::vec3 d) { diffuse = d; }

	glm::vec3 getSpecular() const { return specular; }
	void setSpecular(glm::vec3 s) { specular = s; }

	int getId() const { return 865313516; }
private:
	// Properties
	glm::vec3 ambient, diffuse, specular;
};