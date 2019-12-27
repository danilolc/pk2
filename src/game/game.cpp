//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "game/game.hpp"

//Screen Buffers
int  kuva_peli  = -1;
int  kuva_peli2 = -1;
int  kuva_tausta = -1;

DWORD pisteet = 0;
DWORD piste_lisays = 0;
char pelaajan_nimi[20] = " ";

namespace Game {

	PK2Kartta* map;
	char map_path[PE_PATH_SIZE];

	int vibration;

	int camera_x;
	int camera_y;
	double dcamera_x;
	double dcamera_y;
	double dcamera_a;
	double dcamera_b;

	bool paused = false;

	int keys = 0;
}

