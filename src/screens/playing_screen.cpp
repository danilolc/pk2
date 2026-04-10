//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include <sstream>

#include "engine/PFilesystem.hpp"
#include "playing_screen.hpp"
#include "engine/Piste.hpp"

#include <cstring>

#include "game/game.hpp"
#include "game/gifts.hpp"
#include "game/physics.hpp"

#include "gfx/text.hpp"

#include "gfx/particles.hpp"
#include "gfx/bg_particles.hpp"

#include "gfx/effect.hpp"
#include "gfx/touchscreen.hpp"
#include "episode/episodeclass.hpp"
#include "language.hpp"
#include "sfx.hpp"
#include "system.hpp"
#include "settings/settings.hpp"

PlayingScreen::PlayingScreen(){

}

PlayingScreen::~PlayingScreen(){
	
}

void PlayingScreen::onKeyPressed(const PInput::Key& key){
	Game->onKeyPressed(key);


	const InputSettings& input = Settings.getInput();

	//TODO - touchscreen

	if (key == PInput::Key::ESCAPE || key == PInput::Key::JOY_START) {
		if(test_level)
			fadeQuit();
		else if(!Game->game_over && !Game->level_clear) {
			this->goingToTheMenu = true;
		}
	}
	else if (key == PInput::Key(SDL_SCANCODE_I)) {
		if(dev_mode){
			draw_debug_info = !draw_debug_info;
		}
		else{
			show_fps = !show_fps;
		}					
	}
	else if(key == input.toggleHUD){
		Settings.draw_gui = !Settings.draw_gui;
	}

	else if(key == input.screenshotKey){
		this->takingScreenshot = true;
	}
}

void PlayingScreen::Draw_InGame_DebugInfo() {
	int vali, fy = 70;

	PDraw::set_offset(640, 480);

	vali = PDraw::font_write_line(fontti1,"sprites: ",10,fy);
	PDraw::font_write_line(fontti1,std::to_string(Game->playerSprite->level_sector->sprites.size()),10+vali,fy);
	fy += 10;

	vali = PDraw::font_write_line(fontti1,"active sprites: ",10,fy);
	PDraw::font_write_line(fontti1,std::to_string(debug_active_sprites),10+vali,fy);
	fy += 10;

	vali = PDraw::font_write_line(fontti1,"animated sprites: ",10,fy);
	PDraw::font_write_line(fontti1,std::to_string(debug_drawn_sprites),10+vali,fy);
	fy += 10;

	for (std::size_t i = 0; i < 40; i++) {
		PDraw::font_write_line(fontti1,std::to_string(i),410,10+i*10);
		PrototypeClass*proto = Game->spritePrototypes.get(i);

		if (proto == nullptr) {
			PDraw::font_write_line(fontti1,"-",430,10+i*10);
		} else {
			PDraw::font_write_line(fontti1,proto->filename,430,10+i*10);

			if (degree < 90)
				PDraw::font_write_line(fontti1,proto->transformation_str,545,10+i*10);
			else if (degree < 180)
				PDraw::font_write_line(fontti1,proto->bonus_str,545,10+i*10);
			else if (degree < 270)
				PDraw::font_write_line(fontti1,proto->ammo1_str,545,10+i*10);
			else
				PDraw::font_write_line(fontti1,proto->ammo2_str,545,10+i*10);
		}
	}

	int i = 0;
	for(const LevelEntry& entry: Episode->getLevelEntries()){
		PDraw::font_write_line(fontti1,entry.levelName,0,240+i*10);
		++i;
	}
	
	PDraw::font_write_line(fontti1, std::to_string(Game->playerSprite->x), 10, 410);
	PDraw::font_write_line(fontti1, std::to_string(Game->playerSprite->y), 10, 420);
	PDraw::font_write_line(fontti1, std::to_string(Game->playerSprite->b), 10, 430);
	PDraw::font_write_line(fontti1, std::to_string(Game->playerSprite->a), 10, 440);
	PDraw::font_write_line(fontti1, Game->playerSprite->level_sector->name, 10, 450);

	PDraw::font_write_line(fontti1, Game->level_file, 10, 460);
	PDraw::font_write_line(fontti1, std::to_string(Game->playerSprite->jump_timer), 270, 460);

	PDraw::font_write_line(fontti1, PFilesystem::GetAssetsPath(), 10, 470);

	PDraw::font_write_line(fontti1, std::to_string(Game->playerSprite->super_mode_timer), 610, 470);
	PDraw::font_write_line(fontti1, std::to_string(Game->playerSprite->invisible_timer), 610, 460);
	PDraw::font_write_line(fontti1, std::to_string(Game->button1), 610, 450);
	PDraw::font_write_line(fontti1, std::to_string(Game->button2), 610, 440);
	PDraw::font_write_line(fontti1, std::to_string(Game->button3), 610, 430);

	vali += PDraw::font_write_line(fontti1,std::to_string(Game->timeout),390,screen_height-10);

	PDraw::set_offset(screen_width, screen_height);
}

