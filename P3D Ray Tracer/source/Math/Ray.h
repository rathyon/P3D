#pragma once

class ray {
public:
	ray(const vec3& origin, const vec3& dir);

	const vec3& origin()    const;
	const vec3& direction() const;
	const vec3& camGetPrimaryRay(Camera camera,double x,double y) const;

	vec3 operator()(float t) const;


private:
	vec3  _origin;
	vec3  _dir;

};