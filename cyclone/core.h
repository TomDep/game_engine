#pragma once
#include "precision.h"
#include "math.h"

namespace cyclone {
	class Vector3 {
		public:
			real x;
			real y;
			real z;
		private:
			real pad;

		public:
			Vector3() : x(0), y(0), z(0) {}
			Vector3(const real x, const real y, const real z) : x(x), y(y), z(z) {}
			void invert() {
				x = -x;
				y = -y;
				z = -z;
			}
			real magnitude() const {
				return sqrt(x * x + y * y + z * z);
			}
			real squareMagnitud() const {
				return x * x + y * y + z * z;
			}
			void print() {
				std::cout << "Vector3 : x=" << x << "\t\ty=" << y << "\t\tz=" << z << std::endl;
			}
			/*friend ostream& operator<<(ostream& os, const Vector3& vector) {
				os << "Vector3 : x=" << vector.x << "\t\ty=" << vector.y << "\t\tz=" << vector.z;
				return os;
			}*/
	};	
}
