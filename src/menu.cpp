#include "menu.h"

#include "PisteDraw.h"

#include "controls.h"


PD_RECT menunelio;

bool nimiedit = false;
char menu_nimi_ed_merkki = '\0';
int menu_nimi_index = 0;

int menu_lue_kontrollit = 0;

int menu_valittu_id = 0;
int menu_valinta_id = 1;

bool PK_DrawMenuOption(char *text, int x, int y){
	int pituus = strlen(text)*15;

	if ((hiiri_x > x && hiiri_x < x+pituus && hiiri_y > y && hiiri_y < y+15) ||
		(menu_valittu_id == menu_valinta_id))
	{
		menu_valittu_id = menu_valinta_id;

		if ((
			(PisteInput_Hiiri_Vasen() && hiiri_x > x && hiiri_x < x+pituus && hiiri_y > y && hiiri_y < y+15)
			|| PisteInput_Keydown(PI_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1))
			&& key_delay == 0)
		{
			PK_Soita_Aani_Menu(menu_aani, 100);
			key_delay = 20;
			menu_valinta_id++;
			return true;
		}

		PK_Piirra_LaineTeksti(text, fontti3, x, y);
	}
	else
		PK_Piirra_LaineTeksti_Hidas(text, fontti2, x, y);

	menu_valinta_id++;

	return false;
}

int Draw_MenuSquare(int vasen, int yla, int oikea, int ala, BYTE pvari){
	if (peli_kesken)
		return 0;

	//pvari = 224;

	if (menunelio.left < vasen)
		menunelio.left++;

	if (menunelio.left > vasen)
		menunelio.left--;

	if (menunelio.right < oikea)
		menunelio.right++;

	if (menunelio.right > oikea)
		menunelio.right--;

	if (menunelio.top < yla)
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

	//PisteDraw2_ScreenFill(vasen,yla,oikea,ala,38);

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

			PisteDraw2_ScreenFill(int(dbl_x),int(dbl_y),int(dbl_x+kerroin_x),int(dbl_y+kerroin_y),pvari+vari);
			dbl_x += kerroin_x;
			tumma = !tumma;
		}
		dbl_y += kerroin_y;
	}

	PisteDraw2_ScreenFill(vasen-1,yla-1,oikea+1,yla+2,0);
	PisteDraw2_ScreenFill(vasen-1,yla-1,vasen+2,ala+1,0);
	PisteDraw2_ScreenFill(vasen-1,ala-2,oikea+1,ala+1,0);
	PisteDraw2_ScreenFill(oikea-2,yla-1,oikea+1,ala+1,0);

	PisteDraw2_ScreenFill(vasen-1+1,yla-1+1,oikea+1+1,yla+2+1,0);
	PisteDraw2_ScreenFill(vasen-1+1,yla-1+1,vasen+2+1,ala+1+1,0);
	PisteDraw2_ScreenFill(vasen-1+1,ala-2+1,oikea+1+1,ala+1+1,0);
	PisteDraw2_ScreenFill(oikea-2+1,yla-1+1,oikea+1+1,ala+1+1,0);

	PisteDraw2_ScreenFill(vasen,yla,oikea,yla+1,153);
	PisteDraw2_ScreenFill(vasen,yla,vasen+1,ala,144);
	PisteDraw2_ScreenFill(vasen,ala-1,oikea,ala,138);
	PisteDraw2_ScreenFill(oikea-1,yla,oikea,ala,138);

	return 0;
}
int Draw_OptionButton(int x, int y, bool muuttuja){
	if (!muuttuja)
		PisteDraw2_Image_CutClip(kuva_peli,x,y,473,124,473+31,124+31);
	else
		PisteDraw2_Image_CutClip(kuva_peli,x,y,504,124,504+31,124+31);

	if ((hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31))/* ||
		(menu_valittu_id == menu_valinta_id))*/
	{
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1))
			&& key_delay == 0)
		{
			PK_Soita_Aani_Menu(menu_aani, 100);
			key_delay = 20;
			return true;
		}
	}

	//menu_valinta_id++;

	return false;
}
int PK_Piirra_Menut_Saatolaatikko(int x, int y){ //TODO - traduct
	int val = 45;

	int randx = rand()%3 - rand()%3;
	int randy = rand()%3 - rand()%3;

	if (menu_valittu_id == menu_valinta_id)
		PisteDraw2_Image_CutClip(kuva_peli,x+randx,y+randy,566,124,566+31,124+31);
	else
		PisteDraw2_Image_CutClip(kuva_peli,x,y,566,124,566+31,124+31);

	if (menu_valittu_id == menu_valinta_id+1)
		PisteDraw2_Image_CutClip(kuva_peli,x+val+randx,y+randy,535,124,535+31,124+31);
	else
		PisteDraw2_Image_CutClip(kuva_peli,x+val,y,535,124,535+31,124+31);

	if ((hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31) || (menu_valittu_id == menu_valinta_id))
	{
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1))
			&& key_delay == 0)
		{
			PK_Soita_Aani_Menu(menu_aani, 100);
			key_delay = 20;
			return 1;
		}
	}

	x += val;

	if ((hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31) || (menu_valittu_id == menu_valinta_id+1))
	{
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1))
			&& key_delay == 0)
		{
			PK_Soita_Aani_Menu(menu_aani, 100);
			key_delay = 20;
			return 2;
		}
	}

	menu_valinta_id += 2;

	return 0;
}

