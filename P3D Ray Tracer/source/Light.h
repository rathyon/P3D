#pragma once
#include "Math\Math.h"
#include "Object.h"
#include <vector>

class Light
{
public:
	Light();
	Light(vec3 pos, vec3 color);
	~Light();

	vec3 pos();
	vec3 color();

	//returns Blinn-Phong shading for point light
	vec3 shade(HitInfo& info, std::vector<Object*> objects);

	bool inShadow(HitInfo& info, std::vector<Object*> objects);

private:
	vec3 _pos;
	vec3 _color;
};

