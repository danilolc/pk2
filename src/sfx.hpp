//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
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

int Load_SFX();

void Play_GameSFX(int aani, int voimakkuus, int x, int y, int freq, bool random_freq);
void Play_MenuSFX(int aani, int voimakkuus);