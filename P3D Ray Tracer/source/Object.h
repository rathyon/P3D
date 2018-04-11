#pragma once
#include "Math\Math.h"
#include "Material.h"

struct HitInfo {
	Ray ray;
	float t = MISS;
	Material material;
	vec3 intersection;
	vec3 normal;
	vec3 color; // unused at this time
};

class Object
{
public:
	Object();
	~Object();

	virtual HitInfo intersect(Ray& ray) = 0;

	//returns reflected ray
	Ray reflect(HitInfo& info);

	//returns refracted ray
	Ray refract(HitInfo& info);

	

protected:
	Material _material;
};

