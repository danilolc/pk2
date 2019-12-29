//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "screens/screens.hpp"

#include "PisteEngine.hpp"

#include <cstring>

#include "game/game.hpp"
#include "game/gifts.hpp"
#include "game/sprites.hpp"
#include "gfx/text.hpp"
#include "gfx/particles.hpp"
#include "gui.hpp"
#include "episode.hpp"
#include "language.hpp"
#include "sfx.hpp"
#include "system.hpp"

int debug_active_sprites = 0;

bool draw_dubug_info = false;
int debug_sprites = 0;
int debug_drawn_sprites = 0;

int palikka_animaatio = 0;

int PK_Draw_InGame_BGSprites() {

	double xl, yl, alku_x, alku_y, yk;
	int i;

	for (int in=0; in<MAX_SPRITEJA; in++) {
		PK2Sprite* sprite = &Sprites_List[bgSprites_List[in]];

		if (sprite->tyyppi != nullptr && i != -1) {
			if (!sprite->piilota && sprite->tyyppi->tyyppi == TYYPPI_TAUSTA) {
				//Tarkistetaanko onko sprite tai osa siit� kuvassa

				alku_x = sprite->alku_x;
				alku_y = sprite->alku_y;

				if (sprite->tyyppi->pallarx_kerroin != 0) {
					xl =  alku_x - Game->camera_x-screen_width/2 - sprite->tyyppi->leveys/2;
					xl /= sprite->tyyppi->pallarx_kerroin;
					yl =  alku_y - Game->camera_y-screen_height/2 - sprite->tyyppi->korkeus/2;
					yk = sprite->tyyppi->pallarx_kerroin;///1.5;
					if (yk != 0)
						yl /= yk;


				}
				else
					xl = yl = 0;

				switch(sprite->tyyppi->AI[0]) {
				case AI_TAUSTA_KUU					:	yl += screen_height/3+50; break;
				/*case AI_TAUSTA_LIIKKUU_VASEMMALLE	:	if (sprite->a == 0)
															sprite->a = rand()%3;
														sprite->alku_x -= sprite->a;
														if (sprite->piilossa && sprite->alku_x < Game->camera_x)
														{
													  		  sprite->alku_x = Game->camera_x+screen_width+sprite->tyyppi->leveys*2;
															  sprite->a = rand()%3;
														}
														break;*/
				case AI_LIIKKUU_X_COS:			sprite->AI_Liikkuu_X(cos_table[degree%360]);
												alku_x = sprite->x;
												alku_y = sprite->y;
												break;
				case AI_LIIKKUU_Y_COS:			sprite->AI_Liikkuu_Y(cos_table[degree%360]);
												alku_x = sprite->x;
												alku_y = sprite->y;
												break;
				case AI_LIIKKUU_X_SIN:			sprite->AI_Liikkuu_X(sin_table[degree%360]);
												alku_x = sprite->x;
												alku_y = sprite->y;
												break;
				case AI_LIIKKUU_Y_SIN:			sprite->AI_Liikkuu_Y(sin_table[degree%360]);
												alku_x = sprite->x;
												alku_y = sprite->y;
												break;
				default: break;
				}

				sprite->x = alku_x-xl;
				sprite->y = alku_y-yl;

				//Tarkistetaanko onko sprite tai osa siit� kuvassa
				if (sprite->x - sprite->tyyppi->leveys/2  < Game->camera_x+screen_width &&
					sprite->x + sprite->tyyppi->leveys/2  > Game->camera_x &&
					sprite->y - sprite->tyyppi->korkeus/2 < Game->camera_y+screen_height &&
					sprite->y + sprite->tyyppi->korkeus/2 > Game->camera_y)
				{
					sprite->Piirra(Game->camera_x,Game->camera_y);
					sprite->piilossa = false;

					debug_drawn_sprites++;
				} else {
					if (!Game->paused)
						sprite->Animoi();
					sprite->piilossa = true;
				}

				debug_sprites++;

			}
		}
	}
	return 0;
}
int PK_Draw_InGame_Sprites() {
	debug_sprites = 0;
	debug_drawn_sprites = 0;
	int stars, sx;
	double star_x, star_y;

	for (int i=0;i<MAX_SPRITEJA;i++){
		// Onko sprite n�kyv�
		PK2Sprite* sprite = &Sprites_List[i];
		if (!sprite->piilota && sprite->tyyppi->tyyppi != TYYPPI_TAUSTA){
			//Check whether or not sprite is on the screen
			if (sprite->x - sprite->tyyppi->leveys/2  < Game->camera_x+screen_width &&
				sprite->x + sprite->tyyppi->leveys/2  > Game->camera_x &&
				sprite->y - sprite->tyyppi->korkeus/2 < Game->camera_y+screen_height &&
				sprite->y + sprite->tyyppi->korkeus/2 > Game->camera_y){

				if (sprite->isku > 0 && sprite->tyyppi->tyyppi != TYYPPI_BONUS && sprite->energia < 1){
					int framex = ((degree%12)/3) * 58;
					DWORD hit_x = sprite->x-8, hit_y = sprite->y-8;
					PDraw::image_cutclip(game_assets,hit_x-Game->camera_x-28+8, hit_y-Game->camera_y-27+8,1+framex,83,1+57+framex,83+55);
				}

				if (Game->invisibility == 0 || (!doublespeed && Game->invisibility%2 == 0) || (doublespeed && Game->invisibility%4 <= 1) || sprite != Player_Sprite/*i != pelaaja_index*/)
					sprite->Piirra(Game->camera_x,Game->camera_y);

				if (sprite->energia < 1 && sprite->tyyppi->tyyppi != TYYPPI_AMMUS){
					sx = (int)sprite->x;
					for (stars=0; stars<3; stars++){
						star_x = sprite->x-8 + (sin_table[((stars*120+degree)*2)%359])/3;
						star_y = sprite->y-18 + (cos_table[((stars*120+degree)*2+sx)%359])/8;
						PDraw::image_cutclip(game_assets,star_x-Game->camera_x, star_y-Game->camera_y,1,1,11,11);
					}
				}

					debug_drawn_sprites++;
			} else{
				if (!Game->paused)
					sprite->Animoi();

				if (sprite->energia < 1)
					sprite->piilota = true;
			}

			debug_sprites++;
		}
	}
	return 0;
}

