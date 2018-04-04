#include "Light.h"

Light::Light(void)
{
	ambient.set(2.8f, 2.8f, 2.8f, 1.0f);
	diffuse.set(0.8f, 0.8f, 0.8f, 1.0f);
	specular.set(0.5f, 0.5f, 0.5f, 1.0f);
	position.set(-124.50f, 10.86f, 77.30f);
}

void Light::set(void)
{
	GLfloat l_ambient[4] = {ambient.x, ambient.y, ambient.z, ambient.w };
	GLfloat l_diffuse[4] = {diffuse.x, diffuse.y, diffuse.z, diffuse.w };
	GLfloat l_specular[4] = {specular.x, specular.y, specular.z, specular.w };
	GLfloat l_position[3] = {position.x, position.y, position.z };

	glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, l_position);

	glTranslatef(position.x, position.y, position.z);
	glutSolidSphere(3, 20, 20); 
	glTranslatef(-position.x, -position.y, -position.z);
}

