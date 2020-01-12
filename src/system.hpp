//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "engine/platform.hpp"
#include <string>

extern int screen_width;
extern int screen_height;

extern std::string data_path;

extern int game_assets;
extern int game_assets2;
extern int bg_screen;

extern int key_delay;

extern double cos_table[360];
extern double sin_table[360];
extern int degree;
extern int degree_temp;

extern bool test_level;
extern bool dev_mode;

extern bool doublespeed; //TODO

extern float fps;
extern bool show_fps;

extern bool PK2_error;
extern const char* PK2_error_msg;

void Calculate_SinCos();
int PK2_Error(const char* msg);

void Draw_Cursor(int x, int y);




