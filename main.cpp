/********************************* Libraries ************************************/

// Basics

#include <iostream>						// Standard I/O streams
#include <sstream>
#include <stdlib.h>						// Conversion, random and dynamic memory management
#include <stdio.h>						// I/O operations


//#define GLEW_STATIC
#include <GL/glew.h>
//#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Our game engine
// 
//#include <parea/core.h>
//#include <parea/particle.h>

//#include <shaders/shader.h>

// UI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// Icons from FontAwesome
#include <icons/IconsFontAwesome.h>		// From... https://github.com/juliettef/IconFontCppHeaders/

using namespace std;
//using namespace parea;

/********************************* General variables ************************************/

const bool printing_information = true;
enum STATE {TRACE, DEBUG, INFO, WARN, ERROR, FATAL};

/********************************* Functions ************************************/

template <typename T>
string to_string(T val)
{
	std::ostringstream stream;
	stream << val;
	return stream.str();
}

void print(STATE state, string message) {
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
void error_callback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

// Receiving input events
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	cout << key << " which is " << (char)key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	glfwGetCursorPos(window, &xpos, &ypos);
	print(DEBUG, "Mouse position : (x=" + to_string(xpos) + ";y=" + to_string(ypos) + ")");
}

GLFWwindow* createWindow(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizable) {
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);

	// Setting an error callback
	//glfwSetErrorCallback(error_callback);

	// Set the required callback functions
	//glfwSetKeyCallback(window, key_callback);

	// Set the position callback funtions to show mouse position
	// glfwSetCursorPosCallback(window, cursor_position_callback);
	
	// Making the OpenGL context current
	glfwMakeContextCurrent(window);

	return window;
}

/********************************* Main ************************************/
int main()
{	
	print(INFO, "Loading librairies");

	/********************************* GLFW and glad ************************************/

	const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;
	const int GL_VERSION_MAJOR = 3, GL_VERSION_MINOR = 3;
	const bool resizable = true;

	if (!glfwInit())	
		return(-1);
	
	// Creating a window and context	
	print(INFO, "Creating window");
	GLFWwindow* window = createWindow("Phase 1 - Experimentation", WINDOW_WIDTH, WINDOW_HEIGHT, GL_VERSION_MAJOR, GL_VERSION_MINOR, resizable);
	
	// Checking context
	if (!window) {
		// GLFW Terminating
		print(FATAL, "Window or OpenGL context creation failed");
		glfwTerminate();
		return(-1);
	}

	print(INFO, "Loading glew");
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		print(FATAL, "Failed to initialize GLEW");
		glfwTerminate();
		return(-1);
	}

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
	glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, 0, 1); // essentially set coordinate system
	glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
	glLoadIdentity(); // same as above comment

	/********************************* ImGUI ************************************/

	// ImGUI Initialization
	print(INFO, "Initializating ImGui");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();

	//Merge in icons from Font Awesome
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF("Librairies/resources/icons/fonts/" FONT_ICON_FILE_NAME_FAS, 16.0f, &icons_config, icons_ranges);

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	/********************************* GAME INFORMATION ************************************/

	print(INFO, "----------------------------------  STARTING GAME LOOP  ----------------------------------------");
	int newWeapon = 0;
	float newGravity = 10.0f;
	string selectedWeaponName = "";

	GLfloat pointVertex[] = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
	GLfloat pointVertex2[] = { WINDOW_WIDTH * 0.75, WINDOW_HEIGHT / 2 };

	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		// glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		// Render OpenGL here
		glEnable(GL_POINT_SMOOTH); // make the point circular
		glEnableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're using a vertex array for fixed-function attribute
		glPointSize(100); // must be added before glDrawArrays is called
		glVertexPointer(2, GL_FLOAT, 0, pointVertex); // point to the vertices to be used
		glDrawArrays(GL_POINTS, 0, 1); // draw the vertixes
		glDisableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're finished using the vertex arrayattribute
		glDisable(GL_POINT_SMOOTH); // stop the smoothing to make the points circular

		glEnableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're using a vertex array for fixed-function attribute
		glVertexPointer(2, GL_FLOAT, 0, pointVertex2); // point to the vertices to be used
		glPointSize(10); // must be added before glDrawArrays is called
		glDrawArrays(GL_POINTS, 0, 1); // draw the vertixes
		glDisableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're finished using the vertex arrayattribute


		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// UI INTERFACE
		{
			ImGui::SetNextWindowSize(ImVec2(400,220), 0);

			// ImGUI window creation
			ImGui::Begin("Toolbox");
			// Text that appears in the window
			ImGui::Text("Welcome there stranger!");
			ImGui::Text("Through this interface, you gonna be able to make some\nphysic experiment in this whole new virtual world.");
			
			ImGui::Separator();

			ImGui::Text("Select your weapon");
			static const char* weapons[]{ ICON_FA_GUN "  Pistol", ICON_FA_BOW_ARROW "  Artillery",  ICON_FA_FIRE_ALT "  Fireball", ICON_FA_RAYGUN "  Laser"};
			static int selectedWeapon = 0;
			ImGui::Combo("###selectWeapons", &selectedWeapon, weapons, IM_ARRAYSIZE(weapons));

			if (selectedWeapon != newWeapon) {
				newWeapon = selectedWeapon;
				selectedWeaponName = &weapons[selectedWeapon][5];
				print(DEBUG, "Hey! Just got another weapon " + selectedWeaponName);
			}

			ImGui::Text("\nPlay with gravity!");
			static float sliderGravity = 10.0f;
			ImGui::SliderFloat("###sliderGravity", &sliderGravity, 0.0f, 20.0f);

			if (sliderGravity != newGravity) {
				newGravity = sliderGravity;
				print(DEBUG, "Hey! Just changed gravity to " + to_string(newGravity));
			}

			if (ImGui::Button("Launch###buttonStart", ImVec2(100, 25))) {
				print(DEBUG, "Launching the " + selectedWeaponName + " with a gravity of " + to_string(newGravity));
			}

			// Ends the window
			ImGui::End();
		}

		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);				
		// Take care of all GLFW events
		glfwPollEvents();						
	}
	/********************************* END GAME LOOP ************************************/

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return(-1);
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