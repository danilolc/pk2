//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
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

#include <SDL_rwops.h>

#include <cstring>
#include <string>

EpisodeClass* Episode = nullptr;

void EpisodeClass::Clear_Scores() {

	for (int i=0;i<EPISODI_MAX_LEVELS;i++){
		this->scores.best_score[i] = 0;
		this->scores.best_time[i] = 0;
		strcpy(this->scores.top_player[i]," ");
		strcpy(this->scores.fastest_player[i]," ");
	}

	this->scores.episode_top_score = 0;
	strcpy(this->scores.episode_top_player," ");
	
}

int EpisodeClass::Open_Scores() {

	char versio[4];

	std::string path(data_path);
	path += "scores" PE_SEP;
	path += this->entry.name;
	path += ".dat";
	
	SDL_RWops *file = SDL_RWFromFile(path.c_str(), "rb");
	if (file == nullptr){
		this->Clear_Scores();
		return 1;
	}

	SDL_RWread(file, versio, 4, 1);
	if (strncmp(versio, "1.0", 4) != 0) {
		this->Clear_Scores();
		SDL_RWclose(file);
		return 2;
	}
	
	SDL_RWread(file, &this->scores, sizeof(this->scores), 1);
	SDL_RWclose(file);

	return 0;

}

int EpisodeClass::Save_Scores() {
	
	std::string path(data_path);
	path += "scores" PE_SEP;
	path += this->entry.name;
	path += ".dat";

	SDL_RWops *file = SDL_RWFromFile(path.c_str(), "wb");
	if (file == nullptr) {

		PLog::Write(PLog::ERROR, "PK2", "Can't saving scores");
		return 1;

	}

	SDL_RWwrite(file, "1.0", 1, 4);
	SDL_RWwrite(file, &this->scores, 1, sizeof(this->scores));
	
	SDL_RWclose(file);

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

		this->levels_list[j].cleared = saves_list[save].level_cleared[j];
		this->levels_list[j].all_apples = saves_list[save].all_apples[j];

	}

	this->Load();

}

EpisodeClass::EpisodeClass(const char* player_name, episode_entry entry) {

	this->entry = entry;
	strcpy(this->player_name, player_name);

	for (int j = 0; j < EPISODI_MAX_LEVELS; j++) {
		this->levels_list[j].cleared = 0;
		this->levels_list[j].all_apples = false;
	}
	
	this->Load();
	
}

EpisodeClass::~EpisodeClass() {

	PFile::CloseZip(this->source_zip);

}

PFile::Path EpisodeClass::Get_Dir() {

	std::string path("episodes" PE_SEP);
	path += entry.name + PE_SEP;

	if (this->entry.is_zip)
		return PFile::Path(this->source_zip, path);
	
	return PFile::Path(path);

}
