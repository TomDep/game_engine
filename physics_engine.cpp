// physics_engine.cpp : définit le point d'entrée de l'application.
//

#include "physics_engine.h"
#include "cyclone/core.h"
#include <iostream>

using namespace cyclone;
using namespace std;

int main()
{
	Vector3 v(2,3,1);

	v.invert();
	cout << v << endl;

	v *= 2;
	cout << v << endl;

	Vector3 z = v * -4;
	cout << z << endl;
	return 0;
}
