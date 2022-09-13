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
	v.print();
	//cout << v << endl;
	return 0;
}
