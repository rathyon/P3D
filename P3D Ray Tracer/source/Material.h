#pragma once
#include "Math\Math.h"

class Material
{
public:
	Material(float r, float g, float b, float kd, float ks, float shininess, float transmitance, float IOR);
	Material();
	~Material();

	vec3  diffuseColor();
	vec3  specularColor();
	float r();
	float g();
	float b();
	float kd();
	float ks();
	float shininess();
	float transmitance();
	float IOR();

	void setR(float r);
	void setG(float g);
	void setB(float b);
	void setKD(float kd);
	void setKS(float ks);
	void setShininess(float shininess);
	void setTransmitance(float transmitance);
	void setIOR(float IOR);


private:
	vec3 _diffuse;
	vec3 _specular;
	float _r;
	float _g;
	float _b;
	float _kd;
	float _ks;
	float _shininess;
	float _transmitance;
	float _IOR;

};

