#ifndef P_WAIT
#define P_WAIT

/* INCLUDES ----------------------------------------------------------------------------------*/

#include <windows.h> 

/* DEFINES -----------------------------------------------------------------------------------*/

/* PROTOTYPES --------------------------------------------------------------------------------*/

DWORD PisteWait_Get(void);

DWORD PisteWait_Start(void);

DWORD PisteWait_Wait(DWORD count);

DWORD PisteWait_Laske_Aika(DWORD count);

#endif