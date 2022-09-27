#pragma once

#include <GLFW/glfw3.h>
#include <string>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


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