int DrawMenu_MainMenu(){
	int my = 240;//250;

	Draw_MenuSquare(160, 200, 640-180, 410, 224);

	if (peli_kesken)
	{
		if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_continue),180,my))
		{
			if ((!peli_ohi && !jakso_lapaisty) || lopetusajastin > 1)
				pelin_seuraava_tila = TILA_PELI;
			else
				pelin_seuraava_tila = TILA_KARTTA;

		}
		my += 20;
	}

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_new_game),180,my))
	{
		nimiedit = true;
		menu_nimi_index = strlen(pelaajan_nimi);//   0;
		menu_nimi_ed_merkki = ' ';
		menu_nyt = MENU_NIMI;
		key_delay = 30;
	}
	my += 20;

	if (peli_kesken)
	{
		if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_save_game),180,my))
		{
			menu_nyt = MENU_TALLENNA;
		}
		my += 20;
	}

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_load_game),180,my))
	{
		menu_nyt = MENU_LATAA;
	}
	my += 20;

	if (PK_DrawMenuOption("load language",180,my))
	{
		menu_nyt = MENU_LANGUAGE;
	}
	my += 20;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_controls),180,my))
	{
		menu_nyt = MENU_KONTROLLIT;
	}
	my += 20;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_graphics),180,my))
	{
		menu_nyt = MENU_GRAFIIKKA;
	}
	my += 20;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_sounds),180,my))
	{
		menu_nyt = MENU_AANET;
	}
	my += 20;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_exit),180,my))
		PK_Quit();
	my += 20;

	return 0;
}
int DrawMenu_Name(){
	int mx; //Text cursor
	char merkki;
	bool hiiri_alueella = false;
	int nameSize = (int)strlen(pelaajan_nimi);

	Draw_MenuSquare(90, 150, 640-90, 480-100, 224);

	if (hiiri_x > 180 && hiiri_x < 180+15*20 && hiiri_y > 255 && hiiri_y < 255+18)
		hiiri_alueella = true; //Mouse is in text

	if (hiiri_alueella && PisteInput_Hiiri_Vasen() && key_delay == 0){
		nimiedit = true;
		menu_nimi_index = (hiiri_x - 180)/15; //Set text cursor with the mouse
		key_delay = 10;
	}

	if (nimiedit && key_delay == 0){

		if (PisteInput_Keydown(PI_LEFT)) {
			menu_nimi_index--;
			key_delay = 8;
		}

		if (PisteInput_Keydown(PI_RIGHT)) {
			menu_nimi_index++;
			key_delay = 8;
		}
	}

	if (menu_nimi_index >= 20)
		menu_nimi_index = 19;

	if (menu_nimi_index >= nameSize)
		menu_nimi_index = nameSize;

	if (menu_nimi_index < 0)
		menu_nimi_index = 0;


	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_playermenu_type_name),180,224);

	PisteDraw2_ScreenFill(180-2,255-2,180+19*15+4,255+18+4,0);
	PisteDraw2_ScreenFill(180,255,180+19*15,255+18,50);

	if (nimiedit) { //Draw text cursor
		mx = menu_nimi_index*15 + 180 + rand()%2; //Text cursor x
		PisteDraw2_ScreenFill(mx-2, 254, mx+6+3, 254+20+3, 0);
		PisteDraw2_ScreenFill(mx-1, 254, mx+6, 254+20, 96+16);
		PisteDraw2_ScreenFill(mx+4, 254, mx+6, 254+20, 96+8);
	}

	PK_Piirra_LaineTeksti_Hidas(pelaajan_nimi,fontti2,180,255);
	PisteDraw2_Font_WriteAlpha(fontti3,pelaajan_nimi,180,255,15);

	merkki = PisteInput_Lue_Nappaimisto();

	if (PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1) && key_delay == 0 && nimiedit) {
		nimiedit = false;
	}

	if (merkki != '\0' && (merkki != menu_nimi_ed_merkki || key_delay == 0) && nimiedit && nameSize < 19) {
		menu_nimi_ed_merkki = merkki; // Don't write the same letter many times
		key_delay = 15;

		for(int c = nameSize; c > menu_nimi_index; c--)
			pelaajan_nimi[c] = pelaajan_nimi[c-1];

		pelaajan_nimi[menu_nimi_index] = merkki;
		menu_nimi_index++;
	}

	if (key_delay == 0){
		if (PisteInput_Keydown(PI_DELETE)) {
			for (int c=menu_nimi_index;c<19;c++)
				pelaajan_nimi[c] = pelaajan_nimi[c+1];
			pelaajan_nimi[19] = '\0';
			key_delay = 10;
		}

		if (PisteInput_Keydown(PI_BACK) && menu_nimi_index != 0) {
			for (int c=menu_nimi_index-1;c<19;c++)
				pelaajan_nimi[c] = pelaajan_nimi[c+1];
			pelaajan_nimi[19] = '\0';
			if(pelaajan_nimi[menu_nimi_index] == '\0') pelaajan_nimi[menu_nimi_index-1] = '\0';
			menu_nimi_index--;
			key_delay = 10;
		}

		if (PisteInput_Keydown(PI_RETURN) && pelaajan_nimi[0] != '\0') {
			key_delay = 10;
			merkki = '\0';
			nimiedit = false;
			menu_valittu_id = 1;
		}
	}


	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_playermenu_continue),180,300)) {
		menu_nyt = MENU_EPISODIT;
		menu_nimi_index = 0;
		nimiedit = false;
		menu_valittu_id = menu_valinta_id = 1;

		if (episodi_lkm == 1) {
			strcpy(episodi,episodit[2]);
			pelin_seuraava_tila = TILA_KARTTA;
			peli_kesken = false;
			PK_Uusi_Peli();
		}
	}

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_playermenu_clear),340,300)) {
		memset(pelaajan_nimi,'\0',sizeof(pelaajan_nimi));
		menu_nimi_index = 0;
	}

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_exit),180,400)) {
		menu_nyt = MENU_PAAVALIKKO;
		menu_nimi_index = 0;
		nimiedit = false;
	}

	return 0;
}
int DrawMenu_Load(){
	int my = 0, vali = 0;
	char tpaikka[100];
	char jaksoc[8];
	char ind[4];

	Draw_MenuSquare(40, 70, 640-40, 410, 70);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_loadgame_title),50,90);
	PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_loadgame_info),50,110);
	my = -20;

	for (int i=0;i<MAX_TALLENNUKSIA;i++) {
		itoa(i+1,ind,10);
		strcpy(tpaikka,ind);
		strcat(tpaikka,". ");

		strcat(tpaikka,tallennukset[i].nimi);

		if (PK_DrawMenuOption(tpaikka,100,150+my))
			PK_Tallennukset_Lataa(i);

		if (strcmp(tallennukset[i].episodi," ")!=0) {
			vali = 0;
			vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_loadgame_episode),400,150+my);
			vali += PisteDraw2_Font_Write(fontti1,tallennukset[i].episodi,400+vali,150+my);
			vali = 0;
			vali += PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_loadgame_level),400+vali,160+my);
			itoa(tallennukset[i].jakso,jaksoc,10);
			vali += PisteDraw2_Font_Write(fontti1,jaksoc,400+vali,160+my);
		}

		my += 22;
	}

	my += 20;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_return),180,400))
		menu_nyt = MENU_PAAVALIKKO;

	return 0;
}
int DrawMenu_Save(){
	int my = 0, vali = 0;
	char tpaikka[100];
	char jaksoc[8];
	char ind[4];

	Draw_MenuSquare(40, 70, 640-40, 410, 224);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_savegame_title),50,90);
	PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_savegame_info),50,110);
	my = -20;

	for (int i=0;i<MAX_TALLENNUKSIA;i++)
	{
		itoa(i+1,ind,10);
		strcpy(tpaikka,ind);
		strcat(tpaikka,". ");

		strcat(tpaikka,tallennukset[i].nimi);

		if (PK_DrawMenuOption(tpaikka,100,150+my))
			PK_Tallennukset_Tallenna(i);

		if (strcmp(tallennukset[i].episodi," ")!=0)
		{
			vali = 0;
			vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_savegame_episode),400,150+my);
			vali += PisteDraw2_Font_Write(fontti1,tallennukset[i].episodi,400+vali,150+my);
			vali = 0;
			vali += PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_savegame_level),400+vali,160+my);
			itoa(tallennukset[i].jakso,jaksoc,10);
			vali += PisteDraw2_Font_Write(fontti1,jaksoc,400+vali,160+my);
		}

		my += 22;
	}

	my += 20;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_return),180,400))
		menu_nyt = MENU_PAAVALIKKO;

	return 0;
}
int DrawMenu_Graphics(){
	bool wasFullScreen, wasFiltered, wasFit;
	int my = 150;
	static bool moreOptions = false;

	Draw_MenuSquare(40, 70, 640-40, 410, 224);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_gfx_title),50,90);

	if(moreOptions){
		wasFullScreen = settings.isFullScreen;
		wasFiltered = settings.isFiltered;
		wasFit = settings.isFit;

		if (settings.isFullScreen){
			if (PK_DrawMenuOption("fullscreen mode is on",180,my)){
				settings.isFullScreen = false;
			}
		} else{
			if (PK_DrawMenuOption("fullscreen mode is off",180,my)){
				settings.isFullScreen = true;
			}
		}
		if (Draw_OptionButton(100, my, settings.isFullScreen)) {
			settings.isFullScreen = !settings.isFullScreen;
		}
		my += 30;

		if (settings.isFiltered){
			if (PK_DrawMenuOption("bilinear filter is on",180,my)){
				settings.isFiltered = false;
			}
		} else{
			if (PK_DrawMenuOption("bilinear filter is off",180,my)){
				settings.isFiltered = true;
			}
		}
		if (Draw_OptionButton(100, my, settings.isFiltered)) {
			settings.isFiltered = !settings.isFiltered;
		}
		my += 30;

		if (settings.isFit){
			if (PK_DrawMenuOption("screen fit is on",180,my)){
				settings.isFit = false;
			}
		} else{
			if (PK_DrawMenuOption("screen fit is off",180,my)){
				settings.isFit = true;
			}
		}
		if (Draw_OptionButton(100, my, settings.isFit)) {
			settings.isFit = !settings.isFit;
		}
		my += 30;


		//Can add more options

		if(wasFullScreen != settings.isFullScreen)
			PisteDraw2_FullScreen(settings.isFullScreen);

		if(wasFiltered && !settings.isFiltered)
			PisteDraw2_SetFilter(PD_FILTER_NEAREST);
		if(!wasFiltered && settings.isFiltered)
			PisteDraw2_SetFilter(PD_FILTER_BILINEAR);

		if(wasFit!= settings.isFit)
			PisteDraw2_FitScreen(settings.isFit);


		if (PK_DrawMenuOption("back",100,360)){
			moreOptions = false;
			menu_valittu_id = 0; //Set menu cursor to 0
		}

	}
	else {

		if (settings.lapinakyvat_objektit){
			if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_gfx_tfx_on),180,my))
				settings.lapinakyvat_objektit = false;
		} else{
			if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_gfx_tfx_off),180,my))
				settings.lapinakyvat_objektit = true;
		}
		if (Draw_OptionButton(100, my, settings.lapinakyvat_objektit)) {
			settings.lapinakyvat_objektit = !settings.lapinakyvat_objektit;
		}
		my += 30;


		if (settings.lapinakyvat_menutekstit){
			if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_gfx_tmenus_on),180,my))
				settings.lapinakyvat_menutekstit = false;
		} else{
			if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_gfx_tmenus_off),180,my))
				settings.lapinakyvat_menutekstit = true;
		}
		if (Draw_OptionButton(100, my, settings.lapinakyvat_menutekstit)) {
			settings.lapinakyvat_menutekstit = !settings.lapinakyvat_menutekstit;
		}
		my += 30;


		if (settings.nayta_tavarat){
			if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_gfx_items_on),180,my))
				settings.nayta_tavarat = false;
		} else{
			if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_gfx_items_off),180,my))
				settings.nayta_tavarat = true;
		}
		if (Draw_OptionButton(100, my, settings.nayta_tavarat)) {
			settings.nayta_tavarat = !settings.nayta_tavarat;
		}
		my += 30;


		if (settings.saa_efektit){
			if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_gfx_weather_on),180,my))
				settings.saa_efektit = false;
		} else{
			if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_gfx_weather_off),180,my))
				settings.saa_efektit = true;
		}
		if (Draw_OptionButton(100, my, settings.saa_efektit)) {
			settings.saa_efektit = !settings.saa_efektit;
		}
		my += 30;


		if (settings.tausta_spritet){
			if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_gfx_bgsprites_on),180,my))
				settings.tausta_spritet = false;
		} else{
			if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_gfx_bgsprites_off),180,my))
				settings.tausta_spritet = true;
		}
		if (Draw_OptionButton(100, my, settings.tausta_spritet)) {
			settings.tausta_spritet = !settings.tausta_spritet;
		}
		my += 30;


		if (doublespeed){
			if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_gfx_speed_double),180,my))
				doublespeed = false;
		} else{
			if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_gfx_speed_normal),180,my))
				doublespeed = true;
		}
		if (Draw_OptionButton(100, my, doublespeed)) {
			doublespeed = !doublespeed;
		}
		my += 30;


		if (PK_DrawMenuOption("more",100,360)){
			moreOptions = true;
			menu_valittu_id = 0; //Set menu cursor to 0
		}

	}

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_return),180,400)){
		menu_nyt = MENU_PAAVALIKKO;
		moreOptions = false;
	}

	return 0;
}
int DrawMenu_Sounds(){
	int my = 0;

	Draw_MenuSquare(40, 70, 640-40, 410, 224);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_sound_title),50,90);
	my += 20;

	PisteDraw2_ScreenFill(404,224+my,404+aanenvoimakkuus,244+my,0);
	PisteDraw2_ScreenFill(400,220+my,400+aanenvoimakkuus,240+my,81);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_sound_sfx_volume),180,200+my);
	my += 20;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_sound_less),180,200+my))
		if (aanenvoimakkuus > 0)
			aanenvoimakkuus -= 5;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_sound_more),180+8*15,200+my))
		if (aanenvoimakkuus < 100)
			aanenvoimakkuus += 5;

	if (aanenvoimakkuus < 0)
		aanenvoimakkuus = 0;

	if (aanenvoimakkuus > 100)
		aanenvoimakkuus = 100;

	my+=40;

	PisteDraw2_ScreenFill(404,224+my,404+int(musiikin_max_voimakkuus*1.56),244+my,0);
	PisteDraw2_ScreenFill(400,220+my,400+int(musiikin_max_voimakkuus*1.56),240+my,112);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_sound_music_volume),180,200+my);
	my += 20;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_sound_less),180,200+my))
		if (musiikin_max_voimakkuus > 0)
			musiikin_max_voimakkuus -= 4;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_sound_more),180+8*15,200+my))
		if (musiikin_max_voimakkuus < 64)
			musiikin_max_voimakkuus += 4;

	if (musiikin_max_voimakkuus < 0)
		musiikin_max_voimakkuus = 0;

	if (musiikin_max_voimakkuus > 64)
		musiikin_max_voimakkuus = 64;

	musiikin_voimakkuus = musiikin_max_voimakkuus;

	my += 20;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_return),180,400))
		menu_nyt = MENU_PAAVALIKKO;

	return 0;
}
int DrawMenu_Controls(){
	int my = 0;

	Draw_MenuSquare(40, 70, 640-40, 410, 224);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_title),50,90);

	my = 40;

	if (menu_lue_kontrollit > 0){
		PisteDraw2_ScreenFill(299,74+my+menu_lue_kontrollit*20,584,94+my+menu_lue_kontrollit*20,0);
		PisteDraw2_ScreenFill(295,70+my+menu_lue_kontrollit*20,580,90+my+menu_lue_kontrollit*20,50);
	}

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_moveleft),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_moveright),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_jump),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_duck),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_walkslow),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_eggattack),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_doodleattack),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_useitem),100,90+my);my+=20;

	my = 40;
	PisteDraw2_Font_Write(fontti2,PisteInput_KeyName(kontrolli_vasemmalle),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_KeyName(kontrolli_oikealle),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_KeyName(kontrolli_hyppy),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_KeyName(kontrolli_alas),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_KeyName(kontrolli_juoksu),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_KeyName(kontrolli_hyokkays1),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_KeyName(kontrolli_hyokkays2),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_KeyName(kontrolli_kayta_esine),310,90+my);my+=20;

	/*
	if (hiiri_x > 310 && hiiri_x < 580 && hiiri_y > 130 && hiiri_y < my-20){
		menu_lue_kontrollit = (hiiri_y - 120) / 20;

		if (menu_lue_kontrollit < 0 || menu_lue_kontrollit > 8)
			menu_lue_kontrollit = 0;
		else
			key_delay = 25;


	}*/

	if (menu_lue_kontrollit == 0){
		if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_controls_edit),100,90+my))
			menu_lue_kontrollit = 1;
			menu_valittu_id = 0; //Set menu cursor to 0
	}

	my += 30;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_controls_kbdef),100,90+my)){
		kontrolli_vasemmalle	= PI_LEFT;
		kontrolli_oikealle		= PI_RIGHT;
		kontrolli_hyppy			= PI_UP;
		kontrolli_alas			= PI_DOWN;
		kontrolli_juoksu		= PI_RALT;
		kontrolli_hyokkays1		= PI_RCONTROL;
		kontrolli_hyokkays2		= PI_RSHIFT;
		kontrolli_kayta_esine	= PI_SPACE;
		menu_lue_kontrollit = 0;
		menu_valittu_id = 0;
	}

	my += 20;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_controls_gp4def),100,90+my)){
		kontrolli_vasemmalle	= PI_OHJAIN1_VASEMMALLE;
		kontrolli_oikealle		= PI_OHJAIN1_OIKEALLE;
		kontrolli_hyppy			= PI_OHJAIN1_YLOS;
		kontrolli_alas			= PI_OHJAIN1_ALAS;
		kontrolli_juoksu		= PI_OHJAIN1_NAPPI2;
		kontrolli_hyokkays1		= PI_OHJAIN1_NAPPI1;
		kontrolli_hyokkays2		= PI_OHJAIN1_NAPPI3;
		kontrolli_kayta_esine	= PI_OHJAIN1_NAPPI4;
		menu_lue_kontrollit = 0;
		menu_valittu_id = 0;
	}

	my += 20;

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_controls_gp6def),100,90+my)){
		kontrolli_vasemmalle	= PI_OHJAIN1_VASEMMALLE;
		kontrolli_oikealle		= PI_OHJAIN1_OIKEALLE;
		kontrolli_hyppy			= PI_OHJAIN1_YLOS;//PI_OHJAIN1_NAPPI1;
		kontrolli_alas			= PI_OHJAIN1_ALAS;
		kontrolli_juoksu		= PI_OHJAIN1_NAPPI2;
		kontrolli_hyokkays1		= PI_OHJAIN1_NAPPI1;
		kontrolli_hyokkays2		= PI_OHJAIN1_NAPPI4;
		kontrolli_kayta_esine	= PI_OHJAIN1_NAPPI6;
		menu_lue_kontrollit = 0;
		menu_valittu_id = 0;
	}

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_return),180,400)){
		menu_nyt = MENU_PAAVALIKKO;
		menu_lue_kontrollit = 0;
		menu_valittu_id = 0;
	}

	BYTE k = 0;

	if (key_delay == 0 && menu_lue_kontrollit > 0){
		k = PisteInput_GetKey();

		if (k != 0){
			switch(menu_lue_kontrollit){
				case 1 : kontrolli_vasemmalle  = k; break;
				case 2 : kontrolli_oikealle    = k; break;
				case 3 : kontrolli_hyppy       = k; break;
				case 4 : kontrolli_alas        = k; break;
				case 5 : kontrolli_juoksu      = k; break;
				case 6 : kontrolli_hyokkays1   = k; break;
				case 7 : kontrolli_hyokkays2   = k; break;
				case 8 : kontrolli_kayta_esine = k; break;
				default: PK_Soita_Aani_Menu(ammuu_aani,100); break;
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
int DrawMenu_Episodes(){
	int my = 0;

	Draw_MenuSquare(110, 130, 640-110, 450, 224);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_episodes_choose_episode),50,90);
	my += 80;

	if (episodi_lkm-2 > 10) {
		char luku[20];
		int vali = 90;
		int x = 50,//500,
			y = 50;//300;
		//vali += PisteDraw2_Font_Write(fontti1,"page:",x,y+40);
		itoa(episodisivu+1,luku,10);
		vali += PisteDraw2_Font_Write(fontti1,luku,x+vali,y+20);
		vali += PisteDraw2_Font_Write(fontti1,"/",x+vali,y+20);
		itoa((episodi_lkm/10)+1,luku,10);
		vali += PisteDraw2_Font_Write(fontti1,luku,x+vali,y+20);

		int nappi = PK_Piirra_Menut_Saatolaatikko(x,y);

		if (nappi == 1 && episodisivu > 0)
			episodisivu--;

		if (nappi == 2 && (episodisivu*10)+10 < episodi_lkm)
			episodisivu++;
	}

	for (int i=(episodisivu*10)+2;i<(episodisivu*10)+12;i++){
		if (strcmp(episodit[i],"") != 0){
			if (PK_DrawMenuOption(episodit[i],220,90+my)){
				strcpy(episodi,episodit[i]);
				PK_Lataa_Lisainfot();
				pelin_seuraava_tila = TILA_KARTTA;
				peli_kesken = false;
				PK_Uusi_Peli();
				//PisteDraw2_FadeIn(PD_FADE_NORMAL);
			}
			my += 20;
		}
	}

	/* sivu / kaikki */
	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_return),180,400)){
		menu_nyt = MENU_PAAVALIKKO;
		my += 20;
	}
	PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_episodes_get_more),140,440);

	return 0;
}
int DrawMenu_Language(){
	int my = 0;
	int i;
	//char temp[10];

	Draw_MenuSquare(110, 130, 640-110, 450, 224);

	PisteDraw2_Font_Write(fontti2,"select a language:",50,100);


	for (i=0;i<10;i++){
		if(PK_DrawMenuOption(langmenulist[i],150,150+my)){
			//printf("Selected %s\n",langmenulist[i]);
			strcpy(settings.kieli,langmenulist[i]);
			PK_Lataa_Kieli();
		}
		my += 20;
	}
	my+=180;


	char* next = "next";

	if(totallangs>10){
		if (PK_DrawMenuOption("previous",130,my)){
			if(langlistindex>0){

				for(i=9;i>0;i--)
					strcpy(langmenulist[i],langmenulist[i-1]);
				strcpy(langmenulist[0],langlist[langlistindex-1]);
				langlistindex--;
			}
		}
		if (PK_DrawMenuOption(next,530-(strlen(next)*16/*font weight*/),my)){
			if(langlistindex<totallangs-10){

				for(i=0;i<9;i++)
					strcpy(langmenulist[i],langmenulist[i+1]);
				strcpy(langmenulist[9],langlist[langlistindex+10]);
				langlistindex++;
			}
		}
	}
	my+=20;
	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_return),130,my)){
		menu_nyt = MENU_PAAVALIKKO;
	}

	return 0;
}

