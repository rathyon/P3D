#include "Material.h"

Material::Material(float r, float g, float b, float kd, float ks, float shininess, float transmitance, float IOR)
{
	_r = r;
	_g = g;
	_b = b;
	_kd = kd;
	_ks = ks;
	_shininess = shininess;
	_transmitance = transmitance;
	_IOR = IOR;
	_diffuse = vec3(_r, _g, _b);
	_specular = vec3(_r*0.10f, _g*0.10f, _b*0.10f);
}


Material::~Material()
{
}

vec3 Material::diffuseColor() {
	return _diffuse;
}

vec3 Material::specularColor() {
	return _specular;
}

float Material::r() {
	return _r;
}

float Material::g() {
	return _g;
}

float Material::b() {
	return _b;
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

void Material::setR(float r) {
	_r = r;
}

void Material::setG(float g) {
	_g = g;
}

void Material::setB(float b) {
	_b = b;
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