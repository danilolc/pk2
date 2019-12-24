#include "particle.hpp"

#include "game.hpp"
#include "settings.hpp"

std::vector<Particle*> Particles;
std::vector<Particle*> BGParticles;

const int nof_bg_particles = 300;

void Particles_Update() {
	
	if (!Game::paused)
		for (int i = 0; i < Particles.size();) {
			Particles[i]->update();
			//if (Particles[i]->time_over) {
			//	Particles.erase(i);
			//	continue;
			//}
			i++;
		}

	//Todo - clean "time_over" particles - use deque on Particles

	if (Settings.saa_efektit)
		for (Particle* particle : BGParticles)
			particle->update();

}

void Particles_New(int type, double x, double y, double a, double b, int time, double weight, int color) {

	Particle* particle = new Particle(type, x, y, a, b, rand()%63, time, weight, color);
	Particles.push_back(particle);

}

void Particles_DrawFront() {

	for (Particle* particle : Particles)
		particle->draw();

}

void Particles_DrawBG() {

	if (!Settings.saa_efektit) return;

	for (Particle* particle : BGParticles)
		particle->draw();

}

void Particles_LoadBG(PK2Kartta* map) {
	int i = 0;
	Particle* particle;

	while (i < nof_bg_particles){
		Particle* particle = new Particle(
			BGPARTICLE_NOTHING,   //type
			rand()%screen_width,  //x
			rand()%screen_height, //y
			rand()%10-rand()%10,  //a
			rand()%9+1,           //b
			rand()%63,            //anim
			1,                    //time -- unused
			rand()%10,            //weight
			0);                   //color
		BGParticles.push_back(particle);
		i++;
	}

	if (map->ilma == ILMA_SADE || map->ilma == ILMA_SADEMETSA)
		for( i = 0; i < nof_bg_particles; i++)
			BGParticles[i]->set_type(BGPARTICLE_WATERDROP);

	if (map->ilma == ILMA_METSA || map->ilma == ILMA_SADEMETSA)
		for( i = 0; i < nof_bg_particles / 8; i++)
			BGParticles[i]->set_type(BGPARTICLE_LEAF1 + rand()%4);

	if (map->ilma == ILMA_LUMISADE){
		for( i = 0; i < nof_bg_particles / 2; i++)
			BGParticles[i]->set_type(BGPARTICLE_FLAKE4);
		for( i = 0; i < nof_bg_particles / 3; i++)
			BGParticles[i]->set_type(BGPARTICLE_FLAKE1 + rand()%2+1);//3
	}

}

void Particles_Clear() {
	while (Particles.size() > 0) {
		delete Particles.back();
		Particles.pop_back();
	}
	while (BGParticles.size() > 0) {
		delete BGParticles.back();
		BGParticles.pop_back();
	}
}
