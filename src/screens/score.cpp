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
#include "episode.hpp"
#include "sfx.hpp"
#include "game/sprites.hpp"
#include "system.hpp"

#include "PisteDraw.hpp"
#include "PisteInput.hpp"
#include "PisteSound.hpp"

#include <cstring>

bool siirry_pistelaskusta_karttaan = false;

int pistelaskuvaihe = 0;
int pistelaskudelay = 0;

DWORD fake_pisteet = 0;
DWORD bonuspisteet = 0;
DWORD aikapisteet = 0;
DWORD energiapisteet = 0;
DWORD esinepisteet = 0;
DWORD pelastuspisteet = 0;

bool map_new_record = false;
bool map_new_time_record = false;
bool episode_new_record = false;

int EpisodeScore_Compare(int jakso, DWORD episteet, DWORD aika, bool final_score){
	
	int ret = 0;

	if (!final_score) {
		if (episteet > Episode::scores.best_score[jakso]) {
			strcpy(Episode::scores.top_player[jakso],Episode::player_name);
			Episode::scores.best_score[jakso] = episteet;
			map_new_record = true;
			ret++;
		}
		if ((aika < Episode::scores.best_time[jakso] || Episode::scores.best_time[jakso] == 0) && Game::map->aika > 0) {
			strcpy(Episode::scores.fastest_player[jakso],Episode::player_name);
			Episode::scores.best_time[jakso] = aika;
			map_new_time_record = true;
			ret++;
		}
	}
	else {
		if (episteet > Episode::scores.episode_top_score) {
		    Episode::scores.episode_top_score = episteet;
			strcpy(Episode::scores.episode_top_player,Episode::player_name);
			episode_new_record = true;
			ret++;
		}
	}

	return ret;

}

