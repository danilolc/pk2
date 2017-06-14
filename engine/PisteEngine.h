//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef P_ENGINE
#define P_ENGINE

#ifdef _WIN32
#include "win32hacks.h"
#endif

#include "types.h"

#include "PisteDraw.h"
#include "PisteInput.h"
#include "PisteSound.h"
#include "PisteUtils.h"
#include "PisteLanguage.h"

int Piste_Init();
int Piste_Loop(bool &running,int (*Game)());
int Piste_Quit();

int Piste_IgnoreFrame();
void Piste_SetFPS(int fps);

#endif
