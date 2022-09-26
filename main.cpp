// Basics
#include <iostream>						// Standard I/O streams
#include <sstream>
#include <stdlib.h>						// Conversion, random and dynamic memory management
#include <stdio.h>						// I/O operations

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

// UI
#include "UIManager.h"

// Logging
#include "spdlog/spdlog.h";

class MainApp {

	const uint32_t WINDOW_WIDTH = 800;
	const uint32_t WINDOW_HEIGHT = 600;
	const int GL_VERSION_MAJOR = 3, GL_VERSION_MINOR = 3;

public:

	void run() {
		initLogger();
		initGLFW();
		initGLEW();
		initOpenGL();
		initUI();

		mainLoop();

		cleanUp();
	}

private:

	/* ---------- VARIABLES ---------- */
	GLFWwindow* window;
	UIManager* uiManager;

	/* ---------- METHODS ---------- */
	void initLogger() {
		spdlog::set_level(spdlog::level::debug); // Set global log level to debug
		spdlog::set_pattern("[%^%l%$] %v");
	}

	void initGLFW() {
		glfwInit();
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Game Engine", nullptr, nullptr);

		// Set all the required options for GLFW
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, false);

		glfwMakeContextCurrent(window);
	}

	void initGLEW() {
		if (glewInit() != GLEW_OK) {
			spdlog::error("Failed to initialize GLEW");
		}
	}

	void initOpenGL() {
		
		// Specify the viewport of OpenGL in the Window
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		// Projection matrix defines the properties of the camera that views the objects in the world coordinate frame. 
		// Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
		glMatrixMode(GL_PROJECTION); 

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	}

	void initUI() {
		uiManager = new UIManager();
		uiManager->init(window);
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window))
		{
			// Events
			glfwPollEvents();

			// Updates

			// Render
			glClear(GL_COLOR_BUFFER_BIT);

			uiManager->render();

			// Swap the back buffer with the front buffer
			glfwSwapBuffers(window);
		}
	}

	void cleanUp() {
		uiManager->cleanUp();

		glfwDestroyWindow(window);
		glfwTerminate();
	}
};

int main()
{
	MainApp app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}