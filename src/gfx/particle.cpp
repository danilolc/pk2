//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "gfx/particle.hpp"

#include "engine/PDraw.hpp"
#include "game/game.hpp"

#include "settings/settings.hpp"
#include "system.hpp"

Particle::Particle(int type, double x, double y, double a, double b, int anim, int time, double weight, int color) {

	this->type = type;
	this->x = x;
	this->y = y;
	this->a = a;
	this->b = b;
	this->anim = anim;
	this->time = time;
	this->weight = weight;
	this->color = color;
	
}

Particle::~Particle() {}

void Particle::draw(int camera_x, int camera_y) {

	alpha = time;
	if (alpha > 100) alpha = 100;

	this->cam_x = camera_x;
	this->cam_y = camera_y;

	if (time > 0)
		switch (this->type) {
			case PARTICLE_STAR:        draw_star();      break;
			case PARTICLE_FEATHER:     draw_feather();   break;
			case PARTICLE_DUST_CLOUDS: draw_dust();      break;
			case PARTICLE_LIGHT:       draw_light();     break;
			case PARTICLE_SPARK:       draw_spark();     break;
			case PARTICLE_POINT:       draw_dot();       break;
			case PARTICLE_SMOKE:       draw_smoke();     break;
		}
}

void Particle::set_type(int type){

	this->type = type;

}

bool Particle::time_over() {

	return (time == 0);

}

void Particle::draw_dot() {

	PDraw::screen_fill(x-cam_x, y-cam_y, x-cam_x+1, y-cam_y+1, color+25);

}

void Particle::draw_star() {

	if (alpha > 99)
		PDraw::image_cutclip(Game->gfxTexture, x-cam_x, y-cam_y, 1, 1, 11, 11);
	else
		PDraw::image_cutcliptransparent(Game->gfxTexture, 2, 2, 10, 10, x-cam_x, y-cam_y, alpha, color);

}

void Particle::draw_hit() {

	int framex = ((degree%12)/3) * 58;
	PDraw::image_cutclip(Game->gfxTexture,x-cam_x-28+8, y-cam_y-27+8,1+framex,83,1+57+framex,83+55);
}

void Particle::draw_light() {
	PDraw::image_cutcliptransparent(Game->gfxTexture, 1, 14, 13, 13, x-cam_x, y-cam_y, alpha, color);
}

void Particle::draw_spark() {
	PDraw::image_cutcliptransparent(Game->gfxTexture, 99, 14, 7, 7, x-cam_x, y-cam_y, alpha, color);
}

void Particle::draw_feather() {

	int xplus = (anim/7) * 21;
	PDraw::image_cutclip(Game->gfxTexture,x-cam_x,y-cam_y,14+xplus,1,34+xplus,12);
	anim++;
	if (anim > 63)
		anim = 0;

}

void Particle::draw_smoke() {

	int frame = (anim/7);
	int xplus = (frame%17) * 36;
	int yplus = 0;

	if (anim < 7*30) {

		if (frame > 16)
			yplus = 32;

		PDraw::image_cutclip(Game->gfxTexture,x-cam_x,y-cam_y,1+xplus,338+yplus,34+xplus,367+yplus);
		anim++;
	}

}

void Particle::draw_dust() {

	if (alpha > 99)
		PDraw::image_cutclip(Game->gfxTexture,x-cam_x,y-cam_y,226,2,224,49);
	else
		PDraw::image_cutcliptransparent(Game->gfxTexture, 226, 2, 18, 19, int(x)-cam_x, int(y)-cam_y, alpha, color);
	PDraw::image_cutclip(Game->gfxTexture,x-cam_x,y-cam_y,226, 2, 18, 19);

}

void Particle::update() {

	if (this->time > 0){
		this->x += this->a;
		this->y += this->b;

		if (this->weight > 0)
			this->b += this->weight;

		this->time--;
	}

}
