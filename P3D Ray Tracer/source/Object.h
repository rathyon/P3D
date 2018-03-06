#pragma once
#include "Math\Math.h"

class Object
{
public:
	Object();
	~Object();

	virtual float intersect(ray ray) = 0;

private:
	vec3 _color;

};

