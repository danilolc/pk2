//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "screens/screens.hpp"

#include "engine/Piste.hpp"

#include <cstring>

#include "game/game.hpp"
#include "game/gifts.hpp"
#include "game/sprites.hpp"
#include "gfx/text.hpp"
#include "gfx/particles.hpp"
#include "gfx/effect.hpp"
#include "gui.hpp"
#include "episode/episodeclass.hpp"
#include "language.hpp"
#include "sfx.hpp"
#include "system.hpp"
#include "settings.hpp"

static bool draw_debug_info = false;

static int debug_drawn_sprites = 0;
static int debug_active_sprites = 0;

bool Is_Sprite_Visible(SpriteClass* sprite) {

	return (sprite->x - sprite->tyyppi->kuva_frame_leveys/2  < Game->camera_x + screen_width &&
			sprite->x + sprite->tyyppi->kuva_frame_leveys/2  > Game->camera_x &&
			sprite->y - sprite->tyyppi->kuva_frame_korkeus/2 < Game->camera_y + screen_height &&
			sprite->y + sprite->tyyppi->kuva_frame_korkeus/2 > Game->camera_y);
	
}

int Draw_InGame_BGSprites() {

	for (SpriteClass* sprite : bgSprites_List) {

		if (sprite->piilota)
			continue;

		double alku_x = sprite->alku_x;
		double alku_y = sprite->alku_y;

		double xl, yl, yk;

		if (sprite->tyyppi->pallarx_kerroin != 0) {
			
			xl =  alku_x - Game->camera_x-screen_width/2 - sprite->tyyppi->leveys/2;
			xl /= sprite->tyyppi->pallarx_kerroin;
			yl =  alku_y - Game->camera_y-screen_height/2 - sprite->tyyppi->korkeus/2;
			yk = sprite->tyyppi->pallarx_kerroin;///1.5;
			if (yk != 0)
				yl /= yk;

		}
		else {

			xl = yl = 0;

		}

		switch(sprite->tyyppi->AI[0]) {
		case AI_TAUSTA_KUU					:	yl += screen_height/3+50; break;
		/*case AI_TAUSTA_LIIKKUU_LEFT	:	if (sprite->a == 0)
													sprite->a = rand()%3;
												sprite->alku_x -= sprite->a;
												if (sprite->piilossa && sprite->alku_x < Game->camera_x)
												{
														sprite->alku_x = Game->camera_x+screen_width+sprite->tyyppi->leveys*2;
														sprite->a = rand()%3;
												}
												break;*/
		case AI_LIIKKUU_X_COS:			sprite->AI_Liikkuu_X(cos_table(degree));
										alku_x = sprite->x;
										alku_y = sprite->y;
										break;
		case AI_LIIKKUU_Y_COS:			sprite->AI_Liikkuu_Y(cos_table(degree));
										alku_x = sprite->x;
										alku_y = sprite->y;
										break;
		case AI_LIIKKUU_X_SIN:			sprite->AI_Liikkuu_X(sin_table(degree));
										alku_x = sprite->x;
										alku_y = sprite->y;
										break;
		case AI_LIIKKUU_Y_SIN:			sprite->AI_Liikkuu_Y(sin_table(degree));
										alku_x = sprite->x;
										alku_y = sprite->y;
										break;
		default: break;
		}

		sprite->x = alku_x-xl;
		sprite->y = alku_y-yl;
		//Check whether the sprite is on the screen
		if (Is_Sprite_Visible(sprite)) {
			sprite->Piirra(Game->camera_x,Game->camera_y);

			if (!Game->paused)
				sprite->HandleEffects();

			sprite->piilossa = false;
			debug_drawn_sprites++;
		} else {
			if (!Game->paused)
				sprite->Animoi();
			sprite->piilossa = true;
		}
	}
	return 0;
}

