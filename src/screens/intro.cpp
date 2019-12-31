//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "screens/screens.hpp"

#include "gfx/text.hpp"
#include "gui.hpp"
#include "game/game.hpp"
#include "language.hpp"
#include "system.hpp"
#include "settings.hpp"

#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"
#include "engine/PSound.hpp"

DWORD introlaskuri = 0;
bool siirry_introsta_menuun = false;

int Draw_Intro(){

	DWORD pistelogo_alku	= 300;
	DWORD pistelogo_loppu	= pistelogo_alku + 500;
	DWORD tekijat_alku		= pistelogo_loppu + 80;
	DWORD tekijat_loppu		= tekijat_alku + 720;
	DWORD testaajat_alku	= tekijat_loppu + 80;
	DWORD testaajat_loppu	= testaajat_alku + 700;
	DWORD kaantaja_alku		= testaajat_loppu + 100;
	DWORD kaantaja_loppu	= kaantaja_alku + 300;

	PDraw::screen_fill(0);
	PDraw::image_cutclip(bg_screen, 280, 80, 280, 80, 640, 480);

	if ((introlaskuri / 10) % 50 == 0)
		PDraw::image_cutclip(bg_screen,353, 313, 242, 313, 275, 432);

	if (introlaskuri > pistelogo_alku && introlaskuri < pistelogo_loppu) {

		//int x = introlaskuri - pistelogo_alku - 194;
		int kerroin = 120 / (introlaskuri - pistelogo_alku);
		int x = 120 - kerroin;

		if (x > 120)
			x = 120;

		PDraw::image_cutclip(bg_screen,/*120*/x,230, 37, 230, 194, 442);

		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_presents), fontti1, 230, 400, pistelogo_alku, pistelogo_loppu-20, introlaskuri);

	}

	if (introlaskuri > tekijat_alku) {
		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_a_game_by),fontti1, 120, 200, tekijat_alku, tekijat_loppu, introlaskuri);
		CreditsText_Draw("janne kivilahti 2003",		            fontti1, 120, 220, tekijat_alku+20, tekijat_loppu+20, introlaskuri);
		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_original), fontti1, 120, 245, tekijat_alku+40, tekijat_loppu+40, introlaskuri);
		CreditsText_Draw("antti suuronen 1998",		            fontti1, 120, 265, tekijat_alku+50, tekijat_loppu+50, introlaskuri);
		CreditsText_Draw("sdl porting by",		                fontti1, 120, 290, tekijat_alku+70, tekijat_loppu+70, introlaskuri);
		CreditsText_Draw("samuli tuomola 2010",		            fontti1, 120, 310, tekijat_alku+80, tekijat_loppu+80, introlaskuri);
		CreditsText_Draw("sdl2 port and bug fixes",               fontti1, 120, 335, tekijat_alku + 90, tekijat_loppu + 90, introlaskuri);
		CreditsText_Draw("danilo lemos 2017",                     fontti1, 120, 355, tekijat_alku + 100, tekijat_loppu + 100, introlaskuri);
	}

	if (introlaskuri > testaajat_alku) {
		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_tested_by),fontti1, 120, 230, testaajat_alku, testaajat_loppu, introlaskuri);
		CreditsText_Draw("antti suuronen",			fontti1, 120, 250, testaajat_alku+10, testaajat_loppu+10, introlaskuri);
		CreditsText_Draw("toni hurskainen",			fontti1, 120, 260, testaajat_alku+20, testaajat_loppu+20, introlaskuri);
		CreditsText_Draw("juho rytk�nen",				fontti1, 120, 270, testaajat_alku+30, testaajat_loppu+30, introlaskuri);
		CreditsText_Draw("annukka korja",				fontti1, 120, 280, testaajat_alku+40, testaajat_loppu+40, introlaskuri);
		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_thanks_to),fontti1, 120, 300, testaajat_alku+70, testaajat_loppu+70, introlaskuri);
		CreditsText_Draw("oskari raunio",				fontti1, 120, 310, testaajat_alku+70, testaajat_loppu+70, introlaskuri);
		CreditsText_Draw("assembly organization",		fontti1, 120, 320, testaajat_alku+70, testaajat_loppu+70, introlaskuri);
	}

	if (introlaskuri > kaantaja_alku) {
		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_translation), fontti1, 120, 230, kaantaja_alku, kaantaja_loppu, introlaskuri);
		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_translator),  fontti1, 120, 250, kaantaja_alku+20, kaantaja_loppu+20, introlaskuri);
	}
	return 0;
}

int Screen_Intro_Init() {
	//PisteLog_Kirjoita("- Initializing intro screen\n");
	GUI_Change(UI_TOUCH_TO_START);
	if (Settings.isWide)
		PDraw::set_xoffset(80);
	else
		PDraw::set_xoffset(0);
	PDraw::screen_fill(0);

	//PisteLog_Kirjoita("  - Loading picture: gfx/intro.bmp\n");
	PDraw::image_delete(bg_screen);
	bg_screen = PDraw::image_load("gfx/intro.bmp", true);

	//PisteLog_Kirjoita("  - Loading music: music/INTRO.XM\n");

	if (PSound::start_music("music" PE_SEP "intro.xm") != 0)
		PK2_Error("Can't load intro.xm");

	PSound::set_musicvolume(Settings.music_max_volume);

	introlaskuri = 0;
	//siirry_pistelaskusta_karttaan = false;

	PDraw::fade_in(PDraw::FADE_FAST);

	return 0;
}

int Screen_Intro(){
	Draw_Intro();

	degree = 1 + degree % 360;

	introlaskuri++;

	if (siirry_introsta_menuun && !PDraw::is_fading()){
		next_screen = SCREEN_MENU;
	}
	if (key_delay > 0) key_delay--;
	if (key_delay == 0)
		if (PisteInput_Keydown(PI_RETURN) || PisteInput_Keydown(PI_SPACE) || introlaskuri == 3500){
			siirry_introsta_menuun = true;
			PDraw::fade_out(PDraw::FADE_SLOW);
		}

	return 0;
}