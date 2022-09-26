#include "UIManager.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "spdlog/spdlog.h";

// Icons from FontAwesome
#include <icons/IconsFontAwesome.h>	// From... https://github.com/juliettef/IconFontCppHeaders/

void UIManager::init(GLFWwindow* window) {
	// ImGUI Initialization
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();	(void)io;

	//io.Fonts->AddFontDefault();
	//Merge in icons from Font Awesome
	//static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	//ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	//io.Fonts->AddFontFromFileTTF("Librairies/resources/icons/fonts/" FONT_ICON_FILE_NAME_FAS, 16.0f, &icons_config, icons_ranges);

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void UIManager::render() {
	// Tell OpenGL a new frame is about to begin
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// UI INTERFACE
	{
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
		static float sliderGravity = 10.0f;
		ImGui::SliderFloat("###sliderGravity", &sliderGravity, 0.0f, 20.0f);

		if (sliderGravity != newGravity) {
			newGravity = sliderGravity;
			spdlog::debug("Hey! Just changed gravity to {}", newGravity);
		}

		if (ImGui::Button("Launch###buttonStart", ImVec2(100, 25))) {
			spdlog::debug("Launching the {} with a gravity of {}", selectedWeaponName, newGravity);
		}

		// Ends the window
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::cleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}