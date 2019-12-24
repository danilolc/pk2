#include "screens.hpp"

#include "sfx.hpp"
#include "settings.hpp"
#include "gui.hpp"
#include "language.hpp"
#include "text.hpp"
#include "game.hpp"

#include "map.hpp"

#include "PisteDraw.hpp"
#include "PisteInput.hpp"

#include <cstring>

int current_screen = SCREEN_NOT_SET;
int next_screen = SCREEN_FIRST_START;



int Screen_First_Start() {
	
	PDraw::screen_fill(0);
	srand((unsigned)time(NULL));
	
	GUI_Change(UI_TOUCH_TO_START);
	strcpy(pelaajan_nimi, tekstit->Hae_Teksti(PK_txt.player_default_name));
	
	
	if (!test_level) {
		strcpy(episodi, "");
		strcpy(seuraava_kartta, "untitle1.map");
	}

	jakso = 1;

	Precalculate_SinCos();

	Fadetext_Init();

	PK2Kartta_Aseta_Ruudun_Mitat(screen_width, screen_height);

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

	//PDraw::image_delete(kuva_peli);
	//kuva_peli = PDraw::image_load("gfx/pk2stuff.bmp", false);

	Load_Fonts();

	//Sprites_clear();
	//Gifts_Clean();
	//Particles_Clear();

	PK_Search_Episode();
	PK_Start_Saves();
	PK_Search_File();

	Load_SFX();
	
	PDraw::fade_in(PDraw::FADE_SLOW);

	PK_Calculate_Tiles();

	//Gifts_Clean();

	PDraw::image_delete(kuva_tausta);
	kuva_tausta = PDraw::image_load("gfx/menu.bmp", true);

	PK_Empty_Records();

	PK_Search_Records("data/saves.dat");
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

//Common to every screen
int Screen_Loop() {

	if (next_screen != current_screen) Screen_Change();
	
	PK_Updade_Mouse();
	
	switch (current_screen){
		case SCREEN_INTRO   : Screen_Intro();      break;
		case SCREEN_MENU    : Screen_Menu();       break;
		case SCREEN_MAP     : Screen_Map();        break;
		case SCREEN_GAME    : Screen_InGame();     break;
		case SCREEN_SCORING : Screen_ScoreCount(); break;
		case SCREEN_END     : Screen_Ending();     break;
		default             : PK_Fade_Quit();      break;
	}

	// Update music volume
	bool update = false;
	if (music_volume != music_volume_now)
		update = true;

	if (update) {
		if (Settings.music_max_volume > 64)
			Settings.music_max_volume = 64;

		if (Settings.music_max_volume < 0)
			Settings.music_max_volume = 0;

		if (music_volume > Settings.music_max_volume)
			music_volume = Settings.music_max_volume;

		if (music_volume_now < music_volume)
			music_volume_now++;

		if (music_volume_now > music_volume)
			music_volume_now--;

		if (music_volume_now > 64)
			music_volume_now = 64;

		if (music_volume_now < 0)
			music_volume_now = 0;

		PSound::set_musicvolume(music_volume_now);
	}

	static bool wasPressed = false;

	bool skipped = !skip_frame && doublespeed; // If is in double speed and don't skip this frame, so the last frame was skipped, and it wasn't drawn
	if (PisteInput_Keydown(PI_ESCAPE) && key_delay == 0 && !skipped){ //Don't activate menu whith a not drawn screen
		if(test_level)
			PK_Fade_Quit();
		else{
			if (menu_nyt != MENU_MAIN || current_screen != SCREEN_MENU){
				next_screen = SCREEN_MENU;
				menu_nyt = MENU_MAIN;
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

	if ((closing_game && !PDraw::is_fading()) || PK2_error)
		Piste::stop();
	
	return 0;
}