int PK_Draw_ScoreCount(){
	char luku[20];
	int vali = 20;
	int my = 0, x, y;
	int kuutio, aste;
	int	vari = 0, kerroin;

	PDraw::screen_fill(0);
	PDraw::image_clip(bg_screen, 0, 0);

	for (aste=0;aste<18;aste++) {

		kerroin = (int)(cos_table[(degree+aste*3)%180]);

		x = (int)(sin_table[(degree+aste*10)%360]*2)+kerroin;
		y = (int)(cos_table[(degree+aste*10)%360]*2);//10 | 360 | 2
		//PDraw::image_clip(game_assets,320+x,240+y,157,46,181,79);
		kuutio = (int)(sin_table[(degree+aste*3)%360]);
		if (kuutio < 0) kuutio = -kuutio;

		PDraw::screen_fill(320-x,240-y,320-x+kuutio,240-y+kuutio,VARI_TURKOOSI+8);
	}
	for (aste=0;aste<18;aste++) {

		x = (int)(sin_table[(degree+aste*10)%360]*3);
		y = (int)(cos_table[(degree+aste*10)%360]*3);//10 | 360 | 3
		//PDraw::image_clip(game_assets,320+x,240+y,157,46,181,79);
		kuutio = (int)(sin_table[(degree+aste*2)%360])+18;
		if (kuutio < 0) kuutio = -kuutio;//0;//
		if (kuutio > 100) kuutio = 100;

		//PDraw::screen_fill(320+x,240+y,320+x+kuutio,240+y+kuutio,VARI_TURKOOSI+10);
		PDraw::image_cutcliptransparent(game_assets, 247, 1, 25, 25, 320+x, 240+y, kuutio, 32);
	}

	PDraw::font_write(fontti4,tekstit->Hae_Teksti(PK_txt.score_screen_title),100+2,72+2);
	PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.score_screen_title),100,72);
	PDraw::font_write(fontti4,tekstit->Hae_Teksti(PK_txt.score_screen_level_score),100+2,102+2);
	vali = PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.score_screen_level_score),100,102);//250,80
	fake_pisteet = bonuspisteet + aikapisteet + energiapisteet + esinepisteet + pelastuspisteet;
	ltoa(fake_pisteet,luku,10);
	PDraw::font_write(fontti4,luku,400+2,102+2);
	PDraw::font_write(fontti2,luku,400,102);
	my = 0;

	PDraw::font_write(fontti4,tekstit->Hae_Teksti(PK_txt.score_screen_bonus_score),100+2,192+2+my);
	vali = PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.score_screen_bonus_score),100,192+my);
	ltoa(bonuspisteet,luku,10);
	PDraw::font_write(fontti4,luku,400+2,192+2+my);
	PDraw::font_write(fontti2,luku,400,192+my);
	my += 30;

	PDraw::font_write(fontti4,tekstit->Hae_Teksti(PK_txt.score_screen_time_score),100+2,192+2+my);
	vali = PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.score_screen_time_score),100,192+my);
	ltoa(aikapisteet,luku,10);
	PDraw::font_write(fontti4,luku,400+2,192+2+my);
	PDraw::font_write(fontti2,luku,400,192+my);
	my += 30;

	PDraw::font_write(fontti4,tekstit->Hae_Teksti(PK_txt.score_screen_energy_score),100+2,192+2+my);
	vali = PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.score_screen_energy_score),100,192+my);
	ltoa(energiapisteet,luku,10);
	PDraw::font_write(fontti4,luku,400+2,192+2+my);
	PDraw::font_write(fontti2,luku,400,192+my);
	my += 30;

	PDraw::font_write(fontti4,tekstit->Hae_Teksti(PK_txt.score_screen_item_score),100+2,192+2+my);
	vali = PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.score_screen_item_score),100,192+my);
	ltoa(esinepisteet,luku,10);
	PDraw::font_write(fontti4,luku,400+2,192+2+my);
	PDraw::font_write(fontti2,luku,400,192+my);
	my += 30;

	x = 110;
	y = 192 + my;

	for (int i = 0; i < MAX_GIFTS; i++)
		if (Gifts_Get(i) != -1)	{
			Gifts_Draw(i, x, y);
			x += 38;
		}

	my += 10;

	if (pistelaskuvaihe >= 4){
		PDraw::font_write(fontti4,tekstit->Hae_Teksti(PK_txt.score_screen_total_score),100+2,192+2+my);
		vali = PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.score_screen_total_score),100,192+my);//250,80
		ltoa(Episode::player_score,luku,10);
		PDraw::font_write(fontti4,luku,400+2,192+2+my);
		PDraw::font_write(fontti2,luku,400,192+my);
		my += 25;

		if (map_new_record) {
			PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.score_screen_new_level_hiscore),100+rand()%2,192+my+rand()%2);
			my += 25;
		}
		if (map_new_time_record) {
			PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.score_screen_new_level_best_time),100+rand()%2,192+my+rand()%2);
			my += 25;
		}
		if (episode_new_record) {
			PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.score_screen_new_episode_hiscore),100+rand()%2,192+my+rand()%2);
			my += 25;
		}
	}

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.score_screen_continue),100,430)){
		PSound::set_musicvolume(0);
		siirry_pistelaskusta_karttaan = true;
		PDraw::fade_out(PDraw::FADE_SLOW);
		//next_screen = SCREEN_MAP;
	}

	PK_Draw_Cursor(mouse_x, mouse_y);

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
	DWORD temp_pisteet = 0;
	temp_pisteet += Game::score;
	temp_pisteet += Game::timeout * 5;
	temp_pisteet += Player_Sprite->energia * 300;
	for (int i = 0; i < MAX_GIFTS; i++)
		if (Gifts_Get(i) != -1)
			temp_pisteet += Gifts_GetProtot(i)->pisteet + 500;

	//if (Episode::levels_list[Game::level_id].lapaisty)
	//if (Episode::levels_list[Game::level_id].jarjestys == jakso-1)
	if (!Game::repeating)
		Episode::player_score += temp_pisteet;

	fake_pisteet = 0;
	pistelaskuvaihe = 0;
	pistelaskudelay = 30;
	bonuspisteet = 0,
	aikapisteet = 0,
	energiapisteet = 0,
	esinepisteet = 0,
	pelastuspisteet = 0;

	char pisteet_tiedosto[PE_PATH_SIZE] = "scores.dat";
	int vertailun_tulos;

	/* Tutkitaan onko pelaajarikkonut kent�n piste- tai nopeusenn�tyksen */
	vertailun_tulos = EpisodeScore_Compare(Game::level_id, temp_pisteet, Game::map->aika - Game::timeout, false);
	if (vertailun_tulos > 0) {
		Episode::Save_Scores(pisteet_tiedosto);
	}

	/* Tutkitaan onko pelaaja rikkonut episodin piste-enn�tyksen */
	vertailun_tulos = EpisodeScore_Compare(0, Episode::player_score, 0, true);
	if (vertailun_tulos > 0)
		Episode::Save_Scores(pisteet_tiedosto);

	PSound::set_musicvolume(Settings.music_max_volume);

	siirry_pistelaskusta_karttaan = false;

	PDraw::fade_in(PDraw::FADE_FAST);
}

