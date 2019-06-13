#pragma once

#include "PisteLanguage.hpp"
#include "map.hpp"
#include "types.hpp"
#include "platform.hpp"
#include "sprite.hpp"
#include <vector>
#include <string>

enum PARTICLES{
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

namespace PK2 {

class Particle {

	public:

		Particle(int type, double x, double y, double a, double b, int anim, int time, double weight, int color);
		~Particle();
		void draw();
		void update();

		void set_type(int type);
		bool time_over();

	private:

		int	type;
		int	time;

		double x, y, a, b;
		int anim;
		
		double weight;
		int color;
		int alpha;

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

class Particle_System {

	public:

	Particle_System();
	~Particle_System();

	void update();
	void new_particle(int type, double x, double y, double a, double b, int time, double weight, int color);

	void draw_front_particles();
	void draw_bg_particles();

	void load_bg_particles(PK2Kartta* map);

	void clear_particles();

	private:

	std::vector<Particle*> Particles;
	std::vector<Particle*> BGParticles;

	const int nof_bg_particles = 300;

};

class SpriteSystem {

	int next_free_prototype = 0;

	int Get_Prototype(std::string filename);

	int  protot_get(char *polku, char *tiedosto);
	
	int  protot_get_sound(std::string str);
	void protot_get_transformation(int i);
	void protot_get_bonus(int i);
	void protot_get_ammo1(int i);
	void protot_get_ammo2(int i);

	void add_bg(int index);
	

	public:

	SpriteSystem();
	~SpriteSystem();

	PK2Sprite* player;

	std::vector<PK2Sprite_Prototype*> prototypes;

	std::vector<PK2Sprite> sprites;
	std::vector<int> background_sprites;


	int  protot_get_all();
	void protot_clear_all();

	void clear();
	void add(int protoype_id, int pelaaja, double x, double y, int parent_sprite, bool isbonus);
	void add_ammo(int protoype_id, int pelaaja, double x, double y, int emo);
	void sort_bg();
	void start_directions();
	
};

#define MAX_GIFTS 4

class GiftSystem {
	public:
	int list[MAX_GIFTS];
	int gift_count;

	int get(int i);
	void set(int index, int value);

	int count();
	PK2Sprite_Prototype* get_protot(int i);

	void draw(int i, int x, int y);

	void clean();
	bool add(int prototype_id);
	int  use();
	int  change_order();

	GiftSystem();
	~GiftSystem();
};

}


//In game variables
namespace Game {

	extern PK2::Particle_System* Particles;
	extern PK2::SpriteSystem* Sprites;
	extern PK2::GiftSystem* Gifts;

	extern PK2Kartta* current_map;
	extern char map_path[PE_PATH_SIZE];

	extern int vibration;

	extern int camera_x;
	extern int camera_y;
	extern double dcamera_x;
	extern double dcamera_y;
	extern double dcamera_a;
	extern double dcamera_b;

}

namespace Effect {

    void Feathers(DWORD x, DWORD y);
    void Splash(DWORD x, DWORD y, BYTE color);
    void Explosion(DWORD x, DWORD y, BYTE color);
    void Smoke(DWORD x, DWORD y, BYTE color);
    void SmokeClouds(DWORD x, DWORD y);
    void Stars(DWORD x, DWORD y, BYTE color);
    void Destruction(BYTE tehoste, DWORD x, DWORD y);

}



void PK_Start_Info(char *text);
extern PisteLanguage *tekstit;
struct LANGUAGE{
	int setup_options,
		setup_videomodes,
		setup_music_and_sounds,
		setup_music,
		setup_sounds,
		setup_language,
		setup_play,
		setup_exit,

		intro_presents,
		intro_a_game_by,
		intro_original,
		intro_tested_by,
		intro_thanks_to,
		intro_translation,
		intro_translator,

		mainmenu_new_game,
		mainmenu_continue,
		mainmenu_load_game,
		mainmenu_save_game,
		mainmenu_controls,
		mainmenu_graphics,
		mainmenu_sounds,
		mainmenu_exit,

		mainmenu_return,

		loadgame_title,
		loadgame_info,
		loadgame_episode,
		loadgame_level,

		savegame_title,
		savegame_info,
		savegame_episode,
		savegame_level,

		controls_title,
		controls_moveleft,
		controls_moveright,
		controls_jump,
		controls_duck,
		controls_walkslow,
		controls_eggattack,
		controls_doodleattack,
		controls_useitem,
		controls_edit,
		controls_kbdef,
		controls_gp4def,
		controls_gp6def,

		gfx_title,
		gfx_tfx_on,
		gfx_tfx_off,
		gfx_tmenus_on,
		gfx_tmenus_off,
		gfx_items_on,
		gfx_items_off,
		gfx_weather_on,
		gfx_weather_off,
		gfx_bgsprites_on,
		gfx_bgsprites_off,
		gfx_speed_normal,
		gfx_speed_double,

		sound_title,
		sound_sfx_volume,
		sound_music_volume,
		sound_more,
		sound_less,

		playermenu_type_name,
		playermenu_continue,
		playermenu_clear,
		player_default_name,

		episodes_choose_episode,
		episodes_no_maps,
		episodes_get_more,

		map_total_score,
		map_next_level,
		map_episode_best_player,
		map_episode_hiscore,
		map_level_best_player,
		map_level_hiscore,
		map_level_fastest_player,
		map_level_best_time,

		score_screen_title,
		score_screen_level_score,
		score_screen_bonus_score,
		score_screen_time_score,
		score_screen_energy_score,
		score_screen_item_score,
		score_screen_total_score,
		score_screen_new_level_hiscore,
		score_screen_new_level_best_time,
		score_screen_new_episode_hiscore,
		score_screen_continue,

		game_score,
		game_time,
		game_energy,
		game_items,
		game_attack1,
		game_attack2,
		game_keys,
		game_clear,
		game_timebonus,
		game_ko,
		game_timeout,
		game_tryagain,
		game_locksopen,
		game_newdoodle,
		game_newegg,
		game_newitem,
		game_loading,
		game_paused,

		game_collectables,

		end_congratulations,
		end_chickens_saved,
		end_the_end,

		info01,
		info02,
		info03,
		info04,
		info05,
		info06,
		info07,
		info08,
		info09,
		info10,
		info11,
		info12,
		info13,
		info14,
		info15,
		info16,
		info17,
		info18,
		info19;
};
extern LANGUAGE PK_txt;