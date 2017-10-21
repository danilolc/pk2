#ifndef PK2_MENU
#define PK2_MENU

enum {
	MENU_PAAVALIKKO,
	MENU_EPISODIT,
	MENU_KONTROLLIT,
	MENU_GRAFIIKKA,
	MENU_AANET,
	MENU_NIMI,
	MENU_LATAA,
	MENU_TALLENNA,
	MENU_LANGUAGE
};

int menu_nyt = MENU_PAAVALIKKO;

bool PK_DrawMenuOption(char *text, int x, int y);

int PK_Main_Menut();
int PK_Start_Menu();

#endif
