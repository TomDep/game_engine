#include <iostream>
using namespace std;

#include <parea/core.h>
#include <parea/particle.h>
using namespace parea;

/*#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>*/

int main()
{	
	Vector3 p(1, 2, 3);
	Vector3 v(1, -1, 2);

	// Particle
	Particle u(p, v);
	cout << "nous sommes la : " << u.getPosition() << endl;

	u.setAcceleration(Vector3(0, 1, -1));

	u.update(10);

	cout << "donnees de la particule : " << u.getPosition() << endl;

	/**************************************************************************************/

	exit(EXIT_SUCCESS);
}
