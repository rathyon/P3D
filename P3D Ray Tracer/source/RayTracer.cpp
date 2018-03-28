#include "RayTracer.h"

//returns the result of the called ray tracing method
vec3 rayTrace(int x, int y) {
	//call the type of tracing here
	//return naiveTrace(x,y);
	return stochasticTrace(x, y, 2);
}

// casts a single ray per pixel (in the center)
vec3 naiveTrace(int x, int y) {
	return trace(Ray(camera, (float)x, (float)y), DEPTH);
}

// casts several rays per pixel randomly distributed inside "cells" in a "matrix"
vec3 stochasticTrace(int x, int y, int matrix_size) {
	vec3 color = vec3(0.0);

	for (int row = 0; row < matrix_size; row++) {
		for (int column = 0; column < matrix_size; column++) {
			Ray ray = Ray(camera, 
				(float)x + ((float)row + frand() / (float)matrix_size), 
				(float)y + ((float)column + frand()) / (float) matrix_size);
			color += trace(ray, DEPTH);
		}
	}

	return color / (float)(matrix_size*matrix_size);
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
		for (Light* light : lights) {
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