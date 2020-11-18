//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PFile.hpp"
#include "engine/platform.hpp"
#include <string>

extern int screen_width;
extern int screen_height;

#ifdef __ANDROID__
// Variables used on data menu

extern bool external_dir;

extern bool external_writable;
extern const char* External_Path;
extern const char* Internal_Path;

extern bool save_on_external;
extern bool save_on_internal;

extern char external_save_code[8];
extern char internal_save_code[8];

#endif

extern char id_code[8];

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

extern bool doublespeed;

extern float fps;
extern bool show_fps;

extern bool PK2_error;
extern const char* PK2_error_msg;

void Id_To_String(u32 id, char* string);

void Calculate_SinCos();
int  PK2_Error(const char* msg);

int Clicked();

void Draw_Cursor(int x, int y);
void Move_DataPath(std::string new_path);

void Prepare_DataPath();

bool FindAsset(PFile::Path* path, const char* default_dir);