void Draw_InGame_Sprites() {

	for (SpriteClass* sprite : Sprites_List) {

		if (sprite->tyyppi->type == TYPE_BACKGROUND)
			continue;
		
		if (sprite->piilota)
			continue;

		if (Is_Sprite_Visible(sprite)) {

			// Draw impact circle
			if (sprite->damage_timer > 0 && sprite->tyyppi->type != TYPE_BONUS && sprite->energia < 1){
				int framex = ((degree%12)/3) * 58;
				u32 hit_x = sprite->x-8;
				u32 hit_y = sprite->y-8;
				PDraw::image_cutclip(game_assets,hit_x-Game->camera_x-28+8, hit_y-Game->camera_y-27+8,1+framex,83,1+57+framex,83+55);
			}

			if (!(sprite->pelaaja && dev_mode && PInput::Keydown(PInput::Y) && degree % 2 == 0))
				sprite->Piirra(Game->camera_x,Game->camera_y);

			// Draw stars on dead sprite
			if (sprite->energia < 1 && sprite->tyyppi->type != TYPE_PROJECTILE){
				int sx = (int)sprite->x;
				for (int stars=0; stars<3; stars++){
					double star_x = sprite->x - 8  + sin_table((stars*120+degree)*2)      / 3;
					double star_y = sprite->y - 18 + cos_table((stars*120+degree)*2 + sx) / 8;
					PDraw::image_cutclip(game_assets,star_x-Game->camera_x, star_y-Game->camera_y,1,1,11,11);
				}
			}

			if (!Game->paused)
				sprite->HandleEffects();

			debug_drawn_sprites++;

		} else {

			if (!Game->paused)
				sprite->Animoi();

			// Delete death body
			if (sprite->energia < 1)
				sprite->piilota = true;
			
		}
	}
}

