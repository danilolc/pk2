#ifndef PK2_MAIN
#define PK2_MAIN

enum { //Screen
	TILA_EI_ALUSTETTU,
	TILA_PERUSALUSTUS,
	TILA_INTRO,
	TILA_MENUT,
	TILA_KARTTA,
	TILA_PELI,
	TILA_PISTELASKU,
	TILA_LOPPU
};

int pelin_seuraava_tila = TILA_PERUSALUSTUS;
int pelin_tila = TILA_EI_ALUSTETTU;

void PK_Quit();


#endif
