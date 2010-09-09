#ifndef P_WAIT
#define P_WAIT

/* INCLUDES ----------------------------------------------------------------------------------*/


/* DEFINES -----------------------------------------------------------------------------------*/

typedef unsigned short WORD;
typedef unsigned long  DWORD;

/* PROTOTYPES --------------------------------------------------------------------------------*/

DWORD PisteWait_Get(void);

DWORD PisteWait_Start(void);

DWORD PisteWait_Wait(DWORD count);

DWORD PisteWait_Laske_Aika(DWORD count);

#endif
