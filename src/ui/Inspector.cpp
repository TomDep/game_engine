#include "Inspector.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <string>

#include "../gameObjects/components/Transform.h"

void Inspector::render() {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3.0f, 3.0f));
	ImGui::Begin("Inspector", nullptr, windowFlags);
	ImGui::PopStyleVar();
	if (selectedEntity != nullptr) {
		char nameBuffer[25];
		strcpy_s(nameBuffer, selectedEntity->getName().c_str());
		ImGui::Text("Entity name"); ImGui::SameLine(); ImGui::InputText("##EntityName", nameBuffer, 25);
		ImGui::Text("Entity id : %d", selectedEntity->getId());

		ImGui::Spacing();

		renderTransform();
	}
	ImGui::End();
}

void Inspector::renderTransform() {
	Transform* transform = selectedEntity->getTransform();

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		// Position
		glm::vec3 position = transform->getPosition();

		float posX = position.x;
		float posY = position.y;
		float posZ = position.z;

		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x / 3 - 20);
		ImGui::Text("Position");
		ImGui::Text("X"); ImGui::SameLine(); ImGui::InputScalar("##XPosition", ImGuiDataType_Float, &posX, NULL); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputScalar("##YPosition", ImGuiDataType_Float, &posY, NULL); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputScalar("##ZPosition", ImGuiDataType_Float, &posZ, NULL);

		transform->setPosition(glm::vec3(posX, posY, posZ));

		// Scale
		glm::vec3 scale = transform->getScale();

		float scaleX = scale.x;
		float scaleY = scale.y;
		float scaleZ = scale.z;

		ImGui::Text("Scale");
		ImGui::Text("X"); ImGui::SameLine(); ImGui::InputScalar("##XScale", ImGuiDataType_Float, &scaleX, NULL); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputScalar("##YScale", ImGuiDataType_Float, &scaleY, NULL); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputScalar("##ZScale", ImGuiDataType_Float, &scaleZ, NULL);

		transform->setScale(glm::vec3(scaleX, scaleY, scaleZ));

		// Rotation
		glm::vec3 rotation = transform->getRotation();

		float rotationX = rotation.x;
		float rotationY = rotation.y;
		float rotationZ = rotation.z;

		ImGui::Text("Rotation");
		ImGui::Text("X"); ImGui::SameLine(); ImGui::InputScalar("##XRotation", ImGuiDataType_Float, &rotationX, NULL); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputScalar("##YRotation", ImGuiDataType_Float, &rotationY, NULL); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputScalar("##ZRotation", ImGuiDataType_Float, &rotationZ, NULL);

		transform->setRotation(glm::vec3(rotationX, rotationY, rotationZ));
	}
}