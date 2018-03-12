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
			//std::cout << " entity type: " << entityType << std::endl;
		}
		inputStream.close();
	}
	else
		std::cout << "ERROR NFFParser: file can't be reached" << std::endl;
}

std::string NFFParser::GetEntityType()
{
	std::string entityType;
	inputStream >> entityType;
	return entityType;
}

void NFFParser::CreateEntity(std::string objType, std::vector<Light *> &lights, std::vector <Object *> &objects)
{
	if (!objType.compare("s")) // SPHERE compare returns 0 if strings are equal
	{
		SphereArgs sphere;
		inputStream >> sphere.centerX;
		inputStream >> sphere.centerY;
		inputStream >> sphere.centerZ;
		inputStream >> sphere.radius;
		//std::cout << "sphere! x: " << sphere.centerX << " y: " << sphere.centerY << " z: " << sphere.centerZ << " radius: " << sphere.radius << std::endl;

		Material sphereMat = Material(vec3(1.0f, 0.9f, 0.7f), 0.5f, 0.5f, 30.0827f, 0.0f, 1.0f);
		Sphere* sphereObj = new Sphere(vec3(sphere.centerX, sphere.centerY, sphere.centerZ), sphere.radius, sphereMat);
		objects.push_back(sphereObj);
	}
}

Camera NFFParser::ParseCamera()
{
	std::string entityType;

	if (inputStream.is_open())
	{
		while (inputStream.good())
		{
			ParseCameraParams();
		}
		//printCameraParams();
		inputStream.close();
	}
	else
		std::cout << "ERROR NFFParser: file can't be reached" << std::endl;

	//return Camera(vec3(2.1f, 1.3f, 1.7f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), 45.0f, 0.01f, 1000.0f, 512, 512);
	return createCamera();
}

void NFFParser::ParseCameraParams()
{
	std::string str;
	inputStream >> str;

	if (!str.compare("from"))
	{
		inputStream >> cameraParams.from.Fx;
		inputStream >> cameraParams.from.Fy;
		inputStream >> cameraParams.from.Fz;
	}
	else if(!str.compare("at"))
	{
		inputStream >> cameraParams.at.Ax;
		inputStream >> cameraParams.at.Ay;
		inputStream >> cameraParams.at.Az;
	}
	else if (!str.compare("up"))
	{
		inputStream >> cameraParams.up.Ux;
		inputStream >> cameraParams.up.Uy;
		inputStream >> cameraParams.up.Uz;
	}
	else if (!str.compare("angle"))
	{
		inputStream >> cameraParams.angle;
	}
	else if (!str.compare("hither"))
	{
		inputStream >> cameraParams.hither;
	}
	else if (!str.compare("resolution"))
	{
		inputStream >> cameraParams.res.resX;
		inputStream >> cameraParams.res.resY;
	}
}

Camera NFFParser::createCamera()
{
	From from = cameraParams.from;
	At at = cameraParams.at;
	Up up = cameraParams.up;
	float angle = cameraParams.angle;
	float hither = cameraParams.hither;
	float yon = 10000.0f;
	Resolution res = cameraParams.res;

	return Camera(vec3(from.Fx, from.Fy, from.Fz),
					vec3(at.Ax, at.Ay, at.Az),
					vec3(up.Ux, up.Uy, up.Uz),
					angle, hither, yon,
					res.resX, res.resY);
}

void NFFParser::printCameraParams()
{
	std::cout << "From x: " << cameraParams.from.Fx << " y: " << cameraParams.from.Fy << " z: " << cameraParams.from.Fz << std::endl;
	std::cout << "At x: " << cameraParams.at.Ax << " y: " << cameraParams.at.Ay << " z: " << cameraParams.at.Az << std::endl;
	std::cout << "Up x: " << cameraParams.up.Ux << " y: " << cameraParams.up.Uy << " z: " << cameraParams.up.Uz << std::endl;
	std::cout << "Angle: " << cameraParams.angle << std::endl;
	std::cout << "Hither: " << cameraParams.hither << std::endl;
	std::cout << "res x: " << cameraParams.res.resX << " y: " << cameraParams.res.resY << std::endl;
}


