#include "Triangle.h"

Triangle::Triangle(vec3 a, vec3 b, vec3 c)
{
	_a = a;
	_b = b;
	_c = c;

	vec3 aux1 = _b - _a;
	vec3 aux2 = _c - _a;

	_normal = normalize(cross(aux1, aux2));

	vec3 min = vec3(std::min(std::min(_a.x, _b.x), _c.x),
		std::min(std::min(_a.y, _b.y), _c.y),
		std::min(std::min(_a.z, _b.z), _c.z));

	vec3 max = vec3(std::max(std::max(_a.x, _b.x), _c.x),
		std::max(std::max(_a.y, _b.y), _c.y),
		std::max(std::max(_a.z, _b.z), _c.z));

	_bbox = new BBox(min, max);
}

Triangle::Triangle(vec3 a, vec3 b, vec3 c, Material mat)
{
	_a = a;
	_b = b;
	_c = c;
	_material = mat;

	vec3 aux1 = _b - _a;
	vec3 aux2 = _c - _a;

	_normal = normalize(cross(aux1, aux2));

	vec3 min = vec3(std::min(std::min(_a.x, _b.x), _c.x),
		std::min(std::min(_a.y, _b.y), _c.y),
		std::min(std::min(_a.z, _b.z), _c.z));

	vec3 max = vec3(std::max(std::max(_a.x, _b.x), _c.x),
		std::max(std::max(_a.y, _b.y), _c.y),
		std::max(std::max(_a.z, _b.z), _c.z));

	_bbox = new BBox(min, max);
}


Triangle::~Triangle()
{
}

vec3 Triangle::a() {
	return _a;
}

vec3 Triangle::b() {
	return _b;
}

vec3 Triangle::c() {
	return _c;
}

vec3 Triangle::normal() {
	return _normal;
}

Material Triangle::material() {
	return _material;
}

void Triangle::setMaterial(Material mat) {
	_material = mat;
}

HitInfo Triangle::intersect(Ray& ray) {
	HitInfo info;
	info.ray = ray;
	info.material = _material;
	info.normal = _normal;

	float a = _a.x - _b.x, 
		b = _a.x - _c.x, 
		c = ray.direction().x, 
		d = _a.x - ray.origin().x;

	float e = _a.y - _b.y,
		f = _a.y - _c.y,
		g = ray.direction().y,
		h = _a.y - ray.origin().y;

	float i = _a.z - _b.z,
		j = _a.z - _c.z,
		k = ray.direction().z,
		l = _a.z - ray.origin().z;

	float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	float q = g * i - e * k, s = e * j - f * i;

	float inv_denom = 1.0 / (a * m + b * q + c * s);

	float beta = (d*m - b * n - c * p) * inv_denom;
	if (beta < 0.0f) {
		info.t = MISS;
		return info;
	}

	float r = e * l - h * i;
	float gamma = (a*n + d * q + c * r) * inv_denom;

	if (gamma < 0.0f || beta + gamma > 1.0f) {
		info.t = MISS;
		return info;
	}

	float t = (a*p - b * r + d * s) * inv_denom;

	if (t < 0.0f) {
		info.t = MISS;
		return info;
	}

	info.t = t;
	info.intersection = ray.origin() + t * ray.direction();

	return info;
}
