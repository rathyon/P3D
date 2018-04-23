
#include "Lens.h"
//#include <random>
#include "Math.h"

Lens::Lens(const float& radius, const  float& focalDistance, const int& samples, const vec3& pos, const vec3& at, const vec3& up, const float& fovy, const float& near, const float& far, const int& ResX, const int& ResY)
	: Camera(pos, at, up, fovy, near, far, ResX, ResY), _radius(radius), _focalDistance(focalDistance), _samples(samples) {

	/**/
	setPos(pos);
	setAt(at);
	setUp(up);
	setFovy(fovy);
	setNear(near);
	setFar(far);
	setResX(ResX);
	setResY(ResY);
	setLookAt();
	setRight();
	setUp();
	setLensFocalDistance(_focalDistance);
	//setCameraFocalDistance(pos, at);
	setHeight();
	setWidth();
	/**/

	/** /
	_lookat = normalize(pos - at); //w
	_right = normalize(cross(up, _lookat)); //u
	_up = normalize(cross(_lookat, _right)); //v

	_focalDistance = (_pos - _at).lengthSqr();
	//_height = 2.0f * _focalDistance * tan(degToRad(_fovy)/2.0f);
	_height = 2.0f * _focalDistance * tan(_fovy / 2.0f);
	_width = ((float)_resX / (float)_resY) * _height;
	/**/
}

Lens::Lens()
{
}

const vec3 Lens::rayDirection(const vec3& pixelPoint, const vec3& lensPoint) {
	//assumindo near como viewPlane
	vec3 point;
	point.x = pixelPoint.x * _focalDistance / getNear();
	point.y = pixelPoint.y * _focalDistance / getNear();

	vec3 dir = (point.x - lensPoint.x) * getRight() + (point.y - lensPoint.y) * getUp() - _focalDistance * getLookAt();
	dir.normalize();

	return dir;
}

const void Lens::generateRandomSamples(const int& samples) {



	for (int i = 0; i < samples; i++) {
		/*
		float distance = frand() * _radius;
		float angle = frand() * 360.0f;
		float angleToRadians = (angle * PI) / 360.0f;
		float X = cos(angleToRadians);
		float Y = sin(angleToRadians);
		//float X = cos(angle);
		//float Y = sin(angle);



		vec3* randomPoint = new vec3(getRight() * distance * X + getUp() * distance * Y + getLookAt());
		randomSamples.push_back(randomPoint);
		*/

		float rx = frand();
		float ry = frand();
		float r = sqrt(rx);
		float theta = 2.0f * 3.1415f * ry;
		randomSamples.push_back(vec3(r * cos(theta), r * sin(theta), 0));

	}

};

void Lens::clearVector() {
	randomSamples.clear();
}
const float& Lens::getFocalDistance() const {
	return _focalDistance;
}

const float& Lens::getRadius() const {
	return _radius;
}

const int& Lens::getSamples() const {
	return _samples;
}


