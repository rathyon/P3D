#pragma once
#include "Math\Math.h"
#include "Light.h"
#include "Material.h"

struct HitInfo{
	Ray ray;
	float t = MISS;
	Material material;
	vec3 intersection;
	vec3 normal;
	vec3 color;
};

class Object
{
public:
	Object();
	~Object();

	virtual HitInfo intersect(Ray& ray) = 0;

	//returns BlinnPhong shading result
	vec3 shade(Light& light, HitInfo& hf);

	//returns reflection ray
	Ray reflect(Light& light, HitInfo& info);

	//returns refraction ray
	Ray refract(HitInfo& info);

	

protected:
	Material _material;
};

