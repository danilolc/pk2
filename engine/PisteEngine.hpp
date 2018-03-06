//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef P_ENGINE
#define P_ENGINE

#include "platform.hpp"

#include "PisteDraw.hpp"
#include "PisteInput.hpp"
#include "PisteSound.hpp"
#include "PisteUtils.hpp"
#include "PisteLanguage.hpp"

void Piste_IgnoreFrame();
void Piste_SetFPS(int fps);
int  Piste_GetFPS();
void Piste_SetDebug(bool set);

int Piste_Init(int width, int height, const char* name);
int Piste_Loop(bool &running, int (*GameLogic)());
int Piste_Quit();

#endif
