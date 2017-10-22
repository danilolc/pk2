#ifndef PK2_MAIN
#define PK2_MAIN

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

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

bool test_level = false;
bool dev_mode = false;

int RUUDUN_LEVEYS  = SCREEN_WIDTH;
int RUUDUN_KORKEUS = SCREEN_HEIGHT;

int KARTANPIIRTO_LEVEYS     = SCREEN_WIDTH;
int KARTANPIIRTO_KORKEUS    = SCREEN_HEIGHT;
bool RAJAA_KARTANPIIRTOALUE = true;

bool PK2_virhe = false;
bool lopeta_peli = false;

bool running = true;
bool unload = false;
bool taulut_laskettu = false;

char	viesti[60];

void PK_Quit();

#endif
