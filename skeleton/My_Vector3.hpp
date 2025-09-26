#pragma once

#include <cmath>

struct My_Vector3 {
	float x, y, z;

	My_Vector3(float x, float y, float z)
		:x(x),y(y),z(z){ }
	physx::PxVec3 turn() {
		return { x,y,z };
	}

	float module() const{
		return std::sqrt(x * x + y * y + z * z);
	}
	float normalize() {
		auto mod = module();
		x /= mod;
		y /= mod;
		z /= mod;
	}
	float dot(const My_Vector3& v1,const My_Vector3& v2) const {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	My_Vector3& operator *=(const float d) {
		x *= d;
		y *= d;
		z *= d;
		return *this;
	}
	friend My_Vector3 operator*(My_Vector3 v, const float d) {
		v *= d;
		return v;
	}
	My_Vector3& operator+=(const My_Vector3 v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	friend My_Vector3 operator+(My_Vector3 lhs, const My_Vector3& rhs) {
		lhs += rhs;
		return lhs;
	}
	My_Vector3& operator-=(const My_Vector3 v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	friend My_Vector3 operator-(My_Vector3 lhs, const My_Vector3& rhs) {
		lhs -= rhs;
		return lhs;
	}

	static My_Vector3 zero() {
		return My_Vector3(0, 0, 0);
	};
};

inline bool operator==(const My_Vector3& v1, const My_Vector3& v2) {
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}