//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "screens/screens.hpp"

#include "gfx/text.hpp"
#include "language.hpp"
#include "game/game.hpp"
#include "game/gifts.hpp"
#include "gui.hpp"
#include "episode/episodeclass.hpp"
#include "sfx.hpp"
#include "game/sprites.hpp"
#include "system.hpp"
#include "settings.hpp"
#include "save.hpp"

#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"
#include "engine/PSound.hpp"
#include "engine/PUtils.hpp"

#include <cstring>

enum {
	COUNT_NOTHING,
	COUNT_BONUS,
	COUNT_TIME,
	COUNT_ENERGY,
	COUNT_GIFTS,
	COUNT_APPLES,
	COUNT_ENDED
};

bool going_to_map;

int counting_phase;
int counting_delay;

float apples_xoffset;
u32 apples_counted;
u32 apples_not_counted;

u32 total_seconds;

u32 total_score;
u32 bonus_score;
s32 time_score;
u32 energy_score;
u32 gifts_score;

bool map_new_record;
bool map_new_time_record;
bool episode_new_record;

int LevelScore_Compare(int level, u32 score, u32 apples, s32 time){
	
	int ret = 0;

	if (!Episode->scores.has_score[level] || (score > Episode->scores.best_score[level])) {

		strcpy(Episode->scores.top_player[level], Episode->player_name);
		Episode->scores.best_score[level] = score;
		Episode->scores.has_score[level] = true;
		map_new_record = true;
		ret++;
	
	}

	if (Game->apples_count > 0) {
		if (apples > Episode->scores.max_apples[level]) {

			Episode->scores.max_apples[level] = apples;
			ret++;

		}
	}

	if (Game->has_time) {

		if (!Episode->scores.has_time[level] || (time < Episode->scores.best_time[level])) {

			strcpy(Episode->scores.fastest_player[level],Episode->player_name);
			Episode->scores.best_time[level] = time;
			Episode->scores.has_time[level] = true;
			map_new_time_record = true;
			ret++;

		}
	}

	return ret;

}
int EpisodeScore_Compare(u32 score){

	int ret = 0;

	if (score > Episode->scores.episode_top_score) {

		Episode->scores.episode_top_score = score;
		strcpy(Episode->scores.episode_top_player,Episode->player_name);
		episode_new_record = true;
		ret++;

	}

	return ret;

}

