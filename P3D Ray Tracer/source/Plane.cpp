#include "Plane.h"

Plane::Plane(vec3 a, vec3 b, vec3 c) {
	_a = a;
	_b = b;
	_c = c;
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

void Plane::setMaterial(float r, float g, float b, float kd, float ks, float shininess, float transmitance, float IOR) {
	/*
	material.setR(r); 
	material.setG(g);
	material.setB(b);
	material.setKD(kd);
	material.setKS(ks);
	material.setShininess(shininess);
	material.setTransmitance(transmitance);
	material.setIOR(IOR);
	*/
	material = Material(r, g, b, kd, ks, shininess, transmitance, IOR);
}
float Plane::intersect(ray ray) {
	vec3 aux1 = _b - _a;
	vec3 aux2 = _c - _a;

	normal = normalize(cross(aux1, aux2));

	if (dot(normal, ray.direction()) == 0.0f) {
		return MISS;
	}
	float t = (dot((ray.origin() - _a), normal)) / (dot(normal, ray.direction()));

	if (t < 0) {
		return MISS;
	}
	return t;

	//print("HIT");
}
/*
vec3 Plane::shade(Light light, vec3 intersectPoint, ray ray) {

	//Difuse component
	vec3 lightDir = light.origin - intersectPoint;
	float distance = lightDir.length();
	lightDir.normalize();
	float NdotL = dot(normal,lightDir);
	NdotL = NdotL < 0 ? 0.0f : NdotL;
	vec3 diffuseComponent = material.kd() * material.diffuseColor() * light.diffuseColor() * NdotL;

	//Specular component
	vec3 H = normalize(lightDir + ray.direction);
	float NDotH = dot(normal, H);
	vec3 specularComponent = material.ks()*light.specularColor()*material.specularColor()*pow(NDotH,material.shininess);

	//Ambient component
	//vec3 ambientComponent = ka * material.kd() * light.diffuseColor() * material.diffuseColor();

	return diffuseComponent + specularComponent /*+ ambientComponent*/;