int Screen_ScoreCount(){
	PK_Draw_ScoreCount();

	degree = 1 + degree % 360;

	// PISTELASKU

	int energia = Player_Sprite->energia;

	if (pistelaskudelay == 0){
		if (bonuspisteet < Game::score){
			pistelaskuvaihe = 1;
			pistelaskudelay = 0;
			bonuspisteet += 10;

			if (degree%7==1)
				Play_MenuSFX(score_sound, 70);

			if (bonuspisteet >= Game::score){
				bonuspisteet = Game::score;
				pistelaskudelay = 50;
			}

		} else if (Game::timeout > 0){
			pistelaskuvaihe = 2;
			pistelaskudelay = 0;
			aikapisteet+=5;
			Game::timeout--;

			if (degree%10==1)
				Play_MenuSFX(score_sound, 70);

			if (Game::timeout == 0)
				pistelaskudelay = 50;

		} else if (Player_Sprite->energia > 0){
			pistelaskuvaihe = 3;
			pistelaskudelay = 10;
			energiapisteet+=300;
			Player_Sprite->energia--;

			Play_MenuSFX(score_sound, 70);

		} else if (Gifts_Count() > 0){
			pistelaskuvaihe = 4;
			pistelaskudelay = 30;
			esinepisteet += Gifts_GetProtot(0)->pisteet + 500;
			Gifts_Remove(0); 
			Play_MenuSFX(jump_sound, 100);
		}
		else pistelaskuvaihe = 5;
	}

	if (pistelaskudelay > 0)
		pistelaskudelay--;

	if (siirry_pistelaskusta_karttaan && !PDraw::is_fading()){
		/*tarkistetaan oliko viimeinen jakso*/

		if (Game::level_id == EPISODI_MAX_LEVELS-1) { // ihan niin kuin joku tekisi n�in monta jaksoa...
			next_screen = SCREEN_END;
			//Episode::started = false;
		}
		else if (Episode::levels_list[Game::level_id+1].jarjestys == -1) {
			next_screen = SCREEN_END;
			//Episode::started = false;
		}
		else { // jos ei niin palataan karttaan...
			next_screen = SCREEN_MAP;
		}
	}

	if (key_delay == 0){
		if (PisteInput_Keydown(PI_RETURN) && pistelaskuvaihe == 5){
			siirry_pistelaskusta_karttaan = true;
			PDraw::fade_out(PDraw::FADE_SLOW);
			PSound::set_musicvolume(0);
			key_delay = 20;
		}

		if (PisteInput_Keydown(PI_RETURN) && pistelaskuvaihe < 5){
			pistelaskuvaihe = 5;
			bonuspisteet = Game::score;
			aikapisteet += Game::timeout * 5;
			Game::timeout = 0;
			energiapisteet += Player_Sprite->energia * 300;
			Player_Sprite->energia = 0;
			for (int i = 0; i < Gifts_Count(); i++)
				esinepisteet += Gifts_GetProtot(i)->pisteet + 500;
			
			Gifts_Clean(); //TODO esineita = 0;

			key_delay = 20;
		}
	}

	return 0;
}