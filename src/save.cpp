//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "save.hpp"

#include <SDL_rwops.h>

#include <cstring>

#define SAVES_PATH "data/saves.dat"
#define VERSION "1"

PK2SAVE saves_list[MAX_SAVES];

int Empty_Records() {

	for (int i = 0; i < MAX_SAVES; i++){
		saves_list[i].kaytossa = false;
		strcpy(saves_list[i].episodi," ");
		strcpy(saves_list[i].nimi,"empty");
		saves_list[i].jakso = 0;
		saves_list[i].pisteet = 0;
		for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
			saves_list[i].jakso_lapaisty[j] = false;
	}

	return 0;
}

int Save_All_Records() {

	char versio[2] = VERSION;
	char lkm[8];

	itoa(MAX_SAVES, lkm, 10);

	SDL_RWops *file = SDL_RWFromFile(SAVES_PATH, "wb");
	if (file == nullptr) {
		printf("Error saving records\n");
		return 1;
	}
	
	SDL_RWwrite(file, versio, 1, sizeof(versio));
	SDL_RWwrite(file, lkm, 1, sizeof(lkm));
	SDL_RWwrite(file, saves_list, 1, sizeof(saves_list));
	
	SDL_RWclose(file);
	
	return 0;

}

int Load_SaveFile(){
	char versio[2];
	char count_c[8];
	int count, i;

	Empty_Records();

	SDL_RWops *file = SDL_RWFromFile(SAVES_PATH, "rb");
	if (file == nullptr){
		printf("No save file\n");
		return 1;
	}

	SDL_RWread(file, versio, sizeof(versio), 1);

	if (strcmp(versio,"1")==0) {
		SDL_RWread(file, count_c, sizeof(count_c), 1);
		count = atoi(count_c);
		if (count > MAX_SAVES)
			count = MAX_SAVES;
		
		SDL_RWread(file, saves_list, sizeof(PK2SAVE)*count , 1);
	
	}

	SDL_RWclose(file);

	return 0;
}

int Save_Records(int i){
	saves_list[i].kaytossa = true;
	strcpy(saves_list[i].episodi, Episode::name);
	strcpy(saves_list[i].nimi, Episode::player_name);
	saves_list[i].jakso = jakso;
	saves_list[i].pisteet = Episode::score;

	for (int j = 0;j < EPISODI_MAX_LEVELS;j++)
		saves_list[i].jakso_lapaisty[j] = jaksot[j].lapaisty;

	Save_All_Records();

	return 0;
}
