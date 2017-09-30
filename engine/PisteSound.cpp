//#########################
//PisteEngine - PisteSound
//by Janne Kivilahti from Piste Gamez
//#########################

#include <cstdio>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "PisteSound.h"
#include "PisteUtils.h"
#include "platform.h"

#define AUDIO_FREQ 44100

const int MAX_SOUNDS = 300;

int def_freq = 22050;
int mus_volume = 100;
int sfx_volume = 100;

Mix_Chunk* indexes[MAX_SOUNDS]; //The original chunks loaded
Uint8* freq_chunks[MIX_CHANNELS]; //The chunk allocated for each channel

Mix_Music* music = NULL;
char playingMusic[_MAX_PATH] = "";

int PisteSound_LoadSFX(char* filename){
	int i = -1;
	for(i=0;i<MAX_SOUNDS;i++)
		if (indexes[i] == NULL){
			indexes[i] = Mix_LoadWAV(filename);
			break;
		}
	return i;
}
void PisteSound_PlaySFX(int index){
	PisteSound_PlaySFX(index, sfx_volume, def_freq);
}
void PisteSound_PlaySFX(int index, int volume, int freq){
	int channel;

	if(index == -1) return;
	if(indexes[index] == NULL) return;

	volume = volume * 128 / 100;
	indexes[index]->volume = volume;

	Uint8* bkp_buf = indexes[index]->abuf;
	Uint32 bkp_len = indexes[index]->alen;

	//Change the chunk frequency
	SDL_AudioCVT cvt;
	SDL_BuildAudioCVT(&cvt, MIX_DEFAULT_FORMAT, 2, freq, MIX_DEFAULT_FORMAT, 2, def_freq);
	if (cvt.needed) {
		cvt.len = indexes[index]->alen;
		cvt.buf = (Uint8*)SDL_malloc(cvt.len * cvt.len_mult);
		if (cvt.buf == NULL) return;
		SDL_memcpy(cvt.buf, bkp_buf, indexes[index]->alen);
		if(SDL_ConvertAudio(&cvt) < 0) return;

		indexes[index]->abuf = cvt.buf;
		indexes[index]->alen = cvt.len_cvt;

		for(channel=0; channel<MIX_CHANNELS; channel++) //Find a free channel
			if(!Mix_Playing(channel)) break;

		if(channel == MIX_CHANNELS) return;

		if(freq_chunks[channel] != NULL) SDL_free(freq_chunks[channel]); //Don't ovewrite a allocated pointer
		freq_chunks[channel] = cvt.buf; //Save the buffer to delete it after play

		Mix_PlayChannel(channel, indexes[index], 0);
	}
	else{
		Mix_PlayChannel(-1, indexes[index], 0);
	}

	indexes[index]->abuf = bkp_buf;
	indexes[index]->alen = bkp_len;
}
void PisteSound_SetSFXVolume(int volume){
	sfx_volume = volume;
}
int PisteSound_FreeSFX(int index){
	if(indexes[index] != NULL)
		Mix_FreeChunk(indexes[index]);
	indexes[index] = NULL;
	return 0;
}
void PisteSound_ResetSFX(){
	int i;
	for(i=0;i<MAX_SOUNDS;i++)
		PisteSound_FreeSFX(i);
}

int PisteSound_StartMusic(char* filename){
	PisteUtils_RemoveSpace(filename);
	if(!strcmp(playingMusic,filename)) return 0;

	music = Mix_LoadMUS(filename);
	if (music == NULL){
		printf("Music load error: %s\n",Mix_GetError());
		return -1;
	}
	if (Mix_PlayMusic( music, -1) == -1){
		printf("Musir play error: %s\n",Mix_GetError());
		return -1;
	}

	strcpy(playingMusic,filename);
	return 0;
}
void PisteSound_SetMusicVolume(int volume){
	Mix_VolumeMusic(volume * 128 / 100);
	mus_volume = volume;
}
void PisteSound_StopMusic(){
	Mix_FadeOutMusic(1);
}

int PisteSound_Start(){
	if( Mix_OpenAudio(AUDIO_FREQ, MIX_DEFAULT_FORMAT, 2, 4096) < 0){
		printf("Unable to init Mixer: %s\n", Mix_GetError());
		return -1;
	}

	Mix_Init(MIX_INIT_MOD);
	return 0;
}
int PisteSound_Update(){
	for(int i=0;i<MIX_CHANNELS;i++)
		if(!Mix_Playing(i) && freq_chunks[i] != NULL){
				SDL_free(freq_chunks[i]); //Make sure that all allocated chunks will be deleted after playing
				freq_chunks[i] = NULL;
		}

}
int PisteSound_End(){
	PisteSound_ResetSFX();
	if(music != NULL) Mix_FreeMusic(music);
	Mix_CloseAudio();
	return 0;
}
