#include "NFFParser.h"

NFFParser::NFFParser()
{
}

NFFParser::NFFParser::NFFParser(const std::string nffToInput)
{
	inputStream = std::ifstream(nffToInput);
}

void NFFParser::ParseObjectsAndLights(std::vector<Light*> &lights, std::vector<Object*> &objects)
{
	std::string entityType;
	std::string args;

	if (inputStream.is_open())
	{
		while (inputStream.good())
		{
			entityType = GetEntityType();
			CreateEntity(entityType, lights, objects);
			std::cout << " entity type: " << entityType << std::endl;
		}
		inputStream.close();
	}
	else
		std::cout << "ERROR NFFParser: file can't be reached" << std::endl;

	//CreateEntity();
	//AddEntityToVector();
}

void NFFParser::ParseCamera()
{
}

std::string NFFParser::GetEntityType()
{
	std::string entityType;
	inputStream >> entityType;
	return entityType;
}



void NFFParser::CreateEntity(std::string objType, std::vector<Light *> &lights, std::vector <Object *> &objects)
{
	if (!objType.compare("s")) // compare returns 0 if strings are equal
	{
		SphereArgs sphere;
		inputStream >> sphere.centerX;
		inputStream >> sphere.centerY;
		inputStream >> sphere.centerZ;
		inputStream >> sphere.radius;
		std::cout << "sphere! x: " << sphere.centerX << " y: " << sphere.centerY << " z: " << sphere.centerZ << " radius: " << sphere.radius << std::endl;

		
		Material sphereMat = Material(vec3(1.0f, 0.9f, 0.7f), 0.5f, 0.5f, 30.0827f, 0.0f, 1.0f);
		Sphere* sphereObj = new Sphere(vec3(sphere.centerX, sphere.centerY, sphere.centerZ), sphere.radius, sphereMat);
		objects.push_back(sphereObj);
	}
}


