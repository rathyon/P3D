#pragma once
#include "Math\Math.h"

class Object
{
#define MISS -1.0f;
public:
	Object();
	~Object();

	virtual float intersect(ray ray) = 0;
	//virtual vec3 shade(Light light, vec3 point, vec3 inv_ray_dir) = 0;
	/*
	NdotL
	NdotH

	N - intersection point & center
	L - intersection point & light_pos
	H - L & negative ray direction
	*/

private:
	vec3 _color;

};

