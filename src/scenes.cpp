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
	PisteDraw2_FadeIn(PD_FADE_FAST);
}

//PK_Draw_Map
int PK_Piirra_Kartta(){
	char luku[20];
	int vali = 20;

	PisteDraw2_ScreenFill(0);
	PisteDraw2_Image_Clip(kuva_tausta,0,0);

	PisteDraw2_Font_Write(fontti4,episodi,100+2,72+2);
	PisteDraw2_Font_Write(fontti2,episodi,100,72);

	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_map_total_score),100+2,92+2);
	vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_map_total_score),100,92);//250,80
	ltoa(pisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,100+vali+2+15,92+2);
	PisteDraw2_Font_Write(fontti2,luku,100+vali+15,92);

	if (episodipisteet.episodin_top_pisteet > 0) {
		vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_map_episode_best_player),360,72);
		PisteDraw2_Font_Write(fontti1,episodipisteet.episodin_top_pelaaja,360+vali+10,72);
		vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_map_episode_hiscore),360,92);
		ltoa(episodipisteet.episodin_top_pisteet,luku,10);
		PisteDraw2_Font_Write(fontti2,luku,360+vali+15,92);
	}

	vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_map_next_level),100,120);
	ltoa(jakso,luku,10);
	PisteDraw2_Font_Write(fontti1,luku,100+vali+15,120);

		//PK_Partikkelit_Piirra();

	if (jaksoja == 0) {
		PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_episodes_no_maps),180,290);
	}

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_return),100,430)){
		pelin_seuraava_tila = TILA_MENUT;
		menu_nyt = MENU_PAAVALIKKO;
	}

	int nuppi_x = 0, nuppi_y = 0;
	int tyyppi = 0;
	int paluu;
	int min = 0, sek = 0;
	int ikoni;
	int sinx = 0, cosy = 0;
	int pekkaframe = 0;


	for (int i=0;i<=jaksoja;i++) {
		if (strcmp(jaksot[i].nimi,"")!=0 && jaksot[i].jarjestys > 0) {
			tyyppi = 0;							//0 harmaa
			if (jaksot[i].jarjestys == jakso)
				tyyppi = 1;						//1 vihre�
			if (jaksot[i].jarjestys > jakso)
				tyyppi = 2;						//2 oranssi
			if (jaksot[i].lapaisty)
				tyyppi = 0;

			if (jaksot[i].x == 0)
				jaksot[i].x = 142+i*30;

			if (jaksot[i].y == 0)
				jaksot[i].y = 270;

			ikoni = jaksot[i].ikoni;

			//PisteDraw2_Image_Clip(kuva_peli,jaksot[i].x-4,jaksot[i].y-4-30,1+(ikoni*27),452,27+(ikoni*27),478);
			PisteDraw2_Image_CutClip(kuva_peli,jaksot[i].x-9,jaksot[i].y-14,1+(ikoni*28),452,28+(ikoni*28),479);

			if (tyyppi==1) {
				//PisteDraw2_Image_Clip(kuva_peli,jaksot[i].x-30,jaksot[i].y-4,157,46,181,79);
				sinx = (int)(sin_table[degree%360]/2);
				cosy = (int)(cos_table[degree%360]/2);
				pekkaframe = 28*((degree%360)/120);
				PisteDraw2_Image_CutClip(kuva_peli,jaksot[i].x+sinx-12,jaksot[i].y-17+cosy,157+pekkaframe,46,181+pekkaframe,79);
			}

			paluu = PK_Piirra_Nuppi(jaksot[i].x-5, jaksot[i].y-10, tyyppi);

			// jos klikattu
			if (paluu == 2) {
				if (tyyppi < 2 || (dev_mode && tyyppi > 0)) {
					strcpy(seuraava_kartta,jaksot[i].tiedosto);
					//jakso = i;
					jakso_indeksi_nyt = i;
					siirry_kartasta_peliin = true;
					PisteDraw2_FadeOut(PD_FADE_SLOW);
					musiikin_voimakkuus = 0;
					PK_Soita_Aani_Menu(kieku_aani,90);
				}
				else
					PK_Soita_Aani_Menu(ammuu_aani,100);
			}

			itoa(jaksot[i].jarjestys,luku,10);
			PisteDraw2_Font_Write(fontti1,luku,jaksot[i].x-12+2,jaksot[i].y-29+2);
			//PisteDraw2_Font_Write(fontti2,luku,jaksot[i].x+3,jaksot[i].y-27);


			if (paluu > 0) {

				int info_x = 489+3, info_y = 341-26;
				/*
				PisteDraw2_ScreenFill(jaksot[i].x-3, jaksot[i].y+26,jaksot[i].x + 130, jaksot[i].y+26+120,1);
				PisteDraw2_ScreenFill(jaksot[i].x-2, jaksot[i].y+27,jaksot[i].x + 129, jaksot[i].y+25+120,43);
				PisteDraw2_ScreenFill(jaksot[i].x-1, jaksot[i].y+28,jaksot[i].x + 128, jaksot[i].y+24+120,38);*/
				PisteDraw2_Image_CutClip(kuva_peli,info_x-3,info_y+26,473,0,607,121);
				PisteDraw2_Font_Write(fontti1,jaksot[i].nimi,info_x,info_y+30);

				if (episodipisteet.pisteet[i] > 0) {
					PisteDraw2_Font_WriteAlpha(fontti1,tekstit->Hae_Teksti(txt_map_level_best_player),info_x,info_y+50,75);
					PisteDraw2_Font_Write(fontti1,episodipisteet.top_pelaajat[i],info_x,info_y+62);
					vali = 8 + PisteDraw2_Font_WriteAlpha(fontti1,tekstit->Hae_Teksti(txt_map_level_hiscore),info_x,info_y+74,75);
					ltoa(episodipisteet.pisteet[i],luku,10);
					PisteDraw2_Font_Write(fontti1,luku,info_x+vali,info_y+75);
				}

				if (episodipisteet.ajat[i] > 0) {
					PisteDraw2_Font_WriteAlpha(fontti1,tekstit->Hae_Teksti(txt_map_level_fastest_player),info_x,info_y+98,75);
					PisteDraw2_Font_Write(fontti1,episodipisteet.nopeimmat_pelaajat[i],info_x,info_y+110);

					vali = 8 + PisteDraw2_Font_WriteAlpha(fontti1,tekstit->Hae_Teksti(txt_map_level_best_time),info_x,info_y+122,75);
					min = episodipisteet.ajat[i]/60;
					sek = episodipisteet.ajat[i]%60;

					itoa(min,luku,10);
					vali += PisteDraw2_Font_Write(fontti1,luku,info_x+vali,info_y+122);
					vali += PisteDraw2_Font_Write(fontti1,":",info_x+vali,info_y+122);
					itoa(sek,luku,10);
					PisteDraw2_Font_Write(fontti1,luku,info_x+vali,info_y+122);
				}
			}
		}
	}

	PK_Piirra_Kursori(hiiri_x,hiiri_y);

	return 0;
}
int PK_Main_Kartta(){
	PK_Piirra_Kartta();

	degree = 1 + degree % 360;

	if (siirry_kartasta_peliin && !PisteDraw2_IsFading()) {
		pelin_seuraava_tila = TILA_PELI;
		//strcpy(seuraava_kartta,jaksot[i].tiedosto);
		peli_kesken = false;//true;
		//PisteDraw2_FadeIn(PD_FADE_NORMAL);
	}

	if (key_delay > 0)
		key_delay--;

	return 0;
}
void PK_Start_Map(){

	if (!peli_kesken)
	{
		if (lataa_peli != -1)
		{
			//PisteLog_Kirjoita("  - Loading a saved game \n");
			PK_Jaksot_Hae();

			for (int j = 0;j < EPISODI_MAX_JAKSOJA;j++)
				 jaksot[j].lapaisty = tallennukset[lataa_peli].jakso_lapaisty[j];

			lataa_peli = -1;
			peli_kesken = true;
			peli_ohi = true;
			jakso_lapaisty = true;
			lopetusajastin = 0;

		}
		else
		{
			//PisteLog_Kirjoita("  - Starting a new game \n");
			PK_Jaksot_Alusta();	// jos ladataan peli, asetetaan l�p�istyarvot jaksoille aikaisemmin
			PK_Jaksot_Hae();
		}

		//PisteLog_Kirjoita("  - Loading top scores \n");
		char topscoretiedosto[_MAX_PATH] = "scores.dat";
		PK_Episodipisteet_Lataa(topscoretiedosto);
	}

	/* Ladataan kartan taustakuva ...*/
	char mapkuva[_MAX_PATH] = "map.bmp";
	PK_Lisaa_Episodin_Hakemisto(mapkuva);
	if(!PisteUtils_CheckFile(mapkuva)) 	strcpy(mapkuva, "gfx/map.bmp")

	char mapmusa[_MAX_PATH] = "map.mp3";
	do {
		PK_Lisaa_Episodin_Hakemisto(mapmusa);
		if(PisteUtils_CheckFile(mapmusa)) break;
		strcpy(mapmusa,"map.ogg");
		PK_Lisaa_Episodin_Hakemisto(mapmusa);
		if(PisteUtils_CheckFile(mapmusa)) break;
		strcpy(mapmusa,"map.xm");
		PK_Lisaa_Episodin_Hakemisto(mapmusa);
		if(PisteUtils_CheckFile(mapmusa)) break;
		strcpy(mapmusa,"map.mod");
		PK_Lisaa_Episodin_Hakemisto(mapmusa);
		if (PisteUtils_CheckFile(mapmusa)) break;
		strcpy(mapmusa,"map.it");
		PK_Lisaa_Episodin_Hakemisto(mapmusa);
		if (PisteUtils_CheckFile(mapmusa)) break;
		strcpy(mapmusa,"map.s3m");
		PK_Lisaa_Episodin_Hakemisto(mapmusa);
		if (PisteUtils_CheckFile(mapmusa)) break;
		strcpy(mapmusa,"music/map.mp3");
		if (PisteUtils_CheckFile(mapmusa)) break;
		strcpy(mapmusa,"music/map.ogg");
		if (PisteUtils_CheckFile(mapmusa)) break;
		strcpy(mapmusa,"music/map.xm");
		break;
	} while(0);

	StartScene(mapkuva, mapmusa);

	siirry_kartasta_peliin = false; // TODO - means?

	PisteDraw2_FadeIn(PD_FADE_SLOW);
}

