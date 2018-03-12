#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Object.h"
#include "Sphere.h"
#include "Light.h"
#include "NFFStructs.h"

class NFFParser
{
private:
	std::string line;
	std::ifstream inputStream;
	CameraParams cameraParams;

public:
	NFFParser();
	NFFParser(const std::string nffToInput);

	void ParseObjectsAndLights(std::vector<Light *> &lights, std::vector <Object *> &objects);
	Camera ParseCamera();
	std::string GetEntityType();
	void CreateEntity(std::string objType, std::vector<Light *> &lights, std::vector <Object *> &objects);
	void ParseCameraParams();
	Camera createCamera();

	void printCameraParams();
};