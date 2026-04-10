//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

/**
 * TODO
 * Completely redesign this file and system.cpp
 */


#include "engine/PFile.hpp"
#include "engine/types.hpp"
#include <string>

extern int screen_width;
extern int screen_height;

//extern char id_code[8];

extern int global_gfx_texture;
extern int global_gfx_texture2;
extern int default_palette;


//extern int bg_screen;
//extern bool mouse_hidden;

extern double cos_table[360];
extern double sin_table[360];

#define cos_table(i) cos_table[((int)(i) % 360 + 360) % 360]
#define sin_table(i) sin_table[((int)(i) % 360 + 360) % 360]

extern int degree;
extern int degree_temp;

extern bool test_level;
extern bool dev_mode;

extern float fps;
extern bool show_fps;



#define FADE_FAST   0.05
#define FADE_NORMAL 0.02
#define FADE_SLOW   0.01

void Fade_in(float speed);
void Fade_out(float speed);
float Screen_Alpha();
bool Is_Fading();
void Update_Colors();

void StartLightningEffect();

void Calculate_SinCos();

//int Clicked();

void Draw_Cursor(int x, int y);

int Set_Screen_Mode(int mode);