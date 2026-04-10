//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "input_settings.hpp"

#include "engine/PInput.hpp"
#include "engine/types.hpp"

#include "engine/PFile.hpp"
#include <string>

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

class PK2SETTINGS {
public:	
	std::string language;

	bool  transparent_text = false;

	bool  draw_gui = false;
	bool  touchscreen_mode = false;
	
	int   fps;
	bool  isFullScreen = true;
	bool  double_speed = false;
	u8    shader_type;


	// Audio
	u8  music_max_volume;
	u8  sfx_max_volume;

	const InputSettings& getInput()const{
		return  this->useJoystick ? this->joystickInput : this->keyboardInput;
	}


	PInput::Key* getKeyByMenuID(unsigned int menuId){
		return this->useJoystick ? this->joystickInput.getKeyByMenuID(menuId) : this->keyboardInput.getKeyByMenuID(menuId);
	}

	friend void from_json(const nlohmann::json& j, PK2SETTINGS& settings);
	friend void to_json(nlohmann::json& j, const PK2SETTINGS& settings);


	bool useJoystick = false;
    bool useControllerVibrations = false;


	bool isUsingJoystick()const;
    float getJoystickAxis(int axis)const;
    void vibrateController(int duration, float strength = 0.5)const;

	InputSettings keyboardInput;
	InputSettings joystickInput;
};

extern PK2SETTINGS Settings;

void Settings_Open();
void Settings_Save();