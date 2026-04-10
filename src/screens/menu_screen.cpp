//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include <iostream>
#include <sstream>
#include "menu_screen.hpp"
#include "utils/open_browser.hpp"

#include "settings/settings.hpp"
#include "settings/config_txt.hpp"

#include "gfx/touchscreen.hpp"
#include "game/game.hpp"
#include "gfx/text.hpp"
#include "sfx.hpp"
#include "language.hpp"
#include "episode/episodeclass.hpp"
#include "episode/mapstore.hpp"
#include "episode/save_slots.hpp"
#include "system.hpp"
#include "version.hpp"

#include "engine/Piste.hpp"
#include "engine/PFilesystem.hpp"
#include "engine/PString.hpp"

#include <cstring>
#include <stdexcept>
#include <SDL_system.h>


#include "keyboard_navigation.hpp"

void MenuScreen::Draw_BGSquare(int left, int top, int right, int bottom, u8 pvari){
	
	if (Episode)
		return;

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

	left   += (int)(sin_table( degree*2)     / 2.0);
	right  += (int)(cos_table( degree*2)     / 2.0);
	top	   += (int)(sin_table((degree*2)+20) / 2.0);
	bottom += (int)(cos_table((degree*2)+40) / 2.0);

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
}


bool MenuScreen::drawMenuTextS(const std::string& text, int x, int y){
	const int TEXT_H = 20; 

	int length = text.size() * 15;

	const Point2D& mousePos = PInput::InputSystem::instance().getMousePos();

	bool mouse_on = mousePos.x > x && mousePos.x < x + length 
		&& mousePos.y > y && mousePos.y < y + TEXT_H
		&& !mouse_hidden;

	if ( mouse_on || (chosen_menu_id == menus_count) ) {

		chosen_menu_id = menus_count;
		Wavetext_Draw(text.c_str(), fontti3, x, y);//

		if ( (this->mousePressed && mouse_on) || this->enterPressed ) {

			Play_MenuSFX(sfx_global.menu_sound, 100);
			menus_count++;
			
			return true;

		}
	} else {
	
		WavetextSlow_Draw(text.c_str(), fontti2, x, y);
	
	}

	menus_count++;
	return false;

}

bool MenuScreen::drawMenuTextS2(const std::string& text, int x, int y){
	const int TEXT_H = 20; 

	int length = text.size() * 15;

	const Point2D& mousePos = PInput::InputSystem::instance().getMousePos();

	bool mouse_on = mousePos.x > x && mousePos.x < x + length 
		&& mousePos.y > y && mousePos.y < y + TEXT_H
		&& !mouse_hidden;

	if ( mouse_on || (chosen_menu_id == menus_count) ) {

		chosen_menu_id = menus_count;
		Wavetext_Draw(text.c_str(), fontti3, x, y);//

		if ( (this->mousePressed && mouse_on) || this->enterPressed ) {

			this->menuKeyDelay = 9;
			Play_MenuSFX(sfx_global.menu_sound, 100);
			menus_count++;
			return true;
		} else if (this->menuKeyDelay==0) {

			if( (mouse_on && PInput::Key::MOUSE_LEFT.isPressed()) ||
			PInput::Key::RETURN.isPressed() ||
			PInput::Key::SPACE.isPressed() ||
			PInput::Key::JOY_A.isPressed() ||
			PInput::Key::JOY_START.isPressed() ){ 
				this->menuKeyDelay = 9;
				Play_MenuSFX(sfx_global.menu_sound, 100);
				menus_count++;
				return true;
			}
		}
	} else {
	
		WavetextSlow_Draw(text.c_str(), fontti2, x, y);
	
	}

	menus_count++;
	return false;

}


void MenuScreen::drawMenuTextControls(const std::string& text, unsigned int key, int x, int y){
	if(this->selected_control_key==0){
		if(this->drawMenuTextS(text, x, y)){
			this->selected_control_key = key;
		}
	}
	else if(this->selected_control_key==key){
		PDraw::font_write_line(fontti3, text, x, y);
	}
	else{
		PDraw::font_write_line(fontti2, text, x, y);
	}
}

void MenuScreen::drawBoolBoxGroup(bool& value, bool&changed, const std::string& text_true, const std::string& text_false){
	if (value){
		if (drawMenuTextS(text_true,180,my)) {
			value = false;
			changed = true;
		}
	} else{
		if (drawMenuTextS(text_false,180,my)) {
			value = true;
			changed = true;
		}
	}
	if (PK2gui::Draw_BoolBox(100, my, value, true, mousePressed)) {
		value = !value;
		changed = true;
	}
	my += 40;//30;
}


