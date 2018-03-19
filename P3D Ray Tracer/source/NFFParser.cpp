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
	if (!objType.compare("f"))
	{
		inputStream >> materialToUse.red;
		inputStream >> materialToUse.green;
		inputStream >> materialToUse.blue;
		inputStream >> materialToUse.Kd; 
		inputStream >> materialToUse.Ks;
		inputStream >> materialToUse.Shine;
		inputStream >> materialToUse.T;
		inputStream >> materialToUse.indexOfRefraction;
	}
	else if (!objType.compare("s")) // SPHERE compare returns 0 if strings are equal
	{
		SphereArgs sphere;
		inputStream >> sphere.centerX;
		inputStream >> sphere.centerY;
		inputStream >> sphere.centerZ;
		inputStream >> sphere.radius;
		//std::cout << "sphere! x: " << sphere.centerX << " y: " << sphere.centerY << " z: " << sphere.centerZ << " radius: " << sphere.radius << std::endl;

		Material sphereMat = GetMaterial();
		Sphere* sphereObj = new Sphere(vec3(sphere.centerX, sphere.centerY, sphere.centerZ), sphere.radius, sphereMat);
		objects.push_back(sphereObj);
	}
	else if (!objType.compare("l"))
	{
		LightArgs light;

		inputStream >> light.pos.x;
		inputStream >> light.pos.y;
		inputStream >> light.pos.z;

		inputStream >> light.color.x;
		inputStream >> light.color.y;
		inputStream >> light.color.z;

		Light* lightObj = new Light(light.pos, light.color);
		lights.push_back(lightObj);
	}
	else if (!objType.compare("pl"))
	{
		PlaneArgs plane;

		inputStream >> plane.point1.x;
		inputStream >> plane.point1.y;
		inputStream >> plane.point1.z; 
		inputStream >> plane.point2.x;
		inputStream >> plane.point2.y;
		inputStream >> plane.point2.z; 
		inputStream >> plane.point3.x;
		inputStream >> plane.point3.y;
		inputStream >> plane.point3.z;

		Plane* planeObj = new Plane(plane.point1, plane.point2, plane.point3 );

		Material planeMat = GetMaterial();
		planeObj->setMaterial(planeMat);
		objects.push_back(planeObj);
	}
	else if (!objType.compare("p"))
	{
		int numberOfVertices;
		inputStream >> numberOfVertices;
		switch(numberOfVertices)
		{
		case TRIANGLE:
			TriangleArgs triangle;

			inputStream >> triangle.vertice1.x;
			inputStream >> triangle.vertice1.y;
			inputStream >> triangle.vertice1.z;

			inputStream >> triangle.vertice2.x;
			inputStream >> triangle.vertice2.y;
			inputStream >> triangle.vertice2.z;

			inputStream >> triangle.vertice3.x;
			inputStream >> triangle.vertice3.y;
			inputStream >> triangle.vertice3.z;

			Triangle* triangleObj = new Triangle(triangle.vertice1, triangle.vertice2, triangle.vertice3);

			Material triangleMat = GetMaterial();
			triangleObj->setMaterial(triangleMat);
			objects.push_back(triangleObj);

			//std::cout << "vertice 1: " << triangleObj->a() << "vertice 2: " << triangleObj->b() << "vertice 3: " << triangleObj->c() << std::endl;
			break;
		}
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

void NFFParser::ParseBackgroundColor(vec3 & backgroundColorToSet)
{
	std::string str;
	inputStream >> str;
	vec3 backgroundColor;

	if (!str.compare("b"))
	{
		inputStream >> backgroundColor.x;
		inputStream >> backgroundColor.y;
		inputStream >> backgroundColor.z;
		backgroundColorToSet = backgroundColor;
		return;
	}
	else backgroundColorToSet = vec3(0.078, 0.361, 0.753);
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

Material NFFParser::GetMaterial()
{
	return Material(vec3(materialToUse.red, materialToUse.green, materialToUse.blue), materialToUse.Kd, materialToUse.Ks, materialToUse.Shine, materialToUse.T, materialToUse.indexOfRefraction);
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


