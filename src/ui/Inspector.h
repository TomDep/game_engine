#pragma once

#include "../gameObjects/Entity.h"
#include "Panel.h"

class Inspector : public Panel {
public:
	void render();

	void setSelectedEntity(Entity* entity) { selectedEntity = entity; }
private:
	void renderTransform();

	Entity* selectedEntity;
};