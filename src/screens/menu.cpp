
#include "screens.hpp"

#include "settings.hpp"
#include "gui.hpp"
#include "game.hpp"
#include "text.hpp"
#include "sfx.hpp"
#include "language.hpp"
#include "episode.hpp"
#include "save.hpp"

#include "PisteDraw.hpp"
#include "PisteInput.hpp"
#include "PisteUtils.hpp"
#include "PisteSound.hpp"

#include <cstring>

enum MENU{
	MENU_MAIN,
	MENU_EPISODES,
	MENU_CONTROLS,
	MENU_GRAPHICS,
	MENU_SOUNDS,
	MENU_NAME,
	MENU_LOAD,
	MENU_TALLENNA,
	MENU_LANGUAGE
};

struct MENU_RECT {
	int left, right;
	int top, bottom;
} menunelio;

int menu_nyt = MENU_MAIN;
int menu_lue_kontrollit = 0;
int menu_name_index = 0;
char menu_name_last_mark = '\0';
int menu_valittu_id = 0;
int menu_valinta_id = 1;

bool nimiedit = false;

int PK_Draw_Menu_Square(int vasen, int yla, int oikea, int ala, BYTE pvari){
	if (episode_started)
		return 0;

	//pvari = 224;

	if (menunelio.left < vasen)
		menunelio.left++;

	if (menunelio.left > vasen)
		menunelio.left--;//TODO - ERROR
		menunelio.top++;

	if (menunelio.top > yla)
		menunelio.top--;

	if (menunelio.bottom < ala)
		menunelio.bottom++;

	if (menunelio.bottom > ala)
		menunelio.bottom--;

	vasen = (int)menunelio.left;
	oikea = (int)menunelio.right;
	yla	= (int)menunelio.top;
	ala = (int)menunelio.bottom;

	vasen += (int)(sin_table[(degree*2)%359]/2.0);
	oikea += (int)(cos_table[(degree*2)%359]/2.0);
	yla	+= (int)(sin_table[((degree*2)+20)%359]/2.0);
	ala += (int)(cos_table[((degree*2)+40)%359]/2.0);

	//PDraw::screen_fill(vasen,yla,oikea,ala,38);

	double kerroin_y = (ala - yla) / 19.0;
	double kerroin_x = (oikea - vasen) / 19.0;
	double dbl_y = yla;
	double dbl_x = vasen;
	bool tumma = true;
	int vari = 0;

	for (int y=0;y<19;y++) {

		dbl_x = vasen;

		for (int x=0;x<19;x++) {
			//vari = (x+y) / 6;
			vari = (x/4)+(y/3);
			if (tumma) vari /= 2;

			PDraw::screen_fill(int(dbl_x),int(dbl_y),int(dbl_x+kerroin_x),int(dbl_y+kerroin_y),pvari+vari);
			dbl_x += kerroin_x;
			tumma = !tumma;
		}
		dbl_y += kerroin_y;
	}

	PDraw::screen_fill(vasen-1,yla-1,oikea+1,yla+2,0);
	PDraw::screen_fill(vasen-1,yla-1,vasen+2,ala+1,0);
	PDraw::screen_fill(vasen-1,ala-2,oikea+1,ala+1,0);
	PDraw::screen_fill(oikea-2,yla-1,oikea+1,ala+1,0);

	PDraw::screen_fill(vasen-1+1,yla-1+1,oikea+1+1,yla+2+1,0);
	PDraw::screen_fill(vasen-1+1,yla-1+1,vasen+2+1,ala+1+1,0);
	PDraw::screen_fill(vasen-1+1,ala-2+1,oikea+1+1,ala+1+1,0);
	PDraw::screen_fill(oikea-2+1,yla-1+1,oikea+1+1,ala+1+1,0);

	PDraw::screen_fill(vasen,yla,oikea,yla+1,153);
	PDraw::screen_fill(vasen,yla,vasen+1,ala,144);
	PDraw::screen_fill(vasen,ala-1,oikea,ala,138);
	PDraw::screen_fill(oikea-1,yla,oikea,ala,138);

	return 0;
}
bool Draw_Menu_Text(bool active, char *teksti, int x, int y){
	if(!active){
		WavetextSlow_Draw(teksti, fontti2, x, y);
		return false;
	}

	int pituus = strlen(teksti)*15;

	if ((hiiri_x > x && hiiri_x < x+pituus && hiiri_y > y && hiiri_y < y+15) ||
		(menu_valittu_id == menu_valinta_id)){
		menu_valittu_id = menu_valinta_id;

		if ((
			(PisteInput_Hiiri_Vasen() && hiiri_x > x && hiiri_x < x+pituus && hiiri_y > y && hiiri_y < y+15)
			|| PisteInput_Keydown(PI_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1))
			&& key_delay == 0){
			Play_MenuSFX(menu_aani, 100);
			key_delay = 20;
			menu_valinta_id++;
			return true;
		}

		Wavetext_Draw(teksti, fontti3, x, y);
	}
	else
		WavetextSlow_Draw(teksti, fontti2, x, y);

	menu_valinta_id++;

	return false;
}
int  PK_Draw_Menu_BoolBox(int x, int y, bool muuttuja, bool active){
	PDraw::RECT img_src, img_dst = {(DWORD)x,(DWORD)y,0,0};

	if(muuttuja) img_src = {504,124,31,31};
	else img_src = {473,124,31,31};

	if(active){
		PDraw::image_cutclip(kuva_peli,img_src,img_dst);
	} else{
		PDraw::image_cutcliptransparent(kuva_peli,img_src,img_dst,50);
		return false;
	}

	if (hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31){
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1))
			&& key_delay == 0){

			Play_MenuSFX(menu_aani, 100);
			key_delay = 20;
			return true;
		}
	}

	return false;
}
int  PK_Draw_Menu_BackNext(int x, int y){
	int val = 45;

	int randx = rand()%3 - rand()%3;
	int randy = rand()%3 - rand()%3;

	if (menu_valittu_id == menu_valinta_id)
		PDraw::image_cutclip(kuva_peli,x+randx,y+randy,566,124,566+31,124+31);
	else
		PDraw::image_cutclip(kuva_peli,x,y,566,124,566+31,124+31);

	if (menu_valittu_id == menu_valinta_id+1)
		PDraw::image_cutclip(kuva_peli,x+val+randx,y+randy,535,124,535+31,124+31);
	else
		PDraw::image_cutclip(kuva_peli,x+val,y,535,124,535+31,124+31);

	if ((hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31) || (menu_valittu_id == menu_valinta_id)){
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1))
			&& key_delay == 0){
			Play_MenuSFX(menu_aani, 100);
			key_delay = 20;
			return 1;
		}
	}

	x += val;

	if ((hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31) || (menu_valittu_id == menu_valinta_id+1)){
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1))
			&& key_delay == 0){
			Play_MenuSFX(menu_aani, 100);
			key_delay = 20;
			return 2;
		}
	}

	menu_valinta_id += 2;

	return 0;
}

