#include "Wind.h"

using namespace std;
Wind::Wind(void)
{
	reset();
}

Wind::~Wind(void)
{
}

void Wind::reset(void)
{
	v.x = ((rand() % 401)-200)/100.0f;
	v.z = ((rand() % 401)-200)/100.0f;

	v_change.x = float( ( rand() % int(fabs(v.x*20.0f)+1.0f) ) - int(v.x*10.0f) ); 
	v_change.x /= 100.0f;
	if( (v.x >= 0 && v_change.x < 0) || (v.x <= 0 && v_change.x > 0) )
		v_change.x = -v_change.x;

	v_change.z = float( ( rand() % int(fabs(v.z*20.0f)+1) ) - int(v.z*10.0f) ); 
	v_change.z /= 100.0f;
	if( (v.z >= 0 && v_change.z < 0) || (v.z <= 0 && v_change.z > 0) )
		v_change.z = -v_change.z;
}
