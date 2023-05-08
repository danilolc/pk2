//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "game/game.hpp"

#include "game/gifts.hpp"
#include "system.hpp"
#include "gfx/text.hpp"
#include "gfx/particles.hpp"
#include "gfx/effect.hpp"
#include "episode/episodeclass.hpp"
#include "settings.hpp"
#include "gui.hpp"
#include "language.hpp"

#include "engine/PLog.hpp"
#include "engine/PSound.hpp"
#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"

#include <cstring>

GameClass* Game = nullptr;

GameClass::GameClass(int idx) {

	this->level_id = idx;
	this->map_file = Episode->levels_list[idx].tiedosto;

	if (Episode->level_status[idx] & LEVEL_PASSED)
		this->repeating = true;

}

GameClass::GameClass(std::string map_file) {

	this->map_file = map_file;

	bool found = false;
	for (uint i = 0; i < Episode->level_count; i++) {
		if (map_file == Episode->levels_list[i].tiedosto) {
			this->level_id = i;
			found = true;
		}
	}

	if (!found) {
		PLog::Write(PLog::FATAL, "PK2", "Couldn't find %s on episode", map_file.c_str());
		PK2_Error("Couldn't find test map on episode");
	}

}

GameClass::~GameClass() {}

int GameClass::Start() {

	if (this->started)
		return 1;

	// TODO - put these on the class initializer
	Gifts_Clean(); //Reset gifts
	Sprites_clear(); //Reset sprites
	Prototypes_ClearAll(); //Reset prototypes
	Fadetext_Init(); //Reset fade text
	GUI_Reset(); //Reset GUI

	if (this->Open_Map() == 1)
		PK2_Error("Can't load map");

	this->Calculate_Tiles();

	PSound::set_musicvolume(Settings.music_max_volume);

	this->started = true;

	return 0;
	
}

int GameClass::Finnish() {

	if (this->level_clear)
		return -1;
	
	this->level_clear = true;

	if (PSound::start_music(PFile::Path("music" PE_SEP "hiscore.xm")) == -1)
		PK2_Error("Can't find hiscore.xm");	
	
	Episode->level_status[this->level_id] |= LEVEL_PASSED;
	
	if (this->apples_count > 0)
		if (this->apples_got >= this->apples_count)
			Episode->level_status[this->level_id] |= LEVEL_ALLAPPLES;

	Episode->Update_NextLevel();
	
	PSound::set_musicvolume_now(Settings.music_max_volume);

	return 0;

}

int GameClass::Calculete_TileMasks() {
	
	u8 *buffer = nullptr;
	u32 leveys;
	int x, y;
	u8 color;

	PDraw::drawimage_start(map.tiles_buffer, buffer, leveys);
	for (int mask=0; mask<BLOCK_MAX_MASKS; mask++){
		for (x=0; x<32; x++){
			y=0;
			while (y<31 && (color = buffer[x+(mask%10)*32 + (y+(mask/10)*32)*leveys])==255)
				y++;

			palikkamaskit[mask].alas[x] = y;
		}

		for (x=0; x<32; x++){
			y=31;
			while (y>=0 && (color = buffer[x+(mask%10)*32 + (y+(mask/10)*32)*leveys])==255)
				y--;

			palikkamaskit[mask].ylos[x] = 31-y;
		}
	}
	PDraw::drawimage_end(map.tiles_buffer);

	return 0;
}

//PK2KARTTA::Clean_TileBuffer()
int GameClass::Clean_TileBuffer() {

	u8 *buffer = NULL;
	u32 leveys;
	int x,y;

	int w, h;
	PDraw::image_getsize(map.tiles_buffer, w, h);

	PDraw::drawimage_start(map.tiles_buffer, buffer, leveys);
	for (y = 0; y < h; y++)
		for(x = 0; x < w; x++)
			if (buffer[x+y*leveys] == 254)
				buffer[x+y*leveys] = 255;
	PDraw::drawimage_end(map.tiles_buffer);

	if (map.bg_tiles_buffer < 0)
			return 0;
	
	// Clan bg buffer
	PDraw::image_getsize(map.bg_tiles_buffer, w, h);

	PDraw::drawimage_start(map.bg_tiles_buffer, buffer, leveys);
	for (y = 0; y < h; y++)
		for(x = 0; x < w; x++)
			if (buffer[x+y*leveys] == 254)
				buffer[x+y*leveys] = 255;
	PDraw::drawimage_end(map.bg_tiles_buffer);

	return 0;
}

