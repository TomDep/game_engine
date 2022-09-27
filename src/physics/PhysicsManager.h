#pragma once
#include "parea/Particle.h"

class PhysicsManager
{
public:
	PhysicsManager();

	void update(float dt);

private:
	Particle* u;
};