int PK_Draw_Menu_Main(){
	int my = PisteUtils_Is_Mobile()? 260 : 240;//250;

	PK_Draw_Menu_Square(160, 200, 640-180, 410, 224);

	if (episode_started){
		if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_continue),180,my)){
			if ((!peli_ohi && !jakso_lapaisty) || lopetusajastin > 1)
				next_screen = SCREEN_GAME;
			else
				next_screen = SCREEN_MAP;

		}
		my += 20;
	}

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_new_game),180,my)){
		nimiedit = true;
		menu_name_index = strlen(pelaajan_nimi);//   0;
		menu_name_last_mark = ' ';
		menu_nyt = MENU_NAME;
		key_delay = 30;
	}
	my += 20;

	if (episode_started){
		if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_save_game),180,my)){
			menu_nyt = MENU_TALLENNA;
		}
		my += 20;
	}

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_load_game),180,my)){
		menu_nyt = MENU_LOAD;
	}
	my += 20;

	if (Draw_Menu_Text(true,"load language",180,my)){
		menu_nyt = MENU_LANGUAGE;
	}
	my += 20;

	if (!PisteUtils_Is_Mobile()) {
		if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_controls),180,my)){
			menu_nyt = MENU_CONTROLS;
		}
		my += 20;
	}

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_graphics),180,my)){
		menu_nyt = MENU_GRAPHICS;
	}
	my += 20;

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_sounds),180,my)){
		menu_nyt = MENU_SOUNDS;
	}
	my += 20;

	if (!PisteUtils_Is_Mobile()) {
		if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_exit),180,my))
			PK_Fade_Quit();
		my += 20;
	}
	return 0;
}
int PK_Draw_Menu_Name(){
	int mx; //Text cursor
	char merkki;
	bool hiiri_alueella = false;
	int nameSize = (int)strlen(pelaajan_nimi);

	PK_Draw_Menu_Square(90, 150, 640-90, 480-100, 224);

	if (hiiri_x > 180 && hiiri_x < 180+15*20 && hiiri_y > 255 && hiiri_y < 255+18)
		hiiri_alueella = true; //Mouse is in text

	if (hiiri_alueella && PisteInput_Hiiri_Vasen() && key_delay == 0){
		nimiedit = true;
		menu_name_index = (hiiri_x - 180)/15; //Set text cursor with the mouse
		key_delay = 10;
	}

	if (nimiedit && key_delay == 0){

		if (PisteInput_Keydown(PI_LEFT)) {
			menu_name_index--;
			key_delay = 8;
		}

		if (PisteInput_Keydown(PI_RIGHT)) {
			menu_name_index++;
			key_delay = 8;
		}
	}

	if (menu_name_index >= 20)
		menu_name_index = 19;

	if (menu_name_index >= nameSize)
		menu_name_index = nameSize;

	if (menu_name_index < 0)
		menu_name_index = 0;


	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.playermenu_type_name),180,224);

	PDraw::screen_fill(180-2,255-2,180+19*15+4,255+18+4,0);
	PDraw::screen_fill(180,255,180+19*15,255+18,50);

	if (nimiedit) { //Draw text cursor
		mx = menu_name_index*15 + 180 + rand()%2; //Text cursor x
		PDraw::screen_fill(mx-2, 254, mx+6+3, 254+20+3, 0);
		PDraw::screen_fill(mx-1, 254, mx+6, 254+20, 96+16);
		PDraw::screen_fill(mx+4, 254, mx+6, 254+20, 96+8);
	}

	WavetextSlow_Draw(pelaajan_nimi,fontti2,180,255);
	PDraw::font_writealpha(fontti3,pelaajan_nimi,180,255,15);

	merkki = PisteInput_Lue_Nappaimisto();

	if (PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1) && key_delay == 0 && nimiedit) {
		nimiedit = false;
	}

	if (merkki != '\0' && (merkki != menu_name_last_mark || key_delay == 0) && nimiedit && nameSize < 19) {
		menu_name_last_mark = merkki; // Don't write the same letter many times
		key_delay = 15;

		for(int c = nameSize; c > menu_name_index; c--)
			pelaajan_nimi[c] = pelaajan_nimi[c-1];

		pelaajan_nimi[menu_name_index] = merkki;
		menu_name_index++;
	}

	if (key_delay == 0){
		if (PisteInput_Keydown(PI_DELETE)) {
			for (int c=menu_name_index;c<19;c++)
				pelaajan_nimi[c] = pelaajan_nimi[c+1];
			pelaajan_nimi[19] = '\0';
			key_delay = 10;
		}

		if (PisteInput_Keydown(PI_BACK) && menu_name_index != 0) {
			for (int c=menu_name_index-1;c<19;c++)
				pelaajan_nimi[c] = pelaajan_nimi[c+1];
			pelaajan_nimi[19] = '\0';
			if(pelaajan_nimi[menu_name_index] == '\0') pelaajan_nimi[menu_name_index-1] = '\0';
			menu_name_index--;
			key_delay = 10;
		}

		if (PisteInput_Keydown(PI_RETURN) && pelaajan_nimi[0] != '\0') {
			key_delay = 10;
			merkki = '\0';
			nimiedit = false;
			menu_valittu_id = 1;
		}
	}


	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.playermenu_continue),180,300)) {
		menu_nyt = MENU_EPISODES;
		menu_name_index = 0;
		nimiedit = false;
		menu_valittu_id = menu_valinta_id = 1;

		if (episodi_lkm == 1) {
			strcpy(episodi,episodit[2]);
			next_screen = SCREEN_MAP;
			episode_started = false;
			PK_New_Game();
		}
	}

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.playermenu_clear),340,300)) {
		memset(pelaajan_nimi,'\0',sizeof(pelaajan_nimi));
		menu_name_index = 0;
	}

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_exit),180,400)) {
		menu_nyt = MENU_MAIN;
		menu_name_index = 0;
		nimiedit = false;
	}

	return 0;
}
int PK_Draw_Menu_Load(){
	int my = 0, vali = 0;
	char tpaikka[100];
	char jaksoc[8];
	char ind[4];

	PK_Draw_Menu_Square(40, 70, 640-40, 410, 70);

	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.loadgame_title),50,90);
	PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.loadgame_info),50,110);
	my = -20;

	for ( int i = 0; i < MAX_SAVES; i++ ) {
		itoa(i+1,ind,10);
		strcpy(tpaikka,ind);
		strcat(tpaikka,". ");

		strcat(tpaikka,tallennukset[i].nimi);

		if (Draw_Menu_Text(true,tpaikka,100,150+my)) {
			Load_Records(i);
			next_screen = SCREEN_MAP;
			lataa_peli = i;
			//episode_started = false;
		}

		if (strcmp(tallennukset[i].episodi," ")!=0) {
			vali = 0;
			vali = PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.loadgame_episode),400,150+my);
			vali += PDraw::font_write(fontti1,tallennukset[i].episodi,400+vali,150+my);
			vali = 0;
			vali += PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.loadgame_level),400+vali,160+my);
			itoa(tallennukset[i].jakso,jaksoc,10);
			vali += PDraw::font_write(fontti1,jaksoc,400+vali,160+my);
		}

		my += 22;
	}

	my += 20;

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),180,400))
		menu_nyt = MENU_MAIN;

	return 0;
}
int PK_Draw_Menu_Save(){
	int my = 0, vali = 0;
	char tpaikka[100];
	char jaksoc[8];
	char ind[4];

	PK_Draw_Menu_Square(40, 70, 640-40, 410, 224);

	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.savegame_title),50,90);
	PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.savegame_info),50,110);
	my = -20;

	for (int i=0;i<MAX_SAVES;i++)
	{
		itoa(i+1,ind,10);
		strcpy(tpaikka,ind);
		strcat(tpaikka,". ");

		strcat(tpaikka,tallennukset[i].nimi);

		if (Draw_Menu_Text(true,tpaikka,100,150+my))
			Save_Records(i);

		if (strcmp(tallennukset[i].episodi," ")!=0)
		{
			vali = 0;
			vali = PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.savegame_episode),400,150+my);
			vali += PDraw::font_write(fontti1,tallennukset[i].episodi,400+vali,150+my);
			vali = 0;
			vali += PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.savegame_level),400+vali,160+my);
			itoa(tallennukset[i].jakso,jaksoc,10);
			vali += PDraw::font_write(fontti1,jaksoc,400+vali,160+my);
		}

		my += 22;
	}

	my += 20;

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),180,400))
		menu_nyt = MENU_MAIN;

	return 0;
}
int PK_Draw_Menu_Graphics(){
	bool wasFullScreen, wasFiltered, wasFit, wasWide;
	int my = 150;
	static bool moreOptions = false;

	PK_Draw_Menu_Square(40, 70, 640-40, 410, 224);

	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.gfx_title),50,90);

	if(moreOptions){
		wasFullScreen = Settings.isFullScreen;
		wasFiltered = Settings.isFiltered;
		wasFit = Settings.isFit;
		wasWide = Settings.isWide;

		if (Settings.isFullScreen){
			if (Draw_Menu_Text(true,"fullscreen mode is on",180,my)){
				Settings.isFullScreen = false;
			}
		} else{
			if (Draw_Menu_Text(true,"fullscreen mode is off",180,my)){
				Settings.isFullScreen = true;
			}
		}
		if (PK_Draw_Menu_BoolBox(100, my, Settings.isFullScreen, true)) {
			Settings.isFullScreen = !Settings.isFullScreen;
		}
		my += 30;

		if (Settings.isFiltered){
			if (Draw_Menu_Text(true,"bilinear filter is on",180,my)){
				Settings.isFiltered = false;
			}
		} else{
			if (Draw_Menu_Text(true,"bilinear filter is off",180,my)){
				Settings.isFiltered = true;
			}
		}
		if (PK_Draw_Menu_BoolBox(100, my, Settings.isFiltered, true)) {
			Settings.isFiltered = !Settings.isFiltered;
		}
		my += 30;

		if (Settings.isFit){
			if (Draw_Menu_Text(true,"screen fit is on",180,my)){
				Settings.isFit = false;
			}
		} else{
			if (Draw_Menu_Text(true,"screen fit is off",180,my)){
				Settings.isFit = true;
			}
		}
		if (PK_Draw_Menu_BoolBox(100, my, Settings.isFit, true)) {
			Settings.isFit = !Settings.isFit;
		}
		my += 30;

		bool res_active = true;

		if (Settings.isWide) {
			if (Draw_Menu_Text(res_active,"screen size 800x480", 180, my)) {
				Settings.isWide = false;
			}
		}
		else {
			if (Draw_Menu_Text(res_active,"screen size 640x480", 180, my)) {
				Settings.isWide = true;
			}
		}
		if (PK_Draw_Menu_BoolBox(100, my, Settings.isWide, res_active)) {
			Settings.isWide = !Settings.isWide;
		}
		my += 30;

		//Can add more options here

		if(wasFullScreen != Settings.isFullScreen) // If fullscreen changes
			PDraw::set_fullscreen(Settings.isFullScreen);

		if(wasFiltered && !Settings.isFiltered) // If filter changes
			PDraw::set_filter(PDraw::FILTER_NEAREST);
		if(!wasFiltered && Settings.isFiltered)
			PDraw::set_filter(PDraw::FILTER_BILINEAR);

		if(wasFit != Settings.isFit) // If fit changes
			PDraw::fit_screen(Settings.isFit);

		if (wasWide != Settings.isWide) {
			screen_width = Settings.isWide ? 800 : 640;
			PK2Kartta_Aseta_Ruudun_Mitat(screen_width, screen_height);
			PDraw::change_resolution(screen_width, screen_height);
			
			if(episode_started)
				PDraw::image_fill(kuva_tausta, 0);
			
			if (Settings.isWide) PDraw::set_xoffset(80);
			else PDraw::set_xoffset(0);
		}

		if (Draw_Menu_Text(true,"back",100,360)){
			moreOptions = false;
			menu_valittu_id = 0; //Set menu cursor to 0
		}

	}
	else {

		if (Settings.lapinakyvat_objektit){
			if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_tfx_on),180,my))
				Settings.lapinakyvat_objektit = false;
		} else{
			if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_tfx_off),180,my))
				Settings.lapinakyvat_objektit = true;
		}
		if (PK_Draw_Menu_BoolBox(100, my, Settings.lapinakyvat_objektit, true)) {
			Settings.lapinakyvat_objektit = !Settings.lapinakyvat_objektit;
		}
		my += 30;


		if (Settings.lapinakyvat_menutekstit){
			if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_tmenus_on),180,my))
				Settings.lapinakyvat_menutekstit = false;
		} else{
			if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_tmenus_off),180,my))
				Settings.lapinakyvat_menutekstit = true;
		}
		if (PK_Draw_Menu_BoolBox(100, my, Settings.lapinakyvat_menutekstit, true)) {
			Settings.lapinakyvat_menutekstit = !Settings.lapinakyvat_menutekstit;
		}
		my += 30;


		if (Settings.nayta_tavarat){
			if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_items_on),180,my))
				Settings.nayta_tavarat = false;
		} else{
			if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_items_off),180,my))
				Settings.nayta_tavarat = true;
		}
		if (PK_Draw_Menu_BoolBox(100, my, Settings.nayta_tavarat, true)) {
			Settings.nayta_tavarat = !Settings.nayta_tavarat;
		}
		my += 30;


		if (Settings.saa_efektit){
			if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_weather_on),180,my))
				Settings.saa_efektit = false;
		} else{
			if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_weather_off),180,my))
				Settings.saa_efektit = true;
		}
		if (PK_Draw_Menu_BoolBox(100, my, Settings.saa_efektit, true)) {
			Settings.saa_efektit = !Settings.saa_efektit;
		}
		my += 30;


		if (Settings.tausta_spritet){
			if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_bgsprites_on),180,my))
				Settings.tausta_spritet = false;
		} else{
			if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_bgsprites_off),180,my))
				Settings.tausta_spritet = true;
		}
		if (PK_Draw_Menu_BoolBox(100, my, Settings.tausta_spritet, true)) {
			Settings.tausta_spritet = !Settings.tausta_spritet;
		}
		my += 30;


		if (doublespeed){
			if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_speed_double),180,my))
				doublespeed = false;
		} else{
			if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_speed_normal),180,my))
				doublespeed = true;
		}
		if (PK_Draw_Menu_BoolBox(100, my, doublespeed, true)) {
			doublespeed = !doublespeed;
		}
		my += 30;

		if (!PisteUtils_Is_Mobile()) {
			if (Draw_Menu_Text(true,"more",100,360)){
				moreOptions = true;
				menu_valittu_id = 0; //Set menu cursor to 0
			}
		}

	}

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),180,400)){
		menu_nyt = MENU_MAIN;
		moreOptions = false;
	}

	return 0;
}
int PK_Draw_Menu_Sounds(){
	int my = 0;

	PK_Draw_Menu_Square(40, 70, 640-40, 410, 224);

	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.sound_title),50,90);
	my += 20;

	PDraw::screen_fill(404,224+my,404+Settings.sfx_max_volume,244+my,0);
	PDraw::screen_fill(400,220+my,400+Settings.sfx_max_volume,240+my,81);

	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.sound_sfx_volume),180,200+my);
	my += 20;

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.sound_less),180,200+my))
		if (Settings.sfx_max_volume > 0)
			Settings.sfx_max_volume -= 5;

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.sound_more),180+8*15,200+my))
		if (Settings.sfx_max_volume < 100)
			Settings.sfx_max_volume += 5;

	if (Settings.sfx_max_volume < 0)
		Settings.sfx_max_volume = 0;

	if (Settings.sfx_max_volume > 100)
		Settings.sfx_max_volume = 100;

	my+=40;

	PDraw::screen_fill(404,224+my,404+int(Settings.music_max_volume*1.56),244+my,0);
	PDraw::screen_fill(400,220+my,400+int(Settings.music_max_volume*1.56),240+my,112);

	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.sound_music_volume),180,200+my);
	my += 20;

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.sound_less),180,200+my))
		if (Settings.music_max_volume > 0)
			Settings.music_max_volume -= 4;

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.sound_more),180+8*15,200+my))
		if (Settings.music_max_volume < 64)
			Settings.music_max_volume += 4;

	if (Settings.music_max_volume < 0)
		Settings.music_max_volume = 0;

	if (Settings.music_max_volume > 64)
		Settings.music_max_volume = 64;

	music_volume = Settings.music_max_volume;

	my += 20;

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),180,400))
		menu_nyt = MENU_MAIN;

	return 0;
}
int PK_Draw_Menu_Controls(){
	int my = 0;

	PK_Draw_Menu_Square(40, 70, 640-40, 410, 224);

	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.controls_title),50,90);

	my = 40;

	if (menu_lue_kontrollit > 0){
		PDraw::screen_fill(299,74+my+menu_lue_kontrollit*20,584,94+my+menu_lue_kontrollit*20,0);
		PDraw::screen_fill(295,70+my+menu_lue_kontrollit*20,580,90+my+menu_lue_kontrollit*20,50);
	}

	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.controls_moveleft),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.controls_moveright),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.controls_jump),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.controls_duck),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.controls_walkslow),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.controls_eggattack),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.controls_doodleattack),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.controls_useitem),100,90+my);my+=20;

	my = 40;
	PDraw::font_write(fontti2,PisteInput_KeyName(Settings.control_left),310,90+my);my+=20;
	PDraw::font_write(fontti2,PisteInput_KeyName(Settings.control_right),310,90+my);my+=20;
	PDraw::font_write(fontti2,PisteInput_KeyName(Settings.control_jump),310,90+my);my+=20;
	PDraw::font_write(fontti2,PisteInput_KeyName(Settings.control_down),310,90+my);my+=20;
	PDraw::font_write(fontti2,PisteInput_KeyName(Settings.control_walk_slow),310,90+my);my+=20;
	PDraw::font_write(fontti2,PisteInput_KeyName(Settings.control_attack1),310,90+my);my+=20;
	PDraw::font_write(fontti2,PisteInput_KeyName(Settings.control_attack2),310,90+my);my+=20;
	PDraw::font_write(fontti2,PisteInput_KeyName(Settings.control_open_gift),310,90+my);my+=20;

	/*
	if (hiiri_x > 310 && hiiri_x < 580 && hiiri_y > 130 && hiiri_y < my-20){
		menu_lue_kontrollit = (hiiri_y - 120) / 20;

		if (menu_lue_kontrollit < 0 || menu_lue_kontrollit > 8)
			menu_lue_kontrollit = 0;
		else
			key_delay = 25;


	}*/

	if (menu_lue_kontrollit == 0){
		if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.controls_edit),100,90+my))
			menu_lue_kontrollit = 1;
			menu_valittu_id = 0; //Set menu cursor to 0
	}

	my += 30;

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.controls_kbdef),100,90+my)){
		Settings.control_left      = PI_LEFT;
		Settings.control_right     = PI_RIGHT;
		Settings.control_jump      = PI_UP;
		Settings.control_down      = PI_DOWN;
		Settings.control_walk_slow = PI_RALT;
		Settings.control_attack1   = PI_RCONTROL;
		Settings.control_attack2   = PI_RSHIFT;
		Settings.control_open_gift = PI_SPACE;
		menu_lue_kontrollit = 0;
		menu_valittu_id = 0;
	}

	my += 20;

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.controls_gp4def),100,90+my)){
		Settings.control_left      = PI_OHJAIN1_VASEMMALLE;
		Settings.control_right     = PI_OHJAIN1_OIKEALLE;
		Settings.control_jump      = PI_OHJAIN1_YLOS;
		Settings.control_down      = PI_OHJAIN1_ALAS;
		Settings.control_walk_slow = PI_OHJAIN1_NAPPI2;
		Settings.control_attack1   = PI_OHJAIN1_NAPPI1;
		Settings.control_attack2   = PI_OHJAIN1_NAPPI3;
		Settings.control_open_gift = PI_OHJAIN1_NAPPI4;
		menu_lue_kontrollit = 0;
		menu_valittu_id = 0;
	}

	my += 20;

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.controls_gp6def),100,90+my)){
		Settings.control_left      = PI_OHJAIN1_VASEMMALLE;
		Settings.control_right     = PI_OHJAIN1_OIKEALLE;
		Settings.control_jump      = PI_OHJAIN1_YLOS;//PI_OHJAIN1_NAPPI1;
		Settings.control_down      = PI_OHJAIN1_ALAS;
		Settings.control_walk_slow = PI_OHJAIN1_NAPPI2;
		Settings.control_attack1   = PI_OHJAIN1_NAPPI1;
		Settings.control_attack2   = PI_OHJAIN1_NAPPI4;
		Settings.control_open_gift = PI_OHJAIN1_NAPPI6;
		menu_lue_kontrollit = 0;
		menu_valittu_id = 0;
	}

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),180,400)){
		menu_nyt = MENU_MAIN;
		menu_lue_kontrollit = 0;
		menu_valittu_id = 0;
	}

	BYTE k = 0;

	if (key_delay == 0 && menu_lue_kontrollit > 0){
		k = PisteInput_GetKey();

		if (k != 0){
			switch(menu_lue_kontrollit){
				case 1 : Settings.control_left      = k; break;
				case 2 : Settings.control_right     = k; break;
				case 3 : Settings.control_jump      = k; break;
				case 4 : Settings.control_down      = k; break;
				case 5 : Settings.control_walk_slow = k; break;
				case 6 : Settings.control_attack1   = k; break;
				case 7 : Settings.control_attack2   = k; break;
				case 8 : Settings.control_open_gift = k; break;
				default: Play_MenuSFX(ammuu_aani,100); break;
			}

			key_delay = 20;
			menu_lue_kontrollit++;
		}

		if (menu_lue_kontrollit > 8) {
			menu_lue_kontrollit = 0;
			menu_valittu_id = 0;
		}
	}

	my += 20;

	return 0;
}
int PK_Draw_Menu_Episodes(){
	int my = 0;

	PK_Draw_Menu_Square(110, 130, 640-110, 450, 224);

	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.episodes_choose_episode),50,90);
	my += 80;

	if (episodi_lkm-2 > 10) {
		char luku[20];
		int vali = 90;
		int x = 50,//500,
			y = 50;//300;
		//vali += PDraw::font_write(fontti1,"page:",x,y+40);
		itoa(episodisivu+1,luku,10);
		vali += PDraw::font_write(fontti1,luku,x+vali,y+20);
		vali += PDraw::font_write(fontti1,"/",x+vali,y+20);
		itoa((episodi_lkm/10)+1,luku,10);
		vali += PDraw::font_write(fontti1,luku,x+vali,y+20);

		int nappi = PK_Draw_Menu_BackNext(x,y);

		if (nappi == 1 && episodisivu > 0)
			episodisivu--;

		if (nappi == 2 && (episodisivu*10)+10 < episodi_lkm)
			episodisivu++;
	}

	for (int i=(episodisivu*10)+2;i<(episodisivu*10)+12;i++){
		if (strcmp(episodit[i],"") != 0){
			if (Draw_Menu_Text(true,episodit[i],220,90+my)){
				strcpy(episodi,episodit[i]);
				Load_InfoText();
				next_screen = SCREEN_MAP;
				episode_started = false;
				PK_New_Game();
				//PDraw::fade_in(PD_FADE_NORMAL);
			}
			my += 20;
		}
	}

	/* sivu / kaikki */
	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),180,400)){
		menu_nyt = MENU_MAIN;
		my += 20;
	}
	PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.episodes_get_more),140,440);

	return 0;
}
int PK_Draw_Menu_Language(){
	int my = 0;
	int i;

	PK_Draw_Menu_Square(110, 130, 640-110, 450, 224);

	PDraw::font_write(fontti2,"select a language:",50,100);


	for (i=0;i<10;i++){
		if(Draw_Menu_Text(true,langmenulist[i],150,150+my)){
			//printf("Selected %s\n",langmenulist[i]);
			strcpy(Settings.kieli,langmenulist[i]);
			Load_Language();
		}
		my += 20;
	}
	my+=180;


	int direction;
	if(totallangs>10){
		direction = PK_Draw_Menu_BackNext(400,my-20);
		if(direction == 1){
			if(langlistindex>0){

				for(i=9;i>0;i--)
					strcpy(langmenulist[i],langmenulist[i-1]);
				strcpy(langmenulist[0],langlist[langlistindex-1]);
				langlistindex--;
			}
		}
		if(direction == 2){
			if(langlistindex<totallangs-10){

				for(i=0;i<9;i++)
					strcpy(langmenulist[i],langmenulist[i+1]);
				strcpy(langmenulist[9],langlist[langlistindex+10]);
				langlistindex++;
			}
		}
	}
	my+=20;
	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),130,my)){
		menu_nyt = MENU_MAIN;
	}

	return 0;
}

