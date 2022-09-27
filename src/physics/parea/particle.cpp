#pragma once

#include "Particle.h"




/*******************CONSTRUCTORS*******************/
Particle::Particle()
{
	position = Vector3(0.0, 0.0, 0.0);
	velocity = Vector3(0.0, 0.0, 0.0);
	acceleration = Vector3(0.0f, 0.0f, -G);
	//hard values for testing
	inverseMass = 1.0f;
	damping = 1.0f;
}

Particle::Particle(Vector3 p, Vector3 v)
{
	position = p;
	velocity = v;
	acceleration = Vector3(0.0f, 0.0f, -G);
	//hard values for testing
	inverseMass = 1.0f;
	damping = 1.0f;
}


void Particle::integrate(float dt) {
	//Can not integrate object with infinite mass
	if (inverseMass <= 0.0f) return;

	velocity = velocity + acceleration * dt;
	//velocity -= damping * dt;
	position = position + velocity * dt + acceleration * dt * dt;

	if (position.z < 0)
		position.z = 0;

}

void Particle::addForce(Vector3 force) {
	velocity += force;
}