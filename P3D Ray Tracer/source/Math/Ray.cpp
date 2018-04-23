#include "Math.h"
#include "Lens.h"
Ray::Ray() {
}

Ray::Ray(const vec3& origin, const vec3& dir)
	: _origin(origin), _dir(dir) { }

Ray::Ray(const Camera& camera, const float& x, const float& y) {
	_origin = camera.getPos();

	float df = camera.getFocalDistance();
	vec3 right = camera.getRight();
	vec3 up = camera.getUp();
	vec3 lookAt = camera.getLookAt();
	float h = camera.getHeight();
	float w = camera.getWidth();
	float ResX = (float)camera.getResX();
	float ResY = (float)camera.getResY();

	//casts ray in the lower left corner of the pixel ***WHEN*** X and Y match the pixel coords
	_dir = normalize((-df * lookAt) +
		h * (y / ResY - 0.5f)*up +
		w * (x / ResX - 0.5f)*right);
}
/*
Ray::Ray(const Lens& thinLens,const vec3& lensPoint, const float& x, const float& y) {
_origin = thinLens.getPos();

float df = thinLens.getFocalDistance();
vec3 right = thinLens.getRight();
vec3 up = thinLens.getUp();
vec3 lookAt = thinLens.getLookAt();
float h = thinLens.getHeight();
float w = thinLens.getWidth();
float ResX = (float)thinLens.getResX();
float ResY = (float)thinLens.getResY();

//casts ray in the lower left corner of the pixel ***WHEN*** X and Y match the pixel coords
_dir = normalize((-df * lookAt) +
h * (y / ResY - 0.5)*up +
w * (x / ResX - 0.5)*right);
}
*/

const vec3& Ray::origin() const {
	return _origin;
}

const vec3& Ray::direction() const {
	return _dir;
}

vec3 Ray::operator()(float t) const {
	return _origin + t * _dir;
}