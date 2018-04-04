#include "Jumper.h"

extern Hill hill;
extern Wind wind;

using namespace std;

Jumper::Jumper(void)
{
	reset();
	computer = false;
	computer_level = 0;
}


void Jumper::reset(void)
{
	position.set(-228.56f, 10.86f, 77.09f);
	shadow_z = 77.09f;

	on_bar = true;
	flies = false;
	landed = false;
	jumped = false;
	fell_down = 0;

	jump_speed = 0.0f;
	jump_length = 0.0f;

	v = 2.0f;
	angle = 24.0082f;

	jumper_angle = 24.0082f - 90.0f;
	skis_angle = 24.0082f;

	weight = 61.0f;
	height = 1.7f;

	front_jumper_area = 0.68f;
	side_jumper_area = 0.20f;//?

	front_skis_area = 0.48f;
	side_skis_area = 0.02f;//?

	hill_points_index = 1;//pomiedzy 1 i 2
	FRICTION_FACTOR = 0.01f;

	skis_shadow_points.push_back(Vector3f(1.03f, 0.77f, 0.0f));
	skis_shadow_points.push_back(Vector3f(1.07f, 0.65f, 0.0f));
	skis_shadow_points.push_back(Vector3f(-1.02f, -0.03f, 0.0f));
	skis_shadow_points.push_back(Vector3f(-0.99f, -0.13f, 0.0f));

	skis_shadow_points.push_back(Vector3f(1.08f, -0.61f, 0.0f));
	skis_shadow_points.push_back(Vector3f(1.04f, -0.73f, 0.0f));
	skis_shadow_points.push_back(Vector3f(-1.04f, -0.02f, 0.0f));
	skis_shadow_points.push_back(Vector3f(-1.00f, -0.09f, 0.0f));

	stop = false;

	
}

void Jumper::init(void)
{
	model_skis = new cObj("media/nartyblenda/jazda/","narty.obj");
	model_skis_fly = new cObj("media/nartyblenda/lot/","narty.obj");

	model_jumper = new cObj("media/skoczekblenda/normalny/","skoczek.obj");
	model_jumper_inrun = new cObj("media/skoczekblenda/rozbieg/","skoczek.obj");
}


float Jumper::get_z_on_slope(float x)
{
	int points_index = 0;
	for(unsigned int i=hill_points_index-1;i<hill.points.size()-1;i++){
		if(x > hill.points[i].x && x < hill.points[i+1].x){
			points_index = i;
			break;
		}
	}
	
	float d_X = hill.points[points_index+1].x - hill.points[points_index].x;
	float d_Z = hill.points[points_index+1].z - hill.points[points_index].z; //pionowa
	float tg = -(d_Z/d_X);
	float slope_x = fabs(x - hill.points[points_index+1].x);
	float slope_z = hill.points[points_index+1].z + slope_x * tg;	
	
	return slope_z;
}

void Jumper::render_shadow(float angle)	//kat w radianach, funkcja rysuje cien nart
{
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0f,0.0f,0.0f, 0.2f);

	glBegin(GL_QUADS);
	for(unsigned int i=0; i<skis_shadow_points.size(); i++)
	{
		float x = position.x+skis_shadow_points[i].x * cos(angle);
		float y = position.y+skis_shadow_points[i].y;
		float z = get_z_on_slope(x)+0.1f;		
		glVertex3f(x,y,z);
	}
	glEnd();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void Jumper::render(void)
{
	glTranslatef(position.x, position.y, position.z);
	
	if(!fell_down){
		glRotatef(skis_angle, 0.0f, 1.0f, 0.0f);
		if(jumped && !landed){
			model_skis_fly->render();
		}else{
			model_skis->render();
		}
		glRotatef(-skis_angle, 0.0f, 1.0f, 0.0f);

		glRotatef(jumper_angle + 90.0f, 0.0f, 1.0f, 0.0f);
		if(jumped){
			model_jumper->render();
		}else{
			model_jumper_inrun->render();
		}
		glRotatef(-jumper_angle - 90.0f, 0.0f, 1.0f, 0.0f);
	}else{
		if(fell_down==2){
			glRotatef(-angle + 90.f, 0.0f, 1.0f, 0.0f);
			model_jumper->render();
			glRotatef(angle - 90.f, 0.0f, 1.0f, 0.0f);
		}else if(fell_down==1){
			glTranslatef(0.f,0.f,0.3f);
			glRotatef(-angle - 90.f, 0.0f, 1.0f, 0.0f);
			model_jumper->render();
			glRotatef(angle + 90.f, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.f,0.f,-0.3f);
		}
		
	}
	glTranslatef(-position.x, -position.y, -position.z);

	if(jumped && !landed){
		render_shadow(skis_angle * RADIAN);
	}

	if(fell_down){ //odjezdzajace narty po upadku
		glTranslatef(skis.position.x, skis.position.y, skis.position.z);
		glRotatef(-angle, 0.0f, 1.0f, 0.0f);
		model_skis->render();
		glRotatef(angle, 0.0f, 1.0f, 0.0f);
		glTranslatef(-skis.position.x, -skis.position.y, -skis.position.z);
	}
}

