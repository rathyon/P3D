#pragma once

class Camera {
public:
	Camera(const vec3& pos, const vec3& at, const vec3& up, const float& fovy, const float& near, const float& far,const int& ResX, const int& ResY);
	
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