// This moves the collisions of the blocks palette
int GameClass::Move_Blocks() {

	this->lasketut_palikat[BLOCK_LIFT_HORI].vasen = (int)cos_table(degree);
	this->lasketut_palikat[BLOCK_LIFT_HORI].oikea = (int)cos_table(degree);

	this->lasketut_palikat[BLOCK_LIFT_VERT].ala = (int)sin_table(degree);
	this->lasketut_palikat[BLOCK_LIFT_VERT].yla = (int)sin_table(degree);

	int kytkin1_y = 0,
		kytkin2_y = 0,
		kytkin3_x = 0;

	if (this->button1 > 0) {
		kytkin1_y = 64;

		if (this->button1 < 64)
			kytkin1_y = this->button1;

		if (this->button1 > map.button1_time - 64)
			kytkin1_y = map.button1_time - this->button1;
	}

	if (this->button2 > 0) {
		kytkin2_y = 64;

		if (this->button2 < 64)
			kytkin2_y = this->button2;

		if (this->button2 > map.button2_time - 64)
			kytkin2_y = map.button2_time - this->button2;
	}

	if (this->button3 > 0) {
		kytkin3_x = 64;

		if (this->button3 < 64)
			kytkin3_x = this->button3;

		if (this->button3 > map.button3_time - 64)
			kytkin3_x = map.button3_time - this->button3;
	}

	kytkin1_y /= 2;
	kytkin2_y /= 2;
	kytkin3_x /= 2;

	this->lasketut_palikat[BLOCK_BUTTON1].ala = kytkin1_y;
	this->lasketut_palikat[BLOCK_BUTTON1].yla = kytkin1_y;

	this->lasketut_palikat[BLOCK_BUTTON2_UP].ala = -kytkin2_y;
	this->lasketut_palikat[BLOCK_BUTTON2_UP].yla = -kytkin2_y;

	this->lasketut_palikat[BLOCK_BUTTON2_DOWN].ala = kytkin2_y;
	this->lasketut_palikat[BLOCK_BUTTON2_DOWN].yla = kytkin2_y;

	this->lasketut_palikat[BLOCK_BUTTON2].ala = kytkin2_y;
	this->lasketut_palikat[BLOCK_BUTTON2].yla = kytkin2_y;

	this->lasketut_palikat[BLOCK_BUTTON3_RIGHT].oikea = kytkin3_x;
	this->lasketut_palikat[BLOCK_BUTTON3_RIGHT].vasen = kytkin3_x;
	this->lasketut_palikat[BLOCK_BUTTON3_RIGHT].koodi = BLOCK_LIFT_HORI;

	this->lasketut_palikat[BLOCK_BUTTON3_LEFT].oikea = -kytkin3_x;
	this->lasketut_palikat[BLOCK_BUTTON3_LEFT].vasen = -kytkin3_x;
	this->lasketut_palikat[BLOCK_BUTTON3_LEFT].koodi = BLOCK_LIFT_HORI;

	this->lasketut_palikat[BLOCK_BUTTON3].ala = kytkin3_x;
	this->lasketut_palikat[BLOCK_BUTTON3].yla = kytkin3_x;

	return 0;

}

