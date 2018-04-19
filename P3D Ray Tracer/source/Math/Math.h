#pragma once

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "vec3.h"
#include "vec4.h"
#include "Camera.h"
#include "Ray.h"

//returns float between 0.0 and 1.0
float frand();

//not sure if this works!
inline float clamp(float val, float min, float max) {
	return std::max(min, std::min(val, max));
}