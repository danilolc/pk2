//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PSound.hpp"

#include "engine/PLog.hpp"
#include "engine/PUtils.hpp"

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

static Mix_Music* overlay_music = NULL;
static PFile::RW* overlay_rw = NULL;
static bool overlay_playing = false;

static Mix_Music* music = NULL;
static PFile::RW* music_rw = NULL;
static PFile::Path playingMusic = PFile::Path("");

struct audio_data_t {

	//index = -1 -> clean channel
	int index, channel, volume, panoramic, freq;

};

static SDL_Thread* queue_thread = NULL;
static SDL_sem* semafore = NULL;
static SDL_mutex* queue_mutex = NULL;
static SDL_mutex* chunks_mutex = NULL;
static SDL_atomic_t thread_done;
static std::queue<audio_data_t> sound_queue;

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

	SDL_LockMutex(chunks_mutex);

	for( int i = 0; i < MAX_SOUNDS; i++ )
		if (chunks[i] == NULL) {

			PFile::RW* rw = path.GetRW("r");
			if (rw == nullptr) {

				PLog::Write(PLog::ERR, "PSound", "Couldn't open %s", path.c_str());
				break;

			} else {

				chunks[i] = Mix_LoadWAV_RW((SDL_RWops*)rw, 0);
				rw->close();
			
				ret = i;

			}

			break;

		}
	
	SDL_UnlockMutex(chunks_mutex);
	
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
	
	SDL_LockMutex(chunks_mutex);

	//Save a backup of the parameter that will be ovewrited
	Uint8* bkp_buf = chunks[index]->abuf;
	Uint32 bkp_len = chunks[index]->alen;

	if (freq != def_freq) {

		int error = change_frequency(index, channel, freq);
		if (error != 0) {
			
			PLog::Write(PLog::ERR, "PSound", "Can't change frequency");
			chunks[index]->abuf = bkp_buf;
			chunks[index]->alen = bkp_len;
			SDL_UnlockMutex(chunks_mutex);

			return -1;

		}
		
	}

	//PLog::Write(PLog::DEBUG, "PSound", "Playing channel %i with frequency %i", channel, freq);

	int error = Mix_PlayChannel(channel, chunks[index], 0);
	chunks[index]->abuf = bkp_buf;
	chunks[index]->alen = bkp_len;

	SDL_UnlockMutex(chunks_mutex);

	return error;

}

static int queue_function(void* data) {

	while (!SDL_AtomicGet(&thread_done)) {

        SDL_SemWait(semafore);

		while (1) {

			SDL_LockMutex(queue_mutex);

			if (sound_queue.size() == 0) {

				SDL_UnlockMutex(queue_mutex);
				break;

			}

			audio_data_t audio_data = sound_queue.front();
			sound_queue.pop();

			SDL_UnlockMutex(queue_mutex);

			if (audio_data.index == -1) {

				u8* pointer = freq_chunks[audio_data.channel];
				freq_chunks[audio_data.channel] = NULL;
				SDL_free(pointer);
				channel_playing[audio_data.channel] = false;
				continue;

			}

			int error = change_frequency_and_play(audio_data.index, audio_data.channel, audio_data.freq);
			if (error == -1) {

				PLog::Write(PLog::ERR, "PSound", "Can't play chunk");

			}

		}
    
	}

	return 0;

}


static void send_queue(int index, int channel, int volume, int panoramic, int freq) {

	audio_data_t audio_data;
	audio_data.index = index;
	audio_data.channel = channel;
	audio_data.volume = volume;
	audio_data.panoramic = panoramic;
	audio_data.freq = freq;

	SDL_LockMutex(queue_mutex);
	sound_queue.push(audio_data);
	SDL_UnlockMutex(queue_mutex);

}

int play_sfx(int index, int volume, int panoramic, int freq) {

	if(index == -1) return 1;
	if(chunks[index] == NULL) return 2;

	int channel = find_channel();
	if (channel == -1) {
	
		PLog::Write(PLog::WARN, "PSound", "No free channels available");
		return -1;
	
	}
	
	channel_playing[channel] = true;

	set_channel(channel, panoramic, volume);

	if (queue_thread != NULL) {

		send_queue(index, channel, volume, panoramic, freq);
		return channel;

	}

	int error = change_frequency_and_play(index, channel, freq);

	if (error == -1) {

		PLog::Write(PLog::ERR, "PSound", "Can't play chunk");
		return -1;

	}

	return channel;

}

