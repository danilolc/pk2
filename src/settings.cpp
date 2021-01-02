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

PK2SETTINGS Settings;

int Settings_GetId(PFile::Path path, u32& id) {

	PFile::RW *file = path.GetRW("r");
	if (file == nullptr) {

		return 1;
		
	}

	char version[4];

	file->read(version, 4);
	if (strncmp(version, SETTINGS_VERSION, 4) != 0) {

		id = 0;
		file->close();
		return 2;

	}

	file->read(id);
	file->close();
	return 0;

}

void Settings_Init() {

	auto clockk = high_resolution_clock::now();
	uint ns = duration_cast<nanoseconds>(clockk.time_since_epoch()).count();

	uint timer = time(NULL);
	timer ^= timer << 1;
	
	uint seed = ns ^ timer;

	srand(seed);
	Settings.id = rand();
	Settings.id <<= 1;
	Settings.id ^= rand();

	strcpy(Settings.language, Language_Name());

	Settings.draw_transparent = true;
	Settings.transparent_text = false;
	Settings.draw_weather = true;
	Settings.draw_itembar = true;
	Settings.bg_sprites = true;

	Settings.fps = SETTINGS_VSYNC;
	Settings.isFullScreen = true;
	Settings.double_speed = false;
	Settings.shader_type = SETTINGS_SHADER_LINEAR;

	Settings.control_left      = PInput::LEFT;
	Settings.control_right     = PInput::RIGHT;
	Settings.control_jump      = PInput::UP;
	Settings.control_down      = PInput::DOWN;
	Settings.control_walk_slow = PInput::LALT;
	Settings.control_attack1   = PInput::LCONTROL;
	Settings.control_attack2   = PInput::LSHIFT;
	Settings.control_open_gift = PInput::SPACE;

	Settings.vibration     = 0xFFFF/2;
	Settings.joy_left      = PInput::JOY_LEFT;
	Settings.joy_right     = PInput::JOY_RIGHT;
	Settings.joy_jump      = PInput::JOY_UP;
	Settings.joy_down      = PInput::JOY_DOWN;
	Settings.joy_walk_slow = PInput::JOY_Y;
	Settings.joy_attack1   = PInput::JOY_X;
	Settings.joy_attack2   = PInput::JOY_A;
	Settings.joy_open_gift = PInput::JOY_B;

	Settings.music_max_volume = 50;
	Settings.sfx_max_volume = 90;

	Id_To_String(Settings.id, id_code);

}

int Settings_Open() {

	PFile::Path path(data_path, SETTINGS_FILE);

	PFile::RW* file = path.GetRW("r");

	if (file == nullptr) {
		Settings_Init();
		Settings_Save();
		return 1;
	}

	char version[4];
	file->read(version, 4);
	
	if (strncmp(version, SETTINGS_VERSION, 4) != 0) { 
		// If settings isn't in current version
		Settings_Init();
        Settings_Save();
		return 2;
	}

	file->read(Settings.id);
	file->read(Settings.language, sizeof(Settings.language));

	file->read(Settings.draw_transparent);
	file->read(Settings.transparent_text);
	file->read(Settings.draw_weather);
	file->read(Settings.draw_itembar);
	file->read(Settings.bg_sprites);
	
	file->read(Settings.fps);
	file->read(Settings.isFullScreen);
	file->read(Settings.double_speed);
	file->read(Settings.shader_type);
	
	file->read(Settings.control_left);
	file->read(Settings.control_right);
	file->read(Settings.control_jump);
	file->read(Settings.control_down);
	file->read(Settings.control_walk_slow);
	file->read(Settings.control_attack1);
	file->read(Settings.control_attack2);
	file->read(Settings.control_open_gift);

	file->read(Settings.vibration);
	file->read(Settings.joy_left);
	file->read(Settings.joy_right);
	file->read(Settings.joy_jump);
	file->read(Settings.joy_down);
	file->read(Settings.joy_walk_slow);
	file->read(Settings.joy_attack1);
	file->read(Settings.joy_attack2);
	file->read(Settings.joy_open_gift);

	file->read(Settings.music_max_volume);
	file->read(Settings.sfx_max_volume);
	
	file->close();

	Id_To_String(Settings.id, id_code);

	PLog::Write(PLog::DEBUG, "PK2", "Opened settings");
	
	return 0;

}

int Settings_Save() {

	PFile::Path path(data_path, SETTINGS_FILE);

	PFile::RW* file = path.GetRW("w");
	if (file == nullptr) {

		PLog::Write(PLog::ERR, "PK2", "Can't save settings");
		return 1;
	
	}
	
	// Save value by value, this defines the file structure
	file->write(SETTINGS_VERSION, sizeof(SETTINGS_VERSION));

	file->write(Settings.id);
	file->write(Settings.language, sizeof(Settings.language));

	file->write(Settings.draw_transparent);
	file->write(Settings.transparent_text);
	file->write(Settings.draw_weather);
	file->write(Settings.draw_itembar);
	file->write(Settings.bg_sprites);
	
	file->write(Settings.fps);
	file->write(Settings.isFullScreen);
	file->write(Settings.double_speed);
	file->write(Settings.shader_type);
	
	file->write(Settings.control_left);
	file->write(Settings.control_right);
	file->write(Settings.control_jump);
	file->write(Settings.control_down);
	file->write(Settings.control_walk_slow);
	file->write(Settings.control_attack1);
	file->write(Settings.control_attack2);
	file->write(Settings.control_open_gift);

	file->write(Settings.vibration);
	file->write(Settings.joy_left);
	file->write(Settings.joy_right);
	file->write(Settings.joy_jump);
	file->write(Settings.joy_down);
	file->write(Settings.joy_walk_slow);
	file->write(Settings.joy_attack1);
	file->write(Settings.joy_attack2);
	file->write(Settings.joy_open_gift);

	file->write(Settings.music_max_volume);
	file->write(Settings.sfx_max_volume);
	
	file->close();
	
	PLog::Write(PLog::DEBUG, "PK2", "Saved settings");

	return 0;

}