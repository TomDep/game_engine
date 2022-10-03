#pragma once

// Basics
#include <time.h>
#include <iostream>						// Standard I/O streams
#include <sstream>
#include <stdlib.h>						// Conversion, random and dynamic memory management
#include <stdio.h>						// I/O operations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glad/glad.h"					
#include <GLFW/glfw3.h>					// Must be included after glad

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"

using namespace std;

// Logging
#include "spdlog/spdlog.h"

#include "ui/UIManager.h"
#include "graphics/Renderer.h"
#include "graphics/Camera.h"
#include "physics/PhysicsManager.h"
#include "physics/RigidBody.h"
#include "scene/Scene.h"
#include "scene/SceneSerializer.h"

#include "gameObjects/components/Mesh.h"
#include "gameObjects/components/MeshRenderer.h"
#include "gameObjects/components/lights/DirectionalLightEmitter.h"
#include "gameObjects/components/lights/PointLightEmitter.h"

class MainApp {
public:

	static MainApp& getInstance() {
		static MainApp instance;
		return instance;
	}

	/* ---------- Callbacks ---------- */
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		MainApp& app = MainApp::getInstance();

		double xoffset = xpos - app.lastX;
		double yoffset = app.lastY - ypos; // reversed since y-coordinates range from bottom to top
		app.lastX = xpos;
		app.lastY = ypos;

		if (app.uiManager->showWindow == true) return;

		Camera* camera = app.renderer->getCamera();
		const float sensitivity = camera->getSensitivity();
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		float pitch = camera->getPitch();
		float yaw = camera->getYaw();

		pitch += (float) yoffset;
		yaw += (float) xoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		if (pitch > 179.0f)
			pitch = 179.0f;
		if (pitch < -179.0f)
			pitch = -179.0f;

		if (app.firstMouse) // initially set to true
		{
			app.lastX = xpos;
			app.lastY = ypos;
			app.firstMouse = false;

			pitch = 0;
			yaw = -90;
		}

		camera->setPitchAndYaw(pitch, yaw);
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		MainApp& app = MainApp::getInstance();
		if (app.uiManager->showWindow == true) return;

		float fov = app.renderer->getFov();
		fov -= (float)yoffset;

		if (fov < 1.0f)
			app.renderer->setFov(1.0f);
		if (fov > 45.0f)
			app.renderer->setFov(45.0f);		
		
		app.renderer->setFov(fov);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{			
		MainApp& app = MainApp::getInstance();

		// Shortcuts
		//if (key == GLFW_KEY_P && action == GLFW_PRESS) app.uiManager->startSimulation();
		//if (key == GLFW_KEY_R && action == GLFW_PRESS) app.uiManager->resetSimulation();

		if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
			Camera* cam = app.renderer->getCamera();

			if (app.uiManager->showWindow == true) {
				app.uiManager->showWindow = false;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else {
				app.uiManager->showWindow = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		MainApp& app = MainApp::getInstance();

		app.WINDOW_WIDTH = width;
		app.WINDOW_HEIGHT = height;
	}

	const int GL_VERSION_MAJOR = 3, GL_VERSION_MINOR = 3;

public:

	void run() {
		initLogger();
		initGLFW();
		initOpenGL();
		initRenderer();
		initPhysics();
		initScene();
		initUI();

		spdlog::info("Initialization complete !");

		mainLoop();

		cleanUp();
	}

	/* ---------- VARIABLES ---------- */
	GLFWwindow* window;
	UIManager* uiManager;
	PhysicsManager* physicsManager;
	Scene* scene;

	int WINDOW_WIDTH = 1080;
	int WINDOW_HEIGHT = 720;
	
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	
	Renderer* renderer;

	double lastX = (double)WINDOW_WIDTH / 2.0f, lastY = (double)WINDOW_HEIGHT / 2.0f;
	bool firstMouse;

private:
	MainApp(void) {}				// private constructor necessary to allow only 1 instance
	MainApp(MainApp const&);		// prevent copies
	void operator=(MainApp const&); // prevent assignments

	/* ---------- METHODS ---------- */
	void initLogger() {
		spdlog::set_level(spdlog::level::debug); // Set global log level to debug
		spdlog::set_pattern("[%^%l%$] %v");
	}

	void initGLFW() {
		glfwInit();

		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Game Engine", nullptr, nullptr);
		glfwMaximizeWindow(window);
		glfwGetWindowSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);

		// Set all the required options for GLFW
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, true);
		glfwSwapInterval(1); // Enable vsync

		glfwMakeContextCurrent(window);

		firstMouse = true;
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetKeyCallback(window, key_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetWindowSizeCallback(window, framebuffer_size_callback);
	}

	void initOpenGL() {
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::runtime_error("Failed to initialize GLAD");
		}

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glEnable(GL_DEPTH_TEST);
	}

