#include "AreaLight.h"

AreaLight::AreaLight()
{
	float colorContributionByLight = (float)1 / (float)NUMBER_OF_POINTLIGHTS_PER_AREA_LIGHT;
	colorForEachPointLight = vec3(colorContributionByLight);
	CalculateLightPosition();
}

void AreaLight::CalculateLightPosition()
{
	double lightX, lightY, lightZ;
	vec3 lightPosition;

	for (int x = 0; x < NUMBER_OF_LINES; x++)
		for (int y = 0; y < NUMBER_OF_LINES; y++)
		{
			int pointLightIndex = x * NUMBER_OF_LINES + y;
			lightX = 4 + x * SPACING_BETWEEN_POINTLIGHTS;
			lightY = 3 + y * SPACING_BETWEEN_POINTLIGHTS;
			lightZ = 1;
			lights[pointLightIndex] = new Light(vec3(lightX, lightY, lightZ), colorForEachPointLight);
		}
}

