//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "bg_particles.hpp"
#include <array>
#include "bg_particle.hpp"
#include "system.hpp"

namespace BG_Particles{


static std::array<BgParticle, BACKGROUND_PARTICLES_NUMBER> bg_particles;

void Init(int weather, int rain_color){

	for(BgParticle& particle: bg_particles){
		particle.setType(BGPARTICLE_NOTHING);

		particle.x = rand()%screen_width;
		particle.y = rand()%screen_height;
		particle.a = rand()%10-rand()%10;
		particle.b = rand()%9+1;
	}

	if (weather == WEATHER_RAIN || weather == WEATHER_RAIN_LEAVES){
		for(BgParticle& p : bg_particles){
			p.setType(BGPARTICLE_WATERDROP);
			p.color = (u8)rain_color;
		}
	}
	
	
	if(weather == WEATHER_LEAVES || weather == WEATHER_RAIN_LEAVES){
		for( std::size_t i = 0; i < bg_particles.size() / 8; i++){
			bg_particles[i].setType(BGPARTICLE_LEAF1 + rand()%4);
		}			
	}

	if (weather == WEATHER_SNOW){
		std::size_t i = 0;
		while(i < bg_particles.size() / 3){
			bg_particles[i].setType(BGPARTICLE_FLAKE1 + rand()%2+1);
			++i;
		}

		while (i < bg_particles.size() / 2){
			bg_particles[i].setType(BGPARTICLE_FLAKE4);
			++i;
		}
	}

	else if(weather == WEATHER_DANDELIONS){
		for( std::size_t i = 0; i < bg_particles.size() / 6; i++){
			bg_particles[i].setType(BGPARTICLE_DANDELION1 + rand() % 3);
			bg_particles[i].b = double(rand()%18 - 7)/10;
			bg_particles[i].a = double(rand()%17 - 8)/10;
		}
	}
}

void Draw(int camera_x, int camera_y){
	for(const BgParticle& particle: bg_particles){
		if(particle.getType()!=BGPARTICLE_NOTHING){
			particle.draw(camera_x, camera_y);
		}
	}
}

void Update(int camera_x, int camera_y){
    for(BgParticle& particle: bg_particles){
        if(particle.getType()!=BGPARTICLE_NOTHING){
			particle.checkScreenBorders(camera_x, camera_y);
			particle.update();
		}
    }
}

}