int MenuScreen::Draw_BackNext(int x, int y) {
	int val = 45;

	int randx = rand()%3 - rand()%3;
	int randy = rand()%3 - rand()%3;


	const Point2D& mousePos = PInput::InputSystem::instance().getMousePos();

	bool mouse_on1 = mousePos.x > x && mousePos.x < x + 31
		&& mousePos.y > y && mousePos.y < y + 31
		&& !mouse_hidden;

	bool mouse_on2 = mousePos.x > x+val && mousePos.x < x+val + 31 
		&& mousePos.y > y && mousePos.y < y + 31
		&& !mouse_hidden;

	if (mouse_on1 || chosen_menu_id == menus_count) {
		PDraw::image_cutclip(global_gfx_texture,x+randx,y+randy,566,124,566+31,124+31);
		chosen_menu_id = menus_count;
	} else
		PDraw::image_cutclip(global_gfx_texture,x,y,566,124,566+31,124+31);

	if (mouse_on2 || chosen_menu_id == menus_count+1) {
		PDraw::image_cutclip(global_gfx_texture,x+val+randx,y+randy,535,124,535+31,124+31);
		chosen_menu_id = menus_count+1;
	} else
		PDraw::image_cutclip(global_gfx_texture,x+val,y,535,124,535+31,124+31);

	int ret = 0;

	if ((this->mousePressed && mouse_on1) || (this->enterPressed && chosen_menu_id == menus_count)) {
	
		Play_MenuSFX(sfx_global.menu_sound, 100);
		ret = 1;
	
	} else if ((this->mousePressed && mouse_on2) || (this->enterPressed && chosen_menu_id == menus_count + 1)) {
	
		Play_MenuSFX(sfx_global.menu_sound, 100);
		ret = 2;
	
	}

	menus_count += 2;

	return ret;
}


int MenuScreen::Draw_Radio(int x, int y, int num, int sel) {

	const PDraw::RECT sel_src = {504,124,31,31};
	const PDraw::RECT uns_src = {473,124,31,31};
	
	int val = 50;

	int randx = rand()%3 - rand()%3;
	int randy = rand()%3 - rand()%3;


	const Point2D& mousePos = PInput::InputSystem::instance().getMousePos();

	bool mouse_on_y = mousePos.y > y && mousePos.y < y + 31 && !mouse_hidden;

	int ret = -1;
	for (int i = 0; i < num; i++) {

		int xn = x + i * (val + 31);
		int yn = y;

		bool mouse_on = mouse_on_y && mousePos.x > xn && mousePos.x < xn + 31;

		if (mouse_on) {
			chosen_menu_id = menus_count + i;
		}

		if (chosen_menu_id == menus_count + i) {
			xn += randx;
			yn += randy;
		}

		if ((this->mousePressed && mouse_on) || (this->enterPressed && chosen_menu_id == menus_count + i)) {

			Play_MenuSFX(sfx_global.menu_sound, 100);
			sel = i;
			ret = i;

		}
		
		PDraw::RECT img_dst = { xn, yn, 0, 0};
		if (sel == i)
			PDraw::image_cutclip(global_gfx_texture,sel_src,img_dst);
		else
			PDraw::image_cutclip(global_gfx_texture,uns_src,img_dst);

	}

	menus_count += num;
	return ret;

}

void MenuScreen::Draw_Menu_Main() {
	int my = Episode==nullptr ? 223 : 250; //200

	Draw_BGSquare(160, 200, 640-180, 380, 224);
	
	if (Episode){
		if (drawMenuText(PK_txt.mainmenu_continue,180,my)){
			if (Game)
				next_screen = SCREEN_GAME;
			else
				next_screen = SCREEN_MAP;

		}
		my += 20;
	}

	if(config_txt.save_slots){
		if (drawMenuText(PK_txt.mainmenu_new_game,180,my)){
			this->playerNameEdit.setText(config_txt.player);
			this->playerNameEdit.startInput();

			menu_nyt = MENU_NAME;
		}
		my += 20;

		if (Episode){
			if (drawMenuText(PK_txt.mainmenu_save_game,180,my)){
				menu_nyt = MENU_SAVE;
			}
			my += 20;
		}

		if (drawMenuText(PK_txt.mainmenu_load_game,180,my)){
			menu_nyt = MENU_LOAD;
		}
		my += 20;	
	}
	else{
		if (drawMenuText(PK_txt.setup_play,180,my)){
			menu_nyt = MENU_EPISODES;
		}
		my += 20;
	}

	if(drawMenuText(PK_txt.setup_options,180,my)){
		menu_nyt = MENU_SETTINGS;
	}
	my += 20;
	
	if(config_txt.links_menu == LINKS_MENU_MAIN){
		if(drawMenuText(PK_txt.mainmenu_links,180,my)){
			menu_nyt = MENU_LINKS;
		}
		my += 20;
	}

	if (Settings.touchscreen_mode && Game) {
		if (drawMenuText(PK_txt.mainmenu_map,180,my)) {
			next_screen = SCREEN_MAP;

			delete Game;
			Game = nullptr;
		}
		my += 20;
	}
	else{

		#ifdef __ANDROID__

		if (drawMenuTextS("Install zip",180,my)){
			Android_InstallZipEpisode();
		}
		my += 20;

		#endif


		if (drawMenuText(PK_txt.mainmenu_exit,180,my)){
			fadeQuit();
		}
		my += 40;
	}
}

