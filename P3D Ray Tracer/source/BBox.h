#pragma once
#include "Object.h"
class BBox : public Object
{
public:
	BBox(vec3 min, vec3 max);
	~BBox();

	HitInfo intersect(Ray& ray);
	void setMaterial(Material& mat);

private:
	vec3 _min;
	vec3 _max;
};

