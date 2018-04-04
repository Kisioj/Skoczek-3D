#pragma once
#include "Vector3f.h"
#include "Hill.h"
#include "Wind.h"
#include "cObj.h"
#include <ctime>
#include <cmath>
#include <vector>



//zakres kataw przy ktorych nasz skoczek sie nie wywali przy ladowaniu (w stopniach)
const float MIN_JUMPER_ANGLE = 60.0f;
const float MAX_JUMPER_ANGLE = 110.0f;

const float MIN_SKIS_ANGLE = -10.0f;
const float MAX_SKIS_ANGLE = 50.0f;

class Skis //po upadku narty odczepiaja sie i maja swoja wlasna pozycje
{
public:
	Vector3f position;
	float FRICTION_FACTOR;
	float v;
	float angle;
	unsigned int hill_points_index;
};


class Jumper
{
public:
	Vector3f position;

	Skis skis;

	cObj* model_jumper;
	cObj* model_jumper_inrun;	//rozbieg

	cObj* model_skis;
	cObj* model_skis_fly;

	std::vector<Vector3f> skis_shadow_points;

	bool on_bar;
	bool flies;
	bool landed;
	bool jumped;
	unsigned int fell_down;

	float jump_speed;
	float jump_length;

	float v;
	float angle;

	float jumper_angle;
	float skis_angle;

	float weight;
	float height;

	float front_jumper_area;
	float side_jumper_area;

	float front_skis_area;
	float side_skis_area;

	float shadow_z; //x and y will be the same as jumper's

	unsigned int hill_points_index;

	float FRICTION_FACTOR;

	bool stop;

	bool computer;
	unsigned int computer_level;

	Jumper(void);
	void init(void);
	void render(void);
	void render_shadow(float);
	float get_z_on_slope(float x); //z coord of shadow on the slope
	void move(void);
	void jump(void);
	void collision(void);
	void reset(void);
};
