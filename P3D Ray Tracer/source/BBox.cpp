#include "BBox.h"

BBox::BBox(vec3 min, vec3 max) : _min(min), _max(max) { }

BBox::~BBox()
{
}

void BBox::setMaterial(Material& mat) {
	_material = mat;
}

HitInfo BBox::intersect(Ray& ray) {
	HitInfo info;
	info.ray = ray;
	info.material = _material;
	float tMin = FLT_EPSILON; //hygienic value, could be 0.0 and would work fine
	float tMax = FLT_MAX;

	int axis = 0;

	float invDir, tNear, tFar;
	//for each coordinate, x,y,z...
	for (int i = 0; i < 3; i++) {
		// what happens when 1/0? must check...
		invDir = 1.0f / ray.direction()[i];

		tNear = (_min[i] - ray.origin()[i]) * invDir;
		tFar = (_max[i] - ray.origin()[i]) * invDir;

		// order them properly...
		if (tNear > tFar) {
			std::swap(tNear, tFar);
		}

		// get closer to real entry point...
		if (tNear > tMin) {
			tMin = tNear;
			axis = i;
		}
		// get closer to real exit point...
		if (tFar < tMax)
			tMax = tFar;

		// if (min > max) ever occurs, ray misses
		if (tMin > tMax) {
			info.t = MISS;
			return info;
		}
	}
	info.t = tMin;

	//since they're AA boxes...
	if (axis == 0) {
		info.normal = vec3(1.0, 0,0);
	}
	else if (axis == 1) {
		info.normal = vec3(0, 1.0, 0);
	}
	else {
		info.normal = vec3(0, 0, 1.0);
	}

	info.intersection = ray.origin() + tMin * ray.direction();

	return info;
}
