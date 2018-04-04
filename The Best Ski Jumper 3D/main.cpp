#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <IL/il.h>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include "structures.h"
#include "cObj.h"
#include "Camera.h"
#include "Light.h"
#include "Hill.h"
#include "Jumper.h"
#include "Wind.h"
using namespace std;

#define M_PI 3.14159265358979323846

//maxymalny kat pomiedzy nartami a narciarzem w locie (w stopniach)
const float MAX_ANGLE_VARIANCE = 90.f;
//minimalny kat pomiedzy nartami a narciarzem w locie (w stopniach)
const float MIN_ANGLE_VARIANCE = 10.f;


Camera camera;
Light light;
Hill hill;
Jumper jumper;
Wind wind;


int camera_type = 1;


void drawGround()
{
    GLfloat extent      = 590.0f;
    GLfloat stepSize    = 20.0f;
    GLfloat groundLevel = -50.0f;
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
	glVertex3f(-extent, -extent, groundLevel-0.1f);
    glVertex3f(-extent, extent, groundLevel-0.1f);
    glVertex3f(extent, extent, groundLevel-0.1f);
    glVertex3f(extent, -extent, groundLevel-0.1f);
    glEnd();
	glBegin(GL_QUADS);
	static int color = 1;
	color = 1;
	for(GLint z = GLint(-extent); z < extent; z += GLint(stepSize) ){
		for(GLint x = GLint(-extent); x < extent; x += GLint(stepSize) ){
			color%=2;
			color++;
			if(color==1){
				glColor3f(0.9f, 0.9f, 0.9f);
			}else{
				glColor3f(0.95f, 0.95f, 0.95f);
			}
			glVertex3f(x+1.0f, z+1.0f, groundLevel);
			glVertex3f(x+1.0f, z+stepSize-1.0f, groundLevel);
			glVertex3f(x+stepSize-1.0f, z+stepSize-1.0f, groundLevel);
			glVertex3f(x+stepSize-1.0f, z+1.0f, groundLevel);
		}
	}
    glEnd();
	glEnable(GL_TEXTURE_2D);
}

void handleUpKeypress(unsigned char key, int x, int y) {
	switch(key){
		case 'p':
		case 'P':
			jumper.stop = !jumper.stop;
			break;
	}
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
		case 'w':
            break;
		case ' ':
			if(!jumper.computer){
				if(jumper.on_bar){
					jumper.on_bar = false;
				}else if(!jumper.jumped){
					jumper.jump();
				}
			}
			break;
	}
}


bool UP = false;
bool DOWN = false;
bool LEFT = false;
bool RIGHT = false;

void keyboard_check(){
	if(UP){
		if(jumper.jumped && abs(jumper.skis_angle - jumper.jumper_angle) < MAX_ANGLE_VARIANCE){
			jumper.jumper_angle -= 0.3f;
			if(jumper.jumper_angle<=-180.f)
				jumper.jumper_angle=-jumper.jumper_angle;
		}
	}
	if(DOWN){
		if(jumper.jumped && abs(jumper.skis_angle - jumper.jumper_angle)  > MIN_ANGLE_VARIANCE ){
			jumper.jumper_angle += 0.3f;
			if(jumper.jumper_angle>=180.f)
				jumper.jumper_angle=-jumper.jumper_angle;
		}
	}
	if(LEFT){
		if(jumper.jumped && abs(jumper.skis_angle - jumper.jumper_angle) > MIN_ANGLE_VARIANCE){
			jumper.skis_angle -= 0.3f;
			if(jumper.skis_angle<=-180.f)
				jumper.skis_angle=-jumper.skis_angle;
		}
	}
	if(RIGHT){
		if(jumper.jumped && abs(jumper.skis_angle - jumper.jumper_angle) < MAX_ANGLE_VARIANCE){
			jumper.skis_angle += 0.3f;
			if(jumper.skis_angle>=180.f)
				jumper.skis_angle=-jumper.skis_angle;
		}
	}

}


void handleSpecialUpKeypress(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:
			if(!jumper.computer) UP = false;
			break;
		case GLUT_KEY_DOWN:
			if(!jumper.computer) DOWN = false;
			break;
		case GLUT_KEY_LEFT:
			if(!jumper.computer) LEFT = false;
			break;
		case GLUT_KEY_RIGHT:
			if(!jumper.computer) RIGHT = false;
			break;
		case GLUT_KEY_F8:
			jumper.stop = !jumper.stop;
			break;
	}
}

void handleSpecialKeypress(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_F1:
			camera_type = 1;
			break;
		case GLUT_KEY_F2:
			camera_type = 2;
			break;
		case GLUT_KEY_F3:
			camera_type = 3;
			break;
		case GLUT_KEY_F4:
			camera_type = 4;
			break;
		case GLUT_KEY_UP:
			if(!jumper.computer) UP = true;
			break;
		case GLUT_KEY_DOWN:
			if(!jumper.computer) DOWN = true;
			break;
		case GLUT_KEY_LEFT:
			if(!jumper.computer) LEFT = true;
			break;
		case GLUT_KEY_RIGHT:
			if(!jumper.computer) RIGHT = true;
			break;
	}
}