int GameClass::Calculate_Tiles() {
	
	PK2BLOCK palikka;

	for (int i=0;i<150;i++){
		palikka = this->lasketut_palikat[i];

		palikka.vasen  = 0;
		palikka.oikea  = 0;//32
		palikka.yla	   = 0;
		palikka.ala    = 0;//32

		palikka.koodi  = i;

		if ((i < 80 || i > 139) && i != 255){
			palikka.tausta = false;

			palikka.oikealle	= BLOCK_WALL;
			palikka.vasemmalle	= BLOCK_WALL;
			palikka.ylos		= BLOCK_WALL;
			palikka.alas		= BLOCK_WALL;

			// Erikoislattiat

			if (i > 139){
				palikka.oikealle	= BLOCK_BACKGROUND;
				palikka.vasemmalle	= BLOCK_BACKGROUND;
				palikka.ylos		= BLOCK_BACKGROUND;
				palikka.alas		= BLOCK_BACKGROUND;
			}

			// L�pik�velt�v� lattia

			if (i == BLOCK_BARRIER_DOWN){
				palikka.oikealle	= BLOCK_BACKGROUND;
				palikka.ylos		= BLOCK_BACKGROUND;
				palikka.alas		= BLOCK_WALL;
				palikka.vasemmalle	= BLOCK_BACKGROUND;
				palikka.ala -= 27;
			}

			// M�et

			if (i > 49 && i < 60){
				palikka.oikealle	= BLOCK_BACKGROUND;
				palikka.ylos		= BLOCK_WALL;
				palikka.alas		= BLOCK_WALL;
				palikka.vasemmalle	= BLOCK_BACKGROUND;
				palikka.ala += 1;
			}

			// Kytkimet

			if (i >= BLOCK_BUTTON1 && i <= BLOCK_BUTTON3){
				palikka.oikealle	= BLOCK_WALL;
				palikka.ylos		= BLOCK_WALL;
				palikka.alas		= BLOCK_WALL;
				palikka.vasemmalle	= BLOCK_WALL;
			}
		}
		else{
			palikka.tausta = true;

			palikka.oikealle	= BLOCK_BACKGROUND;
			palikka.vasemmalle	= BLOCK_BACKGROUND;
			palikka.ylos		= BLOCK_BACKGROUND;
			palikka.alas		= BLOCK_BACKGROUND;
		}

		if (i > 131 && i < 140)
			palikka.water = true;
		else
			palikka.water = false;

		this->lasketut_palikat[i] = palikka;
	}

	Move_Blocks();

	return 0;
}

int GameClass::Open_Map() {
	
	PFile::Path path = Episode->Get_Dir(map_file);
	
	if (map.Load(path) == 1) {

		PLog::Write(PLog::ERR, "PK2", "Can't load map \"%s\" at \"%s\"", map_file.c_str(), path.c_str());
		return 1;
	
	}

	timeout = map.aika * TIME_FPS;

	if (timeout > 0)
		has_time = true;
	else
		has_time = false;

	if (!Episode->use_button_timer) {
		map.button1_time = SWITCH_INITIAL_VALUE;
		map.button2_time = SWITCH_INITIAL_VALUE;
		map.button3_time = SWITCH_INITIAL_VALUE;
	}

	if (strcmp(map.versio,"1.2") == 0 || strcmp(map.versio,"1.3") == 0)
		if (Prototypes_GetAll() == 1)
			return 1;

	Calculete_TileMasks();

	if (Clean_TileBuffer() == 1)
		return 1;

	Place_Sprites();

	if (this->chick_mode)
		PLog::Write(PLog::DEBUG, "PK2", "Chick mode on");

	Select_Start();
	
	this->keys = Count_Keys();

	Sprites_start_directions();

	Particles_Clear();
	Particles_LoadBG(&map);

	if ( strcmp(map.musiikki, "") != 0 ) {

		PFile::Path music_path = Episode->Get_Dir(map.musiikki);

		if (!FindAsset(&music_path, "music" PE_SEP)) {

			PLog::Write(PLog::ERR, "PK2", "Can't find music \"%s\", trying \"song01.xm\"", music_path.GetFileName().c_str());
			music_path = PFile::Path("music" PE_SEP "song01.xm");

		}
		
		if (PSound::start_music(music_path) == -1)
			PLog::Write(PLog::FATAL, "PK2", "Can't load any music file");

	}
	return 0;
}

