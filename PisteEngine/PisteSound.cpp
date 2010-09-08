/* 
PisteEngine - PisteSound 0.1	
09.12.2001	Janne Kivilahti / Piste Gamez
*/

/* INCLUDES ----------------------------------------------------------------------------------*/

#include "PisteSound.h"
#include "PisteSFX.h"
#include "PisteLog.h"

/* DEFINES -----------------------------------------------------------------------------------*/


/* TYPE DEFINITIONS --------------------------------------------------------------------------*/

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

/* VARIABLES ---------------------------------------------------------------------------------*/

const int				MAX_SFX	= 500;

UCHAR					PS_kanavia;				// 1 = mono, 2 = stereo
DWORD					PS_samplerate;			
UCHAR					PS_bitrate;				// 8 tai 16

HWND					PS_main_window_handle	= NULL; // globally track main window
HINSTANCE				PS_hinstance_app		= NULL; // globally track hinstance

LPDIRECTSOUND			PS_lpds;
LPDIRECTSOUNDBUFFER		PS_lpdsbuffer;
DSBUFFERDESC			PS_dsbd;						// direct soundin bufferin kuvaus
WAVEFORMATEX			PS_pcmwf;

PisteSFX				*soundit[MAX_SFX];

char					PS_virhe[100];

bool					PS_unload = true;

/* PROTOTYPES --------------------------------------------------------------------------------*/

/* METHODS -----------------------------------------------------------------------------------*/

int PisteSound_Alusta(HWND &main_window_handle, HINSTANCE &hinstance_app, 
					  UCHAR kanavia, DWORD samplerate, UCHAR bitrate)
{
	if (PS_unload) {
	
		PS_main_window_handle	= (HWND &)main_window_handle;
		PS_hinstance_app		= (HINSTANCE &)hinstance_app;	

		strcpy(PS_virhe,"unknown error with sound system.");

		if (FAILED(DirectSoundCreate(NULL, &PS_lpds, NULL)))
		{
			strcpy(PS_virhe,"could not create direct sound surface.");
			PisteLog_Kirjoita("[Error] Piste Sound: Could not create direct sound surface!\n");
			return PS_VIRHE;
		}

		if (FAILED(PS_lpds->SetCooperativeLevel(PS_main_window_handle, DSSCL_NORMAL)))
		{
			strcpy(PS_virhe,"sound system could not cooperate with windows.");
			PisteLog_Kirjoita("[Error] Piste Sound: Could not cooperate with Windows!\n");
			return PS_VIRHE;
		}

		PS_kanavia		= kanavia;
		PS_samplerate	= samplerate;
		PS_bitrate		= bitrate;

		for (int i=0;i<MAX_SFX;i++)
			if (soundit[i])
				delete soundit[i];

		PS_unload = false;
	}

	return 0;
}

int PisteSound_SFX_Uusi(char *filename)
{
	int i=0, virhe;
	bool loytyi = false;

	PisteLog_Kirjoita("- Piste Sound: Loading sound file: ");
	PisteLog_Kirjoita(filename);
	PisteLog_Kirjoita("\n");

	/* Etsitään löytyikö jo samasta tiedostosta ladattu ääni */
		
	for (i=0;i<MAX_SFX;i++)
		if (soundit[i] != NULL)
			if (strcmp(filename,soundit[i]->tiedosto)==0 && strcmp(soundit[i]->tiedosto," ")!=0)
				return i;
	
	i = 0;

	while (i<MAX_SFX && !loytyi)
	{
		if (soundit[i] == NULL)
		{
			soundit[i] = new PisteSFX(PS_kanavia, PS_samplerate, PS_bitrate);

			if ((virhe = soundit[i]->Lataa(	filename,
											DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY,
											PS_lpds,
											PS_pcmwf,
											PS_dsbd))>0)
			{
				strcpy(PS_virhe,"could not load file ");
				strcat(PS_virhe,filename);
				if (virhe == 1)
					strcat(PS_virhe,": \n target buffer is not empty.");
				if (virhe == 2)
					strcat(PS_virhe,": \n no file / wrong type / locked.");
				if (virhe == 3)
					strcat(PS_virhe,": \n duplicate buffers failed.");

				PisteLog_Kirjoita("[Error] Piste Sound: ");
				PisteLog_Kirjoita(PS_virhe);
				PisteLog_Kirjoita("\n");
				return PS_VIRHE;
			}
			
			loytyi = true;
		}
		i++;
	}

	if (!loytyi)
	{
		strcpy(PS_virhe,"can't load more sounds.");
		PisteLog_Kirjoita("[Error] Piste Sound: Maximum limit of sounds exceeded!\n");
		return PS_VIRHE;
	}

	return i-1;
}

int PisteSound_Aanet_Paalla(bool play)
{
	PisteSFX_Set_Sfx(play);
	return 0;
}

int PisteSound_SFX_Tuhoa(int sfx_index)
{
	int i=0;
	bool loytyi = false;


	if (sfx_index > -1 && sfx_index < MAX_SFX)
		if (soundit[sfx_index] != NULL)
		{
			soundit[sfx_index]->Tuhoa();
			soundit[sfx_index] = NULL;
		}

	return i;
}

int PisteSound_SFX_Soita(int sfx_index)
{
	if (soundit[sfx_index] != NULL)
		if (soundit[sfx_index]->Soita()!=0)
		{
			strcpy(PS_virhe,"error playing a sound");
			return PS_VIRHE;	
		}

	return 0;
}

int PisteSound_SFX_Soita(int sfx_index, int volume)
{
	if (soundit[sfx_index] != NULL)
		if (soundit[sfx_index]->Soita(volume)!=0)
		{
			strcpy(PS_virhe,"error playing a sound");
			PisteLog_Kirjoita("[Error] Piste Sound: Error playing a sound!\n");
			return PS_VIRHE;	
		}

	return 0;
}

int PisteSound_SFX_Soita(int sfx_index, int volume, int pan, int freq)
{
	if (soundit[sfx_index] != NULL)
		if (soundit[sfx_index]->Soita(volume, pan, freq)!=0)
		{
			strcpy(PS_virhe,"error playing a sound");
			PisteLog_Kirjoita("[Error] Piste Sound: Error playing a sound!\n");
			return PS_VIRHE;	
		}

	return 0;
}

int PisteSound_Lopeta(void)
{
	if (!PS_unload) {
	
		for (int i=0;i<MAX_SFX;i++)
			if (soundit[i] != NULL)
			{
				delete soundit[i];
				soundit[i] = NULL;
			}
		
		if (PS_lpdsbuffer)
		{
			PS_lpdsbuffer->Release();
		}

		if (PS_lpds)
		{
			PS_lpds->Release();
		}

		PS_unload = true;
	}
	return 0;
}

LPDIRECTSOUND PisteSound_Get_DirectSound()
{
	return PS_lpds;
}

char *PisteSound_Virheilmoitus()
{
	return PS_virhe;
}