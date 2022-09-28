#pragma once
#include "../gameObjects/Scene.h"

class PhysicsManager
{
public:
	static const float G;

	PhysicsManager();

	void update(float dt);

	/* ---------- Getters & Setters ---------- */
	void setCurrentScene(Scene* scene) { currentScene = scene; }

private:
	Scene* currentScene = nullptr;
};