void Jumper::move(void)
{
	if(on_bar){
		hill_points_index = 1;
		v = 2.0f;
	}

	if(!on_bar){
		static unsigned int time = clock();
		float t = (clock() - time)/(CLOCKS_PER_SEC/1000.0f)/1000.0f;
		time = clock();
		if(t > 0.1){t = 0.00;}	//zatrzymanie gry, gdy uzytkownik przeciaga okienko
		/*
		//jezeli ktos ma slaby refleks to mozna zwolnic gre np. 2 albo 4 krotnie
		if(jumped)
			t/=4;
		*/

		float a = G * sin(-angle * RADIAN);
		float s = v * t + a * pow(t,2.0f) / 2.0f;

		if(position.x >= hill.outrun.x && FRICTION_FACTOR < 2.0f){
			FRICTION_FACTOR = 2.0f;
		}

		float tmp_v = v;
		float tmp_a = a;
		float tmp_s = s;
		float tmp_angle = angle;

		if(!flies)
		{
			float tarcie = fabs(FRICTION_FACTOR * G * cos(angle) * t);
			if(tarcie > v + a*t) tarcie = v + a*t;

			v += a*t - tarcie;
			if(v < 0.1f) v = 0.0f;

			for(unsigned int i=hill_points_index;i<hill.points.size()-1;i++){
				if(position.x > hill.points[i].x && position.x < hill.points[i+1].x){
					hill_points_index = i;
					break;
				}
			}
			float d_X = hill.points[hill_points_index+1].x - hill.points[hill_points_index].x;
			float d_Z = hill.points[hill_points_index+1].z - hill.points[hill_points_index].z; //pionowa

			angle = (atan2(d_Z,d_X) * 180.0f / PI);
			skis_angle = -angle;
			jumper_angle = skis_angle - 90.f;


			float tg = -(d_Z/d_X);
			float slope_x = fabs(position.x - hill.points[hill_points_index+1].x);
			float slope_z = hill.points[hill_points_index+1].z + slope_x * tg;	
			
			shadow_z = slope_z + sin(angle * RADIAN) * s;

			if(position.z < slope_z)
				position.z = slope_z+0.01f;

			if(!landed && !jumped && position.x > hill.takeoff.x){
				flies = true;
				jump_speed = v*3.6f;
			}
		}else{
			float alpha = -angle * RADIAN;
			float skis_alpha = skis_angle * RADIAN;
			float jumper_alpha = jumper_angle * RADIAN;

			//rozk³adamy wektor prêdkoœci v na wektory sk³adowe vx i vy
			float vx = v * cos(alpha);
			float vy = v * sin(alpha);
			///////////////////////////////
			////////SI£A CIÊ¯KOŒCI/////////
			///////////////////////////////
			float weight_F = weight * G;
			float weight_ay = weight_F / weight;
			vy += weight_ay*t;
			//----------------KONIEC SI£Y CIÊ¯KOŒCI-----------------//
			
			
			///////////////////////////////
			/////////SI£A WIATRU///////////
			///////////////////////////////			
			float ro = 1.2f; //gêstoœæ powietrza
			
			//wind_v - skladowa wektora predkosci w kierunku przeciwnym do kierunku lotu skoczka
			float wind_v = sqrt( pow(wind.v.x+wind.v_change.x, 2.0f) + pow(wind.v.z+wind.v_change.z, 2.0f) );
			float wind_angle = asin( (wind.v.z+wind.v_change.z)/wind_v);
			if(wind.v.z < 0 && wind.v.x < 0){wind_angle = - (180*PI/180) - wind_angle;}
			else if(wind.v.z >= 0 && wind.v.x < 0){wind_angle = (180*PI/180) - wind_angle;}
			
			wind_v *= abs(cos(wind_angle-alpha));
			if(wind.v.z<0){wind_v=-wind_v;}
			//----------------KONIEC WIATRU-----------------//

			///////////////////////////////
			//////////SI£A NOŒNA///////////
			///////////////////////////////	
			float Cx = 12.0f; //wspolczynnik sily nosnej
			float lift_F = 0.f;
			float kat = 0.f;

			//w przypadku, gdy skoczek nie wyskoczy z progu, nie osiaga sily nosnej
			if(!jumped) {Cx = 0;} 

			if(jumper_alpha>alpha || skis_alpha>alpha){	//jezeli zawodnik jest za bardzo pochylony wzgledem toru ruchu to nie wytwarza sily nosnej
				Cx = 0;
			}else{
				kat = abs(alpha - jumper_alpha);
				if(kat < PI/6) Cx *= kat*3;
				else if(kat < PI/3) Cx *= (PI - kat*3);
				else Cx=0;
				lift_F = abs(sin(alpha - jumper_alpha)) * (v - wind_v) * ro * front_jumper_area * Cx;

				//Cx = 12.0f;
				kat = abs(alpha - skis_alpha);
				if(kat < PI/6) Cx *= kat*3;
				else if(kat < PI/3) Cx *= (PI - kat*3);
				else Cx=0;
				lift_F += abs(sin(alpha - skis_alpha)) * (v - wind_v) * ro * front_skis_area * Cx;
			}	

			float lift_ay = lift_F * abs(sin(jumper_alpha)) / weight;
			float lift_ax = lift_F * abs(cos(jumper_alpha)) / weight;
			
			vy -= lift_ay*2.f  * t;
			vx += lift_ax * t;
			//----------------KONIEC SI£Y NOŒNEJ-----------------//
			

			//znow skladamy wektor predkosci
			v = sqrt( pow(vx, 2.0f) + pow(vy, 2.0f) );

			alpha = asin(vy/v);

			
			///////////////////////////////
			//////OPÓR AERODYNAMICZNY//////
			///////////////////////////////	
			float S = abs(sin(alpha - skis_alpha)) * front_skis_area + abs(sin(alpha -jumper_alpha)) * front_jumper_area;
			
			//double Cd = 1.8;
			float Cd = (3.0f - 3.0f*( abs(90.0f - fmod((abs(alpha - skis_alpha)*180.0f/PI),180.0f))/90.0f )) * front_skis_area/(front_skis_area+front_jumper_area);
			Cd+= (3.0f - 3.0f*( abs(90.0f - fmod((abs(alpha - jumper_alpha)*180.0f/PI),180.0f))/90.0f )) * front_jumper_area/(front_skis_area+front_jumper_area);
			
			float drag_F = 0.5f * ro * pow(v,2.0f) * S * Cd;

			float drag_a = drag_F / weight;

			v -= drag_a*t;
			//----------------KONIEC OPORU AERODYNAMICZNEGO-----------------//
			

			angle = -alpha * 180.0f / PI;
		}
		if(stop){
			v = tmp_v;
			a = tmp_a;
			s = tmp_s;
			angle = tmp_angle;
		}else{
			if(landed && v<0.1f){

			}else{
				position.x += cos(angle * RADIAN) * s;
				position.z += sin(angle * RADIAN) * s;
			}
		}
		if(flies) collision();

		if(fell_down){
			if(!stop){
				if(skis.position.x < hill.points.back().x){
					float skis_a = G * sin(-skis.angle * RADIAN);
					float skis_s = skis.v * t + skis_a * pow(t, 2.0f) / 2.0f;
					float skis_tarcie = skis.FRICTION_FACTOR * G * cos(skis.angle) * t;
					if(skis_tarcie > skis.v + skis_a * t ) skis_tarcie = skis.v + skis_a * t;
					skis.v += skis_a*t - skis_tarcie;

					for(unsigned int i=skis.hill_points_index;i<hill.points.size()-1;i++){
						if(skis.position.x > hill.points[i].x && skis.position.x < hill.points[i+1].x){
							skis.hill_points_index = i;
							break;
						}
					}
					float d_X = hill.points[skis.hill_points_index+1].x - hill.points[skis.hill_points_index].x;
					float d_Z = hill.points[skis.hill_points_index+1].z - hill.points[skis.hill_points_index].z;

					float tg = -(d_Z/d_X);
					float slope_x = fabs(skis.position.x - hill.points[skis.hill_points_index+1].x);
					float slope_z = hill.points[skis.hill_points_index+1].z + slope_x * tg;	
					
					if(skis.position.z < slope_z) 
						skis.position.z = slope_z+0.01f;

					skis.angle = (atan2(d_Z,d_X) * 180.0f / PI);

					skis.position.x += cos(skis.angle * RADIAN) * skis_s;
					skis.position.z += sin(skis.angle * RADIAN) * skis_s;
				}
			}
		}
	}
}

