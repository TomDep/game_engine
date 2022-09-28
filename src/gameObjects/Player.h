#pragma once
#include "Entity.h"
#include "../graphics/Camera.h"

class Player {

public:
	Player(Camera* camera) : camera(camera) {
	}

private:

	Camera* camera;
};

