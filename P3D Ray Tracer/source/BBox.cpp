#include "BBox.h"

BBox::BBox()
{
}

BBox::BBox(vec3 min, vec3 max) : _min(min), _max(max) { }

BBox::~BBox()
{
}

vec3 BBox::min() {
	return _min;
}
vec3 BBox::max() {
	return _max;
}

void BBox::setMin(vec3 min) {
	_min = min;
}
void BBox::setMax(vec3 max) {
	_max = max;
}

bool BBox::inside(vec3 point) {

	if (point.x > _min.x && point.x < _max.x) {
		if (point.y > _min.y && point.y < _max.y) {
			if (point.z > _min.z && point.z < _max.z)
				return true;
		}
	}
}

HitInfo BBox::intersect(Ray& ray) {
	HitInfo info;
	info.ray = ray;
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
	info.material = Material(vec3(1, 0, 0), 1, 0, 0, 0, 1);

	return info;
}