#include "game.hpp"

#include "map.hpp"
#include <cmath>

int screen_width  = 800;
int screen_height = 480;

char tyohakemisto[PE_PATH_SIZE] = ".";

int mouse_x = 10;
int mouse_y = 10;
int key_delay = 0;

double cos_table[360];
double sin_table[360];

int degree = 0;
int degree_temp = 0;

//Screen Buffers
int  kuva_peli  = -1;
int  kuva_peli2 = -1;
int  kuva_tausta = -1;

DWORD pisteet = 0;
DWORD piste_lisays = 0;
char pelaajan_nimi[20] = " ";

bool PK2_error = false;
const char* PK2_error_msg = NULL;

int PK2_Error(const char* msg) {
	PK2_error = true;
	PK2_error_msg = msg;
}

void Precalculate_SinCos(){
	int i;
	for (i=0; i<360; i++) cos_table[i] = cos(M_PI*2* (i%360)/180)*33;
	for (i=0; i<360; i++) sin_table[i] = sin(M_PI*2* (i%360)/180)*33;
}

namespace Game {

	PK2Kartta* current_map;
	char map_path[PE_PATH_SIZE];

	int vibration;

	PK2SETTINGS settings;
	
	int camera_x;
	int camera_y;
	double dcamera_x;
	double dcamera_y;
	double dcamera_a;
	double dcamera_b;

	bool paused = false;

	int keys = 0;
}

