#pragma once

#include "Math\vec3.h"
#include "Light.h"

#define NUMBER_OF_POINTLIGHTS_PER_AREA_LIGHT 9
#define NUMBER_OF_LINES sqrt(NUMBER_OF_POINTLIGHTS_PER_AREA_LIGHT)
#define	SPACING_BETWEEN_POINTLIGHTS 0.15


class AreaLight
{
public:
	 AreaLight();
	 Light* lights[NUMBER_OF_POINTLIGHTS_PER_AREA_LIGHT];

private:
	vec3 colorForEachPointLight;
	void CalculateLightPosition();
};
