//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "episode/mapstore.hpp"

#include "engine/platform.hpp"
#include "engine/PFile.hpp"
#include "engine/PLang.hpp"

const int EPISODI_MAX_LEVELS = 100; //50;

struct PK2LEVEL {

	char  tiedosto[PE_PATH_SIZE];
	char  nimi[40];
	int   x, y;
	u32   order;
	u32   icon;
	
};

enum LEVEL_STATUS {

	LEVEL_PASSED    = 0b01,
	LEVEL_ALLAPPLES = 0b10

};

struct PK2EPISODESCORES {

	u8   has_score[EPISODI_MAX_LEVELS];          // if somebody played the level

	u32  best_score[EPISODI_MAX_LEVELS];         // the best score of each level in episode
	char top_player[EPISODI_MAX_LEVELS][20];     // the name of the player with more score in each level on episode
	
	u32  max_apples[EPISODI_MAX_LEVELS];         // max big apples get on level
	
	u8   has_time[EPISODI_MAX_LEVELS];           // if the level has time counter
	s32  best_time[EPISODI_MAX_LEVELS];          // the best time in frames
	char fastest_player[EPISODI_MAX_LEVELS][20]; // the name of the fastest player in each level

	u32  episode_top_score;
	char episode_top_player[20];
	
};

//Scores 1.0
struct PK2EPISODESCORES10 {

	u32  best_score[EPISODI_MAX_LEVELS];         // the best score of each level in episode
	char top_player[EPISODI_MAX_LEVELS][20];     // the name of the player with more score in each level on episode
	u32  best_time[EPISODI_MAX_LEVELS];          // the best time in (dec)conds
	char fastest_player[EPISODI_MAX_LEVELS][20]; // the name of the fastest player in each level

	u32  episode_top_score;
	char episode_top_player[20];
	
};

class EpisodeClass {

	public:
	
		episode_entry entry;
		PFile::Zip* source_zip;
		
		char player_name[20] = " ";
		u32 player_score = 0;

		u32 next_level = 1;
		u32 level_count = 0;

		bool glows = false;
		bool hide_numbers = false;
		bool ignore_collectable = false;
		bool require_all_levels = false;
		bool no_ending = false;
		bool use_button_timer = false;
		std::string collectable_name = "big apple";
		
		PK2LEVEL levels_list[EPISODI_MAX_LEVELS];
		u8 level_status[EPISODI_MAX_LEVELS];

		PK2EPISODESCORES scores;

		PLang infos;

		EpisodeClass(int save);
		EpisodeClass(const char* player_name, episode_entry entry);
		~EpisodeClass();

		void Load();

		void Load_Info();
		void Load_Assets();

		PFile::Path Get_Dir(std::string file);

		int  Open_Scores();
		int  Save_Scores();

		void Update_NextLevel();

	private:
		
		void Clear_Scores();

};

extern EpisodeClass* Episode;
