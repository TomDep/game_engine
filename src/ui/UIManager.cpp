#include "UIManager.h"

#include <spdlog/spdlog.h>

// Icons from FontAwesome
#include "IconsFontAwesome.h"	// From... https://github.com/juliettef/IconFontCppHeaders/

void UIManager::init(GLFWwindow* window) {
	// ImGUI Initialization
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	
	//Merge in icons from Font Awesome
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF("res/fonts/" FONT_ICON_FILE_NAME_FAS, 16.0f, &icons_config, icons_ranges);

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void UIManager::startSimulation() {
	spdlog::debug("Launching the {} with a gravity of {}", selectedWeaponName, newGravity);

	physicsManager->setGravityConstant(newGravity);
	physicsManager->enable();
}

void UIManager::resetSimulation() {
	// Deactivate the physics
	physicsManager->disable();

	std::vector<Entity*>* entities = currentScene->getEntities();

	// We should not be doing that !
	// There should be a init state saved in the scene that is loaded when restarting the scene
	Entity* myCube = entities->at(0);
	myCube->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
}

void UIManager::render() {
	PhysicsManager* pManager = getPhysicsManager();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// UI INTERFACE
	if(showWindow){
		ImGui::SetNextWindowSize(ImVec2(400, 220), 0);

		// ImGUI window creation
		ImGui::Begin("Toolbox");
		// Text that appears in the window
		ImGui::Text("Welcome there stranger!");
		ImGui::Text("Through this interface, you gonna be able to make some\nphysic experiment in this whole new virtual world.");

		ImGui::Separator();

		ImGui::Text("Select your weapon");
		static const char* weapons[]{ ICON_FA_GUN "  Pistol", ICON_FA_BOW_ARROW "  Artillery",  ICON_FA_FIRE_ALT "  Fireball", ICON_FA_RAYGUN "  Laser" };
		static int selectedWeapon = 0;
		ImGui::Combo("###selectWeapons", &selectedWeapon, weapons, IM_ARRAYSIZE(weapons));

		if (selectedWeapon != newWeapon) {
			newWeapon = selectedWeapon;
			selectedWeaponName = &weapons[selectedWeapon][5];
			spdlog::debug("Hey! Just got another weapon {}", selectedWeaponName);
		}

		ImGui::Text("\nPlay with gravity!");
		static float sliderGravity = physicsManager->getGravityConstant();
		ImGui::SliderFloat("###sliderGravity", &sliderGravity, 0.0f, 50.0f);

		if (sliderGravity != newGravity) {
			newGravity = sliderGravity;
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

		// Ends the window
		ImGui::End();
	}

	ImGui::Render();
}

void UIManager::cleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}