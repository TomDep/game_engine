#pragma once
#include <vector>
#include "Entity.h"

class Scene {
public:
	Scene() {}

	/* ---------- Getters & Setters ---------- */
	std::vector<Entity*> getEntities() const { return entities; }

	void addEntity(Entity* entity) {
		entities.push_back(entity);
	}

private:
	std::vector<Entity*> entities;
};

