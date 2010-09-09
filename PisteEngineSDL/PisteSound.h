#ifndef P_SOUND
#define P_SOUND

/* INCLUDES ----------------------------------------------------------------------------------*/

#include <SDL/SDL_mixer.h>

#define DIRECTSOUND_VERSION 0x0700

/* DEFINES -----------------------------------------------------------------------------------*/

#define		PS_VIRHE  -1

#define SOUND_NULL     0
#define SOUND_LOADED   1
#define SOUND_PLAYING  2
#define SOUND_STOPPED  3

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;
typedef unsigned long  HWND;
typedef unsigned long  HINSTANCE;

#ifndef __midasdll_h
#define __midasdll_h
#ifdef __cplusplus
extern "C" {
#endif

typedef Mix_Music* MIDASmodule;
typedef DWORD MIDASmodulePlayHandle;
typedef DWORD MIDASsample;
typedef DWORD MIDASsamplePlayHandle;
enum MIDASoptions
{
    MIDAS_OPTION_NONE = 0,
    MIDAS_OPTION_MIXRATE,
    MIDAS_OPTION_OUTPUTMODE,
    MIDAS_OPTION_MIXBUFLEN,
    MIDAS_OPTION_MIXBUFBLOCKS,
    MIDAS_OPTION_DSOUND_MODE,
    MIDAS_OPTION_DSOUND_HWND,
    MIDAS_OPTION_DSOUND_OBJECT,
    MIDAS_OPTION_DSOUND_BUFLEN,
    MIDAS_OPTION_16BIT_ULAW_AUTOCONVERT,
    MIDAS_OPTION_FILTER_MODE,
    MIDAS_OPTION_MIXING_MODE,
    MIDAS_OPTION_DEFAULT_STEREO_SEPARATION,
    MIDAS_OPTION_FORCE_NO_SOUND
};
void MIDASstartup();
void MIDASsetOption(int option, DWORD value);
void MIDASinit();
bool MIDASconfig();
void MIDASclose();
void MIDASstartBackgroundPlay(int i);
void MIDASstopBackgroundPlay();
bool MIDASloadConfig(char *fileName);
void MIDASsaveConfig(char *fileName);
void MIDASfreeModule(MIDASmodule module);
MIDASmodule MIDASloadModule(char *fileName);
MIDASmodulePlayHandle MIDASplayModule(MIDASmodule mod, bool loop);
void MIDASstopModule(MIDASmodulePlayHandle mod);
bool MIDASsetMusicVolume(MIDASmodulePlayHandle hnd, int volume);
#ifdef __cplusplus
}
#endif

#endif

const int MAX_SOUNDS      = 100;// max number of sounds in system at once 
const int MAX_POLY_PER_FX = 5;  // max maara samaa ääntä yhtäaikaisesti

/* PROTOTYPES --------------------------------------------------------------------------------*/

int PisteSound_Alusta(HWND &main_window_handle, HINSTANCE &hinstance_app, 
					  UCHAR kanavia, DWORD samplerate, UCHAR bitrate);

//LPDIRECTSOUND PisteSound_Get_DirectSound();

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