	void initUI() {
		uiManager = new UIManager(scene, physicsManager);
		uiManager->init(window);
	}

	void initRenderer() {
		spdlog::debug("[INITIALIZATION][RENDERER] Start");
		glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 7.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		Camera * camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, cameraPos, cameraFront, cameraUp);
		renderer = new Renderer(camera);

		spdlog::debug("[INITIALIZATION][RENDERER] Done");
	}

	void initPhysics() {
		physicsManager = new PhysicsManager();
	}

	void initScene() {
		spdlog::debug("[INITIALIZATION][SCENE] Start");
		scene = new Scene();

		spdlog::debug("[INITIALIZATION][SCENE] Deserializing the scene");
		//SceneSerializer sceneSerializer(scene);
		//sceneSerializer.deserialize("res/scenes/testScene.yaml");

		Entity* sun = new Entity();
		sun->setName("Sun");
		DirectionalLightEmitter* dirSun = new DirectionalLightEmitter(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f));
		sun->addDirectionalLightEmitter(dirSun);
		scene->addEntity(sun);

		Entity* light = new Entity();
		light->setName("Light");
		light->getTransform()->setPosition(glm::vec3(2.0, 2.0, -2.0));
		PointLightEmitter* emitter = new PointLightEmitter(
			1.0f, 0.09f, 0.032f,
			glm::vec3(0.5f, 0.0f, 0.0f),
			glm::vec3(0.8f, 0.0f, 0.5f),
			glm::vec3(1.0f));
		light->addPointLightEmitter(emitter);
		scene->addEntity(light);

		Entity* cubeA = new Entity(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
		cubeA->addMesh(new Mesh());
		cubeA->addMeshRenderer(new MeshRenderer());
		cubeA->setName("Cube");
		
		Entity* cubeA_1 = new Entity(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
		Entity* cubeA_1_1 = new Entity(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
		Entity* cubeA_2 = new Entity(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
		Entity* cubeA_3 = new Entity(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));

		//cubeA_1->addChild(cubeA_1_1);
		//cubeA->addChild(cubeA_1);
		//cubeA->addChild(cubeA_2);
		//cubeA->addChild(cubeA_3);

		scene->addEntity(cubeA);
		physicsManager->setCurrentScene(scene);
		renderer->setCurrentScene(scene);

		spdlog::debug("[INITIALIZATION][SCENE] Done");
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window))
		{
			// Updates
			float currentFrame = (float) glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			physicsManager->update(deltaTime);

			// Events
			processInput();

			// Render
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			uiManager->render();
			renderer->render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Swap the back buffer with the front buffer
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void processInput()
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		// Camera movement
		Camera* camera = renderer->getCamera();

		glm::vec3 cameraPos = camera->getPosition();
		float cameraSpeed = camera->getSpeed() * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * camera->getFront();
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * camera->getFront();
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(camera->getFront(), camera->getUp())) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(camera->getFront(), camera->getUp())) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			cameraPos += cameraSpeed * camera->getUp();
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			cameraPos -= cameraSpeed * camera->getUp();

		camera->setPosition(cameraPos);

	}

	void cleanUp() {
		uiManager->cleanUp();

		glfwDestroyWindow(window);
		glfwTerminate();
	}
};

int main()
{
	srand(time(NULL));
	MainApp& app = MainApp::getInstance();

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}