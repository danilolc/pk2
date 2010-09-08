#ifndef P_SOUND
#define P_SOUND

/* INCLUDES ----------------------------------------------------------------------------------*/

#define DIRECTSOUND_VERSION 0x0700
#include "D:\DXSDK\Include\dsound.h" // MUISTA KORJATA POLKU OIKEIN!!!!
#include <windows.h> 

/* DEFINES -----------------------------------------------------------------------------------*/

#define		PS_VIRHE  -1

#define SOUND_NULL     0
#define SOUND_LOADED   1
#define SOUND_PLAYING  2
#define SOUND_STOPPED  3

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

const int MAX_SOUNDS      = 100;// max number of sounds in system at once 
const int MAX_POLY_PER_FX = 5;  // max maara samaa ääntä yhtäaikaisesti

/* PROTOTYPES --------------------------------------------------------------------------------*/

int PisteSound_Alusta(HWND &main_window_handle, HINSTANCE &hinstance_app, 
					  UCHAR kanavia, DWORD samplerate, UCHAR bitrate);

LPDIRECTSOUND PisteSound_Get_DirectSound();

int PisteSound_Lopeta(void);

int PisteSound_Aanet_Paalla(bool play);

int PisteSound_SFX_Soita(int sfx_index);

int PisteSound_SFX_Soita(int sfx_index, int volume); /* volume: 0 - 100 */

int PisteSound_SFX_Soita(int sfx_index, int volume, int pan, int freq); 
/* pan: -10 000 - 10 000 | freq: 100 -> 100 000 (Hz)  */

int PisteSound_SFX_Tuhoa(int sfx_index);

int PisteSound_SFX_Uusi(char *filename);

char *PisteSound_Virheilmoitus();

#endif