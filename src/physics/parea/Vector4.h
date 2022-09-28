#pragma once

class Vector4 {
public:
	float x;
	float y;
	float z;
	float w;

	Vector4() : x(0), y(0), z(0), w(0) {}
	Vector4(float s) : x(s), y(s), z(s), w(s) {}
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};