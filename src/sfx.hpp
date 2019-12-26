#pragma once

const int SOUND_SAMPLERATE = 22050;

extern int kytkin_aani;
extern int hyppy_aani;
extern int loiskahdus_aani;
extern int avaa_lukko_aani;
extern int menu_aani;
extern int ammuu_aani;
extern int kieku_aani;
extern int tomahdys_aani;
extern int pistelaskuri_aani;

int Load_SFX();

void Play_GameSFX(int aani, int voimakkuus, int x, int y, int freq, bool random_freq);
void Play_MenuSFX(int aani, int voimakkuus);