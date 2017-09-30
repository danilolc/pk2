//#########################
//PisteEngine - PisteSound
//by Janne Kivilahti from Piste Gamez
//#########################

#include <cstdio>

#include <SDL2/SDL_mixer.h>
#include "PisteSound.h"
#include "PisteUtils.h"

const int MAX_SOUNDS = 300;

int def_freq = 22050;
int mus_volume = 100;
int sfx_volume = 100;

Mix_Chunk* indexes[MAX_SOUNDS];
Mix_Music* music = NULL;

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
	if(index == -1 || indexes[index] == NULL) return;
	volume = volume * 128 / 100;
	indexes[index]->volume = volume;

	Uint8* bkp_buf = indexes[index]->abuf;
	Uint32 bkp_len = indexes[index]->alen;

	//Change the chunk frequency
	SDL_AudioCVT cvt;
	//SDL_BuildAudioCVT(&cvt, MIX_DEFAULT_FORMAT, 2, freq, MIX_DEFAULT_FORMAT, 2, def_freq);
	SDL_BuildAudioCVT(&cvt, MIX_DEFAULT_FORMAT, 2, 2000, MIX_DEFAULT_FORMAT, 2, 1000);
	if (cvt.needed) {
		cvt.len = indexes[index]->alen;
		cvt.buf = (Uint8 *)SDL_malloc(cvt.len * cvt.len_mult);
		if (cvt.buf == NULL) return;
		SDL_memcpy(cvt.buf, bkp_buf, indexes[index]->alen);
		if(SDL_ConvertAudio(&cvt) < 0) return;
		indexes[index]->abuf = cvt.buf;
		indexes[index]->alen = cvt.len_cvt;
	}

	Mix_PlayChannel(-1, indexes[index], 0);

	//if(cvt.buf != NULL) SDL_free(cvt.buf);
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
	return 0;
	music = Mix_LoadMUS(filename);
	if (music == NULL){
		printf("Music load error: %s\n",Mix_GetError());
		return -1;
	}
	if (Mix_PlayMusic( music, -1) == -1){
		printf("Musir play error: %s\n",Mix_GetError());
		return -1;
	}
	return 0;
}
void PisteSound_SetMusicVolume(int volume){
	mus_volume = volume;
}
void PisteSound_StopMusic(){
	Mix_FadeOutMusic(1);
}

int PisteSound_Start(){
	if( Mix_OpenAudio(100, MIX_DEFAULT_FORMAT, 2, 4096) < 0){
		printf("Unable to init Mixer: %s\n", Mix_GetError());
		return -1;
	}

	Mix_Init(MIX_INIT_MOD);
	return 0;
}
int PisteSound_End(){
	PisteSound_ResetSFX();
	if(music != NULL) Mix_FreeMusic(music);
	Mix_CloseAudio();
	return 0;
}
