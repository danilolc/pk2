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

const int TIME_FPS = 100;
const int INFO_TIME = 700;

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

class GameClass {

	public:

		int level_id;
		PK2Kartta* map;
		char map_path[PE_PATH_SIZE];

		PK2BLOCK      palikat[300];
		PK2BLOCK      lasketut_palikat[150];//150
		PK2BLOCKMASKI palikkamaskit[BLOCK_MAX_MASKEJA];

		bool game_over = false;
		bool level_clear = false;
		bool repeating = false;

		DWORD exit_timer;

		DWORD timeout = 0;
		int increase_time = 0;
		int seconds = 0;
		bool has_time = false;

		int button_moving = 0;
		int button1 = 0;
		int button2 = 0;
		int button3 = 0;

		DWORD score = 0;
		DWORD score_increment = 0;

		int vibration;

		int invisibility = 0;

		int camera_x;
		int camera_y;
		double dcamera_x;
		double dcamera_y;
		double dcamera_a;
		double dcamera_b;

		bool paused = false;

		int keys = 0;

		int info_timer = 0;
		char info[80] = " ";

		int item_pannel_x = -215;

		GameClass(int idx);
		GameClass(const char* map);
		~GameClass();

		int Start();

		int Move_Blocks();
		void Show_Info(const char *text);

		bool isStarted();

	private:
		
		bool started = false;
		bool from_index;
		
		int Calculete_TileMasks();
		int Clean_TileBuffer();

		int Calculate_Tiles();
		int Open_Map();

};

extern GameClass* Game;

/**/
int PK_Sprite_Bonus_Movement(int i);
int PK_Sprite_Movement(int i);

bool Draw_Menu_Text(bool active, char *teksti, int x, int y);
int PK_MenuShadow_Create(int kbuffer, DWORD kleveys, int kkorkeus, int startx);

bool PK_Check_File(const char *filename);
/**/