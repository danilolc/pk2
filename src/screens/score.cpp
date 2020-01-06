//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
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

#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"
#include "engine/PSound.hpp"

#include <cstring>

bool siirry_pistelaskusta_karttaan = false;

int counting_phase = 0;
int counting_delay = 0;

u32 apples_got = 0;

u32 total_score = 0;
u32 bonus_score = 0;
u32 time_score = 0;
u32 energy_score = 0;
u32 gifts_score = 0;

bool map_new_record = false;
bool map_new_time_record = false;
bool episode_new_record = false;

int EpisodeScore_Compare(int level, u32 episteet, u32 time, bool final_score){
	
	int ret = 0;

	if (!final_score) {

		if (episteet > Episode->scores.best_score[level]) {

			strcpy(Episode->scores.top_player[level], Episode->player_name);
			Episode->scores.best_score[level] = episteet;
			map_new_record = true;
			ret++;

		}
		if ((time < Episode->scores.best_time[level] || Episode->scores.best_time[level] == 0) && Game->map->aika > 0) {

			strcpy(Episode->scores.fastest_player[level],Episode->player_name);
			Episode->scores.best_time[level] = time;
			map_new_time_record = true;
			ret++;

		}

	} else {

		if (episteet > Episode->scores.episode_top_score) {

		    Episode->scores.episode_top_score = episteet;
			strcpy(Episode->scores.episode_top_player,Episode->player_name);
			episode_new_record = true;
			ret++;

		}

	}

	return ret;

}

