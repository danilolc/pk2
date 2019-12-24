
#include "settings.hpp"

#include "PisteInput.hpp"
#include "PisteUtils.hpp"

#include <SDL_rwops.h>
#include <cstring>

#define SETTINGS_PATH "data/settings.ini"
#define SETTINGS_VERSION "1.2"

PK2SETTINGS Settings;

void settings_init() {

	strcpy(Settings.versio, SETTINGS_VERSION);
	Settings.ladattu = false;

	strcpy(Settings.kieli, "english.txt");

	Settings.lapinakyvat_objektit = true;
	Settings.lapinakyvat_menutekstit = false;
	Settings.saa_efektit = true;
	Settings.nayta_tavarat = true;
	Settings.tausta_spritet = true;

	Settings.aanet = true;
	Settings.musiikki = true;

	Settings.control_left      = PI_LEFT;
	Settings.control_right     = PI_RIGHT;
	Settings.control_jump      = PI_UP;
	Settings.control_down      = PI_DOWN;
	Settings.control_walk_slow = PI_RALT;
	Settings.control_attack1   = PI_RCONTROL;
	Settings.control_attack2   = PI_RSHIFT;
	Settings.control_open_gift = PI_SPACE;

	Settings.isFiltered = true;
	Settings.isFit = true;
	Settings.isFullScreen = true;
	Settings.isWide = true;

	Settings.music_max_volume = 64;
	Settings.sfx_max_volume = 90;
}

int settings_open() {
	SDL_RWops *file = SDL_RWFromFile(SETTINGS_PATH, "rb");

	if (file == nullptr){
		settings_init();
		settings_save();
		return 1;
	}

	SDL_RWread(file, Settings.versio, 4, 1);
	
	if (strcmp(Settings.versio, SETTINGS_VERSION) != 0) { 
		// If settings isn't in current version
		settings_init();
        settings_save();
		return 2;
	}
	
	SDL_RWread(file, (char*)&Settings, sizeof(PK2SETTINGS), 1);
	
	Settings.ladattu = true;

	SDL_RWclose(file);
	
	return 0;
}

int settings_save() {
    PisteUtils_CreateDir("data");
	
	SDL_RWops *file = SDL_RWFromFile(SETTINGS_PATH, "wb");

	if (file == nullptr) {
		printf("Error saving settings\n");
		return 1;
	}
	
	SDL_RWwrite(file, SETTINGS_VERSION, 1, 4);
	SDL_RWwrite(file, (char*)&Settings, 1, sizeof(PK2SETTINGS));
	
	SDL_RWclose(file);

	return 0;
}