#pragma once
#include "Math\Math.h"
#include "HitInfo.h"
#include "Material.h"

class BBox
{
public:
	BBox();
	BBox(vec3 min, vec3 max);
	~BBox();

	vec3 min();
	vec3 max();

	void setMin(vec3 min);
	void setMax(vec3 max);

	bool inside(vec3 point);
	HitInfo intersect(Ray& ray);

private:
	vec3 _min;
	vec3 _max;
};

