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

	float intersect(Ray ray);
	vec3 shade(Light light, Ray ray, float t);

private:
	vec3 _center;
	float _radius;
};

