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

struct PK2SETTINGS {
	
	char versio[4];
	u32  id;
	bool ladattu; // if it was started here
	char kieli[PE_PATH_SIZE]; // language

	// grafiikka
	u32 ruudun_korkeus; //Not used
	u32 ruudun_leveys; //Not used
	bool  draw_transparent;
	bool  transparent_text;
	bool  draw_weather;
	bool  draw_itembar;
	bool  bg_sprites;

	// kontrollit
	u32 control_left;
	u32 control_right;
	u32 control_jump;
	u32 control_down;
	u32 control_walk_slow;
	u32 control_attack1;
	u32 control_attack2;
	u32 control_open_gift;

	// audio
	bool musiikki; //Not used
	bool aanet; //Not used

	//Version 1.1
	bool isFullScreen;
	bool isFiltered;
	bool isFit;

	bool isWide;

	//Version 1.2
	u8 music_max_volume;
	u8 sfx_max_volume;

	//Version 1.4
	s32 fps;
	u16 vibration;

	//Version 1.5
	bool double_speed;
	u32 audio_buffer_size;

};

extern PK2SETTINGS Settings;

int Settings_GetId(PFile::Path path, u32& id);

int Settings_Open();
int Settings_Save();