int Draw_InGame_DebugInfo() {
	int vali, fy = 70;
	char lukua[20];

	PDraw::set_offset(640, 480);

	vali = PDraw::font_write(fontti1,"spriteja: ",10,fy);
	sprintf(lukua, "%li", Sprites_List.size());
	PDraw::font_write(fontti1,lukua,10+vali,fy);
	fy += 10;

	vali = PDraw::font_write(fontti1,"aktiivisia: ",10,fy);
	sprintf(lukua, "%i", debug_active_sprites);
	PDraw::font_write(fontti1,lukua,10+vali,fy);
	fy += 10;

	vali = PDraw::font_write(fontti1,"piirretty: ",10,fy);
	sprintf(lukua, "%i", debug_drawn_sprites);
	PDraw::font_write(fontti1,lukua,10+vali,fy);
	fy += 10;

	for (int i = 0; i < 40; i++) {
		sprintf(lukua, "%i", i);
		PDraw::font_write(fontti1,lukua,410,10+i*10);
		if (Prototypes_List[i] == nullptr) {
			PDraw::font_write(fontti1,"-",430,10+i*10);
		} else {
			PDraw::font_write(fontti1,Prototypes_List[i]->tiedosto,430,10+i*10);

			if (degree < 90)
				PDraw::font_write(fontti1,Prototypes_List[i]->muutos_sprite,545,10+i*10);
			else if (degree < 180)
				PDraw::font_write(fontti1,Prototypes_List[i]->bonus_sprite,545,10+i*10);
			else if (degree < 270)
				PDraw::font_write(fontti1,Prototypes_List[i]->ammus1_sprite,545,10+i*10);
			else
				PDraw::font_write(fontti1,Prototypes_List[i]->ammus2_sprite,545,10+i*10);
		}
	}

	for (uint i = 0; i < Episode->level_count; i++)
		if (strcmp(Episode->levels_list[i].nimi,"")!=0)
			PDraw::font_write(fontti1,Episode->levels_list[i].nimi,0,240+i*10);

	char dluku[50];

	sprintf(dluku, "%.7f", Player_Sprite->x); //Player x
	PDraw::font_write(fontti1, dluku, 10, 410);

	sprintf(dluku, "%.7f", Player_Sprite->y); //Player y
	PDraw::font_write(fontti1, dluku, 10, 420);

	sprintf(dluku, "%.7f", Player_Sprite->b); //Player v-speed
	PDraw::font_write(fontti1, dluku, 10, 430);

	sprintf(dluku, "%.7f", Player_Sprite->a); //Player h-speed
	PDraw::font_write(fontti1, dluku, 10, 440);

	PDraw::font_write(fontti1, Game->map_file.c_str(), 10, 460);

	sprintf(lukua, "%i", Player_Sprite->jump_timer);
	PDraw::font_write(fontti1, lukua, 270, 460);

	PDraw::font_write(fontti1, Episode->Get_Dir("").c_str(), 10, 470);

	sprintf(lukua, "%i", Player_Sprite->super_mode_timer);
	PDraw::font_write(fontti1, lukua, 610, 470);
	sprintf(lukua, "%i", Player_Sprite->invisible_timer);
	PDraw::font_write(fontti1, lukua, 610, 460);
	sprintf(lukua, "%i", Game->button1);
	PDraw::font_write(fontti1, lukua, 610, 450);
	sprintf(lukua, "%i", Game->button2);
	PDraw::font_write(fontti1, lukua, 610, 440);
	sprintf(lukua, "%i", Game->button3);
	PDraw::font_write(fontti1, lukua, 610, 430);

	sprintf(lukua, "%i", Game->timeout);
	vali += PDraw::font_write(fontti1,lukua,390,screen_height-10);

	PDraw::set_offset(screen_width, screen_height);
	return 0;
}
int Draw_InGame_DevKeys() {

	const char txt0[] = "dev mode";
	int char_w = PDraw::font_write(fontti1, txt0, 0, screen_height - 10) / strlen(txt0);
	int char_h = 10;

	const char help[] = "h: help";

	if (!PInput::Keydown(PInput::H)) {
		PDraw::font_write(fontti1, help, screen_width - strlen(help) * char_w, screen_height - 10);
		return 0;
	}
	const char txts[][32] = {
		"z: press buttons",
		"x: release buttons",
		"l: open locks",
		"k: open skull blocks",
		"t: toggle speed",
		"g: toggle transparency",
		"w: toggle window mode",
		"i: toggle debug info",
		"u: go up",
		"y: ghost mode",
		"r: back to start",
		"v: set invisible",
		"s: set super mode",
		"e: set energy to max",
		"a: set rooster",
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
		PDraw::font_write(fontti1, txts[i], posx, posy + i*10);

	return 0;
}
int Draw_InGame_BG() {

	int pallarx = ( Game->camera_x % (640*3) ) / 3;
	int pallary = ( Game->camera_y % (480*3) ) / 3;

	if (Game->map.tausta == BACKGROUND_STATIC){
	
		PDraw::image_clip(Game->map.background_buffer,0,0);
		PDraw::image_clip(Game->map.background_buffer,640,0);
	
	} else if (Game->map.tausta == BACKGROUND_PARALLAX_HORI){
	
		PDraw::image_clip(Game->map.background_buffer,0   - pallarx,0);
		PDraw::image_clip(Game->map.background_buffer,640 - pallarx,0);

		if (screen_width > 640)
			PDraw::image_clip(Game->map.background_buffer,640*2 - pallarx,0);
	
	} else if (Game->map.tausta == BACKGROUND_PARALLAX_VERT){
	
		PDraw::image_clip(Game->map.background_buffer,0,0   - pallary);
		PDraw::image_clip(Game->map.background_buffer,0,480 - pallary);

		if (screen_width > 640){
			PDraw::image_clip(Game->map.background_buffer,640,0   - pallary);
			PDraw::image_clip(Game->map.background_buffer,640,480 - pallary);
		}
	
	} else if (Game->map.tausta == BACKGROUND_PARALLAX_VERT_AND_HORI){
	
		PDraw::image_clip(Game->map.background_buffer,0   - pallarx, 0-pallary);
		PDraw::image_clip(Game->map.background_buffer,640 - pallarx, 0-pallary);
		PDraw::image_clip(Game->map.background_buffer,0   - pallarx, 480-pallary);
		PDraw::image_clip(Game->map.background_buffer,640 - pallarx, 480-pallary);

		if (screen_width > 640){
			PDraw::image_clip(Game->map.background_buffer,640*2 - pallarx,0-pallary);
			PDraw::image_clip(Game->map.background_buffer,640*2 - pallarx,480-pallary);
		}
	
	}

	return 0;

}

int Draw_InGame_Gifts() {

	int x,y;

	y = screen_height-35;//36
	x = Game->item_pannel_x + 35;//40

	for (int i=0;i<MAX_GIFTS;i++)
		if (Gifts_Get(i) != nullptr){
			Gifts_Draw(i, x, y);
			x += 38;
		}

	return 0;
}
int Draw_InGame_Lower_Menu() {

	char luku[16];
	int vali = 0;

	int x, y;

	//////////////
	// Draw time
	//////////////
	if (Game->has_time > 0) {

		float shown_time = float(Game->timeout) / 60;
		int min = int(shown_time/60);
		int sek = int(shown_time)%60;

		x = screen_width / 2 + 69;
		y = screen_height-39;
		PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.game_time),x,y-20);

		sprintf(luku, "%i", min);
		
		vali += ShadowedText_Draw(luku, x, y);
		vali += PDraw::font_write(fontti1,":",x+vali,y+9);

		if (sek < 10)
			vali += ShadowedText_Draw("0", x + vali, y);
		
		sprintf(luku, "%i", sek);
		vali += ShadowedText_Draw(luku, x + vali, y);

	}

	/////////////////
	// Draw keys
	/////////////////
	if (Game->keys > 0){
		x = screen_width / 2 + 210;
		y = screen_height - 39;
		PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.game_keys),x,y-20);

		sprintf(luku, "%i", Game->keys);
		ShadowedText_Draw(luku, x, y);
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
		PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.game_items),15,screen_height-65);

	Draw_InGame_Gifts();

	return 0;
}

