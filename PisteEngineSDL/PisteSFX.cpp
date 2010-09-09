#include <iostream> 
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <fcntl.h>

#include "PisteSFX.h"

#define DSBVOLUME_MAX MIX_MAX_VOLUME
#define DSBVOLUME_MIN 0

bool play_sfx = true;

PisteSFX::PisteSFX()
{
	strcpy(this->tiedosto," ");
}

PisteSFX::PisteSFX(UCHAR kanavia, DWORD samplerate, UCHAR bitrate)
{
	this->kanavia		= kanavia;
	this->samplerate	= samplerate;
	this->bitrate		= bitrate;
	strcpy(this->tiedosto," ");
}

PisteSFX::~PisteSFX()
{
	Tuhoa();
}

void PisteSFX::Tuhoa()
{
	Mix_FreeChunk(sound);
}

int PisteSFX::Lataa(char *filename, int control_flags)
{

		if (PisteSFX_Lataa_WAV(this, filename, control_flags) == -1)
			return 2;

		strcpy(this->tiedosto,filename);

	return 0;
}

int PisteSFX::Soita()
{
	if (play_sfx)
	{
		Mix_VolumeChunk(sound, MIX_MAX_VOLUME/2);
		if(Mix_PlayChannel(-1, sound, 0)==-1) {
  	  printf("Mix_PlayChannel: %s\n",Mix_GetError());
		}
	}
	return 0;
}

int PisteSFX::Soita(int volume)
{
	if (play_sfx)
	{
		volume = (100-volume) * -30;//-100

		if (volume < DSBVOLUME_MIN)
			volume = DSBVOLUME_MIN;

		if (volume > DSBVOLUME_MAX)
			volume = DSBVOLUME_MAX;

		Mix_Volume(1,volume);
		if(Mix_PlayChannel(-1, sound, 0)==-1) {
  	  printf("Mix_PlayChannel: %s\n",Mix_GetError());
		}
	}
	return 0;
}

int PisteSFX::Soita(int volume, int pan, int freq)
{
	if (play_sfx)
	{
		volume = (100-volume) * -30;//-100

		if (volume < DSBVOLUME_MIN)
			volume = DSBVOLUME_MIN;

		if (volume > DSBVOLUME_MAX)
			volume = DSBVOLUME_MAX;

		if (pan < -10000)
			pan = -10000;

		if (pan > 10000)
			pan = 10000;

		if (freq < 100)
			freq = 100;

		if (freq > 100000)
			freq = 100000;
/*
		if (FAILED(this->dsbuffer[buffer_index]->SetVolume(volume)))
			return 1;

		if (FAILED(this->dsbuffer[buffer_index]->SetPan(pan)))
			return 1;

		if (FAILED(this->dsbuffer[buffer_index]->SetFrequency(freq)))
			return 1;

		if (FAILED(this->dsbuffer[buffer_index]->Play(0,0,0)))
			return 1;
*/
		Mix_Volume(1,volume);
		if(Mix_PlayChannel(-1, sound, 0)==-1) {
  	  printf("Mix_PlayChannel: %s\n",Mix_GetError());
		}
	}
	return 0;
}

int PisteSFX_Set_Sfx(bool play)
{
	play_sfx = play;
	return 0;
}

bool PisteSFX_Get_Sfx(void)
{
	return play_sfx;
}

int PisteSFX_Lataa_WAV(PisteSFX *soundi, char *filename, int control_flags)
{
	soundi->sound = Mix_LoadWAV(filename);
	if(soundi->sound == NULL) {
		fprintf(stderr, "PisteSFX_Lataa_WAV(%s): %s\n", filename, Mix_GetError());
	}
	return(0);

} // end DSound_Load_WAV
