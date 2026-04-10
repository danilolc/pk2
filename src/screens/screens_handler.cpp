//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "screens_handler.hpp"


#include "exceptions.hpp"
#include "sfx.hpp"
#include "settings/settings.hpp"
#include "gfx/touchscreen.hpp"

#include "system.hpp"
#include "language.hpp"

#include "engine/Piste.hpp"
#include "engine/PFilesystem.hpp"

#include <sstream>
#include <stdexcept>

#include "intro_screen.hpp"
#include "menu_screen.hpp"
#include "map_screen.hpp"
#include "playing_screen.hpp"
#include "score_screen.hpp"
#include "ending_screen.hpp"

ScreensHandler::ScreensHandler():
	screens_map({
		{SCREEN_INTRO, new IntroScreen()},
		{SCREEN_MENU, new MenuScreen()},
		{SCREEN_MAP, new MapScreen()},
		{SCREEN_GAME, new PlayingScreen()},
		{SCREEN_SCORING, new ScoreScreen()},
		{SCREEN_END, new EndingScreen()},
	}){
	
	Fadetext_Init();
	if(Settings.touchscreen_mode){
		TouchScreenControls.load();
	}

	if(tekstit!=nullptr){
		delete tekstit;
		tekstit = nullptr;
	}
	tekstit = new PLang();

	try{
		Load_Language(Settings.language);
	}
	catch(const std::exception& e){
		PLog::Write(PLog::ERR, "PK2", e.what());
		//Fallback to English
		Settings.language = "english.txt";
		Load_Language(Settings.language);
	}

	langlist = PFilesystem::ScanOriginalAssetsDirectory(PFilesystem::LANGUAGE_DIR, ".txt");
	PInput::InputSystem::instance().searchForInputDevices();

	int ret = Set_Screen_Mode(Settings.shader_type);
	if (ret != 0) {
		Settings.shader_type = SETTINGS_MODE_LINEAR;
		ret = Set_Screen_Mode(SETTINGS_MODE_LINEAR);
		Settings_Save();
	}
	
	PRender::set_fullscreen(Settings.isFullScreen);

	ret = -1;

	if (Settings.fps == SETTINGS_VSYNC)
		ret = Piste::set_fps(-1);
	else if (Settings.fps == SETTINGS_30FPS)
		ret = Piste::set_fps(30);
	else if (Settings.fps == SETTINGS_60FPS)
		ret = Piste::set_fps(60);
	else if (Settings.fps == SETTINGS_85FPS)
		ret = Piste::set_fps(85);
	else if (Settings.fps == SETTINGS_120FPS)
		ret = Piste::set_fps(120);

	if (ret != 0) {
		PLog::Write(PLog::ERR, "PK2", "FPS mode not supported, changing to 60fps");
		Piste::set_fps(60);
		Settings.fps = SETTINGS_60FPS;
		Settings_Save();
	}

	std::optional<PFile::Path> p = PFilesystem::FindVanillaAsset("pk2stuff.bmp", PFilesystem::GFX_DIR);
	if(!p.has_value()){
		throw std::runtime_error("\"pk2stuff.bmp\" not found!");
	}	
	PDraw::image_load(global_gfx_texture, *p, true);

	p = PFilesystem::FindVanillaAsset("pk2stuff2.bmp", PFilesystem::GFX_DIR);
	if(!p.has_value()){
		throw std::runtime_error("\"pk2stuff2.bmp\" not found!");
	}
	PDraw::image_load(global_gfx_texture2, *p, true);

	//PSound::load_overlay_music(PFile::Path("music" PE_SEP "super.xm"));  // why? what is so special about this one xm that it needs to be loaded at runtime?
	// I propose that we resort to start_music() for powerups that require special effects.	

	sfx_global.loadAll();

	Fade_in(FADE_SLOW);
	PSound::set_musicvolume_now(Settings.music_max_volume);


	PInput::InputSystem::instance().addKeyDownListener([&](const PInput::Key& key){this->onKeyPressed(key);});
	PInput::InputSystem::instance().addKeyUpListener([&](const PInput::Key& key){this->onKeyReleased(key);});
	PInput::InputSystem::instance().addPhysicalMouseMotionListener([&](){Screen::mouse_hidden=false;});
}

ScreensHandler::~ScreensHandler(){

	for(std::pair<int, Screen*> p : this->screens_map){
		if(p.second!=nullptr){
			delete p.second;
			p.second = nullptr;
		}	
	}
	this->screens_map.clear();
	sfx_global.free();

	PDraw::image_delete(global_gfx_texture);
	PDraw::image_delete(global_gfx_texture2);
}


void ScreensHandler::onKeyPressed(const PInput::Key& key){
	//std::cout<<"Key pressed: "<<key.getName()<<std::endl;

	this->current_screen->onKeyPressed(key);

#ifndef __ANDROID__
	if(key==Settings.getInput().fullscreenModeSwitch || (key==PInput::Key::RETURN && PInput::Key::LALT.isPressed())){
		Settings.isFullScreen = !Settings.isFullScreen;
		PRender::set_fullscreen(Settings.isFullScreen);
	}
#endif
}

void ScreensHandler::onKeyReleased(const PInput::Key& key){
	//std::cout<<"Key released: "<<key.getName()<<std::endl;
	
	this->current_screen->onKeyReleased(key);
}


//Main Loop
void ScreensHandler::Loop() {

	if(Screen::next_screen != this->current_screen_index){
		this->current_screen_index = Screen::next_screen;

		auto it = this->screens_map.find(Screen::next_screen);
		if(it==this->screens_map.end()){
			std::ostringstream os;
			os<<"Screen with index "<<Screen::next_screen<<" not found!";
			throw PExcept::PException(os.str());
		}
		this->current_screen = it->second;		

		Fade_in(FADE_NORMAL);

		this->current_screen->Init();
	}	

	if (Settings.touchscreen_mode)
		TouchScreenControls.update();

	PInput::InputSystem::instance().mouseKeysEnabled = this->current_screen->mouseKeysEnabled;
	this->current_screen->Loop();
	this->current_screen->clearMouseInput();

	if (Screen::closing_game && !Is_Fading()){
		Piste::stop();
	}
	// Fade and thunder
	Update_Colors();
}