int free_sfx(int index) {

	SDL_LockMutex(chunks_mutex);

	if(chunks[index] != NULL) {
		
		Mix_FreeChunk(chunks[index]);
		chunks[index] = NULL;
	
	}

	SDL_UnlockMutex(chunks_mutex);

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

int load_overlay_music(PFile::Path path) { //TODO - load ovarlay from zip

	if (overlay_playing)
		resume_music();

	if (overlay_music) {
		Mix_FreeMusic(overlay_music);
		overlay_rw->close();
	}

	overlay_rw = path.GetRW("r");
	overlay_music = Mix_LoadMUS_RW((SDL_RWops*) overlay_rw, 0);

	if (overlay_music == NULL) {

		PLog::Write(PLog::WARN, "PSound", Mix_GetError());
		Mix_ClearError();
		overlay_rw->close();

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

	if (music) {
		Mix_FreeMusic(music);
		music_rw->close();
	}

	playingMusic = path;

	music_rw = path.GetRW("r");
	music = Mix_LoadMUS_RW((SDL_RWops*) music_rw, 0);
	
	if (music == NULL) {

		PLog::Write(PLog::WARN, "PSound", Mix_GetError());
		Mix_ClearError();
		music_rw->close();

		return -1;
	
	}
	if (Mix_PlayMusic(music, -1) == -1) {

		PLog::Write(PLog::ERR, "PSound", Mix_GetError());
		Mix_ClearError();

		Mix_FreeMusic(music);
		music = NULL;
		music_rw->close();

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

	Mix_HaltMusic();
	overlay_playing = false;
	playingMusic = PFile::Path("");

}

void channelDone(int channel) {

	if (queue_thread) {

		audio_data_t audio_data;
		audio_data.index = -1;
		audio_data.channel = channel;

		SDL_LockMutex(queue_mutex);
		sound_queue.push(audio_data);
		SDL_UnlockMutex(queue_mutex);

		return;

	}

	u8* pointer = freq_chunks[channel];
	freq_chunks[channel] = NULL;
    SDL_free(pointer);
	channel_playing[channel] = false;

}


int init(int buffer_size, bool multi_thread) {

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

	overlay_playing = false;

	if (multi_thread) {

		SDL_AtomicSet(&thread_done, 0);
		semafore = SDL_CreateSemaphore(0);
		queue_mutex = SDL_CreateMutex();
		chunks_mutex = SDL_CreateMutex();
		queue_thread = SDL_CreateThread(queue_function, "Audio Thread", NULL);

	}

	return 0;

}

int update() {

	const float VOLUME_DELAY = 19.f;

	mus_volume_now = (VOLUME_DELAY * mus_volume_now + mus_volume) / (VOLUME_DELAY + 1);

	int volume = mus_volume_now * MIX_MAX_VOLUME / 100;

	if (volume != Mix_VolumeMusic(-1))
		Mix_VolumeMusic(volume);
	
	if (overlay_playing && !Mix_PlayingMusic())
		resume_music();

	SDL_LockMutex(queue_mutex);
	int size = sound_queue.size();
	SDL_UnlockMutex(queue_mutex);
	if (size > 0)
		SDL_SemPost(semafore);

	return 0;

}

int terminate() {

	clear_channels();
	reset_sfx();
	
	if(music != NULL) {
		Mix_FreeMusic(music);
		music_rw->close();
	}
	music = NULL;
	music_rw = NULL;

	if(overlay_music != NULL) {
		Mix_FreeMusic(overlay_music);
		overlay_rw->close();
	}
	overlay_music = NULL;
	overlay_rw = NULL;

	if (queue_thread != NULL) {

		SDL_AtomicSet(&thread_done, 1);
		SDL_SemPost(semafore);
		SDL_WaitThread(queue_thread, NULL);
		SDL_DestroySemaphore(semafore);
		SDL_DestroyMutex(queue_mutex);
		SDL_DestroyMutex(chunks_mutex);

	}

	Mix_CloseAudio();
	Mix_Quit();
	return 0;

}

}
