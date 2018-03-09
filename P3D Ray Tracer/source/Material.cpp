#include "Material.h"

Material::Material(vec3 color, float kd, float ks, float shininess, float transmitance, float IOR)
{
	_color = color;
	_kd = kd;
	_ks = ks;
	_shininess = shininess;
	_transmitance = transmitance;
	_IOR = IOR;
}

Material::Material() {

}


Material::~Material()
{
}

vec3 Material::color() {
	return _color;
}

float Material::kd() {
	return _kd;
}

float Material::ks() {
	return _ks;
}

float Material::shininess() {
	return _shininess;
}

float Material::transmitance() {
	return _transmitance;
}

float Material::IOR() {
	return _IOR;
}

//SETTERS

void Material::setColor(vec3 color) {
	_color = color;
}

void Material::setKD(float kd) {
	_kd = kd;
}

void Material::setKS(float ks) {
	_ks =ks;
}

void Material::setShininess(float shininess) {
	_shininess = shininess;
}

void Material::setTransmitance(float transmitance) {
	_transmitance = transmitance;
}

void Material::setIOR(float IOR) {
	_IOR = IOR;
}