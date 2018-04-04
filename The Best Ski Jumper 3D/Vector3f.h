#pragma once

struct Vector3f
{
	float x;
	float y;
	float z;
	Vector3f(void){};
	Vector3f(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	};
	void set(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	};
};
