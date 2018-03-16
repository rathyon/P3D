#include "Plane.h"

Plane::Plane(vec3 a, vec3 b, vec3 c){
	_a = a;
	_b = b;
	_c = c;

	vec3 aux1 = _b - _a;
	vec3 aux2 = _c - _a;

	_normal = normalize(cross(aux1, aux2));
}

Plane::~Plane()
{
}

vec3 Plane::a() {
	return _a;
}

vec3 Plane::b() {
	return _b;
}

vec3 Plane::c() {
	return _c;
}

vec3 Plane::normal() {
	return _normal;
}

Material Plane::material() {
	return _material;
}

void Plane::setMaterial(Material mat) {
	_material = mat;
}

HitInfo Plane::intersect(Ray& ray) {
	HitInfo info;
	info.normal = _normal;
	info.ray = ray;
	info.material = _material;

	if (dot(_normal, ray.direction()) == 0.0f) {
		info.t = MISS;
		return info;
	}
	float t = - (dot((ray.origin() - _a), _normal)) / (dot(_normal, ray.direction()));

	if (t < 0) {
		info.t = MISS;
		return info;
	}

	info.t = t;
	info.intersection = ray.origin() + t * ray.direction();
	return info;
}