int PK_Draw_InGame_DebugInfo() {
	int vali, fy = 70;
	char lukua[20];

	if (Settings.isWide)
		PDraw::set_xoffset(80);
	else
		PDraw::set_xoffset(0);

	vali = PDraw::font_write(fontti1,"spriteja: ",10,fy);
	itoa(debug_sprites,lukua,10);
	PDraw::font_write(fontti1,lukua,10+vali,fy);
	fy += 10;

	vali = PDraw::font_write(fontti1,"aktiivisia: ",10,fy);
	itoa(debug_active_sprites,lukua,10);
	PDraw::font_write(fontti1,lukua,10+vali,fy);
	fy += 10;

	vali = PDraw::font_write(fontti1,"piirretty: ",10,fy);
	itoa(debug_drawn_sprites,lukua,10);
	PDraw::font_write(fontti1,lukua,10+vali,fy);
	fy += 10;

	for (int i=0;i<40;i++){
		itoa(Prototypes_List[i].indeksi,lukua,10);
		PDraw::font_write(fontti1,lukua,410,10+i*10);
		PDraw::font_write(fontti1,Prototypes_List[i].tiedosto,430,10+i*10);
		PDraw::font_write(fontti1,Prototypes_List[i].bonus_sprite,545,10+i*10);
	}

	for (int i=0;i<EPISODI_MAX_LEVELS;i++)
		if (strcmp(Episode::levels_list[i].nimi,"")!=0)
			PDraw::font_write(fontti1,Episode::levels_list[i].nimi,0,240+i*10);

	char dluku[50];

	sprintf(dluku, "%.7f", Player_Sprite->x); //Player x
	PDraw::font_write(fontti1, dluku, 10, 410);

	sprintf(dluku, "%.7f", Player_Sprite->y); //Player y
	PDraw::font_write(fontti1, dluku, 10, 420);

	sprintf(dluku, "%.7f", Player_Sprite->b); //Player v-speed
	PDraw::font_write(fontti1, dluku, 10, 430);

	sprintf(dluku, "%.7f", Player_Sprite->a); //Player h-speed
	PDraw::font_write(fontti1, dluku, 10, 440);

	PDraw::font_write(fontti1, Game->map_path, 10, 460);

	itoa(Player_Sprite->hyppy_ajastin, lukua, 10);
	PDraw::font_write(fontti1, lukua, 270, 460);

	char tpolku[PE_PATH_SIZE] = "";
	Episode::Get_Dir(tpolku);

	PDraw::font_write(fontti1,tpolku,10,470);

	itoa(Game->invisibility,lukua,10);
	PDraw::font_write(fontti1,lukua,610,470);

	itoa(Game->button1, lukua, 10);
	PDraw::font_write(fontti1, lukua, 610, 460);
	itoa(Game->button2, lukua, 10);
	PDraw::font_write(fontti1, lukua, 610, 450);
	itoa(Game->button3, lukua, 10);
	PDraw::font_write(fontti1, lukua, 610, 440);

	PDraw::set_xoffset(0);
	return 0;
}
int PK_Draw_InGame_DevKeys() {

	const char txt0[] = "dev mode";
	int char_w = PDraw::font_write(fontti1, txt0, 0, screen_height - 10) / strlen(txt0);
	int char_h = 10;

	const char help[] = "h: help";

	if (!PisteInput_Keydown(PI_H)) {
		PDraw::font_write(fontti1, help, screen_width - strlen(help) * char_w, screen_height - 10);
		return 0;
	}
	const char txts[][32] = {
		"z: press buttons",
		"x: release buttons",
		"b: draw bounding box",
		"l: open locks",
		"k: open skull blocks",
		"t: toggle speed",
		"g: toggle transparency",
		"w: toggle window mode",
		"i: toggle debug info",
		"u: go up",
		"r: back to start",
		"v: set invisible",
		"e: set energy to max",
		"end: end level",
		"mouse: move camera",
		"lshift: set rooster"
	};

	int nof_txt = sizeof(txts) / 32;

	int max_size = 0;
	
	for (int i = 0; i < nof_txt; i++)
		if (strlen(txts[i]) > max_size) max_size = strlen(txts[i]);

	int posx = screen_width - max_size * char_w;
	int posy = screen_height - char_h * nof_txt;

	PDraw::screen_fill(posx - 4, posy - 4, screen_width, screen_height, 0);
	PDraw::screen_fill(posx - 2, posy - 2, screen_width, screen_height, 38);
	
	for (int i = 0; i < nof_txt; i++)
		PDraw::font_write(fontti1, txts[i], posx, posy + i*10);

	return 0;
}
int PK_Draw_InGame_BG() {

	int pallarx = ( Game->camera_x % (640*3) ) / 3;
	int pallary = ( Game->camera_y % (480*3) ) / 3;

	PDraw::screen_fill(34);//0

	if (Game->map->tausta == TAUSTA_STAATTINEN){
		PDraw::image_clip(Game->map->taustakuva_buffer,0,0);
		PDraw::image_clip(Game->map->taustakuva_buffer,640,0);
	}

	if (Game->map->tausta == TAUSTA_PALLARX_HORI){
		PDraw::image_clip(Game->map->taustakuva_buffer,0   - pallarx,0);
		PDraw::image_clip(Game->map->taustakuva_buffer,640 - pallarx,0);

		if (screen_width > 640)
			PDraw::image_clip(Game->map->taustakuva_buffer,640*2 - pallarx,0);
	}

	if (Game->map->tausta == TAUSTA_PALLARX_VERT){
		PDraw::image_clip(Game->map->taustakuva_buffer,0,0   - pallary);
		PDraw::image_clip(Game->map->taustakuva_buffer,0,480 - pallary);

		if (screen_width > 640){
			PDraw::image_clip(Game->map->taustakuva_buffer,640,0   - pallary);
			PDraw::image_clip(Game->map->taustakuva_buffer,640,480 - pallary);
		}
	}

	if (Game->map->tausta == TAUSTA_PALLARX_VERT_JA_HORI){
		PDraw::image_clip(Game->map->taustakuva_buffer,0   - pallarx, 0-pallary);
		PDraw::image_clip(Game->map->taustakuva_buffer,640 - pallarx, 0-pallary);
		PDraw::image_clip(Game->map->taustakuva_buffer,0   - pallarx, 480-pallary);
		PDraw::image_clip(Game->map->taustakuva_buffer,640 - pallarx, 480-pallary);

		if (screen_width > 640){
			PDraw::image_clip(Game->map->taustakuva_buffer,640*2 - pallarx,0-pallary);
			PDraw::image_clip(Game->map->taustakuva_buffer,640*2 - pallarx,480-pallary);
		}
	}

	return 0;
}

