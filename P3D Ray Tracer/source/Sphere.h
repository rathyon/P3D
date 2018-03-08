#pragma once
#include "Object.h"
#include "Math\Math.h"

class Sphere : public Object
{
public:
	Sphere(vec3 center, float radius);
	~Sphere();

	vec3 center();
	float radius();

	float intersect(ray ray);
	
	//temp
	vec3 shade(vec3 light, float t, ray ray);

private:
	vec3 _center;
	float _radius;
};

