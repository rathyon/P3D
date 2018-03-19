#include "Object.h"

Object::Object()
{
}


Object::~Object()
{
}

vec3 Object::shade(Light& light, HitInfo& info) {

	vec3 collision_point = info.intersection;
	vec3 N = info.normal;
	vec3 V = -info.ray.direction();

	if (dot(V, N) < 0.0f) {
		N = -N;
	}

	// L vector
	vec3 L = normalize(light.pos() - collision_point);

	// H vector
	vec3 H = (L + V) / ((L + V).length());

	float NdotL = dot(N, L);
	NdotL < 0.0f ? 0.0f : NdotL;

	vec3 diffuse = info.material.color() * info.material.kd() * NdotL * light.color();

	float NdotH = dot(N, H);
	NdotH < 0.0f ? 0.0f : NdotH;
	vec3 specular = info.material.color() * info.material.ks() *pow(NdotH, info.material.shininess()) * light.color();

	return diffuse + specular;

}

Ray Object::reflect(HitInfo& info) {
	vec3 R = -info.ray.direction();

	vec3 dir = normalize(2 * (dot(R, info.normal))*info.normal - R);

	return Ray(info.intersection + OFFSET*dir, dir);
}

// REWRITE THIS
Ray Object::refract(HitInfo& info) {
	
	vec3 V = -info.ray.direction();
	vec3 N = info.normal;

	float ior_i = 1.0f;
	float ior_t = info.material.IOR();

	if (dot(V, N) < 0.0f) {
		std::swap(ior_i, ior_t);
		N = -N;
	}

	vec3 VT = dot(V, N)*N - V;

	float sin_i = VT.length();
	float sin_t = (ior_i / ior_t)*sin_i;
	float cos_t = sqrt((1.0f - (sin_t*sin_t)));
	vec3 T = VT / sin_i;
	vec3 R = sin_t * T + cos_t * (-N);
	return Ray(info.intersection + OFFSET * R, R);

}

