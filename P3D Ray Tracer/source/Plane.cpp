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
	return _mat;
}

void Plane::setMaterial(Material mat) {
	_mat = mat;
}
float Plane::intersect(ray ray) {
	if (dot(_normal, ray.direction()) == 0.0f) {
		return MISS;
	}
	float t = - (dot((ray.origin() - _a), _normal)) / (dot(_normal, ray.direction()));

	if (t < 0) {
		return MISS;
	}
	return t;

	//print("HIT");
}

vec3 Plane::shade(Light light, ray ray, float t) {

	vec3 collision_point = ray.origin() + t*ray.direction();
	vec3 N = _normal;

	// L vector
	vec3 L = normalize(light.pos() - collision_point);

	// H vector
	vec3 V = normalize(ray.origin() - collision_point);
	vec3 H = (L + V) / ((L + V).length());

	float NdotL = dot(N, L);
	NdotL < 0.0f ? 0.0f : NdotL;

	vec3 diffuse = _mat.color() * _mat.kd() * NdotL * light.color();

	float NdotH = dot(N, H);
	NdotH < 0.0f ? 0.0f : NdotH;
	vec3 specular = _mat.color() * _mat.ks() *pow(NdotH, 100) * light.color();

	return diffuse + specular;

}

