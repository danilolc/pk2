#include "screens.hpp"

#include "sfx.hpp"
#include "settings.hpp"
#include "gui.hpp"
#include "language.hpp"
#include "gfx/text.hpp"
#include "game.hpp"
#include "save.hpp"

#include "map.hpp"

#include "PisteEngine.hpp"

#include <cstring>
#include <ctime>

int current_screen = SCREEN_NOT_SET;
int next_screen = SCREEN_FIRST_START;

bool closing_game = false;

void PK_Fade_Quit() {
	if(!closing_game) PDraw::fade_out(PDraw::FADE_FAST);
	closing_game = true;
	PSound::set_musicvolume(0);
}

int Screen_First_Start() {
	
	PDraw::screen_fill(0);
	srand((unsigned)time(NULL));
	
	GUI_Change(UI_TOUCH_TO_START);
	strcpy(pelaajan_nimi, tekstit->Hae_Teksti(PK_txt.player_default_name));
	
	if (!test_level) {
		strcpy(episodi, "");
		strcpy(current_map_name, "untitle1.map");
	}

	jakso = 1;

	Precalculate_SinCos();

	Fadetext_Init();

	PK2Kartta_Set_Screen_Size(screen_width, screen_height);

	kartta = new PK2Kartta();
	
	if (Settings.isFiltered)
		PDraw::set_filter(PDraw::FILTER_BILINEAR);
	else
		PDraw::set_filter(PDraw::FILTER_NEAREST);
	
	PDraw::fit_screen(Settings.isFit);
	PDraw::set_fullscreen(Settings.isFullScreen);
	PDraw::change_resolution(Settings.isWide ? 800 : 640, 480);

	PDraw::image_load(kuva_peli, "gfx/pk2stuff.bmp", false);
	PDraw::image_load(kuva_peli2, "gfx/pk2stuff2.bmp", false);
	PDraw::image_load(kuva_tausta, "gfx/menu.bmp", true);

	//Sprites_clear();
	//Gifts_Clean();
	//Particles_Clear();

	//PK_Search_Episode();
	PK_Start_Saves();
	Load_Maps();

	Load_SFX();
	
	PDraw::fade_in(PDraw::FADE_SLOW);

	//PK_Calculate_Tiles();

	Load_SaveFile();
	
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
		default                 : PK_Fade_Quit();           break;
	}

	current_screen = next_screen;

	return 0;
}

int Updade_Mouse(){
	if(PisteUtils_Is_Mobile()){
    	float x, y;

		if (PisteInput_GetTouchPos(x, y) == 0) {
			mouse_x = screen_width * x - PDraw::get_xoffset();
			mouse_y = screen_height * y;
			return 1;
		}
	}

	MOUSE pos = PisteInput_UpdateMouse(current_screen == SCREEN_MAP, Settings.isFullScreen);
	pos.x -= PDraw::get_xoffset();

	if (pos.x < 0) pos.x = 0;
	if (pos.y < 0) pos.y = 0;
	if (pos.x > 640-19) pos.x = 640-19;
	if (pos.y > 480-19) pos.y = 480-19;

	mouse_x = pos.x;
	mouse_y = pos.y;

	return 0;
}

//Common to every screen
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
		default             : PK_Fade_Quit();      break;
	}

	/*
	static bool wasPressed = false;

	bool skipped = !skip_frame && doublespeed; // If is in double speed and don't skip this frame, so the last frame was skipped, and it wasn't drawn
	if (PisteInput_Keydown(PI_ESCAPE) && key_delay == 0 && !skipped){ //Don't activate menu whith a not drawn screen
		if(test_level)
			PK_Fade_Quit();
		else { //Exit with esc
			if (menu_nyt != MENU_MAIN || current_screen != SCREEN_MENU){
				next_screen = SCREEN_MENU;
				//menu_nyt = MENU_MAIN;
				degree_temp = degree;
			}
			else if (current_screen == SCREEN_MENU && !wasPressed && PisteInput_Keydown(PI_ESCAPE) && menu_lue_kontrollit == 0){ // Just pressed escape in menu
				if(menu_valittu_id == menu_valinta_id-1)
					PK_Fade_Quit();
				else {
					menu_valittu_id = menu_valinta_id-1; // Set to "exit" option
					//window_activated = false;
					//PisteInput_ActivateWindow(false);
				}
			}
		}
	}

	wasPressed = PisteInput_Keydown(PI_ESCAPE);
	*/

	if ((closing_game && !PDraw::is_fading()) || PK2_error)
		Piste::stop();
	
	return 0;
}