void MenuScreen::Draw_Menu_Links(){
	Draw_BGSquare(160, 200, 640-180, 450, 224);
	int my = 223;
	if(Episode){
		my = 200;
	}

	if (drawMenuTextS("Discord",180,my)){
		OpenBrowser(URL_DISCORD);
	}
	my += 20;
	if (drawMenuTextS("GitHub",180,my)){
		OpenBrowser(URL_GITHUB);
	}
	my += 20;
	if (drawMenuTextS("ProBoards",180,my)){
		OpenBrowser(URL_PROBOARDS);
	}
	my += 20;
	if (drawMenuTextS("PisteGamez.net",180,my)){
		OpenBrowser(URL_PISTEGAMEZ);
	}
	my += 20;
	if (drawMenuTextS("PK2 Fanpage",180,my)){
		OpenBrowser(URL_MAKYUNI);
	}

	if (drawMenuText(PK_txt.mainmenu_return,180,400)){
		menu_nyt = MENU_MAIN;
	}
}

void MenuScreen::Draw_Menu_Settings(){
	Draw_BGSquare(160, 200, 640-180, 450, 224);

	int my = 223;

	if(Episode){
		my = 200;
	}

	if (drawMenuTextS("language",180,my)){
		menu_nyt = MENU_LANGUAGE;
	}
	my += 20;

	if (drawMenuText(PK_txt.mainmenu_controls,180,my)){
		menu_nyt = MENU_CONTROLS;
	}
	my += 20;

	if (drawMenuText(PK_txt.mainmenu_graphics,180,my)){
		menu_nyt = MENU_GRAPHICS;
	}
	my += 20;

	if (drawMenuText(PK_txt.mainmenu_sounds,180,my)){
		menu_nyt = MENU_SOUNDS;
	}
	my += 20;

	if (drawMenuText(PK_txt.mainmenu_return,180,400)){
		menu_nyt = MENU_MAIN;
	}
}

void MenuScreen::Draw_Menu_Name() {
	Draw_BGSquare(90, 160, 640-90, 480-80, 224);

	int tx_start = 180;
	int ty_start = 254;
	
	if(this->playerNameEdit.draw(tekstit->Get_Text(PK_txt.playermenu_type_name),
	tx_start, ty_start, mousePressed)){
		menu_nyt = MENU_EPISODES;
	}
	menus_count+=1;

	if (drawMenuText(PK_txt.playermenu_continue,tx_start,ty_start + 50)) {
		this->playerNameEdit.endInput();		
		menu_nyt = MENU_EPISODES;
	}

	if (drawMenuText(PK_txt.playermenu_clear,tx_start + 180,ty_start + 50)) {
		this->playerNameEdit.clear();
	}

	if (drawMenuText(PK_txt.mainmenu_back,tx_start,ty_start + 100)) {
		this->playerNameEdit.endInput();
		menu_nyt = MENU_MAIN;
	}
}

