#include "Math.h"

ray::ray(const vec3& origin, const vec3& dir)
	: _origin(origin), _dir(dir){ }


const vec3& ray::origin() const {
	return _origin;
}

const vec3& ray::direction() const {
	return _dir;
}

vec3 ray::operator()(float t) const {
	return _origin + t * _dir;
}