int PK_Draw_Menu(){
	PDraw::screen_fill(0);
	PDraw::image_clip(kuva_tausta, (episode_started && Settings.isWide)? -80 : 0, 0);

	menu_valinta_id = 1;

	switch (menu_nyt)
	{
	case MENU_MAIN     : PK_Draw_Menu_Main();     break;
	case MENU_EPISODES : PK_Draw_Menu_Episodes(); break;
	case MENU_GRAPHICS : PK_Draw_Menu_Graphics(); break;
	case MENU_SOUNDS   : PK_Draw_Menu_Sounds();   break;
	case MENU_CONTROLS : PK_Draw_Menu_Controls(); break;
	case MENU_NAME     : PK_Draw_Menu_Name();     break;
	case MENU_LOAD     : PK_Draw_Menu_Load();     break;
	case MENU_TALLENNA : PK_Draw_Menu_Save();     break;
	case MENU_LANGUAGE : PK_Draw_Menu_Language(); break;
	default            : PK_Draw_Menu_Main();     break;
	}

	PK_Draw_Cursor(hiiri_x, hiiri_y);

	return 0;
}

int Screen_Menu_Init() {
	GUI_Change(UI_CURSOR);

	PDraw::set_xoffset(Settings.isWide? 80 : 0);

	PK_Search_Episode();

	if (!episode_started)
	{
		PDraw::image_delete(kuva_tausta);
		kuva_tausta = PDraw::image_load("gfx/menu.bmp", true);
		PSound::start_music("music/song09.xm"); //theme.xm
		music_volume = Settings.music_max_volume;
	}
	else
	{
		int w, h;
		PDraw::image_getsize(kuva_tausta, w, h);
		if (w != screen_width)
		{
			PDraw::image_delete(kuva_tausta);
			kuva_tausta = PDraw::image_new(screen_width, screen_height);
		}
		PDraw::image_snapshot(kuva_tausta); //TODO - take snapshot without text and cursor
		PK_MenuShadow_Create(kuva_tausta, 640, 480, Settings.isWide? 110 : 30);
	}

	menu_nyt = MENU_MAIN; //TODO - test

	menunelio.left = 320 - 5;
	menunelio.top = 240 - 5;
	menunelio.right = 320 + 5;
	menunelio.bottom = 240 + 5;

	PDraw::screen_fill(0);
	menu_valittu_id = 1;
}

