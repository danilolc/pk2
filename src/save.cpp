#include "save.hpp"

#include "game.hpp"

#include <SDL_rwops.h>

#include <cstring>

#define SAVES_PATH "data/saves.dat"

PK2SAVE tallennukset[MAX_SAVES];

int Empty_Records() {

	for (int i = 0; i < MAX_SAVES; i++){
		tallennukset[i].kaytossa = false;
		strcpy(tallennukset[i].episodi," ");
		strcpy(tallennukset[i].nimi,"empty");
		tallennukset[i].jakso = 0;
		tallennukset[i].pisteet = 0;
		for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
			tallennukset[i].jakso_lapaisty[j] = false;
	}

	return 0;
}

int Save_All_Records() {

	char versio[2] = "1";
	char lkm[8];

	itoa(MAX_SAVES, lkm, 10);

	SDL_RWops *file = SDL_RWFromFile(SAVES_PATH, "wb");
	if (file == nullptr) {
		printf("Error saving records\n");
		return 1;
	}
	
	SDL_RWwrite(file, versio, 1, sizeof(versio));
	SDL_RWwrite(file, lkm, 1, sizeof(lkm));
	SDL_RWwrite(file, tallennukset, 1, sizeof(tallennukset));
	
	SDL_RWclose(file);
	
	return 0;

}

int Load_Save(int i){
	if (strcmp(tallennukset[i].episodi," ")!=0) {

		strcpy(episodi,tallennukset[i].episodi);
		strcpy(pelaajan_nimi, tallennukset[i].nimi);
		jakso = tallennukset[i].jakso;
		pisteet = tallennukset[i].pisteet;

		for (int j = 0;j < EPISODI_MAX_LEVELS;j++)
				jaksot[j].lapaisty = tallennukset[i].jakso_lapaisty[j];
		
		PK_Start_Saves(); //TODO -?
		
	}
		

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
		
		SDL_RWread(file, tallennukset, sizeof(PK2SAVE)*count , 1);
	}

	SDL_RWclose(file);

	return 0;
}

int Save_Records(int i){
	tallennukset[i].kaytossa = true;
	strcpy(tallennukset[i].episodi, episodi);
	strcpy(tallennukset[i].nimi, pelaajan_nimi);
	tallennukset[i].jakso = jakso;
	tallennukset[i].pisteet = pisteet;

	for (int j = 0;j < EPISODI_MAX_LEVELS;j++)
		tallennukset[i].jakso_lapaisty[j] = jaksot[j].lapaisty;

	Save_All_Records();

	return 0;
}
