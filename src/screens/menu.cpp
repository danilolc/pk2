//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "screens/screens.hpp"

#include "settings.hpp"
#include "gui.hpp"
#include "game/game.hpp"
#include "gfx/text.hpp"
#include "sfx.hpp"
#include "language.hpp"
#include "episode/episodeclass.hpp"
#include "episode/mapstore.hpp"
#include "save.hpp"
#include "system.hpp"
#include "version.hpp"

#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"
#include "engine/PUtils.hpp"
#include "engine/PSound.hpp"

#include <cstring>

enum MENU {

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

} bg_square;

int menu_nyt = MENU_MAIN;
int menu_lue_kontrollit = 0;

uint menu_name_index = 0;
char menu_name_last_mark = '\0';
char menu_name[20] = "";

uint menu_valittu_id = 0;
uint menu_valinta_id = 1;

uint episode_page = 0;

uint langlistindex = 0;

bool editing_name = false;

int Draw_BGSquare(int left, int top, int right, int bottom, u8 pvari){
	
	if (Episode)
		return 0;

	if (bg_square.left < left)
		bg_square.left++;

	if (bg_square.left > left)
		bg_square.left--;

	if (bg_square.right < right)
		bg_square.right++;

	if (bg_square.right > right)
		bg_square.right--;

	if (bg_square.top < top)
		bg_square.top++;

	if (bg_square.top > top)
		bg_square.top--;

	if (bg_square.bottom < bottom)
		bg_square.bottom++;

	if (bg_square.bottom > bottom)
		bg_square.bottom--;

	left = bg_square.left;
	right = bg_square.right;
	top	= bg_square.top;
	bottom = bg_square.bottom;

	left += (int)(sin_table[(degree*2)%359]/2.0);
	right += (int)(cos_table[(degree*2)%359]/2.0);
	top	+= (int)(sin_table[((degree*2)+20)%359]/2.0);
	bottom += (int)(cos_table[((degree*2)+40)%359]/2.0);

	//PDraw::screen_fill(left,top,right,bottom,38);

	double mult_y = (bottom - top) / 19.0;
	double mult_x = (right - left) / 19.0;
	double dbl_y = top;
	double dbl_x = left;
	bool dark = true;
	int vari = 0;

	//Draw squares
	for (int y=0;y<19;y++) {
		dbl_x = left;

		for (int x=0;x<19;x++) {
			
			vari = (x / 4) + (y / 3);
			if (dark) vari /= 2;

			PDraw::screen_fill(int(dbl_x),int(dbl_y),int(dbl_x+mult_x),int(dbl_y+mult_y),pvari+vari);
			
			dbl_x += mult_x;
			dark = !dark;

		}

		dbl_y += mult_y;
	}

	//Draw borders
	PDraw::screen_fill(left-1,top-1,right+1,top+2,0);
	PDraw::screen_fill(left-1,top-1,left+2,bottom+1,0);
	PDraw::screen_fill(left-1,bottom-2,right+1,bottom+1,0);
	PDraw::screen_fill(right-2,top-1,right+1,bottom+1,0);

	PDraw::screen_fill(left-1+1,top-1+1,right+1+1,top+2+1,0);
	PDraw::screen_fill(left-1+1,top-1+1,left+2+1,bottom+1+1,0);
	PDraw::screen_fill(left-1+1,bottom-2+1,right+1+1,bottom+1+1,0);
	PDraw::screen_fill(right-2+1,top-1+1,right+1+1,bottom+1+1,0);

	PDraw::screen_fill(left,top,right,top+1,153);
	PDraw::screen_fill(left,top,left+1,bottom,144);
	PDraw::screen_fill(left,bottom-1,right,bottom,138);
	PDraw::screen_fill(right-1,top,right,bottom,138);

	return 0;

}

