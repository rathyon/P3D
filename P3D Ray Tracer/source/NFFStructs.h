#pragma once
// file describing the arguments for each possible entity in NFF files

enum EntityType{ sphere };

struct SphereArgs
{
	float centerX;
	float centerY;
	float centerZ;
	float radius;
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