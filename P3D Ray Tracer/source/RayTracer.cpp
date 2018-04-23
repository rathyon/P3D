#include "RayTracer.h"

//returns the result of the called ray tracing method
vec3 rayTrace(int x, int y) {
	//call the type of tracing here
	return naiveTrace(x,y);
	//return stochasticTrace(x, y, 4);
	//return jitteringTrace(x, y, 4);
	//return adaptiveTrace((float) x, (float) y); // DONT USE IT YET -> STACK OVERFLOW
	//return depthTrace(x, y);
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
//////////////////////////////////////////Ini Depth Tracer//////////////////////
vec3 depthTrace(int x, int y) {
	vec3 color = vec3(0.0);
	vec3 viewPlanePoint = vec3(0.0);
	thinLens.generateRandomSamples(thinLens.getSamples()); //gera o vetor com x samples
	
	vec3 right = thinLens.getRight();
	vec3 up = thinLens.getUp();
	vec3 lookAt = thinLens.getLookAt();
	float h = thinLens.getHeight();
	float w = thinLens.getWidth();
	float ResX = (float)thinLens.getResX();
	float ResY = (float)thinLens.getResY();
	//converter os pontos x e y de ecrã para mundo
	//viewPlanePoint = w * (x / ResX) * right + h * (y / ResY) * up;
	 /*
	viewPlanePoint = w * (x / ResX - 0.5f) * right + h * (y / ResY - 0.5f) * up;

	for (vec3 vec : thinLens.randomSamples) {
		vec3 samp = thinLens.getRadius() * vec;
		vec3 dir = thinLens.rayDirection(viewPlanePoint, samp);
		Ray ray = Ray(thinLens.getPos() + samp, dir);
		color += trace(ray, DEPTH);
	}
	*/

	viewPlanePoint = w * (x / ResX - 0.5f) * right + h * (y / ResY - 0.5f) * up;

	for (vec3 vec : thinLens.randomSamples) {
		vec3 samp = thinLens.getRadius() * vec;
		vec3 dir = thinLens.rayDirection(viewPlanePoint, samp);
		Ray ray = Ray(thinLens.getPos() + samp, dir);
		color += trace(ray, DEPTH);
	}
	/*
	for (vec3 vec : thinLens.randomSamples) {
		vec3 sample = thinLens.getRadius() * vec;
		vec3 dir = thinLens.rayDirection(viewPlanePoint, sample); //vec é a posição random na lente
		Ray ray = Ray((thinLens.getPos()) + sample, dir);
		color += trace(ray, DEPTH);
	}
	*/
	thinLens.clearVector(); //limpa o vetor com pontos random no raio
	return color / (float)thinLens.getSamples();
}
//////////////////////////////////////////Fim Depth Tracer//////////////////////
vec3 stochasticTrace(int x, int y, int matrix_size) {
	vec3 color = vec3(0.0);
	
	for (int point = 0; point < matrix_size*matrix_size; point++) {
		float randomX = frand();
		float randomY = frand();
		color += depthTrace((float)x + randomX, (float)y + randomY);
		/*
		Ray ray = Ray(camera,
		(float)x + randomX , (float)y + randomY);
		
		color += trace(ray, DEPTH);*/
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
	HitInfo new_info;

	if (GRID_ON) {
		if (grid.getBBox()->intersect(ray).t != MISS) {
			info = grid.traverse(ray);
		}
	}
	else {
		for (int i = 0; i < objects.size(); i++) {
			new_info = objects[i]->intersect(ray);

			// ignore if it missed
			if (new_info.t == MISS) {
				continue;
			}
			// if there was a hit before, grab the smallest t
			else if (info.t != MISS && new_info.t < info.t) {
				info = new_info;
			}
			// if nothing has been hit yet, grab the first hit
			else if (info.t == MISS && new_info.t > info.t) {
				info = new_info;
			}
		}
	}

	vec3 color = vec3(0.0f);

	if (info.t != MISS) {
		for (Light* light : lights) {
			if (GRID_ON)
				color += light->shade(info, grid);
			else
				color += light->shade(info, objects);
		}

		if (depth > 0) {
			if (info.material.transmitance() > 0.0f) {
				Ray refraction = refract(info);
				if (refraction.direction() != vec3(0.0f)) { // vec3(0) is returned when there is no refraction
					color += info.material.transmitance() * trace(refraction, depth - 1);
				}
			}
			if (info.material.ks() > 0.0f && RFL_ON == true) {
				Ray reflection = reflect(info);
				color += info.material.ks() * trace(reflection, depth - 1);
			}
		}
	}
	else {
		color = background_color;
	}
	return color;
}