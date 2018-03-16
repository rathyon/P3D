#pragma once
#include "Object.h"
#include "Math\Math.h"

class Triangle : public Object {

public:
	Triangle(vec3 a, vec3 b, vec3 c);
	Triangle(vec3 a, vec3 b, vec3 c, Material mat);
	~Triangle();

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