/************************************************************************
 *
 ************************************************************************/
 
typedef struct {
    int width;
	int height;
	char* title;
 
	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;
glutWindow win;


/***************************************************************************
 * 
 ***************************************************************************/
 
void render_string(int x, int y, string s, void* font){
	if(font==GLUT_BITMAP_TIMES_ROMAN_24){
		glColor3f(1.0f,0.0f,0.0f);
	}else{
		glColor3f(0.0f,0.0f,0.0f);
	}
	glRasterPos2i(x, y);
	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
}


void text()
{
	glDisable(GL_LIGHT0);
	stringstream sstring;
	sstring.precision(4);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, win.width, 0.0, win.height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	static unsigned int bg_width = 225;
	static unsigned int bg_height = 55;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	  glColor4f(1.f,1.f,1.f,0.8f);
	  glVertex2i(0, win.height);
	  glVertex2i(0, win.height-bg_height);
      glVertex2i(bg_width,win.height-bg_height);
      glVertex2i(bg_width, win.height);

	  if(jumper.stop){
		glVertex2i(win.width/2 - 40, win.height/2 - 20);
		glVertex2i(win.width/2 - 40, win.height/2 + 20);
		glVertex2i(win.width/2 + 40, win.height/2 + 20);
		glVertex2i(win.width/2 + 40, win.height/2 - 20);
	  }
    glEnd();
	glEnable(GL_DEPTH_TEST);

	bg_width=255;
	bg_height=55;

	//Informacja czy jest obecnie wcisnieta pasua (F8) czy nie:
	if(jumper.stop){
		render_string(win.width/2 - 30,win.height/2-9,"Pauza",GLUT_BITMAP_TIMES_ROMAN_24);
	}

	//Informacja o punkcie konstrukcyjnym
	render_string(10,win.height-15,"Skocznia POLSKA - K105",GLUT_BITMAP_9_BY_15);

	//Informacja o graczu, ktory skacze:
	string gracz = "Gracz ";
	gracz += (jumper.computer?"komputerowy":"ludzki");
	if(jumper.computer){
		gracz+=" - ";
		gracz+=(jumper.computer_level==1?"latwy":(jumper.computer_level==2?"sredni":"trudny"));
	}
	render_string(10,win.height-30,gracz,GLUT_BITMAP_9_BY_15);

	//Informacja o obecnej predkosci:
	float v = jumper.v*3.6f;
	if(v < 0.1f || jumper.on_bar) v = 0.0f;
	sstring << "Predkosc: " << v << "km/h";
	render_string(10,win.height-45,sstring.str(),GLUT_BITMAP_9_BY_15);
	sstring.str("");


	if(jumper.jumped){
		bg_height=70;
		bg_width=275;
		sstring<<"Predkosc na progu: " << jumper.jump_speed <<"km/h";
		render_string(10,win.height-60,sstring.str(),GLUT_BITMAP_9_BY_15);
		sstring.str("");
	}

	//Informacja o dlugosci skoku:
	if(jumper.landed){
		bg_height=85;
		sstring<<"Dlugosc skoku: " << jumper.jump_length <<"m";
		if(jumper.fell_down) sstring<<" - upadek";
		render_string(10,win.height-75,sstring.str(),GLUT_BITMAP_9_BY_15);
		sstring.str("");
	}
	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();
}

void display() 
{	

	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);		//no idea why, without this text print text function won't work
	glLoadIdentity();

	
	camera.set();
	light.set();

	glPushMatrix();
	
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	drawGround();
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);

	glPopMatrix();

	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	hill.render();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	jumper.render();
	
	text();

	glutSwapBuffers();

}

 void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 0.1f, 10000.0f);
}


 
void setupPerspective(int width, int height, double fovy, double aspect, double zNear, double zFar) {
	double xmin, xmax, ymin, ymax;
	ymax = zNear * tan(fovy * M_PI / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}





void initialize () 
{
    ilInit();
    ilClearColour( 255, 255, 255, 000 );

	setupPerspective(win.width, win.height, 60.0f, float(win.width)/float(win.height), 1.0f, 2500.0f);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	jumper.init();
	hill.init();

	srand(unsigned int(time(NULL)));
	
}


void ai_landing(){
	UP = false;
	DOWN = false;
	LEFT = false;
	RIGHT = false;

	float d_X = hill.points[jumper.hill_points_index+1].x - hill.points[jumper.hill_points_index].x;
	float d_Z = hill.points[jumper.hill_points_index+1].z - hill.points[jumper.hill_points_index].z;

	float tg = -(d_Z/d_X);
	float kat_nachylenia_stoku = tg * 180.f / PI;


	if(kat_nachylenia_stoku-jumper.jumper_angle < MIN_JUMPER_ANGLE+5.0f){
		UP = true;
	}else if(kat_nachylenia_stoku-jumper.jumper_angle > MAX_JUMPER_ANGLE-5.0f){
		DOWN = true;
	}

	if(kat_nachylenia_stoku-jumper.skis_angle < MIN_SKIS_ANGLE+5.0f){
		LEFT = true;
	}else if(kat_nachylenia_stoku-jumper.skis_angle > MAX_SKIS_ANGLE-5.0f){
		RIGHT = true;
	}
	
}

void ai_proper_position(){
	if(fabs(-jumper.angle - jumper.jumper_angle) > 28.f){
		DOWN = true;
	}else if(fabs(-jumper.angle - jumper.jumper_angle) < 23.f){
		UP = true;
	}

	if(fabs(-jumper.angle - jumper.skis_angle) > 38.f){
		RIGHT = true;
	}else if(fabs(-jumper.angle - jumper.skis_angle) < 33.f){
		LEFT = true;
	}

}

clock_t ai_start;
void ai(){
	if(jumper.computer){
		UP = false;
		DOWN = false;
		LEFT = false;
		RIGHT = false;
		if(jumper.on_bar){
			if(clock()-ai_start>1000){
				jumper.on_bar = false;
			}
		}else if(!jumper.jumped && !jumper.flies){//zjezdza po rozbiegu
			switch(jumper.computer_level){
				case 1:
					if(jumper.position.x > hill.canjump.x) jumper.jump();
					break;
				case 2:
				case 3:
					if(jumper.position.x > -156.f) jumper.jump();
					break;
			}
		}else if(jumper.jumped && !jumper.landed){//wybil sie z progu i leci
			switch(jumper.computer_level){
				case 1:
					if(jumper.position.x > -140.f) ai_landing();
					break;
				case 2:
					if(jumper.position.x > -130.f) ai_landing();
					break;
				case 3:
					ai_proper_position();
					if(jumper.position.x > -85.f) ai_landing();
					
					break;
			}
		}
	}
}

const int TIMER_MS = 1;	//1000 klatek na sekunde, ale display tylko 40 klatek na sekunde
void update(int value) {

	static clock_t t1 = clock();
	
	float diff = (((float)clock() - (float)t1) / 1000000.0F ) * 1000;   
	if(diff > 0.025){
		glutPostRedisplay();
		t1 = clock();
	}
	ai();
	keyboard_check();
	jumper.move();
	
	
	camera.center.set(jumper.position.x, jumper.position.y, jumper.position.z);
	switch(camera_type){
		case 1:
			camera.eye.set(jumper.position.x+10.0f, jumper.position.y-10.0f, jumper.position.z+5.0f);
			break;
		case 2:
			camera.eye.set(jumper.position.x, jumper.position.y-10.0f, jumper.position.z+5.0f);
			break;
		case 3:
			camera.eye.set(jumper.position.x-3.0f, jumper.position.y, jumper.position.z+3.5f);
			camera.center.set(jumper.position.x, jumper.position.y, jumper.position.z+1.5f);
			break;
		case 4:
			camera.eye.set(jumper.position.x, jumper.position.y-20.0f, jumper.position.z+6.0f);
			camera.center.set(jumper.position.x, jumper.position.y, jumper.position.z+3.0f);
			break;
		default:
			camera.eye.set(jumper.position.x+10.0f, jumper.position.y-10.0f, jumper.position.z+5.0f);
	}
	
	glutTimerFunc(TIMER_MS, update, 0);
}

void menu(int num){
	if(num==5){
		exit(0);
	}else{
		if(num==0){
			jumper.computer = false;
			jumper.computer_level = 0;
		}else{
			jumper.computer = true;
			jumper.computer_level = num;
			ai_start = clock();
		}
		jumper.reset();
		wind.reset();
	}
} 

void createMenu(void){
	static int menu_id;
	static int submenu_id;
	submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Czlowiek", 0);
    glutAddMenuEntry("Komputer - latwy", 1);
    glutAddMenuEntry("Komputer - sredni", 2);
    glutAddMenuEntry("Komputer - trudny", 3);
	menu_id = glutCreateMenu(menu);
    glutAddSubMenu("Nowy Skoczek", submenu_id);
    glutAddMenuEntry("Wyjscie", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) 
{
	win.width = 800;
	win.height = 600;
	win.title = "The Best Ski Jumper 3D";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 2500.0f;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA );
	glutInitWindowSize(win.width,win.height);
	glutCreateWindow(win.title);
	glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
	glutKeyboardUpFunc(handleUpKeypress);
	glutSpecialFunc(handleSpecialKeypress);
	glutSpecialUpFunc(handleSpecialUpKeypress);
	glutTimerFunc(TIMER_MS, update, 0);
	
	initialize();
	createMenu();
	glutMainLoop();

	return 0;
}