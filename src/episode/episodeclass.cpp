//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "episode/episodeclass.hpp"

#include "episode/mapstore.hpp"
#include "game/mapclass.hpp"
#include "language.hpp"
#include "system.hpp"
#include "save.hpp"

#include "engine/PLog.hpp"
#include "engine/PUtils.hpp"
#include "engine/PFile.hpp"
#include "engine/PDraw.hpp"

#include <cstring>
#include <string>

EpisodeClass* Episode = nullptr;

#define VERSION "1.0"

void EpisodeClass::Clear_Scores() {

	for ( int i = 0; i < EPISODI_MAX_LEVELS; i++ ) {

		this->scores.best_score[i] = 0;
		this->scores.best_time[i] = 0;
		strcpy(this->scores.top_player[i], " ");
		strcpy(this->scores.fastest_player[i], " ");
	
	}

	this->scores.episode_top_score = 0;
	strcpy(this->scores.episode_top_player, " ");
	
}

int EpisodeClass::Open_Scores() {

	char versio[4];

	PFile::Path path(data_path + "scores" PE_SEP + this->entry.name + ".dat");
	
	PFile::RW* file = path.GetRW("rb");
	if (file == nullptr){
		this->Clear_Scores();
		return 1;
	}

	PFile::ReadRW(file, versio, 4);
	if (strncmp(versio, VERSION, 4) != 0) {

		this->Clear_Scores();
		PFile::CloseRW(file);
		return 2;

	}
	
	PFile::ReadRW(file, &this->scores, sizeof(this->scores));
	PFile::CloseRW(file);

	return 0;

}

int EpisodeClass::Save_Scores() {
	
	char versio[4] = VERSION;

	PFile::Path path(data_path + "scores" PE_SEP + this->entry.name + ".dat");

	PFile::RW* file = path.GetRW("wb");
	if (file == nullptr) {

		PLog::Write(PLog::ERR, "PK2", "Can't save scores");
		return 1;

	}

	PFile::WriteRW(file, versio, 4);
	PFile::WriteRW(file, &this->scores, sizeof(this->scores));
	
	PFile::CloseRW(file);

	return 0;

}

//TODO - Load info from different languages
void EpisodeClass::Load_Info() {

	PLang* temp;

	PFile::Path infofile = PFile::Path(this->Get_Dir(), "infosign.txt");
	char otsikko[] = "info00";
	int indeksi1, indeksi2, i;

	temp = new PLang();

	if (infofile.Find()){
		if (temp->Read_File(infofile)){

			for (i = 0 ; i<19 ; i++){
				if(i+1 >= 10) otsikko[4] = '1'; //Make "info" + itos(i)
				otsikko[5] = '1' + (char)(i%10);

				indeksi1 = tekstit->Hae_Indeksi(otsikko);
				indeksi2 = temp->Hae_Indeksi(otsikko);

				if (indeksi1 != -1 && indeksi2 != -1)
					tekstit->Replace_Text(indeksi1,temp->Get_Text(indeksi2));
			}
		}
	}

	delete temp;

}

//TODO - don't load the same image again
void EpisodeClass::Load_Assets() {

	PFile::Path path = this->Get_Dir();

	path.SetFile("pk2stuff.bmp");
	if (FindAsset(&path, "gfx" PE_SEP)) {

		PDraw::image_load(game_assets, path, true);

	} else {

		PLog::Write(PLog::ERR, "PK2", "Can't load map bg");

	}

	path.SetFile("pk2stuff2.png");
	if (FindAsset(&path, "gfx" PE_SEP)) {

		PDraw::image_load(game_assets2, path, true);

	} else {

		PLog::Write(PLog::ERR, "PK2", "Can't load map bg");

	}

}

void EpisodeClass::Load() {
	
	if (entry.is_zip)
		this->source_zip = PFile::OpenZip(data_path + "mapstore" PE_SEP + entry.zipfile);

	PFile::Path path = this->Get_Dir();
	std::vector<std::string> list = path.scandir(".map");
	this->level_count = list.size();

	MapClass *temp = new MapClass();

	for (int i = 0; i < this->level_count; i++) {

		char* mapname = this->levels_list[i].tiedosto;
		strcpy(mapname, list[i].c_str());

		if (temp->Load_Plain_Data(PFile::Path(path, mapname)) == 0) {

			strcpy(this->levels_list[i].nimi, temp->nimi);
			this->levels_list[i].x = temp->x;//   142 + i*35;
			this->levels_list[i].y = temp->y;// 270;
			this->levels_list[i].order = temp->jakso;
			this->levels_list[i].icon = temp->icon;

		}

	}

	delete temp;


	// Order levels
	PK2LEVEL temp2;
	bool stop = false;

	while (!stop){
		stop = true;

		for (int i = 0; i < this->level_count - 1; i++) {

			if (this->levels_list[i].order > this->levels_list[i+1].order) {

				temp2 = this->levels_list[i];
				this->levels_list[i] = this->levels_list[i+1];
				this->levels_list[i+1] = temp2;
				stop = false;

			}

		}
	}
	
	this->Open_Scores();
	this->Load_Info();

}

EpisodeClass::EpisodeClass(int save) {

	//Search the id
	int sz = episodes.size();
	bool set = false;
	for (int i = 0; i < sz; i++) {

		if (episodes[i].name.compare(saves_list[save].episode) == 0) {
			if (set)
				PLog::Write(PLog::WARN, "PK2", "Episode conflict on %s, choosing the first one", saves_list[save].episode);
			else {
				this->entry = episodes[i];
				set = true;
			}
		}

	}
	
	strcpy(this->player_name, saves_list[save].name);
	this->level = saves_list[save].level;
	this->player_score = saves_list[save].score;

	for (int j = 0; j < EPISODI_MAX_LEVELS; j++) {

		this->level_status[j] = saves_list[save].level_status[j];

	}

	this->Load();

}

EpisodeClass::EpisodeClass(const char* player_name, episode_entry entry) {

	this->entry = entry;
	strcpy(this->player_name, player_name);

	for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
		this->level_status[j] = 0;
	
	this->Load();
	
}

EpisodeClass::~EpisodeClass() {

	if (this->entry.is_zip)
		PFile::CloseZip(this->source_zip);

}

PFile::Path EpisodeClass::Get_Dir() {

	std::string path("episodes" PE_SEP);
	path += entry.name + PE_SEP;

	if (this->entry.is_zip)
		return PFile::Path(this->source_zip, path);
	
	return PFile::Path(path);

}
