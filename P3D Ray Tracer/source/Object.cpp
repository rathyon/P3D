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

	// L vector
	vec3 L = normalize(light.pos() - collision_point);

	// H vector
	vec3 V = normalize(info.ray.origin() - collision_point);
	vec3 H = (L + V) / ((L + V).length());

	float NdotL = dot(N, L);
	NdotL < 0.0f ? 0.0f : NdotL;

	vec3 diffuse = info.material.color() * info.material.kd() * NdotL * light.color();

	float NdotH = dot(N, H);
	NdotH < 0.0f ? 0.0f : NdotH;
	vec3 specular = info.material.color() * info.material.ks() *pow(NdotH, info.material.shininess()) * light.color();

	return diffuse + specular;

}

Ray Object::reflect(Light& light, HitInfo& info) {
	vec3 L = normalize(light.pos() - info.intersection);

	vec3 dir = normalize(2 * (dot(L, info.normal))*info.normal - L);

	return Ray(info.intersection + OFFSET*dir, dir);
}
