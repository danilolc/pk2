//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include <iostream>

#include "map_screen.hpp"

#include "engine/PLog.hpp"
#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"
#include "engine/PSound.hpp"
#include "engine/PFilesystem.hpp"

#include <array>
#include <string>

#include <cstring>

#include "game/game.hpp"
#include "game/spriteclass.hpp"
#include "settings/settings.hpp"
#include "gfx/text.hpp"
#include "language.hpp"
#include "gfx/touchscreen.hpp"
#include "episode/episodeclass.hpp"
#include "sfx.hpp"
#include "system.hpp"
#include "exceptions.hpp"

MapScreen::MapScreen(){
	this->mouseKeysEnabled = true;
}

int MapScreen::PK_Draw_Map_Button(int x, int y, int type){

	const int BORDER = 23; //Max 23
	int ret = 0;

	const Point2D mousePos = PInput::InputSystem::instance().getMousePos();
	
	if (mousePos.x > x && mousePos.x < x + BORDER
		&& mousePos.y > y && mousePos.y < y + BORDER) {

		if (this->enterPressed || this->mousePressed) {
			return 2;
		}

		if (type == 0)
			PDraw::image_cutcliptransparent(global_gfx_texture, 247, 1, 25, 25, x-3, y-3, 60, 32);
		if (type == 1)
			PDraw::image_cutcliptransparent(global_gfx_texture, 247, 1, 25, 25, x-3, y-3, 60, 96);
		if (type == 2)
			PDraw::image_cutcliptransparent(global_gfx_texture, 247, 1, 25, 25, x-3, y-3, 60, 64);

		ret = 1;
	}

	int flash = 50 + (int)(sin_table(degree)*2);

	if (flash < 0)
		flash = 0;
	
	if (type == 1)
		PDraw::image_cutcliptransparent(global_gfx_texture, 247, 1, 25, 25, x-3, y-3, flash, 96);

	if (((degree/45)+1)%4==0 || type==0)
		PDraw::image_cutclip(global_gfx_texture,x-1,y-1,1 + 25*type,58,23 + 25*type,80);

	return ret;
}

