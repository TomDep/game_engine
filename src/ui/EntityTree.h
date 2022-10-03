#pragma once

#include "Panel.h"

#include "../scene/Scene.h"
#include "../gameObjects/Entity.h"

#include <imgui.h> 

class EntityTree : public Panel {

public:
	EntityTree(Scene* scene);
	void render();

	Entity* getSelectedEntity() const { return selectedEntity; }
private:
	Scene* scene;
	Entity* selectedEntity = nullptr;

	bool isCurrentIdOpened(int id);
	vector<int> idOpened;

	int renderEntityChildren(Entity* entity);
	ImGuiTreeNodeFlags base_flags;
};