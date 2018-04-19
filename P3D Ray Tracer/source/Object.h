#pragma once
#include "Math\Math.h"
#include "HitInfo.h"
#include "BBox.h"
#include "Material.h"

class Object
{
public:
	Object();
	~Object();

	virtual HitInfo intersect(Ray& ray) = 0;

	BBox* getBBox();

protected:
	Material _material;
	BBox* _bbox;
};

//returns reflected ray
Ray reflect(HitInfo& info);

//returns refracted ray
Ray refract(HitInfo& info);