int Draw_ScoreCount() {

	char luku[20];
	int x, y;

	PDraw::screen_fill(0);
	PDraw::image_clip(bg_screen, 0, 0);

	/* BG Effect */
	for (int i = 0; i < 18; i++) {


		int kerroin = (int)(cos_table[(degree+i*3)%180]);

		x = (int)(sin_table[(degree+i*10)%360]*2)+kerroin;
		y = (int)(cos_table[(degree+i*10)%360]*2);//10 | 360 | 2
		//PDraw::image_clip(game_assets,320+x,240+y,157,46,181,79);
		int kuutio = (int)(sin_table[(degree+i*3)%360]);
		if (kuutio < 0) kuutio = -kuutio;

		PDraw::screen_fill(320-x,240-y,320-x+kuutio,240-y+kuutio,COLOR_TURQUOISE+8);

	}
	for (int i = 0; i < 18; i++) {

		x = (int)(sin_table[(degree+i*10)%360]*3);
		y = (int)(cos_table[(degree+i*10)%360]*3);//10 | 360 | 3
		//PDraw::image_clip(game_assets,320+x,240+y,157,46,181,79);
		int kuutio = (int)(sin_table[(degree+i*2)%360])+18;
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
	
	ltoa(total_score,luku,10);
	ShadowedText_Draw(luku, 400, my);
	my += 70;

	ShadowedText_Draw(tekstit->Get_Text(PK_txt.score_screen_bonus_score), 100, my);
	
	ltoa(bonus_score,luku,10);
	ShadowedText_Draw(luku, 400, my);
	my += 30;

	ShadowedText_Draw(tekstit->Get_Text(PK_txt.score_screen_time_score), 100, my);
	
	ltoa(time_score,luku,10);
	ShadowedText_Draw(luku, 400, my);
	my += 30;

	ShadowedText_Draw(tekstit->Get_Text(PK_txt.score_screen_energy_score), 100, my);

	ltoa(energy_score,luku,10);
	ShadowedText_Draw(luku, 400, my);
	my += 30;

	ShadowedText_Draw(tekstit->Get_Text(PK_txt.score_screen_item_score), 100, my);
	
	ltoa(gifts_score,luku,10);
	ShadowedText_Draw(luku, 400, my);
	my += 40;

	x = 110;
	for (int i = 0; i < MAX_GIFTS; i++) {
		
		if (Gifts_Get(i) != -1)	{
			Gifts_Draw(i, x, my);
			x += 38;
		}

	}
	my += 20;

	// Draw apples
	if (Game->apples_count > 0) {

		uint i;
		for (i = 0; i < apples_got; i++) {
			if (apples_got >= Game->apples_count)
				PDraw::image_cutclip(game_assets2, 100 + i * 32 + rand()%2, my + rand()%2, 61, 379, 87, 406);
			else
				PDraw::image_cutclip(game_assets2, 100 + i * 32, my, 61, 379, 87, 406);
			
		}
		for (; i < Game->apples_count; i++)
			PDraw::image_cutcliptransparent(game_assets2, 61, 379, 26, 26, 100 + i * 32, my, 20, 0);

	}

	my += 40;

	if (counting_phase >= 6) {
		
		ShadowedText_Draw(tekstit->Get_Text(PK_txt.score_screen_total_score), 100, my);
		
		ltoa(Episode->player_score,luku,10);
		ShadowedText_Draw(luku, 400, my);
		my += 25;

		//if (apples_got >= Game->apples_count && apples_got > 0) {

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

	if (Draw_Menu_Text(true,tekstit->Get_Text(PK_txt.score_screen_continue),15,430)) {

		siirry_pistelaskusta_karttaan = true;
		PSound::set_musicvolume(0);
		PDraw::fade_out(PDraw::FADE_SLOW);

	}

	Draw_Cursor(PInput::mouse_x, PInput::mouse_y);

	return 0;
}


int Screen_ScoreCount_Init() {

	GUI_Change(UI_CURSOR);
	if (Settings.isWide)
		PDraw::set_xoffset(80);
	else
		PDraw::set_xoffset(0);
	PDraw::screen_fill(0);

	PDraw::image_delete(bg_screen);
	bg_screen = PDraw::image_load("gfx/menu.bmp", true);
	PK_MenuShadow_Create(bg_screen, 640, 480, 30);

	map_new_record = false;
	map_new_time_record = false;
	episode_new_record = false;

	// Lasketaan pelaajan kokonaispisteet etuk�teen
	u32 temp_score = 0;
	temp_score += Game->score;
	temp_score += Game->timeout * 5;
	temp_score += Player_Sprite->energia * 300;
	for (int i = 0; i < MAX_GIFTS; i++)
		if (Gifts_Get(i) != -1)
			temp_score += Gifts_GetProtot(i)->pisteet + 500;

	if (!Game->repeating)
		Episode->player_score += temp_score;

	apples_got = 0;
	total_score = 0;
	counting_phase = 0;
	counting_delay = 30;
	bonus_score = 0;
	time_score = 0;
	energy_score = 0;
	gifts_score = 0;

	char pisteet_tiedosto[PE_PATH_SIZE] = "scores.dat";

	/* Check if broken level score and time record */
	int compare_result = EpisodeScore_Compare(Game->level_id, temp_score, Game->map->aika - Game->timeout, false);
	if (compare_result > 0) {

		Episode->Save_Scores(pisteet_tiedosto);

	}

	/* Check if broken episode score */ //TODO
	compare_result = EpisodeScore_Compare(0, Episode->player_score, 0, true);
	if (compare_result > 0) {

		Episode->Save_Scores(pisteet_tiedosto);

	}

	PSound::set_musicvolume(Settings.music_max_volume);

	siirry_pistelaskusta_karttaan = false;

	PDraw::fade_in(PDraw::FADE_FAST);

	return 0;

}

int Screen_ScoreCount() {

	Draw_ScoreCount();

	degree = 1 + degree % 360;

	// PISTELASKU

	if (counting_delay == 0){
		if (bonus_score < Game->score) {

			counting_phase = 2;
			counting_delay = 0;
			bonus_score += 10;

			if (degree%7==1)
				Play_MenuSFX(score_sound, 70);

			if (bonus_score >= Game->score){
				bonus_score = Game->score;
				counting_delay = 50;
			}

		} else if (Game->timeout > 0) {

			counting_phase = 3;
			counting_delay = 0;
			time_score+=5;
			Game->timeout--;

			if (degree%10==1)
				Play_MenuSFX(score_sound, 70);

			if (Game->timeout == 0)
				counting_delay = 50;

		} else if (Player_Sprite->energia > 0) {

			counting_phase = 4;
			counting_delay = 10;
			energy_score+=300;
			Player_Sprite->energia--;

			Play_MenuSFX(score_sound, 70);

		} else if (Gifts_Count() > 0) {
			
			counting_phase = 5;
			counting_delay = 30;
			gifts_score += Gifts_GetProtot(0)->pisteet + 500;
			Gifts_Remove(0); 
			Play_MenuSFX(jump_sound, 100);

		} else if (Game->apples_got > 0) {

			counting_phase = 1;
			counting_delay = 10;

			apples_got++;
			Game->apples_got--;
			Play_MenuSFX(apple_sound, 70);

			if (Game->apples_got == 0)
				counting_delay = 20;
		
		} else {
			
			counting_phase = 6;
		
		}
	}

	if (counting_delay > 0)
		counting_delay--;

	if (key_delay == 0){
		if (PInput::Keydown(PInput::RETURN)) {

			if(counting_phase == 6) {

				siirry_pistelaskusta_karttaan = true;
				PDraw::fade_out(PDraw::FADE_SLOW);
				PSound::set_musicvolume(0);
				key_delay = 20;

			} else {

				counting_phase = 6;

				apples_got += Game->apples_got;
				Game->apples_got = 0;

				bonus_score = Game->score;
				time_score += Game->timeout * 5;
				Game->timeout = 0;
				energy_score += Player_Sprite->energia * 300;
				Player_Sprite->energia = 0;
				for (int i = 0; i < Gifts_Count(); i++)
					gifts_score += Gifts_GetProtot(i)->pisteet + 500;
				
				Gifts_Clean();

				key_delay = 20;

			}

		}
	}

	if (siirry_pistelaskusta_karttaan && !PDraw::is_fading()){

		if (Game->level_id == EPISODI_MAX_LEVELS - 1) {

			next_screen = SCREEN_END;
		
		} else if (Game->level_id == Episode->level_count - 1) {

			next_screen = SCREEN_END;
		
		} else {

			next_screen = SCREEN_MAP;
		
		}

		delete Game;
		Game = nullptr;
	}

	return 0;

}