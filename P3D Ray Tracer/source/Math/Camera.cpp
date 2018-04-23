#include "Math.h"

Camera::Camera(const vec3& pos, const vec3& at, const vec3& up, const float& fovy, const float& near, const float& far, const int& ResX, const int& ResY)
	: _pos(pos), _at(at), _up(up), _fovy(fovy), _near(near), _far(far), _resX(ResX), _resY(ResY) {

	_lookat = normalize(pos - at); //w
	_right = normalize(cross(up, _lookat)); //u
	_up = normalize(cross(_lookat, _right)); //v

	_focalDistance = (_pos - _at).lengthSqr();
	//_height = 2.0f * _focalDistance * tan(degToRad(_fovy)/2.0f);
	_height = 2.0f * _focalDistance * tan(_fovy / 2.0f);
	_width = ((float)_resX / (float)_resY) * _height;

}

Camera::Camera()
{
}
void Camera::setRight() {
	_right = normalize(cross(_up, _lookat));
}
void Camera::setUp() {
	_up = normalize(cross(_lookat, _right));
}
void Camera::setLookAt() {

	_lookat = normalize(_pos - _at);
}

void Camera::setHeight() {
	_height = 2.0f *_focalDistance * tan(degToRad(_fovy) / 2.0f);

}

void Camera::setFocalDistance(vec3 pos, vec3 at) {
	_focalDistance = (pos - at).lengthSqr();

}

void Camera::setLensFocalDistance(float lensFocalDistance) {
	_focalDistance = lensFocalDistance;

}

void Camera::setWidth() {
	_width = ((float)_resX / (float)_resY) * _height;
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


//////////////////////////////////////////////SETTERS//////////////////////
void Camera::setPos(vec3 pos) {
	_pos = pos;
}
void Camera::setAt(vec3 at) {
	_at = at;
}
void Camera::setUp(vec3 up) {
	_up = up;
}
void Camera::setFovy(float fovy) {
	_fovy = fovy;
}
void Camera::setNear(float near) {
	_near = near;
}
void Camera::setFar(float far) {
	_far = far;
}
void Camera::setResX(int resX) {
	_resX = resX;
}
void Camera::setResY(int resY) {
	_resY = resY;
}

