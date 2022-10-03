#pragma once
#include "Panel.h"
#include "../physics/PhysicsManager.h"

class PhysicsPanel : public Panel {
public:
	PhysicsPanel(PhysicsManager* physicsManager) : physicsManager(physicsManager) {}
	void render();
private:
	void startSimulation();
	void resetSimulation();

	PhysicsManager* physicsManager;
	float newGravity = 9.81f;
};