int Draw_InGame_UI(){
	char luku[16];
	int vali = 20;
	int my = 14;

	/////////////////
	// Draw Energy
	/////////////////
	vali = PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.game_energy),60,my);
	sprintf(luku, "%i", Player_Sprite->energia);	
	ShadowedText_Draw(luku, 60 + vali, my);

	/////////////////
	// Draw Invisible
	/////////////////
	if(Player_Sprite->invisible_timer > 0){
		vali = PDraw::font_write(fontti1,"invisible:",60,my+27);
		sprintf(luku, "%i", Player_Sprite->invisible_timer/60);	
		PDraw::font_write(fontti2,luku,60+vali+1,my+27+1);
		PDraw::font_write(fontti2,luku,60+vali,my+27);
	}

	/////////////////
	// Draw Mini Apple
	/////////////////
	if (Game->apples_count > 0) {
		if (Game->apples_got == Game->apples_count)
			PDraw::image_cutcliptransparent(game_assets2, 
				45, 379, 13, 15, 
				my, my, 
				sin_table(degree)*1.5+60, COLOR_RED);
		else
			PDraw::image_cutcliptransparent(game_assets2, 
				45, 379, 13, 15, 
				my, my, 
				0, COLOR_GRAY);
	}

	/////////////////
	// Draw Score
	/////////////////
	vali = PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.game_score),230,my);
	sprintf(luku, "%i", Game->score);		
	ShadowedText_Draw(luku, 230 + vali, my);

	/////////////////
	// Draw Ammunition
	/////////////////
	if (Player_Sprite->ammus2 != nullptr){
		PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.game_attack1), screen_width-170,my);
		Player_Sprite->ammus2->Piirra(screen_width-170,my+10,0);
	}

	if (Player_Sprite->ammus1 != nullptr){
		PDraw::font_write(fontti1,tekstit->Get_Text(PK_txt.game_attack2), screen_width-90,my+15);
		Player_Sprite->ammus1->Piirra(screen_width-90,my+25,0);
	}

	/////////////////
	// Draw Info
	/////////////////
	if (Game->info_timer > 0){
		int box_size = Game->info_text.size() * 8 + 8; // 300

		MAP_RECT alue = {screen_width/2-(box_size/2),60,screen_width/2+(box_size/2),60+20};

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

		if (Game->info_timer-11 >= 100)
			PDraw::font_write(fontti1,Game->info_text.c_str(),alue.left+4,alue.top+4);
		else
			PDraw::font_writealpha(fontti1,Game->info_text.c_str(),alue.left+4,alue.top+4,Game->info_timer-11);
	}

	return 0;
}

