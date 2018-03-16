#pragma once

class Ray {
public:
	Ray();
	Ray(const vec3& origin, const vec3& dir);
	Ray(const Camera& camera, const int& x, const int& y);

	const vec3& origin()    const;
	const vec3& direction() const;

	vec3 operator()(float t) const;


private:
	vec3  _origin;
	vec3  _dir;

};