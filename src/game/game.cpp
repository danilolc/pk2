//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "game/game.hpp"

#include "system.hpp"
#include "gifts.hpp"
#include "gfx/text.hpp"
#include "gfx/particles.hpp"
#include "episode.hpp"


#include "PisteSound.hpp"
#include "PisteDraw.hpp"

#include <cstring>

//Screen Buffers

namespace Game {

PK2Kartta* map;
char map_path[PE_PATH_SIZE];

PK2BLOCK      palikat[300];
PK2BLOCK      lasketut_palikat[150];//150
PK2BLOCKMASKI palikkamaskit[BLOCK_MAX_MASKEJA];

bool started = false;
bool game_over = false;
bool level_clear = false;
bool repeating = false;

DWORD exit_timer;

DWORD timeout = 0;
int increase_time = 0;
int seconds = 0;
bool has_time = false;

int button_moving = 0;
int button1 = 0;
int button2 = 0;
int button3 = 0;

DWORD score = 0;
DWORD score_increment = 0;

int vibration;

int invisibility = 0;

int camera_x;
int camera_y;
double dcamera_x;
double dcamera_y;
double dcamera_a;
double dcamera_b;

bool paused = false;

int keys = 0;

int info_timer = 0;
char info[80] = " ";

int item_pannel_x = 0;

}

void Game::Start_Info(const char *text) {

	if (strcmp(text, Game::info) != 0 || Game::info_timer == 0) {

		strcpy(Game::info, text);
		Game::info_timer = INFO_TIME;
	
	}
}

void Calculate_MovableBlocks_Position() {
	Game::lasketut_palikat[BLOCK_HISSI_HORI].vasen = (int)cos_table[degree%360];
	Game::lasketut_palikat[BLOCK_HISSI_HORI].oikea = (int)cos_table[degree%360];

	Game::lasketut_palikat[BLOCK_HISSI_VERT].ala = (int)sin_table[degree%360];
	Game::lasketut_palikat[BLOCK_HISSI_VERT].yla = (int)sin_table[degree%360];

	int kytkin1_y = 0,
		kytkin2_y = 0,
		kytkin3_x = 0;

	if (Game::button1 > 0) {
		kytkin1_y = 64;

		if (Game::button1 < 64)
			kytkin1_y = Game::button1;

		if (Game::button1 > KYTKIN_ALOITUSARVO-64)
			kytkin1_y = KYTKIN_ALOITUSARVO - Game::button1;
	}

	if (Game::button2 > 0) {
		kytkin2_y = 64;

		if (Game::button2 < 64)
			kytkin2_y = Game::button2;

		if (Game::button2 > KYTKIN_ALOITUSARVO-64)
			kytkin2_y = KYTKIN_ALOITUSARVO - Game::button2;
	}

	if (Game::button3 > 0) {
		kytkin3_x = 64;

		if (Game::button3 < 64)
			kytkin3_x = Game::button3;

		if (Game::button3 > KYTKIN_ALOITUSARVO-64)
			kytkin3_x = KYTKIN_ALOITUSARVO - Game::button3;
	}

	kytkin1_y /= 2;
	kytkin2_y /= 2;
	kytkin3_x /= 2;

	Game::lasketut_palikat[BLOCK_KYTKIN1].ala = kytkin1_y;
	Game::lasketut_palikat[BLOCK_KYTKIN1].yla = kytkin1_y;

	Game::lasketut_palikat[BLOCK_KYTKIN2_YLOS].ala = -kytkin2_y;
	Game::lasketut_palikat[BLOCK_KYTKIN2_YLOS].yla = -kytkin2_y;

	Game::lasketut_palikat[BLOCK_KYTKIN2_ALAS].ala = kytkin2_y;
	Game::lasketut_palikat[BLOCK_KYTKIN2_ALAS].yla = kytkin2_y;

	Game::lasketut_palikat[BLOCK_KYTKIN2].ala = kytkin2_y;
	Game::lasketut_palikat[BLOCK_KYTKIN2].yla = kytkin2_y;

	Game::lasketut_palikat[BLOCK_KYTKIN3_OIKEALLE].oikea = kytkin3_x;
	Game::lasketut_palikat[BLOCK_KYTKIN3_OIKEALLE].vasen = kytkin3_x;
	Game::lasketut_palikat[BLOCK_KYTKIN3_OIKEALLE].koodi = BLOCK_HISSI_HORI;

	Game::lasketut_palikat[BLOCK_KYTKIN3_VASEMMALLE].oikea = -kytkin3_x;
	Game::lasketut_palikat[BLOCK_KYTKIN3_VASEMMALLE].vasen = -kytkin3_x;
	Game::lasketut_palikat[BLOCK_KYTKIN3_VASEMMALLE].koodi = BLOCK_HISSI_HORI;

	Game::lasketut_palikat[BLOCK_KYTKIN3].ala = kytkin3_x;
	Game::lasketut_palikat[BLOCK_KYTKIN3].yla = kytkin3_x;
}

