#include "PhysicsPanel.h"

#include <imgui.h>

void PhysicsPanel::render() {
	ImGui::Begin("Toolbox");

	ImGui::Text("Play with gravity!");
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

void PhysicsPanel::startSimulation() {
	physicsManager->setGravityConstant(newGravity);
	physicsManager->enable();
}

void PhysicsPanel::resetSimulation() {
	// Deactivate the physics
	physicsManager->disable();
}