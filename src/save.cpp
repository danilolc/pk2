//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "save.hpp"

#include "episode/mapstore.hpp"
#include "system.hpp"

#include "engine/PLog.hpp"

#include <cstring>
#include <string>

#define SAVES_FILE "saves.dat"
#define VERSION "2"

PK2SAVE saves_list[MAX_SAVES];

int Empty_Records() {

	for (int i = 0; i < MAX_SAVES; i++) {

		saves_list[i].empty = true;
		strcpy(saves_list[i].episode," ");
		strcpy(saves_list[i].name,"empty");
		saves_list[i].level = 0;
		saves_list[i].score = 0;
		for (int j = 0; j < EPISODI_MAX_LEVELS; j++) {

			saves_list[i].level_cleared[j] = false;
			saves_list[i].all_apples[j] = false;

		}
		
	}

	return 0;
	
}

int Save_All_Records() {

	char versio[2] = VERSION;
	char count_c[8];

	itoa(MAX_SAVES, count_c, 10);

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
		if (count > MAX_SAVES)
			count = MAX_SAVES;

		PFile::ReadRW(file, saves_list, sizeof(PK2SAVE) * count);
	
	} else if (strncmp(versio,"1", 2) == 0) {

		PFile::ReadRW(file, count_c, sizeof(count_c));
		count = atoi(count_c);
		if (count > MAX_SAVES)
			count = MAX_SAVES;

		PK2SAVE_V1 temp[MAX_SAVES];

		PFile::ReadRW(file, temp, sizeof(PK2SAVE_V1) * count);

		for (int i = 0; i < count; i++) {
			
			saves_list[i].empty = !temp[i].kaytossa;
			saves_list[i].level = temp[i].jakso;
			strcpy(saves_list[i].episode, temp[i].episodi);
			strcpy(saves_list[i].name, temp[i].nimi);
			saves_list[i].score = temp[i].pisteet;

			for (int j = 0; j < EPISODI_MAX_LEVELS;j++) {
				saves_list[i].level_cleared[j] = temp[i].jakso_cleared[j];
				saves_list[i].all_apples[j] = false;
			}

		}
	
	}

	PFile::CloseRW(file);

	return 0;

}

int Save_Records(int i) {

	saves_list[i].empty = false;
	strcpy(saves_list[i].episode, Episode->entry.name.c_str());
	strcpy(saves_list[i].name, Episode->player_name);
	saves_list[i].level = Episode->level;
	saves_list[i].score = Episode->player_score;

	for (int j = 0;j < EPISODI_MAX_LEVELS;j++) {

		saves_list[i].level_cleared[j] = Episode->levels_list[j].cleared;
		saves_list[i].all_apples[j] = Episode->levels_list[j].all_apples;

	}

	Save_All_Records();

	return 0;

}