int PK_Draw_InGame_Gifts() {

	int x,y;

	y = screen_height-35;//36
	x = Game->item_pannel_x + 35;//40

	for (int i=0;i<MAX_GIFTS;i++)
		if (Gifts_Get(i) != -1){
			Gifts_Draw(i, x, y);
			x += 38;
		}

	return 0;
}
int PK_Draw_InGame_Lower_Menu() {

	char luku[15];
	int vali = 0;

	int x, y;

	//////////////
	// Draw time
	//////////////
	if (Game->timeout > 0){
		Game->timeout += Game->increase_time;
		float shown_sec = (float)(Game->timeout * TIME_FPS + Game->seconds) / 60;
		int min = (int)shown_sec/60,
			sek = (int)shown_sec%60;

		x = screen_width / 2 - 546 / 2 + 342;
		y = screen_height-39;
		PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.game_time),x,y-20);

		itoa(min,luku,10);
		PDraw::font_write(        fontti4,luku,x+1,y+1);
		vali += PDraw::font_write(fontti2,luku,x,y);
		vali += PDraw::font_write(fontti1,":",x+vali,y+9);

		if (Game->increase_time > 0) {
			itoa((int)(Game->increase_time * TIME_FPS) / 60, luku, 10);
			Fadetext_New(fontti2, luku, x + vali, y, 49, true);
			Game->increase_time = 0;
		}

		if (sek < 10){
			PDraw::font_write(        fontti4,"0",x+vali+1,y+1);
			vali += PDraw::font_write(fontti2,"0",x+vali,y);
		}
		itoa(sek,luku,10);

		PDraw::font_write(        fontti4,luku,x+vali+1,y+1);
		vali += PDraw::font_write(fontti2,luku,x+vali,y);
	}

	/////////////////
	// Draw keys
	/////////////////
	if (Game->keys > 0){
		x = screen_width / 2 - 546 / 2 + 483;
		y = screen_height-39;
		PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.game_keys),x,y-20);

		itoa(Game->keys,luku,10);
		PDraw::font_write(fontti4,luku,x+1,y+1);
		PDraw::font_write(fontti2,luku,x,y);
	}

	/////////////////
	// Draw Gifts
	/////////////////
	if (Gifts_Count() > 0 && Game->item_pannel_x < 10)
		Game->item_pannel_x++;

	if (Gifts_Count() == 0 && Game->item_pannel_x > -215)
		Game->item_pannel_x--;

	if (Game->item_pannel_x > -215)
		PDraw::image_cutclip(game_assets,Game->item_pannel_x,screen_height-60,
		                        1,216,211,266);
	if (Game->item_pannel_x > 5)
		PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.game_items),15,screen_height-65);

	PK_Draw_InGame_Gifts();

	return 0;
}

