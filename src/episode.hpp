//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "platform.hpp"


const int EPISODI_MAX_LEVELS = 100; //50;
const int MAX_EPISODEJA	= 300;

struct PK2LEVEL {
	char  tiedosto[PE_PATH_SIZE];
	char  nimi[40];
	int   x,y;
	int   jarjestys;
	bool  lapaisty;
	int   ikoni;
};

struct PK2EPISODESCORES {
	DWORD best_score[EPISODI_MAX_LEVELS];         // the best score of each level in episode
	char  top_player[EPISODI_MAX_LEVELS][20];     // the name of the player with more score in each level on episode
	DWORD best_time[EPISODI_MAX_LEVELS];          // the best time of each level
	char  fastest_player[EPISODI_MAX_LEVELS][20]; // the name of the fastest player in each level

	DWORD episode_top_score;
	char  episode_top_player[20];
};

namespace Episode {
	extern bool started;
	extern char player_name[20];
	extern DWORD score;

	void Load_Save(int save);
	void Load_New(const char* player_name, const char* episode);
	void Get_Dir(char *tiedosto);

	void Load();
}

extern int jakso;
extern int jaksoja;
extern int jakso_indeksi_nyt;
extern PK2LEVEL jaksot[EPISODI_MAX_LEVELS];
extern char episodi[PE_PATH_SIZE];

extern DWORD lopetusajastin;
extern DWORD fake_pisteet;

extern PK2EPISODESCORES episodipisteet;

extern bool jakso_uusi_ennatys;
extern bool jakso_uusi_ennatysaika;
extern bool episodi_uusi_ennatys;

void Fade_Quit();

void Clean_Maps();

void EpisodeScore_Start();
int  EpisodeScore_Compare(int jakso, DWORD episteet, DWORD aika, bool loppupisteet);
int  EpisodeScore_Open(char *filename);
int  EpisodeScore_Save(char *filename);
void Episode::Get_Dir(char *tiedosto);
