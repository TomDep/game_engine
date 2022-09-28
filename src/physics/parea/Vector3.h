#include <ostream>
class Vector3 {
public:
	float x;
	float y;
	float z;

public:
	/******* DEFAULT CONSTUCTOR *******/
	Vector3() : x(0), y(0), z(0) {}
	/******* EXPLICIT CONSTUCTOR *******/
	Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

	/******* FLIP ALL COMPONENTS OF THE VECTOR *******/
	void invert() {
		x = -x;
		y = -y;
		z = -z;
	}
	/******* VECTOR AND DIRECTION *******/
	float magnitude() const {
		return (float)sqrt(x * x + y * y + z * z);
	}
	float squareMagnitude() const {
		return x * x + y * y + z * z;
	}
	void normalize() {
		float l = magnitude();
		if (l > 0) {
			(*this) *= ((float)1) / l;
		}
	}
	/******* SCALAR AND VECTOR MULTIPLICATION *******/
	void operator*=(const float value) {
		x *= value;
		y *= value;
		z *= value;
	}
	Vector3 operator*(const float value) const {
		return Vector3(x * value, y * value, z * value);
	}
	/******* VECTOR ADDITION *******/
	void operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	Vector3 operator+(const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	/******* VECTOR SUBTRACTION *******/
	void operator-=(const Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	Vector3 operator-(const float s) const {
		return Vector3(x - s, y - s, z - s);
	}

	Vector3 operator-(const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	/******* VECTOR ADDITION AND SCALING *******/
	void addScaledVector(const Vector3& v, float scale) {
		x += v.x * scale;
		y += v.y * scale;
		z += v.z * scale;
	}
	/******* COMPONENT PRODUCT *******/
	void componentProductUpdate(const Vector3& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}
	Vector3 componentProduct(const Vector3& v) const {
		return Vector3(x * v.x, y * v.y, z * v.z);
	}
	/******* SCALAR PRODUCT *******/
	float scalarProduct(const Vector3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}
	float operator*(const Vector3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}
	/******* VECTOR PRODUCT *******/
	Vector3 vectorProduct(const Vector3& v) const {
		return Vector3(y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}
	void operator%=(const Vector3& v) {
		(*this) = vectorProduct(v);
	}
	Vector3 operator%(const Vector3& v) const {
		return Vector3(y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}
	/******* ORTHONORMAL BASIS *******/
	void makeOrthonormalBasis(Vector3* a, Vector3* b, Vector3* c) {
		a->normalize();
		(*c) = (*a) % (*b);
		if (c->squareMagnitude() == 0.0) {
			return;
		}
		c->normalize();
		(*b) = (*c) % (*a);
	}
	/******* PRINTING FUNCTION *******/
	friend std::ostream& operator << (std::ostream& os, const Vector3& vector) {
		return os << "Vector3 : x=" << vector.x << "\t\ty=" << vector.y << "\t\tz=" << vector.z;
	}
};