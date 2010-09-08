#ifndef P_SFX
#define P_SFX

#include "D:\Visual Studio\MyProjects\PisteEngine\PisteSound.h"
#include <io.h>
class PisteSFX
{
	private:

	int Lataa_Wav(char *filename, int control_flags, LPDIRECTSOUND lpds, 
			  WAVEFORMATEX pcmwf, DSBUFFERDESC dsbd);		

	public:
	LPDIRECTSOUNDBUFFER dsbuffer[MAX_POLY_PER_FX];
	int state[MAX_POLY_PER_FX];
	int rate;
	int size;
	int id;
	int buffer_index;
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
	int Lataa(char *filename, int control_flags, LPDIRECTSOUND lpds, 
			  WAVEFORMATEX pcmwf, DSBUFFERDESC dsbd);
};

int  PisteSFX_Set_Sfx(bool play);
bool PisteSFX_Get_Sfx(void);

int  PisteSFX_Lataa_WAV(PisteSFX *soundi, char *filename, int control_flags, LPDIRECTSOUND lpds, 
					WAVEFORMATEX pcmwf, DSBUFFERDESC dsbd);

#endif