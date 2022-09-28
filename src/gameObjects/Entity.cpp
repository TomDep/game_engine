#include "Entity.h"

Entity::Entity(glm::vec3 p, glm::vec3 s, glm::vec3 r) : position(p), scale(s), rotation(r) {
	updateModelMatrix();
}