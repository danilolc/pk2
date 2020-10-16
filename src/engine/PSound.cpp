//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PSound.hpp"

#include "engine/PLog.hpp"
#include "engine/PUtils.hpp"

#include <SDL_mixer.h>

#include <cstring>

#define PS_FREQ        MIX_DEFAULT_FREQUENCY
#define PS_FORMAT      MIX_DEFAULT_FORMAT
#define PS_CHANNELS    MIX_DEFAULT_CHANNELS
#define PS_BUFFER_SIZE /*4096*/ /*2048*/ 1024 /*512*/ //game speed vs audio latency

namespace PSound {

const int MAX_SOUNDS = 300;

static int def_freq = 22050;

static int mus_volume = 100;
static int mus_volume_now = 100;


static Mix_Chunk* chunks[MAX_SOUNDS]; //The original chunks loaded
static Uint8* freq_chunks[CHANNELS];  //The chunk allocated for each channel

static Mix_Music* overlay_music = NULL;
static bool overlay_playing = false;

static Mix_Music* music = NULL;
static std::string playingMusic = "";

static int find_channel() {
	
	for( int channel = 0; channel < CHANNELS; channel++ )
		if( !Mix_Playing(channel) )
			return channel;

	return -1;

}

//Change the chunk frequency of sound index
static int change_frequency(int index, int channel, int freq) {

	u16 format;
	int channels;
	Mix_QuerySpec(NULL, &format, &channels);

	SDL_AudioCVT cvt;
	SDL_BuildAudioCVT(&cvt, format, channels, freq, format, channels, def_freq);

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
			rw->close();
			
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

int play_sfx(int index, int volume, int panoramic, int freq) {
	
	if(index == -1) return 1;
	if(chunks[index] == NULL) return 2;

	int channel = find_channel();
	if (channel == -1) {
	
		//PLog::Write(PLog::ERR, "PSound", "play_sfx got index -1");
		return -1;
	
	}

	//Save a backup of the parameter that will be ovewrited
	Uint8* bkp_buf = chunks[index]->abuf;
	Uint32 bkp_len = chunks[index]->alen;

	if (freq != def_freq) {

		int error = change_frequency(index, channel, freq);
		if (error != 0) {
			
			PLog::Write(PLog::ERR, "PSound", "Can't change frequency");
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

		PLog::Write(PLog::ERR, "PSound", "Can't play chunk");
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

int resume_music() {

	if (!overlay_playing) {

		return -1;

	}

	if (music == NULL) {

		PLog::Write(PLog::WARN, "PSound", "No music loaded");
		return -1;
	
	}
	if (Mix_PlayMusic(music, -1) == -1) {

		PLog::Write(PLog::ERR, "PSound", Mix_GetError());
		Mix_ClearError();
		return -1;
	
	}

	overlay_playing = false;

	return 0;
	
}

int play_overlay_music() {

	if (overlay_playing) {

		return -1;

	}

	if (overlay_music == NULL) {

		PLog::Write(PLog::WARN, "PSound", "No overlay music loaded");
		return -1;
	
	}

	Mix_HaltMusic();
	if (Mix_PlayMusic(overlay_music, -1) == -1) {

		PLog::Write(PLog::ERR, "PSound", Mix_GetError());
		Mix_ClearError();
		return -1;
	
	}

	overlay_playing = true;

	return 0;

}

int load_overlay_music(PFile::Path path) {

	if (overlay_music)
		Mix_FreeMusic(music);

	PFile::RW* rw = path.GetRW("rb");
	overlay_music = Mix_LoadMUS_RW((SDL_RWops*) rw, 0);
	rw->close();

	if (overlay_music == NULL) {

		PLog::Write(PLog::WARN, "PSound", Mix_GetError());
		Mix_ClearError();
		return -1;
	
	}

	PLog::Write(PLog::DEBUG, "PSound", "Loaded %s to overlay music", path.c_str());
	return 0;

}

int start_music(PFile::Path path) {

	if (playingMusic == path)
		return 1;
	
	Mix_HaltMusic();
	overlay_playing = false;

	if (music)
		Mix_FreeMusic(music);

	PFile::RW* rw = path.GetRW("rb");
	music = Mix_LoadMUS_RW((SDL_RWops*) rw, 0);
	rw->close();

	if (music == NULL) {

		PLog::Write(PLog::WARN, "PSound", Mix_GetError());
		Mix_ClearError();
		return -1;
	
	}
	if (Mix_PlayMusic(music, -1) == -1) {

		PLog::Write(PLog::ERR, "PSound", Mix_GetError());
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

	Mix_HaltMusic();
	overlay_playing = false;

}

void channelDone(int channel) {

	u8* pointer = freq_chunks[channel];
	freq_chunks[channel] = NULL;

    SDL_free(pointer);

}

int init() {

	if (Mix_OpenAudio(PS_FREQ, PS_FORMAT, PS_CHANNELS, PS_BUFFER_SIZE) < 0) {
	
		PLog::Write(PLog::FATAL, "PSound", "Unable to init Mixer: %s", Mix_GetError());
		return -1;

	}

	Mix_Init(MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_AllocateChannels(CHANNELS);
	Mix_ChannelFinished(channelDone);

	PLog::Write(PLog::DEBUG, "PSound", "Desired %ihz 0x%x", PS_FREQ, PS_FORMAT);

	int frequency;
	u16 format;
	Mix_QuerySpec(&frequency, &format, NULL);

	PLog::Write(PLog::DEBUG, "PSound", "Got %ihz 0x%x", frequency, format);

	overlay_playing = false;

	return 0;

}

int update() {

	if (mus_volume_now < mus_volume)
		Mix_VolumeMusic(++mus_volume_now * MIX_MAX_VOLUME / 100);

	else if (mus_volume_now > mus_volume)
		Mix_VolumeMusic(--mus_volume_now * MIX_MAX_VOLUME / 100);
	
	if (overlay_playing && !Mix_PlayingMusic())
		resume_music();

	return 0;

}

int terminate() {

	clear_channels();
	reset_sfx();
	
	if(music != NULL) Mix_FreeMusic(music);
	music = NULL;
	
	if(overlay_music != NULL) Mix_FreeMusic(music);
	overlay_music = NULL;

	Mix_CloseAudio();
	Mix_Quit();
	return 0;

}

}
