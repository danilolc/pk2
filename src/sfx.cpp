//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "sfx.hpp"

#include "system.hpp"
#include "settings.hpp"
#include "game/game.hpp"

#include "engine/PLog.hpp"
#include "engine/PSound.hpp"

#include <cmath>

struct GameSFX {

    bool used;
    u32 x, y; // TODO - Update sprite position
    int volume;

};

GameSFX sfx_list[PSound::CHANNELS];

int switch_sound = -1;
int jump_sound = -1;
int splash_sound = -1;
int open_locks_sound = -1;
int menu_sound = -1;
int moo_sound = -1;
int doodle_sound = -1;
int pump_sound = -1;
int score_sound = -1;
int apple_sound = -1;

int Load_SFX() {
    
    PFile::Path path("sfx" PE_SEP);

    path.SetFile("switch3.wav");
    switch_sound = PSound::load_sfx(path);
    if (switch_sound == -1)
        PK2_Error("Can't find switch3.wav");

    path.SetFile("jump4.wav");
    jump_sound = PSound::load_sfx(path);
    if (jump_sound == -1)
        PK2_Error("Can't find jump4.wav");

    path.SetFile("splash.wav");
    splash_sound = PSound::load_sfx(path);
    if (splash_sound == -1)
        PK2_Error("Can't find splash.wav");

    path.SetFile("openlock.wav");
    open_locks_sound = PSound::load_sfx(path);
    if (open_locks_sound == -1) {
        PK2_Error("Can't find openlock.wav");
    }

    path.SetFile("menu2.wav");
    menu_sound = PSound::load_sfx(path);
    if (menu_sound == -1)
        PK2_Error("Can't find menu2.wav");

    path.SetFile("moo.wav");
    moo_sound = PSound::load_sfx(path);
    if (moo_sound == -1)
        PK2_Error("Can't find moo.wav");

    path.SetFile("doodle.wav");
    doodle_sound = PSound::load_sfx(path);
    if (doodle_sound == -1)
        PK2_Error("Can't find doodle.wav");

    path.SetFile("pump.wav");
    pump_sound = PSound::load_sfx(path);
    if (pump_sound == -1)
        PK2_Error("Can't find pump.wav");

    path.SetFile("counter.wav");
    score_sound = PSound::load_sfx(path);
    if (score_sound == -1) {
        PK2_Error("Can't find counter.wav");
    }

    path.SetFile("app_bite.wav");
    apple_sound = PSound::load_sfx(path);
    if (apple_sound == -1) {
        PK2_Error("Can't find app_bite.wav");
    }

    for (int i = 0; i < PSound::CHANNELS; i++)
        sfx_list[i].used = false;

    return 0;
}

int get_pan(int x, int y) {

    int pan = Game->camera_x - x + (screen_width / 2);
    return pan * 2;

}

int attenuate_volume(int volume, int x, int y) {

    const int max_dist = 50;

    float mult = 1;

    int sta_x = Game->camera_x;
    int sta_y = Game->camera_y;
    int end_x = Game->camera_x + screen_width;
    int end_y = Game->camera_y + screen_height;

    if (x < sta_x)
        mult /= float(sta_x - x + max_dist) / max_dist;

    if (x > end_x)
        mult /= float(x - end_x + max_dist) / max_dist;
    
    if (y < sta_y)
        mult /= float(sta_y - y + max_dist) / max_dist;
    
    if (y > end_y)
        mult /= float(y - end_y + max_dist) / max_dist;
    
    mult *= float(Settings.sfx_max_volume) / 100;

    if (mult < 0)
        mult = 0;
    if (mult > 1)
        mult = 1;

    return int(mult*mult*volume);

}

// Set panning and volume based on x, y and volume
void update_channel(int channel) {

    int x = sfx_list[channel].x;
    int y = sfx_list[channel].y;
    int volume = sfx_list[channel].volume;

    int pan = get_pan(x, y);
    int vol = attenuate_volume(volume, x, y);

    PSound::set_channel(channel, pan, vol);

}

void Update_GameSFX() {

    for (int i = 0; i < PSound::CHANNELS; i++)
        if (sfx_list[i].used) {
            if (!PSound::is_playing(i))
                sfx_list[i].used = false;
            else
                update_channel(i);
        }
    
}

void Play_GameSFX(int sound, int volume, int x, int y, int freq, bool random_freq){
	if (sound > -1 && Settings.sfx_max_volume > 0 && volume > 0) {

        if (random_freq)
			freq = freq + rand()%4000 - rand()%2000;

        int pan = get_pan(x, y);
        int vol = attenuate_volume(volume, x, y);

        int channel = PSound::play_sfx(sound, vol, pan, freq);
        if (channel == -1) {

            PLog::Write(PLog::ERR, "PK2", "Can't play sound");
            return;

        }

        sfx_list[channel].used = true;
        sfx_list[channel].x = x;
        sfx_list[channel].y = y;
        sfx_list[channel].volume = volume;
			
	}
}

void Play_MenuSFX(int sound, int volume){
	if (sound > -1 && Settings.sfx_max_volume > 0 && volume > 0){
		volume = volume / (100 / Settings.sfx_max_volume);

		if (volume > 100)
			volume = 100;

		if (volume < 0)
			volume = 0;

		int freq = 22050 + rand()%5000 - rand()%5000;

		int channel = PSound::play_sfx(sound, Settings.sfx_max_volume, 0, freq);
        
        if (channel == -1) {
        
            PLog::Write(PLog::ERR, "PK2", "Can't play menu sound");
            return;
        
        }
        
        sfx_list[channel].used = false;

	}

}