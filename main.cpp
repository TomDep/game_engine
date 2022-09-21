/********* Libraries ************/

// Basics
#include <iostream>						// Standard I/O streams
using namespace std;

#include <stdlib.h>						// Conversion, random and dynamic memory management
#include <stdio.h>						// I/O operations

// Our game engine
#include <parea/core.h>
#include <parea/particle.h>
using namespace parea;

// Graphic
#define GLFW_INCLUDE_NONE				//To make sure there will be no header conflicts
#include <GLFW/glfw3.h>
#include <glad/glad.h>

// UI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

/********* General variables ************/

// Window dimensions
const int WIDTH = 800, HEIGHT = 600;

const bool printing_information = true;
enum STATE {TRACE, DEBUG, INFO, WARN, ERROR, FATAL};

/********* Functions ************/

void print(STATE state, const char* message) {
	if (printing_information) {
		switch (state){
			case TRACE:
				cout << "TRACE : "; break;
			case DEBUG:
				cout << "DEBUG : "; break;
			case INFO:
				cout << "INFO : "; break;
			case ERROR:
				cout << "ERROR : "; break;
			case WARN:
				cout << "WARN : "; break;
			case FATAL:
				cout << "FATAL : "; break;
		}
		cout << message << endl;
	}
}

// Setting an error callback
void error_callback(int error, const char* description){
	fprintf(stderr, "Error: %s\n", description);
}

// Receiving input events
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	cout << key << " which is " << (char)key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

/********* Main ************/
int main()
{	
	print(INFO, "Loading librairies");

	// GLFW Initialization
	if (!glfwInit())	
		exit(EXIT_FAILURE);

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Creating a window and context
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Phase 1 - Experimentation", NULL, NULL);
	print(INFO, "Creating window");

	// Checking context
	if (!window && !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		// Window or OpenGL context creation failed
		// GLFW Terminating
		print(FATAL, "Window or OpenGL context creation failed");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Making the OpenGL context current
	glfwMakeContextCurrent(window);

	// Setting an error callback
	glfwSetErrorCallback(error_callback);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// GL Initialization
	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);


	// ImGUI Initialization 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	print(INFO, "Starting game loop");

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);

		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGUI window creation
		ImGui::Begin("My name is window, ImGUI window");
		// Text that appears in the window
		ImGui::Text("Hello there stranger!");
		// Ends the window
		ImGui::End();

		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	print(INFO, "Done!");

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	exit(EXIT_SUCCESS);
}


/*

First test - particle movement


	Vector3 p(1, 2, 3);
	Vector3 v(1, -1, 2);

	// Particle
	Particle u(p, v);
	cout << "Nous sommes la : " << u.getPosition() << endl;

	u.setAcceleration(Vector3(0, 1, -1));

	u.update(10);

	cout << "Données de la particule : " << u.getPosition() << endl;

*/