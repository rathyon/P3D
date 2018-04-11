#include "Object.h"

Object::Object()
{
}


Object::~Object()
{
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

