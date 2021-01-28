//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "screens/screens.hpp"

#include "engine/PLog.hpp"
#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"
#include "engine/PSound.hpp"
#include "engine/PUtils.hpp"

#include <cstring>

#include "game/game.hpp"
#include "settings.hpp"
#include "gfx/text.hpp"
#include "language.hpp"
#include "gui.hpp"
#include "episode/episodeclass.hpp"
#include "sfx.hpp"
#include "save.hpp"
#include "system.hpp"

bool going_to_game = false;

int PK_Draw_Map_Button(int x, int y, int type){

	const int BORDER = 23; //Max 23
	int ret = 0;
	
	if (PInput::mouse_x > x && PInput::mouse_x < x + BORDER
		&& PInput::mouse_y > y && PInput::mouse_y < y + BORDER) {

		if (Clicked()) {
			key_delay = 30;
			return 2;
		}

		if (type == 0)
			PDraw::image_cutcliptransparent(game_assets, 247, 1, 25, 25, x-4, y-4, 60, 32);
		if (type == 1)
			PDraw::image_cutcliptransparent(game_assets, 247, 1, 25, 25, x-2, y-2, 60, 96);
		if (type == 2)
			PDraw::image_cutcliptransparent(game_assets, 247, 1, 25, 25, x-4, y-4, 60, 64);

		ret = 1;
	}

	int flash = 50 + (int)(sin_table[degree%360]*2);

	if (flash < 0)
		flash = 0;
	
	if (type == 1)
		PDraw::image_cutcliptransparent(game_assets, 247, 1, 25, 25, x-2, y-2, flash, 96);

	if (((degree/45)+1)%4==0 || type==0)
		PDraw::image_cutclip(game_assets,x,y,1 + 25*type,58,23 + 25*type,80);

	return ret;
}

int PK_Draw_Map() {

	char luku[20];
	int vali = 20;

	PDraw::image_clip(bg_screen, 0, 0);

	ShadowedText_Draw(Episode->entry.name.c_str(),100,72);

	vali = ShadowedText_Draw(tekstit->Get_Text(PK_txt.map_total_score), 100, 92);
	
	sprintf(luku, "%i", Episode->player_score);
	ShadowedText_Draw(luku, 100 + vali + 15, 92);

	if (Episode->scores.episode_top_score > 0) {

		vali = PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.map_episode_best_player),360,72);
		PDraw::font_write(fontti1,Episode->scores.episode_top_player,360+vali+10,72);
		
		vali = PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.map_episode_hiscore),360,92);
		sprintf(luku, "%i", Episode->scores.episode_top_score);
		PDraw::font_write(fontti2,luku,360+vali+15,92);

	}

	vali = PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.map_next_level),100,120);
	sprintf(luku, "%i", Episode->level);
	PDraw::font_write(fontti1,luku,100+vali+15,120);

	//PK_Particles_Draw();

	if (Episode->level_count == 0) {
		PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.episodes_no_maps),180,290);
	}
	
	if (!going_to_game) {
		if (Draw_Menu_Text(tekstit->Get_Text(PK_txt.mainmenu_return),100,430))
			next_screen = SCREEN_MENU;
	} else {
		WavetextSlow_Draw(tekstit->Get_Text(PK_txt.mainmenu_return), fontti2, 100, 430);
	}

	int paluu;
	int icon;
	int sinx = 0, cosy = 0;
	int pekkaframe = 0;

	u32 njakso = Episode->level_count;
	for (u32 i = 0; i < Episode->level_count; i++)
		if (!Episode->level_status[i] && Episode->levels_list[i].order < njakso)
			njakso = Episode->levels_list[i].order; // Find the first unclear level
	
	if(Episode->level < njakso)
		Episode->level = njakso;

	for (u32 i = 0; i < Episode->level_count; i++) {
		if (strcmp(Episode->levels_list[i].nimi,"")!=0 && Episode->levels_list[i].order > 0) {
			
			int type = -1;
			if (Episode->levels_list[i].order == Episode->level)
				type = 1;
			if (Episode->levels_list[i].order > Episode->level)
				type = 2;
			if (Episode->level_status[i] != 0)
				type = 0;

			if (Episode->levels_list[i].x == 0)
				Episode->levels_list[i].x = 172+i*30;

			if (Episode->levels_list[i].y == 0)
				Episode->levels_list[i].y = 270;

			icon = Episode->levels_list[i].icon;

			int assets = game_assets;
			if (icon >= 22) {
				icon -= 22;
				assets = game_assets2;
			}

			//PDraw::image_clip(game_assets,Episode->levels_list[i].x-4,Episode->levels_list[i].y-4-30,1+(icon*27),452,27+(icon*27),478);
			PDraw::image_cutclip(assets,Episode->levels_list[i].x-9,Episode->levels_list[i].y-14,1+(icon*28),452,28+(icon*28),479);

			if ( type == 1 ) {
				sinx = (int)(sin_table[degree%360]/2);
				cosy = (int)(cos_table[degree%360]/2);
				pekkaframe = 28*((degree%360)/120);
				PDraw::image_cutclip(game_assets,Episode->levels_list[i].x+sinx-12,Episode->levels_list[i].y-17+cosy,157+pekkaframe,46,181+pekkaframe,79);
			}

			paluu = PK_Draw_Map_Button(Episode->levels_list[i].x-5, Episode->levels_list[i].y-10, type);

			if (Episode->level_status[i] & LEVEL_ALLAPPLES)
				PDraw::image_cutclip(game_assets2, 
					Episode->levels_list[i].x - 10,
					Episode->levels_list[i].y, 45, 379, 58, 394);

			// if clicked
			if (paluu == 2) {
				if (type != 2 || dev_mode) {

					Game = new GameClass(i);
					
					going_to_game = true;
					Fade_out(FADE_SLOW);
					PSound::set_musicvolume(0);
					Play_MenuSFX(doodle_sound,90);
				
				} else {

					Play_MenuSFX(moo_sound,100);

				}
			}

			if (!Episode->hide_numbers) {
				sprintf(luku, "%i", Episode->levels_list[i].order);
				PDraw::font_write(fontti1,luku,Episode->levels_list[i].x-12+2,Episode->levels_list[i].y-29+2);
			}

			// if mouse hoover
			if (paluu > 0) {

				int info_x = 489+3, info_y = 341-26;

				PDraw::image_cutclip(game_assets,info_x-3,info_y+26,473,0,607,121);
				PDraw::font_write(fontti1,Episode->levels_list[i].nimi,info_x,info_y+30);

				if (Episode->scores.has_score[i]) { 
					
					PDraw::font_writealpha(fontti1,tekstit->Get_Text(PK_txt.map_level_best_player),info_x,info_y+50,75);
					PDraw::font_write(fontti1,Episode->scores.top_player[i],info_x,info_y+62);
					vali = 8 + PDraw::font_writealpha(fontti1,tekstit->Get_Text(PK_txt.map_level_hiscore),info_x,info_y+74,75);
					sprintf(luku, "%i", Episode->scores.best_score[i]);
					PDraw::font_write(fontti1,luku,info_x+vali,info_y+75);
				
                }

				if (Episode->scores.has_time[i]) {

					PDraw::font_writealpha(fontti1,tekstit->Get_Text(PK_txt.map_level_fastest_player),info_x,info_y+98,75);
					PDraw::font_write(fontti1,Episode->scores.fastest_player[i],info_x,info_y+110);

					vali = 8 + PDraw::font_writealpha(fontti1,tekstit->Get_Text(PK_txt.map_level_best_time),info_x,info_y+122,75);

					s32 time = Episode->scores.best_time[i] / 60;
					if (time < 0) {
						time = -time;
						vali += PDraw::font_write(fontti1,"-",info_x+vali,info_y+122);
					}

					s32 min = time / 60;
					s32 sek = time % 60;

					sprintf(luku, "%i", min);
					vali += PDraw::font_write(fontti1,luku,info_x+vali,info_y+122);
					vali += PDraw::font_write(fontti1,":",info_x+vali,info_y+122);
                    if (sek < 10)
                        vali += PDraw::font_write(fontti1,"0",info_x+vali,info_y+122);
					sprintf(luku, "%i", sek);
					PDraw::font_write(fontti1,luku,info_x+vali,info_y+122);

				}
			}
		}
	}

	return 0;
}

