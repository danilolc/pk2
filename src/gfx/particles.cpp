//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "gfx/particle.hpp"

#include "game/mapclass.hpp"
#include "settings.hpp"
#include "system.hpp"

#include <cstdlib>
#include <vector>

std::vector<Particle> Particles;
std::vector<Particle> BGParticles;

const int nof_bg_particles = 300;

void Particles_Update() {
	
	//for (Particle p : Particles) { This method doesn't work
	for (uint i = 0; i < Particles.size();) {

		Particles[i].update();

		if (Particles[i].time_over()) {

			Particles.erase(Particles.begin() + i);
			continue;

		}

		i++;

	}

	for (uint i = 0; i < BGParticles.size(); i++)
		BGParticles[i].update();

}

void Particles_New(int type, double x, double y, double a, double b, int time, double weight, int color) {

	Particles.push_back( Particle(type, x, y, a, b, rand()%63, time, weight, color) );

}

void Particles_DrawFront(int cam_x, int cam_y) {

	for (uint i = 0; i < Particles.size(); i++)
		Particles[i].draw(cam_x, cam_y);

}

void Particles_DrawBG(int cam_x, int cam_y) {

	if (Settings.draw_weather)
		for (uint i = 0; i < BGParticles.size(); i++)
			BGParticles[i].draw(cam_x, cam_y);

}

void Particles_LoadBG(MapClass* map) {
	
	for (int i = 0; i < nof_bg_particles; i++) {

		BGParticles.push_back( Particle(
			BGPARTICLE_NOTHING,   //type
			rand()%screen_width,  //x
			rand()%screen_height, //y
			rand()%10-rand()%10,  //a
			rand()%9+1,           //b
			rand()%63,            //anim
			1,                    //time -- unused
			rand()%10,            //weight
			0));                  //color
		
	}

	if (map->ilma == ILMA_SADE || map->ilma == ILMA_SADEMETSA)
		for( int i = 0; i < nof_bg_particles; i++)
			BGParticles[i].set_type(BGPARTICLE_WATERDROP);

	if (map->ilma == ILMA_METSA || map->ilma == ILMA_SADEMETSA)
		for( int i = 0; i < nof_bg_particles / 8; i++)
			BGParticles[i].set_type(BGPARTICLE_LEAF1 + rand()%4);

	if (map->ilma == ILMA_LUMISADE){
		for( int i = 0; i < nof_bg_particles / 2; i++)
			BGParticles[i].set_type(BGPARTICLE_FLAKE4);
		for( int i = 0; i < nof_bg_particles / 3; i++)
			BGParticles[i].set_type(BGPARTICLE_FLAKE1 + rand()%2+1);//3
	}

}

void Particles_Clear() {

	Particles.clear();
	BGParticles.clear();

}
