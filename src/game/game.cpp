//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "game/game.hpp"

#include "game/gifts.hpp"
#include "system.hpp"
#include "gfx/text.hpp"
#include "gfx/particles.hpp"
#include "episode/episodeclass.hpp"
#include "settings.hpp"

#include "engine/PLog.hpp"
#include "engine/PSound.hpp"
#include "engine/PDraw.hpp"

#include <cstring>

GameClass* Game = nullptr;

GameClass::GameClass(int idx) {

	this->level_id = idx;
	this->from_index = true;

}

GameClass::GameClass(std::string map_file) {

	this->map_file = map_file;
	this->from_index = false;

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

GameClass::~GameClass() {

	if (map)
		delete map;

}

int GameClass::Start() {

	if (this->started)
		return 1;
	
	this->map = new MapClass();

	if(this->from_index) {

		int index = this->level_id;

		this->map_file = Episode->levels_list[index].tiedosto;
		if (Episode->level_status[index] & LEVEL_PASSED)
			this->repeating = true;
		else
			this->repeating = false;
		
	}

	this->level_clear = false;

	Gifts_Clean(); //Reset gifts
	Sprites_clear(); //Reset sprites
	Prototypes_ClearAll(); //Reset prototypes
	Fadetext_Init(); //Reset fade text

	if (this->Open_Map() == 1)
		PK2_Error("Can't load map");

	this->Calculate_Tiles();

	PSound::set_musicvolume(Settings.music_max_volume);

	this->started = true;

	return 0;
	
}

int GameClass::Finnish() {

	if (PSound::start_music(PFile::Path("music" PE_SEP "hiscore.xm")) == -1)
		PK2_Error("Can't find hiscore.xm");
	
	this->level_clear = true;
	
	Episode->level_status[this->level_id] |= LEVEL_PASSED;
	
	if (this->apples_count > 0)
		if (this->apples_got >= this->apples_count)
			Episode->level_status[this->level_id] |= LEVEL_ALLAPPLES;

	if (Episode->levels_list[this->level_id].order == Episode->level)
		Episode->level++;
	
	PSound::set_musicvolume_now(Settings.music_max_volume);

	return 0;

}

int GameClass::Calculete_TileMasks(){
	
	u8 *buffer = nullptr;
	u32 leveys;
	int x, y;
	u8 color;

	PDraw::drawimage_start(map->tiles_buffer, buffer, leveys);
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
	PDraw::drawimage_end(map->tiles_buffer);

	return 0;
}

//PK2KARTTA::Clean_TileBuffer()
int GameClass::Clean_TileBuffer() {

	u8 *buffer = NULL;
	u32 leveys;
	int x,y;

	int w, h;
	PDraw::image_getsize(map->tiles_buffer, w, h);

	PDraw::drawimage_start(map->tiles_buffer, buffer, leveys);
	for (y = 0; y < h; y++)
		for(x = 0; x < w; x++)
			if (buffer[x+y*leveys] == 254)
				buffer[x+y*leveys] = 255;
	PDraw::drawimage_end(map->tiles_buffer);

	if (map->bg_tiles_buffer < 0)
			return 0;
	
	// Clan bg buffer
	PDraw::image_getsize(map->bg_tiles_buffer, w, h);

	PDraw::drawimage_start(map->bg_tiles_buffer, buffer, leveys);
	for (y = 0; y < h; y++)
		for(x = 0; x < w; x++)
			if (buffer[x+y*leveys] == 254)
				buffer[x+y*leveys] = 255;
	PDraw::drawimage_end(map->bg_tiles_buffer);

	return 0;
}

// This moves the collisions of the blocks palette
int GameClass::Move_Blocks() {

	this->lasketut_palikat[BLOCK_HISSI_HORI].vasen = (int)cos_table[degree%360];
	this->lasketut_palikat[BLOCK_HISSI_HORI].oikea = (int)cos_table[degree%360];

	this->lasketut_palikat[BLOCK_HISSI_VERT].ala = (int)sin_table[degree%360];
	this->lasketut_palikat[BLOCK_HISSI_VERT].yla = (int)sin_table[degree%360];

	int kytkin1_y = 0,
		kytkin2_y = 0,
		kytkin3_x = 0;

	if (this->button1 > 0) {
		kytkin1_y = 64;

		if (this->button1 < 64)
			kytkin1_y = this->button1;

		if (this->button1 > SWITCH_INITIAL_VALUE-64)
			kytkin1_y = SWITCH_INITIAL_VALUE - this->button1;
	}

	if (this->button2 > 0) {
		kytkin2_y = 64;

		if (this->button2 < 64)
			kytkin2_y = this->button2;

		if (this->button2 > SWITCH_INITIAL_VALUE-64)
			kytkin2_y = SWITCH_INITIAL_VALUE - this->button2;
	}

	if (this->button3 > 0) {
		kytkin3_x = 64;

		if (this->button3 < 64)
			kytkin3_x = this->button3;

		if (this->button3 > SWITCH_INITIAL_VALUE-64)
			kytkin3_x = SWITCH_INITIAL_VALUE - this->button3;
	}

	kytkin1_y /= 2;
	kytkin2_y /= 2;
	kytkin3_x /= 2;

	this->lasketut_palikat[BLOCK_KYTKIN1].ala = kytkin1_y;
	this->lasketut_palikat[BLOCK_KYTKIN1].yla = kytkin1_y;

	this->lasketut_palikat[BLOCK_KYTKIN2_YLOS].ala = -kytkin2_y;
	this->lasketut_palikat[BLOCK_KYTKIN2_YLOS].yla = -kytkin2_y;

	this->lasketut_palikat[BLOCK_KYTKIN2_ALAS].ala = kytkin2_y;
	this->lasketut_palikat[BLOCK_KYTKIN2_ALAS].yla = kytkin2_y;

	this->lasketut_palikat[BLOCK_KYTKIN2].ala = kytkin2_y;
	this->lasketut_palikat[BLOCK_KYTKIN2].yla = kytkin2_y;

	this->lasketut_palikat[BLOCK_KYTKIN3_OIKEALLE].oikea = kytkin3_x;
	this->lasketut_palikat[BLOCK_KYTKIN3_OIKEALLE].vasen = kytkin3_x;
	this->lasketut_palikat[BLOCK_KYTKIN3_OIKEALLE].koodi = BLOCK_HISSI_HORI;

	this->lasketut_palikat[BLOCK_KYTKIN3_VASEMMALLE].oikea = -kytkin3_x;
	this->lasketut_palikat[BLOCK_KYTKIN3_VASEMMALLE].vasen = -kytkin3_x;
	this->lasketut_palikat[BLOCK_KYTKIN3_VASEMMALLE].koodi = BLOCK_HISSI_HORI;

	this->lasketut_palikat[BLOCK_KYTKIN3].ala = kytkin3_x;
	this->lasketut_palikat[BLOCK_KYTKIN3].yla = kytkin3_x;

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

			if (i == BLOCK_ESTO_ALAS){
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

			if (i >= BLOCK_KYTKIN1 && i <= BLOCK_KYTKIN3){
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
	
	if (map->Load(path) == 1) {

		PLog::Write(PLog::ERR, "PK2", "Can't load map \"%s\" at \"%s\"", map_file.c_str(), path.c_str());
		return 1;
	
	}

	timeout = map->aika * TIME_FPS;

	if (timeout > 0)
		has_time = true;
	else
		has_time = false;

	if (strcmp(map->versio,"1.2") == 0 || strcmp(map->versio,"1.3") == 0)
		if (Prototypes_GetAll() == 1)
			return 1;

	Calculete_TileMasks();

	if (Clean_TileBuffer() == 1)
		return 1;

	map->Place_Sprites();
	map->Select_Start();
	map->Calculate_Edges();

	keys = map->Count_Keys();
	
	Sprites_start_directions();

	Particles_Clear();
	Particles_LoadBG(map);

	if ( strcmp(map->musiikki, "") != 0 ) {

		PFile::Path music_path = Episode->Get_Dir(map->musiikki);

		if (!FindAsset(&music_path, "music" PE_SEP)) {

			PLog::Write(PLog::ERR, "PK2", "Can't find music \"%s\", trying \"song01.xm\"", music_path.GetFileName().c_str());
			music_path = PFile::Path("music" PE_SEP "song01.xm");

		}
		
		if (PSound::start_music(music_path) == -1)
			PLog::Write(PLog::FATAL, "PK2", "Can't load any music file");

	}
	return 0;
}

void GameClass::Show_Info(const char *text) {

	if (strcmp(text, info) != 0 || info_timer == 0) {

		strcpy(info, text);
		info_timer = INFO_TIME;
	
	}
}

bool GameClass::isStarted() {

	return started;

}