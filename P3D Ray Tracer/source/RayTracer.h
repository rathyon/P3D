#pragma once

#include "Math\Math.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "BBox.h"
#include "Grid.h"
#include "Light.h"
#include "AreaLight.h"
#include "StochasticLight.h"
#include "Material.h"
#include "NFFParser.h"

#define DEPTH 5
#define RFL_ON true
#define adaptiveThreshold 0.3f

extern Camera camera;
extern vec3 background_color;
extern std::vector<Object*> objects;
extern std::vector<Light*> lights;
extern Grid grid;

// Generalized function
// for pixel (x,y)
vec3 rayTrace(int x, int y);

// traces a given ray
vec3 trace(Ray ray, int depth);

// RAY TRACING METHODS
vec3 naiveTrace(int x, int y);
vec3 stochasticTrace(int x, int y, int matrix_size);
vec3 jitteringTrace(int x, int y, int matrix_size);

vec3 adaptiveTrace(float x, float y); //initial call
vec3 adaptiveTrace(Ray ll, Ray lr, Ray ul, Ray ur, float x, float y, float size); //recursive call