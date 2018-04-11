#pragma once

#include "Math\Math.h"
#include "Light.h"

#define POINTLIGHTS 9
#define AREADIM sqrt(POINTLIGHTS)
#define	SPACING 0.15


class AreaLight : public Light
{
public:
	 AreaLight(vec3 pos, vec3 color);
	 ~AreaLight();

	 Light* lights[POINTLIGHTS];
	 vec3 shade(HitInfo& info, std::vector<Object*> objects);

private:
	vec3 _pos;
	vec3 _color;
	float _contribution;

	void prepareLights();
};
