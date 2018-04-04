#pragma once
#include <GL/glut.h>
#include "Vector3f.h"
#include "Vector4f.h"


class Light
{
public:
	Vector4f ambient;
	Vector4f diffuse;
	Vector4f specular;
	Vector3f position;

	Light(void);
	void set(void);
};
