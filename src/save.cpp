#include "save.hpp"

#include "game.hpp"

#include <iostream> //TODO - remove
#include <fstream> //TODO - remove
#include <cstring>

#define SAVES_PATH "data/saves.dat"

PK2SAVE tallennukset[MAX_SAVES];

int Empty_Records(){

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

//PK_Search_Records
int Save_All_Records(char *filename){
	char versio[2] = "1";
	char lkm[8];

	itoa(MAX_SAVES,lkm,10);

	std::ofstream *file = new std::ofstream(filename, ios::binary);
	file->write(versio, sizeof(versio));
	file->write(lkm,    sizeof(lkm));

	for (int i=0;i< MAX_SAVES;i++)
		file->write((char *)&tallennukset[i], sizeof(tallennukset[i]));

	delete file;

	return 0;
}

//Load_Save
int Load_Records(int i){
	if (strcmp(tallennukset[i].episodi," ")!=0) {

		strcpy(episodi,tallennukset[i].episodi);
		strcpy(pelaajan_nimi, tallennukset[i].nimi);
		jakso = tallennukset[i].jakso;
		pisteet = tallennukset[i].pisteet;

		for (int j = 0;j < EPISODI_MAX_LEVELS;j++)
				jaksot[j].lapaisty = tallennukset[i].jakso_lapaisty[j];
		PK_Start_Saves();

		
	}

	return 0;
}

//Load_SaveFile
int Load_Records(char *filename){
	char versio[2];
	char lkmc[8];
	int lkm, i;

	std::ifstream *tiedosto = new std::ifstream(filename, ios::binary);

	Empty_Records();

	if (tiedosto->fail()){
		delete (tiedosto);
		return 1;
	}

	tiedosto->read(versio,	sizeof(versio));

	if (strcmp(versio,"1")==0){
		tiedosto->read(lkmc, sizeof(lkmc));
		lkm = atoi(lkmc);

		for (i=0;i<lkm;i++)
			tiedosto->read ((char *)&tallennukset[i], sizeof (tallennukset[i]));
	}

	delete (tiedosto);

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

	Save_All_Records("data/saves.dat");

	return 0;
}
