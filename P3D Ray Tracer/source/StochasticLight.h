#pragma once

#include "Math\Math.h"
#include "Light.h"

#define STOCHASTIC_DIM 0.4f

class StochasticLight : public Light
{
public:
	StochasticLight(vec3 pos, vec3 color);
	~StochasticLight();

	vec3 shade(HitInfo& info, std::vector<Object*> objects);
	bool inShadow(HitInfo& info, std::vector<Object*> objects);

private:
	vec3 _pos;
	vec3 _color;

	vec3 _a = vec3(1.0f, 0, 0);
	vec3 _b = vec3(0, 1.0f, 0);
};

