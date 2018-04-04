#include "Camera.h"

Camera::Camera(void)
{
	eye.set(-209.57f, 0.86f, 87.54f);
	center.set(-229.57f, 10.86f, 77.54f);
	up.set(0.0f, 0.0f, 1.0f);
}

void Camera::set(void)
{
	gluLookAt(	eye.x, eye.y, eye.z,
				center.x, center.y, center.z,
				up.x, up.y, up.z );
}