int PK_Draw_InGame_UI(){
	char luku[15];
	int vali = 20;
	int my = 8;

	/////////////////
	// Draw Energy
	/////////////////
	vali = PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.game_energy),40,my);
	ltoa(Player_Sprite->energia,luku,10);
	PDraw::font_write(fontti4,luku,40+vali+1,my+1);
	PDraw::font_write(fontti2,luku,40+vali,my);

	/////////////////
	// Draw Invisible
	/////////////////
	if(Game->invisibility > 0){
		vali = PDraw::font_write(fontti1,"invisible:",40,my+27);
		ltoa(Game->invisibility/60,luku,10);
		PDraw::font_write(fontti2,luku,40+vali+1,my+27+1);
		PDraw::font_write(fontti2,luku,40+vali,my+27);
	}

	/////////////////
	// Draw Score
	/////////////////
	vali = PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.game_score),230,my);
	ltoa(Game->score,luku,10);
	PDraw::font_write(fontti4,luku,230+vali+1,my+1);
	PDraw::font_write(fontti2,luku,230+vali,my);

	/////////////////
	// Draw Ammunition
	/////////////////
	if (Player_Sprite->ammus2 != -1){
		PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.game_attack1), screen_width-170,my);
		Prototypes_List[Player_Sprite->ammus2].Piirra(screen_width-170,my+10,0);
	}

	if (Player_Sprite->ammus1 != -1){
		PDraw::font_write(fontti1,tekstit->Hae_Teksti(PK_txt.game_attack2), screen_width-90,my+15);
		Prototypes_List[Player_Sprite->ammus1].Piirra(screen_width-90,my+25,0);
	}

	/////////////////
	// Draw Info
	/////////////////
	if (Game->info_timer > 0){
		int ilm_pituus = strlen(Game->info) * 8 + 8; // 300

		RECT alue = {screen_width/2-(ilm_pituus/2),60,screen_width/2+(ilm_pituus/2),60+20};

		if (Game->info_timer < 20){
			alue.top	+= (20 - Game->info_timer) / 2;
			alue.bottom -= (20 - Game->info_timer) / 2;
		}

		if (Game->info_timer > INFO_TIME - 20){
			alue.top	+= 10 - (INFO_TIME - Game->info_timer) / 2;
			alue.bottom -= 10 - (INFO_TIME - Game->info_timer) / 2;
		}

		PDraw::screen_fill(alue.left-1,alue.top-1,alue.right+1,alue.bottom+1,51);
		PDraw::screen_fill(alue.left,alue.top,alue.right,alue.bottom,38);

		if (Game->info_timer >= 100)
			PDraw::font_write(fontti1,Game->info,alue.left+4,alue.top+4);
		else
			PDraw::font_writealpha(fontti1,Game->info,alue.left+4,alue.top+4,Game->info_timer);
	}

	return 0;
}