void MenuScreen::Draw_Menu_Load() {

	int my = 0, vali = 0;
	//char number[32];
	//char jaksoc[8];
	//char ind[8];

	Draw_BGSquare(40, 70, 640-40, 410, 67);

	PDraw::font_write_line(fontti2,tekstit->Get_Text(PK_txt.loadgame_title),50,90);
	PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.loadgame_info),50,110);
	my = -20;

	for ( std::size_t i = 0; i < PK2save::saveSlots.size(); i++ ) {

		std::ostringstream os;

		const PK2save::PK2SaveSlot& slot = PK2save::saveSlots[i];
		
		if (i == 10) {
			
			if(slot.empty)
				break;
			os<<"bk. ";
			my += 13;

		} else {
			os<<i+1<<". ";
		}
		
		if (slot.empty){
			os<<tekstit->Get_Text(PK_txt.savegame_empty);
		}
		else{
			os<<slot.player;
		}

		std::string number = os.str();
		

		if (drawMenuTextS(number,100,150+my)) {
			if (!PK2save::saveSlots[i].empty) {
				if (Game) {
					delete Game;
					Game = nullptr;
				}
				if (Episode) {
					delete Episode;
					Episode = nullptr;
				}

				Episode = new EpisodeClass("", slot.episode);
				PK2save::LoadSlot(Episode, i);
				next_screen = SCREEN_MAP;
			}
		}

		if (!slot.empty) {

			vali = 0;
			vali += PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.savegame_episode),400,150+my);
			vali += PDraw::font_write_line(fontti1,slot.episode.name,400+vali,150+my);
			
			vali = 0;
			if (!slot.completed) {
				vali += PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.savegame_level),400+vali,160+my);
				vali += PDraw::font_write_line(fontti1,std::to_string(slot.next_level),400+vali,160+my);
			} else {
				vali += PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.savegame_completed), 400+vali,160+my);
			}
		}

		my += 22;
	}

	my += 20;

	if (drawMenuText(PK_txt.mainmenu_return,180,400))
		menu_nyt = MENU_MAIN;

}

void MenuScreen::Draw_Menu_Save() {

	int my = 0, vali = 0;

	Draw_BGSquare(40, 70, 640-40, 410, 224);

	PDraw::font_write_line(fontti2,tekstit->Get_Text(PK_txt.savegame_title),50,90);
	PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.savegame_info),50,110);
	my = -20;

	for (int i = 0; i < 10; i++) {
		std::ostringstream os;
		const PK2save::PK2SaveSlot& slot = PK2save::saveSlots[i];
		os<<i+1<<". ";

		if (slot.empty){
			os<<tekstit->Get_Text(PK_txt.savegame_empty);
		}
		else{
			os<<slot.player;
		}

		std::string number = os.str();

		if (drawMenuTextS(number,100,150+my) && Episode!=nullptr){
			PK2save::SaveSlot(Episode, i);
		}
			

		if (!slot.empty) {

			vali = 0;
			vali += PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.savegame_episode),400,150+my);
			vali += PDraw::font_write_line(fontti1,slot.episode.name,400+vali,150+my);
			
			vali = 0;
			if (!slot.completed) {
				vali += PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.savegame_level),400+vali,160+my);
				vali += PDraw::font_write_line(fontti1,std::to_string(slot.next_level),400+vali,160+my);
			} else {
				vali += PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.savegame_completed),400+vali,160+my);
			}
		}

		my += 22;
	}

	my += 20;

	if (drawMenuText(PK_txt.mainmenu_return,180,400))
		menu_nyt = MENU_MAIN;

}