void PlayingScreen::Draw_InGame_DevKeys() {

	const char txt0[] = "dev mode";
	int char_w = PDraw::font_write_line(fontti1, txt0, 0, screen_height - 10) / strlen(txt0);
	int char_h = 10;

	const char help[] = "h: help";

	if (!PInput::Key(SDL_SCANCODE_H).isPressed()) {
		PDraw::font_write_line(fontti1, help, screen_width - strlen(help) * char_w, screen_height - 10);
		return;
	}
	const char txts[][32] = {
		"z: press buttons",
		"x: release buttons",
		"l: open locks",
		"k: open skull blocks",
		"t: toggle speed",
		"i: toggle debug info",
		"u: go up",
		"y: ghost mode",
		"r: back to start",
		"v: set invisibility",
		"s: set super mode",
		"e: set full energy",
		"a: set player",
		"end: end level",
		"mouse: move camera",
	};

	uint nof_txt = sizeof(txts) / 32;

	uint max_size = 0;
	
	for (uint i = 0; i < nof_txt; i++)
		if (strlen(txts[i]) > max_size) max_size = strlen(txts[i]);

	int posx = screen_width - max_size * char_w;
	int posy = screen_height - char_h * nof_txt;

	PDraw::screen_fill(posx - 4, posy - 4, screen_width, screen_height, 0);
	PDraw::screen_fill(posx - 2, posy - 2, screen_width, screen_height, 38);
	
	for (uint i = 0; i < nof_txt; i++)
		PDraw::font_write_line(fontti1, txts[i], posx, posy + i*10);
}


void PlayingScreen::Draw_InGame_Lower_Menu() {
	int x, y;

	if (Game->has_time) {
		int vali = 0;

		int timeout = Game->timeout;
		if(timeout<0)timeout = 0;


		float shown_time = float(timeout) / 60;
		int min = int(shown_time/60);
		int sek = int(shown_time)%60;

		x = screen_width / 2 + 69;
		y = screen_height-39;
		PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.game_time),x,y-20);

		//sprintf(luku, "%i", min);
		vali += ShadowedText_Draw(std::to_string(min), x, y);
		vali += PDraw::font_write_line(fontti1,":",x+vali,y+9);

		if (sek < 10)
			vali += ShadowedText_Draw("0", x + vali, y);
		
		//sprintf(luku, "%i", sek);
		vali += ShadowedText_Draw(std::to_string(sek), x + vali, y);

	}

	/////////////////
	// Draw keys / enemies
	/////////////////
	if(Game->level.game_mode==GAME_MODE_KILL_ALL){
		x = screen_width / 2 + 210;
		y = screen_height - 39;
		PDraw::font_write_line(fontti1,"enemies:",x,y-20);
		ShadowedText_Draw(std::to_string(Game->enemies), x, y);

	}
	else if (Game->keys > 0){
		x = screen_width / 2 + 210;
		y = screen_height - 39;
		PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.game_keys),x,y-20);
		ShadowedText_Draw(std::to_string(Game->keys), x, y);
	}

	/////////////////
	// Draw Gifts
	/////////////////

	if (Game->gifts.count() > 0 && Game->item_panel_x < 10)
	Game->item_panel_x++;

	if (Game->gifts.count() == 0 && Game->item_panel_x > -215)
		Game->item_panel_x--;

	if (Game->item_panel_x > -215)
		PDraw::image_cutclip(Game->gfxTexture,Game->item_panel_x,screen_height-60,
								1,216,211,266);
	if (Game->item_panel_x > 5)
		PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.game_items),15,screen_height-65);

	Game->gifts.draw(Game->item_panel_x + 35, screen_height-35);
}

