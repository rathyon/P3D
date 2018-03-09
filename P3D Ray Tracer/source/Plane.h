#pragma once
#include "Math\Math.h"
#include "Object.h"
#include "Material.h"
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
	float intersect(Ray ray);
	vec3 shade(Light light, Ray ray, float t);

private:
	vec3 _a;
	vec3 _b;
	vec3 _c;
	vec3 _normal;
	
}; 
