//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PLang.hpp"

#include <vector>
#include <string>

extern PLang *tekstit;

const uint MAX_INFOS = 100;

struct LANGUAGE {

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

		end_congratulations,
		end_chickens_saved,
		end_the_end,

		infos[MAX_INFOS];
	
};

extern LANGUAGE PK_txt;

extern std::vector<std::string> langlist;

const char* Language_Name();
int Load_Language(const char* language);