bool Draw_Menu_Text(bool active, const char *teksti, int x, int y, char end) {

	if(!active) {
		WavetextSlow_Draw(teksti, fontti2, x, y, end);
		return false;
	}

	const int TEXT_H = 20; 

	int length = strlen(teksti) * 15;

	bool mouse_on = PInput::mouse_x > x && PInput::mouse_x < x + length 
		&& PInput::mouse_y > y && PInput::mouse_y < y + TEXT_H;

	if ( mouse_on || (menu_valittu_id == menu_valinta_id) ) {

		menu_valittu_id = menu_valinta_id;
		Wavetext_Draw(teksti, fontti3, x, y, end);//

		if (( (PInput::MouseLeft() && mouse_on) || PInput::Keydown(PInput::SPACE)
			/*|| PInput::Ohjain_Nappi(PI_PELIOHJAIN_1, PI_OHJAIN_NAPPI_1)*/)
			&& key_delay == 0) {

			Play_MenuSFX(menu_sound, 100);
			key_delay = 20;
			menu_valinta_id++;
			return true;

		}

		//Wavetext_Draw(teksti, fontti3, x, y);

	} else {
	
		WavetextSlow_Draw(teksti, fontti2, x, y, end);
	
	}
	menu_valinta_id++;

	return false;
}

int Draw_BoolBox(int x, int y, bool muuttuja, bool active) {
	
	PDraw::RECT img_src, img_dst = {(u32)x,(u32)y,0,0};

	if(muuttuja) img_src = {504,124,31,31};
	else img_src = {473,124,31,31};

	if(active){
		PDraw::image_cutclip(game_assets,img_src,img_dst);
	} else{
		PDraw::image_cutcliptransparent(game_assets,img_src,img_dst,50, 255);
		return false;
	}

	if (PInput::mouse_x > x && PInput::mouse_x < x+30 && PInput::mouse_y > y && PInput::mouse_y < y+31){
		if ((PInput::MouseLeft() || PInput::Keydown(PInput::SPACE)/* || PInput::Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1)*/)
			&& key_delay == 0){

			Play_MenuSFX(menu_sound, 100);
			key_delay = 20;
			return true;
		}
	}

	return false;
}

int  Draw_BackNext(int x, int y){
	int val = 45;

	int randx = rand()%3 - rand()%3;
	int randy = rand()%3 - rand()%3;

	if (menu_valittu_id == menu_valinta_id)
		PDraw::image_cutclip(game_assets,x+randx,y+randy,566,124,566+31,124+31);
	else
		PDraw::image_cutclip(game_assets,x,y,566,124,566+31,124+31);

	if (menu_valittu_id == menu_valinta_id+1)
		PDraw::image_cutclip(game_assets,x+val+randx,y+randy,535,124,535+31,124+31);
	else
		PDraw::image_cutclip(game_assets,x+val,y,535,124,535+31,124+31);

	if ((PInput::mouse_x > x && PInput::mouse_x < x+30 && PInput::mouse_y > y && PInput::mouse_y < y+31) || (menu_valittu_id == menu_valinta_id)){
		if ((PInput::MouseLeft() || PInput::Keydown(PInput::SPACE)/* || PInput::Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1)*/)
			&& key_delay == 0){
			Play_MenuSFX(menu_sound, 100);
			key_delay = 20;
			return 1;
		}
	}

	x += val;

	if ((PInput::mouse_x > x && PInput::mouse_x < x+30 && PInput::mouse_y > y && PInput::mouse_y < y+31) || (menu_valittu_id == menu_valinta_id+1)){
		if ((PInput::MouseLeft() || PInput::Keydown(PInput::SPACE)/* || PInput::Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1)*/)
			&& key_delay == 0){
			Play_MenuSFX(menu_sound, 100);
			key_delay = 20;
			return 2;
		}
	}

	menu_valinta_id += 2;

	return 0;
}

void Draw_Menu_Main() {
	int my = PUtils::Is_Mobile()? 260 : 240;//250;

	Draw_BGSquare(160, 200, 640-180, 410, 224);

	if (Episode){
		if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_continue),180,my)){
			if (Game)
				next_screen = SCREEN_GAME;
			else
				next_screen = SCREEN_MAP;

		}
		my += 20;
	}

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_new_game),180,my)){
		strcpy(menu_name, tekstit->Get_Text(PK_txt.player_default_name));
		menu_name_index = strlen(menu_name);
		menu_name_last_mark = ' ';
		
		editing_name = true;
		PInput::StartKeyboard();

		menu_nyt = MENU_NAME;
		key_delay = 30;
	}
	my += 20;

	if (Episode){
		if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_save_game),180,my)){
			menu_nyt = MENU_TALLENNA;
		}
		my += 20;
	}

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_load_game),180,my)){
		menu_nyt = MENU_LOAD;
	}
	my += 20;

	if (Draw_Menu_Text(true,"load language",180,my)){
		menu_nyt = MENU_LANGUAGE;
	}
	my += 20;

	if (!PUtils::Is_Mobile()) {
		if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_controls),180,my)){
			menu_nyt = MENU_CONTROLS;
		}
		my += 20;
	}

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_graphics),180,my)){
		menu_nyt = MENU_GRAPHICS;
	}
	my += 20;

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_sounds),180,my)){
		menu_nyt = MENU_SOUNDS;
	}
	my += 20;

	if (!PUtils::Is_Mobile()) {
		if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_exit),180,my))
			Fade_Quit();
		my += 20;
	}

}