int PK_Piirra_Pistelasku(){
	char luku[20];
	int vali = 20;
	int my = 0, x, y;
	int kuutio, aste;
	int	vari = 0, kerroin;

	PisteDraw2_ScreenFill(0);
	PisteDraw2_Image_Clip(kuva_tausta,0,0);

	for (aste=0;aste<18;aste++) {

		kerroin = (int)(cos_table[(degree+aste*3)%180]);

		x = (int)(sin_table[(degree+aste*10)%360]*2)+kerroin;
		y = (int)(cos_table[(degree+aste*10)%360]*2);//10 | 360 | 2
		//PisteDraw2_Image_Clip(kuva_peli,320+x,240+y,157,46,181,79);
		kuutio = (int)(sin_table[(degree+aste*3)%360]);
		if (kuutio < 0) kuutio = -kuutio;

		PisteDraw2_ScreenFill(320-x,240-y,320-x+kuutio,240-y+kuutio,VARI_TURKOOSI+8);
	}
	for (aste=0;aste<18;aste++) {

		x = (int)(sin_table[(degree+aste*10)%360]*3);
		y = (int)(cos_table[(degree+aste*10)%360]*3);//10 | 360 | 3
		//PisteDraw2_Image_Clip(kuva_peli,320+x,240+y,157,46,181,79);
		kuutio = (int)(sin_table[(degree+aste*2)%360])+18;
		if (kuutio < 0) kuutio = -kuutio;//0;//
		if (kuutio > 100) kuutio = 100;

		//PisteDraw2_ScreenFill(320+x,240+y,320+x+kuutio,240+y+kuutio,VARI_TURKOOSI+10);
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 247, 1, 25, 25, 320+x, 240+y, kuutio, 32);
	}

	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_title),100+2,72+2);
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_title),100,72);
	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_level_score),100+2,102+2);
	vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_level_score),100,102);//250,80
	fake_pisteet = bonuspisteet + aikapisteet + energiapisteet + esinepisteet + pelastuspisteet;
	ltoa(fake_pisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,400+2,102+2);
	PisteDraw2_Font_Write(fontti2,luku,400,102);
	my = 0;

	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_bonus_score),100+2,192+2+my);
	vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_bonus_score),100,192+my);
	ltoa(bonuspisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,400+2,192+2+my);
	PisteDraw2_Font_Write(fontti2,luku,400,192+my);
	my += 30;

	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_time_score),100+2,192+2+my);
	vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_time_score),100,192+my);
	ltoa(aikapisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,400+2,192+2+my);
	PisteDraw2_Font_Write(fontti2,luku,400,192+my);
	my += 30;

	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_energy_score),100+2,192+2+my);
	vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_energy_score),100,192+my);
	ltoa(energiapisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,400+2,192+2+my);
	PisteDraw2_Font_Write(fontti2,luku,400,192+my);
	my += 30;

	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_item_score),100+2,192+2+my);
	vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_item_score),100,192+my);
	ltoa(esinepisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,400+2,192+2+my);
	PisteDraw2_Font_Write(fontti2,luku,400,192+my);
	my += 30;

	x = 110;
	y = 192 + my;

	for (int i=0;i<MAX_ESINEITA;i++)
		if (esineet[i] != NULL)	{
			esineet[i]->Piirra(x-esineet[i]->leveys/2,y-esineet[i]->korkeus/2,0);
			x += 38;
		}

	my += 10;

	if (pistelaskuvaihe >= 4){
		PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_total_score),100+2,192+2+my);
		vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_total_score),100,192+my);//250,80
		ltoa(pisteet,luku,10);
		PisteDraw2_Font_Write(fontti4,luku,400+2,192+2+my);
		PisteDraw2_Font_Write(fontti2,luku,400,192+my);
		my += 25;

		if (jakso_uusi_ennatys) {
			PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_new_level_hiscore),100+rand()%2,192+my+rand()%2);
			my += 25;
		}
		if (jakso_uusi_ennatysaika) {
			PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_new_level_best_time),100+rand()%2,192+my+rand()%2);
			my += 25;
		}
		if (episodi_uusi_ennatys) {
			PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_new_episode_hiscore),100+rand()%2,192+my+rand()%2);
			my += 25;
		}
	}

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_score_screen_continue),100,430)){
		musiikin_voimakkuus = 0;
		siirry_pistelaskusta_karttaan = true;
		PisteDraw2_FadeOut(PD_FADE_SLOW);
		//pelin_seuraava_tila = TILA_KARTTA;
	}

	PK_Piirra_Kursori(hiiri_x,hiiri_y);

	return 0;
}
int PK_Main_Pistelasku(){
	PK_Piirra_Pistelasku();

	degree = 1 + degree % 360;

	// PISTELASKU

	int energia = spritet[pelaaja_index].energia;

	if (pistelaskudelay == 0)
		if (bonuspisteet < jakso_pisteet){
			pistelaskuvaihe = 1;
			pistelaskudelay = 0;
			bonuspisteet += 10;

			if (degree%7==1)
				PK_Soita_Aani_Menu(pistelaskuri_aani, 70);

			if (bonuspisteet >= jakso_pisteet){
				bonuspisteet = jakso_pisteet;
				pistelaskudelay = 50;
			}

		}
		else if (aika > 0){
			pistelaskuvaihe = 2;
			pistelaskudelay = 0;
			aikapisteet+=5;
			aika--;

			if (degree%10==1)
				PK_Soita_Aani_Menu(pistelaskuri_aani, 70);

			if (aika == 0)
				pistelaskudelay = 50;

		}
		else if (spritet[pelaaja_index].energia > 0){
			pistelaskuvaihe = 3;
			pistelaskudelay = 10;
			energiapisteet+=300;
			spritet[pelaaja_index].energia--;

			PK_Soita_Aani_Menu(pistelaskuri_aani, 70);

		}
		else if (esineita > 0){
			pistelaskuvaihe = 4;
			pistelaskudelay = 30;
			for (int i=0;i<MAX_ESINEITA;i++)
				if (esineet[i] != NULL)
				{
					esinepisteet += esineet[i]->pisteet + 500;
					esineet[i] = NULL;
					PK_Soita_Aani_Menu(hyppy_aani, 100);
					break;
				}
		}
		else pistelaskuvaihe = 5;

	if (pistelaskudelay > 0)
		pistelaskudelay--;

	if (siirry_pistelaskusta_karttaan && !PisteDraw2_IsFading()){
		/*tarkistetaan oliko viimeinen jakso*/

		if (jakso_indeksi_nyt == EPISODI_MAX_JAKSOJA-1) { // ihan niin kuin joku tekisi n�in monta jaksoa...
			pelin_seuraava_tila = TILA_LOPPU;
		}
		else if (jaksot[jakso_indeksi_nyt+1].jarjestys == -1)
			    pelin_seuraava_tila = TILA_LOPPU;
		else // jos ei niin palataan karttaan...
		{
			pelin_seuraava_tila = TILA_KARTTA;
			//peli_kesken = false;
			menu_nyt = MENU_PAAVALIKKO;
		}
	}

	if (key_delay > 0)
		key_delay--;

	if (key_delay == 0){
		if (PisteInput_Keydown(PI_RETURN) && pistelaskuvaihe == 5){
			siirry_pistelaskusta_karttaan = true;
			musiikin_voimakkuus = 0;
			PisteDraw2_FadeOut(PD_FADE_SLOW);
			key_delay = 20;
		}

		if (PisteInput_Keydown(PI_RETURN) && pistelaskuvaihe < 5){
			pistelaskuvaihe = 5;
			bonuspisteet = jakso_pisteet;
			aikapisteet += aika*5;
			aika = 0;
			energiapisteet += spritet[pelaaja_index].energia * 300;
			spritet[pelaaja_index].energia = 0;
			for (int i=0;i<MAX_ESINEITA;i++)
				if (esineet[i] != NULL)
					esinepisteet += esineet[i]->pisteet + 500;
			esineita = 0;

			key_delay = 20;
		}
	}

	return 0;
}
void PK_Start_Scores(){
	StartScene("gfx/menu.bmp", "music/hiscore.xm");

	jakso_uusi_ennatys = false;
	jakso_uusi_ennatysaika = false;
	episodi_uusi_ennatys = false;

	// Lasketaan pelaajan kokonaispisteet etuk�teen
	DWORD temp_pisteet = 0;
	temp_pisteet += jakso_pisteet;
	temp_pisteet += aika*5;
	temp_pisteet += spritet[pelaaja_index].energia * 300;
	for (int i=0;i<MAX_ESINEITA;i++)
		if (esineet[i] != NULL)
			temp_pisteet += esineet[i]->pisteet + 500;

	//if (jaksot[jakso_indeksi_nyt].lapaisty)
	//if (jaksot[jakso_indeksi_nyt].jarjestys == jakso-1)
	pisteet += temp_pisteet;

	if (uusinta)
		pisteet -= temp_pisteet;

	fake_pisteet = 0;
	pistelaskuvaihe = 0;
	pistelaskudelay = 30;
	bonuspisteet = 0,
	aikapisteet = 0,
	energiapisteet = 0,
	esinepisteet = 0,
	pelastuspisteet = 0;

	char pisteet_tiedosto[_MAX_PATH] = "scores.dat";
	int vertailun_tulos;

	/* Tutkitaan onko pelaajarikkonut kent�n piste- tai nopeusenn�tyksen */
	vertailun_tulos = PK_Episodipisteet_Vertaa(jakso_indeksi_nyt,temp_pisteet,kartta->aika-aika,false);
	if (vertailun_tulos > 0) {
		PK_Episodipisteet_Tallenna(pisteet_tiedosto);
	}

	/* Tutkitaan onko pelaaja rikkonut episodin piste-enn�tyksen */
	vertailun_tulos = PK_Episodipisteet_Vertaa(0,pisteet,0,true);
	if (vertailun_tulos > 0)
		PK_Episodipisteet_Tallenna(pisteet_tiedosto);

	siirry_pistelaskusta_karttaan = false;

	PisteDraw2_FadeIn(PD_FADE_FAST);
}
