//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

const int SOUND_SAMPLERATE = 22050;

extern int switch_sound;
extern int jump_sound;
extern int splash_sound;
extern int open_locks_sound;
extern int menu_sound;
extern int moo_sound;
extern int doodle_sound;
extern int pump_sound;
extern int score_sound;
extern int apple_sound;

int Load_SFX();

void Update_GameSFX();
void Play_GameSFX(int aani, int voimakkuus, int x, int y, int freq, bool random_freq);
void Play_MenuSFX(int aani, int voimakkuus);