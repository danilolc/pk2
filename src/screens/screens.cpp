//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "screens.hpp"

#include "sfx.hpp"
#include "settings.hpp"
#include "gui.hpp"
#include "language.hpp"
#include "gfx/text.hpp"
#include "save.hpp"
#include "system.hpp"

#include "game/map.hpp"

#include "PisteEngine.hpp"

#include <cstring>
#include <ctime>

int current_screen = SCREEN_NOT_SET;
int next_screen = SCREEN_FIRST_START;

bool closing_game = false;

void Fade_Quit() {
	if(!closing_game) PDraw::fade_out(PDraw::FADE_FAST);
	closing_game = true;
	PSound::set_musicvolume(0);
}

int Updade_Mouse() {

	int offset = PDraw::get_xoffset();

	if(PisteUtils_Is_Mobile()) {
    	float x, y;

		if (PisteInput_GetTouchPos(x, y) == 0) {
			mouse_x = screen_width * x - offset;
			mouse_y = screen_height * y;
			return 1;
		}
	}

	bool keys_move = (current_screen == SCREEN_MAP);
	bool relative = Settings.isFullScreen;

	if (relative) {

		MOUSE delta = PisteInput_UpdateMouse(keys_move, true);

		mouse_x += delta.x;
		mouse_y += delta.y;

	} else {
		
		MOUSE pos = PisteInput_UpdateMouse(false, false);

		mouse_x = pos.x - offset;
		mouse_y = pos.y;

	}
	
	if (mouse_x < -offset) mouse_x = -offset;
	if (mouse_x > screen_width - offset - 19) mouse_x = screen_width - offset - 19;
	
	if (mouse_y < 0) mouse_y = 0;
	if (mouse_y > screen_height - 19) mouse_y = screen_height - 19;

	return 0;
}

int Screen_First_Start() {

	PDraw::screen_fill(0);
	srand((unsigned)time(NULL));
	
	GUI_Change(UI_TOUCH_TO_START);
	
	Calculate_SinCos();

	Fadetext_Init();

	PK2Kartta_Set_Screen_Size(screen_width, screen_height);
	
	if (Settings.isFiltered)
		PDraw::set_filter(PDraw::FILTER_BILINEAR);
	else
		PDraw::set_filter(PDraw::FILTER_NEAREST);
	
	PDraw::fit_screen(Settings.isFit);
	PDraw::set_fullscreen(Settings.isFullScreen);
	PDraw::change_resolution(Settings.isWide ? 800 : 640, 480);

	PDraw::image_load(game_assets, "gfx/pk2stuff.bmp", false);
	PDraw::image_load(game_assets2, "gfx/pk2stuff2.bmp", false);
	//PDraw::image_load(bg_screen, "gfx/menu.bmp", true);

	//Sprites_clear();
	//Gifts_Clean();
	//Particles_Clear();

	//PK_Search_Episode();
	//Clean_Maps();
	//Load_Episode();

	Load_SFX();

	//PK_Calculate_Tiles();

	Load_SaveFile();

	PDraw::fade_in(PDraw::FADE_SLOW);
	
}

//If the screen change
int Screen_Change() {

	PDraw::fade_in(PDraw::FADE_NORMAL);

	switch (next_screen){
		case SCREEN_FIRST_START : Screen_First_Start();     break;
		case SCREEN_INTRO       : Screen_Intro_Init();      break;
		case SCREEN_MENU        : Screen_Menu_Init();       break;
		case SCREEN_MAP         : Screen_Map_Init();        break;
		case SCREEN_GAME        : Screen_InGame_Init();     break;
		case SCREEN_SCORING     : Screen_ScoreCount_Init(); break;
		case SCREEN_END         : Screen_Ending_Init();     break;
		default                 : Fade_Quit();              break;
	}

	current_screen = next_screen;

	return 0;
}

#include "game/game.hpp"

//Main Loop
int Screen_Loop() {

	if (next_screen != current_screen) Screen_Change();
	
	Updade_Mouse();
	
	switch (current_screen){
		case SCREEN_INTRO   : Screen_Intro();      break;
		case SCREEN_MENU    : Screen_Menu();       break;
		case SCREEN_MAP     : Screen_Map();        break;
		case SCREEN_GAME    : Screen_InGame();     break;
		case SCREEN_SCORING : Screen_ScoreCount(); break;
		case SCREEN_END     : Screen_Ending();     break;
		default             : Fade_Quit();         break;
	}

	if (key_delay > 0)
		key_delay--;

	if ((closing_game && !PDraw::is_fading()) || PK2_error)
		Piste::stop();

	return 0;
}