//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

enum PARTICLE {

	PARTICLE_NOTHING,
	PARTICLE_STAR,
	PARTICLE_FEATHER,
	PARTICLE_DUST_CLOUDS,
	PARTICLE_LIGHT,
	PARTICLE_SPARK,
	PARTICLE_POINT,
	PARTICLE_SMOKE,
	
	BGPARTICLE_NOTHING,
	BGPARTICLE_WATERDROP,
	BGPARTICLE_LEAF1,
	BGPARTICLE_LEAF2,
	BGPARTICLE_LEAF3,
	BGPARTICLE_LEAF4,
	BGPARTICLE_FLAKE1,
	BGPARTICLE_FLAKE2,
	BGPARTICLE_FLAKE3,
	BGPARTICLE_FLAKE4
	
};

class Particle {

	public:

		Particle(int type, double x, double y, double a, double b, int anim, int time, double weight, int color);
		~Particle();
		void draw(int cam_x, int cam_y);
		void update();

		void set_type(int type);
		bool time_over();

	private:

		int type;
		int time;

		double x, y, a, b;
		int anim;
		
		double weight;
		int color;
		int alpha;

		int cam_x, cam_y;

		//Draw FG
		void draw_dot();
		void draw_star();
		void draw_hit();
		void draw_light();
		void draw_spark();
		void draw_feather();
		void draw_smoke();
		void draw_dust();

		//Draw BG
		void draw_waterdrop();
		void draw_leaf1();
		void draw_leaf2();
		void draw_leaf3();
		void draw_leaf4();
		void draw_flake1();
		void draw_flake2();
		void draw_flake3();
		void draw_flake4();

		//Update FG
		void update_fg();

		//Update BG
		void update_bg();
		void update_waterdrop();
		void update_leaf1();
		void update_leaf2();
		void update_leaf3();
		void update_leaf4();
		void update_flake1();
		void update_flake2();
		void update_flake3();
		void update_flake4();

};
