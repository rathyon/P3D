#pragma once
#include "Math\Math.h"

class Material
{
public:
	Material(vec3 color, float kd, float ks, float shininess, float transmitance, float IOR);
	Material();
	~Material();

	vec3 color();
	float kd();
	float ks();
	float shininess();
	float transmitance();
	float IOR();

	void setColor(vec3 color);
	void setKD(float kd);
	void setKS(float ks);
	void setShininess(float shininess);
	void setTransmitance(float transmitance);
	void setIOR(float IOR);


private:
	vec3 _color;
	float _kd;
	float _ks;
	float _shininess;
	float _transmitance;
	float _IOR;

};

