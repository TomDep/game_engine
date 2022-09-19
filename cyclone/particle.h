#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "precision.h"
#include "core.h"
//#include <SFML/Graphics.hpp>

namespace cyclone {
	class Particle {
	protected:
		Vector3 Position;
		Vector3 Velocity;
		Vector3 Acceleration;

		real damping;
		real inverseMass;

	public:
		Particle(Vector3 position, Vector3 velocity) {
			Position.x = position.x;
			Position.y = position.y;
			Position.z = position.z;
			
			Velocity.x = velocity.x;
			Velocity.y = velocity.y;
			//Velocity.z = velocity.z;
		};

		void update(real time) {
			//Position += Velocity * time + Acceleration * time * time * 0.5;
			Position.addScaledVector(Velocity, time);
			Position.addScaledVector(Acceleration, time * time * 0.5);

		};

		void setPosition(Vector3 position) {
			Position = position;
		};

		Vector3 getPosition() {
			return Position;
		};

		Vector3 getAcceleration() {
			return Acceleration;
		};

		void setAcceleration(Vector3 acceleration) {
			Acceleration = acceleration;
		};

		void integrate(real duration);

	};

	/*class ParticleSystem : public Drawable {

	};*/
}