int Calculete_TileMasks(){
	
	BYTE *buffer = nullptr;
	DWORD leveys;
	int x, y;
	BYTE color;

	PDraw::drawimage_start(Game::map->palikat_buffer, buffer, leveys);
	for (int mask=0; mask<BLOCK_MAX_MASKEJA; mask++){
		for (x=0; x<32; x++){
			y=0;
			while (y<31 && (color = buffer[x+(mask%10)*32 + (y+(mask/10)*32)*leveys])==255)
				y++;

			Game::palikkamaskit[mask].alas[x] = y;
		}

		for (x=0; x<32; x++){
			y=31;
			while (y>=0 && (color = buffer[x+(mask%10)*32 + (y+(mask/10)*32)*leveys])==255)
				y--;

			Game::palikkamaskit[mask].ylos[x] = 31-y;
		}
	}
	PDraw::drawimage_end(Game::map->palikat_buffer);

	return 0;
}

int Clean_TileBuffer() {

	BYTE *buffer = NULL;
	DWORD leveys;
	int x,y;

	PDraw::drawimage_start(Game::map->palikat_buffer,*&buffer,(DWORD &)leveys);
	for (y=0;y<480;y++)
		for(x=0;x<320;x++)
			if (buffer[x+y*leveys] == 254)
				buffer[x+y*leveys] = 255;
	PDraw::drawimage_end(Game::map->palikat_buffer);

	return 0;
}

void PK_New_Save() {
	Game::timeout = Game::map->aika;

	if (Game::timeout > 0)
		Game::has_time = true;
	else
		Game::has_time = false;

	Game::exit_timer = 0;

	Game::seconds = TIME_FPS;
	Game::score = 0;
	Game::game_over = false;
	Game::level_clear = false;
	Game::button1 = 0;
	Game::button2 = 0;
	Game::button3 = 0;
	Game::button_moving = 0;
	Game::vibration = 0;

	Game::paused = false;

	Game::info_timer = 0;

	Game::invisibility = 0;
}

int PK_Map_Open(char *nimi) {
	
	char polku[PE_PATH_SIZE];
	strcpy(polku,"");
	Episode::Get_Dir(polku);

	if (Game::map->Lataa(polku, nimi) == 1){
		printf("PK2    - Error loading map '%s' at '%s'\n", Game::map_path, polku);
		return 1;
	}

	PK_New_Save();

	if (strcmp(Game::map->versio,"1.2") == 0 || strcmp(Game::map->versio,"1.3") == 0)
		if (Prototypes_GetAll() == 1)
			return 1;

	Calculete_TileMasks();

	if (Clean_TileBuffer() == 1)
		return 1;

	Game::map->Place_Sprites();
	Game::map->Select_Start();
	Game::map->Calculate_Edges();

	Game::keys = Game::map->Count_Keys();
	
	Sprites_start_directions();

	Particles_Clear();
	Particles_LoadBG(Game::map);

	if ( strcmp(Game::map->musiikki, "") != 0) {
		char music_path[PE_PATH_SIZE] = "";
		Episode::Get_Dir(music_path);
		strcat(music_path, Game::map->musiikki);
		if (PSound::start_music(music_path) != 0) {

			printf("Can't load '%s'. ", music_path);
			strcpy(music_path, "music/");
			strcat(music_path, Game::map->musiikki);
			printf("Trying '%s'.\n", music_path);

			if (PSound::start_music(music_path) != 0) {

				printf("Can't load '%s'. Trying 'music/song01.xm'.\n", music_path);

				if (PSound::start_music("music/song01.xm") != 0){
					PK2_error = true;
					PK2_error_msg = "Can't find song01.xm";
				}
			}
		}
	}
	return 0;
}

