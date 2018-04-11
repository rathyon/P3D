#include "AreaLight.h"

AreaLight::AreaLight(vec3 pos, vec3 color) {
	_pos = pos;
	_color = color;
	_contribution = 1.0f / (float)POINTLIGHTS;
	prepareLights();
}

AreaLight::~AreaLight() {

}

void AreaLight::prepareLights()
{
	float X, Y, Z;
	vec3 lightPosition;

	for (int x = 0; x < AREADIM; x++)
		for (int y = 0; y < AREADIM; y++)
		{
			int idx = x * AREADIM + y;
			X = _pos.x + x * SPACING;
			Y = _pos.y + y * SPACING;
			Z = _pos.z;
			lights[idx] = new Light(vec3(X,Y,Z), _color);
		}
}

vec3 AreaLight::shade(HitInfo& info, std::vector<Object*> objects) {

	vec3 collision_point = info.intersection;
	vec3 N = info.normal;
	vec3 V = -info.ray.direction();

	if (dot(V, N) < 0.0f) {
		N = -N;
	}

	vec3 res = vec3(0.0f);

	for (int i = 0; i < POINTLIGHTS; i++) {

		if (lights[i]->inShadow(info, objects))
			continue;

		// L vector
		vec3 L = normalize(lights[i]->pos() - collision_point);

		// H vector
		vec3 H = (L + V) / ((L + V).length());

		float NdotL = dot(N, L);
		NdotL < 0.0f ? 0.0f : NdotL;

		vec3 diffuse = info.material.color() * info.material.kd() * NdotL * _color;

		float NdotH = dot(N, H);
		NdotH < 0.0f ? 0.0f : NdotH;
		vec3 specular = info.material.color() * info.material.ks() *pow(NdotH, info.material.shininess()) * _color;

		res += (diffuse + specular)*_contribution;
	}

	return res;
}

