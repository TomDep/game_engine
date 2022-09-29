#pragma once
#include "../gameObjects/Scene.h"

// Defining physics constants
const static float EARTH_GRAVITY = 9.81f;

class PhysicsManager
{
public:

	static float GRAVITY_CONSTANT;

	PhysicsManager();
	void update(float dt);

	/* ---------- Getters & Setters ---------- */
	void setCurrentScene(Scene* scene) { currentScene = scene; }

	void setGravityConstant(float newGravityConstant) { gravityConstant = newGravityConstant; }
	float getGravityConstant() const { return gravityConstant; }

	void enable() { enabled = true; }
	void disable() { enabled = false; }

private:
	bool enabled = true;
	float gravityConstant = EARTH_GRAVITY;

	Scene* currentScene = nullptr;

};

