#include"Math.h"
#include <cmath>
#include <algorithm>

/* ============================================================================
vec4 Constructors
==============================================================================*/
vec4::vec4() : x(0), y(0), z(0), w(0) { }
vec4::vec4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) { }
vec4::vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

/* ============================================================================
vec4 Math Operators
==============================================================================*/
vec4 vec4::operator+(const vec4& v) const {
	return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

vec4& vec4::operator+=(const vec4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

vec4 vec4::operator-(const vec4& v) const {
	return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

vec4& vec4::operator-=(const vec4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

vec4 vec4::operator*(float scalar) const {
	return vec4(scalar * x, scalar * y, scalar * z, scalar * w);
}

vec4& vec4::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

vec4 vec4::operator/(float scalar) const {
	return vec4(x / scalar, y / scalar, z / scalar, w / scalar);
}

vec4& vec4::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
	return *this;
}

vec4 vec4::operator-() const {
	return vec4(-x, -y, -z, -w);
}

bool vec4::operator==(const vec4& v) const {
	return (x == v.x && y == v.y && z == v.z && w == v.w);
}

bool vec4::operator!=(const vec4& v) const {
	return !(*this == v);
}

/* ============================================================================
vec4 Access Methods
==============================================================================*/
float vec4::operator[](unsigned int idx) const {
	if (idx == 0)
		return x;

	if (idx == 1)
		return y;

	if (idx == 2)
		return z;

	return w;
}

float& vec4::operator[](unsigned int idx) {
	if (idx == 0)
		return x;

	if (idx == 1)
		return y;

	if (idx == 2)
		return z;

	return w;
}

/* ============================================================================
vec4 Member Methods
==============================================================================*/
float vec4::lengthSqr() const {
	return x * x + y * y + z * z + w * w;
}

float vec4::length() const {
	return std::sqrt(lengthSqr());
}

void vec4::normalize() {
	float lenSqr = lengthSqr();
	if (lenSqr > 0)
		*this /= std::sqrt(lenSqr);
}

float vec4::min() const {
	return std::min(x, std::min(y, std::min(z, w)));
}

float vec4::max() const {
	return std::max(x, std::max(y, std::max(z, w)));
}

/* ============================================================================
vec4 Non-Member Functions
==============================================================================*/

std::istream& operator>>(std::istream& is, vec4& v) {
	is >> v.x;
	is >> v.y;
	is >> v.z;
	is >> v.w;
	return is;
}

std::ostream& operator<<(std::ostream& os, const vec4& v) {
	os << "vec4: [" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";
	return os;
}

vec4 operator*(float scalar, const vec4& v) {
	return v * scalar;
}

vec4 abs(const vec4& v) {
	return vec4(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w));
}

vec4 normalize(const vec4& v) {
	float lenSqr = v.lengthSqr();
	if (lenSqr > 0)
		return v / std::sqrt(lenSqr);
	return vec4(0);
}

float dot(const vec4& v1, const vec4& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

float absDot(const vec4& v1, const vec4& v2) {
	return std::abs(dot(v1, v2));
}

float distance(const vec4& v1, const vec4& v2) {
	vec4 v = v1 - v2;
	return v.length();
}