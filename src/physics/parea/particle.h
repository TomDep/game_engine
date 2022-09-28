#pragma once

#include "precision.h"
#include "Vector3.h"


class Particle {
protected:
	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;

	float damping;
	float inverseMass;

	float G = 9.81;

public:
	Particle();
	Particle(Vector3 position, Vector3 velocity);

	void setPosition(Vector3 position) {
		position = position;
	};

	Vector3 getPosition() {
		return position;
	};

	Vector3 getAcceleration() {
		return acceleration;
	};

	void setAcceleration(Vector3 acceleration) {
		acceleration = acceleration;
	};
	void integrate(float dt);

	void addForce(Vector3 force);
	
};