int PK_Draw_InGame(){
	char luku[15];
	int vali = 20;

	if (!skip_frame){

		PK_Draw_InGame_BG();

		if (Settings.tausta_spritet)
			PK_Draw_InGame_BGSprites();

		Particles_DrawBG(Game->camera_x, Game->camera_y);

		Game->map->Piirra_Taustat(Game->camera_x,Game->camera_y,false);

		PK_Draw_InGame_Sprites();

		//PK_Particles_Draw();
		Particles_DrawFront(Game->camera_x, Game->camera_y);

		Game->map->Piirra_Seinat(Game->camera_x,Game->camera_y, false);

		if (Settings.nayta_tavarat)
			PK_Draw_InGame_Lower_Menu();

		Fadetext_Draw();

		PK_Draw_InGame_UI();

		if (draw_dubug_info)
			PK_Draw_InGame_DebugInfo();
		else {
			if (dev_mode)
				PK_Draw_InGame_DevKeys();
			if (test_level)
				PDraw::font_write(fontti1, "testing level", 0, 480 - 20);
			if (show_fps) {
				if (fps >= 100)
					vali = PDraw::font_write(fontti1, "fps:", 570, 48);
				else
					vali = PDraw::font_write(fontti1, "fps: ", 570, 48);
				fps = Piste::get_fps();
				itoa((int)fps, luku, 10);
				PDraw::font_write(fontti1, luku, 570 + vali, 48);
			}
		}

		if (Game->paused)
			PDraw::font_write(fontti2,tekstit->Hae_Teksti(PK_txt.game_paused),screen_width/2-82,screen_height/2-9);

		if (Game->level_clear)
			Wavetext_Draw(tekstit->Hae_Teksti(PK_txt.game_clear),fontti2,screen_width/2-120,screen_height/2-9);
		else
			if (Game->game_over){
				if (Player_Sprite->energia < 1)
					Wavetext_Draw(tekstit->Hae_Teksti(PK_txt.game_ko),fontti2,screen_width/2-90,screen_height/2-9-10);
				else
					if (Game->timeout < 1 && Game->has_time)
						Wavetext_Draw(tekstit->Hae_Teksti(PK_txt.game_timeout),fontti2,screen_width/2-67,screen_height/2-9-10);

				Wavetext_Draw(tekstit->Hae_Teksti(PK_txt.game_tryagain),fontti2,screen_width/2-75,screen_height/2-9+10);
			}
	}
	
	if (skip_frame) Piste::ignore_frame();

	if (doublespeed) skip_frame = !skip_frame;
	else skip_frame = false;

	palikka_animaatio = 1 + palikka_animaatio % 34;

	return 0;
}



int Screen_InGame_Init(){

	GUI_Change(UI_GAME_BUTTONS);
	PDraw::set_xoffset(0);

	if (!Game->isStarted()) {
		Game->Start();
		degree = 0;
	} else {
		degree = degree_temp;
	}
	
	return 0;
}