void Draw_Menu_Name() {

	bool mouse_on_text = false;
	int nameSize = (int)strlen(menu_name);

	int keyboard_size;
	if(/*editing_name &&*/ PUtils::Is_Mobile())
		keyboard_size = 180;
	else
		keyboard_size = 0;

	if (/*editing_name &&*/ PUtils::Is_Mobile())
		Draw_BGSquare(90, 20, 640-90, 220, 224);
	else
		Draw_BGSquare(90, 160, 640-90, 480-80, 224);
	
	int tx_start = 180;
	int tx_end = tx_start + 15*20;
	int ty_start = 254 - keyboard_size;
	int ty_end = ty_start + 18;

	if (PInput::mouse_x >= tx_start && PInput::mouse_x <= tx_end && PInput::mouse_y >= ty_start && PInput::mouse_y <= ty_end)
		mouse_on_text = true; //Mouse is in text

	if (mouse_on_text && PInput::MouseLeft() && key_delay == 0) {

		PInput::StartKeyboard();
		editing_name = true;
		menu_name_index = (PInput::mouse_x - 180) / 15; //Set text cursor with the mouse
		key_delay = 10;
	
	}

	if (menu_name_index >= 20)
		menu_name_index = 19;

	if (menu_name_index >= nameSize)
		menu_name_index = nameSize;

	if (menu_name_index < 0)
		menu_name_index = 0;

	
	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.playermenu_type_name),tx_start,ty_start - 30);

	PDraw::screen_fill(tx_start-2, ty_start-2, tx_end+4, ty_end+4, 0);
	PDraw::screen_fill(tx_start, ty_start, tx_end, ty_end, mouse_on_text? 54:50);

	if (editing_name) { //Draw text cursor
		int mx = menu_name_index*15 + tx_start + rand() % 2;
		PDraw::screen_fill(mx-2, ty_start, mx+6+3, ty_end+3, 0);
		PDraw::screen_fill(mx-1, ty_start, mx+6, ty_end, 96+16);
		PDraw::screen_fill(mx+4, ty_start, mx+6, ty_end, 96+8);
	}

	WavetextSlow_Draw(menu_name,fontti2,tx_start,ty_start-1);
	PDraw::font_writealpha(fontti3,menu_name,tx_start,ty_start-1,15);

	if (editing_name) {

		char in;
		int key = PInput::ReadKeyboard(&in);

		if (in != '\0') {

			for(int j = sizeof(menu_name) - 1; j > menu_name_index; j--)
				menu_name[j] = menu_name[j-1];
			
			menu_name[menu_name_index] = in;
			menu_name_index++;
			menu_name[19] = '\0';

		}

		/*if (PInput::Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1) && key_delay == 0 && editing_name) {
			editing_name = false;
			PInput::EndKeyboard();
		}*/

		if (key == PInput::DEL) {
			for (int c=menu_name_index;c<19;c++)
				menu_name[c] = menu_name[c+1];
			menu_name[19] = '\0';
		}

		if (key == PInput::BACK && menu_name_index != 0) {
			for (int c=menu_name_index-1;c<19;c++)
				menu_name[c] = menu_name[c+1];
			menu_name[19] = '\0';
			if(menu_name[menu_name_index] == '\0') menu_name[menu_name_index-1] = '\0';
			menu_name_index--;
		}

		if (key == PInput::RETURN && menu_name[0] != '\0') {
			editing_name = false;
			PInput::EndKeyboard();

			menu_valittu_id = 1;
		}

		if (key == PInput::LEFT) {
			menu_name_index--;
			key_delay = 8;
		}

		if (key == PInput::RIGHT) {
			menu_name_index++;
			key_delay = 8;
		}

	}


	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.playermenu_continue),tx_start,ty_start + 50)) {

		editing_name = false;
		PInput::EndKeyboard();
		
		menu_nyt = MENU_EPISODES;
		menu_valittu_id = menu_valinta_id = 1;

	}

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.playermenu_clear),tx_start + 180,ty_start + 50)) {
		memset(menu_name,'\0',sizeof(menu_name));
		menu_name_index = 0;
	}

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_exit),tx_start,ty_start + 100)) {
		menu_nyt = MENU_MAIN;
		menu_name_index = 0;
		editing_name = false;
		PInput::EndKeyboard();
	}

}

