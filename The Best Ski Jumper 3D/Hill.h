#pragma once
#include <GL/glut.h>
#include "cObj.h"
#include "Vector3f.h"
#include <vector>



class Hill
{
public:
	cObj* model;
	Vector3f bar;					//belka startowa
	std::vector<Vector3f> points;	//punkty, w których zmienia siê nachylenie stoku
	Vector3f takeoff;				//próg skoczni
	Vector3f canjump;				//miejsce od ktorego mozna sie wybijac

	unsigned int bula_start;		//index elementu w kontenerze points, oznaczajacy wierzcholek od ktorego zaczyna sie wzluz stoku liczyc odleglosc skoku

	Vector3f outrun;				//dojazd

	Hill(void);
	~Hill(void);

	void init(void);
	void render(void);
};
