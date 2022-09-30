#include "EntityTree.h"

#include <imgui.h>

void EntityTree::render() {
	ImGui::Begin("Entity tree");

	int i = 0;
	for (Entity* entity : *scene->getEntities()) {
		ImGui::Text("Entity::%d", i);
	}

	ImGui::End();
}