int Play_Music() {

	PFile::Path mapmus = Episode->Get_Dir("");

	mapmus.SetFile("map.mp3");
	if (mapmus.Find())
		goto found;
	
	mapmus.SetFile("map.ogg");
	if (mapmus.Find())
		goto found;
	
	mapmus.SetFile("map.xm");
	if (mapmus.Find())
		goto found;
	
	mapmus.SetFile("map.mod");
	if (mapmus.Find())
		goto found;
	
	mapmus.SetFile("map.it");
	if (mapmus.Find())
		goto found;
	
	mapmus.SetFile("map.s3m");
	if (mapmus.Find())
		goto found;
	
	mapmus = PFile::Path("music" PE_SEP "map.mp3");
	if (mapmus.Find())
		goto found;
	
	mapmus.SetFile("map.ogg");
	if (mapmus.Find())
		goto found;
	
	mapmus.SetFile("map.xm");
	
	found:

	PSound::start_music(mapmus);
	PSound::set_musicvolume_now(Settings.music_max_volume);

	return 0;

}

int Screen_Map_Init() {

	// Provisory
	Settings_Save();

	if (!Episode) {
		PK2_Error("Episode not started");
		return 1;
	}

	if(PUtils::Is_Mobile())
		GUI_Change(UI_CURSOR);

	mouse_hidden = false;
	
	PDraw::set_offset(640, 480);

	degree = degree_temp;

	Episode->Load_Assets();

	PFile::Path path = Episode->Get_Dir("map.bmp");

	if (FindAsset(&path, "gfx" PE_SEP)) {

		PDraw::image_load(bg_screen, path, true);

	} else {

		PLog::Write(PLog::ERR, "PK2", "Can't load map bg");

	}

	Play_Music();

	going_to_game = false;

	Fade_in(FADE_SLOW);

	return 0;
	
}

int Screen_Map() {

	PK_Draw_Map();

	degree = 1 + degree % 360;

	if (!going_to_game && key_delay == 0) {

		if (PInput::Keydown(PInput::ESCAPE) || PInput::Keydown(PInput::JOY_START)) {
			next_screen = SCREEN_MENU;
			key_delay = 20;
		}
		
	}

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
	
		Draw_Cursor(PInput::mouse_x, PInput::mouse_y);

	}

	if (going_to_game && !Is_Fading()) {

		next_screen = SCREEN_GAME;
		
		//Draw "loading" text
		PDraw::set_offset(screen_width, screen_height);
		PDraw::screen_fill(0);
		PDraw::font_write(fontti2, tekstit->Get_Text(PK_txt.game_loading), screen_width / 2 - 82, screen_height / 2 - 9);
		Fade_out(0);

	}

	if (Episode->glows)
		if (degree % 4 == 0)
			PDraw::rotate_palette(224,239);

	return 0;
}
