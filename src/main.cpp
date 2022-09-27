// Basics
#include <iostream>						// Standard I/O streams
#include <sstream>
#include <stdlib.h>						// Conversion, random and dynamic memory management
#include <stdio.h>						// I/O operations

#include "glad/glad.h"					
#include <GLFW/glfw3.h>					// Must be included after glad

using namespace std;

// UI
#include "UIManager.h"

// Logging
#include "spdlog/spdlog.h"
#include "graphics/Renderer.h"

class MainApp {

	const uint32_t WINDOW_WIDTH = 800;
	const uint32_t WINDOW_HEIGHT = 600;
	const int GL_VERSION_MAJOR = 3, GL_VERSION_MINOR = 3;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

public:

	void run() {
		initLogger();
		initGLFW();
		initGLAD();
		initOpenGL();
		initRenderer();
		initUI();

		mainLoop();

		cleanUp();
	}

private:

	/* ---------- VARIABLES ---------- */
	GLFWwindow* window;
	UIManager* uiManager;
	Renderer* renderer;

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
		glfwWindowHint(GLFW_RESIZABLE, true);

		glfwMakeContextCurrent(window);
	}

	void initGLAD() {
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::runtime_error("Failed to initialize GLAD");
		}

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glEnable(GL_DEPTH_TEST);
	}

	void initUI() {
		uiManager = new UIManager();
		uiManager->init(window);
	}

	void initRenderer() {
		renderer = new Renderer();
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window))
		{
			// Events
			processInput();

			// Updates

			// Render
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			renderer->render();
			uiManager->render();

			// Swap the back buffer with the front buffer
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void processInput()
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
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