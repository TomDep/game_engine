#pragma once

#include "Shader.h"

class Texture {
public:
	Texture(const char* path, bool png=false, int index=0);
	void use();

	int getIndex() const { return index; }

private:
	int index;
	unsigned int id;
};