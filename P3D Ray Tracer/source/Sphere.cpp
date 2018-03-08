#include "Sphere.h"

Sphere::Sphere(vec3 center, float radius)
{
	_center = center;
	_radius = radius;
}


Sphere::~Sphere()
{
}

vec3 Sphere::center() {
	return _center;
}

float Sphere::radius() {
	return _radius;
}


float Sphere::intersect(ray ray) {
	float dist_squared = (_center - ray.origin()).lengthSqr();
	float B = dot(ray.direction(), _center - ray.origin());
	float r2 = _radius*_radius;
	if (dist_squared > r2) {
		if (B < 0.0f) {
			//return false: there is no collision
			return MISS;
		}
	}
	float R = (B*B) - dist_squared + r2;
	if (R < 0.0f) {
		//return false: there is no collision
		return MISS;
	}

	float t;

	if (dist_squared > r2) {
		t = B - sqrt(R);
	}
	else {
		t = B + sqrt(R);
	}

	//print("HIT");

	return t;

}

vec3 Sphere::shade(vec3 light, float t, ray ray) {

	vec3 collision_point = ray.origin() + t*ray.direction();
	vec3 N = (collision_point - _center) / _radius;

	// L vector
	vec3 L = normalize(light - collision_point);

	// H vector
	vec3 V = normalize(ray.origin() - collision_point);
	vec3 H = (L + V) / ((L + V).length());

	float NdotL = dot(N, L);
	NdotL < 0.0f ? 0.0f : NdotL;

	// assuming Kd = 1 and light is white and red sphere
	vec3 diffuse = vec3(0.7f, 0.0f, 0.0f) * NdotL;

	float NdotH = dot(N, H);
	NdotH < 0.0f ? 0.0f : NdotH;
	// assuming Ks = 1 and light is white and red highlight; shininess = random? lol
	vec3 specular = vec3(0.9f, 0.9f, 0.9f)*pow(NdotH, 100);

	return diffuse + specular;

}