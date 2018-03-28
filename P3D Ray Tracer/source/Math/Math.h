#pragma once

#include <cstdlib>
#include <ctime>
#include "vec3.h"
#include "vec4.h"
#include "Camera.h"
#include "Ray.h"

#define MISS -1.0f
#define OFFSET 0.0001f

//returns float between 0.0 and 1.0
float frand();