int Update_Camera(){

	Game->camera_x = (int)Player_Sprite->x-screen_width / 2;
	Game->camera_y = (int)Player_Sprite->y-screen_height / 2;
	
	if(dev_mode && PisteInput_Hiiri_Vasen()) {
		Game->camera_x += (mouse_x - screen_width / 2);
		Game->camera_y += (mouse_y - screen_height / 2);
	}

	if (Game->vibration > 0) {
		Game->dcamera_x += (rand()%Game->vibration-rand()%Game->vibration)/5;
		Game->dcamera_y += (rand()%Game->vibration-rand()%Game->vibration)/5;

		Game->vibration--;
	}

	if (Game->button_moving > 0) {
		Game->dcamera_x += (rand()%9-rand()%9);//3
		Game->dcamera_y += (rand()%9-rand()%9);

		Game->button_moving--;
	}

	if (Game->dcamera_x != Game->camera_x)
		Game->dcamera_a = (Game->camera_x - Game->dcamera_x) / 15;

	if (Game->dcamera_y != Game->camera_y)
		Game->dcamera_b = (Game->camera_y - Game->dcamera_y) / 15;

	if (Game->dcamera_a > 6)
		Game->dcamera_a = 6;

	if (Game->dcamera_a < -6)
		Game->dcamera_a = -6;

	if (Game->dcamera_b > 6)
		Game->dcamera_b = 6;

	if (Game->dcamera_b < -6)
		Game->dcamera_b = -6;

	Game->dcamera_x += Game->dcamera_a;
	Game->dcamera_y += Game->dcamera_b;

	Game->camera_x = (int)Game->dcamera_x;
	Game->camera_y = (int)Game->dcamera_y;

	if (Game->camera_x < 0)
		Game->camera_x = 0;

	if (Game->camera_y < 0)
		Game->camera_y = 0;

	if (Game->camera_x > int(PK2KARTTA_KARTTA_LEVEYS-screen_width/32)*32)
		Game->camera_x = int(PK2KARTTA_KARTTA_LEVEYS-screen_width/32)*32;

	if (Game->camera_y > int(PK2KARTTA_KARTTA_KORKEUS-screen_height/32)*32)
		Game->camera_y = int(PK2KARTTA_KARTTA_KORKEUS-screen_height/32)*32;

	return 0;
}

