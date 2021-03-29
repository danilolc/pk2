//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "episode/episodeclass.hpp"

#include "engine/platform.hpp"

//const int SAVES_COUNT = 11;

struct PK2SAVE {

	bool  empty;
	u32   level;
	char  episode[PE_PATH_SIZE];
	char  name[20];
	u32   score;
	u8    level_status[EPISODI_MAX_LEVELS];
	
};

extern std::vector<PK2SAVE> saves_list;

int Load_SaveFile();
int Save_Record(int i);