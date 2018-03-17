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

	return Ray(info.intersection + OFFSET*dir, dir,true);
}

Ray Object::refract(HitInfo& info) {
	
	vec3 dir = info.ray.direction();
	vec3 normal = info.normal;
	float IOR = info.material.IOR();
	float iorI = 1, iorT = IOR;
	bool outside = dot(dir, normal)<0;//se for < 0 quer dizer que está fora

	if (!outside) {
		std::swap(iorI, iorT); normal = -normal;//caso esteja dentro, é trocado os IOR e é invertida a normal
	}

	vec3 vt = dot(dir, normal)*normal - dir;

	float sinI = vt.length();
	float sinT = (iorI / iorT)*sinI;
	float cosT = sqrt(1 - sinT * sinT);
	vec3 t = normalize(vt);
	vec3 rT = sinT * t + cosT * (-normal);
	normalize(rT);
	vec3 origin = outside ? info.intersection - OFFSET : info.intersection + OFFSET;
	return Ray(info.intersection + OFFSET, info.t*rT, outside);
	


	
}

