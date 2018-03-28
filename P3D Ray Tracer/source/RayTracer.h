#include "Math\Math.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "BBox.h"
#include "Light.h"
#include "Material.h"
#include "NFFParser.h"

#define DEPTH 5
#define RFL_ON true

extern Camera camera;
extern vec3 background_color;
extern std::vector<Object*> objects;
extern std::vector<Light*> lights;

// Generalized function
// for pixel (x,y)
vec3 rayTrace(int x, int y);

// traces a given ray
vec3 trace(Ray ray, int depth);

// RAY TRACING METHODS
vec3 naiveTrace(int x, int y);
vec3 stochasticTrace(int x, int y, int matrix_size);