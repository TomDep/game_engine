#include "Entity.h"

Entity::Entity() {
	transform = new Transform();
	id = rand();
}

Entity::Entity(glm::vec3 p, glm::vec3 s, glm::vec3 r) {
	transform = new Transform(p, s, r);
	id = rand();
}