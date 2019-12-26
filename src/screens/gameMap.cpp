#include "screens.hpp"

#include "PisteDraw.hpp"
#include "PisteInput.hpp"
#include "PisteSound.hpp"

#include <cstring>

#include "game.hpp"
#include "text.hpp"
#include "language.hpp"
#include "gui.hpp"
#include "episode.hpp"
#include "sfx.hpp"
#include "save.hpp"

bool going_to_game = false;

int PK_Draw_Map_Button(int x, int y, int t){
	int paluu = 0;

	t = t * 25;

	int vilkku = 50 + (int)(sin_table[degree%360]*2);

	if (vilkku < 0)
		vilkku = 0;

	if (hiiri_x > x && hiiri_x < x+17 && hiiri_y > y && hiiri_y < y+17){
		if (key_delay == 0 && (PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE)
													    || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1))){
			key_delay = 30;
			return 2;
		}

		if (t == 25)
			PDraw::image_cutcliptransparent(kuva_peli, 247, 1, 25, 25, x-2, y-2, 60, 96);
		if (t == 0)
			PDraw::image_cutcliptransparent(kuva_peli, 247, 1, 25, 25, x-4, y-4, 60, 32);
		if (t == 50)
			PDraw::image_cutcliptransparent(kuva_peli, 247, 1, 25, 25, x-4, y-4, 60, 64);

		paluu = 1;
	}

	if (t == 25)
		PDraw::image_cutcliptransparent(kuva_peli, 247, 1, 25, 25, x-2, y-2, vilkku, 96);

	if (((degree/45)+1)%4==0 || t==0)
		PDraw::image_cutclip(kuva_peli,x,y,1+t,58,23+t,80);

	return paluu;
}

int PK_Draw_Map(){
	char luku[20];
	int vali = 20;

	PDraw::screen_fill(0);
	PDraw::image_clip(kuva_tausta, 0, 0);

	PDraw::font_write(fontti4,episodi,100+2,72+2);
	PDraw::font_write(fontti2,episodi,100,72);

	PDraw::font_write(fontti4,tekstit->Hae_Teksti(PK_txt.map_total_score),100+2,92+2);
	vali = PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.map_total_score),100,92);//250,80
	ltoa(pisteet,luku,10);
	PDraw::font_write(fontti4,luku,100+vali+2+15,92+2);
	PDraw::font_write(fontti2,luku,100+vali+15,92);

	if (episodipisteet.episode_top_score > 0) {
		vali = PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.map_episode_best_player),360,72);
		PDraw::font_write(fontti1,episodipisteet.episode_top_player,360+vali+10,72);
		vali = PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.map_episode_hiscore),360,92);
		ltoa(episodipisteet.episode_top_score,luku,10);
		PDraw::font_write(fontti2,luku,360+vali+15,92);
	}

	vali = PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.map_next_level),100,120);
	ltoa(jakso,luku,10);
	PDraw::font_write(fontti1,luku,100+vali+15,120);

	//PK_Particles_Draw();

	if (jaksoja == 0) {
		PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.episodes_no_maps),180,290);
	}

	if (Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),100,430)){
		next_screen = SCREEN_MENU;
		//menu_nyt = MENU_MAIN;
	}

	int nuppi_x = 0, nuppi_y = 0;
	int tyyppi = 0;
	int paluu;
	int min = 0, sek = 0;
	int ikoni;
	int sinx = 0, cosy = 0;
	int pekkaframe = 0;

	int njakso = jaksoja;
	for (int i=1;i<=jaksoja;i++)
		if (!jaksot[i].lapaisty && jaksot[i].jarjestys < njakso)
			njakso = jaksot[i].jarjestys; // Find the first unclear level
	if(jakso < njakso)
		jakso = njakso;

	for (int i=0;i<=jaksoja;i++) {
		if (strcmp(jaksot[i].nimi,"")!=0 && jaksot[i].jarjestys > 0) {
			tyyppi = 0;							//0 harmaa
			if (jaksot[i].jarjestys == jakso)
				tyyppi = 1;						//1 vihre�
			if (jaksot[i].jarjestys > jakso)
				tyyppi = 2;						//2 oranssi
			if (jaksot[i].lapaisty)
				tyyppi = 0;

			if (jaksot[i].x == 0)
				jaksot[i].x = 142+i*30;

			if (jaksot[i].y == 0)
				jaksot[i].y = 270;

			ikoni = jaksot[i].ikoni;

			//PDraw::image_clip(kuva_peli,jaksot[i].x-4,jaksot[i].y-4-30,1+(ikoni*27),452,27+(ikoni*27),478);
			PDraw::image_cutclip(kuva_peli,jaksot[i].x-9,jaksot[i].y-14,1+(ikoni*28),452,28+(ikoni*28),479);

			if (tyyppi==1) {
				sinx = (int)(sin_table[degree%360]/2);
				cosy = (int)(cos_table[degree%360]/2);
				pekkaframe = 28*((degree%360)/120);
				PDraw::image_cutclip(kuva_peli,jaksot[i].x+sinx-12,jaksot[i].y-17+cosy,157+pekkaframe,46,181+pekkaframe,79);
			}

			paluu = PK_Draw_Map_Button(jaksot[i].x-5, jaksot[i].y-10, tyyppi);

			// if clicked
			if (paluu == 2) {
				if (tyyppi != 2 || dev_mode) {
					strcpy(current_map_name,jaksot[i].tiedosto);
					jakso_indeksi_nyt = i;
					going_to_game = true;
					PDraw::fade_out(PDraw::FADE_SLOW);
					music_volume = 0;
					Play_MenuSFX(kieku_aani,90);
				}
				else
					Play_MenuSFX(ammuu_aani,100);
			}

			itoa(jaksot[i].jarjestys,luku,10);
			PDraw::font_write(fontti1,luku,jaksot[i].x-12+2,jaksot[i].y-29+2);

			if (paluu > 0) {

				int info_x = 489+3, info_y = 341-26;

				PDraw::image_cutclip(kuva_peli,info_x-3,info_y+26,473,0,607,121);
				PDraw::font_write(fontti1,jaksot[i].nimi,info_x,info_y+30);

				if (episodipisteet.best_score[i] > 0) {
					PDraw::font_writealpha(fontti1,tekstit->Hae_Teksti(PK_txt.map_level_best_player),info_x,info_y+50,75);
					PDraw::font_write(fontti1,episodipisteet.top_player[i],info_x,info_y+62);
					vali = 8 + PDraw::font_writealpha(fontti1,tekstit->Hae_Teksti(PK_txt.map_level_hiscore),info_x,info_y+74,75);
					ltoa(episodipisteet.best_score[i],luku,10);
					PDraw::font_write(fontti1,luku,info_x+vali,info_y+75);
				}

				if (episodipisteet.best_time[i] > 0) {
					PDraw::font_writealpha(fontti1,tekstit->Hae_Teksti(PK_txt.map_level_fastest_player),info_x,info_y+98,75);
					PDraw::font_write(fontti1,episodipisteet.fastest_player[i],info_x,info_y+110);

					vali = 8 + PDraw::font_writealpha(fontti1,tekstit->Hae_Teksti(PK_txt.map_level_best_time),info_x,info_y+122,75);
					min = episodipisteet.best_time[i]/60;
					sek = episodipisteet.best_time[i]%60;

					itoa(min,luku,10);
					vali += PDraw::font_write(fontti1,luku,info_x+vali,info_y+122);
					vali += PDraw::font_write(fontti1,":",info_x+vali,info_y+122);
					itoa(sek,luku,10);
					PDraw::font_write(fontti1,luku,info_x+vali,info_y+122);
				}
			}
		}
	}

	PK_Draw_Cursor(hiiri_x, hiiri_y);

	return 0;
}


