#include "Light.h"

Light::Light(vec3 pos, vec3 color) {
	_pos = pos;
	_color = color;
}


Light::~Light()
{
}

vec3 Light::pos() {
	return _pos;
}

vec3 Light::color() {
	return _color;
}