void Draw_Menu_Load() {

	int my = 0, vali = 0;
	char number[32];
	char jaksoc[8];
	char ind[8];

	Draw_BGSquare(40, 70, 640-40, 410, 70);

	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.loadgame_title),50,90);
	PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.loadgame_info),50,110);
	my = -20;

	for ( int i = 0; i < SAVES_COUNT; i++ ) {
		
		if (i == 10) {
			
			if(saves_list[i].empty)
				break;
			strcpy(number, "bk. ");
			my += 13;

		} else {

			sprintf(ind, "%i", i+1);
			strcpy(number,ind);
			strcat(number,". ");

		}
		
		strcat(number,saves_list[i].name);

		if (Draw_Menu_Text(true,number,100,150+my)) {
			if (!saves_list[i].empty) {
				if (Game) {
					delete Game;
					Game = nullptr;
				}
				if (Episode) {
					if (i != 10) //Don't save then load
						Save_Records(10);
					delete Episode;
					Episode = nullptr;
				}

				Episode = new EpisodeClass(i);
				next_screen = SCREEN_MAP;
			}
		}

		if (strcmp(saves_list[i].episode," ") != 0) {
			vali = 0;
			vali = PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.loadgame_episode),400,150+my);
			vali += PDraw::font_write(fontti1,saves_list[i].episode,400+vali,150+my);
			vali = 0;
			vali += PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.loadgame_level),400+vali,160+my);
			sprintf(jaksoc, "%i", saves_list[i].level);
			vali += PDraw::font_write(fontti1,jaksoc,400+vali,160+my);
		}

		my += 22;
	}

	my += 20;

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_return),180,400))
		menu_nyt = MENU_MAIN;

}

void Draw_Menu_Save() {

	int my = 0, vali = 0;
	char number[32];
	char jaksoc[8];
	char ind[8];

	Draw_BGSquare(40, 70, 640-40, 410, 224);

	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.savegame_title),50,90);
	PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.savegame_info),50,110);
	my = -20;

	for (int i = 0; i < SAVES_COUNT - 1; i++) {

		sprintf(ind, "%i", i+1);
		strcpy(number, ind);
		strcat(number, ". ");

		strcat(number, saves_list[i].name);

		if (Draw_Menu_Text(true,number,100,150+my))
			Save_Records(i);

		if (strcmp(saves_list[i].episode," ")!=0) {

			vali = 0;
			vali = PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.savegame_episode),400,150+my);
			vali += PDraw::font_write(fontti1,saves_list[i].episode,400+vali,150+my);
			vali = 0;
			vali += PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.savegame_level),400+vali,160+my);
			sprintf(jaksoc, "%i", saves_list[i].level);
			vali += PDraw::font_write(fontti1,jaksoc,400+vali,160+my);

		}

		my += 22;
	}

	my += 20;

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_return),180,400))
		menu_nyt = MENU_MAIN;

}

