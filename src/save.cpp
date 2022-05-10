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
#define VERSION "3"

struct PK2SAVE_V1{
	s32   jakso;
	char  episodi[260]; //260, 256, 128?
	char  nimi[20];
	bool  kaytossa;
	bool  jakso_lapaisty[100]; //100, 50?
	bool  _[3]; // ?
	s32   pisteet;
};

PK2SAVE saves_list[SAVES_COUNT];

int Empty_Records() {

	memset(saves_list, 0, sizeof(saves_list));

	for (int i = 0; i < SAVES_COUNT; i++)
		saves_list[i].empty = true;

	return 0;
	
}

int Save_All_Records() {

	char versio[2] = VERSION;
	char count_c[8];

	memset(count_c, 0, sizeof(count_c));
	snprintf(count_c, sizeof(count_c), "%i", SAVES_COUNT);

	PFile::Path path(data_path, SAVES_FILE);
	
	PFile::RW* file = path.GetRW("w");
	if (file == nullptr) {

		PLog::Write(PLog::ERR, "PK2", "Can't save records");
		return 1;

	}

	file->write(versio, sizeof(versio)); // Write version "2"
	file->write(count_c, sizeof(count_c)); // Write count "11"

	// Write saves
	for (int i = 0; i < SAVES_COUNT; i++) {

		file->write(saves_list[i].empty);
		file->write(saves_list[i].next_level);
		file->write(saves_list[i].episode, PE_PATH_SIZE);
		file->write(saves_list[i].name, 20);
		file->write(saves_list[i].score);
		file->write(saves_list[i].level_status, EPISODI_MAX_LEVELS);

	}
	
	file->close();
	
	return 0;

}

int Load_SaveFile() {

	char versio[2];
	char count_c[8];
	int count;

	Empty_Records();

	PFile::Path path(data_path, SAVES_FILE);

	PFile::RW* file = path.GetRW("r");
	if (file == nullptr){

		PLog::Write(PLog::INFO, "PK2", "No save file");
		Save_All_Records();
		return 1;
	
	}

	file->read(versio, sizeof(versio));

	// Version 3 is always little endian
	if (strncmp(versio, "3", 2) == 0) {

		PLog::Write(PLog::INFO, "PK2", "Loading save version 3");

		file->read(count_c, sizeof(count_c));
		count = atoi(count_c);
		if (count > SAVES_COUNT)
			count = SAVES_COUNT;

		for (int i = 0; i < count; i++) {
		
			file->read(saves_list[i].empty);
			file->read(saves_list[i].next_level);
			file->read(saves_list[i].episode, PE_PATH_SIZE);
			file->read(saves_list[i].name, 20);
			file->read(saves_list[i].score);
			file->read(saves_list[i].level_status, EPISODI_MAX_LEVELS);

		}
	
	} else if (strncmp(versio, "2", 2) == 0) {

		PLog::Write(PLog::INFO, "PK2", "Loading save version 2");

		file->read(count_c, sizeof(count_c));
		count = atoi(count_c);
		if (count > SAVES_COUNT)
			count = SAVES_COUNT;

		file->read(saves_list, sizeof(PK2SAVE) * count);
		Save_All_Records(); // Change to the current version
	
	} else if (strncmp(versio, "1", 2) == 0) {

		PLog::Write(PLog::INFO, "PK2", "Loading save version 1");

		file->read(count_c, sizeof(count_c));
		count = atoi(count_c);
		if (count > SAVES_COUNT)
			count = SAVES_COUNT;

		PK2SAVE_V1 save_v1;
		int episodi_size = 0;
		int jakso_lapaisty_size = 0;
		int align_size = 0;

		int s = file->size();
		int save_size = (s - 10) / count;
		int char_field = save_size - 8;
		PLog::Write(PLog::INFO, "PK2", "Save char size = %i", char_field);

		switch(char_field) {

			case 384:
				episodi_size = 260;
				jakso_lapaisty_size = 100;
				align_size = 3;
				break;
			
			case 332:
				episodi_size = 260;
				jakso_lapaisty_size = 50;
				align_size = 1;
				break;

			case 380:
				episodi_size = 256;
				jakso_lapaisty_size = 100;
				align_size = 3;
				break;
			
			case 328:
				episodi_size = 256;
				jakso_lapaisty_size = 50;
				align_size = 1;
				break;

			case 252:
				episodi_size = 128;
				jakso_lapaisty_size = 100;
				align_size = 3;
				break;
			
			case 200:
				episodi_size = 128;
				jakso_lapaisty_size = 50;
				align_size = 1;
				break;

			default: 
				PLog::Write(PLog::ERR, "PK2", "Couldn't load save file");
				file->close();
				return 1;

		}

		for (int i = 0; i < count; i++) {

			file->read(&save_v1.jakso, 4);
			file->read(&save_v1.episodi, episodi_size);
			file->read(&save_v1.nimi, 20);
			file->read(&save_v1.kaytossa, 1);
			file->read(&save_v1.jakso_lapaisty, jakso_lapaisty_size);
			file->read(&save_v1._, align_size);
			file->read(&save_v1.pisteet, 4);

			saves_list[i].empty = !save_v1.kaytossa;
			saves_list[i].next_level = save_v1.jakso;
			strncpy(saves_list[i].episode, save_v1.episodi, 128);
			strncpy(saves_list[i].name, save_v1.nimi, 20);
			saves_list[i].score = save_v1.pisteet;
			for (int j = 0; j < 99; j++)
				saves_list[i].level_status[j] = save_v1.jakso_lapaisty[j+1]? LEVEL_PASSED : 0;

		}
		//Save_All_Records(); // Change to the current version

	} else {

		PLog::Write(PLog::ERR, "PK2", "Can't read this save version");
		file->close();
		return 1;

	}

	file->close();
	return 0;

}

int Save_Record(int i) {

	if (!Episode) return -1;

	//clean record
	memset(&saves_list[i], 0, sizeof(PK2SAVE));

	saves_list[i].empty = false;
	strcpy(saves_list[i].episode, Episode->entry.name.c_str()); //assert size 128
	strcpy(saves_list[i].name, Episode->player_name);
	saves_list[i].next_level = Episode->next_level;
	saves_list[i].score = Episode->player_score;

	for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
		saves_list[i].level_status[j] = Episode->level_status[j];

	Save_All_Records();

	return 0;

}
