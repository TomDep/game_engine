#pragma once
#include <vector>

#include "../gameObjects/Entity.h"

#include <spdlog/spdlog.h>

class Scene {
public:
	Scene() {}

	/* ---------- Getters & Setters ---------- */
	std::vector<Entity*>* getEntities() const { return entities; }
	void addEntity(Entity* entity) {
		entities->push_back(entity);
	}

private:
	std::vector<Entity*>* entities = new std::vector<Entity*>();
};