void Draw_Menu_Graphics() {

	bool wasFullScreen, wasFiltered, wasFit, wasWide;
	int my = 150;
	static bool moreOptions = false;

	Draw_BGSquare(40, 70, 640-40, 410, 224);

	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.gfx_title),50,90);

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
		if (Draw_BoolBox(100, my, Settings.isFullScreen, true)) {
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
		if (Draw_BoolBox(100, my, Settings.isFiltered, true)) {
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
		if (Draw_BoolBox(100, my, Settings.isFit, true)) {
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
		if (Draw_BoolBox(100, my, Settings.isWide, res_active)) {
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
			
			MapClass_Set_Screen_Size(screen_width, screen_height);
			PDraw::change_resolution(screen_width, screen_height);
			
			if(Episode)
				PDraw::image_fill(bg_screen, 0);
			
			PDraw::set_xoffset(Settings.isWide? 80 : 0);
		}

		if (Draw_Menu_Text(true,"back",100,360)){
			moreOptions = false;
			menu_valittu_id = 0; //Set menu cursor to 0
		}

	}
	else {

		if (Settings.draw_transparent){
			if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.gfx_tfx_on),180,my))
				Settings.draw_transparent = false;
		} else{
			if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.gfx_tfx_off),180,my))
				Settings.draw_transparent = true;
		}
		if (Draw_BoolBox(100, my, Settings.draw_transparent, true)) {
			Settings.draw_transparent = !Settings.draw_transparent;
		}
		my += 30;


		if (Settings.transparent_text){
			if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.gfx_tmenus_on),180,my))
				Settings.transparent_text = false;
		} else{
			if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.gfx_tmenus_off),180,my))
				Settings.transparent_text = true;
		}
		if (Draw_BoolBox(100, my, Settings.transparent_text, true)) {
			Settings.transparent_text = !Settings.transparent_text;
		}
		my += 30;


		if (Settings.draw_itembar){
			if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.gfx_items_on),180,my))
				Settings.draw_itembar = false;
		} else{
			if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.gfx_items_off),180,my))
				Settings.draw_itembar = true;
		}
		if (Draw_BoolBox(100, my, Settings.draw_itembar, true)) {
			Settings.draw_itembar = !Settings.draw_itembar;
		}
		my += 30;


		if (Settings.draw_weather){
			if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.gfx_weather_on),180,my))
				Settings.draw_weather = false;
		} else{
			if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.gfx_weather_off),180,my))
				Settings.draw_weather = true;
		}
		if (Draw_BoolBox(100, my, Settings.draw_weather, true)) {
			Settings.draw_weather = !Settings.draw_weather;
		}
		my += 30;


		if (Settings.bg_sprites){
			if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.gfx_bgsprites_on),180,my))
				Settings.bg_sprites = false;
		} else{
			if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.gfx_bgsprites_off),180,my))
				Settings.bg_sprites = true;
		}
		if (Draw_BoolBox(100, my, Settings.bg_sprites, true)) {
			Settings.bg_sprites = !Settings.bg_sprites;
		}
		my += 30;


		if (doublespeed){
			if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.gfx_speed_double),180,my))
				doublespeed = false;
		} else{
			if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.gfx_speed_normal),180,my))
				doublespeed = true;
		}
		if (Draw_BoolBox(100, my, doublespeed, true)) {
			doublespeed = !doublespeed;
		}
		my += 30;

		if (!PUtils::Is_Mobile()) {
			if (Draw_Menu_Text(true,"more",100,360)){
				moreOptions = true;
				menu_valittu_id = 0; //Set menu cursor to 0
			}
		}

	}

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_return),180,400)){
		menu_nyt = MENU_MAIN;
		moreOptions = false;
	}

}

void Draw_Menu_Sounds() {

	int my = 0;

	Draw_BGSquare(40, 70, 640-40, 410, 224);

	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.sound_title),50,90);
	my += 20;

	PDraw::screen_fill(404,224+my,404+Settings.sfx_max_volume,244+my,0);
	PDraw::screen_fill(400,220+my,400+Settings.sfx_max_volume,240+my,81);

	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.sound_sfx_volume),180,200+my);
	my += 20;

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.sound_less),180,200+my))
		if (Settings.sfx_max_volume > 0)
			Settings.sfx_max_volume -= 5;

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.sound_more),180+8*15,200+my))
		if (Settings.sfx_max_volume < 100)
			Settings.sfx_max_volume += 5;

	if (Settings.sfx_max_volume < 0)
		Settings.sfx_max_volume = 0;

	if (Settings.sfx_max_volume > 100)
		Settings.sfx_max_volume = 100;

	my+=40;

	PDraw::screen_fill(404,224+my,404+Settings.music_max_volume,244+my,0);
	PDraw::screen_fill(400,220+my,400+Settings.music_max_volume,240+my,112);

	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.sound_music_volume),180,200+my);
	my += 20;

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.sound_less),180,200+my))
		if (Settings.music_max_volume > 0)
			Settings.music_max_volume -= 5;

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.sound_more),180+8*15,200+my))
		if (Settings.music_max_volume < 100)
			Settings.music_max_volume += 5;

	if (Settings.music_max_volume < 0)
		Settings.music_max_volume = 0;

	if (Settings.music_max_volume > 100)
		Settings.music_max_volume = 100;

	PSound::set_musicvolume(Settings.music_max_volume);

	my += 20;

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_return),180,400))
		menu_nyt = MENU_MAIN;

}

