#pragma once
#include "Math\Math.h"
class Light
{
public:
	Light(vec3 pos, vec3 color);
	~Light();

	vec3 pos();
	vec3 color();

private:
	vec3 _pos;
	vec3 _color;
};

