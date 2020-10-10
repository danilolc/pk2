//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "settings.hpp"

#include "language.hpp"
#include "system.hpp"

#include "engine/PLog.hpp"
#include "engine/PInput.hpp"
#include "engine/PUtils.hpp"

#include <cstring>
#include <string>
#include <chrono>

using namespace std::chrono;

#define SETTINGS_FILE "settings.ini"
#define SETTINGS_VERSION "1.3"

PK2SETTINGS Settings;

int Settings_GetId(PFile::Path path, u32 id) {

	char* version[4];

	PFile::RW *rw = path.GetRW("rb");
	if (rw == nullptr) {

		return 1;
		
	}

	PFile::ReadRW(rw, version, 4);
	if (strncmp(Settings.versio, SETTINGS_VERSION, 4) != 0) {

		id = 0;
		return 2;

	}

	PFile::ReadRW(rw, id);
	return 0;

}

void Settings_Init() {

	strcpy(Settings.versio, SETTINGS_VERSION);
	Settings.ladattu = false;

	auto clockk = high_resolution_clock::now();
	uint ns = duration_cast<nanoseconds>(clockk.time_since_epoch()).count();

	uint timer = time(NULL);
	timer ^= timer << 1;
	
	uint seed = ns ^ timer;

	srand(seed);
	Settings.id = rand();
	Settings.id <<= 1;
	Settings.id ^= rand();

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

	Settings.music_max_volume = 50;
	Settings.sfx_max_volume = 90;

	Id_To_String(Settings.id, id_code);

}

int Settings_Open() {

	PFile::Path path(data_path, SETTINGS_FILE);

	PFile::RW* file = path.GetRW("rb");

	if (file == nullptr) {
		Settings_Init();
		Settings_Save();
		return 1;
	}

	PFile::ReadRW(file, Settings.versio, 4);
	
	if (strncmp(Settings.versio, SETTINGS_VERSION, 4) != 0) { 
		// If settings isn't in current version
		Settings_Init();
        Settings_Save();
		return 2;
	}
	
	PFile::ReadRW(file, Settings.id);
	PFile::ReadRW(file, Settings.ladattu);
	PFile::ReadRW(file, Settings.kieli, sizeof(Settings.kieli));

	PFile::ReadRW(file, Settings.ruudun_leveys);
	PFile::ReadRW(file, Settings.ruudun_korkeus);
	PFile::ReadRW(file, Settings.draw_transparent);
	PFile::ReadRW(file, Settings.transparent_text);
	PFile::ReadRW(file, Settings.draw_weather);
	PFile::ReadRW(file, Settings.draw_itembar);
	PFile::ReadRW(file, Settings.bg_sprites);

	PFile::ReadRW(file, Settings.control_left);
	PFile::ReadRW(file, Settings.control_right);
	PFile::ReadRW(file, Settings.control_jump);
	PFile::ReadRW(file, Settings.control_down);
	PFile::ReadRW(file, Settings.control_walk_slow);
	PFile::ReadRW(file, Settings.control_attack1);
	PFile::ReadRW(file, Settings.control_attack2);
	PFile::ReadRW(file, Settings.control_open_gift);

	PFile::ReadRW(file, Settings.musiikki);
	PFile::ReadRW(file, Settings.aanet);

	PFile::ReadRW(file, Settings.isFullScreen);
	PFile::ReadRW(file, Settings.isFiltered);
	PFile::ReadRW(file, Settings.isFit);
	PFile::ReadRW(file, Settings.isWide);

	PFile::ReadRW(file, Settings.music_max_volume);
	PFile::ReadRW(file, Settings.sfx_max_volume);
	
	PFile::CloseRW(file);
	
	if (PUtils::Is_Mobile()) {

		Settings.isFullScreen = false;
		//Settings.isFullScreen = true; //TODO
		Settings.isWide = true;

	}

	Settings.ladattu = true;
	Id_To_String(Settings.id, id_code);
	
	return 0;

}

int Settings_Save() {

	PFile::Path path(data_path, SETTINGS_FILE);

	PFile::RW *file = path.GetRW("wb");

	if (file == nullptr) {

		PLog::Write(PLog::ERR, "PK2", "Can't save settings");
		return 1;
	
	}
	
	// Save value by value, this defines the file structure
	PFile::WriteRW(file, SETTINGS_VERSION, sizeof(SETTINGS_VERSION));

	PFile::WriteRW(file, Settings.id);
	PFile::WriteRW(file, Settings.ladattu);
	PFile::WriteRW(file, Settings.kieli, sizeof(Settings.kieli));

	PFile::WriteRW(file, Settings.ruudun_leveys);
	PFile::WriteRW(file, Settings.ruudun_korkeus);
	PFile::WriteRW(file, Settings.draw_transparent);
	PFile::WriteRW(file, Settings.transparent_text);
	PFile::WriteRW(file, Settings.draw_weather);
	PFile::WriteRW(file, Settings.draw_itembar);
	PFile::WriteRW(file, Settings.bg_sprites);

	PFile::WriteRW(file, Settings.control_left);
	PFile::WriteRW(file, Settings.control_right);
	PFile::WriteRW(file, Settings.control_jump);
	PFile::WriteRW(file, Settings.control_down);
	PFile::WriteRW(file, Settings.control_walk_slow);
	PFile::WriteRW(file, Settings.control_attack1);
	PFile::WriteRW(file, Settings.control_attack2);
	PFile::WriteRW(file, Settings.control_open_gift);

	PFile::WriteRW(file, Settings.musiikki);
	PFile::WriteRW(file, Settings.aanet);

	PFile::WriteRW(file, Settings.isFullScreen);
	PFile::WriteRW(file, Settings.isFiltered);
	PFile::WriteRW(file, Settings.isFit);
	PFile::WriteRW(file, Settings.isWide);

	PFile::WriteRW(file, Settings.music_max_volume);
	PFile::WriteRW(file, Settings.sfx_max_volume);
	
	PFile::CloseRW(file);

	return 0;

}