#include "Sphere.h"

Sphere::Sphere(vec3 center, float radius)
{
	_center = center;
	_radius = radius;
}


Sphere::~Sphere()
{
}

vec3 Sphere::center() {
	return _center;
}

float Sphere::radius() {
	return _radius;
}


float Sphere::intersect(ray ray) {
	float dist_squared = (_center - ray.origin()).lengthSqr();
	float B = dot(ray.direction(), _center - ray.origin());
	float r2 = _radius*_radius;
	if (dist_squared > r2) {
		if (B < 0.0f) {
			//return false: there is no collision
			return MISS;
		}
	}
	float R = (B*B) - dist_squared + r2;
	if (R < 0.0f) {
		//return false: there is no collision
		return MISS;
	}

	float t;

	if (dist_squared > r2) {
		t = B - sqrt(R);
	}
	else {
		t = B + sqrt(R);
	}

	//print("HIT");

	return t;

}
