#pragma once
#include <GL/glut.h>
#include "Vector3f.h"

class Camera
{
public:
	Vector3f eye;		//eye location
	Vector3f center;	//reference point
	Vector3f up;		//up vector - horizont

	Camera(void);
	void set(void);
};
