#pragma once
#include "Vector3f.h"
#include "cObj.h"
#include <ctime>
#include <cstdlib>
#include <cmath>

class Wind
{
public:
	Vector3f v;				//velocity of the wind
	Vector3f v_change;		//velocity can change in time

	unsigned char wind_change_time;		//used to calculate windows every x seconds


	Wind(void);
	~Wind(void);
	void reset(void);
};
