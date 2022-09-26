#pragma once

#include <GLFW/glfw3.h>
#include <string>

using namespace std;

class UIManager {
public:
	UIManager() {};

	void init(GLFWwindow* window);
	void render();
	void cleanUp();
private:

	int newWeapon = 0;
	float newGravity = 10.0f;
	string selectedWeaponName = "";
};

