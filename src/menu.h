#ifndef PK2_MENU
#define PK2_MENU

enum {//Menu
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
RECT menunelio;

bool PK_DrawMenuOption(char *text, int x, int y);

#endif
