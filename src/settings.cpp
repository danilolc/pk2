//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "settings.hpp"

#include "language.hpp"
#include "system.hpp"

#include "engine/PLog.hpp"
#include "engine/PInput.hpp"
#include "engine/PUtils.hpp"

#include <SDL_rwops.h>

#include <cstring>
#include <string>

#define SETTINGS_FILE "settings.ini"
#define SETTINGS_VERSION "1.2"

PK2SETTINGS Settings;

void Settings_Init() {

	strcpy(Settings.versio, SETTINGS_VERSION);
	Settings.ladattu = false;

	strcpy(Settings.kieli, Language_Name());

	Settings.draw_transparent = true;
	Settings.transparent_text = false;
	Settings.draw_weather = true;
	Settings.draw_itembar = true;
	Settings.bg_sprites = true;

	Settings.aanet = true;
	Settings.musiikki = true;

	Settings.control_left      = PInput::LEFT;
	Settings.control_right     = PInput::RIGHT;
	Settings.control_jump      = PInput::UP;
	Settings.control_down      = PInput::DOWN;
	Settings.control_walk_slow = PInput::LALT;
	Settings.control_attack1   = PInput::LCONTROL;
	Settings.control_attack2   = PInput::LSHIFT;
	Settings.control_open_gift = PInput::SPACE;

	Settings.isFiltered = true;
	Settings.isFit = true;
	Settings.isFullScreen = true;
	Settings.isWide = true;

	Settings.music_max_volume = 35;
	Settings.sfx_max_volume = 100;

}

int Settings_Open() {

	std::string path(data_path + SETTINGS_FILE);

	SDL_RWops *file = SDL_RWFromFile(path.c_str(), "rb");

	if (file == nullptr){
		Settings_Init();
		Settings_Save();
		return 1;
	}

	SDL_RWread(file, Settings.versio, 4, 1);
	
	if (strncmp(Settings.versio, SETTINGS_VERSION, 4) != 0) { 
		// If settings isn't in current version
		Settings_Init();
        Settings_Save();
		return 2;
	}
	
	SDL_RWread(file, &Settings, sizeof(PK2SETTINGS), 1);
	
	if (PUtils::Is_Mobile()) { // TODO - don't change these settings on mobile

		Settings.isFiltered = true;
		Settings.isFit = true;
		//Settings.isFullScreen = true;
		Settings.isWide = true;

	}

	Settings.ladattu = true;

	SDL_RWclose(file);
	
	return 0;

}

int Settings_Save() {

	std::string path(data_path + SETTINGS_FILE);

	SDL_RWops *file = SDL_RWFromFile(path.c_str(), "wb");

	if (file == nullptr) {

		PLog::Write(PLog::ERROR, "PK2", "Can't save settings");
		return 1;
	
	}
	
	SDL_RWwrite(file, SETTINGS_VERSION, 1, 4);
	SDL_RWwrite(file, (char*)&Settings, 1, sizeof(PK2SETTINGS));
	
	SDL_RWclose(file);

	return 0;

}