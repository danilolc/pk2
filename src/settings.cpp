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

#include <ctime>
#include <cstring>
#include <string>

#include <SDL_timer.h>

#define SETTINGS_FILE "settings.ini"

PK2SETTINGS Settings;
GAME_CONTROLS* Input;

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

	u64 counter = SDL_GetPerformanceCounter();
	u32 counter32 = counter ^ (counter >> 32);
	counter32 ^= counter32 << 10;

	u32 timer = time(NULL);
	timer ^= timer << 1;
	
	u32 seed = counter32 ^ timer;

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

	Settings.fps = SETTINGS_60FPS;//SETTINGS_VSYNC;
	Settings.isFullScreen = true;
	Settings.double_speed = false;
	Settings.shader_type = SETTINGS_MODE_LINEAR;

	Settings.keyboard.left      = PInput::LEFT;
	Settings.keyboard.right     = PInput::RIGHT;
	Settings.keyboard.up        = PInput::UP;
	Settings.keyboard.down      = PInput::DOWN;
	Settings.keyboard.jump      = PInput::UP;
	Settings.keyboard.walk_slow = PInput::LALT;
	Settings.keyboard.attack1   = PInput::LCONTROL;
	Settings.keyboard.attack2   = PInput::LSHIFT;
	Settings.keyboard.open_gift = PInput::SPACE;

	Settings.using_controller   = NOT_SET;
	Settings.vibration          = 0xFFFF/2;
	Settings.joystick.left      = PInput::JOY_LEFT;
	Settings.joystick.right     = PInput::JOY_RIGHT;
	Settings.joystick.up        = PInput::JOY_UP;
	Settings.joystick.down      = PInput::JOY_DOWN;
	Settings.joystick.jump      = PInput::JOY_UP;
	Settings.joystick.walk_slow = PInput::JOY_Y;
	Settings.joystick.attack1   = PInput::JOY_A;
	Settings.joystick.attack2   = PInput::JOY_B;
	Settings.joystick.open_gift = PInput::JOY_LEFTSHOULDER;

	Settings.music_max_volume = 30;
	Settings.sfx_max_volume = 95;

	Settings.gui = true;

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
	
	file->read(Settings.keyboard.left);
	file->read(Settings.keyboard.right);
	file->read(Settings.keyboard.up);
	file->read(Settings.keyboard.down);
	file->read(Settings.keyboard.jump);
	file->read(Settings.keyboard.walk_slow);
	file->read(Settings.keyboard.attack1);
	file->read(Settings.keyboard.attack2);
	file->read(Settings.keyboard.open_gift);

	file->read(Settings.using_controller);
	file->read(Settings.vibration);
	file->read(Settings.joystick.left);
	file->read(Settings.joystick.right);
	file->read(Settings.joystick.up);
	file->read(Settings.joystick.down);
	file->read(Settings.joystick.jump);
	file->read(Settings.joystick.walk_slow);
	file->read(Settings.joystick.attack1);
	file->read(Settings.joystick.attack2);
	file->read(Settings.joystick.open_gift);

	file->read(Settings.music_max_volume);
	file->read(Settings.sfx_max_volume);

	file->read(Settings.gui);
	
	file->close();

	Id_To_String(Settings.id, id_code);

	if (Settings.shader_type == SETTINGS_MODE_CRT) {
		screen_width = 640;
		screen_height = 480;
	}

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
	
	file->write(Settings.keyboard.left);
	file->write(Settings.keyboard.right);
	file->write(Settings.keyboard.up);
	file->write(Settings.keyboard.down);
	file->write(Settings.keyboard.jump);
	file->write(Settings.keyboard.walk_slow);
	file->write(Settings.keyboard.attack1);
	file->write(Settings.keyboard.attack2);
	file->write(Settings.keyboard.open_gift);

	file->write(Settings.using_controller);
	file->write(Settings.vibration);
	file->write(Settings.joystick.left);
	file->write(Settings.joystick.right);
	file->write(Settings.keyboard.up);
	file->write(Settings.joystick.down);
	file->write(Settings.joystick.jump);
	file->write(Settings.joystick.walk_slow);
	file->write(Settings.joystick.attack1);
	file->write(Settings.joystick.attack2);
	file->write(Settings.joystick.open_gift);

	file->write(Settings.music_max_volume);
	file->write(Settings.sfx_max_volume);

	file->write(Settings.gui);
	
	file->close();
	
	PLog::Write(PLog::DEBUG, "PK2", "Saved settings");

	return 0;

}