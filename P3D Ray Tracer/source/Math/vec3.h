#pragma once
#include <iostream>

class vec3{
	public:
		float x, y, z;

		vec3();
		vec3(float scalar);
		vec3(float x, float y, float z);

		// Vector math operators
		vec3  operator* (float scalar) const;
		vec3& operator*=(float scalar);
		vec3  operator* (vec3 v) const;

		vec3  operator/ (float scalar) const;
		vec3& operator/=(float scalar);
		vec3  operator/ (vec3 v) const;

		vec3  operator+ (const vec3& v) const;
		vec3& operator+=(const vec3& v);

		vec3  operator- (const vec3& v) const;
		vec3& operator-=(const vec3& v);

		vec3  operator- () const;

		bool operator==(const vec3& v) const;
		bool operator!=(const vec3& v) const;

		// Array-like access
		float  operator[](unsigned int idx) const;
		float& operator[](unsigned int idx);

		// Vector member methods
		float lengthSqr() const;
		float length()    const;

		void  normalize();

		float min() const;
		float max() const;
		unsigned int maxDim() const;
		unsigned int minDim() const;

		// Standard input/ouput
		friend std::istream& operator>>(std::istream& is, vec3& v);
		friend std::ostream& operator<<(std::ostream& os, const vec3& v);

		friend vec3 operator*(float scalar, const vec3& v);

		friend vec3 abs(const vec3& v);
		friend vec3 cross(const vec3& v1, const vec3& v2);
		friend vec3 pow(const vec3& v, float exp);
		friend vec3 normalize(const vec3& v);
		friend vec3 min(const vec3& v1, const vec3& v2);
		friend vec3 max(const vec3& v1, const vec3& v2);

		friend float distance(const vec3& v1, const vec3& v2);
		friend float dot(const vec3& v1, const vec3& v2);
		friend float absDot(const vec3& v1, const vec3& v2);

		friend void basisFromVector(const vec3& v1, vec3* v2, vec3* v3);
};

