//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "episode/episodeclass.hpp"

#include "engine/platform.hpp"

const int MAX_SAVES = 10;

struct PK2SAVE {

	bool  empty;
	u32   level;
	char  episode[PE_PATH_SIZE];
	char  name[20];
	u32   score;
	bool  level_cleared[EPISODI_MAX_LEVELS];
	bool  all_apples[EPISODI_MAX_LEVELS];
	
};

extern PK2SAVE saves_list[MAX_SAVES];

int Load_SaveFile();
int Save_Records(int i);