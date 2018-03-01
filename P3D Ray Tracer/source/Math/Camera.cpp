#include "Math.h"

Camera::Camera(const vec3& eye, const vec3& at, const vec3& up, const double& fovy, const double& near, const double& far,const int& ResX, const int& ResY)
	: _eye(eye), _at(at), _up(up), _fovy(fovy), _near(near), _far(far), _ResX(ResX),_ResY(ResY) {

	calculateExtraComponents()
}


void Camera::calculateExtraComponents() const {
	df = (_eye - _at).lengthSqr();
	height = 2 * df* tan(_fov^y*0.5);
	width = (_ResX / _ResY) * height;
	Ze = df.normalize();
	Xe = cross(_up, Ze).normalize();
	Ye = cross(Ze, Xe);
}



const vec3& Camera::getDf() {
	return df;
}
const vec3& Camera::getXe() {
	return Xe;
}
const vec3& Camera::getYe() {
	return Ye;
}
const vec3& Camera::getZe() {
	return Ze;
}
const float& Camera::getWidth() {
	return width;
}
const float& Camera::getHeight() {
	return height;
}
const float& Camera::getResX() {
	return width;
}
const float& Camera::getResY() {
	return height;
}