int Draw_ScoreCount() {

	char luku[20];
	int x, y;

	PDraw::image_clip(bg_screen);

	/* BG Effect */
	for (int i = 0; i < 18; i++) {


		int kerroin = (int)(cos_table[(degree+i*3)%180]);

		x = (int)(sin_table(degree+i*10)*2)+kerroin;
		y = (int)(cos_table(degree+i*10)*2);//10 | 360 | 2
		//PDraw::image_clip(game_assets,320+x,240+y,157,46,181,79);
		int kuutio = (int)(sin_table(degree+i*3));
		if (kuutio < 0) kuutio = -kuutio;

		PDraw::screen_fill(320-x,240-y,320-x+kuutio,240-y+kuutio,COLOR_TURQUOISE+8);

	}
	for (int i = 0; i < 18; i++) {

		x = (int)(sin_table(degree+i*10)*3);
		y = (int)(cos_table(degree+i*10)*3);//10 | 360 | 3
		//PDraw::image_clip(game_assets,320+x,240+y,157,46,181,79);
		int kuutio = (int)(sin_table(degree+i*2))+18;
		if (kuutio < 0) kuutio = -kuutio;//0;//
		if (kuutio > 100) kuutio = 100;

		//PDraw::screen_fill(320+x,240+y,320+x+kuutio,240+y+kuutio,COLOR_TURQUOISE+10);
		PDraw::image_cutcliptransparent(game_assets, 247, 1, 25, 25, 320+x, 240+y, kuutio, 32);

	}
	/* --------- */

	int my = 60;

	ShadowedText_Draw(tekstit->Get_Text(PK_txt.score_screen_title), 100, my);
	my += 30;

	ShadowedText_Draw(tekstit->Get_Text(PK_txt.score_screen_level_score), 100, my);

	total_score = bonus_score + time_score + energy_score + gifts_score;
	
	sprintf(luku, "%i", total_score);
	ShadowedText_Draw(luku, 400, my);
	my += 70;

	ShadowedText_Draw(tekstit->Get_Text(PK_txt.score_screen_bonus_score), 100, my);
	
	sprintf(luku, "%i", bonus_score);
	ShadowedText_Draw(luku, 400, my);
	my += 30;

	ShadowedText_Draw(tekstit->Get_Text(PK_txt.score_screen_time_score), 100, my);
	
	sprintf(luku, "%i", time_score);
	ShadowedText_Draw(luku, 400, my);
	my += 30;

	ShadowedText_Draw(tekstit->Get_Text(PK_txt.score_screen_energy_score), 100, my);

	sprintf(luku, "%i", energy_score);
	ShadowedText_Draw(luku, 400, my);
	my += 30;

	ShadowedText_Draw(tekstit->Get_Text(PK_txt.score_screen_item_score), 100, my);
	
	sprintf(luku, "%i", gifts_score);
	ShadowedText_Draw(luku, 400, my);
	my += 40;

	x = 110;
	for (int i = 0; i < MAX_GIFTS; i++) {
		
		if (Gifts_Get(i) != nullptr)	{
			Gifts_Draw(i, x, my);
			x += 38;
		}

	}
	my += 20;

	// Draw apples
	PDraw::set_mask(0, 0, 640, 480);
	if (Game->apples_count > 0) {

		uint i = 0;

		for (; i < apples_counted; i++) {

			if (apples_counted >= Game->apples_count)
				PDraw::image_cutclip(game_assets2, apples_xoffset + i * 32 + rand()%2, my + rand()%2, 61, 379, 87, 406);
			else
				PDraw::image_cutclip(game_assets2, apples_xoffset + i * 32, my, 61, 379, 87, 406);

		}
		for (; i < Game->apples_count; i++)
			PDraw::image_cutcliptransparent(game_assets2, 61, 379, 26, 26, apples_xoffset + i * 32, my, 20, 0);
		
	}
	PDraw::reset_mask();

	if (apples_counted >= 13 && apples_not_counted != 0)
		apples_xoffset -= 32.0 / 10;

	my += 40;

	if (counting_phase == COUNT_ENDED) {
		
		ShadowedText_Draw(tekstit->Get_Text(PK_txt.score_screen_total_score), 100, my);
		
		sprintf(luku, "%i", Episode->player_score);
		ShadowedText_Draw(luku, 400, my);
		my += 25;

		//if (apples_counted >= Game->apples_count && apples_counted > 0) {

		//	PDraw::font_write(fontti2, "You have found all apples!", 100 + rand()%2, my + rand()%2);
		//	my += 25;

		//}

		if (map_new_record) {

			PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.score_screen_new_level_hiscore),100+rand()%2,my+rand()%2);
			my += 25;

		}
		if (map_new_time_record) {

			PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.score_screen_new_level_best_time),100+rand()%2,my+rand()%2);
			my += 25;

		}
		if (episode_new_record) {

			PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.score_screen_new_episode_hiscore),100+rand()%2,my+rand()%2);
			my += 25;

		}
	}

	if (Draw_Menu_Text(tekstit->Get_Text(PK_txt.score_screen_continue),15,430)) {

		going_to_map = true;
		PSound::set_musicvolume(0);
		Fade_out(FADE_SLOW);

	}

	if (!PUtils::Is_Mobile() || !Settings.gui)
		Draw_Cursor(PInput::mouse_x, PInput::mouse_y);

	return 0;
}


