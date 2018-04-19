#pragma once
#include "Math\Math.h"
#include "Material.h"

struct HitInfo {
	Ray ray;
	float t = MISS;
	Material material;
	vec3 intersection;
	vec3 normal;
	vec3 color; // unused at this time
};