//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "intro_screen.hpp"

#include "gfx/text.hpp"
#include "gfx/touchscreen.hpp"
#include "game/game.hpp"
#include "language.hpp"
#include "system.hpp"
#include "settings/settings.hpp"
#include "exceptions.hpp"

#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"
#include "engine/PSound.hpp"
#include "engine/PFilesystem.hpp"
#include "engine/PLog.hpp"

#include <stdexcept>

void IntroScreen::Draw(){

	const int LINE_SEPARATOR = 10;
	const int LINE_SEPARATOR_TITLE = 15;

	const int SECTION_SEPARATOR = 25;

	u32 pistelogo_alku  = 280;
	u32 pistelogo_loppu = pistelogo_alku + 500;
	u32 tekijat_alku    = pistelogo_loppu + 120;
	u32 tekijat_loppu   = tekijat_alku + 700;
	u32 testaajat_alku  = tekijat_loppu + 80;
	u32 testaajat_loppu = testaajat_alku + 700;
	u32 kaantaja_alku   = testaajat_loppu + 100;
	u32 kaantaja_loppu  = kaantaja_alku + 300;
	//u32 sdl_alku        = kaantaja_loppu + 100;
	//u32 sdl_loppu       = sdl_alku + 400;

	PDraw::image_cutclip(bg_screen, 280, 80, 280, 80, 640, 480);

	if (intro_counter / 10 % 50 == 0)
		PDraw::image_cutclip(bg_screen,353, 313, 242, 313, 275, 432);

	if (intro_counter > pistelogo_alku && intro_counter < pistelogo_loppu) {

		//int x = intro_counter - pistelogo_alku - 194;
		int kerroin = 120 / (intro_counter - pistelogo_alku);
		int x = 120 - kerroin;

		if (x > 120)
			x = 120;

		int pros = pistelogo_loppu - intro_counter;

		int y = 230;
		if(pros < 100){
			//PDraw::image_cutcliptransparent(bg_screen, 32, 50, 160, 129, x, y, pros, 255);
			//PDraw::image_cutcliptransparent(bg_screen, 37, 376, 194, 66, /*120*/x,y + 146, pros, 192);

			PDraw::image_cutcliptransparent(bg_screen, 37, 230, 194 - 37, 443 - 230, x,y , pros, 255);
		}
		else{
			PDraw::image_cutclip(bg_screen,/*120*/x,y, 37, 230, 194, 443);
		}

		

		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_presents).c_str(), fontti1, 230, 400, pistelogo_alku, pistelogo_loppu-20, intro_counter);

	}

	if (intro_counter > tekijat_alku) {
		int x = 120;
		int y = 200;

		u32 start = tekijat_alku;
		u32 end = tekijat_loppu;

		/*CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_a_game_by).c_str(),fontti1, x, y, start, tekijat_loppu, intro_counter);
		
		y += SECTION_SEPARATOR;

		start +=20;
		end += 20;*/
		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_game_created_by), fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR_TITLE;
		start += 10;
		end += 10;

		CreditsText_Draw("janne kivilahti",			fontti1, x, y, start, end, intro_counter);
		y += SECTION_SEPARATOR;
		start += 20;
		end += 20;

		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_game_improved_by), fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR_TITLE;
		start += 10;
		end += 10;

		CreditsText_Draw("Danilo Lemos",			fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR;
		start += 10;
		end += 10;

		CreditsText_Draw("Saturnin The Alien",			fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR;
		start += 10;
		end += 10;

		CreditsText_Draw("Makyuni",			fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR;
		start += 10;
		end += 10;

		CreditsText_Draw("Rowland Gardner",			fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR;
		start += 10;
		end += 10;

		CreditsText_Draw("Yamano",			fontti1, x, y, start, end, intro_counter);
		y += SECTION_SEPARATOR;
		start += 20;
		end += 20;

		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_original), fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR_TITLE;
		start += 10;
		end += 10;

		CreditsText_Draw("antti suuronen 1998",		fontti1, x, y, start, end, intro_counter); // + 50
		y += LINE_SEPARATOR;
		start += 10;
		end += 10;
		
	}

	if (intro_counter > testaajat_alku) {
		int x = 120;
		int y = 178;

		u32 start = testaajat_alku;
		u32 end = testaajat_loppu;

		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_tested_by),fontti1, x, y, start, testaajat_loppu, intro_counter);
		y += LINE_SEPARATOR_TITLE;
		start += 10;
		end += 10;

		CreditsText_Draw("antti suuronen",			fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR;
		start += 10;
		end += 10;
		
		CreditsText_Draw("toni hurskainen",			fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR;
		start += 10;
		end += 10;
		
		CreditsText_Draw("juho rytkönen",				fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR;
		start += 10;
		end += 10;
		
		CreditsText_Draw("annukka korja",				fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR;
		start += 10;
		end += 10;
		
		CreditsText_Draw("Mipha",				fontti1, x, y, start, end, intro_counter);

		y += SECTION_SEPARATOR;
		start += 30;
		end += 30;
		
		
		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_thanks_to),fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR_TITLE;
		start += 10;
		end += 10;

		
		CreditsText_Draw("oskari raunio",				fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR;
		start += 10;
		end += 10;

		
		CreditsText_Draw("assembly organization",		fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR;
		start += 10;
		end += 10;

		CreditsText_Draw("Daeta",		fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR;
		start += 10;
		end += 10;

		CreditsText_Draw("Averno",		fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR;
		start += 10;
		end += 10;
		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_community),		fontti1, x, y, start, end, intro_counter);
		y += SECTION_SEPARATOR;
		start += 10;
		end += 10;

		CreditsText_Draw("powered by", fontti1, x, y, start, end, intro_counter);
		y += LINE_SEPARATOR_TITLE;
		start += 10;
		end += 10;
		CreditsText_Draw("SDL 2",  fontti1, x, y, start, end, intro_counter);
	}

	if (intro_counter > kaantaja_alku) {
		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_translation), fontti1, 120, 230, kaantaja_alku, kaantaja_loppu, intro_counter);
		CreditsText_Draw(tekstit->Get_Text(PK_txt.intro_translator),  fontti1, 120, 250, kaantaja_alku+20, kaantaja_loppu+20, intro_counter);
	}
}

void IntroScreen::Init() {

	TouchScreenControls.change(UI_TOUCH_TO_START);
	
	PDraw::set_offset(640, 480);

	std::optional<PFile::Path> intro_bmp = PFilesystem::FindVanillaAsset("intro.bmp", PFilesystem::GFX_DIR);
	if(!intro_bmp.has_value()){
		throw std::runtime_error("\"intro.bmp\" not found!");
	}

	PDraw::image_load_with_palette(bg_screen, default_palette, *intro_bmp, false);
	PDraw::palette_set(default_palette);

	std::optional<PFile::Path> intro_xm = PFilesystem::FindVanillaAsset("intro.xm", PFilesystem::MUSIC_DIR);
	if(!intro_xm.has_value()){
		throw std::runtime_error("\"intro.xm\" not found!");
	}

	if (PSound::start_music(*intro_xm) == -1){
		PLog::Write(PLog::ERR, "PK2", "Can't load intro.xm");
	}

	PSound::set_musicvolume(Settings.music_max_volume);

	intro_counter = 0;

	Fade_in(FADE_FAST);
}

void IntroScreen::Loop() {
	
	Draw();

	degree = 1 + degree % 360;

	intro_counter++;

	if (!closing_intro) {

		if (TouchScreenControls.touch || intro_counter >= 2930){
			closing_intro = true;
			Fade_out(FADE_SLOW);
		}

	} else {

		if (!Is_Fading()){
			next_screen = SCREEN_MENU;
		}

	}
}

void IntroScreen::onKeyPressed(const PInput::Key& key){
	if(!closing_intro){
		closing_intro = true;
		Fade_out(FADE_SLOW);
	}
}