//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "gfx/particle.hpp"

#include "game/levelclass.hpp"
#include "system.hpp"

#include <cstdlib>
#include <list>
#include <vector>

std::list<Particle> Particles;
static bool update_done (Particle& p) { 

	p.update();
	return p.time_over(); 

}

void Particles_Update() {
	Particles.remove_if(update_done);
}

void Particles_New(int type, double x, double y, double a, double b, int time, double weight, int color) {

	Particles.emplace_back(type, x, y, a, b, rand()%63, time, weight, color);

}

void Particles_DrawFront(int cam_x, int cam_y) {
	for (Particle& p : Particles)
		p.draw(cam_x, cam_y);

}

void Particles_Clear() {

	Particles.clear();
}
