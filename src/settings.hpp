//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/platform.hpp"

#include "engine/PFile.hpp"

enum {
	SETTINGS_VSYNC,
	SETTINGS_60FPS,
	SETTINGS_85FPS,
	SETTINGS_120FPS,
	SETTINGS_30FPS
};

enum {
	SETTINGS_MODE_NEAREST,
	SETTINGS_MODE_LINEAR,
	SETTINGS_MODE_CRT,
	SETTINGS_MODE_HQX
};

enum {
	SET_FALSE,
	SET_TRUE,
	NOT_SET,
};

#define SETTINGS_VERSION "1.8"

struct GAME_CONTROLS { 

	u32 left;
	u32 right;
	u32 up;
	u32 down;

	u32 jump;
	u32 walk_slow;
	u32 attack1;
	u32 attack2;
	u32 open_gift;

};

struct PK2SETTINGS {
	
	u32  id;
	char language[PE_PATH_SIZE];

	// Graphips
	bool  draw_transparent;
	bool  transparent_text;
	bool  draw_weather;
	bool  draw_itembar;
	bool  bg_sprites;
	
	s32   fps;
	bool  isFullScreen;
	bool  double_speed;
	u8    shader_type;

	// Controls
	GAME_CONTROLS keyboard;
	GAME_CONTROLS joystick;
	u8 using_controller;
	u16 vibration;
	
	// Audio
	u8  music_max_volume;
	u8  sfx_max_volume;

	// GUI
	bool gui;

};

extern PK2SETTINGS Settings;
extern GAME_CONTROLS* Input;

int Settings_GetId(PFile::Path path, u32& id);

int Settings_Open();
int Settings_Save();