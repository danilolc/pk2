//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "gfx/effect.hpp"

#include "gfx/particles.hpp"
#include "game/sprites.hpp"
#include "system.hpp"

#include <cstdlib>

void Effect_Circle_Star(double x, double y, u8 color) {

	if (color == COLOR_NORMAL)
		color = COLOR_GRAY;
		
	float weight = 0.01;
	int time = 50;

	for (int angle = 0; angle < 180; angle += 20) {
		Particles_New(PARTICLE_STAR, x, y, sin_table(angle)/20, cos_table(angle)/20, time, weight, color);
		Particles_New(PARTICLE_SPARK, x, y, sin_table(angle+10)/21, cos_table(angle+10)/21, time, weight, color);
	}

}

void Effect_Super(double x, double y, int w, int h) {

	const int colors[] = {COLOR_BLUE,COLOR_ORANGE,COLOR_GREEN,COLOR_TURQUOISE,COLOR_RED};

	float weight = 0.01;
	int time = 25;

	//int stars = (w * h) / 500;
	int stars = 3;

	for (int i = 0; i < stars; i++) {
		int angle = rand() % 180;
		int px = x + rand()%w - w / 2;
		int py = y + rand()%h - h / 2;

		int d = 30 + rand()%40;
		int color = colors[rand()%5];

		Particles_New(PARTICLE_STAR, px, py, sin_table(angle)/d, cos_table(angle)/d, time, weight, color);
	}
}

void Effect_Points(double x, double y, int w, int h, u8 color) {

	if (color == COLOR_NORMAL)
		color = COLOR_GRAY;

	float weight = 0.01;
	int time = 25;

	int points = 1 + (w * h) / 500;

	for (int i = 0; i < points; i++) {
		int angle = rand() % 180;
		int px = x + rand()%w - w / 2;
		int py = y + rand()%h - h / 2;

		int d = 30 + rand()%40;

		int rnd = rand()%500;

		int type = PARTICLE_POINT;
		if (rnd > 200) type = PARTICLE_LIGHT;
		else if (rnd > 50) type = PARTICLE_SPARK;

		Particles_New(type, px, py, sin_table(angle)/d, cos_table(angle)/d, time, weight, color);
		
	}
}

void Effect_Feathers(double x, double y) {
	for (int i=0;i<9;i++)//6
		Particles_New(PARTICLE_FEATHER,x+rand()%17-rand()%17,y+rand()%20-rand()%10,
							double(rand()%16-rand()%16)/10,double(45+rand()%45)/100,300+rand()%40,0,0);
}

void Effect_Splash(double x, double y, u8 color) {
	/*
	for (int i=0;i<12;i++)
		Particles_New(	PARTICLE_LIGHT,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5,(rand()%7-rand()%7)/3,
							rand()%50+60,0.025,color);*/
	for (int i=0;i<7;i++)
		Particles_New(	PARTICLE_SPARK,x+rand()%17-13,y+rand()%17-13,
							double(rand()%5-rand()%5)/4,double(rand()%4-rand()%7)/3,
							rand()%50+40,0.025,color);//0.015

	for (int i=0;i<20;i++)
		Particles_New(	PARTICLE_POINT,x+rand()%17-13,y+rand()%17-13,
							(rand()%5-rand()%5)/4.0,(rand()%2-rand()%7)/3.0,
							rand()%50+40,0.025,color);//0.015
}

void Effect_Explosion(double x, double y, u8 color) {
	int i;

	for (i=0;i<3;i++)
		Particles_New(	PARTICLE_SMOKE,x+rand()%17-32,y+rand()%17,
							0,double((rand()%4)+3) / -10.0,450,0.0,color);

	for (i=0;i<9;i++)//12
		Particles_New(	PARTICLE_LIGHT,x+rand()%17-13,y+rand()%17-13,
							double(rand()%7-rand()%7)/5.0,double(rand()%7-rand()%7)/3.0,
							rand()%40+60,0.025,color);

	for (i=0;i<8;i++)//8//10
		Particles_New(	PARTICLE_SPARK,x+rand()%17-13,y+rand()%17-13,
							(rand()%3-rand()%3),//(rand()%7-rand()%7)/5,
							double(rand()%7-rand()%7)/3,
							rand()%20+60,0.015,color);//50+60

	for (i=0;i<20;i++)//12
		Particles_New(	PARTICLE_POINT,x+rand()%17-13,y+rand()%17-13,
							double(rand()%7-rand()%7)/5,double(rand()%7-rand()%7)/3,
							rand()%40+60,0.025,color);
}

