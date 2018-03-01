#include "Math.h"

Camera::Camera(const vec3& pos, const vec3& at, const vec3& up, const float& fovy, const float& near, const float& far,const int& ResX, const int& ResY)
	: _pos(pos), _at(at), _up(up), _fovy(fovy), _near(near), _far(far), _resX(ResX),_resY(ResY) {

	_lookat = normalize(pos - at);
	_right = normalize(cross(up, _lookat));
	_up = normalize(cross(_lookat, _right));

	_focalDistance = (_pos - _at).lengthSqr();
	_height = 2.0f * _focalDistance * tan(_fovy*0.5f);
	_width = ((float) _resX / (float) _resY) * _height;

}

const vec3& Camera::getPos() const {
	return _pos;
}
const vec3& Camera::getAt() const {
	return _at;
}
const float& Camera::getFOVY() const {
	return _fovy;
}
const float& Camera::getNear() const {
	return _near;
}
const float& Camera::getFar() const {
	return _far;
}
const vec3& Camera::getRight() const {
	return _right;
}
const vec3& Camera::getUp() const {
	return _up;
}
const vec3& Camera::getLookAt() const {
	return _lookat;
}
const float& Camera::getFocalDistance() const {
	return _focalDistance;
}
const float& Camera::getWidth() const {
	return _width;
}
const float& Camera::getHeight() const {
	return _height;
}
const int& Camera::getResX() const {
	return _resX;
}
const int& Camera::getResY() const {
	return _resY;
}

