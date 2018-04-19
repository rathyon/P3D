#pragma once
#include "Object.h"
#include "Light.h"

class Plane : public Object
{
public:
	Plane(vec3 a, vec3 b, vec3 c);
	~Plane();

	vec3 a();
	vec3 b();
	vec3 c();
	vec3 normal();
	Material material();
	void setMaterial(Material mat);


	HitInfo intersect(Ray& ray);

private:
	vec3 _a;
	vec3 _b;
	vec3 _c;
	vec3 _normal;
}; 