void Draw_Menu_Controls() {
	
	int my = 0;

	Draw_BGSquare(40, 70, 640-40, 410, 224);

	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.controls_title),50,90);

	my = 40;

	if (menu_lue_kontrollit > 0){
		PDraw::screen_fill(299,74+my+menu_lue_kontrollit*20,584,94+my+menu_lue_kontrollit*20,0);
		PDraw::screen_fill(295,70+my+menu_lue_kontrollit*20,580,90+my+menu_lue_kontrollit*20,50);
	}

	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.controls_moveleft),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.controls_moveright),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.controls_jump),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.controls_duck),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.controls_walkslow),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.controls_eggattack),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.controls_doodleattack),100,90+my);my+=20;
	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.controls_useitem),100,90+my);my+=20;

	my = 40;
	PDraw::font_write(fontti2,PInput::KeyName(Settings.control_left),380,90+my);my+=20;
	PDraw::font_write(fontti2,PInput::KeyName(Settings.control_right),380,90+my);my+=20;
	PDraw::font_write(fontti2,PInput::KeyName(Settings.control_jump),380,90+my);my+=20;
	PDraw::font_write(fontti2,PInput::KeyName(Settings.control_down),380,90+my);my+=20;
	PDraw::font_write(fontti2,PInput::KeyName(Settings.control_walk_slow),380,90+my);my+=20;
	PDraw::font_write(fontti2,PInput::KeyName(Settings.control_attack1),380,90+my);my+=20;
	PDraw::font_write(fontti2,PInput::KeyName(Settings.control_attack2),380,90+my);my+=20;
	PDraw::font_write(fontti2,PInput::KeyName(Settings.control_open_gift),380,90+my);my+=20;

	/*
	if (PInput::mouse_x > 310 && PInput::mouse_x < 580 && PInput::mouse_y > 130 && PInput::mouse_y < my-20){
		menu_lue_kontrollit = (PInput::mouse_y - 120) / 20;

		if (menu_lue_kontrollit < 0 || menu_lue_kontrollit > 8)
			menu_lue_kontrollit = 0;
		else
			key_delay = 25;


	}*/

	if (menu_lue_kontrollit == 0){
		if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.controls_edit),100,90+my)) {
			menu_lue_kontrollit = 1;
			menu_valittu_id = 0; //Set menu cursor to 0
		}
	}

	my += 30;

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.controls_kbdef),100,90+my)){
		Settings.control_left      = PInput::LEFT;
		Settings.control_right     = PInput::RIGHT;
		Settings.control_jump      = PInput::UP;
		Settings.control_down      = PInput::DOWN;
		Settings.control_walk_slow = PInput::LALT;
		Settings.control_attack1   = PInput::LCONTROL;
		Settings.control_attack2   = PInput::LSHIFT;
		Settings.control_open_gift = PInput::SPACE;
		menu_lue_kontrollit = 0;
		menu_valittu_id = 0;
	}

	my += 20;

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.controls_gp4def),100,90+my)){
		/*Settings.control_left      = PI_OHJAIN1_VASEMMALLE;
		Settings.control_right     = PI_OHJAIN1_OIKEALLE;
		Settings.control_jump      = PI_OHJAIN1_YLOS;
		Settings.control_down      = PI_OHJAIN1_ALAS;
		Settings.control_walk_slow = PI_OHJAIN1_NAPPI2;
		Settings.control_attack1   = PI_OHJAIN1_NAPPI1;
		Settings.control_attack2   = PI_OHJAIN1_NAPPI3;
		Settings.control_open_gift = PI_OHJAIN1_NAPPI4;*/
		menu_lue_kontrollit = 0;
		menu_valittu_id = 0;
	}

	my += 20;

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.controls_gp6def),100,90+my)){
		/*Settings.control_left      = PI_OHJAIN1_VASEMMALLE;
		Settings.control_right     = PI_OHJAIN1_OIKEALLE;
		Settings.control_jump      = PI_OHJAIN1_YLOS;//PI_OHJAIN1_NAPPI1;
		Settings.control_down      = PI_OHJAIN1_ALAS;
		Settings.control_walk_slow = PI_OHJAIN1_NAPPI2;
		Settings.control_attack1   = PI_OHJAIN1_NAPPI1;
		Settings.control_attack2   = PI_OHJAIN1_NAPPI4;
		Settings.control_open_gift = PI_OHJAIN1_NAPPI6;*/
		menu_lue_kontrollit = 0;
		menu_valittu_id = 0;
	}

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_return),180,400)){
		menu_nyt = MENU_MAIN;
		menu_lue_kontrollit = 0;
		menu_valittu_id = 0;
	}

	u8 k = 0;

	if (key_delay == 0 && menu_lue_kontrollit > 0){
		k = PInput::GetKey();

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
				default: Play_MenuSFX(moo_sound,100); break;
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

}

