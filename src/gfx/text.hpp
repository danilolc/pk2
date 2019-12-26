
#pragma once

#include "PisteLanguage.hpp"

//Fonts
extern int fontti1;
extern int fontti2;
extern int fontti3;
extern int fontti4;
extern int fontti5;

int Load_Fonts(PisteLanguage* lang);

int CreditsText_Draw(char *text, int font, int x, int y, DWORD start, DWORD end, DWORD time);
int Wavetext_Draw(char *teksti, int fontti, int x, int y);
int WavetextSlow_Draw(char *teksti, int fontti, int x, int y);

void Fadetext_Init();
void Fadetext_New(int fontti, char *teksti, DWORD x, DWORD y, DWORD ajastin, bool ui);
int Fadetext_Draw();
void Fadetext_Update();