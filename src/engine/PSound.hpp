//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "engine/platform.hpp"

namespace PSound {

int init();
int update();
int terminate();

int  load_sfx(const char* filename); //0 success, -1 fail
int  play_sfx(int index, int volume, int panoramic, int freq);
int  free_sfx(int index);
void reset_sfx();

int  start_music(const char* filename);
void set_musicvolume(int volume);
void set_musicvolume_now(int volume);
void stop_music();

}
