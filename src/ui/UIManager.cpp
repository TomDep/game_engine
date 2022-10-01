#include "UIManager.h"

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

// Icons from FontAwesome
#include "IconsFontAwesome.h"	// From... https://github.com/juliettef/IconFontCppHeaders/

void UIManager::init(GLFWwindow* window) {
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 410";

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	
	//Merge in icons from Font Awesome
	//static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	//ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	//io.Fonts->AddFontFromFileTTF("res/fonts/" FONT_ICON_FILE_NAME_FAS, 16.0f, &icons_config, icons_ranges);

	ImGui::StyleColorsDark();

	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };

	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.38f, 0.38f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.28f, 0.28f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };

	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void UIManager::startSimulation() {
	physicsManager->setGravityConstant(newGravity);
	physicsManager->enable();
	
	if (toggleDebug)
		spdlog::debug("[TOGGLE][DEBUG] Starting with a gravity of {}", newGravity);
}

void UIManager::resetSimulation() {
	// Deactivate the physics
	physicsManager->disable();

	std::vector<Entity*>* entities = currentScene->getEntities();

	// We should not be doing that !
	// There should be a init state saved in the scene that is loaded when restarting the scene
	Entity* myCube = entities->at(0);
	myCube->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));

	if (toggleDebug)
		spdlog::debug("[TOGGLE][DEBUG] Initializing the position of the entity at {} on y-axis", myCube->getPosition().y);
}

void UIManager::render() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create the docking environment
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
	ImGui::PopStyleVar(3);
	
	ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Close", "Alt + F4");
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene"))
		{
			ImGui::MenuItem("Open a scene", "Ctr + O");
			ImGui::MenuItem("Save the scene", "Ctr + S");
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	// Add panels
	renderPropertiesPanel();

	ImGui::End();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void UIManager::cleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UIManager::renderPropertiesPanel() {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Properties", nullptr, windowFlags);
		ImGui::Text("Properties");
	ImGui::End();
}

void UIManager::renderPhysicsUI() {
	if (!showWindow) return;

	/* ---------- Components ---------- */

	// Start the Dear ImGui frame

	// UI INTERFACE
	if (showWindow) {
		//ImGui::SetNextWindowSize(ImVec2(300, 220), 0);

		// ImGUI window creation
		ImGui::Begin("Toolbox");

		ImGui::Text("Play with gravity!");
		static float sliderGravity = physicsManager->getGravityConstant();
		ImGui::SliderFloat("###sliderGravity", &sliderGravity, 0.0f, 50.0f);

		if (sliderGravity != newGravity) {
			newGravity = sliderGravity;
			if (toggleDebug)
				spdlog::debug("[TOGGLE][DEBUG] Changed gravity to {}", newGravity);
		}

		if (ImGui::Button("Start###buttonStart", ImVec2(100, 25))) {
			// When I press the button start
			startSimulation();
		}

		if (ImGui::Button("Reset###buttonRestart", ImVec2(100, 25))) {
			// When I press the button restart
			resetSimulation();
		}

		if (ImGui::Button("Reset & Start", ImVec2(100, 25))) {
			resetSimulation();
			startSimulation();
		}

		ImGui::Checkbox("Debug", &toggleDebug);

		// Ends the window
		ImGui::End();
	}

}