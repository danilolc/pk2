/* 
 PisteEngineSDL - PisteSound
 <samuli.tuomola@gmail.com>
*/

/* INCLUDES ----------------------------------------------------------------------------------*/

#include "PisteSound.h"
#include "PisteSFX.h"
#include "PisteLog.h"

/* DEFINES -----------------------------------------------------------------------------------*/


/* TYPE DEFINITIONS --------------------------------------------------------------------------*/

/* VARIABLES ---------------------------------------------------------------------------------*/

const int				MAX_SFX	= 500;

UCHAR					PS_kanavia;				// 1 = mono, 2 = stereo
DWORD					PS_samplerate;			
UCHAR					PS_bitrate;				// 8 tai 16

PisteSFX				*soundit[MAX_SFX];

char					PS_virhe[100];

bool					PS_unload = true;

/* PROTOTYPES --------------------------------------------------------------------------------*/

/* METHODS -----------------------------------------------------------------------------------*/

int PisteSound_Alusta(HWND &main_window_handle, HINSTANCE &hinstance_app, 
					  UCHAR kanavia, DWORD samplerate, UCHAR bitrate)
{
	//int init = Mix_Init(MIX_INIT_MOD);
	int audio_buffers = 4096;

	if (PS_unload) {
		PS_kanavia		= kanavia;
		PS_samplerate	= samplerate;
		PS_bitrate		= AUDIO_S8;
		if(bitrate == 16)
			PS_bitrate		= AUDIO_S16SYS;

		for (int i=0;i<MAX_SFX;i++)
			if (soundit[i])
				delete soundit[i];

		PS_unload = false;
	}

	if(Mix_OpenAudio(PS_samplerate, PS_bitrate, PS_kanavia, audio_buffers) != 0) {
	 	fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
		exit(1);
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

			if ((virhe = soundit[i]->Lataa(	filename, 0)) > 0)
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
		//Mix_Quit();
	
		for (int i=0;i<MAX_SFX;i++)
			if (soundit[i] != NULL)
			{
				delete soundit[i];
				soundit[i] = NULL;
			}
		
		PS_unload = true;
	}
	return 0;
}

char *PisteSound_Virheilmoitus()
{
	return PS_virhe;
}

void MIDASstartup() {}
void MIDASsetOption(int option, DWORD value) {}
void MIDASinit() {}
bool MIDASconfig() { return true; }
void MIDASclose() {}
void MIDASstartBackgroundPlay(int i) {}
void MIDASstopBackgroundPlay() {}
bool MIDASloadConfig(char *fileName) { return true; }
void MIDASsaveConfig(char *fileName) {}
void MIDASfreeModule(MIDASmodule module) {
	Mix_FreeMusic(module);
}
MIDASmodule MIDASloadModule(char *fileName) {
	Mix_Music *music = Mix_LoadMUS(fileName);
	if(!music) {
		// maybe it's really uppercase? grasping at straws here
		char *file = strdup(fileName);
		char *c = strrchr(file, '/');
		while(c != NULL && *c != '\0') *c++ = toupper(*c);

		music = Mix_LoadMUS(file);
		if(!music) {
		  printf("Mix_LoadMUS(\"%s\"): %s\n", file, Mix_GetError());
		}
	}
	return music;
}
MIDASmodulePlayHandle MIDASplayModule(MIDASmodule mod, bool loop) {
	if(Mix_PlayMusic(mod, loop)==-1) {
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
	}
}
void MIDASstopModule(MIDASmodulePlayHandle mod) {}
bool MIDASsetMusicVolume(MIDASmodulePlayHandle hnd, int volume) {
	Mix_VolumeMusic(volume);
	return true;
}

