//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "gfx/particle.hpp"

#include "engine/PDraw.hpp"

#include "settings.hpp"
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

			case BGPARTICLE_WATERDROP: draw_waterdrop(); break;
			case BGPARTICLE_LEAF1:     draw_leaf1();     break;
			case BGPARTICLE_LEAF2:     draw_leaf2();     break;
			case BGPARTICLE_LEAF3:     draw_leaf3();     break;
			case BGPARTICLE_LEAF4:     draw_leaf4();     break;
			case BGPARTICLE_FLAKE1:    draw_flake1();    break;
			case BGPARTICLE_FLAKE2:    draw_flake2();    break;
			case BGPARTICLE_FLAKE3:    draw_flake3();    break;
			case BGPARTICLE_FLAKE4:    draw_flake4();    break;
		}
}

void Particle::update() {

	switch (this->type) {
		case PARTICLE_STAR:
		case PARTICLE_FEATHER:
		case PARTICLE_DUST_CLOUDS:
		case PARTICLE_LIGHT:
		case PARTICLE_SPARK:
		case PARTICLE_POINT:
		case PARTICLE_SMOKE: 
			update_fg(); break;
		case BGPARTICLE_WATERDROP: update_waterdrop(); update_bg(); break;
		case BGPARTICLE_LEAF1:     update_leaf1();     update_bg(); break;
		case BGPARTICLE_LEAF2:     update_leaf2();     update_bg(); break;
		case BGPARTICLE_LEAF3:     update_leaf3();     update_bg(); break;
		case BGPARTICLE_LEAF4:     update_leaf4();     update_bg(); break;
		case BGPARTICLE_FLAKE1:    update_flake1();    update_bg(); break;
		case BGPARTICLE_FLAKE2:    update_flake2();    update_bg(); break;
		case BGPARTICLE_FLAKE3:    update_flake3();    update_bg(); break;
		case BGPARTICLE_FLAKE4:    update_flake4();    update_bg(); break;
		default: break;
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

	if (alpha > 99 || !Settings.draw_transparent)
		PDraw::image_cutclip(game_assets, x-cam_x, y-cam_y, 1, 1, 11, 11);
	else
		PDraw::image_cutcliptransparent(game_assets, 2, 2, 10, 10, x-cam_x, y-cam_y, alpha, color);

}

void Particle::draw_hit() {

	int framex = ((degree%12)/3) * 58;
	PDraw::image_cutclip(game_assets,x-cam_x-28+8, y-cam_y-27+8,1+framex,83,1+57+framex,83+55);
}

void Particle::draw_light() {

	if (Settings.draw_transparent)
		PDraw::image_cutcliptransparent(game_assets, 1, 14, 13, 13, x-cam_x, y-cam_y, alpha, color);
	else{
		int vx = (color/32) * 14;
		PDraw::image_cutclip(game_assets,x-cam_x, y-cam_y,1+vx,14+14,14+vx,27+14);
	}

}

void Particle::draw_spark() {

	if (Settings.draw_transparent) {
	
		PDraw::image_cutcliptransparent(game_assets, 99, 14, 7, 7, x-cam_x, y-cam_y, alpha, color);
	
	} else {

		int vx = (color/32) * 8;
		PDraw::image_cutclip(game_assets,x-cam_x, y-cam_y,99+vx,14+14,106+vx,21+14);
	
	}

}

void Particle::draw_feather() {

	int xplus = (anim/7) * 21;
	PDraw::image_cutclip(game_assets,x-cam_x,y-cam_y,14+xplus,1,34+xplus,12);
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

		PDraw::image_cutclip(game_assets,x-cam_x,y-cam_y,1+xplus,338+yplus,34+xplus,366+yplus);
		anim++;
	}

}

