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
	
	//vec3 shade(Light light, vec3 point, vec3 inv_ray_dir);

private:
	vec3 _center;
	float _radius;
};

