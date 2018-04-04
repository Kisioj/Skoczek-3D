#pragma once

struct Vector4f
{
	float x;
	float y;
	float z;
	float w;
	void set(float x, float y, float z, float w){
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	};
};
