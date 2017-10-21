#include "scenes.h"

#include "PisteDraw.h"
#include "PisteSound.h"
#include "PisteInput.h"

#include "graphics.h"

bool siirry_introsta_menuun = false;
bool siirry_lopusta_menuun = false;

int introlaskuri = 0;
int loppulaskuri = 0;

void StartScene(const char* background, const char* music){
	PisteDraw2_SetXOffset(80);
	PisteDraw2_ScreenFill(0);
	PisteDraw2_Image_Delete(kuva_tausta);
	kuva_tausta = PisteDraw2_Image_Load(background,true);

	if (PisteSound_StartMusic(music)!=0)
		PK2_virhe = true;

	musiikin_voimakkuus = musiikin_max_voimakkuus;

	introlaskuri = 0;
	siirry_pistelaskusta_karttaan = false;

	PisteDraw2_FadeIn(PD_FADE_FAST);
}

int DrawEndGame_Image(int x, int y, int tyyppi, int plus, int rapytys){
	int frm = 0;
	int yk = 0;

	if (tyyppi == 1){ // Pekka
		frm = 1;
		if ((degree/10)%10==rapytys) frm = 0;
		yk = (int)sin_table[(degree%360)];
		PisteDraw2_Image_CutClip(kuva_tausta,x+3,y+56, 4, 63, 29, 69);
		if (yk < 0){
			y+=yk;
			frm = 2;
		}
		PisteDraw2_Image_CutClip(kuva_tausta,x,y, 1+frm*35, 1, 32+frm*35, 59);
	}

	if (tyyppi == 2){ // kana (katse oikealle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)cos_table[((degree+plus)%360)];
		PisteDraw2_Image_CutClip(kuva_tausta,x+3,y+36, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PisteDraw2_Image_CutClip(kuva_tausta,x,y, 106+frm*37, 1, 139+frm*37, 38);
	}

	if (tyyppi == 3){ // kana (katse vasemmalle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)cos_table[((degree+plus)%360)];
		PisteDraw2_Image_CutClip(kuva_tausta,x+3,y+36, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PisteDraw2_Image_CutClip(kuva_tausta,x,y, 106+frm*37, 41, 139+frm*37, 77);
	}

	if (tyyppi == 4){ // pikkukana (katse oikealle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)sin_table[(((degree*2)+plus)%360)];
		PisteDraw2_Image_CutClip(kuva_tausta,x+3,y+27, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PisteDraw2_Image_CutClip(kuva_tausta,x,y, 217+frm*29, 1, 243+frm*29, 29);
	}

	if (tyyppi == 5){ // pikkukana (katse vasemmalle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)sin_table[(((degree*2)+plus)%360)];
		PisteDraw2_Image_CutClip(kuva_tausta,x,y+27, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PisteDraw2_Image_CutClip(kuva_tausta,x,y, 217+frm*29, 33, 243+frm*29, 61);
	}

	return 0;
}
int DrawEndGame(){

	DWORD onnittelut_alku	= 300;
	DWORD onnittelut_loppu	= onnittelut_alku + 1000;
	DWORD the_end_alku		= onnittelut_loppu + 80;
	DWORD the_end_loppu		= the_end_alku + 3000;

	PisteDraw2_ScreenFill(0);
	PisteDraw2_Image_CutClip(kuva_tausta,320-233/2,240-233/2, 6, 229, 239, 462);

	DrawEndGame_Image(345, 244, 3, 30, 2);
	DrawEndGame_Image(276, 230, 2, 50, 3);
	DrawEndGame_Image(217, 254, 4, 0, 4);

	DrawEndGame_Image(305, 240, 1, 0, 1);

	DrawEndGame_Image(270, 284, 2, 20, 1);
	DrawEndGame_Image(360, 284, 5, 60, 2);

	if (loppulaskuri > onnittelut_alku) {
		DrawIntro_Text(tekstit->Hae_Teksti(txt_end_congratulations), fontti2, 220, 380, onnittelut_alku, onnittelut_loppu);
		DrawIntro_Text(tekstit->Hae_Teksti(txt_end_chickens_saved), fontti1, 220, 402, onnittelut_alku+30, onnittelut_loppu+30);
	}
	if (loppulaskuri > the_end_alku) {
		DrawIntro_Text(tekstit->Hae_Teksti(txt_end_the_end), fontti2, 280, 190, the_end_alku, the_end_loppu);
	}

	return 0;
}

void DrawIntro_Text(char *teksti, int fontti, int x, int y, DWORD alkuaika, DWORD loppuaika){
	int pros = 100;
	if (introlaskuri > alkuaika && introlaskuri < loppuaika) {

		if (introlaskuri - alkuaika < 100)
			pros = introlaskuri - alkuaika;

		if (loppuaika - introlaskuri < 100)
			pros = loppuaika - introlaskuri;

		if (pros > 0) {
			if (pros < 100)
				PisteDraw2_Font_WriteAlpha(fontti,teksti,x,y,pros);
			else
				PisteDraw2_Font_Write(fontti,teksti,x,y);
		}

	}
}
int DrawIntro(){

	DWORD pistelogo_alku	= 300;
	DWORD pistelogo_loppu	= pistelogo_alku + 500;
	DWORD tekijat_alku		= pistelogo_loppu + 80;
	DWORD tekijat_loppu		= tekijat_alku + 720;
	DWORD testaajat_alku	= tekijat_loppu + 80;
	DWORD testaajat_loppu	= testaajat_alku + 700;
	DWORD kaantaja_alku		= testaajat_loppu + 100;
	DWORD kaantaja_loppu	= kaantaja_alku + 300;

	PisteDraw2_ScreenFill(0);

	PisteDraw2_Image_CutClip(kuva_tausta,280,80, 280, 80, 640, 480);

	if ((introlaskuri / 10) % 50 == 0)
		PisteDraw2_Image_CutClip(kuva_tausta,353, 313, 242, 313, 275, 432);

	if (introlaskuri > pistelogo_alku && introlaskuri < pistelogo_loppu) {

		//int x = introlaskuri - pistelogo_alku - 194;
		int kerroin = 120 / (introlaskuri - pistelogo_alku);
		int x = 120 - kerroin;

		if (x > 120)
			x = 120;

		PisteDraw2_Image_CutClip(kuva_tausta,/*120*/x,230, 37, 230, 194, 442);

		DrawIntro_Text(tekstit->Hae_Teksti(txt_intro_presents), fontti1, 230, 400, pistelogo_alku, pistelogo_loppu-20);

	}

	if (introlaskuri > tekijat_alku) {
		DrawIntro_Text(tekstit->Hae_Teksti(txt_intro_a_game_by),fontti1, 120, 200, tekijat_alku, tekijat_loppu);
		DrawIntro_Text("janne kivilahti 2003",		            fontti1, 120, 220, tekijat_alku+20, tekijat_loppu+20);
		DrawIntro_Text(tekstit->Hae_Teksti(txt_intro_original), fontti1, 120, 245, tekijat_alku+40, tekijat_loppu+40);
		DrawIntro_Text("antti suuronen 1998",		            fontti1, 120, 265, tekijat_alku+50, tekijat_loppu+50);
		DrawIntro_Text("sdl porting by",		                fontti1, 120, 290, tekijat_alku+70, tekijat_loppu+70);
		DrawIntro_Text("samuli tuomola 2010",		            fontti1, 120, 310, tekijat_alku+80, tekijat_loppu+80);
		DrawIntro_Text("sdl2 port and bug fixes",               fontti1, 120, 335, tekijat_alku + 90, tekijat_loppu + 90);
		DrawIntro_Text("danilo lemos 2017",                     fontti1, 120, 355, tekijat_alku + 100, tekijat_loppu + 100);
	}

	if (introlaskuri > testaajat_alku) {
		DrawIntro_Text(tekstit->Hae_Teksti(txt_intro_tested_by),fontti1, 120, 230, testaajat_alku, testaajat_loppu);
		DrawIntro_Text("antti suuronen",			fontti1, 120, 250, testaajat_alku+10, testaajat_loppu+10);
		DrawIntro_Text("toni hurskainen",			fontti1, 120, 260, testaajat_alku+20, testaajat_loppu+20);
		DrawIntro_Text("juho rytk�nen",				fontti1, 120, 270, testaajat_alku+30, testaajat_loppu+30);
		DrawIntro_Text("annukka korja",				fontti1, 120, 280, testaajat_alku+40, testaajat_loppu+40);
		DrawIntro_Text(tekstit->Hae_Teksti(txt_intro_thanks_to),fontti1, 120, 300, testaajat_alku+70, testaajat_loppu+70);
		DrawIntro_Text("oskari raunio",				fontti1, 120, 310, testaajat_alku+70, testaajat_loppu+70);
		DrawIntro_Text("assembly organization",		fontti1, 120, 320, testaajat_alku+70, testaajat_loppu+70);
	}

	if (introlaskuri > kaantaja_alku) {
		DrawIntro_Text(tekstit->Hae_Teksti(txt_intro_translation), fontti1, 120, 230, kaantaja_alku, kaantaja_loppu);
		DrawIntro_Text(tekstit->Hae_Teksti(txt_intro_translator),  fontti1, 120, 250, kaantaja_alku+20, kaantaja_loppu+20);
	}
	return 0;
}

int PK_Main_Intro(){
	DrawIntro();

	degree = 1 + degree % 360;

	introlaskuri++;

	if (siirry_introsta_menuun && !PisteDraw2_IsFading()){
		pelin_seuraava_tila = TILA_MENUT;
		peli_kesken = false;
	}
	if (key_delay > 0) key_delay--;
	if (key_delay == 0)
		if (PisteInput_Keydown(PI_RETURN) || PisteInput_Keydown(PI_SPACE) || introlaskuri == 3500){
			siirry_introsta_menuun = true;
			PisteDraw2_FadeOut(PD_FADE_SLOW);
		}

	return 0;
}
void PK_Start_Intro(){
	StartScene("gfx/intro.bmp", "music/intro.xm");
}

int PK_Main_Loppu(){
	DrawEndGame();

	degree = 1 + degree % 360;

	loppulaskuri++;
	introlaskuri = loppulaskuri; // introtekstej� varten

	if (siirry_lopusta_menuun && !PisteDraw2_IsFading()){
		pelin_seuraava_tila = TILA_MENUT;
		menu_nyt = MENU_PAAVALIKKO;
		peli_kesken = false;
	}

	if (key_delay > 0) key_delay--;

	if (key_delay == 0){
		if (PisteInput_Keydown(PI_RETURN) || PisteInput_Keydown(PI_SPACE)){
			siirry_lopusta_menuun = true;
			musiikin_voimakkuus = 0;
			PisteDraw2_FadeOut(PD_FADE_SLOW);
		}
	}

	return 0;
}
void PK_Start_EndGame(){
	StartScene("gfx/ending.bmp", "music/intro.xm");
}