void Draw_Menu_Episodes() {
	int my = 0;

	Draw_BGSquare(80, 130, 640-80, 450, 224);

	PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.episodes_choose_episode),50,90);
	my += 80;

	int size = episodes.size();
	if (size > 10) {
		
		char luku[36];
		int vali = 90;
		int x = 50;//500,
		int y = 50;//300;
		
		//vali += PDraw::font_write(fontti1,"page:",x,y+40);
		sprintf(luku, "%i", episode_page + 1);
		vali += PDraw::font_write(fontti1,luku,x+vali,y+20);
		vali += PDraw::font_write(fontti1,"/",x+vali,y+20);
		sprintf(luku, "%i", int(size/10) + 1);
		vali += PDraw::font_write(fontti1,luku,x+vali,y+20);

		int nappi = Draw_BackNext(x,y);

		if (nappi == 1 && episode_page > 0)
			episode_page--;

		if (nappi == 2 && (episode_page*10)+10 < size)
			episode_page++;
	}

	for (int i = episode_page*10; i < episode_page*10 + 10; i++) {
		if (i >= size)
			break;
		
		if (Draw_Menu_Text(true, episodes[i].name.c_str(), 110, 90+my)) {
			
			if (Game) {
				delete Game;
				Game = nullptr;
			}
			if (Episode) {
				Save_Records(10);
				delete Episode;
				Episode = nullptr;
			}

			Episode = new EpisodeClass(menu_name, episodes[i]);
			next_screen = SCREEN_MAP;
		}

		if (episodes[i].is_zip)
			PDraw::font_write(fontti1, episodes[i].zipfile.c_str(), 450, 95+my);
		else
			PDraw::font_write(fontti1, "original game", 450, 95+my);
		
		my += 20;
	}

	/* sivu / kaikki */
	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_return),180,400)){
		menu_nyt = MENU_MAIN;
		my += 20;
	}

	//PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.episodes_get_more),140,440);

}

void Draw_Menu_Language() {

	Draw_BGSquare(110, 130, 640-110, 450, 224);

	PDraw::font_write(fontti2,"select a language:",50,100);

	int my = 150;

	uint end = langlist.size();
	if (end > langlistindex + 10)
		end = langlistindex + 10;

	for ( int i = langlistindex; i < end; i++ ) {

		if(Draw_Menu_Text(true,langlist[i].c_str(),150,my,'.')) {

			Load_Language(langlist[i].c_str());
			
			if (Load_Fonts(tekstit) == -1) {

				Load_Language(Settings.kieli);
				Load_Fonts(tekstit);

			} else {

				strcpy(Settings.kieli, langlist[i].c_str());
				Settings_Save();

			}

		}
		
		my += 20;
	}

	my = 380;
	if(langlist.size() > 10) {
		int direction = Draw_BackNext(400,my-20);

		if(direction == 1)
			if(langlistindex > 0)
				langlistindex--;
		
		if(direction == 2)
			if(langlistindex < langlist.size() - 10)
				langlistindex++;

	}

	my += 20;
	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.mainmenu_return),130,my))
		menu_nyt = MENU_MAIN;

}

