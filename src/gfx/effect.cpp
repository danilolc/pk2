#include "gfx/effect.hpp"

#include "gfx/particles.hpp"
#include "sprite.hpp"

void Effect_Feathers(DWORD x, DWORD y) {
	for (int i=0;i<9;i++)//6
		Particles_New(PARTICLE_FEATHER,x+rand()%17-rand()%17,y+rand()%20-rand()%10,
							(rand()%16-rand()%16)/10.0,(45+rand()%45)/100.0,300+rand()%40,0,0);
}

void Effect_Splash(DWORD x, DWORD y, BYTE color) {
	/*
	for (int i=0;i<12;i++)
		Particles_New(	PARTICLE_LIGHT,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5,(rand()%7-rand()%7)/3,
							rand()%50+60,0.025,color);*/
	for (int i=0;i<7;i++)
		Particles_New(	PARTICLE_SPARK,x+rand()%17-13,y+rand()%17-13,
							(rand()%5-rand()%5)/4.0,(rand()%4-rand()%7)/3.0,
							rand()%50+40,0.025,color);//0.015

	for (int i=0;i<20;i++)
		Particles_New(	PARTICLE_POINT,x+rand()%17-13,y+rand()%17-13,
							(rand()%5-rand()%5)/4.0,(rand()%2-rand()%7)/3.0,
							rand()%50+40,0.025,color);//0.015
}

void Effect_Explosion(DWORD x, DWORD y, BYTE color) {
	int i;

	for (i=0;i<3;i++)
		Particles_New(	PARTICLE_SMOKE,x+rand()%17-32,y+rand()%17,
							0,double((rand()%4)+3) / -10.0,450,0.0,color);

	for (i=0;i<9;i++)//12
		Particles_New(	PARTICLE_LIGHT,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5.0,(rand()%7-rand()%7)/3.0,
							rand()%40+60,0.025,color);

	for (i=0;i<8;i++)//8//10
		Particles_New(	PARTICLE_SPARK,x+rand()%17-13,y+rand()%17-13,
							(rand()%3-rand()%3),//(rand()%7-rand()%7)/5,
							(rand()%7-rand()%7)/3,
							rand()%20+60,0.015,color);//50+60

	for (i=0;i<20;i++)//12
		Particles_New(	PARTICLE_POINT,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5.0,(rand()%7-rand()%7)/3.0,
							rand()%40+60,0.025,color);
}

void Effect_Smoke(DWORD x, DWORD y, BYTE color) {
	for (int i=0;i<3;i++)
		Particles_New(	PARTICLE_SMOKE,x+rand()%17-32,y+rand()%17,
							0,double((rand()%3)+3) / -10.0/*-0.3*/,450,0.0,color);
	for (int i=0;i<6;i++)
		Particles_New(	PARTICLE_DUST_CLOUDS,x+rand()%30-rand()%30-10,y+rand()%30-rand()%30+10,
							0,-0.3,rand()%50+60,0,color);
}

void Effect_SmokeClouds(DWORD x, DWORD y) {
	for (int i=0;i<5;i++)
		Particles_New(	PARTICLE_SMOKE,x+rand()%17-32,y+rand()%17,
							0,double((rand()%3)+3) / -10.0/*-0.3*/,450,0.0,0);
}

void Effect_Stars(DWORD x, DWORD y, BYTE color) {
	for (int i=0;i<5;i++)
		Particles_New(PARTICLE_STAR,x-5, y-5, (rand()%3-rand()%3)/1.5, rand()%3*-1,100,(rand()%5+5)/100.0/* 0.05*/,color);//300

	for (int i=0;i<3;i++)//12
		Particles_New(	PARTICLE_POINT,x-5, y-5, (rand()%3-rand()%3)/1.5, rand()%3*-1,100,(rand()%5+5)/100.0,color);
}

void Effect_Destruction(BYTE tehoste, DWORD x, DWORD y) {
	switch (tehoste){
		case TUHOUTUMINEN_HOYHENET			: Effect_Feathers(x, y); break;
		case TUHOUTUMINEN_TAHDET_HARMAA		: Effect_Stars(x,y,0); break;
		case TUHOUTUMINEN_TAHDET_SININEN	: Effect_Stars(x,y,32); break;
		case TUHOUTUMINEN_TAHDET_PUNAINEN	: Effect_Stars(x,y,64); break;
		case TUHOUTUMINEN_TAHDET_VIHREA		: Effect_Stars(x,y,96); break;
		case TUHOUTUMINEN_TAHDET_ORANSSI	: Effect_Stars(x,y,128); break;
		case TUHOUTUMINEN_TAHDET_VIOLETTI	: Effect_Stars(x,y,160); break;
		case TUHOUTUMINEN_TAHDET_TURKOOSI	: Effect_Stars(x,y,192); break;
		case TUHOUTUMINEN_RAJAHDYS_HARMAA	: Effect_Explosion(x,y,0); break;
		case TUHOUTUMINEN_RAJAHDYS_SININEN	: Effect_Explosion(x,y,32); break;
		case TUHOUTUMINEN_RAJAHDYS_PUNAINEN	: Effect_Explosion(x,y,64); break;
		case TUHOUTUMINEN_RAJAHDYS_VIHREA	: Effect_Explosion(x,y,96); break;
		case TUHOUTUMINEN_RAJAHDYS_ORANSSI	: Effect_Explosion(x,y,128); break;
		case TUHOUTUMINEN_RAJAHDYS_VIOLETTI	: Effect_Explosion(x,y,160); break;
		case TUHOUTUMINEN_RAJAHDYS_TURKOOSI	: Effect_Explosion(x,y,192); break;
		case TUHOUTUMINEN_SAVU_HARMAA		: Effect_Smoke(x,y,0); break;
		case TUHOUTUMINEN_SAVU_SININEN		: Effect_Smoke(x,y,32); break;
		case TUHOUTUMINEN_SAVU_PUNAINEN		: Effect_Smoke(x,y,64); break;
		case TUHOUTUMINEN_SAVU_VIHREA		: Effect_Smoke(x,y,96); break;
		case TUHOUTUMINEN_SAVU_ORANSSI		: Effect_Smoke(x,y,128); break;
		case TUHOUTUMINEN_SAVU_VIOLETTI		: Effect_Smoke(x,y,160); break;
		case TUHOUTUMINEN_SAVU_TURKOOSI		: Effect_Smoke(x,y,192); break;
		case TUHOUTUMINEN_SAVUPILVET		: Effect_SmokeClouds(x,y); break;
		default	: break;
	}
}
