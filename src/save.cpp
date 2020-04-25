//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "save.hpp"

#include "episode/mapstore.hpp"
#include "system.hpp"

#include "engine/PLog.hpp"

#include <cstring>
#include <string>

#define SAVES_FILE "saves.dat"
#define VERSION "2"

PK2SAVE saves_list[SAVES_COUNT];

int Empty_Records() {

	memset(saves_list, 0, sizeof(saves_list));

	for (int i = 0; i < SAVES_COUNT; i++) {

		saves_list[i].empty = true;
		strcpy(saves_list[i].episode, " ");
		strcpy(saves_list[i].name,"empty");
		saves_list[i].level = 0;
		saves_list[i].score = 0;
		for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
			saves_list[i].level_status[j] = 0;
		
	}

	return 0;
	
}

int Save_All_Records() {

	char versio[2] = VERSION;
	char count_c[8];

	memset(count_c, 0, sizeof(count_c));
	snprintf(count_c, sizeof(count_c), "%i", SAVES_COUNT);

	PFile::Path path(data_path + SAVES_FILE);
	
	PFile::RW* file = path.GetRW("wb");
	if (file == nullptr) {

		PLog::Write(PLog::ERR, "PK2", "Can't save records");
		return 1;

	}

	PFile::WriteRW(file, versio, sizeof(versio)); // Write version "2"
	PFile::WriteRW(file, count_c, sizeof(count_c)); // Write count "10"
	PFile::WriteRW(file, saves_list, sizeof(saves_list)); // Write saves
	
	PFile::CloseRW(file);
	
	return 0;

}

int Load_SaveFile() {

	char versio[2];
	char count_c[8];
	int count;

	Empty_Records();

	PFile::Path path(data_path + SAVES_FILE);

	PFile::RW* file = path.GetRW("rb");
	if (file == nullptr){

		PLog::Write(PLog::INFO, "PK2", "No save file");
		return 1;
	
	}

	PFile::ReadRW(file, versio, sizeof(versio));

	if (strncmp(versio,"2", 2) == 0) {

		PFile::ReadRW(file, count_c, sizeof(count_c));
		count = atoi(count_c);
		if (count > SAVES_COUNT)
			count = SAVES_COUNT;

		PFile::ReadRW(file, saves_list, sizeof(PK2SAVE) * count);
	
	} else {

		PLog::Write(PLog::INFO, "PK2", "Can't read this save version");
		return 1;

	}

	PFile::CloseRW(file);

	return 0;

}

int Save_Records(int i) {

	memset(&saves_list[i], 0, sizeof(PK2SAVE));

	saves_list[i].empty = false;
	strcpy(saves_list[i].episode, Episode->entry.name.c_str());
	strcpy(saves_list[i].name, Episode->player_name);
	saves_list[i].level = Episode->level;
	saves_list[i].score = Episode->player_score;

	for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
		saves_list[i].level_status[j] = Episode->level_status[j];

	Save_All_Records();

	return 0;

}
