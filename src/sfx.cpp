//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "sfx.hpp"

#include "system.hpp"
#include "settings.hpp"
#include "game/game.hpp"

#include "engine/PSound.hpp"

int switch_sound;
int jump_sound;
int splash_sound;
int open_locks_sound;
int menu_sound;
int moo_sound;
int doodle_sound;
int pump_sound;
int score_sound;
int apple_sound;

int Load_SFX() {

    switch_sound = PSound::load_sfx("sfx/switch3.wav");
    if (switch_sound == -1)
        PK2_Error("Can't find switch3.wav");

    jump_sound = PSound::load_sfx("sfx/jump4.wav");
    if (jump_sound == -1)
        PK2_Error("Can't find jump4.wav");

    splash_sound = PSound::load_sfx("sfx/splash.wav");
    if (splash_sound == -1)
        PK2_Error("Can't find splash.wav");

    open_locks_sound = PSound::load_sfx("sfx/openlock.wav");
    if (open_locks_sound == -1) {
        PK2_Error("Can't find openlock.wav");
    }

    menu_sound = PSound::load_sfx("sfx/menu2.wav");
    if (menu_sound == -1)
        PK2_Error("Can't find menu2.wav");

    moo_sound = PSound::load_sfx("sfx/moo.wav");
    if (moo_sound == -1)
        PK2_Error("Can't find moo.wav");

    doodle_sound = PSound::load_sfx("sfx/doodle.wav");
    if (doodle_sound == -1)
        PK2_Error("Can't find doodle.wav");

    pump_sound = PSound::load_sfx("sfx/pump.wav");
    if (pump_sound == -1)
        PK2_Error("Can't find pump.wav");

    score_sound = PSound::load_sfx("sfx/counter.wav");
    if (score_sound == -1) {
        PK2_Error("Can't find counter.wav");
    }

    apple_sound = PSound::load_sfx("sfx/app_bite.wav");
    if (apple_sound == -1) {
        PK2_Error("Can't find app_bite.wav");
    }

    return 0;
}

void Play_GameSFX(int sound, int volume, int x, int y, int freq, bool random_freq){
	if (sound > -1 && Settings.sfx_max_volume > 0 && volume > 0) {

        const int max_dist = 50;

        int sta_x = Game->camera_x;
        int sta_y = Game->camera_y;
        int end_x = Game->camera_x + screen_width;
        int end_y = Game->camera_y + screen_height;

        if (x < sta_x)
            volume /= float(sta_x - x + max_dist) / (max_dist);

        if (x > end_x)
            volume /= float(x - end_x + max_dist) / (max_dist);
        
        if (y < sta_y)
            volume /= float(sta_y - y + max_dist) / (max_dist);
        
        if (y > end_y)
            volume /= float(y - end_y + max_dist) / (max_dist);
        
        volume *= float(Settings.sfx_max_volume) / 100;

        if (volume < 0)
			return;

        if (volume > 100)
			volume = 100;
        
		int pan = Game->camera_x - x + (screen_width / 2);
        pan *= 2;

        if (random_freq)
			freq = freq + rand()%4000 - rand()%2000;

        int channel = PSound::play_sfx(sound, volume, pan, freq);
		printf("PK2     - Sound on channel %i\n", channel);
			
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
		
        if (channel < 0)
            printf("PK2     - Error playing sound\n", channel);

	}

}