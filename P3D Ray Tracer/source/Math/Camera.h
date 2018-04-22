#pragma once

#include "..\NFFStructs.h"

class Camera {
public:
	Camera(const vec3& pos, const vec3& at, const vec3& up, const float& fovy, const float& near, const float& far,const int& ResX, const int& ResY);
	Camera();

	const vec3& getPos() const;
	const vec3& getAt() const;
	const float& getFOVY() const;
	const float& getNear() const;
	const float& getFar() const;
	const vec3& getRight() const;
	const vec3& getUp() const;
	const vec3& getLookAt() const;
	const float& getFocalDistance() const;
	const float& getWidth() const;
	const float& getHeight() const;
	const int& getResX() const;
	const int& getResY() const;

	void setPos(vec3 pos);
	void setAt(vec3 at);
	void setUp(vec3 up);
	void setFovy(float fovy);
	void setNear(float near);
	void setFar(float far);
	void setResX(int ResX);
	void setResY(int ResY);
	
	vec3 setRight();
	vec3 setUp();
	vec3 setLookAt();
	//float setFocalDistance();
	float setHeight(float focalDistance);
	float setWidth();

private:
	//imported values
	vec3  _pos;
	vec3  _at;
	float _fovy;
	float _near;
	float _far;
	int _resX;
	int _resY;

	//calculated values
	vec3 _right; // X Axis
	vec3 _up; // Y Axis (true up vector)
	vec3 _lookat; // Z Axis
	float _focalDistance; // called "df" - named it something intuitive
	float _height;
	float _width;


};