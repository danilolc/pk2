//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef P_ENGINE
#define P_ENGINE

//#include "types.h"
#include <stdint.h>

typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;

#include "PisteDraw.h"
#include "PisteInput.h"
#include "PisteSound.h"
#include "PisteUtils.h"
#include "PisteLanguage.h"

void Piste_IgnoreFrame();
void Piste_SetFPS(int fps);

int Piste_Init(int width, int height, const char* name);
int Piste_Loop(bool &running, int (*GameLogic)());
int Piste_Quit();

#endif