void GameClass::Place_Sprites() {

	Sprites_clear();
	Sprites_add(Prototypes_List[map.pelaaja_sprite], 1, 0, 0, nullptr, false);

	for (u32 x = 0; x < PK2MAP_MAP_WIDTH; x++) {
		for (u32 y = 0; y < PK2MAP_MAP_HEIGHT; y++) {

			int sprite = map.spritet[x+y*PK2MAP_MAP_WIDTH];
			PrototypeClass* protot = Prototypes_List[sprite];

			if (sprite != 255) {

				if (!Episode->ignore_collectable)
					if (strncmp(protot->nimi, Episode->collectable_name.c_str(), Episode->collectable_name.size()) == 0)
						this->apples_count++;

				if (protot->Onko_AI(AI_CHICK) || protot->Onko_AI(AI_CHICKBOX))
					this->chick_mode = true;

				Sprites_add(protot, 0, x*32, y*32 - protot->korkeus+32, nullptr, false);
				
			}
		}
	}

	Sprites_sort_bg();

}

void GameClass::Select_Start() {

	double  pos_x = 320;
	double  pos_y = 196;

	std::vector<u32> starts;

	for (u32 i = 0; i < PK2MAP_MAP_SIZE; i++)
		if (map.seinat[i] == BLOCK_START)
			starts.push_back(i);

	if (starts.size() > 0) {
		u32 i = starts[rand() % starts.size()];

		u32 x = i % PK2MAP_MAP_WIDTH;
		u32 y = i / PK2MAP_MAP_WIDTH;

		pos_x = x*32;
		pos_y = y*32;

	}

	Player_Sprite->x = pos_x + Player_Sprite->tyyppi->leveys/2;
	Player_Sprite->y = pos_y - Player_Sprite->tyyppi->korkeus/2;

	this->camera_x = (int)Player_Sprite->x;
	this->camera_y = (int)Player_Sprite->y;
	this->dcamera_x = this->camera_x;
	this->dcamera_y = this->camera_y;

}

int GameClass::Count_Keys() {

	int keys = 0;

	for (u32 x=0; x < PK2MAP_MAP_SIZE; x++){
		u8 sprite = map.spritet[x];
		if (sprite != 255)
			if (Prototypes_List[sprite]->avain && 
				Prototypes_List[sprite]->tuhoutuminen != FX_DESTRUCT_EI_TUHOUDU)

				keys++;
	}

	return keys;
}

void GameClass::Change_SkullBlocks() {

	for (u32 x = 0; x < PK2MAP_MAP_WIDTH; x++)
		for (u32 y = 0; y < PK2MAP_MAP_HEIGHT; y++){
			
			u8 front = map.seinat[x+y*PK2MAP_MAP_WIDTH];
			u8 back  = map.taustat[x+y*PK2MAP_MAP_WIDTH];

			if (front == BLOCK_SKULL_FOREGROUND){
				map.seinat[x+y*PK2MAP_MAP_WIDTH] = 255;
				if (back != BLOCK_SKULL_FOREGROUND)
					Effect_SmokeClouds(x*32+24,y*32+6);

			}

			if (back == BLOCK_SKULL_BACKGROUND && front == 255)
				map.seinat[x+y*PK2MAP_MAP_WIDTH] = BLOCK_SKULL_FOREGROUND;
		}

	//Put in game
	this->vibration = 90;//60
	PInput::Vibrate(1000);

	map.Calculate_Edges();
}

void GameClass::Open_Locks() {

	for (u32 x = 0; x < PK2MAP_MAP_WIDTH; x++)
		for (u32 y = 0; y < PK2MAP_MAP_HEIGHT; y++){
			
			u8 palikka = map.seinat[x+y*PK2MAP_MAP_WIDTH];
			
			if (palikka == BLOCK_LOCK){
				map.seinat[x+y*PK2MAP_MAP_WIDTH] = 255;
				Effect_SmokeClouds(x*32+6,y*32+6);
			}
		}

	//Put in game
	this->vibration = 90;//60
	PInput::Vibrate(1000);

	Show_Info(tekstit->Get_Text(PK_txt.game_locksopen));

	map.Calculate_Edges();

}

void GameClass::Show_Info(const char *text) {

	if (info_text.compare(text) != 0 || info_timer == 0) {

		info_text = text;
		info_timer = INFO_TIME;
	
	}
}

bool GameClass::isStarted() {

	return started;

}