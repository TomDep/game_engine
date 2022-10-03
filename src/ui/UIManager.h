#pragma once

//#include <GLFW/glfw3.h>
#include <string>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../physics/PhysicsManager.h"
#include "PhysicsPanel.h"
#include "EntityTree.h"
#include "MainMenuBar.h"
#include "Inspector.h"
#include "Panel.h"

using namespace std;

class UIManager {
public:
	/* ---------- Variables ---------- */
	bool showWindow = true;

	/* ---------- Methods ---------- */
	UIManager(Scene* scene, PhysicsManager* physicsManager);

	void init(GLFWwindow* window);
	void render();
	void cleanUp();	
private:
	void preRender();
	void postRender();
	/* ---------- Variables ---------- */
	PhysicsManager* physicsManager = nullptr;
	Scene* currentScene = nullptr;

	// Panels
	PhysicsPanel* physicsPanel;
	EntityTree* entityTree;
	MainMenuBar* mainMenuBar;
	Inspector* inspector;
};

