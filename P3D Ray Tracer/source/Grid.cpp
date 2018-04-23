#include "Grid.h"

Grid::Grid()
{
	_bbox = new BBox();
}


Grid::~Grid()
{
}

BBox* Grid::getBBox() {
	return _bbox;
}

int Grid::index(int ix, int iy, int iz) {
	return ix + divs.x*iy + divs.x*divs.y*iz;
}

HitInfo Grid::intersect(Cell cell, Ray& ray) {
	HitInfo info, new_info;
	Object* target = nullptr;
	for (int i = 0; i < cell.objects.size(); i++) {
		new_info = cell.objects[i]->intersect(ray);

		// ignore if it missed
		if (new_info.t == MISS) {
			continue;
		}
		// if there was a hit before, grab the smallest t
		else if (info.t != MISS && new_info.t < info.t) {
			info = new_info;
			//std::cout << "Found a smaller t: " << info.t << std::endl;
		}
		// if nothing has been hit yet, grab the first hit
		else if (info.t == MISS && new_info.t > info.t) {
			info = new_info;
			//std::cout << "First hit t: " << info.t << std::endl;
		}
	}

	/** /
	if (info.t != MISS) {
		std::cout << "Hit something: " << ray.direction() << std::endl;
		std::cin.ignore();
	}
	/**/

	return info;
}

void Grid::computeGrid(std::vector<Object*> objects) {
	vec3 min = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 max = vec3(FLT_MIN, FLT_MIN, FLT_MIN);

	int objCount = objects.size();

	for (int i = 0; i < objCount; i++) {
		BBox* bbox = objects[i]->getBBox();

		min.x = std::min(min.x, bbox->min().x);
		min.y = std::min(min.y, bbox->min().y);
		min.z = std::min(min.z, bbox->min().z);

		max.x = std::max(max.x, bbox->max().x);
		max.y = std::max(max.y, bbox->max().y);
		max.z = std::max(max.z, bbox->max().z);

		min = min - GRID_OFFSET;
		max = max + GRID_OFFSET;

		_bbox->setMin(min);
		_bbox->setMax(max);
	}

	vec3 dimension = max - min;
	float volume = dimension.x * dimension.y * dimension.z;
	float density = cbrt(volume / (float)objCount);

	divs.x = (int)trunc(CELL_FACTOR * dimension.x / density) + 1.0f;
	divs.y = (int)trunc(CELL_FACTOR * dimension.y / density) + 1.0f;
	divs.z = (int)trunc(CELL_FACTOR * dimension.z / density) + 1.0f;

	cells.resize(divs.x*divs.y*divs.z);

	for (int i = 0; i < objCount; i++) {
		BBox* bbox = objects[i]->getBBox();

		int ixmin = (int) clamp((bbox->min().x - _bbox->min().x) * divs.x / (_bbox->max().x - _bbox->min().x), 0.0f, divs.x - 1.0f);
		int iymin = (int) clamp((bbox->min().y - _bbox->min().y) * divs.y / (_bbox->max().y - _bbox->min().y), 0.0f, divs.y - 1.0f);
		int izmin = (int) clamp((bbox->min().z - _bbox->min().z) * divs.z / (_bbox->max().z - _bbox->min().z), 0.0f, divs.z - 1.0f);

		int ixmax = (int) clamp((bbox->max().x - _bbox->min().x) * divs.x / (_bbox->max().x - _bbox->min().x), 0.0f, divs.x - 1.0f);
		int iymax = (int) clamp((bbox->max().y - _bbox->min().y) * divs.y / (_bbox->max().y - _bbox->min().y), 0.0f, divs.y - 1.0f);
		int izmax = (int) clamp((bbox->max().z - _bbox->min().z) * divs.z / (_bbox->max().z - _bbox->min().z), 0.0f, divs.z - 1.0f);

		/** /
		std::cout << ixmin << std::endl;
		std::cout << iymin << std::endl;
		std::cout << izmin << std::endl;
		std::cout << ixmax << std::endl;
		std::cout << iymax << std::endl;
		std::cout << izmax << std::endl;
		std::cin.ignore();
		/**/

		for (int z = izmin; z <= izmax; z++) {
			for (int y = iymin; y <= iymax; y++) {
				for (int x = ixmin; x <= ixmax; x++) {
					cells[index(x, y, z)].objects.push_back(objects[i]);
				}
			}
		}
	}

	/** /
	std::cout << "Number of cells: " << cells.size();
	for (int i = 0; i < cells.size(); i++) {
		//std::cout << "*** Cell " << i << " ***" << std::endl;
		std::cout << "Objects: " << cells[i].objects.size() << std::endl;
		//for (int j = 0; j < cells[i].objects.size(); j++) {
			//std::cout << "Ref: " << cells[i].objects[j] << std::endl;
		//}
	}
	std::cin.ignore();
	/**/
}

