#include "Math.h"

ray::ray(const vec3& origin, const vec3& dir)
	: _origin(origin), _dir(dir){ }

ray::ray(const Camera& camera, const int& x, const int& y) {
	_origin = camera.getPos();

	float df = camera.getFocalDistance();
	vec3 right = camera.getRight();
	vec3 up = camera.getUp();
	vec3 lookAt = camera.getLookAt();
	float h = camera.getHeight();
	float w = camera.getWidth();
	float ResX = (float) camera.getResX();
	float ResY = (float) camera.getResY();

	_dir =  normalize((-df * lookAt) + 
			h*((float)y / ResY - 0.5f)*up + 
			w*((float)x / ResX - 0.5f)*right);
	
}


const vec3& ray::origin() const {
	return _origin;
}

const vec3& ray::direction() const {
	return _dir;
}

vec3 ray::operator()(float t) const {
	return _origin + t * _dir;
}