int Draw_InGame() {

	char luku[16];

	debug_drawn_sprites = 0;

	Draw_InGame_BG();
	
	if (Settings.bg_sprites)
		Draw_InGame_BGSprites();

	Particles_DrawBG(Game->camera_x, Game->camera_y);

	Game->map.Piirra_Taustat(Game->camera_x,Game->camera_y);

	Draw_InGame_Sprites();

	Particles_DrawFront(Game->camera_x, Game->camera_y);

	Game->map.Piirra_Seinat(Game->camera_x,Game->camera_y);

	if (Settings.draw_itembar)
		Draw_InGame_Lower_Menu();

	Fadetext_Draw();

	Draw_InGame_UI();

	if (draw_debug_info)
		Draw_InGame_DebugInfo();
	else {
		if (dev_mode)
			Draw_InGame_DevKeys();
		if (test_level)
			PDraw::font_write(fontti1, "testing level", 0, screen_height - 20);
		if (show_fps) {
			
			int fps = Piste::get_fps();
			int txt_size;

			if (fps >= 100)
				txt_size = PDraw::font_write(fontti1, "fps:", 570, 48);
			else
				txt_size = PDraw::font_write(fontti1, "fps: ", 570, 48);
			
			sprintf(luku, "%i", fps);
			PDraw::font_write(fontti1, luku, 570 + txt_size, 48);
		
		}
		if (speedrun_mode) {
			sprintf(luku, "%li", long(Game->frame_count));
			PDraw::font_write(fontti1, luku, 570, 38);
		}
	}

	if (Game->paused) {

		PDraw::font_write(fontti2,tekstit->Get_Text(PK_txt.game_paused),screen_width/2-82,screen_height/2-9);

	}

	if (Game->level_clear) {

		Wavetext_Draw(tekstit->Get_Text(PK_txt.game_clear),fontti2,screen_width/2-120,screen_height/2-9);

	} else if (Game->game_over) {
	
		if (Player_Sprite->energia < 1) {
			
			Wavetext_Draw(tekstit->Get_Text(PK_txt.game_ko),fontti2,screen_width/2-90,screen_height/2-9-10);

		} else if (Game->timeout < 1 && Game->has_time) {

			Wavetext_Draw(tekstit->Get_Text(PK_txt.game_timeout),fontti2,screen_width/2-67,screen_height/2-9-10);
		
		}

		Wavetext_Draw(tekstit->Get_Text(PK_txt.game_tryagain),fontti2,screen_width/2-75,screen_height/2-9+10);
	
	}

	return 0;
}



