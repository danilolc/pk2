//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "engine/platform.hpp"

const int EPISODI_MAX_LEVELS = 100; //50;
const int MAX_EPISODEJA	= 300;

struct PK2LEVEL {

	char  tiedosto[PE_PATH_SIZE];
	char  nimi[40];
	int   x, y;
	int   order;
	bool  cleared;
	bool  all_apples;
	int   icon;
	
};

//TODO - Make Scores 1.1
struct PK2EPISODESCORES {
	DWORD best_score[EPISODI_MAX_LEVELS];         // the best score of each level in episode
	char  top_player[EPISODI_MAX_LEVELS][20];     // the name of the player with more score in each level on episode
	DWORD best_time[EPISODI_MAX_LEVELS];          // the best time of each level
	char  fastest_player[EPISODI_MAX_LEVELS][20]; // the name of the fastest player in each level

	DWORD episode_top_score;
	char  episode_top_player[20];
};

class EpisodeClass {

	public:
	
		char name[PE_PATH_SIZE] = " ";
		
		char player_name[20] = " ";
		DWORD player_score = 0;

		int level = 1;
		int level_count = 0;
		PK2LEVEL levels_list[EPISODI_MAX_LEVELS];
		PK2EPISODESCORES scores;

		EpisodeClass(int save);
		EpisodeClass(const char* player_name, const char* episode);
		~EpisodeClass();

		void Load();

		void Load_Info();

		void Get_Dir(char *tiedosto);

		int  Open_Scores(const char *filename);
		int  Save_Scores(const char *filename);

	private:
		
		void Clear_Scores();

};

extern EpisodeClass* Episode;