void MenuScreen::Draw_Menu_Graphics() {

	//int mx = 0, my = 150, option;

	int mx = 0, option;
	this->my = 150;
	
	static bool moreOptions = false;
	bool save_settings = false;

	Draw_BGSquare(40, 70, 640-40, 410, 224);

	PDraw::font_write_line(fontti2,tekstit->Get_Text(PK_txt.gfx_title),50,90);

	if(moreOptions){
		int  oldfps = Settings.fps;

#ifndef __ANDROID__
		bool wasFullScreen = Settings.isFullScreen;
		if (Settings.isFullScreen){
			if (drawMenuText(PK_txt.gfx_fullscreen_on,180,my)){
				Settings.isFullScreen = false;
			}
		} else{
			if (drawMenuText(PK_txt.gfx_fullscreen_off,180,my)){
				Settings.isFullScreen = true;
			}
		}
		if (PK2gui::Draw_BoolBox(100, my, Settings.isFullScreen, true, mousePressed)) {
			Settings.isFullScreen = !Settings.isFullScreen;
		}
#endif

		my += 40;

		//TODO - Fix touch position when screen fit

		int dx  = PDraw::font_get_text_size(fontti1, tekstit->Get_Text(PK_txt.gfx_texture_filtering)).first;
		int dx2 = PDraw::font_get_text_size(fontti1, tekstit->Get_Text(PK_txt.gfx_game_speed)).first;

		if(dx2 > dx){
			dx = dx2;
		}

		dx += 10;

		PDraw::font_write_line(fontti1, tekstit->Get_Text(PK_txt.gfx_game_speed), 100, my + 17);

		mx = 100 + dx;

		PDraw::font_write_line(fontti1, "v-sync", mx, my);
		mx += 15 + 68;
		PDraw::font_write_line(fontti1, "60 fps", mx, my);
		mx += 15 + 68;
		PDraw::font_write_line(fontti1, "85 fps", mx, my);
		/*mx += 15 + 51;
		PDraw::font_write_line(fontti1, "120fps", mx, my);*/

		my += 10;
		option = Draw_Radio(100 + dx, my, 3, Settings.fps);

		if (option != -1)
			Settings.fps = option;

		my += 31 + 10;

		PDraw::font_write_line(fontti1, tekstit->Get_Text(PK_txt.gfx_texture_filtering), 100, my + 17);

		mx = 100 + dx;
		PDraw::font_write_line(fontti1, "nearest", mx, my);
		mx += 15 + 68;
		PDraw::font_write_line(fontti1, "linear", mx, my);
		mx += 15 + 68;

		/*  Temporarily disabled  */
		// PDraw::font_write_line(fontti1, "crt", mx, my);
		// mx += 15 + 51;
		// PDraw::font_write_line(fontti1, "hqx", mx, my);
		my += 10;

		option = Draw_Radio(100 + dx, my, 2, Settings.shader_type);

		if (option != -1) {
			if (option != Settings.shader_type) {

				int ret = Set_Screen_Mode(option);
				if (ret == 0) {
					Settings.shader_type = option;
					save_settings = true;
				}

			}
		}
		//Add more options here
#ifndef __ANDROID__
		if(wasFullScreen != Settings.isFullScreen) {// If fullscreen changes
			save_settings = true;
			PRender::set_fullscreen(Settings.isFullScreen);
		}
#endif

		if (Settings.fps != oldfps) {
			int ret = -1;
			save_settings = true;
			if (Settings.fps == SETTINGS_VSYNC)
				ret = Piste::set_fps(-1);
			else if (Settings.fps == SETTINGS_60FPS)
				ret = Piste::set_fps(60);
			else if (Settings.fps == SETTINGS_85FPS)
				ret = Piste::set_fps(85);
			else if (Settings.fps == SETTINGS_120FPS)
				ret = Piste::set_fps(120);
			
			if (ret != 0) {
				PLog::Write(PLog::ERR, "PK2", "FPS mode not supported, changing to 60fps");
				Piste::set_fps(60);
				Settings.fps = SETTINGS_60FPS;
				if (oldfps == SETTINGS_60FPS)
					save_settings = false;
			}	
		}

		if (drawMenuText(PK_txt.mainmenu_back,100,360)) {
			moreOptions = false;
			chosen_menu_id = 0; //Set menu cursor to 0
		}

	}
	else {
		this->my = 150;

		this->drawBoolBoxGroup(Settings.transparent_text,
							save_settings,
							tekstit->Get_Text(PK_txt.gfx_tmenus_on),
							tekstit->Get_Text(PK_txt.gfx_tmenus_off));
		
		this->drawBoolBoxGroup(Settings.draw_gui,
							save_settings,
							tekstit->Get_Text(PK_txt.gfx_gui_on),
							tekstit->Get_Text(PK_txt.gfx_gui_off));
		
		this->drawBoolBoxGroup(Settings.double_speed,
							save_settings,
							tekstit->Get_Text(PK_txt.gfx_speed_double),
							tekstit->Get_Text(PK_txt.gfx_speed_normal));

		this->drawBoolBoxGroup(Settings.touchscreen_mode,
			save_settings,
			tekstit->Get_Text(PK_txt.gfx_touchscreen_on),
			tekstit->Get_Text(PK_txt.gfx_touchscreen_off));

		//if (!Settings.touchscreen_mode)
			if (drawMenuText(PK_txt.mainmenu_more,100,360)){
				moreOptions = true;
				chosen_menu_id = 0; //Set menu cursor to 0
			}

	}

	if (drawMenuText(PK_txt.settingsmenu_return,180,400)){
		menu_nyt = MENU_SETTINGS;
		moreOptions = false;
	}

	if (save_settings)
		Settings_Save();

}

