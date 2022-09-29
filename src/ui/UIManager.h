#pragma once

//#include <GLFW/glfw3.h>
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
	
	bool showWindow = true;
	bool buttonStart = false;
	bool buttonRestart = false;

	/* ---------- Components ---------- */
	void setPhysicsManager(PhysicsManager* pManager) { physicsManager = pManager; }
	PhysicsManager* getPhysicsManager() const { return physicsManager; }

	void setScene(Scene* scene) { currentScene = scene; }
	Scene* getScene() const { return currentScene; }

private:

	int newWeapon = 0;
	float newGravity = 10.0f;
	string selectedWeaponName = "";

	/* ---------- Components ---------- */
	PhysicsManager* physicsManager = nullptr;
	Scene* currentScene = nullptr;
};