int PK_Calculate_Tiles() {
	PK2BLOCK palikka;

	for (int i=0;i<150;i++){
		palikka = Game::lasketut_palikat[i];

		palikka.vasen  = 0;
		palikka.oikea  = 0;//32
		palikka.yla	   = 0;
		palikka.ala    = 0;//32

		palikka.koodi  = i;

		if ((i < 80 || i > 139) && i != 255){
			palikka.tausta = false;

			palikka.oikealle	= BLOCK_SEINA;
			palikka.vasemmalle	= BLOCK_SEINA;
			palikka.ylos		= BLOCK_SEINA;
			palikka.alas		= BLOCK_SEINA;

			// Erikoislattiat

			if (i > 139){
				palikka.oikealle	= BLOCK_TAUSTA;
				palikka.vasemmalle	= BLOCK_TAUSTA;
				palikka.ylos		= BLOCK_TAUSTA;
				palikka.alas		= BLOCK_TAUSTA;
			}

			// L�pik�velt�v� lattia

			if (i == BLOCK_ESTO_ALAS){
				palikka.oikealle	= BLOCK_TAUSTA;
				palikka.ylos		= BLOCK_TAUSTA;
				palikka.alas		= BLOCK_SEINA;
				palikka.vasemmalle	= BLOCK_TAUSTA;
				palikka.ala -= 27;
			}

			// M�et

			if (i > 49 && i < 60){
				palikka.oikealle	= BLOCK_TAUSTA;
				palikka.ylos		= BLOCK_SEINA;
				palikka.alas		= BLOCK_SEINA;
				palikka.vasemmalle	= BLOCK_TAUSTA;
				palikka.ala += 1;
			}

			// Kytkimet

			if (i >= BLOCK_KYTKIN1 && i <= BLOCK_KYTKIN3){
				palikka.oikealle	= BLOCK_SEINA;
				palikka.ylos		= BLOCK_SEINA;
				palikka.alas		= BLOCK_SEINA;
				palikka.vasemmalle	= BLOCK_SEINA;
			}
		}
		else{
			palikka.tausta = true;

			palikka.oikealle	= BLOCK_TAUSTA;
			palikka.vasemmalle	= BLOCK_TAUSTA;
			palikka.ylos		= BLOCK_TAUSTA;
			palikka.alas		= BLOCK_TAUSTA;
		}

		if (i > 131 && i < 140)
			palikka.vesi = true;
		else
			palikka.vesi = false;

		Game::lasketut_palikat[i] = palikka;
	}

	Calculate_MovableBlocks_Position();

	return 0;
}

int Game::Start() {

	if (Game::map == nullptr)
		Game::map = new PK2Kartta();

	Game::level_clear = false;

	if (jaksot[jakso_indeksi_nyt].lapaisty)
		Game::repeating = true;
	else
		Game::repeating = false;

	Gifts_Clean(); //Reset gifts
	Sprites_clear(); //Reset sprites
	Prototypes_ClearAll(); //Reset prototypes

	if (PK_Map_Open(Game::map_path) == 1)
		PK2_Error("Can't load map");

	PK_Calculate_Tiles();

	Fadetext_Init(); //Reset fade text

	PSound::set_musicvolume(Settings.music_max_volume);
	
	degree = 0;

	Game::item_pannel_x = -215;
	Game::score_increment = 0;
	
	Game::started = true;
	
}
