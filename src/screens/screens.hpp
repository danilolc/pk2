//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

enum SCREEN{
	SCREEN_NOT_SET,
	SCREEN_FIRST_START,
	SCREEN_INTRO,
	SCREEN_MENU,
	SCREEN_MAP,
	SCREEN_GAME,
	SCREEN_SCORING,
	SCREEN_END
};

extern int current_screen;
extern int next_screen;

void Fade_Quit();

int Screen_Intro_Init();
int Screen_Menu_Init();
int Screen_Map_Init();
int Screen_InGame_Init();
int Screen_ScoreCount_Init();
int Screen_Ending_Init();

int Screen_Intro();
int Screen_Menu();
int Screen_Map();
int Screen_InGame();
int Screen_ScoreCount();
int Screen_Ending();

int Screen_First_Start();
int Screen_Loop();