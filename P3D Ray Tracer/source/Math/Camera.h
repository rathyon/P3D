#pragma once

class Camera {
public:
	Camera(const vec3& eye, const vec3& at, const vec3& up, const double& fovy, const double& near, const double& far,const int& ResX, const int& ResY);

	void calculateExtraComponents() const;
	
	const vec3& getDf();
	const vec3& getXe();
	const vec3& getYe();
	const vec3& getZe();
	const float& getWidth();
	const float& getHeight();
	const float& getResX();
	const float& getResY();
	


private:
	//inicializações de camera
	vec3  _eye;
	vec3  _at;
	vec3  _up;
	double _fovy;
	double _near;
	double _far;
	int _ResX;
	int _ResY;

	//Componentes extra a serem calculadas

	vec3 df;
	vec3 Xe;
	vec3 Ye;
	vec3 Ze;
	float height;
	float width;


};