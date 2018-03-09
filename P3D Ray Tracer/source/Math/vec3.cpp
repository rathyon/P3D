#include"Math.h"
#include <cmath>
#include <algorithm>

/* ============================================================================
vec3 Constructors
==============================================================================*/
vec3::vec3() : x(0), y(0), z(0) { }
vec3::vec3(float scalar) : x(scalar), y(scalar), z(scalar) { }
vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) { }

/* ============================================================================
vec3 Math Operators
==============================================================================*/
vec3 vec3::operator+(const vec3& v) const {
	return vec3(x + v.x, y + v.y, z + v.z);
}

vec3& vec3::operator+=(const vec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

vec3 vec3::operator-(const vec3& v) const {
	return vec3(x - v.x, y - v.y, z - v.z);
}

vec3& vec3::operator-=(const vec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

vec3 vec3::operator*(float scalar) const {
	return vec3(scalar * x, scalar * y, scalar * z);
}

vec3 vec3::operator*(vec3 v) const {
	return vec3(x * v.x, y * v.y, z *v.z);
}

vec3& vec3::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

vec3 vec3::operator/(float scalar) const {
	return vec3(x / scalar, y / scalar, z / scalar);
}

vec3 vec3::operator/(vec3 v) const {
	return vec3(x / v.x, y / v.y, z / v.z);
}

vec3& vec3::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

vec3 vec3::operator-() const {
	return vec3(-x, -y, -z);
}

bool vec3::operator==(const vec3& v) const {
	return (x == v.x && y == v.y && z == v.z);
}

bool vec3::operator!=(const vec3& v) const {
	return !(*this == v);
}

/* ============================================================================
vec3 Access Methods
==============================================================================*/
float vec3::operator[](unsigned int idx) const {
	if (idx == 0)
		return x;

	if (idx == 1)
		return y;

	return z;
}

float& vec3::operator[](unsigned int idx) {
	if (idx == 0)
		return x;

	if (idx == 1)
		return y;

	return z;
}

/* ============================================================================
vec3 Member Methods
==============================================================================*/
float vec3::lengthSqr() const {
	return x * x + y * y + z * z;
}

float vec3::length() const {
	return std::sqrt(lengthSqr());
}

void vec3::normalize() {
	float lenSqr = lengthSqr();
	if (lenSqr > 0)
		*this /= std::sqrt(lenSqr);
}

float vec3::min() const {
	return std::min(x, std::min(y, z));
}

float vec3::max() const {
	return std::max(x, std::max(y, z));
}

unsigned int vec3::maxDim() const {
	if (x > y) {
		if (x > z)
			return 0;
		else
			return 2;
	}
	else {
		if (y > z)
			return 1;
		else
			return 2;
	}
}

unsigned int vec3::minDim() const {
	if (x < y) {
		if (x < z)
			return 0;
		else
			return 2;
	}
	else {
		if (y < z)
			return 1;
		else
			return 2;
	}
}

std::istream& operator>>(std::istream& is, vec3& v) {
	is >> v.x;
	is >> v.y;
	is >> v.z;
	return is;
}

std::ostream& operator<<(std::ostream& os, const vec3& v) {
	os << "vec3: [" << v.x << ", " << v.y << ", " << v.z << "]";
	return os;
}

/* ============================================================================
vec3 Non-Member Functions
==============================================================================*/
vec3 operator*(float scalar, const vec3& v) {
	return v * scalar;
}

vec3 abs(const vec3& v) {
	return vec3(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

vec3 normalize(const vec3& v) {
	float lenSqr = v.lengthSqr();
	if (lenSqr > 0)
		return v / std::sqrt(lenSqr);
	return vec3(0);
}

vec3 min(const vec3& v1, const vec3& v2) {
	return vec3(std::min(v1.x, v2.x),
		std::min(v1.y, v2.y),
		std::min(v1.z, v2.z));
}

vec3 max(const vec3& v1, const vec3& v2) {
	return vec3(std::max(v1.x, v2.x),
		std::max(v1.y, v2.y),
		std::max(v1.z, v2.z));
}

float dot(const vec3& v1, const vec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float absDot(const vec3& v1, const vec3& v2) {
	return std::abs(dot(v1, v2));
}

vec3 cross(const vec3& v1, const vec3& v2) {
	return vec3((v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x));
}

float distance(const vec3& v1, const vec3& v2) {
	vec3 v = v1 - v2;
	return v.length();
}

vec3 pow(const vec3& v, float exp) {
	return vec3(std::pow(v.x, exp),
		std::pow(v.y, exp),
		std::pow(v.z, exp));
}

void basisFromVector(const vec3& v1, vec3* v2, vec3* v3) {
	// Reference: [Duff et. al, 2017] - "Building an Orthonormal Basis, Revisited"
	const float sign = std::copysign(1.0f, v1.z);
	const float a = -1.0f / (sign + v1.z);
	const float b = v1.x * v1.y * a;

	*v2 = vec3(1.0f + sign * v1.x * v1.x * a, sign * b, -sign * v1.x);
	*v3 = vec3(b, sign + v1.y * v1.y * a, -v1.y);
}
