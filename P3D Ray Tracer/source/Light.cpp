#include "Light.h"

Light::Light() {
}

Light::Light(vec3 pos, vec3 color) {
	_pos = pos;
	_color = color;
}

Light::~Light()
{
}

vec3 Light::pos() {
	return _pos;
}

vec3 Light::color() {
	return _color;
}

bool Light::inShadow(HitInfo& info, std::vector<Object*> objects) {

	vec3 origin = info.intersection;
	vec3 L = normalize(_pos - origin);
	Ray feeler = Ray(origin + OFFSET * L, L);

	float light_t = (_pos - origin).length();

	HitInfo feelerInfo;

	for (int i = 0; i < objects.size(); i++) {
		feelerInfo = objects[i]->intersect(feeler);

		if (feelerInfo.t == MISS) {
			continue;
		}
		else if (feelerInfo.t < light_t) {
			return true;
		}
	}
	return false;
}

vec3 Light::shade(HitInfo& info, std::vector<Object*> objects) {

	if (inShadow(info, objects))
		return vec3(0.0f);

	vec3 collision_point = info.intersection;
	vec3 N = info.normal;
	vec3 V = -info.ray.direction();

	if (dot(V, N) < 0.0f) {
		N = -N;
	}

	// L vector
	vec3 L = normalize(_pos - collision_point);

	// H vector
	vec3 H = (L + V) / ((L + V).length());

	float NdotL = dot(N, L);
	NdotL < 0.0f ? 0.0f : NdotL;

	vec3 diffuse = info.material.color() * info.material.kd() * NdotL * _color;

	float NdotH = dot(N, H);
	NdotH < 0.0f ? 0.0f : NdotH;
	vec3 specular = info.material.color() * info.material.ks() *pow(NdotH, info.material.shininess()) * _color;

	return diffuse + specular;

}
