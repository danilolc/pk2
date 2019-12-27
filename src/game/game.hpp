//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "PisteLanguage.hpp"
#include "types.hpp"
#include "platform.hpp"

#include "game/map.hpp"
#include "game/sprites.hpp"
#include "settings.hpp"

extern int kytkin1; //TODO - on inGame.cpp
extern int kytkin2;
extern int kytkin3;
extern int kytkin_tarina;

extern int kuva_peli;
extern int kuva_peli2;
extern int kuva_tausta;

const int TIME_FPS = 100;
extern DWORD timeout;
extern int increase_time;
extern int sekunti;
extern bool aikaraja;

extern int info_timer;
extern char info[80];

extern float fps;
extern bool show_fps;

extern int lataa_peli;

extern int nakymattomyys;

extern DWORD pisteet;
extern DWORD piste_lisays;
extern char pelaajan_nimi[20];

const BYTE BLOCK_MAX_MASKEJA = 150;

enum BLOCKS{
	BLOCK_TAUSTA,          //BLOCK_BACKGROUND
	BLOCK_SEINA,           //BLOCK_WALL
	BLOCK_MAKI_OIKEA_YLOS, //BLOCK_MAX
	BLOCK_MAKI_VASEN_YLOS, //BLOCK_MAX_
	BLOCK_MAKI_OIKEA_ALAS, //BLOCK_MAX_
	BLOCK_MAKI_VASEN_ALAS, //BLOCK_MAX_ 
	BLOCK_MAKI_YLOS,       //BLOCK_MAX_UP
	BLOCK_MAKI_ALAS        //BLOCK_MAX_DOWN
};

struct PK2BLOCK{
	BYTE	koodi;
	bool	tausta;
	BYTE	vasemmalle, oikealle, ylos, alas;
	int 	vasen, oikea, yla, ala;
	bool	vesi;
	bool	reuna;
};

struct PK2BLOCKMASKI{
	short int	ylos[32];
	short int	alas[32];
	short int	vasemmalle[32];
	short int	oikealle[32];
};

extern PK2BLOCK palikat[300];
extern PK2BLOCK	lasketut_palikat[150];//150
extern PK2BLOCKMASKI palikkamaskit[BLOCK_MAX_MASKEJA];

//In game variables
namespace Game {
	
	extern PK2Kartta* map;
	extern char map_path[PE_PATH_SIZE];

	extern int vibration;

	extern int camera_x;
	extern int camera_y;
	extern double dcamera_x;
	extern double dcamera_y;
	extern double dcamera_a;
	extern double dcamera_b;

	extern bool paused;

	extern int keys;
}


/**/
void PK_New_Game();
void PK_Draw_Cursor(int x, int y);

int PK_Sprite_Bonus_Movement(int i);
int PK_Sprite_Movement(int i);

bool Draw_Menu_Text(bool active, char *teksti, int x, int y);
int PK_MenuShadow_Create(int kbuffer, DWORD kleveys, int kkorkeus, int startx);
/**/




bool PK_Check_File(char *filename);

void PK_Start_Info(char *text);