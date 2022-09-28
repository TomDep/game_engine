#pragma once
#include <vector>

#include "Entity.h"
#include "../graphics/lights/DirectionalLight.h"
#include "../graphics/lights/PointLight.h"

#include <spdlog/spdlog.h>

#define MAX_POINT_LIGHT 4

class Scene {
public:
	Scene() {}

	/* ---------- Getters & Setters ---------- */
	std::vector<Entity*> getEntities() const { return entities; }

	void addEntity(Entity* entity) {
		entities.push_back(entity);
	}

	void setDirectionalLight(DirectionalLight* light) { directionalLight = light; }
	DirectionalLight* getDirectionalLight() const { return directionalLight; }

	void addPointLight(PointLight* light) {
		if (nbOfPointLights >= MAX_POINT_LIGHT) {
			spdlog::warn("Couldn't add a new point light. Maximum number already reached : {}", MAX_POINT_LIGHT);
			return;
		}

		pointLights[nbOfPointLights] = light;
		nbOfPointLights++;
	}

	PointLight** getPointLights() const { return pointLights; }
	int getNumberOfPointLight() const { return nbOfPointLights; }

private:
	std::vector<Entity*> entities;

	DirectionalLight* directionalLight = nullptr;

	int nbOfPointLights = 0;
	PointLight** pointLights = (PointLight**) malloc(MAX_POINT_LIGHT * sizeof(PointLight*));
};

