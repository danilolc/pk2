//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "settings.hpp"

#include "language.hpp"
#include "system.hpp"

#include "engine/PLog.hpp"
#include "engine/PInput.hpp"
#include "engine/PJson.hpp"
#include "engine/PFilesystem.hpp"

#include <ctime>
#include <cstring>
#include <string>
#include <fstream>

#include <SDL_timer.h>

const char* SETTINGS_FILE = "settings.json";

PK2SETTINGS Settings;

void Settings_Init() {
	Settings.language = GetDefaultLanguageName();
	Settings.transparent_text = false;
	Settings.draw_gui = true;


	Settings.keyboardInput.setDefault();
	Settings.joystickInput.setDefaultJoystick();

#ifdef __ANDROID__
	Settings.touchscreen_mode = true;
#else
	Settings.touchscreen_mode = false;
#endif

	Settings.fps = SETTINGS_60FPS;//SETTINGS_VSYNC;
	Settings.isFullScreen = true;
	Settings.double_speed = false;
	Settings.shader_type = SETTINGS_MODE_LINEAR;	

	Settings.music_max_volume = 35; //30;
	Settings.sfx_max_volume = 85; //95;
}

void from_json(const nlohmann::json& j, PK2SETTINGS& s){
	j.at("language").get_to(s.language);
	j.at("gui").get_to(s.draw_gui);
	j.at("touchscreen").get_to(s.touchscreen_mode);
	j.at("fps").get_to(s.fps);
	j.at("fullscreen").get_to(s.isFullScreen);
	j.at("double_speed").get_to(s.double_speed);

	j.at("keyboard").get_to(s.keyboardInput);
	j.at("joystick").get_to(s.joystickInput);

	j.at("use_joystick").get_to(s.useJoystick);
	j.at("use_vibration").get_to(s.useControllerVibrations);

	j.at("music_max_volume").get_to(s.music_max_volume);
	j.at("sfx_max_volume").get_to(s.sfx_max_volume);
	j.at("transparent_text").get_to(s.transparent_text);
	j.at("filtering").get_to(s.shader_type);
}

void to_json(nlohmann::json& j, const PK2SETTINGS& s){
	j["language"] = s.language;
	j["gui"] = s.draw_gui;

	j["touchscreen"] = s.touchscreen_mode;
	j["fps"] = s.fps;

	j["fullscreen"] = s.isFullScreen;
	j["double_speed"] = s.double_speed;

	j["keyboard"] = s.keyboardInput;
	j["joystick"] = s.joystickInput;

	j["use_joystick"] = s.useJoystick;
	j["use_vibration"] = s.useControllerVibrations;


	j["music_max_volume"] = s.music_max_volume;
	j["sfx_max_volume"] = s.sfx_max_volume;
	j["transparent_text"] = s.transparent_text;

	j["filtering"] = s.shader_type;
}


void Settings_Open() {

	PFile::Path path = PFilesystem::GetDataFileW(SETTINGS_FILE);

	try{
		if(path.exists()){
			Settings = path.GetJSON().get<PK2SETTINGS>();
		}
		else{
			PLog::Write(PLog::DEBUG, "PK2", "No settings found");
			Settings_Init();
			Settings_Save();
		}
		//
	}
	catch(const PFile::PFileException& e){
		PLog::Write(PLog::DEBUG, "PK2", "No settings found");
		Settings_Init();
		Settings_Save();		
		
	}
	catch(const std::exception& e){
		PLog::Write(PLog::ERR, "PK2", e.what());
		Settings_Init();
		Settings_Save();
	}

}

void Settings_Save() {

	PFile::Path path = PFilesystem::GetDataFileW(SETTINGS_FILE);
	nlohmann::json j = Settings;
	std::ofstream f(path.c_str());

	f << j.dump(4)<<std::endl;
	f.close();
}



void PK2SETTINGS::vibrateController(int duration, float strength)const{
	if(this->useControllerVibrations){
		PInput::InputSystem::instance().vibrate(duration, strength);
	}
}

bool PK2SETTINGS::isUsingJoystick()const{
	return this->useJoystick && PInput::InputSystem::instance().isJoystickAvailable();
}

float PK2SETTINGS::getJoystickAxis(int axis)const{
	if(this->isUsingJoystick()){
		return PInput::InputSystem::instance().getAxis(axis);
	}
	return 0;
}