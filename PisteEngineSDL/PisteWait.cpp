/* 
 PisteEngineSDL - PisteWait
 <samuli.tuomola@gmail.com>
*/

/* INCLUDES ----------------------------------------------------------------------------------*/

#include "PisteWait.h"
#include <time.h>
#include <SDL/SDL.h>
/* DEFINES -----------------------------------------------------------------------------------*/

#define FPS 70

/* TYPE DEFINITIONS --------------------------------------------------------------------------*/


/* VARIABLES ---------------------------------------------------------------------------------*/

DWORD		PW_laskuri = 0;		 

/* METHODS -----------------------------------------------------------------------------------*/

DWORD PisteWait_Get(void)
{	
	return(SDL_GetTicks());
}

DWORD PisteWait_Start(void)
{
	PW_laskuri = PisteWait_Get();
	return(PW_laskuri);
}

DWORD PisteWait_Wait(DWORD count)
{
	//while ((PisteWait_Get() - PW_laskuri) < count);
	Uint32 last_time = PisteWait_Get();

	if (last_time < PW_laskuri + (1000 / FPS))
		SDL_Delay(PW_laskuri + (1000 / FPS) - last_time);

	return (PisteWait_Get());
}

DWORD PisteWait_Laske_Aika(DWORD count)
{
	return(PisteWait_Get() - PW_laskuri); 
}