// DESPERATELY NEEDS OPTIMIZATION!!!!
HitInfo Grid::traverse(Ray& ray) {
	vec3 tmin = vec3((_bbox->min() - ray.origin()) / ray.direction());
	vec3 tmax = vec3((_bbox->max() - ray.origin()) / ray.direction());

	if (tmin.x > tmax.x) std::swap(tmin.x, tmax.x);
	if (tmin.y > tmax.y) std::swap(tmin.y, tmax.y);
	if (tmin.z > tmax.z) std::swap(tmin.z, tmax.z);

	vec3 idx;

	//if ray is inside grid, calc the cell where it starts
	if (_bbox->inside(ray.origin())) {
		idx = ((ray.origin() - _bbox->min())*divs) / (_bbox->max() - _bbox->min());
		idx.x = clamp(idx.x, 0, divs.x - 1);
		idx.y = clamp(idx.y, 0, divs.y - 1);
		idx.z = clamp(idx.z, 0, divs.z - 1);
	}
	//ray is outside grid, calc nearest cell to intersection point
	else {
		// intersection between ray and bbox
		vec3 p = _bbox->intersect(ray).intersection;
		idx = ((p - _bbox->min())*divs) / (_bbox->max() - _bbox->min());
		idx.x = clamp(idx.x, 0, divs.x - 1);
		idx.y = clamp(idx.y, 0, divs.y - 1);
		idx.z = clamp(idx.z, 0, divs.z - 1);
	}

	vec3 dt = (tmax - tmin) / divs;
	vec3 t_next;

	int i_step[3];
	int i_stop[3];

	// compute t_next for starting cells
	for (int i = 0; i < 3; i++) {
		if (ray.direction()[i] > 0.0f) {
			t_next[i] = tmin[i] + (idx[i] + 1)*dt[i];
			i_step[i] = 1;
			i_stop[i] = divs[i];
		}
		else if (ray.direction()[i] < 0.0f) {
			t_next[i] = tmin[i] + (divs[i] - idx[i])*dt[i];
			i_step[i] = -1;
			i_stop[i] = -1;
		}
		else {
			t_next[i] = FLT_MAX; //ray never crosses this plane
			i_step[i] = -1;
			i_stop[i] = -1;
		}
	}

	Cell cell;
	HitInfo miss; //default "constructor" = MISS
	float initial_t = _bbox->intersect(ray).t;

	/** /
	std::cout << "INITIAL T " << initial_t << std::endl;
	std::cout << "T NEXT    " << t_next << std::endl;
	std::cout << "DT        " << dt << std::endl;
	//std::cin.ignore();
	/**/


	// info.t < t_next[i] ALWAYS FAILING!!
	while (true) {
		cell = cells[index(idx[0], idx[1], idx[2])];

		// for X Axis
		if (t_next[0] < t_next[1] && t_next[0] < t_next[2]) {

			HitInfo info = intersect(cell, ray);

			/** /
			if (info.t != MISS) {
				std::cout << info.t << std::endl;
				std::cout << t_next[0] << std::endl;
				std::cin.ignore();
			}
			/**/

			if (info.t != MISS && info.t < t_next[0]) {
				return info;
			}
			t_next[0] += dt[0];
			idx[0] += i_step[0];
			if (idx[0] >= i_stop[0]) {
				return miss;
			}
		}

		else {
			// for Y Axis
			if (t_next[1] < t_next[2]) {

				HitInfo info = intersect(cell, ray);

				/** /
				if (info.t != MISS) {
					std::cout << info.t << std::endl;
					std::cout << t_next[1] << std::endl;
					std::cin.ignore();
				}
				/**/

				if (info.t != MISS && info.t < t_next[1]) {
					return info;
				}
				t_next[1] += dt[1];
				idx[1] += i_step[1];
				if (idx[1] >= i_stop[1]) {
					return miss;
				}
			}
			//for Z Axis
			else {
				HitInfo info = intersect(cell, ray);

				/** /
				if (info.t != MISS) {
					std::cout << info.t << std::endl;
					std::cout << t_next[2] << std::endl;
					std::cin.ignore();
				}
				/**/

				if (info.t != MISS && info.t < t_next[2]) {
					return info;
				}

				t_next[2] += dt[2];
				idx[2] += i_step[2];
				if (idx[2] >= i_stop[2]) {
					return miss;
				}
			}
		}
	}
}