int Draw_Menu() {

	PDraw::image_clip(bg_screen, (Episode && Settings.isWide)? -80 : 0, 0);

	menu_valinta_id = 1;

	switch (menu_nyt) {
		case MENU_MAIN     : Draw_Menu_Main();     break;
		case MENU_EPISODES : Draw_Menu_Episodes(); break;
		case MENU_GRAPHICS : Draw_Menu_Graphics(); break;
		case MENU_SOUNDS   : Draw_Menu_Sounds();   break;
		case MENU_CONTROLS : Draw_Menu_Controls(); break;
		case MENU_NAME     : Draw_Menu_Name();     break;
		case MENU_LOAD     : Draw_Menu_Load();     break;
		case MENU_TALLENNA : Draw_Menu_Save();     break;
		case MENU_LANGUAGE : Draw_Menu_Language(); break;
		default            : Draw_Menu_Main();     break;
	}

	if (!Episode)
		PDraw::font_write(fontti1, PK2_VERSION, 0, 470);

	Draw_Cursor(PInput::mouse_x, PInput::mouse_y);

	return 0;

}



int Screen_Menu_Init() {
	
	if(PUtils::Is_Mobile())
		GUI_Change(UI_CURSOR);
	
	PDraw::set_xoffset(Settings.isWide? 80 : 0);
	
	langlistindex = 0;

	if (!Episode) {

		PDraw::image_load(bg_screen, PFile::Path("gfx" PE_SEP "menu.bmp"), true);
		PSound::start_music(PFile::Path("music" PE_SEP "song09.xm"));
		PSound::set_musicvolume(Settings.music_max_volume);
	
	} else {

		//int w, h;
		//PDraw::image_getsize(bg_screen, w, h);
		//if (w != screen_width) {
		//	PDraw::image_delete(bg_screen);
		//	bg_screen = PDraw::image_new(screen_width, screen_height);
		//}
		//PDraw::image_snapshot(bg_screen); //TODO - take snapshot without text and cursor
		PDraw::create_shadow(bg_screen, 640, 480, Settings.isWide? 110 : 30);

	}

	menu_nyt = MENU_MAIN;

	bg_square.left = 320 - 5;
	bg_square.top = 240 - 5;
	bg_square.right = 320 + 5;
	bg_square.bottom = 240 + 5;

	menu_valittu_id = 1;

	return 0;

}

int Screen_Menu() {
	
	if (!editing_name && key_delay == 0 && menu_lue_kontrollit == 0) {
		if (PInput::Keydown(PInput::UP) || PInput::Keydown(PInput::LEFT) /*||
			PInput::Ohjain_X(PI_PELIOHJAIN_1) < 0 || PInput::Ohjain_Y(PI_PELIOHJAIN_1) < 0*/){
			menu_valittu_id--;

			if (menu_valittu_id < 1)
				menu_valittu_id = menu_valinta_id-1;

			key_delay = 15;
		}

		if (PInput::Keydown(PInput::DOWN) || PInput::Keydown(PInput::RIGHT) /*||
			PInput::Ohjain_X(PI_PELIOHJAIN_1) > 0 || PInput::Ohjain_Y(PI_PELIOHJAIN_1) > 0*/){
			menu_valittu_id++;

			if (menu_valittu_id > menu_valinta_id-1)
				menu_valittu_id = 1;

			key_delay = 15;
			//PInput::mouse_y += 3;
		}

		static bool wasPressed = false;

		if (!wasPressed && PInput::Keydown(PInput::ESCAPE) && menu_nyt == MENU_MAIN) {
			if(menu_valittu_id == menu_valinta_id-1)
				Fade_Quit();
			else
				menu_valittu_id = menu_valinta_id-1;
		}

		wasPressed = PInput::Keydown(PInput::ESCAPE);

	}

	if (editing_name || menu_lue_kontrollit > 0) {
		menu_valittu_id = 0;
	}

	if (menu_nyt != MENU_NAME){
		editing_name = false;
		PInput::EndKeyboard();
	}
	int menu_ennen = menu_nyt;

	Draw_Menu();

	if (menu_nyt != menu_ennen)
		menu_valittu_id = 0;

	degree = 1 + degree % 360;

	if (doublespeed)
		degree = 1 + degree % 360;

	return 0;
	
}