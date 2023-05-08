//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PFile.hpp"
#include "engine/platform.hpp"
#include <string>

// settings text (shouldn't change during execution)

extern int screen_width;
extern int screen_height;

extern int audio_buffer_size;
extern bool audio_multi_thread;
extern int render_method;

#ifdef __ANDROID__
// Variables used on data menu

extern bool external_dir;

extern std::string External_Path;
extern std::string Internal_Path;

#endif

extern char id_code[8];

extern std::string data_path;

extern int game_assets;
extern int game_assets2;
extern int bg_screen;

extern int key_delay;
extern bool mouse_hidden;

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
extern bool speedrun_mode;

extern bool PK2_error;
extern const char* PK2_error_msg;

#define FADE_FAST   0.05
#define FADE_NORMAL 0.02
#define FADE_SLOW   0.01

void Fade_in(float speed);
void Fade_out(float speed);
float Screen_Alpha();
bool Is_Fading();
void Update_Colors();

void Do_Thunder();

void Id_To_String(u32 id, char* string);

void Calculate_SinCos();
int  PK2_Error(const char* msg);

int Clicked();

void Draw_Cursor(int x, int y);
void Move_DataPath(std::string new_path);

void Prepare_DataPath();

bool FindAsset(PFile::Path* path, const char* default_dir);

int Set_Screen_Mode(int mode);