void MenuScreen::Draw_Menu_Sounds() {

	Draw_BGSquare(40, 70, 640-40, 410, 224);

	int my = 0;
	u8 sfx_volume = Settings.sfx_max_volume;
	u8 mus_volume = Settings.music_max_volume;

	PDraw::font_write_line(fontti2,tekstit->Get_Text(PK_txt.sound_title),50,90);
	my += 20;

	PDraw::screen_fill(404,224+my,404+Settings.sfx_max_volume,244+my,0);
	PDraw::screen_fill(400,220+my,400+Settings.sfx_max_volume,240+my,81);

	PDraw::font_write_line(fontti2,tekstit->Get_Text(PK_txt.sound_sfx_volume),180,200+my);
	my += 20;

	const std::string& less = tekstit->Get_Text(PK_txt.sound_less);
	const std::string& more = tekstit->Get_Text(PK_txt.sound_more);

	if (drawMenuTextS2(less,180,200+my)) {
		if (Settings.sfx_max_volume > 0)
			Settings.sfx_max_volume -= 5;
	}

	if (drawMenuTextS2(more,180+8*15,200+my)) {
		if (Settings.sfx_max_volume < 100)
			Settings.sfx_max_volume += 5;
	}

	my += 40;

	PDraw::screen_fill(404,224+my,404+Settings.music_max_volume,244+my,0);
	PDraw::screen_fill(400,220+my,400+Settings.music_max_volume,240+my,112);

	PDraw::font_write_line(fontti2,tekstit->Get_Text(PK_txt.sound_music_volume),180,200+my);
	my += 20;

	if (drawMenuTextS2(less,180,200+my)) {
		if (Settings.music_max_volume > 0)
			Settings.music_max_volume -= 5;
	}

	if (drawMenuTextS2(more,180+8*15,200+my)) {
		if (Settings.music_max_volume < 100)
			Settings.music_max_volume += 5;
	}


	if(this->menuKeyDelay > 0){
		--this->menuKeyDelay;
	}
	
	my += 20;

	if (drawMenuText(PK_txt.settingsmenu_return,180,400))
		menu_nyt = MENU_SETTINGS;
	
	
	if (Settings.sfx_max_volume < 0)
		Settings.sfx_max_volume = 0;

	if (Settings.sfx_max_volume > 100)
		Settings.sfx_max_volume = 100;
	
	if (Settings.music_max_volume < 0)
		Settings.music_max_volume = 0;

	if (Settings.music_max_volume > 100)
		Settings.music_max_volume = 100;

	if (mus_volume != Settings.music_max_volume) {

		if (!Game || !Game->music_stopped)
			PSound::set_musicvolume(Settings.music_max_volume);
		Settings_Save();

	}
	if (sfx_volume != Settings.sfx_max_volume) {
		
		Settings_Save();

	}

}

void MenuScreen::Draw_Menu_Controls() {
	
	int my = 130;
	bool save_settings = false;

	Draw_BGSquare(40, 70, 640-40, 410, 224);

	PDraw::font_write_line(fontti2,tekstit->Get_Text(PK_txt.controls_title),50,90);

	if (selected_control_key > 0){
		PDraw::screen_fill(99,my-16+selected_control_key*20,584,4+my+selected_control_key*20,0);
		PDraw::screen_fill(95,my-20+selected_control_key*20,580,my+selected_control_key*20,50);
	}

	this->drawMenuTextControls(tekstit->Get_Text(PK_txt.controls_moveleft),1, 100,my);my+=20;
	this->drawMenuTextControls(tekstit->Get_Text(PK_txt.controls_moveright),2, 100,my);my+=20;
	this->drawMenuTextControls(tekstit->Get_Text(PK_txt.controls_jump),3, 100,my);my+=20;
	this->drawMenuTextControls(tekstit->Get_Text(PK_txt.controls_duck),4, 100,my);my+=20;
	this->drawMenuTextControls(tekstit->Get_Text(PK_txt.controls_walkslow), 5, 100,my);my+=20;
	this->drawMenuTextControls(tekstit->Get_Text(PK_txt.controls_doodleattack), 6, 100,my);my+=20;
	this->drawMenuTextControls(tekstit->Get_Text(PK_txt.controls_eggattack),7, 100,my);my+=20;
	this->drawMenuTextControls(tekstit->Get_Text(PK_txt.controls_useitem),8, 100,my);my+=20;

	my = 130;

	for(unsigned int i=1;i<=8;++i){

		if(selected_control_key==i){
			PDraw::font_write(fontti2, "Press Key", 380, my);
		}
		else{
			PInput::Key* key = Settings.getKeyByMenuID(i);
			if(key!=nullptr){
				PDraw::font_write(fontti2, key->getName(), 380, my);
			}
		}

		my+=20;
	}

	my += 20;

	if (drawMenuText(PK_txt.controls_get_default,100,my)) {

		if(Settings.useJoystick){
			Settings.joystickInput.setDefaultJoystick();
		}
		else{
			Settings.keyboardInput.setDefault();
		}

		selected_control_key = 0;
		save_settings = true;
	}

	my += 30;

	if(Settings.useJoystick){

		if (Settings.useControllerVibrations){

			if (drawMenuText(PK_txt.controls_vibration_on,100,my)){
				Settings.useControllerVibrations = false;
				save_settings = true;
			}
		} else {
			if (drawMenuText(PK_txt.controls_vibration_off,100,my)){
				Settings.useControllerVibrations = true;
				save_settings = true;
			}
		}
	}
	my += 20;

	if (selected_control_key == 0){

		if(Settings.useJoystick){

			if (drawMenuText(PK_txt.controls_use_keyboard ,100,my)){
				Settings.useJoystick = false;
				chosen_menu_id = 0;
				save_settings = true;
			}			
		}else if(drawMenuText(PK_txt.controls_use_controller, 100,my)){

			Settings.useJoystick = true;
			chosen_menu_id = 0;
			save_settings = true;
		}
	}

	my += 20;

	
	if(my < 400)my=400;

	if (drawMenuText(PK_txt.settingsmenu_return,180,my)){
		menu_nyt = MENU_SETTINGS;
		selected_control_key = 0;
		chosen_menu_id = 0;
	}

	if (save_settings)
		Settings_Save();

}