int Screen_ScoreCount_Init() {

	if(PUtils::Is_Mobile())
		GUI_Change(UI_CURSOR);
	
	PDraw::set_offset(640, 480);

	PDraw::image_delete(bg_screen);
	bg_screen = PDraw::image_load(PFile::Path("gfx" PE_SEP "menu.bmp"), true);
	PDraw::create_shadow(bg_screen, 640, 480);

	map_new_record = false;
	map_new_time_record = false;
	episode_new_record = false;

	Game->score += Game->score_increment;

	// Lasketaan pelaajan kokonaispisteet etuk�teen
	u32 temp_score = 0;
	temp_score += Game->score;
	temp_score += Game->timeout / 12; //(Game->timeout / 60) * 5;
	temp_score += Player_Sprite->energia * 300;
	for (int i = 0; i < MAX_GIFTS; i++)
		if (Gifts_Get(i) != nullptr)
			temp_score += Gifts_Get(i)->pisteet + 500;

	if (!Game->repeating)
		Episode->player_score += temp_score;

	apples_counted = 0;
	apples_not_counted = Game->apples_got;
	apples_xoffset = 100;

	total_score = 0;
	counting_phase = COUNT_NOTHING;
	counting_delay = 30;
	bonus_score = 0;
	time_score = 0;
	energy_score = 0;
	gifts_score = 0;

	/* Check if broken level score and time record */
	int episode_result = EpisodeScore_Compare(Episode->player_score);
	int level_result = 
		LevelScore_Compare(Game->level_id, temp_score, Game->apples_got, Game->map.aika * TIME_FPS - Game->timeout);
	
	if (episode_result > 0 || level_result > 0) {

		Episode->Save_Scores();

	}

	// Save backup when finishing a level
	Save_Record(10);

	PSound::set_musicvolume(Settings.music_max_volume);

	going_to_map = false;
	Fade_in(FADE_FAST);

	return 0;

}

int Screen_ScoreCount() {

	Draw_ScoreCount();

	degree = 1 + degree % 360;

	if (counting_delay == 0) {
	
		if (bonus_score < Game->score) {

			counting_phase = COUNT_BONUS;
			counting_delay = 0;
			bonus_score += 10;

			if (degree%7==1)
				Play_MenuSFX(score_sound, 70);

			if (bonus_score >= Game->score){
				bonus_score = Game->score;
				counting_delay = 50;
			}

		} else if (time_score < Game->timeout / 12) {

			counting_phase = COUNT_TIME;
			counting_delay = 0;
			time_score += 5;

			if (degree%10==1)
				Play_MenuSFX(score_sound, 70);

			if (time_score >= Game->timeout / 12) {
				time_score = Game->timeout / 12;
				counting_delay = 50;
			}

		} else if (Player_Sprite->energia > 0) {

			counting_phase = COUNT_ENERGY;
			counting_delay = 10;
			energy_score+=300;
			Player_Sprite->energia--;

			Play_MenuSFX(score_sound, 70);

		} else if (Gifts_Count() > 0) {
			
			counting_phase = COUNT_GIFTS;
			counting_delay = 30;
			gifts_score += Gifts_Get(0)->pisteet + 500;
			Gifts_Remove(0); 
			Play_MenuSFX(jump_sound, 100);

		} else if (apples_not_counted > 0) {

			counting_phase = COUNT_APPLES;
			counting_delay = 10;

			apples_counted++;
			apples_not_counted--;
			Play_MenuSFX(apple_sound, 70);

			if (apples_not_counted == 0)
				counting_delay = 20;
		
		} else {
			
			counting_phase = COUNT_ENDED;
		
		}
	}

	if (counting_delay > 0)
		counting_delay--;

	if (Clicked() || going_to_map){

		if(counting_phase == COUNT_ENDED && !going_to_map) {

			going_to_map = true;
			Fade_out(FADE_SLOW);
			PSound::set_musicvolume(0);
			key_delay = 20;

		} else {

			counting_phase = COUNT_ENDED;

			apples_counted += apples_not_counted;
			apples_not_counted = 0;
			if (Game->apples_got > 13)
				apples_xoffset = 100.0 - (Game->apples_got - 13) * 32;

			bonus_score = Game->score;

			time_score = Game->timeout / 12;

			energy_score += Player_Sprite->energia * 300;
			Player_Sprite->energia = 0;

			for (int i = 0; i < Gifts_Count(); i++)
				gifts_score += Gifts_Get(i)->pisteet + 500;
			
			Gifts_Clean();

			key_delay = 20;

		}

	}

	if (going_to_map && !Is_Fading()){

		if (Episode->next_level == UINT32_MAX && !Game->repeating && !Episode->no_ending) {

			next_screen = SCREEN_END;

		} else {

			next_screen = SCREEN_MAP;
		
		}

		delete Game;
		Game = nullptr;
	}

	if (Episode->glows)
		if (degree % 4 == 0)
			PDraw::rotate_palette(224,239);

	return 0;

}