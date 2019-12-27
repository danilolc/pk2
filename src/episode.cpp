//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "episode.hpp"

#include "game/map.hpp"
#include "language.hpp"
#include "game/game.hpp" //
#include "system.hpp"
#include "save.hpp"

#include "PisteUtils.hpp"

#include <SDL_rwops.h>

#include <cstring>
#include <memory>

int	jakso = 1;
int jaksoja = 1;
int jakso_indeksi_nyt = 1;
PK2LEVEL jaksot[EPISODI_MAX_LEVELS];

char episodi[PE_PATH_SIZE];

DWORD lopetusajastin = 0;
DWORD fake_pisteet = 0;

PK2EPISODESCORES episodipisteet;

bool jakso_uusi_ennatys = false;
bool jakso_uusi_ennatysaika = false;
bool episodi_uusi_ennatys = false;

namespace Episode {
	bool started = false;

	char player_name[20] = " ";

	DWORD score = 0;
}

void Clean_Maps(){
	for (int i=0;i<EPISODI_MAX_LEVELS;i++){
		strcpy(jaksot[i].nimi,"");
		strcpy(jaksot[i].tiedosto,"");
		jaksot[i].x = 0;
		jaksot[i].y = 0;
		jaksot[i].jarjestys = -1;
		jaksot[i].lapaisty = false;
		jaksot[i].ikoni = 0;
	}
}

void EpisodeScore_Start(){
	for (int i=0;i<EPISODI_MAX_LEVELS;i++){
		episodipisteet.best_score[i] = 0;
		episodipisteet.best_time[i] = 0;
		strcpy(episodipisteet.top_player[i]," ");
		strcpy(episodipisteet.fastest_player[i]," ");
	}

	episodipisteet.episode_top_score = 0;
	strcpy(episodipisteet.episode_top_player," ");
}
int EpisodeScore_Compare(int jakso, DWORD episteet, DWORD aika, bool loppupisteet){
	int paluu = 0;
	if (!loppupisteet) {
		if (episteet > episodipisteet.best_score[jakso]) {
			strcpy(episodipisteet.top_player[jakso],Episode::player_name);
			episodipisteet.best_score[jakso] = episteet;
			jakso_uusi_ennatys = true;
			paluu++;
		}
		if ((aika < episodipisteet.best_time[jakso] || episodipisteet.best_time[jakso] == 0) && Game::map->aika > 0) {
			strcpy(episodipisteet.fastest_player[jakso],Episode::player_name);
			episodipisteet.best_time[jakso] = aika;
			jakso_uusi_ennatysaika = true;
			paluu++;
		}
	}
	else {
		if (episteet > episodipisteet.episode_top_score) {
		    episodipisteet.episode_top_score = episteet;
			strcpy(episodipisteet.episode_top_player,Episode::player_name);
			episodi_uusi_ennatys = true;
			paluu++;
		}
	}
	return paluu;
}
int EpisodeScore_Open(char *filename) {

	char versio[4];

	Episode::Get_Dir(filename);
	
	SDL_RWops *file = SDL_RWFromFile(filename, "r");
	if (file == nullptr){
		EpisodeScore_Start();
		return 1;
	}

	SDL_RWread(file, versio, 4, 1);
	if (strcmp(versio, "1.0") != 0) {
		EpisodeScore_Start();
		SDL_RWclose(file);
		return 2;
	}
	
	SDL_RWread(file, &episodipisteet, sizeof(episodipisteet), 1);
	SDL_RWclose(file);

	return 0;

}
int EpisodeScore_Save(char *filename) {

	Episode::Get_Dir(filename);

	SDL_RWops *file = SDL_RWFromFile(filename, "w");
	if (file == nullptr) {
		printf("Error saving scores\n");
		return 1;
	}

	SDL_RWwrite(file, "1.0", 1, 4);
	SDL_RWwrite(file, &episodipisteet, 1, sizeof(episodipisteet));
	
	SDL_RWclose(file);

	return 0;

}

//TODO - Load info from different languages
void Load_InfoText() { 
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
	jaksoja = PisteUtils_Scandir(".map", hakemisto, list, EPISODI_MAX_LEVELS);

	PK2Kartta *temp = new PK2Kartta();

	for (i = 0; i <= jaksoja; i++){
		strcpy(jaksot[i].tiedosto, list[i]);
		if (temp->Lataa_Pelkat_Tiedot(hakemisto,jaksot[i].tiedosto) == 0){
			strcpy(jaksot[i].nimi, temp->nimi);
			jaksot[i].x = temp->x;//   142 + i*35;
			jaksot[i].y = temp->y;// 270;
			jaksot[i].jarjestys = temp->jakso;
			jaksot[i].ikoni = temp->ikoni;
		}
	}

	delete temp;

	for (; i < EPISODI_MAX_LEVELS; i++){
		strcpy(jaksot[i].nimi,"");
		strcpy(jaksot[i].tiedosto,"");
		jaksot[i].x = 0;
		jaksot[i].y = 0;
		jaksot[i].jarjestys = -1;
		jaksot[i].ikoni = 0;
	}

	PK2LEVEL jakso;

	bool lopeta = false;

	while (!lopeta){
		lopeta = true;

		for (i=0;i<jaksoja;i++){
			if (jaksot[i].jarjestys > jaksot[i+1].jarjestys){
				jakso = jaksot[i];
				jaksot[i] = jaksot[i+1];
				jaksot[i+1] = jakso;
				lopeta = false;
			}
		}
	}
	
	char topscoretiedosto[PE_PATH_SIZE] = "scores.dat";
	EpisodeScore_Open(topscoretiedosto);

	Load_InfoText();
	
	Episode::started = true;
}

void Episode::Load_Save(int save) {

	strcpy(episodi,tallennukset[save].episodi);
	strcpy(Episode::player_name, tallennukset[save].nimi);
	jakso = tallennukset[save].jakso;
	Episode::score = tallennukset[save].pisteet;

	for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
			jaksot[j].lapaisty = tallennukset[save].jakso_lapaisty[j];

	Episode::Load();

}
void Episode::Load_New(const char* player_name, const char* episode) {

	strcpy(episodi, episode);
	strcpy(Episode::player_name, player_name);
	jakso = 1;
	Episode::score = 0;

	for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
		jaksot[j].lapaisty = 0;
	
	Episode::Load();
	
}

void Episode::Get_Dir(char *tiedosto){
	char uusi_tiedosto[255];

	strcpy(uusi_tiedosto, game_path);
	strcat(uusi_tiedosto, "/episodes/");
	strcat(uusi_tiedosto, episodi);
	strcat(uusi_tiedosto, "/");
	strcat(uusi_tiedosto, tiedosto);
	strcpy(tiedosto, uusi_tiedosto);
}
