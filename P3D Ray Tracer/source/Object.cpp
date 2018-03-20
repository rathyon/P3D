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
	vec3 I = info.ray.direction();
	vec3 N = info.normal;
	float NdotI = dot(N, I);
	NdotI < -1.0f ? -1.0f : NdotI;
	NdotI > 1.0f ? 1.0f : NdotI;

	//account for internal reflections...
	if (NdotI > 0.0f) {
		N = -N;
	}
	vec3 R = normalize(I - 2.0f*NdotI*N);

	return Ray(info.intersection + OFFSET * R, R);
}

// REWRITE THIS
Ray Object::refract(HitInfo& info) {
	vec3 N = info.normal;
	vec3 I = info.ray.direction();
	float NdotI = dot(N, I);
	NdotI < -1.0f ? -1.0f : NdotI;
	NdotI > 1.0f ? 1.0f : NdotI;
	float eta_i = 1.0f, eta_t = info.material.IOR();

	if (NdotI < 0.0f) {
		NdotI = -NdotI;
	}
	else {
		N = -info.normal;
		std::swap(eta_i, eta_t);
	}
	float eta = eta_i / eta_t;
	float cos_i = NdotI;
	cos_i < -1.0f ? -1.0f : cos_i;
	cos_i > 1.0f ? 1.0f : cos_i;

	float root_term = 1.0f - (eta*eta) * (1.0f - (cos_i*cos_i));
	if (root_term < 0.0f) {
		// there is no refraction
		return Ray(vec3(0.0f), vec3(0.0f));
	}
	else {
		vec3 T = normalize(eta * I + (eta*NdotI - sqrt(root_term)) * N);
		return Ray(info.intersection + OFFSET * T, T);
	}

}