void MenuScreen::onKeyPressed(const PInput::Key& k){
	
	if(selected_control_key > 0){

		bool save_settings = false;
		if(k==PInput::Key::ESCAPE){
			selected_control_key = 0;
			chosen_menu_id = 0;
		} else {

			PInput::Key* key = Settings.getKeyByMenuID(this->selected_control_key);
			if(key==nullptr) {
				Play_MenuSFX(sfx_global.moo_sound,100);
			} else {
				*key = k;
			}

			++chosen_menu_id;
			save_settings = true;

			selected_control_key = 0;
		}

		if (save_settings){
			Settings_Save();
		}			
	} else {
		PK2gui::KeyNav::injectKey(k);
		Screen::onKeyPressed(k);
	}
}

void MenuScreen::Draw_Menu_Episodes() {
	int my = 0;

	Draw_BGSquare(80, 130, 640-80, 450, 224);

	PDraw::font_write_line(fontti2,tekstit->Get_Text(PK_txt.episodes_choose_episode),50,90);
	my += 80;

	size_t size = episodes.size();
	if (size > 10) {
		
		//char luku[36];
		int vali = 90;
		int x = 50;//500,
		int y = 50;//300;
		
		//vali += PDraw::font_write_line(fontti1,"page:",x,y+40);
		//sprintf(luku, "%i", episode_page + 1);
		vali += PDraw::font_write_line(fontti1,std::to_string(episode_page + 1),x+vali,y+20);
		vali += PDraw::font_write_line(fontti1,"/",x+vali,y+20);
		//sprintf(luku, "%i", int(size/10) + 1);
		vali += PDraw::font_write_line(fontti1,std::to_string(int(size/10) + 1),x+vali,y+20);

		int nappi = Draw_BackNext(x,y);

		if (nappi == 1 && episode_page > 0)
			episode_page--;

		if (nappi == 2 && (episode_page*10)+10 < size)
			episode_page++;
	}

	for (uint i = episode_page*10; i < episode_page*10 + 10; i++) {
		if (i >= size)
			break;

		if (episodes[i].is_zip)
			PDraw::font_write_line(fontti1, episodes[i].zipfile, 450, 95+my);

		if (drawMenuTextS( episodes[i].name, 110, 90+my)) {
			if (Game) {
				delete Game;
				Game = nullptr;
			}
			if (Episode) {
				delete Episode;
				Episode = nullptr;
			}

			if(config_txt.save_slots){
				Episode = new EpisodeClass(this->playerNameEdit.getText(), episodes[i]);
			}
			else{
				Episode = new EpisodeClass(config_txt.player, episodes[i]);
			}

			
			next_screen = SCREEN_MAP;
		}
		
		my += 20;
	}

	/* sivu / kaikki */
	if (drawMenuText(PK_txt.mainmenu_return,180,400)){
		menu_nyt = MENU_MAIN;
		my += 20;
	}

	//PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.episodes_get_more),140,440);

}

