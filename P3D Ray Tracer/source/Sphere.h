#pragma once
#include "Object.h"
#include "Math\Math.h"

class Sphere : public Object
{
public:
	Sphere(vec3 center, float radius);
	Sphere(vec3 center, float radius, Material mat);
	~Sphere();

	vec3 center();
	float radius();
	Material material();

	void setMaterial(Material mat);

	HitInfo intersect(Ray& ray);

private:
	vec3 _center;
	float _radius;
};

