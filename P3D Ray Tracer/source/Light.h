#pragma once
#include "Math\Math.h"
#include "Object.h"
#include "Grid.h"
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
	vec3 shade(HitInfo& info, Grid& grid);

	bool inShadow(HitInfo& info, std::vector<Object*> objects);
	bool inShadow(HitInfo& info, Grid& grid);

private:
	vec3 _pos;
	vec3 _color;
};