void PlayingScreen::Draw_InGame_UI(){
	int vali = 20;
	int my = 14;

	/////////////////
	// Draw Energy
	/////////////////
	vali = PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.game_energy),60,my);

	//SpriteClass* Game->playerSprite = Game->playerSprite;
	ShadowedText_Draw(std::to_string(Game->playerSprite->energy), 60 + vali, my);

	/////////////////
	// Draw Invisible
	/////////////////
	if(Game->playerSprite->super_mode_timer > 0){
		vali = PDraw::font_write_line(fontti1, tekstit->Get_Text(PK_txt.game_supermode),60,my+27);
		//sprintf(luku, "%i", Game->playerSprite->invisible_timer/60);
		std::string super_mode_timer_s = std::to_string(Game->playerSprite->super_mode_timer/60);
		PDraw::font_write_line(fontti2,super_mode_timer_s,60+vali+1,my+27+1);
		PDraw::font_write_line(fontti2,super_mode_timer_s,60+vali,my+27);
	}

	else if(Game->playerSprite->invisible_timer > 0){
		vali = PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.game_invisible),60,my+27);
		//sprintf(luku, "%i", Game->playerSprite->invisible_timer/60);
		std::string invisible_timer_s = std::to_string(Game->playerSprite->invisible_timer/60);
		PDraw::font_write_line(fontti2,invisible_timer_s,60+vali+1,my+27+1);
		PDraw::font_write_line(fontti2,invisible_timer_s,60+vali,my+27);
	}

	/////////////////
	// Draw Mini Apple
	/////////////////
	if (Game->apples_count > 0) {
		if (Game->apples_got == Game->apples_count)
			PDraw::image_cutcliptransparent(global_gfx_texture2, 
				45, 379, 13, 15, 
				my, my, 
				sin_table(degree)*1.5+60, COLOR_RED);
		else
			PDraw::image_cutcliptransparent(global_gfx_texture2, 
				45, 379, 13, 15, 
				my, my, 
				0, COLOR_GRAY);
	}

	/////////////////
	// Draw Score
	/////////////////
	vali = PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.game_score),230,my);
	//sprintf(luku, "%i", Game->score);		
	ShadowedText_Draw(std::to_string(Game->score), 230 + vali, my);

	/////////////////
	// Draw Ammunition
	/////////////////
	if (Game->playerSprite->ammo2 != nullptr){
		PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.game_attack1), screen_width-170,my);
		Game->playerSprite->ammo2->draw(screen_width-170,my+10,0);
	}

	if (Game->playerSprite->ammo1 != nullptr){
		PDraw::font_write_line(fontti1,tekstit->Get_Text(PK_txt.game_attack2), screen_width-90,my+15);
		Game->playerSprite->ammo1->draw(screen_width-90,my+25,0);
	}

	/////////////////
	// Draw Info
	/////////////////
	Game->drawInfoText();
}

void PlayingScreen::drawDevStuff(){
	if (this->draw_debug_info)
		Draw_InGame_DebugInfo();
	else {
		if (dev_mode)
			Draw_InGame_DevKeys();
		if (test_level)
			PDraw::font_write_line(fontti1, "testing level", 0, screen_height - 20);
		if (show_fps) {
			
			int fps = Piste::get_fps();
			int txt_size;

			if (fps >= 100)
				txt_size = PDraw::font_write_line(fontti1, "fps:", 570, 48);
			else
				txt_size = PDraw::font_write_line(fontti1, "fps: ", 570, 48);
			
			PDraw::font_write_line(fontti1, std::to_string(fps), 570 + txt_size, 48);
		
		}
	}
}

