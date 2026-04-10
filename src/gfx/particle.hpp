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
	PARTICLE_SMOKE
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

		void draw_dot();
		void draw_star();
		void draw_hit();
		void draw_light();
		void draw_spark();
		void draw_feather();
		void draw_smoke();
		void draw_dust();

};