void MapScreen::Draw() {

	PDraw::image_clip(bg_screen, 0, 0);

	ShadowedText_Draw(Episode->entry.name, 100, 72);

	int ysize = ShadowedText_Draw(tekstit->Get_Text(PK_txt.map_total_score), 100, 92);
	
	ShadowedText_Draw(std::to_string(Episode->getPlayerScore()), 100 + ysize + 15, 92);

	if (Episode->scoresTable.episodeTopScore > 0) {

		ysize = PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.map_episode_best_player),360,72).first;
		PDraw::font_write_line(fontti1,Episode->scoresTable.episodeTopPlayer,360+ysize+10,72);
		
		ysize = PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.map_episode_hiscore),360,92).first;
		PDraw::font_write_line(fontti2,std::to_string(Episode->scoresTable.episodeTopScore), 360+ysize+15,92);

	}

	if (Episode->next_level <= Episode->getHighestLevelNumber()) {
		ysize = PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.map_next_level),100,120).first;
		PDraw::font_write_line(fontti1,std::to_string(Episode->next_level),100+ysize + 8,120);
	}

	if (Episode->getLevelsNumber() == 0) {
		const std::string& txt = tekstit->Get_Text(PK_txt.episodes_no_maps);
		std::pair<int, int> p = PDraw::font_get_text_size(fontti2, txt);
		PDraw::font_write(fontti2, txt, 320 - p.first/2, 240 - p.second);
		//PDraw::font_write(fontti2,txt,180,290);
	}
	
	if (!going_to_game) {
		if (drawMenuText(PK_txt.mainmenu_return,100,430))
			next_screen = SCREEN_MENU;
	} else {
		WavetextSlow_Draw(tekstit->Get_Text(PK_txt.mainmenu_return).c_str(), fontti2, 100, 430);
	}

	//for (u32 i = 0; i < Episode->level_count; i++) {
	u32 i = 0;
	for(const LevelEntry& entry: Episode->getLevelEntries()){
		if (!entry.levelName.empty() && entry.number > 0) {
			
			int type = -1;
			if (entry.number == Episode->next_level)
				type = 1;
			if (entry.number > Episode->next_level)
				type = 2;
			if (entry.status != 0)
				type = 0;

			int x = entry.map_x;
			int y = entry.map_y;

			int icon = entry.icon_id;

			int assets = global_gfx_texture;
			if (icon >= 22) {
				icon -= 22;
				assets = global_gfx_texture2;
			}

			PDraw::image_cutclip(assets,x-9,y-14,1+(icon*28),452,28+(icon*28),479);

			//draw circling rooster head
			if ( type == 1 ) {
				int sinx = (int)(sin_table(degree)/2);
				int cosy = (int)(cos_table(degree)/2);
				int pekkaframe = 28*((degree%360)/120);
				PDraw::image_cutclip(global_gfx_texture,x+sinx-8,y-17+cosy,157+pekkaframe,46,182+pekkaframe,80);
			}

			int paluu = PK_Draw_Map_Button(x-5, y-10, type);

			if (!Episode->ignore_collectable) {
				if (entry.status & LEVEL_ALLAPPLES){
					PDraw::image_cutclip(global_gfx_texture2, 
						x - 10,
						y, 45, 379, 58, 394);
				}
				else if(entry.status & LEVEL_HAS_BIG_APPLES){
					PDraw::image_cutclip(global_gfx_texture2, 
						x - 10,
						y, 45, 397, 58, 412);
				}
				//else //TODO - draw transparent apples
				//	PDraw::image_cutcliptransparent(global_gfx_texture2, 
				//		45, 379, 58-45, 394-379, x - 10, y, sin_table(degree)*3 - 10, COLOR_GRAY);
			}

			if (Episode->isCompleted()) {

				int dd = (degree / 3) % 60;
				int order = entry.number;

				int a = 0;
				if (order < dd)
					a = 100 - (dd - order) * 5;
				
				if (a > 0)
					PDraw::image_cutcliptransparent(global_gfx_texture, 247, 1, 25, 25, x-8, y-13, a, COLOR_TURQUOISE);

			}

			// if clicked
			if (paluu == 2) {
				if (type != 2 || dev_mode) {

					if(Game!=nullptr){
						delete Game;
						Game = nullptr;
					}

					Game = new GameClass(i);
					
					going_to_game = true;
					Fade_out(FADE_SLOW);
					PSound::set_musicvolume(0);
					Play_MenuSFX(Episode->sfx.doodle_sound,90);
				
				} else {

					Play_MenuSFX(Episode->sfx.moo_sound,100);

				}
			}

			if (!Episode->hide_numbers) {
				//sprintf(luku, "%i", Episode->levels_list[i].order);
				PDraw::font_write_line(fontti1,std::to_string(entry.number),x-12+2,y-29+2);
			}

			// if mouse hoover
			if (paluu > 0) {

				int info_x = 489+3, info_y = 341-26;

				PDraw::image_cutclip(global_gfx_texture,info_x-3,info_y+26,473,0,608,122);

				info_y += 30;
				info_y += PDraw::font_write(fontti1,entry.levelName,info_x,info_y).second;
				info_y += 10;				

				LevelScore* levelScore = Episode->scoresTable.getScoreByLevelName(entry.fileName);
				if(levelScore!=nullptr){

					PDraw::font_writealpha_s(fontti1,tekstit->Get_Text(PK_txt.map_level_best_player),info_x,info_y,75);
					info_y+=12;

					PDraw::font_write_line(fontti1,levelScore->topPlayer,info_x,info_y);
					info_y+=12;

					ysize = 8 + PDraw::font_writealpha_s(fontti1,tekstit->Get_Text(PK_txt.map_level_hiscore),info_x,info_y,75).first;


					//sprintf(luku, "%i", Episode->scores.best_score[i]);
					info_y+=1;
					PDraw::font_write_line(fontti1,std::to_string(levelScore->bestScore),info_x+ysize,info_y);

					if(levelScore->hasTime){
						info_y+=23;

						PDraw::font_writealpha_s(fontti1,tekstit->Get_Text(PK_txt.map_level_fastest_player),info_x,info_y,75);
						info_y+=12;

						PDraw::font_write_line(fontti1,levelScore->fastestPlayer,info_x,info_y);
						info_y+=12;

						ysize = 8 + PDraw::font_writealpha_s(fontti1,tekstit->Get_Text(PK_txt.map_level_best_time),info_x,info_y,75).first;

						s32 time = levelScore->bestTime / 60;
						if (time < 0) {
							time = -time;
							ysize += PDraw::font_write_line(fontti1,"-",info_x+ysize,info_y);
						}

						s32 min = time / 60;
						s32 sek = time % 60;
						
						std::string min_s = std::to_string(min);
						std::string sek_s = std::to_string(sek);

						//sprintf(luku, "%i", min);
						ysize += PDraw::font_write_line(fontti1,min_s,info_x+ysize,info_y);
						ysize += PDraw::font_write_line(fontti1,":",info_x+ysize,info_y);
						if (sek < 10)
							ysize += PDraw::font_write_line(fontti1,"0",info_x+ysize,info_y);
						//sprintf(luku, "%i", sek);
						PDraw::font_write_line(fontti1,sek_s,info_x+ysize,info_y);
					}
				}
			}
		}
		++i;
	}
}

