//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "episode/mapstore.hpp"

#include "engine/platform.hpp"
#include "engine/PFile.hpp"

const int EPISODI_MAX_LEVELS = 100; //50;
const int MAX_EPISODEJA	= 300;

struct PK2LEVEL {

	char  tiedosto[PE_PATH_SIZE];
	char  nimi[40];
	int   x, y;
	int   order;
	int   icon;
	
};

enum LEVEL_STATUS {

	LEVEL_UNCLEAR   = 0b00,
	LEVEL_PASSED    = 0b01,
	LEVEL_ALLAPPLES = 0b10

};

//TODO - Make Scores 1.1
struct PK2EPISODESCORES {

	u32  best_score[EPISODI_MAX_LEVELS];         // the best score of each level in episode
	char top_player[EPISODI_MAX_LEVELS][20];     // the name of the player with more score in each level on episode
	u32  best_time[EPISODI_MAX_LEVELS];          // the best time of each level
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

		int level = 1;
		int level_count = 0;
		
		PK2LEVEL levels_list[EPISODI_MAX_LEVELS];
		u8 level_status[EPISODI_MAX_LEVELS];

		PK2EPISODESCORES scores;

		EpisodeClass(int save);
		EpisodeClass(const char* player_name, episode_entry entry);
		~EpisodeClass();

		void Load();

		void Load_Info();
		void Load_Assets();

		PFile::Path Get_Dir();

		int  Open_Scores();
		int  Save_Scores();

	private:
		
		void Clear_Scores();

};

extern EpisodeClass* Episode;