void Particle::draw_dust() {

	if (alpha > 99 || !Settings.draw_transparent)
		PDraw::image_cutclip(game_assets,x-cam_x,y-cam_y,226,2,224,49);
	else
		PDraw::image_cutcliptransparent(game_assets, 226, 2, 18, 19, int(x)-cam_x, int(y)-cam_y, alpha, color);
	PDraw::image_cutclip(game_assets,x-cam_x,y-cam_y,226, 2, 18, 19);

}

void Particle::draw_waterdrop() {

	int kx = int(x)-cam_x;
	int ky = int(y)-cam_y;

	PDraw::screen_fill(kx,ky,kx+1,ky+4,40+(int)b);

}

void Particle::draw_leaf1() {

	int kx = int(x)-cam_x,
		ky = int(y)-cam_y;

	PDraw::screen_fill(kx,ky,kx+2,ky+2,96+6+(int)b+int(x+y)%10);

}

void Particle::draw_leaf2() {

	int kx = int(x)-cam_x,
		ky = int(y)-cam_y,
		frame = (int(y/10)%4)*23;

	PDraw::image_cutclip(game_assets,kx,ky,1+frame,141,21+frame,152);

}

void Particle::draw_leaf3() {

	int kx = int(x)-cam_x,
		ky = int(y)-cam_y,
		frame = (int(y/5)%4)*20;

	PDraw::image_cutclip(game_assets,kx,ky,93+frame,141,109+frame,150);

}

void Particle::draw_leaf4() {

	int kx = int(x)-cam_x,
		ky = int(y)-cam_y,
		frame = (int(y/5)%2)*14;

	PDraw::image_cutclip(game_assets,kx,ky,173+frame,141,183+frame,150);

}

void Particle::draw_flake1() {

	int kx = int(x)-cam_x,
		ky = int(y)-cam_y;

	PDraw::image_cutclip(game_assets,kx,ky,1,155,8,162);

}

void Particle::draw_flake2() {

	int kx = int(x)-cam_x,
		ky = int(y)-cam_y;

	PDraw::image_cutclip(game_assets,kx,ky,11,155,16,160);

}

void Particle::draw_flake3() {

	int kx = int(x)-cam_x,
		ky = int(y)-cam_y;

	PDraw::image_cutclip(game_assets,kx,ky,19,155,22,158);

}

void Particle::draw_flake4() {

	int kx = int(x)-cam_x,
		ky = int(y)-cam_y;

	PDraw::screen_fill(kx,ky,kx+2,ky+2,20+(int)b);

}

void Particle::update_waterdrop() {

	y += b / 3.0 + 2.0;

}

void Particle::update_leaf1() {

	x += a / 9.0;
	y += b / 9.0;

}

void Particle::update_leaf2() {

	x += a / 14.0;
	y += b / 9.0;

}

void Particle::update_leaf3() {

	x += a / 12.0;
	y += b / 9.0;

}

void Particle::update_leaf4() {

	x += a / 11.0;
	y += b / 9.0;

}

void Particle::update_flake1() {

	x += sin_table(y)/50.0;
	y += b / 7.0;

}

void Particle::update_flake2() {

	x += sin_table(y)/100.0;
	y += b / 8.0;

}

void Particle::update_flake3() {

	y += b / 8.0;

}

void Particle::update_flake4() {

	y += b / 9.0;

}

void Particle::update_fg() {

	if (this->time > 0){
		this->x += this->a;
		this->y += this->b;

		if (this->weight > 0)
			this->b += this->weight;

		this->time--;
	}

}

void Particle::update_bg() {

	if ( x  >  cam_x + screen_width )
		x  =  cam_x + int(x - cam_x + screen_width) % screen_width;
	if ( x  <  cam_x )
		x  =  cam_x + screen_width - int(cam_x - x) % screen_width;
	if ( y  >  cam_y + screen_height )
		y  =  cam_y + int(y - cam_y + screen_height) % screen_height;
	if ( y  <  cam_y )
		y  =  cam_y + screen_height - int(cam_y - y) % screen_height;
	
}

