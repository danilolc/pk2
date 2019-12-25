#pragma once

#include "PisteLanguage.hpp"

#include "map.hpp"
#include "sprite.hpp"

#include "types.hpp"
#include "platform.hpp"

#include "settings.hpp"

#include <vector>

extern int kytkin1; //TODO - on inGame.cpp
extern int kytkin2;
extern int kytkin3;

extern int screen_width;
extern int screen_height;

extern char tyohakemisto[PE_PATH_SIZE];

extern int hiiri_x;
extern int hiiri_y;
extern int key_delay;

extern int kuva_peli;
extern int kuva_peli2;
extern int kuva_tausta;

extern double cos_table[360];
extern double sin_table[360];
extern int degree;
extern int degree_temp;

extern PK2Kartta *kartta;

extern bool PK2_error;
extern const char* PK2_error_msg;

int PK2_Error(const char* msg);
void Precalculate_SinCos();

extern DWORD pisteet;
extern DWORD piste_lisays;
extern char pelaajan_nimi[20];

//In game variables
namespace Game {
	
	extern PK2Kartta* current_map;
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

bool PK_Check_File(char *filename);
void PK_Load_EpisodeDir(char *tiedosto);

void PK_Start_Info(char *text);