int Screen_InGame_Init(){

	if(PUtils::Is_Mobile()) {
		if (Settings.gui)
			GUI_Change(UI_GAME_BUTTONS);
		else 
			GUI_Change(UI_TOUCH_TO_START);
	}
	
	PDraw::set_offset(screen_width, screen_height);

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
	
	if(dev_mode && PInput::MouseLeft() && !PUtils::Is_Mobile()) {
		Game->camera_x += PInput::mouse_x - screen_width / 2;
		Game->camera_y += PInput::mouse_y - screen_height / 2;
	}

	if (Game->vibration > 0) {
		Game->dcamera_x += (rand()%Game->vibration-rand()%Game->vibration)/5;
		Game->dcamera_y += (rand()%Game->vibration-rand()%Game->vibration)/5;

		Game->vibration--;
	}

	if (Game->button_vibration > 0) {
		Game->dcamera_x += (rand()%9-rand()%9);//3
		Game->dcamera_y += (rand()%9-rand()%9);

		Game->button_vibration--;
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

	if (Game->camera_x > int(PK2MAP_MAP_WIDTH-screen_width/32)*32)
		Game->camera_x = int(PK2MAP_MAP_WIDTH-screen_width/32)*32;

	if (Game->camera_y > int(PK2MAP_MAP_HEIGHT-screen_height/32)*32)
		Game->camera_y = int(PK2MAP_MAP_HEIGHT-screen_height/32)*32;

	return 0;
}

int Screen_InGame(){

	if (!Game->level_clear && (!Game->has_time || Game->timeout > 0)) {
		Game->map.Animoi(degree, Game->palikka_animaatio/7, Game->button1, Game->button2, Game->button3);
		Game->palikka_animaatio = 1 + Game->palikka_animaatio % 34;
	}

	Update_Camera();
	Update_GameSFX();

	if (!Game->paused) {

		Particles_Update();

		if (!Game->level_clear && (!Game->has_time || Game->timeout > 0)) {
			debug_active_sprites = Update_Sprites();
			Game->frame_count++;
		}
		Fadetext_Update();

	}

	static bool skip_frame = false;

	if (Settings.double_speed) skip_frame = !skip_frame;
	else skip_frame = false;

	if (!skip_frame) {

		Draw_InGame();

	} else {

		Piste::ignore_frame();

	}
	
	Game->Move_Blocks();

	if (!Game->paused) {

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

		if (Game->has_time && !Game->level_clear) {
			if (Game->timeout > 0)
				Game->timeout--;
			else
				Game->game_over = true;
			
		}
	}

	if (Player_Sprite->energia < 1 && !Game->game_over) {

		Game->game_over = true;
		key_delay = 50; //TODO - reduce
	
	}

	if (Game->level_clear || Game->game_over) {

		if (Game->exit_timer > 1)
			Game->exit_timer--;

		if (Game->exit_timer == 0)
			Game->exit_timer = 700;//800;//2000;

		if (PInput::Keydown(Input->attack1) || PInput::Keydown(Input->attack2) ||
			PInput::Keydown(Input->jump) || Clicked() ||
			Gui_egg || Gui_doodle || Gui_gift || Gui_up || Gui_down || Gui_menu)
			if (Game->exit_timer > 2 && Game->exit_timer < 500/*600*//*1900*/ && key_delay == 0)
				Game->exit_timer = 2;

		if (Game->exit_timer == 2) {
			
			Fade_out(FADE_NORMAL);
			if (Game->game_over)
				PSound::set_musicvolume(0);
		
		}
	}

	if (key_delay == 0) {
		if (!Game->game_over && !Game->level_clear) {
			if (PInput::Keydown(Input->open_gift) || Gui_gift) {
				Gifts_Use();
				key_delay = 10;
			}
			
			if (PInput::Keydown(PInput::P)) {
				Game->paused = !Game->paused;
				key_delay = 20;
			}
			
			if (PInput::Keydown(PInput::DEL)) {
				Player_Sprite->energia = 0;
			}

			if (PInput::Keydown(PInput::TAB) || PInput::Keydown(PInput::JOY_GUIDE) || Gui_tab){
				Gifts_ChangeOrder();
				key_delay = 10;
			}

			if (!skip_frame) {
				if (PInput::Keydown(PInput::ESCAPE) || PInput::Keydown(PInput::JOY_START) || Gui_menu || Gui_touch) {
					if(test_level)
						Fade_Quit();
					else {
						next_screen = SCREEN_MENU;
						degree_temp = degree;
					}
					key_delay = 20;
				}
			}
		}

		if (!dev_mode)
			if (PInput::Keydown(PInput::I)) {
				show_fps = !show_fps;
				key_delay = 20;
			}
	}

	if (dev_mode){ //Debug
		if (key_delay == 0) {
			if (PInput::Keydown(PInput::F)) {
				show_fps = !show_fps;
				key_delay = 20;
			}
			if (PInput::Keydown(PInput::Z)) {
				if (Game->button1 < Game->map.button1_time - 64) Game->button1 = Game->map.button1_time;
				if (Game->button2 < Game->map.button2_time - 64) Game->button2 = Game->map.button2_time;
				if (Game->button3 < Game->map.button3_time - 64) Game->button3 = Game->map.button3_time;
				key_delay = 20;
			}
			if (PInput::Keydown(PInput::X)) {
				if (Game->button1 > 64) Game->button1 = 64;
				if (Game->button2 > 64) Game->button2 = 64;
				if (Game->button3 > 64) Game->button3 = 64;
				key_delay = 20;
			}
			if (PInput::Keydown(PInput::T)) {
				Settings.double_speed = !Settings.double_speed;
				key_delay = 20;
			}
			if (PInput::Keydown(PInput::G)) {
				Settings.draw_transparent = !Settings.draw_transparent;
				key_delay = 20;
			}
			if (PInput::Keydown(PInput::L)) {
				Game->keys = 0;
				Game->Open_Locks();
				key_delay = 20;
			}
			if (PInput::Keydown(PInput::K)) {
				Game->Change_SkullBlocks();
				key_delay = 20;
			}
			if (PInput::Keydown(PInput::W)) {
				Settings.isFullScreen = !Settings.isFullScreen;
				PRender::set_fullscreen(Settings.isFullScreen);
				key_delay = 20;
			}
			if (PInput::Keydown(PInput::I)) {
				draw_debug_info = !draw_debug_info;
				key_delay = 20;
			}
			if (PInput::Keydown(PInput::R)) {
				Game->Select_Start();
				Player_Sprite->energia = 10;
				Player_Sprite->piilota = false;
				key_delay = 20;
			}
			if (PInput::Keydown(PInput::END)) {
				key_delay = 20;
				Game->Finnish();
			}
			if (PInput::Keydown(PInput::A)/* && key_delay == 0*/) {
				//key_delay = 20;
				*Player_Sprite = SpriteClass(Prototypes_List[0], 1, Player_Sprite->x, Player_Sprite->y);
				Effect_Stars(Player_Sprite->x, Player_Sprite->y, COLOR_VIOLET);
			}
		}
		if (PInput::Keydown(PInput::U))
			Player_Sprite->b = -10;
		if (PInput::Keydown(PInput::E)) {
			Player_Sprite->energia = 10;
			Game->game_over = false;
		} if (PInput::Keydown(PInput::V))
			Player_Sprite->invisible_timer = 3000;
		if (PInput::Keydown(PInput::S)) {
			PSound::play_overlay_music();
			Player_Sprite->super_mode_timer = 490;
			key_delay = 30;
		}

	}

	if (Game->exit_timer == 1 && !Is_Fading()) {
		
		if(test_level) {

			Piste::stop();

		} else if (Game->level_clear) {

			next_screen = SCREEN_SCORING;

		} else {
			
			delete Game;
			Game = nullptr;

			next_screen = SCREEN_MAP;
		
		}

	}

	// TODO - FIX
	if (next_screen == SCREEN_MENU) {

		int w, h;
		PDraw::image_getsize(bg_screen, w, h);
		if (w != screen_width) {
			PDraw::image_delete(bg_screen);
			bg_screen = PDraw::image_new(screen_width, screen_height);
		}
		PDraw::image_snapshot(bg_screen);

	}

	return 0;
}