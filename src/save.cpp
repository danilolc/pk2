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
#define VERSION "4"

PK2SAVE saves_list[SAVES_COUNT];

int Empty_Records() {

	//memset(saves_list, 0, sizeof(saves_list));

	for (int i = 0; i < SAVES_COUNT; i++) {
		saves_list[i].empty = true;
		saves_list[i].episode = "";
		saves_list[i].level = 0;
		saves_list[i].level_count = 0;
		memset(saves_list[i].level_status, 0, sizeof(saves_list[i].level_status));
		memset(saves_list[i].name, 0, sizeof(saves_list[i].name));
		saves_list[i].score = 0;
	}

	return 0;
	
}

int Save_All_Records() {

	char versio[2] = VERSION;
	u32 count = SAVES_COUNT;
	u32 unused_data = 0;

	PFile::Path path(data_path, SAVES_FILE);
	
	PFile::RW* file = path.GetRW("w");
	if (file == nullptr) {

		PLog::Write(PLog::ERR, "PK2", "Can't save records");
		return 1;

	}

	file->write(versio, sizeof(versio)); // Write version ("2")
	file->write(unused_data);             // Write unused data (0)
	file->write(count);                  // Write count (11)

	// Write saves
	for (uint i = 0; i < count; i++) {

		file->write(saves_list[i].empty);
		file->write(saves_list[i].level);
		file->write(saves_list[i].episode);
		file->write(saves_list[i].name, 20);
		file->write(saves_list[i].score);
		file->write(saves_list[i].level_count);
		file->write(saves_list[i].level_status, saves_list[i].level_count);

	}
	
	file->close();
	
	return 0;

}

int Load_SaveFile() {

	char versio[2];
	char count_c[8];
	u32  count;
	u32  unused_data;

	Empty_Records();

	PFile::Path path(data_path, SAVES_FILE);

	PFile::RW* file = path.GetRW("r");
	if (file == nullptr){

		PLog::Write(PLog::INFO, "PK2", "No save file");
		Save_All_Records();
		return 1;
	
	}

	file->read(versio, sizeof(versio));

	// Version 4 has unused_data and variable quantity of levels and names size
	if (strncmp(versio, "4", 2) == 0) {

		PLog::Write(PLog::INFO, "PK2", "Loading save version 4");

		file->read(unused_data);
		file->read(count);

		for (u32 i = 0; i < count; i++) {
		
			file->read(saves_list[i].empty);
			file->read(saves_list[i].level);
			file->read(saves_list[i].episode);
			file->read(saves_list[i].name, 20);
			file->read(saves_list[i].score);
			file->read(saves_list[i].level_count);
			file->read(saves_list[i].level_status, saves_list[i].level_count);

		}
	
	// Version 3 is always little endian
	} if (strncmp(versio, "3", 2) == 0) {

		char episode[PE_PATH_SIZE];

		PLog::Write(PLog::INFO, "PK2", "Loading save version 3");

		file->read(count_c, sizeof(count_c));
		count = atoi(count_c);
		if (count > SAVES_COUNT)
			count = SAVES_COUNT;

		for (u32 i = 0; i < count; i++) {
		
			file->read(saves_list[i].empty);
			file->read(saves_list[i].level);
			
			file->read(episode, PE_PATH_SIZE);
			episode[PE_PATH_SIZE - 1] = '\0';
			saves_list[i].episode = episode;

			file->read(saves_list[i].name, 20);
			file->read(saves_list[i].score);
			file->read(saves_list[i].level_status, EPISODI_MAX_LEVELS);

		}
	
	} else if (strncmp(versio, "2", 2) == 0) {
		//TODO AAAAAAAAAA
		char episode[PE_PATH_SIZE];

		PLog::Write(PLog::INFO, "PK2", "Loading save version 2");

		file->read(count_c, sizeof(count_c));
		count = atoi(count_c);
		if (count > SAVES_COUNT)
			count = SAVES_COUNT;

		file->read(saves_list, sizeof(PK2SAVE) * count);
		Save_All_Records(); // Change to the current version
	
	} else if (strncmp(versio, "1", 2) == 0) {

		PLog::Write(PLog::INFO, "PK2", "Can't read saves version 1");
		file->close();
		return 1;

		/*struct PK2SAVE_V1{
			s32   jakso;
			char  episodi[260]; //260, 256, 128?
			char  nimi[20];
			bool  kaytossa;
			bool  jakso_lapaisty[100]; //100, 50?
			u32   pisteet;
		};*/

	} else {

		PLog::Write(PLog::INFO, "PK2", "Can't read this save version");
		file->close();
		return 1;

	}

	file->close();
	return 0;

}

int Save_Record(int i) {

	//clean record
	//memset(&saves_list[i], 0, sizeof(PK2SAVE));

	saves_list[i].empty = false;
	saves_list[i].episode = Episode->entry.name;
	strcpy(saves_list[i].name, Episode->player_name);
	saves_list[i].level = Episode->level;
	saves_list[i].score = Episode->player_score;

	for (uint j = 0; j < Episode->level_count; j++)
		saves_list[i].level_status[j] = Episode->levels_list[j].status;

	Save_All_Records();

	return 0;

}