void MapScreen::Play_Music() {
	static const std::array<std::string, 6> map_music_filenames = {
		"map.xm",
		"map.ogg",
		"map.mp3",
		"map.s3m",
		"map.mod",
		"map.it",
	};

	std::optional<PFile::Path> mapmus = {};

	for(const std::string& music_name:map_music_filenames){
		mapmus = PFilesystem::FindEpisodeAsset(music_name, PFilesystem::MUSIC_DIR);

		if(mapmus.has_value())break;
	}
	if(!mapmus.has_value()){
		//mapmus =   PFile::Path("music" PE_SEP);
		for(const std::string& music_name:map_music_filenames){
			//mapmus.SetFile(music_name);

			mapmus = PFilesystem::FindVanillaAsset(music_name, PFilesystem::MUSIC_DIR);
			if(mapmus.has_value())break;
		}
	}

	if(mapmus.has_value()){
		PSound::start_music(*mapmus);
		PSound::set_musicvolume_now(Settings.music_max_volume);
	}
	else{
		PLog::Write(PLog::ERR,"PK2","Map music not found!");
	}
}

void MapScreen::Init() {

	if (!Episode) {
		throw PExcept::PException("Episode not started!");
	}

	TouchScreenControls.change(UI_CURSOR);

	mouse_hidden = false;
	
	PDraw::set_offset(640, 480);

	degree = degree_temp;

	// Load custom assets (should be done when creating Episode)
	Episode->loadAssets();

	std::optional<PFile::Path> path = PFilesystem::FindAsset("map.bmp", PFilesystem::GFX_DIR, ".png");

	if (path.has_value()) {
		PDraw::image_load_with_palette(bg_screen, default_palette, *path, true);
		PDraw::palette_set(default_palette);

	} else {

		PLog::Write(PLog::ERR, "PK2", "Can't load map bg");

	}

	Play_Music();

	going_to_game = false;

	Fade_in(FADE_SLOW);
}


void MapScreen::onKeyPressed(const PInput::Key& k){
	if(!going_to_game){

		if(k==PInput::Key::ESCAPE || k==PInput::Key::JOY_START){
			next_screen = SCREEN_MENU;
		}
	}

	Screen::onKeyPressed(k);
}


void MapScreen::Loop() {

	this->Draw();

	degree = 1 + degree % 360;
	if (next_screen == SCREEN_MENU) {

		int w, h;
		PDraw::image_getsize(bg_screen, w, h);
		if (w != screen_width) {
			PDraw::image_delete(bg_screen);
			bg_screen = PDraw::image_new(screen_width, screen_height);
		}
		PDraw::image_snapshot(bg_screen);

		degree_temp = degree;

	} else {

		this->drawMouseCursor();
	}

	if (going_to_game && !Is_Fading()) {

		next_screen = SCREEN_GAME;
		
		//Draw "loading" text
		PDraw::set_offset(screen_width, screen_height);
		PDraw::screen_fill(0);

		const std::string& txt = tekstit->Get_Text(PK_txt.game_loading);
		std::pair<int, int> p = PDraw::font_get_text_size(fontti2, txt);

		int x = screen_width / 2 - p.first / 2;
		int y = screen_height / 2 - p.second / 2;
		PDraw::font_write(fontti2, txt, x, y);
		Fade_out(0);

	}

	if (Episode->glows)
		if (degree % 4 == 0)
			PDraw::rotate_palette(224,239);
}
