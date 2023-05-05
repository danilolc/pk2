//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "screens/screens.hpp"

#include "sfx.hpp"
#include "settings.hpp"
#include "gui.hpp"
#include "language.hpp"
#include "gfx/text.hpp"
#include "save.hpp"
#include "system.hpp"

#include "game/mapclass.hpp"
#include "episode/episodeclass.hpp"
#include "language.hpp"

#include "engine/Piste.hpp"

#include <cstring>
#include <ctime>

int current_screen = SCREEN_FIRST_START;
int next_screen = SCREEN_NOT_SET;

uint menu_valittu_id = 0;
uint menu_valinta_id = 1;

static bool closing_game = false;

void Fade_Quit() {

	if(!closing_game) Fade_out(FADE_FAST);
	closing_game = true;
	PSound::set_musicvolume(0);
	
}

bool Draw_Menu_Text(const char *teksti, int x, int y, char end) {

	const int TEXT_H = 20; 

	int length = strlen(teksti) * 15;

	bool mouse_on = PInput::mouse_x > x && PInput::mouse_x < x + length 
		&& PInput::mouse_y > y && PInput::mouse_y < y + TEXT_H
		&& !mouse_hidden;

	if ( mouse_on || (menu_valittu_id == menu_valinta_id) ) {

		menu_valittu_id = menu_valinta_id;
		Wavetext_Draw(teksti, fontti3, x, y, end);//

		int c = Clicked();
		if ( (c == 1 && mouse_on) || (c > 1) ) {

			Play_MenuSFX(menu_sound, 100);
			key_delay = 20;
			menu_valinta_id++;
			
			return true;

		}

		//Wavetext_Draw(teksti, fontti3, x, y);

	} else {
	
		WavetextSlow_Draw(teksti, fontti2, x, y, end);
	
	}

	menu_valinta_id++;

	return false;
}

int Screen_First_Start() {

	if(test_level)
		srand(0);
	else
		srand(Settings.id);
	
	Calculate_SinCos();

	Fadetext_Init();

	if(PUtils::Is_Mobile())
		GUI_Load();

	tekstit = new PLang();
	if (Load_Language(Settings.language) != 0) {

		PLog::Write(PLog::ERR, "PK2", "Could not find %s!", Settings.language);
		strcpy(Settings.language, "english.txt");
		
		if(Load_Language(Settings.language) != 0) {

			PLog::Write(PLog::FATAL, "PK2", "Could not find the default language file!");
			PK2_Error("Error");
			return -1;

		}

	}
	
	if (Load_Fonts(tekstit) != 0) {

		strcpy(Settings.language, "english.txt");
		if(Load_Language(Settings.language) != 0) {

			PLog::Write(PLog::FATAL, "PK2", "Could not find the default language file!");
			PK2_Error("Error");
			return -1;

		}

		if (Load_Fonts(tekstit) != 0) {

			PLog::Write(PLog::FATAL, "PK2", "Couldn't load fonts!");
			PK2_Error("Error");
			return -1;

		}
	
	}

	langlist = PFile::Path("language" PE_SEP).scandir(".txt");
	
	Search_Episodes();
	
	PInput::SetVibration(Settings.vibration);

	if (Settings.using_controller == SET_TRUE)
		Input = &Settings.joystick;
	else if (Settings.using_controller == SET_FALSE)
		Input = &Settings.keyboard;
	else if (PInput::ControllerFound())
		Input = &Settings.joystick;
	else
		Input = &Settings.keyboard;

	int ret = Set_Screen_Mode(Settings.shader_type);
	if (ret != 0) {
		Settings.shader_type = SETTINGS_MODE_LINEAR;
		ret = Set_Screen_Mode(SETTINGS_MODE_LINEAR);
		Settings_Save();
	}
	
	PRender::set_fullscreen(Settings.isFullScreen);

	ret = -1;

	if (PUtils::Is_Mobile()) {
	
		ret = Piste::set_fps(-1);
	
	} else {

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

	}

	if (ret != 0) {
		PLog::Write(PLog::ERR, "PK2", "FPS mode not supported, changing to 60fps");
		Piste::set_fps(60);
		Settings.fps = SETTINGS_60FPS;
		Settings_Save();
	}

	PDraw::image_load(game_assets, PFile::Path("gfx" PE_SEP "pk2stuff.bmp"), false);
	PDraw::image_load(game_assets2, PFile::Path("gfx" PE_SEP "pk2stuff2.png"), false);

	PSound::load_overlay_music(PFile::Path("music" PE_SEP "super.xm"));

	Load_SFX();

	Load_SaveFile();

	Fade_in(FADE_SLOW);
	PSound::set_musicvolume_now(Settings.music_max_volume);
	
	return 0;

}

//If the screen change
int Screen_Change() {

	Fade_in(FADE_NORMAL);

	switch (next_screen) {
		case SCREEN_INTRO   : Screen_Intro_Init();      break;
		case SCREEN_MENU    : Screen_Menu_Init();       break;
		case SCREEN_MAP     : Screen_Map_Init();        break;
		case SCREEN_GAME    : Screen_InGame_Init();     break;
		case SCREEN_SCORING : Screen_ScoreCount_Init(); break;
		case SCREEN_END     : Screen_Ending_Init();     break;
		case SCREEN_LUA     : Screen_Lua_Init();        break;
		default             : Fade_Quit();              break;
	}

	current_screen = next_screen;

	return 0;
}

//Main Loop
int Screen_Loop() {

	if (next_screen != current_screen) Screen_Change();

	if (PK2_error) return 1;
	
	bool keys_move = (current_screen == SCREEN_MAP);
	bool relative = Settings.isFullScreen;
	PInput::UpdateMouse(keys_move, relative);
	
	if (PUtils::Is_Mobile())
		GUI_Update();

	switch (current_screen) {
		case SCREEN_INTRO   : Screen_Intro();      break;
		case SCREEN_MENU    : Screen_Menu();       break;
		case SCREEN_MAP     : Screen_Map();        break;
		case SCREEN_GAME    : Screen_InGame();     break;
		case SCREEN_SCORING : Screen_ScoreCount(); break;
		case SCREEN_END     : Screen_Ending();     break;
		case SCREEN_LUA     : Screen_Lua();        break;
		default             : Fade_Quit();         break;
	}

	if (PK2_error) return 1;

	if (key_delay > 0)
		key_delay--;

	if (closing_game && !Is_Fading())
		Piste::stop();

	// Fade and thunder
	Update_Colors();

	return 0;

}