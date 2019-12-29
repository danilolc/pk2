//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "episode.hpp"

#include "game/map.hpp"
#include "language.hpp"
#include "system.hpp"
#include "save.hpp"

#include "PisteUtils.hpp"

#include <SDL_rwops.h>

#include <cstring>

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

int EpisodeClass::Open_Scores(const char *filename) {

	char versio[4];

	char path[PE_PATH_SIZE];

	strcpy(path, filename);
	this->Get_Dir(path);
	
	SDL_RWops *file = SDL_RWFromFile(path, "r");
	if (file == nullptr){
		this->Clear_Scores();
		return 1;
	}

	SDL_RWread(file, versio, 4, 1);
	if (strcmp(versio, "1.0") != 0) {
		this->Clear_Scores();
		SDL_RWclose(file);
		return 2;
	}
	
	SDL_RWread(file, &this->scores, sizeof(this->scores), 1);
	SDL_RWclose(file);

	return 0;

}

int EpisodeClass::Save_Scores(const char *filename) {
	
	char path[PE_PATH_SIZE];
	strcpy(path, filename);

	this->Get_Dir(path);

	SDL_RWops *file = SDL_RWFromFile(path, "w");
	if (file == nullptr) {
		printf("Error saving scores\n");
		return 1;
	}

	SDL_RWwrite(file, "1.0", 1, 4);
	SDL_RWwrite(file, &this->scores, 1, sizeof(this->scores));
	
	SDL_RWclose(file);

	return 0;

}

//TODO - Load info from different languages
void EpisodeClass::Load_Info() {

	PisteLanguage* temp;

	char infofile[PE_PATH_SIZE] = "infosign.txt";
	char otsikko[] = "info00";
	int indeksi1, indeksi2, i;

	temp = new PisteLanguage();
	this->Get_Dir(infofile);

	if (PK_Check_File(infofile)){
		if (temp->Read_File(infofile)){

			for (i = 0 ; i<19 ; i++){
				if(i+1 >= 10) otsikko[4] = '1'; //Make "info" + itos(i)
				otsikko[5] = '1' + (char)(i%10);

				indeksi1 = tekstit->Hae_Indeksi(otsikko);
				indeksi2 = temp->Hae_Indeksi(otsikko);

				if (indeksi1 != -1 && indeksi2 != -1)
					tekstit->Replace_Text(indeksi1,temp->Hae_Teksti(indeksi2));
			}
		}
	}

	delete temp;

}

void EpisodeClass::Load() {
	
	int i = 0;
	char path[PE_PATH_SIZE];
	char list[EPISODI_MAX_LEVELS][PE_PATH_SIZE];
	
	memset(list, '\0', sizeof(list));

	strcpy(path,"");
	this->Get_Dir(path);
	this->level_count = PisteUtils_Scandir(".map", path, list, EPISODI_MAX_LEVELS);

	PK2Kartta *temp = new PK2Kartta();

	for (i = 0; i <= this->level_count; i++){
		strcpy(this->levels_list[i].tiedosto, list[i]);
		if (temp->Lataa_Pelkat_Tiedot(path,this->levels_list[i].tiedosto) == 0){
			strcpy(this->levels_list[i].nimi, temp->nimi);
			this->levels_list[i].x = temp->x;//   142 + i*35;
			this->levels_list[i].y = temp->y;// 270;
			this->levels_list[i].jarjestys = temp->jakso;
			this->levels_list[i].ikoni = temp->ikoni;
		}
	}

	delete temp;

	for (; i < EPISODI_MAX_LEVELS; i++){
		strcpy(this->levels_list[i].nimi,"");
		strcpy(this->levels_list[i].tiedosto,"");
		this->levels_list[i].x = 0;
		this->levels_list[i].y = 0;
		this->levels_list[i].jarjestys = -1;
		this->levels_list[i].ikoni = 0;
	}

	PK2LEVEL temp2;

	bool stop = false;

	while (!stop){
		stop = true;

		for (i=0;i<this->level_count;i++){
			if (this->levels_list[i].jarjestys > this->levels_list[i+1].jarjestys){
				temp2 = this->levels_list[i];
				this->levels_list[i] = this->levels_list[i+1];
				this->levels_list[i+1] = temp2;
				stop = false;
			}
		}
	}
	
	this->Open_Scores("scores.dat");
	this->Load_Info();

}

EpisodeClass::EpisodeClass(int save) {

	strcpy(this->name,saves_list[save].episodi);
	strcpy(this->player_name, saves_list[save].nimi);
	this->level = saves_list[save].jakso;
	this->player_score = saves_list[save].pisteet;

	for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
		this->levels_list[j].lapaisty = saves_list[save].jakso_lapaisty[j];

	this->Load();

}

EpisodeClass::EpisodeClass(const char* player_name, const char* episode) {

	strcpy(this->name, episode);
	strcpy(this->player_name, player_name);

	for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
		this->levels_list[j].lapaisty = 0;
	
	this->Load();
	
}

EpisodeClass::~EpisodeClass() {



}

void EpisodeClass::Get_Dir(char *tiedosto){
	char uusi_tiedosto[255];

	strcpy(uusi_tiedosto, game_path);
	strcat(uusi_tiedosto, "/episodes/");
	strcat(uusi_tiedosto, EpisodeClass::name);
	strcat(uusi_tiedosto, "/");
	strcat(uusi_tiedosto, tiedosto);
	strcpy(tiedosto, uusi_tiedosto);
}
