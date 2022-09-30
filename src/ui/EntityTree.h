#pragma once

#include "../scene/Scene.h"

class EntityTree {

public:
	EntityTree(Scene* scene) : scene(scene) {};
	void render();
private:
	Scene* scene;
};