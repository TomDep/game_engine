#pragma once

#include <assert.h>
#include "cyclone/particle.h"

using namespace cyclone;

void Particle::integrate(real duration) {
	if (inverseMass <= 0.0f) return;

	assert(duration > 0.0);

	// update linear position
	Position.addScaledVector(Velocity, duration);

	//Work out the acceleration from the force
	Vector3 resultingAcc = Acceleration;

	//Update linear velocity from the acceleration
	Velocity.addScaledVector(resultingAcc, duration);

	//Impose drag.
	Velocity *= real_pow(damping, duration);

	//clear the forces
	//clearAccumulator();
}