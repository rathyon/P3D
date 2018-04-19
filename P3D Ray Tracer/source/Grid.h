#pragma once
#include "Object.h"
#include "BBox.h"
#include <vector>

#define GRID_OFFSET 0.0001f
#define CELL_FACTOR 2.0f

struct Cell {
	std::vector<Object*> objects;
};

class Grid
{
public:
	Grid();
	~Grid();

	BBox* getBBox();
	int index(int ix, int iy, int iz);

	void computeGrid(std::vector<Object*> objects);
	HitInfo traverse(Ray& ray);

private:
	BBox* _bbox;
	vec3 divs;
	std::vector<Cell> cells;

	//intersects a ray with a cell
	HitInfo intersect(Cell cell, Ray& ray);
};

