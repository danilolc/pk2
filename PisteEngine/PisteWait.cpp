/* 
PisteEngine - PisteWait 1.0	
09.12.2001	Janne Kivilahti / Piste Gamez
*/

/* INCLUDES ----------------------------------------------------------------------------------*/

#include "PisteWait.h"
#include <time.h>
/* DEFINES -----------------------------------------------------------------------------------*/


/* TYPE DEFINITIONS --------------------------------------------------------------------------*/


/* VARIABLES ---------------------------------------------------------------------------------*/

DWORD		PW_laskuri = 0;		 

/* METHODS -----------------------------------------------------------------------------------*/

DWORD PisteWait_Get(void)
{	
	return(GetTickCount());
}

DWORD PisteWait_Start(void)
{
	PW_laskuri = PisteWait_Get();
	return(PW_laskuri);
}

DWORD PisteWait_Wait(DWORD count)
{
	while ((PisteWait_Get() - PW_laskuri) < count);
	return (PisteWait_Get());
}

DWORD PisteWait_Laske_Aika(DWORD count)
{
	return(PisteWait_Get() - PW_laskuri); 
}