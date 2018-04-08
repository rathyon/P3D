#include "RayTracer.h"

//returns the result of the called ray tracing method
vec3 rayTrace(int x, int y) {
	areaTestLight = new AreaLight();
	//call the type of tracing here
	return naiveTrace(x,y);
	//return stochasticTrace(x, y, 4);
	//return jitteringTrace(x, y, 4);
	//return adaptiveTrace((float) x, (float) y); // DONT USE IT YET -> STACK OVERFLOW
}

// casts a single ray per pixel (in the center)
vec3 naiveTrace(int x, int y) {
	return trace(Ray(camera, (float)x, (float)y), DEPTH);
}

// casts several rays per pixel randomly distributed inside "cells" in a "matrix"
vec3 jitteringTrace(int x, int y, int matrix_size) {
	vec3 color = vec3(0.0);

	for (int row = 0; row < matrix_size; row++) {
		for (int column = 0; column < matrix_size; column++) {
			Ray ray = Ray(camera, 
				(float)x + ((float)row + frand()) / (float)matrix_size, (float)y + ((float)column + frand()) / (float) matrix_size);
			color += trace(ray, DEPTH);
		}
	}

	return color / (float)(matrix_size*matrix_size);
}

vec3 stochasticTrace(int x, int y, int matrix_size) {
	vec3 color = vec3(0.0);

	for (int point = 0; point < matrix_size*matrix_size; point++) {
		
			Ray ray = Ray(camera,
				(float)x + frand() , (float)y + frand());
			color += trace(ray, DEPTH);
		
	}

	return color / (float)(matrix_size*matrix_size);
}

// DONT USE IT YET -> STACK OVERFLOW
vec3 adaptiveTrace(Ray lower_left, Ray lower_right, Ray upper_left, Ray upper_right, float x, float y, float size) {
	vec3 ll_color = trace(lower_left, DEPTH);
	vec3 ul_color = trace(upper_left, DEPTH);
	vec3 lr_color = trace(lower_right, DEPTH);
	vec3 ur_color = trace(upper_right, DEPTH);
	bool subdivide = false;

	// optimize this chain of if's
	vec3 ul_ll_diff = abs(ul_color - ll_color);
	if (ul_ll_diff.x + ul_ll_diff.y + ul_ll_diff.z >= adaptiveThreshold)
		subdivide = true;

	vec3 ur_ul_diff = abs(ur_color - ul_color);
	if (ur_ul_diff.x + ur_ul_diff.y + ur_ul_diff.z >= adaptiveThreshold)
		subdivide = true;

	vec3 ur_lr_diff = abs(ur_color - lr_color);
	if (ur_lr_diff.x + ur_lr_diff.y + ur_lr_diff.z >= adaptiveThreshold)
		subdivide = true;

	vec3 lr_ll_diff = abs(lr_color - ll_color);
	if (lr_ll_diff.x + lr_ll_diff.y + lr_ll_diff.z >= adaptiveThreshold)
		subdivide = true;

	if (subdivide) {
		float subsize = size * 0.5;
		Ray center_left = Ray(camera, x, y + (1.0*subsize));
		Ray center_down = Ray(camera, x + (1.0*subsize), y);
		Ray center = Ray(camera, x + (1.0*subsize), y + (1.0*subsize));
		Ray center_up = Ray(camera, x + (1.0*subsize), y + (1.0*size));
		Ray center_right = Ray(camera, x + (1.0*size), y + (1.0*subsize));

		vec3 subcolor1 = adaptiveTrace(lower_left, center_down, center_left, center, x, y, subsize);
		vec3 subcolor2 = adaptiveTrace(center_left, center, upper_left, center_up, x, y + (1.0*subsize), subsize);
		vec3 subcolor3 = adaptiveTrace(center_down, lower_right, center, center_right, x + (1.0*subsize), y, subsize);
		vec3 subcolor4 = adaptiveTrace(center, center_right, center_up, upper_right, x + (1.0*subsize), y + (1.0*subsize), subsize);

		return (subcolor1 + subcolor2 + subcolor3 + subcolor4) / 4.0;
	}
	else {
		return (ll_color + ul_color + ur_color + lr_color) / 4.0;
	}
}

vec3 adaptiveTrace(float x, float y) {
	Ray lower_left = Ray(camera, x, y);
	Ray upper_left = Ray(camera, x, y +1.0);
	Ray lower_right = Ray(camera, x+1.0, y);
	Ray upper_right = Ray(camera, x+1.0, y+1.0);

	return adaptiveTrace(lower_left, lower_right, upper_left, upper_right, x, y, 1.0);
}


//traces a ray
vec3 trace(Ray ray, int depth) {
	HitInfo info; // info.t is by default = MISS

	int target;
	HitInfo new_info;

	for (int i = 0; i < objects.size(); i++) {
		new_info = objects[i]->intersect(ray);

		// ignore if it missed
		if (new_info.t == MISS) {
			continue;
		}
		// if there was a hit before, grab the smallest t
		else if (info.t != MISS && new_info.t < info.t) {
			info = new_info;
			target = i;
		}
		// if nothing has been hit yet, grab the first hit
		else if (info.t == MISS && new_info.t > info.t) {
			info = new_info;
			target = i;
		}
	}

	vec3 color = vec3(0.0f);

	if (info.t != MISS) {
		//for (Light* light : lights) {
		for (Light* light : areaTestLight->lights) {
			// cast shadow feeler
			vec3 origin = info.intersection;
			vec3 L = normalize(light->pos() - origin);
			Ray feeler = Ray(origin + OFFSET * L, L);

			float light_t = (light->pos() - origin).length();
			bool in_shadow = false;

			HitInfo feelerInfo;

			for (int i = 0; i < objects.size(); i++) {
				feelerInfo = objects[i]->intersect(feeler);

				if (feelerInfo.t == MISS) {
					continue;
				}
				// se est� na sombra, disparar um shadow ray por cada point light na area light
				// a cor nesse ponto � dada pela percentagem de point lights em que os raios acertam
				if (feelerInfo.t != MISS && feelerInfo.t < light_t) {
					in_shadow = true;
					break;
				}
			}
			if (!in_shadow) {
				color += objects[target]->shade(*light, info);
			}
		} // end of for each light

		if (depth > 0) {
			if (info.material.transmitance() > 0.0f) {
				Ray refraction = objects[target]->refract(info);
				if (refraction.direction() != vec3(0.0f)) { // vec3(0) is returned when there is no refraction
					color += info.material.transmitance() * trace(refraction, depth - 1);
				}
			}
			if (info.material.ks() > 0.0f && RFL_ON == true) {
				Ray reflection = objects[target]->reflect(info);
				color += info.material.ks() * trace(reflection, depth - 1);
			}

		}
	}
	else {
		color = background_color;
	}
	return color;
}