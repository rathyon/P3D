#include "Sphere.h"

Sphere::Sphere(vec3 center, float radius)
{
	_center = center;
	_radius = radius;
}

Sphere::Sphere(vec3 center, float radius, Material mat)
{
	_center = center;
	_radius = radius;
	_material = mat;
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

Material Sphere::material() {
	return _material;
}

void Sphere::setMaterial(Material mat) {
	_material = mat;
}


HitInfo Sphere::intersect(Ray& ray) {
	HitInfo info;
	info.ray = ray;
	info.material = _material;

	float dist_squared = (_center - ray.origin()).lengthSqr();
	float B = dot(ray.direction(), _center - ray.origin());
	float r2 = _radius*_radius;
	if (dist_squared > r2) {
		if (B < 0.0f) {
			//return false: there is no collision
			info.t = MISS;
			return info;
		}
	}
	float R = (B*B) - dist_squared + r2;
	if (R < 0.0f) {
		//return false: there is no collision
		info.t = MISS;
		return info;
	}

	float t;

	if (dist_squared > r2) {
		t = B - sqrt(R);
	}
	else {
		t = B + sqrt(R);
	}

	info.t = t;
	info.intersection = ray.origin() + t * ray.direction();
	info.normal = (info.intersection - _center) / _radius;

	// if ray origin is inside sphere, flip normal
	if (dist_squared < r2) {
		info.normal = -info.normal;
	}

	return info;
}