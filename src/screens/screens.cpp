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

static bool closing_game = false;

void Fade_Quit() {

	if(!closing_game) PDraw::fade_out(PDraw::FADE_FAST);
	closing_game = true;
	PSound::set_musicvolume(0);
	
}

int Screen_First_Start() {

	if(test_level)
		srand(time(NULL));
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

	MapClass_Set_Screen_Size(screen_width, screen_height);
	
	PInput::SetVibration(Settings.vibration);

	if (PInput::ControllerFound())
		Input = &Settings.joystick;

	// TODO set shader
	/*if (Settings.isFiltered)
		PRender::set_filter(PRender::FILTER_LINEAR);
	else
		PRender::set_filter(PRender::FILTER_NEAREST);*/

	PRender::set_fullscreen(Settings.isFullScreen);

	if (Settings.fps == SETTINGS_VSYNC)
		Piste::set_fps(-1);
	else if (Settings.fps == SETTINGS_30FPS)
		Piste::set_fps(30);
	else if (Settings.fps == SETTINGS_60FPS)
		Piste::set_fps(60);
	else if (Settings.fps == SETTINGS_85FPS)
		Piste::set_fps(85);
	else if (Settings.fps == SETTINGS_120FPS)
		Piste::set_fps(120);
	
	PDraw::image_load(game_assets, PFile::Path("gfx" PE_SEP "pk2stuff.bmp"), false);
	PDraw::image_load(game_assets2, PFile::Path("gfx" PE_SEP "pk2stuff2.png"), false);

	Load_SFX();

	Load_SaveFile();

	PDraw::fade_in(PDraw::FADE_SLOW);
	PSound::set_musicvolume_now(Settings.music_max_volume);

	PSound::load_overlay_music(PFile::Path("music" PE_SEP "super.xm"));
	
	return 0;

}

//If the screen change
int Screen_Change() {

	PDraw::fade_in(PDraw::FADE_NORMAL);

	switch (next_screen) {
		case SCREEN_INTRO   : Screen_Intro_Init();      break;
		case SCREEN_MENU    : Screen_Menu_Init();       break;
		case SCREEN_MAP     : Screen_Map_Init();        break;
		case SCREEN_GAME    : Screen_InGame_Init();     break;
		case SCREEN_SCORING : Screen_ScoreCount_Init(); break;
		case SCREEN_END     : Screen_Ending_Init();     break;
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
		default             : Fade_Quit();         break;
	}

	if (PK2_error) return 1;

	if (key_delay > 0)
		key_delay--;

	if (closing_game && !PDraw::is_fading())
		Piste::stop();

	return 0;

}