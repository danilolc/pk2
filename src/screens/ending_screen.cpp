//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "screens/screens.hpp"

#include "episode/episodeclass.hpp"
#include "gfx/text.hpp"
#include "gui.hpp"
#include "game/game.hpp"
#include "language.hpp"
#include "system.hpp"
#include "settings.hpp"

#include "engine/PLog.hpp"
#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"
#include "engine/PSound.hpp"
#include "engine/PUtils.hpp"

#include "engine/types.hpp"

static u32 loppulaskuri = 0;
static bool siirry_lopusta_menuun = false;

int Draw_EndGame_Image(int x, int y, int tyyppi, int plus, int rapytys){
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

	return 0;
}
int Draw_EndGame(){

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
		CreditsText_Draw(tekstit->Get_Text(PK_txt.end_congratulations), fontti2, 220, 380, onnittelut_alku, onnittelut_loppu, loppulaskuri);
		CreditsText_Draw(tekstit->Get_Text(PK_txt.end_chickens_saved), fontti1, 220, 402, onnittelut_alku+30, onnittelut_loppu+30, loppulaskuri);
	}
	if (loppulaskuri > the_end_alku) {
		CreditsText_Draw(tekstit->Get_Text(PK_txt.end_the_end), fontti2, 280, 190, the_end_alku, the_end_loppu, loppulaskuri);
	}

	return 0;
}

int Screen_Ending_Init() {
	
	if(PUtils::Is_Mobile())
		GUI_Change(UI_TOUCH_TO_START);
	
	PDraw::set_offset(640, 480);

	PFile::Path path = Episode->Get_Dir("ending.bmp");
	if (FindAsset(&path, "gfx" PE_SEP)) {

		PDraw::image_load(bg_screen, path, true);

	} else {

		PLog::Write(PLog::ERR, "PK2", "Can't load ending bg"); //"Can't load map bg"

	}

	if (PSound::start_music(PFile::Path("music" PE_SEP "intro.xm")) == -1)
		PK2_Error("Can't load intro.xm");

	PSound::set_musicvolume(Settings.music_max_volume);

	loppulaskuri = 0;
	siirry_lopusta_menuun = false;

	Fade_in(FADE_FAST);

	return 0;

}

int Screen_Ending(){

	Draw_EndGame();

	degree = 1 + degree % 360;

	loppulaskuri++;
	//introlaskuri = loppulaskuri; // introtekstej� varten

	if (siirry_lopusta_menuun && !Is_Fading()) {
		PSound::set_musicvolume_now(Settings.music_max_volume);
		//next_screen = SCREEN_MENU;
		next_screen = SCREEN_MAP;
	}

	if (key_delay == 0) {
		if (Clicked() || Gui_touch) {
			siirry_lopusta_menuun = true;
			PSound::set_musicvolume(0);
			Fade_out(FADE_SLOW);
		}
	}

	if (Episode->glows)
		if (degree % 4 == 0)
			PDraw::rotate_palette(224,239);

	return 0;
}