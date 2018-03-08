#pragma once
#include "Math\Math.h"
#include "Object.h"
#include "Material.h"

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
	void setMaterial(float r, float g, float b, float kd, float ks, float shininess, float transmitance, float IOR);
	void setMaterial(Material mat);
	float intersect(ray ray);

	//vec3 shade(Light light, vec3 intersectionPoint, ray ray);

private:
	vec3 _a;
	vec3 _b;
	vec3 _c;
	vec3 _normal;
	Material _mat;
	
}; 
