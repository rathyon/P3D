#include "Math.h"

ray::ray(const vec3& origin, const vec3& dir)
	: _origin(origin), _dir(dir){ }


const vec3& ray::origin() const {
	return _origin;
}

const vec3& ray::direction() const {
	return _dir;
}

const vec3& ray::camGetPrimaryRay(Camera camera, double x, double y) const {
	vec3 df = camera.getDf();
	vec3 Xe = camera.getXe();
	vec3 Ye = camera.getYe();
	vec3 Ze = camera.getZe();
	float height = camera.getHeight();
	float width = camera.getWidth();
	float ResX = camera.getResX();
	float ResY = camera.getResY();

	_dir = -df * Ze + height * (y / ResY - 0.5)*Ye + width*(x / ResX - 0.5)*Xe;
}

vec3 ray::operator()(float t) const {
	return _origin + t * _dir;
}