void Effect_Smoke(double x, double y, u8 color) {
	for (int i=0;i<3;i++)
		Particles_New(	PARTICLE_SMOKE,x+rand()%17-32,y+rand()%17,
							0,double((rand()%3)+3) / -10.0/*-0.3*/,450,0.0,color);
	for (int i=0;i<6;i++)
		Particles_New(	PARTICLE_DUST_CLOUDS,x+rand()%30-rand()%30-10,y+rand()%30-rand()%30+10,
							0,-0.3,rand()%50+60,0,color);
}

void Effect_SmokeClouds(double x, double y) {
	for (int i=0;i<5;i++)
		Particles_New(	PARTICLE_SMOKE,x+rand()%17-32,y+rand()%17,
							0,double((rand()%3)+3) / -10.0/*-0.3*/,450,0.0,0);
}

void Effect_Stars(double x, double y, u8 color) {
	for (int i = 0; i < 5; i++)
		Particles_New(PARTICLE_STAR,x-5, y-5, double(rand()%30-rand()%30)/15, rand()%3*-1,100,(rand()%5+5)/100.0/* 0.05*/,color);//300

	for (int i=0;i<3;i++)//12
		Particles_New(	PARTICLE_POINT,x-5, y-5, double(rand()%30-rand()%30)/15, rand()%3*-1,100,(rand()%5+5)/100.0,color);
}

void Effect_Destruction(u8 tehoste, double x, double y) {
	switch (tehoste){
		case FX_DESTRUCT_HOYHENET          : Effect_Feathers(x, y); break;
		case FX_DESTRUCT_TAHDET_HARMAA     : Effect_Stars(x,y,0); break;
		case FX_DESTRUCT_TAHDET_SININEN	   : Effect_Stars(x,y,32); break;
		case FX_DESTRUCT_TAHDET_PUNAINEN   : Effect_Stars(x,y,64); break;
		case FX_DESTRUCT_TAHDET_VIHREA     : Effect_Stars(x,y,96); break;
		case FX_DESTRUCT_TAHDET_ORANSSI    : Effect_Stars(x,y,128); break;
		case FX_DESTRUCT_TAHDET_VIOLETTI   : Effect_Stars(x,y,160); break;
		case FX_DESTRUCT_TAHDET_TURKOOSI   : Effect_Stars(x,y,192); break;
		case FX_DESTRUCT_RAJAHDYS_HARMAA   : Effect_Explosion(x,y,0); break;
		case FX_DESTRUCT_RAJAHDYS_SININEN  : Effect_Explosion(x,y,32); break;
		case FX_DESTRUCT_RAJAHDYS_PUNAINEN : Effect_Explosion(x,y,64); break;
		case FX_DESTRUCT_RAJAHDYS_VIHREA   : Effect_Explosion(x,y,96); break;
		case FX_DESTRUCT_RAJAHDYS_ORANSSI  : Effect_Explosion(x,y,128); break;
		case FX_DESTRUCT_RAJAHDYS_VIOLETTI : Effect_Explosion(x,y,160); break;
		case FX_DESTRUCT_RAJAHDYS_TURKOOSI : Effect_Explosion(x,y,192); break;
		case FX_DESTRUCT_SAVU_HARMAA       : Effect_Smoke(x,y,0); break;
		case FX_DESTRUCT_SAVU_SININEN      : Effect_Smoke(x,y,32); break;
		case FX_DESTRUCT_SAVU_PUNAINEN     : Effect_Smoke(x,y,64); break;
		case FX_DESTRUCT_SAVU_VIHREA       : Effect_Smoke(x,y,96); break;
		case FX_DESTRUCT_SAVU_ORANSSI      : Effect_Smoke(x,y,128); break;
		case FX_DESTRUCT_SAVU_VIOLETTI     : Effect_Smoke(x,y,160); break;
		case FX_DESTRUCT_SAVU_TURKOOSI     : Effect_Smoke(x,y,192); break;
		case FX_DESTRUCT_SAVUPILVET        : Effect_SmokeClouds(x,y); break;
		default	: break;
	}
}
