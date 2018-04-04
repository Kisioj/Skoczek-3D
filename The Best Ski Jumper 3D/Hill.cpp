#include "Hill.h"

using namespace std;

Hill::Hill(void)
{
	bar.set(-228.56f, 10.86f, 77.09f);
	takeoff.set(-154.50f, 10.86f, 51.30f);
	canjump.set(-160.46f, 10.86f, 51.93f);
	outrun.set(-25.72f, 10.86f, 4.71f);
	
	bula_start = 22;

	//rozbieg
	points.push_back(Vector3f(-231.60f, 10.86f, 77.54f));
	points.push_back(Vector3f(-229.57f, 10.86f, 77.54f));
	points.push_back(Vector3f(-191.20f, 10.86f, 60.45f));
	points.push_back(Vector3f(-189.36f, 10.86f, 59.67f));
	points.push_back(Vector3f(-187.51f, 10.86f, 58.92f));
	points.push_back(Vector3f(-185.64f, 10.86f, 58.21f));
	points.push_back(Vector3f(-183.76f, 10.86f, 57.52f));
	points.push_back(Vector3f(-181.87f, 10.86f, 56.87f));
	points.push_back(Vector3f(-179.97f, 10.86f, 56.25f));
	points.push_back(Vector3f(-178.05f, 10.86f, 55.67f));
	points.push_back(Vector3f(-176.13f, 10.86f, 55.12f));
	points.push_back(Vector3f(-174.20f, 10.86f, 54.60f));
	points.push_back(Vector3f(-172.26f, 10.86f, 54.12f));
	points.push_back(Vector3f(-170.31f, 10.86f, 53.67f));
	points.push_back(Vector3f(-168.35f, 10.86f, 53.25f));
	points.push_back(Vector3f(-166.39f, 10.86f, 52.87f));
	points.push_back(Vector3f(-164.42f, 10.86f, 52.52f));
	points.push_back(Vector3f(-162.44f, 10.86f, 52.21f));
	points.push_back(Vector3f(-160.46f, 10.86f, 51.93f));
	points.push_back(Vector3f(-158.48f, 10.86f, 51.69f));
	points.push_back(Vector3f(-156.49f, 10.86f, 51.48f));
	points.push_back(Vector3f(-154.50f, 10.86f, 51.30f));

	//niebieski - bula
	points.push_back(Vector3f(-154.50f, 10.86f, 48.80f));
	points.push_back(Vector3f(-151.50f, 10.86f, 48.80f));
	points.push_back(Vector3f(-149.50f, 10.86f, 48.77f));
	points.push_back(Vector3f(-147.50f, 10.86f, 48.70f));
	points.push_back(Vector3f(-145.50f, 10.86f, 48.59f));
	points.push_back(Vector3f(-143.51f, 10.86f, 48.45f));
	points.push_back(Vector3f(-141.52f, 10.86f, 48.24f));
	points.push_back(Vector3f(-139.53f, 10.86f, 48.00f));
	points.push_back(Vector3f(-137.55f, 10.86f, 47.72f));
	points.push_back(Vector3f(-135.58f, 10.86f, 47.38f));
	points.push_back(Vector3f(-133.62f, 10.86f, 46.99f));
	points.push_back(Vector3f(-131.66f, 10.86f, 46.58f));
	points.push_back(Vector3f(-129.72f, 10.86f, 46.09f));
	points.push_back(Vector3f(-127.79f, 10.86f, 45.58f));
	points.push_back(Vector3f(-125.87f, 10.86f, 45.03f));
	points.push_back(Vector3f(-123.97f, 10.86f, 44.41f));
	points.push_back(Vector3f(-122.07f, 10.86f, 43.76f));
	points.push_back(Vector3f(-116.44f, 10.86f, 41.70f));
	points.push_back(Vector3f(-108.97f, 10.86f, 38.84f));
	points.push_back(Vector3f(-99.70f, 10.86f, 35.09f));
	points.push_back(Vector3f(-90.49f, 10.86f, 31.18f));
	points.push_back(Vector3f(-72.22f, 10.86f, 23.05f));

	//czerwona - dobry skok
	points.push_back(Vector3f(-53.81f, 10.86f, 15.23f));
	points.push_back(Vector3f(-44.54f, 10.86f, 11.49f));

	//zielony - bardzo dobry skok i dojazd dojazd
	points.push_back(Vector3f(-37.07f, 10.86f, 8.62f));
	points.push_back(Vector3f(-33.31f, 10.86f, 7.25f));
	points.push_back(Vector3f(-29.53f, 10.86f, 5.95f));
	points.push_back(Vector3f(-25.72f, 10.86f, 4.71f));
	points.push_back(Vector3f(-23.80f, 10.86f, 4.16f));
	points.push_back(Vector3f(-21.87f, 10.86f, 3.65f));
	points.push_back(Vector3f(-19.93f, 10.86f, 3.16f));
	points.push_back(Vector3f(-17.98f, 10.86f, 2.71f));
	points.push_back(Vector3f(-16.02f, 10.86f, 2.30f));
	points.push_back(Vector3f(-14.06f, 10.86f, 1.91f));
	points.push_back(Vector3f(-12.09f, 10.86f, 1.57f));
	points.push_back(Vector3f(-10.12f, 10.86f, 1.25f));
	points.push_back(Vector3f(-8.14f, 10.86f, 0.98f));
	points.push_back(Vector3f(-6.15f, 10.86f, 0.73f));
	points.push_back(Vector3f(-4.16f, 10.86f, 0.52f));
	points.push_back(Vector3f(-2.17f, 10.86f, 0.35f));
	points.push_back(Vector3f(-0.17f, 10.86f, 0.21f));
	points.push_back(Vector3f(1.82f, 10.86f, 0.10f));
	points.push_back(Vector3f(3.82f, 10.86f, 0.03f));
	points.push_back(Vector3f(5.82f, 10.86f, 0.00f));


	//zielony - koñcówka; pod górkê
	points.push_back(Vector3f(7.82f, 10.86f, 0.03f));
	points.push_back(Vector3f(9.82f, 10.86f, 0.10f));
	points.push_back(Vector3f(11.82f, 10.86f, 0.21f));
	points.push_back(Vector3f(13.81f, 10.86f, 0.35f));
	points.push_back(Vector3f(15.81f, 10.86f, 0.52f));
	points.push_back(Vector3f(17.79f, 10.86f, 0.73f));
	points.push_back(Vector3f(19.78f, 10.86f, 0.98f));
	points.push_back(Vector3f(21.76f, 10.86f, 1.25f));
	points.push_back(Vector3f(23.74f, 10.86f, 1.57f));
	points.push_back(Vector3f(25.70f, 10.86f, 1.91f));
	points.push_back(Vector3f(27.67f, 10.86f, 2.30f));
	points.push_back(Vector3f(29.62f, 10.86f, 2.71f));
	points.push_back(Vector3f(31.57f, 10.86f, 3.16f));
	points.push_back(Vector3f(33.51f, 10.86f, 3.65f));
	points.push_back(Vector3f(35.45f, 10.86f, 4.16f));
	points.push_back(Vector3f(37.37f, 10.86f, 4.71f));
	points.push_back(Vector3f(58.41f, 10.86f, 11.15f));

	/*	
	for(unsigned int i=0;i<points.size()-1;i++){
		float d_X = points[i+1].x - points[i].x;
		float d_Y = points[i+1].z - points[i].z;
		float angleInDegress = atan2(d_Y,d_X) * 180 / PI;
		cout << angleInDegress << endl;
	}
	*/
	
	
}

Hill::~Hill(void)
{
}

void Hill::init(void)
{
	model = new cObj("media/mamut_blend/","untitled.obj");
}




void Hill::render(void)
{
	model->render();
}