int Screen_Map_Init() {
	GUI_Change(UI_CURSOR);
	if (Settings.isWide)
		PDraw::set_xoffset(80);
	else
		PDraw::set_xoffset(0);
	PDraw::screen_fill(0);

	if (!episode_started)
	{
		if (true) //lataa_peli != -1)
		{
			Load_Maps();

			//for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
			//	jaksot[j].lapaisty = tallennukset[lataa_peli].jakso_lapaisty[j];

			//lataa_peli = -1;
			episode_started = true;
			peli_ohi = true;
			jakso_lapaisty = true;
			lopetusajastin = 0;
		}
		else
		{
			PK_Start_Saves(); // jos ladataan peli, asetetaan l�p�istyarvot jaksoille aikaisemmin
			Load_Maps();
		}
		char topscoretiedosto[PE_PATH_SIZE] = "scores.dat";
		EpisodeScore_Open(topscoretiedosto);
	}

	/* Ladataan kartan taustakuva ...*/
	char mapkuva[PE_PATH_SIZE] = "map.bmp";
	Load_EpisodeDir(mapkuva);
	//PisteLog_Kirjoita("  - Loading map picture ");
	//PisteLog_Kirjoita(mapkuva);
	//PisteLog_Kirjoita(" from episode folder \n");

	PDraw::image_delete(kuva_tausta);
	kuva_tausta = PDraw::image_load(mapkuva, true);
	if (kuva_tausta == -1)
		kuva_tausta = PDraw::image_load("gfx/map.bmp", true);

	/* Ladataan kartan musiikki ...*/
	char mapmusa[PE_PATH_SIZE] = "map.mp3";
	do
	{
		Load_EpisodeDir(mapmusa);
		if (PK_Check_File(mapmusa))
			break;
		strcpy(mapmusa, "map.ogg");
		Load_EpisodeDir(mapmusa);
		if (PK_Check_File(mapmusa))
			break;
		strcpy(mapmusa, "map.xm");
		Load_EpisodeDir(mapmusa);
		if (PK_Check_File(mapmusa))
			break;
		strcpy(mapmusa, "map.mod");
		Load_EpisodeDir(mapmusa);
		if (PK_Check_File(mapmusa))
			break;
		strcpy(mapmusa, "map.it");
		Load_EpisodeDir(mapmusa);
		if (PK_Check_File(mapmusa))
			break;
		strcpy(mapmusa, "map.s3m");
		Load_EpisodeDir(mapmusa);
		if (PK_Check_File(mapmusa))
			break;
		strcpy(mapmusa, "music/map.mp3");
		if (PK_Check_File(mapmusa))
			break;
		strcpy(mapmusa, "music/map.ogg");
		if (PK_Check_File(mapmusa))
			break;
		strcpy(mapmusa, "music/map.xm");
		break;
	} while (0);

	PSound::start_music(mapmusa);

	music_volume = Settings.music_max_volume;

	going_to_game = false;

	PDraw::fade_in(PDraw::FADE_SLOW);
}

int Screen_Map(){
	PK_Draw_Map();

	degree = 1 + degree % 360;

	if (going_to_game && !PDraw::is_fading()) {
		next_screen = SCREEN_GAME;
		
		episode_started = false;

		//Draw "loading" text
		PDraw::set_xoffset(0);
		PDraw::screen_fill(0);
		PDraw::font_write(fontti2, tekstit->Hae_Teksti(PK_txt.game_loading), screen_width / 2 - 82, screen_height / 2 - 9);
		PDraw::fade_out(0);
	}

	if (key_delay > 0)
		key_delay--;

	return 0;
}