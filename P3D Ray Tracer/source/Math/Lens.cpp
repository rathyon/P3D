
#include "Lens.h"
//#include <random>
#include "Math.h"
Lens::Lens(const float& radius, const  float& focalDistance, const int& samples, const vec3& pos, const vec3& at, const vec3& up, const float& fovy, const float& near, const float& far, const int& ResX, const int& ResY)
	 :_radius(radius), _focalDistance(focalDistance), _samples(samples) {
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
	setHeight(_focalDistance);
	setWidth();
	
	
}

Lens::Lens()
{
}

const vec3 Lens::rayDirection(const vec3& pixelPoint, const vec3& lensPoint) {
	//assumindo near como viewPlane
	vec3 point;
	point.x = pixelPoint.x * _focalDistance / getNear();
	point.y = pixelPoint.x * _focalDistance / getNear();

	vec3 dir = (point.x - lensPoint.x) * getRight() + (point.y - lensPoint.y) * getUp() - _focalDistance * getLookAt();
	dir.normalize();

	return dir;
}

const void Lens::generateRandomSamples(const int& samples) {
	
	//std::default_random_engine generator;
	//std::uniform_int_distribution<float> distribution(getPos().x, getPos().x+_radius); //entre posição da camera e o radius
	
	for (int i = 0; i < samples;i++) {
		//float randomX = distribution(generator) * cos(rand() % 361); //multiplicar o numero por um coseno para mapear num circulo esse valor em X
		//float randomY = distribution(generator) * sin(rand() % 361); //multiplicar o numero por um seno para mapear num circulo esse valor em Y
		//vec3 randomPoint = vec3(randomX, randomY, getPos().z);
		//randomSamples.push_back(&randomPoint);//talvez esteja mal n percebo o erro
		float distance = frand() * _radius;
		float angle = frand() * 360.0f;
		float X = cos(angle);
		float Y = sin(angle);
		vec3* randomPoint = new vec3(getRight() * distance * X + getUp() * distance * Y + getLookAt());
		randomSamples.push_back(randomPoint);
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


