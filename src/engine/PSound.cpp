//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "engine/PSound.hpp"

#include "engine/PLog.hpp"
#include "engine/PUtils.hpp"

#include <SDL_mixer.h>

#include <cstring>

#define AUDIO_FREQ 44100

namespace PSound {

const int MAX_SOUNDS = 300;

static int def_freq = 22050;

static int mus_volume = 100;
static int mus_volume_now = 100;

static Mix_Chunk* chunks[MAX_SOUNDS]; //The original chunks loaded
static Uint8* freq_chunks[CHANNELS];  //The chunk allocated for each channel

static Mix_Music* music = NULL;
static std::string playingMusic = "";

int find_channel() {
	
	for( int channel = 0; channel < CHANNELS; channel++ )
		if( !Mix_Playing(channel) )
			return channel;

	return -1;

}

//Change the chunk frequency of sound index
int Change_Frequency(int index, int channel, int freq) {

	SDL_AudioCVT cvt;
	SDL_BuildAudioCVT(&cvt, MIX_DEFAULT_FORMAT, 2, freq, MIX_DEFAULT_FORMAT, 2, def_freq);

	if (cvt.needed) {

		cvt.len = chunks[index]->alen;
		cvt.buf = (Uint8*)SDL_malloc(cvt.len * cvt.len_mult);
		if (cvt.buf == NULL)
			return -2;

		SDL_memcpy(cvt.buf, chunks[index]->abuf, chunks[index]->alen);
		if(SDL_ConvertAudio(&cvt) < 0) {

			SDL_free(cvt.buf);
			return -3;
		
		}

		chunks[index]->abuf = cvt.buf;
		chunks[index]->alen = cvt.len_cvt;

		//Save the buffer to delete it after play
		if(freq_chunks[channel] != NULL)
			SDL_free( freq_chunks[channel] );
		freq_chunks[channel] = cvt.buf; 

	}

	return 0;

}

int load_sfx(PFile::Path path) {

	for( int i = 0; i < MAX_SOUNDS; i++ )
		if (chunks[i] == NULL) {

			PFile::RW* rw = path.GetRW("rb");
			chunks[i] = Mix_LoadWAV_RW((SDL_RWops*)rw, 0);
			PFile::CloseRW(rw);
			
			return i;

		}
	
	return -1;
}

//panoramic from -1000 to 1000
//volume from 0 to 100
int set_channel(int channel, int panoramic, int volume) {

	volume = volume * MIX_MAX_VOLUME / 100;

	int pan_left = (panoramic + 1000) * 255 / 2000;
	int pan_right = 254 - pan_left;

	if (pan_left < 0) pan_left = 0;
	if (pan_left > 255) pan_left = 255;
	if (pan_right < 0) pan_right = 0;
	if (pan_right > 255) pan_right = 255;

	Mix_SetPanning(channel, pan_left, pan_right);
	Mix_Volume(channel, volume);

	return 0;
	
}

int play_sfx(int index, int volume, int panoramic, int freq){
	
	if(index == -1) return 1;
	if(chunks[index] == NULL) return 2;

	int channel = find_channel();
	if (channel == -1) {
	
		PLog::Write(PLog::ERROR, "PSound", "play_sfx got index -1");
		return -1;
	
	}

	//Save a backup of the parameter that will be ovewrited
	Uint8* bkp_buf = chunks[index]->abuf;
	Uint32 bkp_len = chunks[index]->alen;

	if (freq != def_freq) {

		int error = Change_Frequency(index, channel, freq);
		if (error != 0) {
			
			PLog::Write(PLog::ERROR, "PSound", "Can't change frequency");
			chunks[index]->abuf = bkp_buf;
			chunks[index]->alen = bkp_len;
			return -1;

		}
		
	}

	set_channel(channel, panoramic, volume);

	//PLog::Write(PLog::DEBUG, "PSound", "Playing channel %i with frequency %i", channel, freq);

	int error = Mix_PlayChannel(channel, chunks[index], 0);
	chunks[index]->abuf = bkp_buf;
	chunks[index]->alen = bkp_len;

	if (error == -1) {

		PLog::Write(PLog::ERROR, "PSound", "Can't play chunk");
		return -1;

	}

	return channel;

}

int free_sfx(int index) {

	if(chunks[index] != NULL) {
		
		Mix_FreeChunk(chunks[index]);
		chunks[index] = NULL;
	
	}

	return 0;

}

void reset_sfx() {

	for(int i = 0; i < MAX_SOUNDS; i++)
		free_sfx(i);

}

void clear_channels() {

	Mix_HaltChannel(-1);

}

int start_music(PFile::Path path) {

	if (playingMusic == path)
		return 1;

	PFile::RW* rw = path.GetRW("rb");
	music = Mix_LoadMUS_RW((SDL_RWops*) rw, 0);
	PFile::CloseRW(rw);

	if (music == NULL) {

		PLog::Write(PLog::WARN, "PSound", Mix_GetError());
		Mix_ClearError();
		return -1;
	
	}
	if (Mix_PlayMusic(music, -1) == -1) {

		PLog::Write(PLog::ERROR, "PSound", Mix_GetError());
		Mix_ClearError();
		return -1;
	
	}

	Mix_VolumeMusic(mus_volume_now * MIX_MAX_VOLUME / 100);
	
	PLog::Write(PLog::DEBUG, "PSound", "Loaded %s", path.c_str());
	playingMusic = path;

	return 0;
	
}

void set_musicvolume(int volume) {

	mus_volume = volume;

}

void set_musicvolume_now(int volume) {

	mus_volume = volume;
	mus_volume_now = volume;
	Mix_VolumeMusic(volume * MIX_MAX_VOLUME / 100);
	
}
void stop_music(){

	Mix_FadeOutMusic(0);

}

void channelDone(int channel) {

	Uint8* pointer = freq_chunks[channel];
	freq_chunks[channel] = NULL;

    SDL_free(pointer);

}

int init() {

	//chunksize - game speed vs audio latency
	if(Mix_OpenAudio(AUDIO_FREQ, MIX_DEFAULT_FORMAT, 2, /*4096*/ 2048/*1024*//*512*/) < 0) {

		PLog::Write(PLog::FATAL, "PSound", "Unable to init Mixer: %s", Mix_GetError());
		return -1;

	}

	Mix_Init(MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_AllocateChannels(CHANNELS);
	Mix_ChannelFinished(channelDone);

	return 0;

}

int update() {

	if (mus_volume_now < mus_volume)
		Mix_VolumeMusic(++mus_volume_now * MIX_MAX_VOLUME / 100);

	else if (mus_volume_now > mus_volume)
		Mix_VolumeMusic(--mus_volume_now * MIX_MAX_VOLUME / 100);

	return 0;

}

int terminate() {

	clear_channels();
	reset_sfx();
	
	if(music != NULL) Mix_FreeMusic(music);
	music = NULL;
	
	Mix_CloseAudio();
	return 0;

}

}
