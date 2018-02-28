#pragma once
#include <iostream>

class vec4 {
	public:
		float x, y, z, w;

		vec4();
		vec4(float scalar);
		vec4(float x, float y, float z, float w);

		// Vector math operators
		vec4  operator* (float scalar) const;
		vec4& operator*=(float scalar);

		vec4  operator/ (float scalar) const;
		vec4& operator/=(float scalar);

		vec4  operator+ (const vec4& v) const;
		vec4& operator+=(const vec4& v);

		vec4  operator- (const vec4& v) const;
		vec4& operator-=(const vec4& v);

		vec4  operator- () const;

		bool operator==(const vec4& v) const;
		bool operator!=(const vec4& v) const;

		// Array-like access
		float  operator[](unsigned int idx) const;
		float& operator[](unsigned int idx);

		// Vector member methods
		float lengthSqr() const;
		float length()    const;

		void  normalize();

		float min() const;
		float max() const;

		// Standard input/ouput
		friend std::istream& operator>>(std::istream& is, vec4& v);
		friend std::ostream& operator<<(std::ostream& os, const vec4& v);

		friend vec4 operator*(float scalar, const vec4& v);

		friend vec4 abs(const vec4& v);
		friend vec4 normalize(const vec4& v);

		friend float distance(const vec4& v1, const vec4& v2);
		friend float dot(const vec4& v1, const vec4& v2);
		friend float absDot(const vec4& v1, const vec4& v2);

};