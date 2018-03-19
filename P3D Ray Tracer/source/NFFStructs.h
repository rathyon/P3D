#pragma once
// file describing the arguments for each possible entity in NFF files
#include "Math\vec3.h"

enum EntityType{ sphere };

struct SphereArgs
{
	float centerX;
	float centerY;
	float centerZ;
	float radius;
};

struct LightArgs
{
	vec3 pos, color;
};

struct PlaneArgs
{
	vec3 point1, point2, point3;
};

struct MaterialProperties
{
	float red, green, blue, Kd, Ks, Shine, T, indexOfRefraction;
};

struct TriangleArgs
{
	vec3 vertice1, vertice2, vertice3;
};

struct From 
{ 
	float Fx, Fy, Fz;
};

struct At 
{
	float Ax, Ay, Az;
};

struct Up
{
	float Ux, Uy, Uz;
};

struct Resolution
{
	float resX, resY;
};

struct CameraParams
{
	From from;
	At at;
	Up up;
	float angle, hither, yon = 10000.0f;
	Resolution res;
};