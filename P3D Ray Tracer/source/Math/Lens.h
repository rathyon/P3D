#pragma once

#include "Camera.h"
#include <vector>

class Lens :public Camera{
public:
	Lens(const float& radius,const float& viewDistance,const  float& focalDistance, const int& samples);
	

	
	const float& getRadius() const;
	const float& getFocalDistance() const;
	const int& getSamples() const;
	std::vector<vec3*> randomSamples;
	
	const void generateRandomSamples(const int& samples);
	const vec3 rayDirection(const vec3& pixelPoint,const vec3& lensPoint);
	void Lens::clearVector();
private:
	//imported values
	
	float _radius;
	float _viewDistance;
	float _focalDistance;
	int _samples;
	

	


};