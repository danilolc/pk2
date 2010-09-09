#ifndef P_SFX
#define P_SFX

#include "PisteSound.h"

#include <SDL/SDL_mixer.h>

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

#define _MAX_PATH 128  //TODO: real value

class PisteSFX
{
	private:

	int Lataa_Wav(char *filename, int control_flags);

	public:
	Mix_Chunk *sound;
	char tiedosto[_MAX_PATH];
	UCHAR kanavia;
	DWORD samplerate;
	UCHAR bitrate;
	
	PisteSFX();
	PisteSFX(UCHAR kanavia, DWORD samplerate, UCHAR bitrate);
	virtual ~PisteSFX();
	
	void Tuhoa();
	int Soita();
	int Soita(int volume);
	int Soita(int volume, int pan, int freq);
	int Lataa(char *filename, int control_flags);
};

int  PisteSFX_Set_Sfx(bool play);
bool PisteSFX_Get_Sfx(void);

int  PisteSFX_Lataa_WAV(PisteSFX *soundi, char *filename, int control_flags);

#endif
