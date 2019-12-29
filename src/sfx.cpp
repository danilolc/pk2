//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "sfx.hpp"

#include "system.hpp"
#include "settings.hpp"
#include "game/game.hpp"

#include "PisteSound.hpp"

int switch_sound;
int jump_sound;
int splash_sound;
int open_locks_sound;
int menu_sound;
int moo_sound;
int doodle_sound;
int pump_sound;
int score_sound;

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

}

void Play_GameSFX(int sound, int volume, int x, int y, int freq, bool random_freq){
	if (sound > -1 && Settings.sfx_max_volume > 0 && volume > 0){
		if (x < Game->camera_x + screen_width && x > Game->camera_x && y < Game->camera_y + screen_height && y > Game->camera_y){
			volume = volume / (100 / Settings.sfx_max_volume);

			if (volume > 100)
				volume = 100;

			if (volume < 0)
				volume = 0;

			int pan = Game->camera_x + (screen_width / 2) - x;
			pan *= -2;

			if (random_freq)
				freq = freq + rand()%4000 - rand()%2000;

			int err = PSound::play_sfx(sound, Settings.sfx_max_volume, pan, freq);
			if (err)
				printf("PK2     - Error playing sound. Error %i\n", err);
		}
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

		int err = PSound::play_sfx(sound, Settings.sfx_max_volume, 0, freq);
		if (err)
			printf("PK2     - Error playing sound. Error %i\n", err);
	}
}