int Draw_Menu(){
	PisteDraw2_Image_Clip(kuva_tausta, peli_kesken? -80 : 0 ,0);

	menu_valinta_id = 1;

	switch (menu_nyt){
    	case MENU_PAAVALIKKO : DrawMenu_MainMenu(); break;
    	case MENU_EPISODIT   : DrawMenu_Episodes(); break;
    	case MENU_GRAFIIKKA  : DrawMenu_Graphics(); break;
    	case MENU_AANET      : DrawMenu_Sounds();   break;
    	case MENU_KONTROLLIT : DrawMenu_Controls(); break;
    	case MENU_NIMI       : DrawMenu_Name();     break;
    	case MENU_LATAA      : DrawMenu_Load();     break;
    	case MENU_TALLENNA   : DrawMenu_Save();     break;
    	case MENU_LANGUAGE   : DrawMenu_Language(); break;
    	default              : DrawMenu_MainMenu(); break;
	}

	PK_Piirra_Kursori(hiiri_x,hiiri_y);

	return 0;
}

int PK_Main_Menut(){
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

	if (menu_nyt != MENU_NIMI){
		nimiedit = false;
	}
	int menu_ennen = menu_nyt;

	Draw_Menu();

	if (menu_nyt != menu_ennen)
		menu_valittu_id = 0;

	degree = 1 + degree % 360;

	if (doublespeed)
		degree = 1 + degree % 360;

	if (key_delay > 0)
		key_delay--;

	return 0;
}
int PK_Start_Menu(){
    PisteDraw2_SetXOffset(80);
    PK_Episodit_Hae();

    if (!peli_kesken){
        PisteDraw2_Image_Delete(kuva_tausta);
        kuva_tausta = PisteDraw2_Image_Load("gfx/menu.bmp",true);
        PisteSound_StartMusic("music/song09.xm");//theme.xm
        musiikin_voimakkuus = musiikin_max_voimakkuus;
    }
    else{
        int w, h;
        PisteDraw2_Image_GetSize(kuva_tausta, w, h);
        if( w != KARTANPIIRTO_LEVEYS ){
            PisteDraw2_Image_Delete(kuva_tausta);
            kuva_tausta = PisteDraw2_Image_New(KARTANPIIRTO_LEVEYS, KARTANPIIRTO_KORKEUS);
        }
        PisteDraw2_Image_Snapshot(kuva_tausta); //TODO - take snapshot without text and cursor
        PK_Sumenna_Kuva(kuva_tausta, 640, 480, 110);
    }

    //menu_valittu_id = 1; //TODO - needed?

    menunelio.left = 320-5;
    menunelio.top = 240-5;
    menunelio.right = 320+5;
    menunelio.bottom = 240+5;

    PisteDraw2_ScreenFill(0);
}