int Screen_Menu(){

	if (!nimiedit && key_delay == 0 && menu_lue_kontrollit == 0) {
		if (PisteInput_Keydown(PI_UP) || PisteInput_Keydown(PI_LEFT) ||
			PisteInput_Ohjain_X(PI_PELIOHJAIN_1) < 0 || PisteInput_Ohjain_Y(PI_PELIOHJAIN_1) < 0){
			menu_valittu_id--;

			if (menu_valittu_id < 1)
				menu_valittu_id = menu_valinta_id-1;

			key_delay = 15;
		}

		if (PisteInput_Keydown(PI_DOWN) || PisteInput_Keydown(PI_RIGHT) ||
			PisteInput_Ohjain_X(PI_PELIOHJAIN_1) > 0 || PisteInput_Ohjain_Y(PI_PELIOHJAIN_1) > 0){
			menu_valittu_id++;

			if (menu_valittu_id > menu_valinta_id-1)
				menu_valittu_id = 1;

			key_delay = 15;
			//hiiri_y += 3;
		}
	}

	if (nimiedit || menu_lue_kontrollit > 0){
		menu_valittu_id = 0;
	}

	if (menu_nyt != MENU_NAME){
		nimiedit = false;
	}
	int menu_ennen = menu_nyt;

	PK_Draw_Menu();

	if (menu_nyt != menu_ennen)
		menu_valittu_id = 0;

	degree = 1 + degree % 360;

	if (doublespeed)
		degree = 1 + degree % 360;

	if (key_delay > 0)
		key_delay--;

	return 0;
}