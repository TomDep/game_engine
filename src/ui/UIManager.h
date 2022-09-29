#pragma once

#include <GLFW/glfw3.h>
#include <string>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../physics/PhysicsManager.h"

using namespace std;

class UIManager {
public:
	UIManager() {};

	void init(GLFWwindow* window);
	void render();
	void cleanUp();	
	
	bool showWindow = false;

	/* ---------- Components ---------- */
	void setPhysicsManager(PhysicsManager* pManager) { physicsManager = pManager; }
	PhysicsManager* getPhysicsManager() const { return physicsManager; }

private:

	int newWeapon = 0;
	float newGravity = 10.0f;
	string selectedWeaponName = "";

	/* ---------- Components ---------- */
	PhysicsManager* physicsManager = nullptr;
};

