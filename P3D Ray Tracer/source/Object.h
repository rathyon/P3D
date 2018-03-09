#pragma once
#include "Math\Math.h"
#include "Light.h"
#include "Material.h"

class Object
{
public:
	Object();
	~Object();

	virtual float intersect(ray ray) = 0;
	virtual vec3 shade(Light light, ray ray, float t) = 0;

protected:
	Material _mat;
};

