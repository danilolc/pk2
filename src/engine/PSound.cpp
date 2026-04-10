//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "PSound.hpp"

#include "PLog.hpp"

#include <SDL_mixer.h>

#include <cstring>
#include <queue>

#define PS_FREQ        MIX_DEFAULT_FREQUENCY
#define PS_FORMAT      MIX_DEFAULT_FORMAT
#define PS_CHANNELS    MIX_DEFAULT_CHANNELS

namespace PSound {

const int MAX_SOUNDS = 300;

static int def_freq = 22050;

static u8 mus_volume = 100;
static float mus_volume_now = 100;

static Mix_Chunk* chunks[MAX_SOUNDS]; //The original chunks loaded
static Uint8* freq_chunks[CHANNELS];  //The chunk allocated for each channel

static bool channel_playing[CHANNELS];

static Mix_Music* music = NULL;
static PFile::RW* music_rw = NULL;
static PFile::Path playingMusic = PFile::Path("");

struct audio_data_t {

	//index = -1 -> clean channel
	int index, channel, volume, panoramic, freq;

};

static int find_channel() {
	
	for( int channel = 0; channel < CHANNELS; channel++ )
		if( !channel_playing[channel] )
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

bool is_playing(int channel) {

	return channel_playing[channel];

}

int load_sfx(PFile::Path path) {

	int ret = -1;

	for( int i = 0; i < MAX_SOUNDS; i++ )
		if (chunks[i] == NULL) {

			try{
				PFile::RW rw = path.GetRW2("r");
				chunks[i] = Mix_LoadWAV_RW((SDL_RWops*)(rw._rwops), 0);
				rw.close();

				ret = i;
			}
			catch(const PFile::PFileException& e){
				PLog::Write(PLog::ERR, "PSound", e.what());
				PLog::Write(PLog::ERR, "PSound", "Couldn't open %s", path.c_str());
			}
			break;
		}
		
	return ret;
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


int change_frequency_and_play(int index, int channel, int freq) {
	
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

	//PLog::Write(PLog::DEBUG, "PSound", "Playing channel %i with frequency %i", channel, freq);

	int error = Mix_PlayChannel(channel, chunks[index], 0);
	chunks[index]->abuf = bkp_buf;
	chunks[index]->alen = bkp_len;

	return error;

}

int play_sfx(int index, int volume, int panoramic, int freq) {

	if(index == -1) return 1;
	if(chunks[index] == NULL) return 2;

	int channel = find_channel();
	if (channel == -1) {
	
		//PLog::Write(PLog::WARN, "PSound", "No free channels available");
		return -1;
	
	}
	
	channel_playing[channel] = true;

	set_channel(channel, panoramic, volume);


	int error = change_frequency_and_play(index, channel, freq);

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

	if (music == nullptr) {
		static bool warned = false;
		if (!warned) {
			PLog::Write(PLog::WARN, "PSound", "No music loaded");
			warned = true;
		}
		return -1;
	
	}
	if (Mix_PlayMusic(music, -1) == -1) {

		PLog::Write(PLog::ERR, "PSound", Mix_GetError());
		Mix_ClearError();
		return -1;
	
	}
	return 0;
	
}


int start_music(PFile::Path path) {

	if (playingMusic == path)
		return 1;
	
	Mix_HaltMusic();
	if (music!=nullptr) {
		Mix_FreeMusic(music);
		music = nullptr;
	}

	if(music_rw!=nullptr){
		delete music_rw;
		music_rw = nullptr;
	}
	playingMusic = path;

	try{
		music_rw = new PFile::RW(path.GetRW2("r"));
		music = Mix_LoadMUS_RW((SDL_RWops*)(music_rw->_rwops), 0);
	}
	catch(const PFile::PFileException& e){
		PLog::Write(PLog::ERR, "PSound", e.what());
		PLog::Write(PLog::ERR, "PSound", "Unable to load music \"%s\"",path.c_str());
	}
	
	if (music == nullptr) {

		PLog::Write(PLog::WARN, "PSound", Mix_GetError());
		Mix_ClearError();

		delete music_rw;
		music_rw = nullptr;

		return -1;
	
	}
	if (Mix_PlayMusic(music, -1) == -1) {

		PLog::Write(PLog::ERR, "PSound", Mix_GetError());
		Mix_ClearError();

		Mix_FreeMusic(music);
		music = nullptr;
		delete music_rw;
		music_rw = nullptr;

		return -1;
	
	}

	Mix_VolumeMusic(mus_volume_now * MIX_MAX_VOLUME / 100);
	
	PLog::Write(PLog::DEBUG, "PSound", "Loaded %s", path.c_str());
	return 0;
	
}

void set_musicvolume(u8 volume) {

	mus_volume = volume;

}

void set_musicvolume_now(u8 volume) {

	mus_volume = volume;
	mus_volume_now = volume;
	Mix_VolumeMusic(float(volume) * MIX_MAX_VOLUME / 100);
	
}

void stop_music(){

	clear_channels();
	Mix_HaltMusic();
	playingMusic = PFile::Path("");

}

void channelDone(int channel) {
	u8* pointer = freq_chunks[channel];
	freq_chunks[channel] = NULL;
    SDL_free(pointer);
	channel_playing[channel] = false;

}


int init(int buffer_size) {

	if (Mix_OpenAudio(PS_FREQ, PS_FORMAT, PS_CHANNELS, buffer_size) < 0) {
	
		PLog::Write(PLog::FATAL, "PSound", "Unable to init Mixer: %s", Mix_GetError());
		return -1;

	}

	for (int i = 0; i < CHANNELS; i++) {
		freq_chunks[i] = NULL;
		channel_playing[i] = false;
	}

	Mix_Init(MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_AllocateChannels(CHANNELS);
	Mix_ChannelFinished(channelDone);

	PLog::Write(PLog::DEBUG, "PSound", "buffer size: %i", buffer_size);
	PLog::Write(PLog::DEBUG, "PSound", "Desired %ihz 0x%x", PS_FREQ, PS_FORMAT);
	PLog::Write(PLog::DEBUG, "PSound", "Audio driver: %s", SDL_GetCurrentAudioDriver());

	int frequency;
	u16 format;
	Mix_QuerySpec(&frequency, &format, NULL);

	PLog::Write(PLog::DEBUG, "PSound", "Got %ihz 0x%x", frequency, format);

	return 0;

}

int update() {

	const float VOLUME_DELAY = 19.f;

	mus_volume_now = (VOLUME_DELAY * mus_volume_now + mus_volume) / (VOLUME_DELAY + 1);

	int volume = mus_volume_now * MIX_MAX_VOLUME / 100;

	if (volume != Mix_VolumeMusic(-1))
		Mix_VolumeMusic(volume);
	
	if (!Mix_PlayingMusic())
		resume_music();

	return 0;

}

int terminate() {

	clear_channels();
	reset_sfx();
	
	if(music != nullptr) {
		Mix_FreeMusic(music);
		music = nullptr;
	}

	if(music_rw != nullptr){
		delete music_rw;
		music_rw = nullptr;
	}

	Mix_CloseAudio();
	Mix_Quit();
	return 0;

}

}
