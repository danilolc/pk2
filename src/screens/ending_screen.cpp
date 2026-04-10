//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "ending_screen.hpp"

#include "episode/episodeclass.hpp"
#include "gfx/text.hpp"
#include "gfx/touchscreen.hpp"
#include "game/game.hpp"
#include "language.hpp"
#include "system.hpp"
#include "settings/settings.hpp"
#include "exceptions.hpp"



#include "engine/PLog.hpp"
#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"
#include "engine/PSound.hpp"
#include "engine/PFilesystem.hpp"

#include "engine/types.hpp"

void EndingScreen::Draw_EndGame_Image(int x, int y, int tyyppi, int plus, int rapytys){
	int frm = 0;
	int yk = 0;

	if (tyyppi == 1){ // Pekka
		frm = 1;
		if ((degree/10)%10==rapytys) frm = 0;
		yk = (int)sin_table(degree);
		PDraw::image_cutclip(bg_screen,x+3,y+56, 4, 63, 29, 69);
		if (yk < 0){
			y+=yk;
			frm = 2;
		}
		PDraw::image_cutclip(bg_screen,x,y, 1+frm*35, 1, 32+frm*35, 59);
	}

	if (tyyppi == 2){ // kana (katse oikealle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)cos_table(degree+plus);
		PDraw::image_cutclip(bg_screen,x+3,y+36, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PDraw::image_cutclip(bg_screen,x,y, 106+frm*37, 1, 139+frm*37, 38);
	}

	if (tyyppi == 3){ // kana (katse vasemmalle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)cos_table(degree+plus);
		PDraw::image_cutclip(bg_screen,x+3,y+36, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PDraw::image_cutclip(bg_screen,x,y, 106+frm*37, 41, 139+frm*37, 77);
	}

	if (tyyppi == 4){ // pikkukana (katse oikealle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)sin_table((degree*2)+plus);
		PDraw::image_cutclip(bg_screen,x+3,y+27, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PDraw::image_cutclip(bg_screen,x,y, 217+frm*29, 1, 243+frm*29, 29);
	}

	if (tyyppi == 5){ // pikkukana (katse vasemmalle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)sin_table((degree*2)+plus);
		PDraw::image_cutclip(bg_screen,x,y+27, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PDraw::image_cutclip(bg_screen,x,y, 217+frm*29, 33, 243+frm*29, 61);
	}
}
void EndingScreen::Draw_EndGame(){

	u32 onnittelut_alku	= 300;
	u32 onnittelut_loppu	= onnittelut_alku + 1000;
	u32 the_end_alku		= onnittelut_loppu + 80;
	u32 the_end_loppu		= the_end_alku + 3000;

	PDraw::image_cutclip(bg_screen,320-233/2,240-233/2, 6, 229, 239, 462);

	Draw_EndGame_Image(345, 244, 3, 30, 2);
	Draw_EndGame_Image(276, 230, 2, 50, 3);
	Draw_EndGame_Image(217, 254, 4, 0, 4);

	Draw_EndGame_Image(305, 240, 1, 0, 1);

	Draw_EndGame_Image(270, 284, 2, 20, 1);
	Draw_EndGame_Image(360, 284, 5, 60, 2);

	if (loppulaskuri > onnittelut_alku) {
		CreditsText_Draw_Centered(tekstit->Get_Text(PK_txt.end_congratulations), fontti2, 380, onnittelut_alku, onnittelut_loppu, loppulaskuri);
		CreditsText_Draw_Centered(tekstit->Get_Text(PK_txt.end_chickens_saved), fontti1, 402, onnittelut_alku+30, onnittelut_loppu+30, loppulaskuri);

		/*CreditsText_Draw(tekstit->Get_Text(PK_txt.end_congratulations).c_str(), fontti2, 220, 380, onnittelut_alku, onnittelut_loppu, loppulaskuri);
		CreditsText_Draw(tekstit->Get_Text(PK_txt.end_chickens_saved).c_str(), fontti1, 220, 402, onnittelut_alku+30, onnittelut_loppu+30, loppulaskuri);*/
	}
	if (loppulaskuri > the_end_alku) {
		CreditsText_Draw_Centered(tekstit->Get_Text(PK_txt.end_the_end), fontti2, 190, the_end_alku, the_end_loppu, loppulaskuri);

		//CreditsText_Draw(tekstit->Get_Text(PK_txt.end_the_end).c_str(), fontti2, 280, 190, the_end_alku, the_end_loppu, loppulaskuri);
	}
}

void EndingScreen::Init() {

	TouchScreenControls.change(UI_TOUCH_TO_START);
	this->touchscreenWasPressed = true;
	
	PDraw::set_offset(640, 480);

	std::optional<PFile::Path> path = PFilesystem::FindAsset("ending.bmp", PFilesystem::GFX_DIR, ".png");

	if (path.has_value()) {

		PDraw::image_load_with_palette(bg_screen, default_palette, *path, true);
		PDraw::palette_set(default_palette);

	} else {
		throw PExcept::PException("\"ending.bmp\" not found!");
	}

	path = PFilesystem::FindVanillaAsset("intro.xm", PFilesystem::MUSIC_DIR);
	if(!path.has_value()){
		throw PExcept::PException("\"intro.xm\" not found!");
	}
	else{

		if (PSound::start_music(*path) == -1){
			PLog::Write(PLog::ERR, "PK2", "Cannot load \"intro.xm\"!");
		}
	}


	
		

	PSound::set_musicvolume(Settings.music_max_volume);

	loppulaskuri = 0;
	change_to_next_screen = false;

	Fade_in(FADE_FAST);
}

void EndingScreen::onKeyPressed(const PInput::Key& key){
	change_to_next_screen = true;
	PSound::set_musicvolume(0);
	Fade_out(FADE_SLOW);
}

void EndingScreen::Loop(){

	Draw_EndGame();

	degree = 1 + degree % 360;

	loppulaskuri++;
	//introlaskuri = loppulaskuri; // introtekstej� varten

	if(Settings.touchscreen_mode){

		if(TouchScreenControls.any&&! this->touchscreenWasPressed){
			change_to_next_screen = true;
			PSound::set_musicvolume(0);
			Fade_out(FADE_SLOW);			
		}

		this->touchscreenWasPressed = TouchScreenControls.any;
	}

	if (change_to_next_screen && !Is_Fading()) {
		PSound::set_musicvolume_now(Settings.music_max_volume);
		//next_screen = SCREEN_MENU;
		next_screen = SCREEN_MAP;
	}

	if (Episode->glows)
		if (degree % 4 == 0)
			PDraw::rotate_palette(224,239);
}