void MenuScreen::Draw_Menu_Language() {

	Draw_BGSquare(80, 130, 640-80, 450, 224);
	//Draw_BGSquare(110, 130, 640-110, 450, 224);

	PDraw::font_write_line(fontti2,"choose a language",50,100);

	int my = 150;

	std::size_t end = langlist.size();
	if (end > langlistindex + 10)
		end = langlistindex + 10;

	for ( std::size_t i = langlistindex; i < end; i++ ) {

		std::string lang_name = PString::removeSuffix(langlist[i], ".txt");

		if(drawMenuTextS(lang_name,110,my)) { //150

			Settings.language = langlist[i];

			try{
				Load_Language(Settings.language);
				Settings_Save();
			}
			catch(const std::exception& e){
				PLog::Write(PLog::ERR, "PK2", e.what());
				//Fallback to English
				Settings.language = "english.txt";
				Load_Language(Settings.language);
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

	if (drawMenuText(PK_txt.settingsmenu_return,180,400))
		menu_nyt = MENU_SETTINGS;

}

void MenuScreen::Draw() {

	PDraw::image_clip(bg_screen);

	menus_count = 1;

	switch (menu_nyt) {
		case MENU_MAIN     : Draw_Menu_Main();     break;
		case MENU_SETTINGS : Draw_Menu_Settings(); break;
		case MENU_EPISODES : Draw_Menu_Episodes(); break;
		case MENU_GRAPHICS : Draw_Menu_Graphics(); break;
		case MENU_SOUNDS   : Draw_Menu_Sounds();   break;
		case MENU_CONTROLS : Draw_Menu_Controls(); break;
		case MENU_NAME     : Draw_Menu_Name();     break;
		case MENU_LOAD     : Draw_Menu_Load();     break;
		case MENU_SAVE 	   : Draw_Menu_Save();     break;
		case MENU_LANGUAGE : Draw_Menu_Language(); break;
		case MENU_LINKS    : Draw_Menu_Links();    break;
		default            : Draw_Menu_Main();     break;
	}

	if (!Episode){
		PDraw::font_write_line(fontti1, PK2_VERSION_STR_MENU, 0, 470);
		if(config_txt.links_menu==LINKS_MENU_BOTTOM){
			this->bottomLinksMenu.draw(this->mousePressed);
		}
	}

	this->drawMouseCursor();
}



void MenuScreen::Init() {

	TouchScreenControls.change(UI_CURSOR);
	
	PDraw::set_offset(640, 480);
	
	langlistindex = 0;

	if (!Episode) {

		std::optional<PFile::Path> menu_path = PFilesystem::FindVanillaAsset("menu.bmp", PFilesystem::GFX_DIR);
		if(!menu_path.has_value()){
			throw std::runtime_error("\"menu.bmp\" not found!");
		}

		PDraw::image_load_with_palette(bg_screen, default_palette, *menu_path, true);
		PDraw::palette_set(default_palette);

		std::optional<PFile::Path> song9_path = PFilesystem::FindVanillaAsset("song09.xm", PFilesystem::MUSIC_DIR);
		if(!song9_path.has_value()){
			throw std::runtime_error("\"song09.xm\" not found!");
		}

		PSound::start_music(*song9_path);
		PSound::set_musicvolume(Settings.music_max_volume);
	
	} else {

		PDraw::create_shadow(bg_screen, 640, 480);

	}

	menu_nyt = MENU_MAIN;

	bg_square.left = 320 - 5;
	bg_square.top = 240 - 5;
	bg_square.right = 320 + 5;
	bg_square.bottom = 240 + 5;

	chosen_menu_id = 1;

}

void MenuScreen::Loop() {

	


	PK2gui::KeyNav::Nav nav = PK2gui::KeyNav::readKeyNav();

	if(!this->playerNameEdit.updateNavigation(nav)){

		switch (nav)
		{
		case PK2gui::KeyNav::UP:
		case PK2gui::KeyNav::LEFT:
			chosen_menu_id--;
			mouse_hidden = true;

			if (chosen_menu_id < 1)
				chosen_menu_id = menus_count-1;
			
			break;
		
		case PK2gui::KeyNav::DOWN:
		case PK2gui::KeyNav::RIGHT:
			chosen_menu_id++;
			mouse_hidden = true;

			if (chosen_menu_id > menus_count-1)
				chosen_menu_id = 1;

			break;

		case PK2gui::KeyNav::ESCAPE:{
			mouse_hidden = true;

			switch (menu_nyt)
			{
				case MENU_MAIN:{
					if(chosen_menu_id == menus_count-1)
						fadeQuit();
					else
						chosen_menu_id = menus_count-1;
				}
				break;
			case MENU_CONTROLS:
			case MENU_GRAPHICS:
			case MENU_LANGUAGE:
			case MENU_SOUNDS:
				menu_nyt = MENU_SETTINGS;
				break;

			default:
				menu_nyt = MENU_MAIN;
				break;
			}
		}
		break;
		
		default:
			break;
		}
	}

	int menu_ennen = menu_nyt;

	Draw();

	if (menu_nyt != menu_ennen)
		chosen_menu_id = 0;

	degree = 1 + degree % 360;

	if (Settings.double_speed)
		degree = 1 + degree % 360;
}