// physics_engine.cpp : définit le point d'entrée de l'application.
//

#include "physics_engine.h"
#include "cyclone/core.h"
#include <iostream>
#include "cyclone/particle.h"

using namespace cyclone;
using namespace std;

int main()
{
	Vector3 p(1,2,3);
	Vector3 v(1, -1, 2);

	// Particle
	Particle u(p, v);
	cout << "nous sommes la : " << u.getPosition() << endl;

	u.setAcceleration(Vector3(0, 1, -1));

	u.update(10);

	cout << "donnees de la particule : " << u.getPosition() << endl;
	return 0;
}
