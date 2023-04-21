//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PFile.hpp"
#include "engine/platform.hpp"

namespace PSound {

const int CHANNELS = 32;//16;

int init(int buffer_size, bool multi_thread);
int update();
int terminate();

bool is_playing(int channel);
int  load_sfx(PFile::Path path);
int  set_channel(int channel, int panoramic, int volume);
int  play_sfx(int index, int volume, int panoramic, int freq);
int  free_sfx(int index);
void reset_sfx();

int  start_music(PFile::Path path);
void set_musicvolume(u8 volume);
void set_musicvolume_now(u8 volume);
void stop_music();

int resume_music();
int play_overlay_music();
int load_overlay_music(PFile::Path path);

}
