//#########################
//PisteEngine - PisteSound
//by Janne Kivilahti from Piste Gamez
//#########################
#pragma once

#include "platform.hpp"

namespace PSound {

int init();
int update();
int terminate();

int  load_sfx(char* filename); //0 success, -1 fail
int  play_sfx(int index);
int  play_sfx(int index, int volume, int panoramic, int freq);
void set_sfxvolume(int volume);
int  free_sfx(int index);
void reset_sfx();

int  start_music(char* filename);
void set_musicvolume(int volume);
void set_musicvolume_now(int volume);
void stop_music();

}
