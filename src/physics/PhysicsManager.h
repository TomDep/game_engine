#pragma once
#include "../gameObjects/Scene.h"

class PhysicsManager
{
public:
	PhysicsManager();

	void update(float dt);

	/* ---------- Getters & Setters ---------- */
	void setCurrentScene(Scene* scene) { currentScene = scene; }

	float getGravity() { return gravity; }
	void setGravity(float g) { gravity = g; }

private:
	float gravity;
	
	Scene* currentScene = nullptr;

};

