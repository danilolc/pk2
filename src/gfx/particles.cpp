//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "gfx/particle.hpp"

#include "game/mapclass.hpp"
#include "settings.hpp"
#include "system.hpp"

#include <cstdlib>
#include <list>
#include <vector>

std::list<Particle> Particles;
std::vector<Particle> BGParticles;

const int nof_bg_particles = 300;

static bool update_done (Particle& p) { 

	p.update();
	return p.time_over(); 

}

void Particles_Update() {

	Particles.remove_if(update_done);

	for (Particle& p : BGParticles)
		p.update();

}

void Particles_New(int type, double x, double y, double a, double b, int time, double weight, int color) {

	Particles.emplace_back(type, x, y, a, b, rand()%63, time, weight, color);

}

void Particles_DrawFront(int cam_x, int cam_y) {

	for (Particle& p : Particles)
		p.draw(cam_x, cam_y);

}

void Particles_DrawBG(int cam_x, int cam_y) {

	if (Settings.draw_weather)
		for (Particle& p : BGParticles)
			p.draw(cam_x, cam_y);

}

void Particles_LoadBG(MapClass* map) {
	
	for (int i = 0; i < nof_bg_particles; i++) {

		BGParticles.emplace_back(
			BGPARTICLE_NOTHING,   //type
			rand()%screen_width,  //x
			rand()%screen_height, //y
			rand()%10-rand()%10,  //a
			rand()%9+1,           //b
			rand()%63,            //anim
			1,                    //time -- unused
			rand()%10,            //weight
			0);                   //color
		
	}

	if (map->ilma == WEATHER_RAIN || map->ilma == WEATHER_RAIN_LEAVES)
		for( int i = 0; i < nof_bg_particles; i++)
			BGParticles[i].set_type(BGPARTICLE_WATERDROP);

	if (map->ilma == WEATHER_LEAVES || map->ilma == WEATHER_RAIN_LEAVES)
		for( int i = 0; i < nof_bg_particles / 8; i++)
			BGParticles[i].set_type(BGPARTICLE_LEAF1 + rand()%4);

	if (map->ilma == WEATHER_SNOW){
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
