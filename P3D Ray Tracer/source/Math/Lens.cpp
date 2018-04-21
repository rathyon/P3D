
#include "Lens.h"
#include <random>
Lens::Lens(const float& radius, const float& viewDistance,const float& focalDistance, const int& samples)
	: _radius(radius), _viewDistance(viewDistance), _focalDistance(focalDistance), _samples(samples) {

}

const vec3 Lens::rayDirection(const vec3& pixelPoint, const vec3& lensPoint) {

	vec3 point;
	point.x = pixelPoint.x * _focalDistance / _viewDistance;
	point.y = pixelPoint.x * _focalDistance / _viewDistance;

	vec3 dir = (point.x - lensPoint.x) * getUp() + (point.y - lensPoint.y) * getRight() - _focalDistance * getLookAt();
	dir.normalize();

	return dir;
}

const void Lens::generateRandomSamples(const int& samples) {
	
	std::default_random_engine generator;
	std::uniform_int_distribution<float> distribution(getPos().x, getPos().x+_radius); //entre posição da camera e o radius
	

	for (int i = 0; i < samples;i++) {
		float randomX = distribution(generator) * cos(rand() % 361); //multiplicar o numero por um coseno para mapear num circulo esse valor em X
		float randomY = distribution(generator) * sin(rand() % 361); //multiplicar o numero por um seno para mapear num circulo esse valor em Y
		vec3 randomPoint = vec3(randomX, randomY, getPos().z);
		randomSamples.push_back(&randomPoint);//talvez esteja mal n percebo o erro

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