void PlayingScreen::Draw() {
	LevelSector* sector = Game->playerSprite->level_sector;


	debug_drawn_sprites = 0;

	sector->drawBackground(Game->camera_x, Game->camera_y);

	sector->sprites.drawBGsprites(Game->camera_x, Game->camera_y, Game->paused, this->debug_drawn_sprites);

	BG_Particles::Draw(Game->camera_x, Game->camera_y);

	Game->level.drawBackgroundTiles(Game->camera_x,Game->camera_y, sector);

	sector->sprites.drawSprites(Game->camera_x, Game->camera_y, Game->paused, this->debug_drawn_sprites);

	Particles_DrawFront(Game->camera_x, Game->camera_y);

	Game->level.drawForegroundTiles(Game->camera_x,Game->camera_y, sector);

	sector->sprites.drawFGsprites(Game->camera_x, Game->camera_y, Game->paused, this->debug_drawn_sprites);

	if(Settings.draw_gui){
		Draw_InGame_Lower_Menu();
	}
	
	else if (Game->gifts.count() > 0){
		Game->item_panel_x = 10;
	}
	else{
		Game->item_panel_x = -215;
	}

	Fadetext_Draw();

	if(Settings.draw_gui){
		Draw_InGame_UI();
	}
	
	if (Game->paused) {
		const std::string& txt = tekstit->Get_Text(PK_txt.game_paused);
		std::pair<int, int> p = PDraw::font_get_text_size(fontti2, txt);
		PDraw::font_write_line(fontti2,txt,screen_width/2-p.first/2,screen_height/2-p.second/2);
	}

	if (Game->level_clear) {
		const std::string& txt = tekstit->Get_Text(PK_txt.game_clear);
		std::pair<int,int> p = PDraw::font_get_text_size(fontti2, txt);
		Wavetext_Draw(txt.c_str(),fontti2,screen_width/2-p.first/2,screen_height/2-p.second/2);

	} else if (Game->game_over) {

		if (Game->playerSprite->energy < 1) {
			const std::string& txt = tekstit->Get_Text(PK_txt.game_ko);
			std::pair<int, int> p = PDraw::font_get_text_size(fontti2, txt);

			Wavetext_Draw(txt.c_str(),fontti2,screen_width/2-p.first/2,screen_height/2 - p.second);

		} else if (Game->timeout < 1 && Game->has_time) {

			const std::string& txt = tekstit->Get_Text(PK_txt.game_timeout);
			std::pair<int, int> p = PDraw::font_get_text_size(fontti2, txt);

			Wavetext_Draw(txt.c_str(),fontti2,screen_width/2-p.first/2,screen_height/2 - p.second);
		}

		const std::string& txt = tekstit->Get_Text(PK_txt.game_tryagain);
		std::pair<int, int> p = PDraw::font_get_text_size(fontti2, txt);

		Wavetext_Draw(txt.c_str(),fontti2,screen_width/2-p.first/2,screen_height/2 );
	}

	if(Settings.draw_gui){
		this->drawDevStuff();
	}

	/**
	 * @brief 
	 * For debugging touchscreen controls
	 */
	if(dev_mode && Settings.touchscreen_mode){
		this->drawMouseCursor();
	}
}

void PlayingScreen::Init(){

	TouchScreenControls.change(UI_GAME_BUTTONS);
	
	PDraw::set_offset(screen_width, screen_height);

	if (!Game->isStarted()) {

		Game->start();
		degree = 0;
	
	} else {
	
		degree = degree_temp;
	
	}
}

void PlayingScreen::Loop(){

	Game->update(this->debug_active_sprites);

	static bool skip_frame = false;

	if (Settings.double_speed) skip_frame = !skip_frame;
	else skip_frame = false;

	if (!skip_frame) {

		this->Draw();

		if(this->takingScreenshot){
			this->takingScreenshot = false;
			std::string name = PFilesystem::GetScreenshotName();
			PLog::Write(PLog::INFO, "PK2", "Taken %s", name.c_str());
			PDraw::take_screenshot(name);
		}
	} else {

		Piste::ignore_frame();

	}
	
	if (Game->exit_timer == 1 && !Is_Fading()) {
		if (Game->level_clear) {

			next_screen = SCREEN_SCORING;

		}
		else if(test_level){
			Piste::stop();
		}
		else {		
			delete Game;
			Game = nullptr;
			next_screen = SCREEN_MAP;
		}
	}
	
	if (this->goingToTheMenu || TouchScreenControls.menu) {
		this->goingToTheMenu = false;
		int w, h;
		PDraw::image_getsize(bg_screen, w, h);
		if (w != screen_width) {
			PDraw::image_delete(bg_screen);
			bg_screen = PDraw::image_new(screen_width, screen_height);
		}
		PDraw::image_snapshot(bg_screen);


		next_screen = SCREEN_MENU;
		degree_temp = degree;
	}
}