int Screen_InGame(){

	PK2Kartta_Animoi(degree, palikka_animaatio/7, Game->button1, Game->button2, Game->button3, false);
	Update_Camera();

	if (!Game->paused) {
		Particles_Update();
		if (!Game->level_clear && (!Game->has_time || Game->timeout > 0))
			Update_Sprites();
		Fadetext_Update();
	}

	PK_Draw_InGame();

	Game->Move_Blocks();

	if (!Game->paused){
		degree = 1 + degree % 360;//359;

		if (Game->button1 > 0)
			Game->button1 --;

		if (Game->button2 > 0)
			Game->button2 --;

		if (Game->button3 > 0)
			Game->button3 --;

		if (Game->info_timer > 0)
			Game->info_timer--;

		if (Game->score_increment > 0){
			Game->score++;
			Game->score_increment--;
		}

		if (Game->has_time && !Game->level_clear){
			if (Game->seconds > 0)
				Game->seconds --;
			else{
				Game->seconds = TIME_FPS;
				if (Game->timeout > 0)
					Game->timeout--;
				else
					Game->game_over = true;
			}
		}

		if (Game->invisibility > 0)
			Game->invisibility--;
	}

	if (Player_Sprite->energia < 1 && !Game->game_over){
		Game->game_over = true;
		key_delay = 50; //TODO - reduce
	}

	if (Game->level_clear || Game->game_over){
		if (Game->exit_timer > 1)
			Game->exit_timer--;

		if (Game->exit_timer == 0)
			Game->exit_timer = 500;//800;//2000;

		if (PisteInput_Keydown(Settings.control_attack1) || PisteInput_Keydown(Settings.control_attack2) ||
			PisteInput_Keydown(Settings.control_jump) || PisteInput_Keydown(PI_RETURN))
			if (Game->exit_timer > 2 && Game->exit_timer < 400/*600*//*1900*/ && key_delay == 0)
				Game->exit_timer = 2;

		if (Game->exit_timer == 2) {
			PDraw::fade_out(PDraw::FADE_NORMAL);
			//PSound::set_musicvolume(0);
		
		}
	}

	if (key_delay == 0){
		if (!Game->game_over && !Game->level_clear) {
			if (PisteInput_Keydown(Settings.control_open_gift)) {
				Gifts_Use();
				key_delay = 10;
			}
			
			if (PisteInput_Keydown(PI_P)) {
				Game->paused = !Game->paused;
				key_delay = 20;
			}
			
			if (PisteInput_Keydown(PI_DELETE)) {
				Player_Sprite->energia = 0;
			}

			if (PisteInput_Keydown(PI_TAB)){
				Gifts_ChangeOrder();
				key_delay = 10;
			}

			if (PisteInput_Keydown(PI_ESCAPE)) {
				if(test_level)
					Fade_Quit();
				else {
					next_screen = SCREEN_MENU;
					degree_temp = degree;
				}
				key_delay = 20;
			}
		}

		if (!dev_mode)
			if (PisteInput_Keydown(PI_I)) {
				show_fps = !show_fps;
				key_delay = 20;
			}
	}

	if (dev_mode){ //Debug
		if (key_delay == 0) {
			if (PisteInput_Keydown(PI_F)) {
				show_fps = !show_fps;
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_Z)) {
				if (Game->button1 < KYTKIN_ALOITUSARVO - 64) Game->button1 = KYTKIN_ALOITUSARVO;
				if (Game->button2 < KYTKIN_ALOITUSARVO - 64) Game->button2 = KYTKIN_ALOITUSARVO;
				if (Game->button3 < KYTKIN_ALOITUSARVO - 64) Game->button3 = KYTKIN_ALOITUSARVO;
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_X)) {
				if (Game->button1 > 64) Game->button1 = 64;
				if (Game->button2 > 64) Game->button2 = 64;
				if (Game->button3 > 64) Game->button3 = 64;
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_T)) {
				doublespeed = !doublespeed;
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_G)) {
				Settings.lapinakyvat_objektit = !Settings.lapinakyvat_objektit;
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_L)) {
				Game->keys = 0;
				Game->map->Open_Locks();
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_K)) {
				Game->map->Change_SkullBlocks();
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_W)) {
				Settings.isFullScreen = !Settings.isFullScreen;
				PDraw::set_fullscreen(Settings.isFullScreen);
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_I)) {
				draw_dubug_info = !draw_dubug_info;
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_R)) {
				Game->map->Select_Start();
				Player_Sprite->energia = 10;
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_END)) {
				key_delay = 20;
				if (PSound::start_music("music/hiscore.xm") != 0){ //TODO - put it on a function
					PK2_Error("Can't find hiscore.xm");
				}
				Game->level_clear = true;
				Episode::levels_list[Game->level_id].lapaisty = true;
				if (Episode::levels_list[Game->level_id].jarjestys == Episode::level)
					Episode::level++;
				PSound::set_musicvolume_now(Settings.music_max_volume);
			}
			if (PisteInput_Keydown(PI_LSHIFT)/* && key_delay == 0*/) {
				//key_delay = 20;
				for (int r = 1; r<6; r++)
					//Particles_New(PARTICLE_SPARK, player->x + rand() % 10 - rand() % 10, player->y + rand() % 10 - rand() % 10, 0, 0, rand() % 100, 0.1, 32);
					Particles_New(PARTICLE_SPARK, Player_Sprite->x + rand() % 10 - rand() % 10, Player_Sprite->y + rand() % 10 - rand() % 10, 0, 0, rand() % 100, 0.1, 32);
				*Player_Sprite = PK2Sprite(&Prototypes_List[PROTOTYYPPI_KANA], 1, false, Player_Sprite->x, Player_Sprite->y);
			}
		}
		if (PisteInput_Keydown(PI_U))
			Player_Sprite->b = -10;
		if (PisteInput_Keydown(PI_E))
			Player_Sprite->energia = Player_Sprite->tyyppi->energia;
		if (PisteInput_Keydown(PI_V))
			Game->invisibility = 3000;
	}

	if (Game->exit_timer == 1 && !PDraw::is_fading()) {
		//Game->started = false;
		
		if (Game->level_clear) next_screen = SCREEN_SCORING;
		else {
			
			delete Game;
			Game = nullptr;

			if(test_level) Fade_Quit();
			else next_screen = SCREEN_MAP;
		
		}

	}

	return 0;
}