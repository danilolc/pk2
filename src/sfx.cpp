//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "sfx.hpp"

#include "system.hpp"
#include "settings.hpp"
#include "game/game.hpp"

#include "PisteSound.hpp"

int kytkin_aani;
int hyppy_aani;
int loiskahdus_aani;
int avaa_lukko_aani;
int menu_aani;
int ammuu_aani;
int kieku_aani;
int tomahdys_aani;
int pistelaskuri_aani;

int Load_SFX() {

    kytkin_aani = PSound::load_sfx("sfx/switch3.wav");
    if (kytkin_aani == -1)
        PK2_Error("Can't find switch3.wav");

    hyppy_aani = PSound::load_sfx("sfx/jump4.wav");
    if (hyppy_aani == -1)
        PK2_Error("Can't find jump4.wav");

    loiskahdus_aani = PSound::load_sfx("sfx/splash.wav");
    if (loiskahdus_aani == -1)
        PK2_Error("Can't find splash.wav");

    avaa_lukko_aani = PSound::load_sfx("sfx/openlock.wav");
    if (avaa_lukko_aani == -1) {
        PK2_Error("Can't find openlock.wav");
    }

    menu_aani = PSound::load_sfx("sfx/menu2.wav");
    if (menu_aani == -1)
        PK2_Error("Can't find menu2.wav");

    ammuu_aani = PSound::load_sfx("sfx/moo.wav");
    if (ammuu_aani == -1)
        PK2_Error("Can't find moo.wav");

    kieku_aani = PSound::load_sfx("sfx/doodle.wav");
    if (kieku_aani == -1)
        PK2_Error("Can't find doodle.wav");

    tomahdys_aani = PSound::load_sfx("sfx/pump.wav");
    if (tomahdys_aani == -1)
        PK2_Error("Can't find pump.wav");

    pistelaskuri_aani = PSound::load_sfx("sfx/counter.wav");
    if (pistelaskuri_aani == -1) {
        PK2_Error("Can't find counter.wav");
    }

}

void Play_GameSFX(int aani, int voimakkuus, int x, int y, int freq, bool random_freq){
	if (aani > -1 && Settings.sfx_max_volume > 0 && voimakkuus > 0){
		if (x < Game::camera_x + screen_width && x > Game::camera_x && y < Game::camera_y + screen_height && y > Game::camera_y){
			voimakkuus = voimakkuus / (100 / Settings.sfx_max_volume);

			if (voimakkuus > 100)
				voimakkuus = 100;

			if (voimakkuus < 0)
				voimakkuus = 0;

			int pan = Game::camera_x + (screen_width / 2) - x;
			pan *= -2;

			if (random_freq)
				freq = freq + rand()%4000 - rand()%2000;

			int err = PSound::play_sfx(aani, Settings.sfx_max_volume, pan, freq);
			if (err)
				printf("PK2     - Error playing sound. Error %i\n", err);
		}
	}
}

void Play_MenuSFX(int aani, int voimakkuus){
	if (aani > -1 && Settings.sfx_max_volume > 0 && voimakkuus > 0){
		voimakkuus = voimakkuus / (100 / Settings.sfx_max_volume);

		if (voimakkuus > 100)
			voimakkuus = 100;

		if (voimakkuus < 0)
			voimakkuus = 0;

		int freq = 22050 + rand()%5000 - rand()%5000;

		int err = PSound::play_sfx(aani, Settings.sfx_max_volume, 0, freq);
		if (err)
			printf("PK2     - Error playing sound. Error %i\n", err);
	}
}