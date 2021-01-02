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
	SETTINGS_120FPS
};

enum {
	SETTINGS_SHADER_NEAREST,
	SETTINGS_SHADER_LINEAR,
	SETTINGS_SHADER_HQX,
	SETTINGS_SHADER_CRT
};

#define SETTINGS_VERSION "1.6"

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
	u32 control_left;
	u32 control_right;
	u32 control_jump;
	u32 control_down;
	u32 control_walk_slow;
	u32 control_attack1;
	u32 control_attack2;
	u32 control_open_gift;

	u16 vibration;
	u32 joy_left;
	u32 joy_right;
	u32 joy_jump;
	u32 joy_down;
	u32 joy_walk_slow;
	u32 joy_attack1;
	u32 joy_attack2;
	u32 joy_open_gift;
	
	// Audio
	u8  music_max_volume;
	u8  sfx_max_volume;

};

extern PK2SETTINGS Settings;

int Settings_GetId(PFile::Path path, u32& id);

int Settings_Open();
int Settings_Save();