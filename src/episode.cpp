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

namespace Episode {
	bool started = false;

	char name[PE_PATH_SIZE] = " ";
	
	char player_name[20] = " ";
	DWORD player_score = 0;

	int level = 1;
	int level_count = 0;
	PK2LEVEL levels_list[EPISODI_MAX_LEVELS];
	PK2EPISODESCORES scores;
	
}

void Episode::Clear_Scores(){
	for (int i=0;i<EPISODI_MAX_LEVELS;i++){
		Episode::scores.best_score[i] = 0;
		Episode::scores.best_time[i] = 0;
		strcpy(Episode::scores.top_player[i]," ");
		strcpy(Episode::scores.fastest_player[i]," ");
	}

	Episode::scores.episode_top_score = 0;
	strcpy(Episode::scores.episode_top_player," ");
}

int Episode::Open_Scores(char *filename) {

	char versio[4];

	Episode::Get_Dir(filename);
	
	SDL_RWops *file = SDL_RWFromFile(filename, "r");
	if (file == nullptr){
		Episode::Clear_Scores();
		return 1;
	}

	SDL_RWread(file, versio, 4, 1);
	if (strcmp(versio, "1.0") != 0) {
		Episode::Clear_Scores();
		SDL_RWclose(file);
		return 2;
	}
	
	SDL_RWread(file, &Episode::scores, sizeof(Episode::scores), 1);
	SDL_RWclose(file);

	return 0;

}

int Episode::Save_Scores(char *filename) {

	Episode::Get_Dir(filename);

	SDL_RWops *file = SDL_RWFromFile(filename, "w");
	if (file == nullptr) {
		printf("Error saving scores\n");
		return 1;
	}

	SDL_RWwrite(file, "1.0", 1, 4);
	SDL_RWwrite(file, &Episode::scores, 1, sizeof(Episode::scores));
	
	SDL_RWclose(file);

	return 0;

}



//TODO - Load info from different languages
void Episode::Load_Info() { 
	PisteLanguage* temp;
	char infofile[PE_PATH_SIZE] = "infosign.txt";
	char otsikko[] = "info00";
	int indeksi1, indeksi2, i;

	temp = new PisteLanguage();
	Episode::Get_Dir(infofile);

	if (PK_Check_File(infofile)){
		if (temp->Read_File(infofile)){

			for (i = 0 ; i<19 ; i++){
				if(i+1 >= 10) otsikko[4] = '1'; //Make "info" + itos(i)
				otsikko[5] = '1' + (char)(i%10);

				indeksi1 = tekstit->Hae_Indeksi(otsikko);
				indeksi2 = temp->Hae_Indeksi(otsikko);

				if (indeksi1 != -1 && indeksi2 != -1)
					tekstit->Korvaa_Teksti(indeksi1,temp->Hae_Teksti(indeksi2));
			}
		}
	}

	delete (temp);
}

void Episode::Load() {
	
	int i = 0;
	char hakemisto[PE_PATH_SIZE];
	char list[EPISODI_MAX_LEVELS][PE_PATH_SIZE];
	
	memset(list, '\0', sizeof(list));

	strcpy(hakemisto,"");
	Episode::Get_Dir(hakemisto);
	Episode::level_count = PisteUtils_Scandir(".map", hakemisto, list, EPISODI_MAX_LEVELS);

	PK2Kartta *temp = new PK2Kartta();

	for (i = 0; i <= Episode::level_count; i++){
		strcpy(Episode::levels_list[i].tiedosto, list[i]);
		if (temp->Lataa_Pelkat_Tiedot(hakemisto,Episode::levels_list[i].tiedosto) == 0){
			strcpy(Episode::levels_list[i].nimi, temp->nimi);
			Episode::levels_list[i].x = temp->x;//   142 + i*35;
			Episode::levels_list[i].y = temp->y;// 270;
			Episode::levels_list[i].jarjestys = temp->jakso;
			Episode::levels_list[i].ikoni = temp->ikoni;
		}
	}

	delete temp;

	for (; i < EPISODI_MAX_LEVELS; i++){
		strcpy(Episode::levels_list[i].nimi,"");
		strcpy(Episode::levels_list[i].tiedosto,"");
		Episode::levels_list[i].x = 0;
		Episode::levels_list[i].y = 0;
		Episode::levels_list[i].jarjestys = -1;
		Episode::levels_list[i].ikoni = 0;
	}

	PK2LEVEL jakso;

	bool lopeta = false;

	while (!lopeta){
		lopeta = true;

		for (i=0;i<Episode::level_count;i++){
			if (Episode::levels_list[i].jarjestys > Episode::levels_list[i+1].jarjestys){
				jakso = Episode::levels_list[i];
				Episode::levels_list[i] = Episode::levels_list[i+1];
				Episode::levels_list[i+1] = jakso;
				lopeta = false;
			}
		}
	}
	
	char topscoretiedosto[PE_PATH_SIZE] = "scores.dat";
	Episode::Open_Scores(topscoretiedosto);

	Episode::Load_Info();
	
	Episode::started = true;
}

void Episode::Load_Save(int save) {

	strcpy(Episode::name,saves_list[save].episodi);
	strcpy(Episode::player_name, saves_list[save].nimi);
	Episode::level = saves_list[save].jakso;
	Episode::player_score = saves_list[save].pisteet;

	for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
			Episode::levels_list[j].lapaisty = saves_list[save].jakso_lapaisty[j];

	Episode::Load();

}
void Episode::Load_New(const char* player_name, const char* episode) {

	strcpy(Episode::name, episode);
	strcpy(Episode::player_name, player_name);
	Episode::level = 1;
	Episode::player_score = 0;

	for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
		Episode::levels_list[j].lapaisty = 0;
	
	Episode::Load();
	
}

void Episode::Get_Dir(char *tiedosto){
	char uusi_tiedosto[255];

	strcpy(uusi_tiedosto, game_path);
	strcat(uusi_tiedosto, "/episodes/");
	strcat(uusi_tiedosto, Episode::name);
	strcat(uusi_tiedosto, "/");
	strcat(uusi_tiedosto, tiedosto);
	strcpy(tiedosto, uusi_tiedosto);
}