void Jumper::collision(void)
{
	for(unsigned int i=hill_points_index;i<hill.points.size()-1;i++){
		if(position.x > hill.points[i].x && position.x < hill.points[i+1].x){
			hill_points_index = i;
			break;
		}
	}

	float d_X = hill.points[hill_points_index+1].x - hill.points[hill_points_index].x;
	float d_Z = hill.points[hill_points_index+1].z - hill.points[hill_points_index].z; //pionowa

	float tg = -(d_Z/d_X);
	float slope_x = fabs(position.x - hill.points[hill_points_index+1].x);
	float slope_z = hill.points[hill_points_index+1].z + slope_x * tg;	

	if(position.z < slope_z){
		flies = false;
		landed = true;
		float slope_angle = -tg * 180.0f / PI;

		float sum = 0.f;
		for(unsigned int i = hill.bula_start; i < hill.points.size(); i++){
			if(position.x > hill.points[i].x){
				if(position.x < hill.points[i+1].x){
					sum += sqrt(pow(position.x - hill.points[i].x,2.0f)+pow(slope_z - hill.points[i].z,2.0f));
					break;
				}else{
					sum += sqrt(pow(hill.points[i+1].x - hill.points[i].x,2.0f)+pow(hill.points[i+1].z - hill.points[i].z,2.0f));
				}
			}
		}
		jump_length = sum;
		cout << "Skok na odleglosc: " << jump_length << endl;
		if(abs(slope_angle - (-jumper_angle)) < MIN_JUMPER_ANGLE){
			fell_down = 2;
			cout << "Skok nieustany! Zawodnik przewrocil sie do przodu!" << endl;
		}else if(abs(slope_angle - (-jumper_angle)) > MAX_JUMPER_ANGLE){
			fell_down = 1;
			cout << "Skok nieustany! Zawodnik przewrocil sie do tylu!" << endl;
		}else if(abs(slope_angle - (-skis_angle)) < MIN_SKIS_ANGLE || abs(slope_angle - (-skis_angle)) > MAX_SKIS_ANGLE ){
			if(abs(slope_angle - (-jumper_angle)) < 90.0f) fell_down = 2;
			else fell_down = 1;
			cout << "Skok nieustany! Zawodnik sie przewrocil!" << endl;
		}

		if(!jumped){
			cout << "Skok nieudany! Zawodnik sie przewrocil!" <<endl;
			fell_down = 1;
		}

		if(fell_down){
			FRICTION_FACTOR = 0.8f;
			skis.FRICTION_FACTOR = 0.05f;
			skis.position.x = position.x;
			skis.position.z = position.z;
			skis.position.y = position.y;
			skis.v = v;
			skis.hill_points_index = hill_points_index;
			jump_length = 0.f;
		}

	}
}

void Jumper::jump(void)
{
	if(!flies && !jumped && !landed && position.x > hill.canjump.x)
	{
		flies = true;
		jumped = true;
			
		float vx = fabs(v * cos(angle * RADIAN));
		float vy = fabs(v * sin(angle * RADIAN));
		
		jump_speed = v*3.6f;
		cout << "Jump at " << position.x << endl;

		v = sqrt( pow(vx, 2.0f) + pow(vy, 2.0f) );
		 //podczas idelanego wyskoku z progru, skoczek podnosi kat toru ruchu o 5 stopni
		angle = (asin(vy/v) + 0.127f) * 180.0f / PI;

		jumper_angle = -asin(2.0f/9.0f) * 180.0f / PI;
		skis_angle = angle;
	}
}