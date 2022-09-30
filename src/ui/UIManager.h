#pragma once

//#include <GLFW/glfw3.h>
#include <string>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../physics/PhysicsManager.h"
#include "EntityTree.h"

using namespace std;

class UIManager {
public:
	UIManager(Scene* scene) : currentScene(scene) {
		/* ----- UI Components ----- */
		entityTree = new EntityTree(currentScene);
	};

	void init(GLFWwindow* window);
	void render();
	void cleanUp();	
	
	/* ---------- Variables ---------- */

	bool showWindow = true;

	/* ---------- Components ---------- */
	void setPhysicsManager(PhysicsManager* pManager) { physicsManager = pManager; }
	PhysicsManager* getPhysicsManager() const { return physicsManager; }

	void setScene(Scene* scene) { currentScene = scene; }
	Scene* getScene() const { return currentScene; }

	void startSimulation();
	void resetSimulation();
private:

	/* ---------- Variables ---------- */

	float newGravity = 10.0f;

	bool toggleDebug = false;

	/* ---------- Components ---------- */
	PhysicsManager* physicsManager = nullptr;
	Scene* currentScene = nullptr;

	/* ---------- UI Components ---------- */
	EntityTree* entityTree;
};

