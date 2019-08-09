//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//-------------------------
//PK2 main code
//
//This is the main code of the game,
//it interacts with the Piste Engine
//to do the entire game logic.
//This code does everything, except the
//sprite and map managing, that are made
//in a separated code to be used in the Level Editor.
//-------------------------
//It can be started with the "dev" argument to start the
//cheats and "test" follown by the episode and level to
//open directely on the level.
//	Exemple:
//	"./PK2 dev test rooster\ island\ 2/level13.map"
//	Starts the level13.map on dev mode
//
// (Deta):
// You can now also start the game in windowed mode, by starting it with the "windowed" argument
// Also, you can start it without sound and music, by starting it with the "nosound" argument
// Example:
// ./PK2 windowed nosound
//#########################
#include "PisteEngine.hpp"
#include "PisteLog.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include "game.hpp"

#include <iostream>
#include <array>
#include <sstream>
#include <string>

#define GAME_NAME   "Pekka Kana 2: Community Edition (0.1.6pre)"
#define PK2_VERSION "CE_0.1.6pre"

#ifndef _WIN32
void itoa(int n, char s[], int radix){
	sprintf(s, "%i", n);
}
void ltoa(long n, char s[], int radix){
	sprintf(s, "%ld", n);
}
#endif


//#### Classes
Piste::Game* Engine;

//#### Constants
const int MAX_SAVES = 10;
const BYTE BLOCK_MAX_MASKEJA = 150;

enum UI_MODE{
	UI_TOUCH_TO_START,
	UI_CURSOR,
	UI_GAME_BUTTONS
};

enum BLOCKS{
	BLOCK_TAUSTA,          //BLOCK_BACKGROUND
	BLOCK_SEINA,           //BLOCK_WALL
	BLOCK_MAKI_OIKEA_YLOS, //BLOCK_MAX
	BLOCK_MAKI_VASEN_YLOS, //BLOCK_MAX_
	BLOCK_MAKI_OIKEA_ALAS, //BLOCK_MAX_
	BLOCK_MAKI_VASEN_ALAS, //BLOCK_MAX_ 
	BLOCK_MAKI_YLOS,       //BLOCK_MAX_UP
	BLOCK_MAKI_ALAS        //BLOCK_MAX_DOWN
};


struct PK2BLOCK{
	BYTE	koodi;
	bool	tausta;
	BYTE	vasemmalle, oikealle, ylos, alas;
	int 	vasen, oikea, yla, ala;
	bool	vesi;
	bool	reuna;
};

struct PK2BLOCKMASKI{
	short int	ylos[32];
	short int	alas[32];
	short int	vasemmalle[32];
	short int	oikealle[32];
};

//Episode
struct PK2CollectableSettings {
	bool show_on_map;
	std::string map_image;
	
	// Dimensions to crop the image
	int mapX;
	int mapY;
	int mapW;
	int mapH;

	bool show_in_game;
	std::string game_image;
	int gameX;
	int gameY;
	int gameW;
	int gameH;

	int index_map_image;
	int index_game_image;

	void reset() {
		show_on_map = false;
		map_image = "";

		mapX = 0;
		mapY = 0;
		mapW = 0;
		mapH = 0;

		show_in_game = false;
		game_image = "";

		gameX = 0;
		gameY = 0;
		gameW = 0;
		gameH = 0;

		index_map_image = -1;
		index_game_image = -1;
	}
};

const int EPISODI_MAX_LEVELS = 100; //50;
const int MAX_EPISODEJA	= 300;

const int MAX_ILMOITUKSENNAYTTOAIKA = 700;

struct PK2EPISODESCORES{
	DWORD best_score[EPISODI_MAX_LEVELS];        // the best score of each level in episode
	char top_player[EPISODI_MAX_LEVELS][20];     // the name of the player with more score in each level on episode
	DWORD best_time[EPISODI_MAX_LEVELS];         // the best time of each level
	char fastest_player[EPISODI_MAX_LEVELS][20]; // the name of the fastest player in each level

	DWORD episode_top_score;
	char  episode_top_player[20];
};

//Screen ID
enum SCREEN{
	SCREEN_NOT_SET,
	SCREEN_BASIC_FORMAT,
	SCREEN_INTRO,
	SCREEN_MENU,
	SCREEN_MAP,
	SCREEN_GAME,
	SCREEN_SCORING,
	SCREEN_END
};
//Menu ID
enum MENU{
	MENU_MAIN,
	MENU_EPISODES,
	MENU_CONTROLS,
	MENU_GRAPHICS,
	MENU_SOUNDS,
	MENU_NAME,
	MENU_LOAD,
	MENU_TALLENNA,
	MENU_LANGUAGE
};

const int SOUND_SAMPLERATE = 22050;

int music_volume = 64;
int music_volume_now = 64;

struct PK2Vector2D {
	int x;
	int y;
};

struct PK2LEVEL {
	std::string file;
	std::string	name;

	int	x, y;
	int	level_number;
	int level_number_secret;

	std::vector<PK2Vector2D> collectables_index;

	int	icon;
	int collectables_collected;
	int overall_collectables_collected;
	int collectable_amount;

	int unlock_type;

	int unlock_score;
	int unlock_collectables;

	bool is_secret;
	bool secret_shown;
	bool passed;

	void reset() {
		file = "";
		name = "";

		x = 0;
		y = 0;

		level_number = 0;
		level_number_secret = 0;

		collectables_index.clear();

		collectables_collected = 0;
		overall_collectables_collected = 0;

		secret_shown = false;
		passed = false;
	}
};

const int MAX_FADETEKSTEJA = 50; //40;

struct PK2FADETEXT {
	char teksti[20];
	int fontti;
	int x,y,ajastin;
	bool ui;
};

// TODO clean up and remove unused things
struct PK2SAVE {
	int   jakso;
	
	char  episodi[PE_PATH_SIZE];
	char  nimi[20];
	
	bool  kaytossa;
	bool  jakso_lapaisty[EPISODI_MAX_LEVELS];
	
	bool level_passed[EPISODI_MAX_LEVELS];
	bool level_secret_passed[EPISODI_MAX_LEVELS];
	
	int collectables_collected[EPISODI_MAX_LEVELS];
	int overall_collectables_collected;
	
	std::vector<PK2Vector2D> collectables_index;

	int collectables_amount;

	int current_level;
	int current_secret_level;

	DWORD pisteet;
};

int last_save = -1;

struct PK2SETTINGS {
	char versio[4];
	bool ladattu; // if it was started here
	char kieli[128]; // language

	// grafiikka
	DWORD ruudun_korkeus; //Not used
	DWORD ruudun_leveys; //Not used
	bool  lapinakyvat_objektit;
	bool  lapinakyvat_menutekstit;
	bool  saa_efektit;
	bool  nayta_tavarat;
	bool  tausta_spritet;

	// kontrollit
	DWORD control_left;
	DWORD control_right;
	DWORD control_jump;
	DWORD control_down;
	DWORD control_walk_slow;
	DWORD control_attack1;
	DWORD control_attack2;
	DWORD control_open_gift;

	int gamepad_left;
	int gamepad_right;
	int gamepad_jump;
	int gamepad_down;
	int gamepad_walk_slow;
	int gamepad_attack1;
	int gamepad_attack2;
	int gamepad_open_gift;

	// audio
	bool musiikki;
	bool aanet;

	//Version 1.1
	bool isFullScreen;
	bool isFiltered;
	bool isFit;

	bool isWide;

	bool show_intro;

	//Version 1.2
	BYTE music_max_volume;
	BYTE sfx_max_volume;

};

struct PK2CheckpointStruct {
	BYTE layer_background[PK2KARTTA_KARTTA_KOKO];
	BYTE layer_foreground[PK2KARTTA_KARTTA_KOKO];

	std::vector<PK2Sprite> sprites;

	int player_energy;

	int player_attack1;
	int player_attack2;

	int time;
	int timeout;

	int switch1_timer;
	int switch2_timer;
	int switch3_timer;

	int keys;

	int gifts[MAX_GIFTS];
	int gift_amount;

	int score;

	int startX;
	int startY;

	int collectables_collected;
};

//#### Global Variables
int screen_width  = 800;
int screen_height = 480;

int switch1_time = SWITCH_TIME_DEFAULT;
int switch2_time = SWITCH_TIME_DEFAULT;
int switch3_time = SWITCH_TIME_DEFAULT;

bool test_level = false;
bool dev_mode = false;
bool test_windowed = false;
bool test_mute = false;

bool PK2_error = false;
const char* PK2_error_msg = NULL;

bool closing_game = false;

bool show_collectables;

bool unload = false;
bool precalculated_sincos = false;

struct PK2LevelStats {
	bool level_passed[EPISODI_MAX_LEVELS];
	bool level_secret_passed[EPISODI_MAX_LEVELS];

	std::vector<PK2Vector2D> collectables_index;

	int collectables_collected[EPISODI_MAX_LEVELS];
	int overall_collectables_collected;

	int current_level = 1;
	int current_secret_level = 1;

	void reset() {
		for (int i = 0; i < EPISODI_MAX_LEVELS; i++) {
			level_passed[i] = false;
			level_secret_passed[i] = false;

			collectables_collected[i] = 0;
		}

		overall_collectables_collected = 0;

		current_level = 1;
		current_secret_level = 1;

		collectables_index.clear();
	}
} level_stats;

PK2SETTINGS settings;

PK2CollectableSettings collectable_settings;

int gui_touch,
	gui_egg,
    gui_doodle,
	gui_arr,
	gui_up,
	gui_down,
	gui_left,
	gui_right,
	gui_menu,
	gui_gift,
	gui_tab;

//Debug info
bool	draw_dubug_info = false;
int		debug_sprites = 0;
int		debug_drawn_sprites = 0;
int		debug_active_sprites = 0;

//KARTTA
PK2Kartta *kartta;
char next_map[PE_PATH_SIZE];

// Put this into the Game namespace?
PK2CheckpointStruct* checkpoint_data = nullptr;

namespace Game {

	PK2::Particle_System* Particles;
	PK2::SpriteSystem* Sprites;
	PK2::GiftSystem* Gifts;

	PK2Kartta* current_map;
	char map_path[PE_PATH_SIZE];

	int vibration;

	int camera_x;
	int camera_y;
	double dcamera_x;
	double dcamera_y;
	double dcamera_a;
	double dcamera_b;

}

//PALIKAT JA MASKIT
PK2BLOCK	palikat[300];

PK2BLOCK	lasketut_palikat[150];//150

PK2BLOCKMASKI palikkamaskit[BLOCK_MAX_MASKEJA];

//Fade Text
PK2FADETEXT fadetekstit[MAX_FADETEKSTEJA];
int fadeteksti_index = 0;

//Screen Buffers
int  kuva_peli  = -1;
int  kuva_peli2 = -1;
int  kuva_tausta = -1;

//Fonts
int FONT_SMALL;
int FONT_BIG;
int FONT_WHITE;
int FONT_BLACK;

//Controls
int hiiri_x = 10;
int hiiri_y = 10;
int key_delay = 0;

// menu variables
bool settingsControlKeyboard = true;
bool settingsControlGamepad = false;
bool settingsEditSingle = false;

//JAKSO JA EPISODI
int	jakso = 1;
int episode_amount = 1;
int episodi_lkm = 0;
int jakso_indeksi_nyt = 1;
char episodit[MAX_EPISODEJA][PE_PATH_SIZE];

std::string current_episode;

int  episodisivu = 0;
PK2LEVEL levels[EPISODI_MAX_LEVELS];
bool jakso_lapaisty = false;
bool uusinta = false;
bool game_over = false;
DWORD lopetusajastin = 0;
DWORD jakso_pisteet = 0;
DWORD fake_pisteet = 0;

//Player
DWORD pisteet = 0;
DWORD piste_lisays = 0;
char pelaajan_nimi[20] = " ";

bool name_edit = false;

//PALIKOIHIN LIITTYV�T AJASTIMET
int kytkin1 = 0, kytkin2 = 0, kytkin3 = 0;
int palikka_animaatio = 0;

//��NIEFEKTIT
int kytkin_aani,
	hyppy_aani,
	loiskahdus_aani,
	avaa_lukko_aani,
	menu_aani,
	ammuu_aani,
	kieku_aani,
	tomahdys_aani,
	pistelaskuri_aani;

int sprite_aanet[50]; // spritejen k�ytt�m�t ��nibufferit

//TALLENNUKSET
PK2SAVE saves_list[MAX_SAVES];
int lataa_peli = -1;

//MUUTA
double cos_table[360];
double sin_table[360];

int degree = 0,
	degree_temp = 0;

int avaimia = 0;

// Time
const int TIME_FPS = 60;
DWORD timeout = 0;
int increase_time = 0;
int sekunti = 0;
bool aikaraja = false;

int kytkin_tarina = 0;

int item_paneeli_x = 10;

int info_timer = 0;
//char info[80] = " ";
std::string info = "";

//PISTEIDEN LASKEMINEN
PK2EPISODESCORES episodipisteet;

int pistelaskuvaihe = 0;
int pistelaskudelay = 0;
DWORD	bonuspisteet = 0,
		aikapisteet = 0,
		energiapisteet = 0,
		esinepisteet = 0,
		pelastuspisteet = 0;

bool jakso_uusi_ennatys = false;
bool jakso_uusi_ennatysaika = false;
bool episodi_uusi_ennatys = false;
bool episodi_uusi_ennatys_naytetty = false;

//PELIN MUUTTUJAT
char tyohakemisto[PE_PATH_SIZE];
bool paused = false;
int game_screen = SCREEN_NOT_SET;
int game_next_screen = SCREEN_BASIC_FORMAT;
bool episode_started = false;
bool going_to_game = false;
bool siirry_pistelaskusta_karttaan = false;

int nakymattomyys = 0;

//INTRO
DWORD introlaskuri = 0;
bool siirry_introsta_menuun = false;

//LOPPURUUTU
DWORD loppulaskuri = 0;
bool siirry_lopusta_menuun = false;

//GRAFIIKKA
bool doublespeed = false;
bool skip_frame = false;

//Menus
int menu_nyt = MENU_MAIN;
int menu_lue_kontrollit = 0;
int menu_name_index = 0;
char menu_name_last_mark = '\0';
int menu_valittu_id = 0;
int menu_valinta_id = 1;
RECT menunelio;

//Framerate
float fps = 0;
bool show_fps = false;

//LANGUAGE AND TEXTS OF THE GAME
PisteLanguage *tekstit;
bool customHudText = false;

char langlist[60][PE_PATH_SIZE];
char langmenulist[10][PE_PATH_SIZE];
int langlistindex = 0;
int totallangs = 0;

LANGUAGE PK_txt;

// Prototypes
int  PK_Settings_Save(char *filename);
void PK_Load_EpisodeDir(char *tiedosto);
void PK_Fade_Quit();

// Filesystem
bool PK_Check_File(std::string filename) {
	bool ok = true;

	std::ifstream ifs(filename.c_str());
	
	if (ifs.fail())
		ok = false;
	
	ifs.close();

	return ok;
}

void PK_Settings_Start(){
	settings.ladattu = false;

	strcpy(settings.kieli,"english.txt");

	settings.lapinakyvat_objektit = true;
	settings.lapinakyvat_menutekstit = false;
	settings.saa_efektit = true;
	settings.nayta_tavarat = true;
	settings.tausta_spritet = true;

	settings.aanet = true;
	settings.musiikki = true;

	settings.control_left      = PI_LEFT;
	settings.control_right     = PI_RIGHT;
	settings.control_jump      = PI_UP;
	settings.control_down      = PI_DOWN;
	settings.control_walk_slow = PI_RALT;
	settings.control_attack1   = PI_RCONTROL;
	settings.control_attack2   = PI_RSHIFT;
	settings.control_open_gift = PI_SPACE;

	settings.gamepad_left		= PI_CONTROLLER_LEFT;
	settings.gamepad_right		= PI_CONTROLLER_RIGHT;
	settings.gamepad_jump		= PI_CONTROLLER_B;
	settings.gamepad_down		= PI_CONTROLLER_DOWN;
	settings.gamepad_walk_slow	= PI_CONTROLLER_SHOULDER_LEFT;
	settings.gamepad_attack1	= PI_CONTROLLER_A;
	settings.gamepad_attack2	= PI_CONTROLLER_X;
	settings.gamepad_open_gift	= PI_CONTROLLER_Y;

	settings.isFiltered = true;
	settings.isFit = true;
	settings.isFullScreen = true;
	settings.isWide = true;

	settings.music_max_volume = 64;
	settings.sfx_max_volume = 90;

	music_volume = settings.music_max_volume;
	music_volume_now = settings.music_max_volume - 1;

	PisteUtils_CreateDir("data");
	PK_Settings_Save("data/settings.ini");
}

int PK_Settings_Open(char *filename){
	ifstream *file = new ifstream(filename, ios::binary);

	if (file->fail()){
		delete file;
		PK_Settings_Start();
		return 1;
	}

	file->read(settings.versio, 4); // Read the version from settings file

	if (strcmp(settings.versio, "1.2") != 0) { // If settings version isn't 1.2
		delete file;
		PK_Settings_Start();
		return 2;
	}
	file->read((char*)&settings, sizeof(settings));
	delete file;

	music_volume = settings.music_max_volume;
	music_volume_now = settings.music_max_volume - 1;

	settings.ladattu = true;
	screen_width = settings.isWide ? 800 : 640;

	return 0;
}
int PK_Settings_Save(char *filename){
	std::ofstream ifs(filename, ios::binary);
	ifs.write ("1.2", 4);

	ifs.write ((char *)&settings, sizeof (settings));

	ifs.flush();
	ifs.close();

	return 0;
}

void PK_EpisodeScore_Start(){
	for (int i=0;i<EPISODI_MAX_LEVELS;i++){
		episodipisteet.best_score[i] = 0;
		episodipisteet.best_time[i] = 0;
		strcpy(episodipisteet.top_player[i]," ");
		strcpy(episodipisteet.fastest_player[i]," ");
	}

	episodipisteet.episode_top_score = 0;
	strcpy(episodipisteet.episode_top_player," ");
}

int  PK_EpisodeScore_Compare(int jakso, DWORD episteet, DWORD aika, bool loppupisteet){
	int paluu = 0;
	if (!loppupisteet) {
		if (episteet > episodipisteet.best_score[jakso]) {
			strcpy(episodipisteet.top_player[jakso],pelaajan_nimi);
			episodipisteet.best_score[jakso] = episteet;
			jakso_uusi_ennatys = true;
			paluu++;
		}
		if ((aika < episodipisteet.best_time[jakso] || episodipisteet.best_time[jakso] == 0) && kartta->aika > 0) {
			strcpy(episodipisteet.fastest_player[jakso],pelaajan_nimi);
			episodipisteet.best_time[jakso] = aika;
			jakso_uusi_ennatysaika = true;
			paluu++;
		}
	}
	else {
		if (episteet > episodipisteet.episode_top_score) {
		    episodipisteet.episode_top_score = episteet;
			strcpy(episodipisteet.episode_top_player,pelaajan_nimi);
			episodi_uusi_ennatys = true;
			paluu++;
		}
	}
	return paluu;
}
int  PK_EpisodeScore_Open(char *filename){
	PK_Load_EpisodeDir(filename);

	ifstream *tiedosto = new ifstream(filename, ios::binary);
	char versio[4];

	if (tiedosto->fail()){
		delete (tiedosto);
		PK_EpisodeScore_Start();
		return 1;
	}

	tiedosto->read ((char *)versio, 4);

	if (strcmp(versio,"1.0") == 0) {
		tiedosto->read ((char *)&episodipisteet, sizeof (episodipisteet));
	}

	delete (tiedosto);

	return 0;
}
int  PK_EpisodeScore_Save(char *filename){
	PK_Load_EpisodeDir(filename);

	ofstream *tiedosto = new ofstream(filename, ios::binary);
	tiedosto->write ("1.0", 4);
	tiedosto->write ((char *)&episodipisteet, sizeof (episodipisteet));
	delete (tiedosto);
	return 0;
}

void PK_Load_InfoText() {
	PisteLanguage* temp;
	char infofile[PE_PATH_SIZE] = "infosign.txt";
	char otsikko[] = "info00";
	int indeksi1, indeksi2, i;

	temp = new PisteLanguage();
	PK_Load_EpisodeDir(infofile);

	if (PK_Check_File(infofile)){
		if (temp->Read_File(infofile)){

			for (i = 0 ; i<19 ; i++){
				if(i+1 >= 10) otsikko[4] = '1'; //Make "info" + itos(i)
				otsikko[5] = '1' + (char)(i%10);

				indeksi1 = tekstit->Hae_Indeksi(otsikko);
				indeksi2 = temp->Hae_Indeksi(otsikko);

				if (indeksi1 != -1 && indeksi2 != -1)
					tekstit->Korvaa_Teksti(indeksi1,temp->Hae_Teksti(indeksi2));
			}
		}
	}

	delete (temp);
}
int PK_Load_Font() {
	int ind_font = 0,
		ind_path = 0;

	PisteDraw2_Clear_Fonts();
	ind_path = tekstit->Hae_Indeksi("font path");

	ind_font = tekstit->Hae_Indeksi("font small font");
	if (ind_path == -1 || ind_font == -1) {
		if ((FONT_SMALL = PisteDraw2_Font_Create("language/fonts/", "ScandicSmall.txt")) == -1) {
			PK2_error = true;
			PK2_error_msg = "Can't create font 1 from ScandicSmall.txt";
		}
	}
	else {
		if ((FONT_SMALL = PisteDraw2_Font_Create(tekstit->Hae_Teksti(ind_path), tekstit->Hae_Teksti(ind_font))) == -1) {
			PK2_error = true;
			PK2_error_msg = "Can't create font 1";
		}
	}

	ind_font = tekstit->Hae_Indeksi("font big font normal");
	if (ind_path == -1 || ind_font == -1) {
		if ((FONT_BIG = PisteDraw2_Font_Create("language/fonts/", "ScandicBig1.txt")) == -1) {
			PK2_error = true;
			PK2_error_msg = "Can't create font 1 from ScandicBig1.txt";
		}
	}
	else {
		if ((FONT_BIG = PisteDraw2_Font_Create(tekstit->Hae_Teksti(ind_path), tekstit->Hae_Teksti(ind_font))) == -1) {
			PK2_error = true;
			PK2_error_msg = "Can't create font 2";
		}
	}

	ind_font = tekstit->Hae_Indeksi("font big font hilite");
	if (ind_path == -1 || ind_font == -1) {
		if ((FONT_WHITE = PisteDraw2_Font_Create("language/fonts/", "ScandicBig2.txt")) == -1) {
			PK2_error = true;
			PK2_error_msg = "Can't create font 3 from ScandicBig2.txt";
		}
	}
	else {
		if ((FONT_WHITE = PisteDraw2_Font_Create(tekstit->Hae_Teksti(ind_path), tekstit->Hae_Teksti(ind_font))) == -1) {
			PK2_error = true;
			PK2_error_msg = "Can't create font 3";
		}
	}

	ind_font = tekstit->Hae_Indeksi("font big font shadow");
	if (ind_path == -1 || ind_font == -1) {
		if ((FONT_BLACK = PisteDraw2_Font_Create("language/fonts/", "ScandicBig3.txt")) == -1) {
			PK2_error = true;
			PK2_error_msg = "Can't create font 4 from ScandicBig3.txt";
		}
	}
	else {
		if ((FONT_BLACK = PisteDraw2_Font_Create(tekstit->Hae_Teksti(ind_path), tekstit->Hae_Teksti(ind_font))) == -1) {
			PK2_error = true;
			PK2_error_msg = "Can't create font 4";
		}
	}

	/*
	if ((FONT_BIG = PisteDraw2_Font_Create("language/fonts/","ScandicBig1.txt")) == -1){
	PK2_error = true;
	PK2_error_msg = "Can't create font 2 from ScandicBig1.txt";
	}

	if ((FONT_WHITE = PisteDraw2_Font_Create("language/fonts/","ScandicBig2.txt")) == -1){
	PK2_error = true;
	PK2_error_msg = "Can't create font 3 from ScandicBig2.txt";
	}

	if ((FONT_BLACK = PisteDraw2_Font_Create("language/fonts/","ScandicBig3.txt")) == -1){
	PK2_error = true;
	PK2_error_msg = "Can't create font 4 from ScandicBig3.txt";
	}*/

	return 0;
}

void PK_Load_Custom_Hud_Text() {
	std::string file = "episodes/" + current_episode + "/data/game.txt";
	
	if (PK_Check_File(file)) {
		PisteLanguage *hudLang = new PisteLanguage();

		if (hudLang->Read_File(file)) {
			customHudText = true;

			tekstit->Korvaa_Teksti(PK_txt.game_score, hudLang->Hae_Teksti(hudLang->Hae_Indeksi("score")));
			tekstit->Korvaa_Teksti(PK_txt.game_time, hudLang->Hae_Teksti(hudLang->Hae_Indeksi("game time")));
			tekstit->Korvaa_Teksti(PK_txt.game_energy, hudLang->Hae_Teksti(hudLang->Hae_Indeksi("energy")));
			tekstit->Korvaa_Teksti(PK_txt.game_items, hudLang->Hae_Teksti(hudLang->Hae_Indeksi("items")));
			tekstit->Korvaa_Teksti(PK_txt.game_attack1, hudLang->Hae_Teksti(hudLang->Hae_Indeksi("attack 1")));
			tekstit->Korvaa_Teksti(PK_txt.game_attack2, hudLang->Hae_Teksti(hudLang->Hae_Indeksi("attack 2")));
			tekstit->Korvaa_Teksti(PK_txt.game_keys, hudLang->Hae_Teksti(hudLang->Hae_Indeksi("keys")));
			tekstit->Korvaa_Teksti(PK_txt.game_newdoodle, hudLang->Hae_Teksti(hudLang->Hae_Indeksi("new doodle attack")));
			tekstit->Korvaa_Teksti(PK_txt.game_newegg, hudLang->Hae_Teksti(hudLang->Hae_Indeksi("new egg attack")));
			tekstit->Korvaa_Teksti(PK_txt.game_newitem, hudLang->Hae_Teksti(hudLang->Hae_Indeksi("new item")));
			tekstit->Korvaa_Teksti(PK_txt.game_collectables, hudLang->Hae_Teksti(hudLang->Hae_Indeksi("collectables")));
		}

		delete hudLang;
	}
}

bool PK_Load_Language(){
	char tiedosto[PE_PATH_SIZE];
	int i;

	strcpy(tiedosto,"language/");

	if(totallangs == 0){
		totallangs = PisteUtils_Scandir(".txt", tiedosto, langlist, 60);
		for(i=0;i<10;i++)
			strcpy(langmenulist[i],langlist[i]);
	}

	strcat(tiedosto,settings.kieli);

	if (!tekstit->Read_File(tiedosto))
		return false;

	PK_Load_Font();

	// Aloitusikkuna
	PK_txt.setup_options			= tekstit->Hae_Indeksi("setup options");
	PK_txt.setup_videomodes			= tekstit->Hae_Indeksi("setup video modes");
	PK_txt.setup_music_and_sounds	= tekstit->Hae_Indeksi("setup music & sounds");
	PK_txt.setup_music				= tekstit->Hae_Indeksi("setup music");
	PK_txt.setup_sounds				= tekstit->Hae_Indeksi("setup sounds");
	PK_txt.setup_language			= tekstit->Hae_Indeksi("setup language");
	PK_txt.setup_play				= tekstit->Hae_Indeksi("setup play");
	PK_txt.setup_exit				= tekstit->Hae_Indeksi("setup exit");

	// Intro
	PK_txt.intro_presents			= tekstit->Hae_Indeksi("intro presents");
	PK_txt.intro_a_game_by			= tekstit->Hae_Indeksi("intro a game by");
	PK_txt.intro_original			= tekstit->Hae_Indeksi("intro original character design");
	PK_txt.intro_tested_by			= tekstit->Hae_Indeksi("intro tested by");
	PK_txt.intro_thanks_to			= tekstit->Hae_Indeksi("intro thanks to");
	PK_txt.intro_translation		= tekstit->Hae_Indeksi("intro translation");
	PK_txt.intro_translator			= tekstit->Hae_Indeksi("intro translator");

	// P��valikko
	PK_txt.mainmenu_new_game		= tekstit->Hae_Indeksi("main menu new game");
	PK_txt.mainmenu_continue		= tekstit->Hae_Indeksi("main menu continue");
	PK_txt.mainmenu_load_game		= tekstit->Hae_Indeksi("main menu load game");
	PK_txt.mainmenu_save_game		= tekstit->Hae_Indeksi("main menu save game");
	PK_txt.mainmenu_controls		= tekstit->Hae_Indeksi("main menu controls");
	PK_txt.mainmenu_graphics		= tekstit->Hae_Indeksi("main menu graphics");
	PK_txt.mainmenu_sounds			= tekstit->Hae_Indeksi("main menu sounds");
	PK_txt.mainmenu_exit			= tekstit->Hae_Indeksi("main menu exit game");

	PK_txt.mainmenu_return			= tekstit->Hae_Indeksi("back to main menu");

	// Lataus
	PK_txt.loadgame_title			= tekstit->Hae_Indeksi("load menu title");
	PK_txt.loadgame_info			= tekstit->Hae_Indeksi("load menu info");
	PK_txt.loadgame_episode			= tekstit->Hae_Indeksi("load menu episode");
	PK_txt.loadgame_level			= tekstit->Hae_Indeksi("load menu level");

	// Tallennus
	PK_txt.savegame_title			= tekstit->Hae_Indeksi("save menu title");
	PK_txt.savegame_info			= tekstit->Hae_Indeksi("save menu info");
	PK_txt.savegame_episode			= tekstit->Hae_Indeksi("save menu episode");
	PK_txt.savegame_level			= tekstit->Hae_Indeksi("save menu level");

	// Kontrollit
	PK_txt.controls_title			= tekstit->Hae_Indeksi("controls menu title");
	PK_txt.controls_moveleft		= tekstit->Hae_Indeksi("controls menu move left");
	PK_txt.controls_moveright		= tekstit->Hae_Indeksi("controls menu move right");
	PK_txt.controls_jump			= tekstit->Hae_Indeksi("controls menu jump");
	PK_txt.controls_duck			= tekstit->Hae_Indeksi("controls menu duck");
	PK_txt.controls_walkslow		= tekstit->Hae_Indeksi("controls menu walk slow");
	PK_txt.controls_eggattack		= tekstit->Hae_Indeksi("controls menu egg attack");
	PK_txt.controls_doodleattack	= tekstit->Hae_Indeksi("controls menu doodle attack");
	PK_txt.controls_useitem			= tekstit->Hae_Indeksi("controls menu use item");
	PK_txt.controls_edit			= tekstit->Hae_Indeksi("controls menu edit");
	PK_txt.controls_kbdef			= tekstit->Hae_Indeksi("controls menu keyboard def");
	PK_txt.controls_gp4def			= tekstit->Hae_Indeksi("controls menu gamepad4");
	PK_txt.controls_gp6def			= tekstit->Hae_Indeksi("controls menu gamepad6");

	PK_txt.gfx_title				= tekstit->Hae_Indeksi("graphics menu title");
	PK_txt.gfx_tfx_on				= tekstit->Hae_Indeksi("graphics menu transparency fx on");
	PK_txt.gfx_tfx_off				= tekstit->Hae_Indeksi("graphics menu transparency fx off");
	PK_txt.gfx_tmenus_on			= tekstit->Hae_Indeksi("graphics menu menus are transparent");
	PK_txt.gfx_tmenus_off			= tekstit->Hae_Indeksi("graphics menu menus are not transparent");
	PK_txt.gfx_items_on				= tekstit->Hae_Indeksi("graphics menu item bar is visible");
	PK_txt.gfx_items_off			= tekstit->Hae_Indeksi("graphics menu item bar is not visible");
	PK_txt.gfx_weather_on			= tekstit->Hae_Indeksi("graphics menu weather fx on");
	PK_txt.gfx_weather_off			= tekstit->Hae_Indeksi("graphics menu weather fx off");
	PK_txt.gfx_bgsprites_on			= tekstit->Hae_Indeksi("graphics menu bg sprites on");
	PK_txt.gfx_bgsprites_off		= tekstit->Hae_Indeksi("graphics menu bg sprites off");
	PK_txt.gfx_speed_normal			= tekstit->Hae_Indeksi("graphics menu game speed normal");
	PK_txt.gfx_speed_double			= tekstit->Hae_Indeksi("graphics menu game speed double");

	PK_txt.sound_title				= tekstit->Hae_Indeksi("sounds menu title");
	PK_txt.sound_sfx_volume			= tekstit->Hae_Indeksi("sounds menu sfx volume");
	PK_txt.sound_music_volume		= tekstit->Hae_Indeksi("sounds menu music volume");
	PK_txt.sound_more				= tekstit->Hae_Indeksi("sounds menu more");
	PK_txt.sound_less				= tekstit->Hae_Indeksi("sounds menu less");

	PK_txt.playermenu_type_name		= tekstit->Hae_Indeksi("player screen type your name");
	PK_txt.playermenu_continue		= tekstit->Hae_Indeksi("player screen continue");
	PK_txt.playermenu_clear			= tekstit->Hae_Indeksi("player screen clear");
	PK_txt.player_default_name		= tekstit->Hae_Indeksi("player default name");

	PK_txt.episodes_choose_episode	= tekstit->Hae_Indeksi("episode menu choose episode");
	PK_txt.episodes_no_maps			= tekstit->Hae_Indeksi("episode menu no maps");
	PK_txt.episodes_get_more		= tekstit->Hae_Indeksi("episode menu get more episodes at");

	PK_txt.map_total_score			= tekstit->Hae_Indeksi("map screen total score");
	PK_txt.map_next_level			= tekstit->Hae_Indeksi("map screen next level");
	PK_txt.map_episode_best_player	= tekstit->Hae_Indeksi("episode best player");
	PK_txt.map_episode_hiscore		= tekstit->Hae_Indeksi("episode hiscore");
	PK_txt.map_level_best_player	= tekstit->Hae_Indeksi("level best player");
	PK_txt.map_level_hiscore		= tekstit->Hae_Indeksi("level hiscore");
	PK_txt.map_level_fastest_player = tekstit->Hae_Indeksi("level fastest player");
	PK_txt.map_level_best_time		= tekstit->Hae_Indeksi("level best time");

	PK_txt.score_screen_title		= tekstit->Hae_Indeksi("score screen title");
	PK_txt.score_screen_level_score	= tekstit->Hae_Indeksi("score screen level score");
	PK_txt.score_screen_bonus_score	= tekstit->Hae_Indeksi("score screen bonus score");
	PK_txt.score_screen_time_score	= tekstit->Hae_Indeksi("score screen time score");
	PK_txt.score_screen_energy_score= tekstit->Hae_Indeksi("score screen energy score");
	PK_txt.score_screen_item_score	= tekstit->Hae_Indeksi("score screen item score");
	PK_txt.score_screen_total_score	= tekstit->Hae_Indeksi("score screen total score");
	PK_txt.score_screen_new_level_hiscore	= tekstit->Hae_Indeksi("score screen new level hiscore");
	PK_txt.score_screen_new_level_best_time= tekstit->Hae_Indeksi("score screen new level best time");
	PK_txt.score_screen_new_episode_hiscore= tekstit->Hae_Indeksi("score screen new episode hiscore");
	PK_txt.score_screen_continue		= tekstit->Hae_Indeksi("score screen continue");

	PK_txt.game_score				= tekstit->Hae_Indeksi("score");
	PK_txt.game_time				= tekstit->Hae_Indeksi("game time");
	PK_txt.game_energy				= tekstit->Hae_Indeksi("energy");
	PK_txt.game_items				= tekstit->Hae_Indeksi("items");
	PK_txt.game_attack1				= tekstit->Hae_Indeksi("attack 1");
	PK_txt.game_attack2				= tekstit->Hae_Indeksi("attack 2");
	PK_txt.game_keys				= tekstit->Hae_Indeksi("keys");
	PK_txt.game_clear				= tekstit->Hae_Indeksi("level clear");
	PK_txt.game_timebonus			= tekstit->Hae_Indeksi("time bonus");
	PK_txt.game_ko					= tekstit->Hae_Indeksi("knocked out");
	PK_txt.game_timeout				= tekstit->Hae_Indeksi("time out");
	PK_txt.game_tryagain			= tekstit->Hae_Indeksi("try again");
	PK_txt.game_locksopen			= tekstit->Hae_Indeksi("locks open");
	PK_txt.game_newdoodle			= tekstit->Hae_Indeksi("new doodle attack");
	PK_txt.game_newegg				= tekstit->Hae_Indeksi("new egg attack");
	PK_txt.game_newitem				= tekstit->Hae_Indeksi("new item");
	PK_txt.game_loading				= tekstit->Hae_Indeksi("loading");
	PK_txt.game_paused				= tekstit->Hae_Indeksi("game paused");

	PK_txt.end_congratulations	= tekstit->Hae_Indeksi("end congratulations");
	PK_txt.end_chickens_saved	= tekstit->Hae_Indeksi("end chickens saved");
	PK_txt.end_the_end			= tekstit->Hae_Indeksi("end the end");

	PK_txt.info01					= tekstit->Hae_Indeksi("info01");
	PK_txt.info02					= tekstit->Hae_Indeksi("info02");
	PK_txt.info03					= tekstit->Hae_Indeksi("info03");
	PK_txt.info04					= tekstit->Hae_Indeksi("info04");
	PK_txt.info05					= tekstit->Hae_Indeksi("info05");
	PK_txt.info06					= tekstit->Hae_Indeksi("info06");
	PK_txt.info07					= tekstit->Hae_Indeksi("info07");
	PK_txt.info08					= tekstit->Hae_Indeksi("info08");
	PK_txt.info09					= tekstit->Hae_Indeksi("info09");
	PK_txt.info10					= tekstit->Hae_Indeksi("info10");
	PK_txt.info11					= tekstit->Hae_Indeksi("info11");
	PK_txt.info12					= tekstit->Hae_Indeksi("info12");
	PK_txt.info13					= tekstit->Hae_Indeksi("info13");
	PK_txt.info14					= tekstit->Hae_Indeksi("info14");
	PK_txt.info15					= tekstit->Hae_Indeksi("info15");
	PK_txt.info16					= tekstit->Hae_Indeksi("info16");
	PK_txt.info17					= tekstit->Hae_Indeksi("info17");
	PK_txt.info18					= tekstit->Hae_Indeksi("info18");
	PK_txt.info19					= tekstit->Hae_Indeksi("info19");

	PK_txt.game_collectables = tekstit->Hae_Indeksi("info19") + 1;
	tekstit->Korvaa_Teksti(PK_txt.game_collectables, "Collectables:");

	return true;
}
void PK_Load_EpisodeDir(char *tiedosto){
	char uusi_tiedosto[255];

	strcpy(uusi_tiedosto, tyohakemisto);
	strcat(uusi_tiedosto, "/episodes/");
	strcat(uusi_tiedosto, current_episode.c_str());
	strcat(uusi_tiedosto, "/");
	strcat(uusi_tiedosto, tiedosto);
	strcpy(tiedosto, uusi_tiedosto);
}

void PK_Search_File(){
	int i=0;
	char hakemisto[PE_PATH_SIZE];
	char list[EPISODI_MAX_LEVELS][PE_PATH_SIZE];

	for (int j = 0; j < EPISODI_MAX_LEVELS; j++)
		memset(list[j], '\0', PE_PATH_SIZE);

	PK2Kartta *temp = new PK2Kartta();

	strcpy(hakemisto,"");
	PK_Load_EpisodeDir(hakemisto);
	episode_amount = PisteUtils_Scandir(".map", hakemisto, list, EPISODI_MAX_LEVELS);
	episode_amount += PisteUtils_Scandir(".cemap", hakemisto, list, EPISODI_MAX_LEVELS);

	for (i = 0; i <= episode_amount; i++) {
		levels[i].file = list[i];
		if (temp->Lataa_Pelkat_Tiedot(hakemisto, levels[i].file.c_str()) == 0){
			levels[i].name = temp->map_name;
			levels[i].x = temp->x;//   142 + i*35;
			levels[i].y = temp->y;// 270;
			levels[i].level_number = temp->jakso;
			levels[i].icon = temp->ikoni;

			levels[i].level_number_secret = temp->level_number_secret;

			levels[i].is_secret = temp->is_secret;
			levels[i].unlock_score = temp->unlock_score;
			levels[i].unlock_collectables = temp->unlock_collectables;
			levels[i].unlock_type = temp->unlock;
		}
	}

	PK2LEVEL jakso;

	bool lopeta = false;

	while (!lopeta){
		lopeta = true;

		for (i = 0; i < episode_amount; i++) {
			if (levels[i].level_number > levels[i+1].level_number){
				jakso = levels[i];
				levels[i] = levels[i+1];
				levels[i+1] = jakso;
				lopeta = false;
			}
		}
	}
	delete temp;
}

//==================================================
//(#2) Save
//==================================================

void PK_New_Game(){
	pisteet = 0;
	jakso = 1;

	level_stats.reset();
	
	for (int i = 0; i < EPISODI_MAX_LEVELS; i++) {
		levels[i].reset();
	}

	collectable_settings.reset();

	show_collectables = false;

	PK_Load_Language();
}
void PK_New_Save(){
	timeout = kartta->aika;

	if (timeout > 0)
		aikaraja = true;
	else
		aikaraja = false;

	lopetusajastin = 0;

	sekunti = TIME_FPS;
	jakso_pisteet = 0;
	game_over = false;
	jakso_lapaisty = false;
	kytkin1 = 0;
	kytkin2 = 0;
	kytkin3 = 0;
	kytkin_tarina = 0;
	Game::vibration = 0;

	paused = false;

	info_timer = 0;

	nakymattomyys = 0;
}
void PK_Start_Saves(){
	for (int i=0;i<EPISODI_MAX_LEVELS;i++){
		levels[i].name = "";
		levels[i].file = "";
		levels[i].x = 0;
		levels[i].y = 0;
		levels[i].level_number = -1;
		levels[i].passed = false;
		levels[i].icon = 0;
	}
}

int PK_Alphabetical_Compare(char *a, char *b){
	int apituus = strlen(a);
	int bpituus = strlen(b);
	int looppi = apituus;

	if (bpituus < apituus)
		looppi = bpituus;

	PisteUtils_Lower(a);
	PisteUtils_Lower(b);

	for (int i=0;i<looppi;i++){
		if (a[i] < b[i])
			return 2;
		if (a[i] > b[i])
			return 1;
	}

	if (apituus > bpituus)
		return 1;

	if (apituus < bpituus)
		return 2;

	return 0;
}
int PK_Order_Episodes(){
	char temp[PE_PATH_SIZE] = "";
	bool tehty;

	if (episodi_lkm > 1) {

		for (int i = episodi_lkm-1 ; i>=0 ;i--) {

			tehty = true;

			//for (t=0;t<i;t++) {
			for (int t=2 ; t<i+2 ; t++) {
				if (PK_Alphabetical_Compare(episodit[t],episodit[t+1]) == 1) {
					strcpy(temp, episodit[t]);
					strcpy(episodit[t], episodit[t+1]);
					strcpy(episodit[t+1], temp);
					tehty = false;
				}
			}

			if (tehty)
				return 0;
		}
	}

	return 0;
}
int PK_Search_Episode(){
	int i;
	char hakemisto[PE_PATH_SIZE];

	for (i=0;i<MAX_EPISODEJA;i++)
		strcpy(episodit[i],"");

	strcpy(hakemisto,"episodes/");

	episodi_lkm = PisteUtils_Scandir("/", hakemisto, episodit, MAX_EPISODEJA) - 2;

	PK_Order_Episodes();

	return 0;
}

int PK_Empty_Records(){
	for (int i = 0;i < MAX_SAVES;i++){
		saves_list[i].kaytossa = false;

		strcpy(saves_list[i].episodi," ");
		strcpy(saves_list[i].nimi,"");

		saves_list[i].jakso = 0;
		saves_list[i].pisteet = 0;

		saves_list[i].current_level = 0;
		saves_list[i].current_secret_level = 0;

		for (int j = 0; j < EPISODI_MAX_LEVELS; j++) {
			saves_list[i].jakso_lapaisty[j] = false;
			saves_list[i].collectables_collected[j] = 0;
		}
	}

	return 0;
}
int PK_Search_Records(char *filename){
	char versio[2];
	char lkmc[8];
	int lkm, i;

	ifstream file = ifstream(filename, ios::binary);

	if (file.fail()){
		file.close();

		PK_Empty_Records();
		return 1;
	}

	PK_Empty_Records();

	file.read(versio,	sizeof(versio));

	if (strcmp(versio,"1")==0){
		file.read(lkmc, sizeof(lkmc));
		lkm = atoi(lkmc);

		for (int i = 0; i < MAX_SAVES; i++) {
			size_t size = 0;

			saves_list[i].collectables_index.clear();

			file.read(reinterpret_cast<char*>(&saves_list[i].jakso), sizeof(saves_list[i].jakso));
			file.read(saves_list[i].episodi, sizeof(saves_list[i].episodi));
			file.read(saves_list[i].nimi, sizeof(saves_list[i].nimi));
			file.read(reinterpret_cast<char*>(&saves_list[i].kaytossa), sizeof(saves_list[i].kaytossa));
			file.read(reinterpret_cast<char*>(&saves_list[i].jakso_lapaisty), sizeof(saves_list[i].jakso_lapaisty));
			file.read(reinterpret_cast<char*>(&saves_list[i].collectables_collected), sizeof(saves_list[i].collectables_collected));
			file.read(reinterpret_cast<char*>(&saves_list[i].overall_collectables_collected), sizeof(saves_list[i].overall_collectables_collected));
			file.read(reinterpret_cast<char*>(&size), sizeof(size_t));

			for (size_t j = 0; j < size; j++) {
				PK2Vector2D vec;

				file.read(reinterpret_cast<char*>(&vec), sizeof(vec));

				saves_list[i].collectables_index.push_back(vec);
			}

			file.read(reinterpret_cast<char*>(&saves_list[i].collectables_amount), sizeof(&saves_list[i].collectables_amount));
			file.read(reinterpret_cast<char*>(&saves_list[i].current_level), sizeof(&saves_list[i].current_level));
			file.read(reinterpret_cast<char*>(&saves_list[i].current_secret_level), sizeof(&saves_list[i].current_secret_level));
			file.read(reinterpret_cast<char*>(&saves_list[i].pisteet), sizeof(&saves_list[i].pisteet));
		}

		file.read(reinterpret_cast<char*>(&last_save), sizeof(last_save));
	}

	file.close();

	return 0;
}
int PK_Save_All_Records(char *filename){
	char versio[2] = "1";
	char lkm[8];

	itoa(MAX_SAVES,lkm,10);

	ofstream file = ofstream(filename, ios::binary);
	file.write(versio, sizeof(versio));
	file.write(lkm,    sizeof(lkm));

	for (int i = 0; i < MAX_SAVES; i++) {
		size_t size = saves_list[i].collectables_index.size();

		file.write(reinterpret_cast<char*>(&saves_list[i].jakso), sizeof(saves_list[i].jakso));
		file.write(saves_list[i].episodi, sizeof(saves_list[i].episodi));
		file.write(saves_list[i].nimi, sizeof(saves_list[i].nimi));
		file.write(reinterpret_cast<char*>(&saves_list[i].kaytossa), sizeof(saves_list[i].kaytossa));
		file.write(reinterpret_cast<char*>(&saves_list[i].jakso_lapaisty), sizeof(saves_list[i].jakso_lapaisty));
		file.write(reinterpret_cast<char*>(&saves_list[i].collectables_collected), sizeof(saves_list[i].collectables_collected));
		file.write(reinterpret_cast<char*>(&saves_list[i].overall_collectables_collected), sizeof(saves_list[i].overall_collectables_collected));
		file.write(reinterpret_cast<char*>(&size), sizeof(size_t));

		for (size_t j = 0; j < size; j++) {
			PK2Vector2D vec = saves_list[i].collectables_index[j];

			file.write(reinterpret_cast<char*>(&vec), sizeof(vec));
		}

		file.write(reinterpret_cast<char*>(&saves_list[i].collectables_amount), sizeof(&saves_list[i].collectables_amount));
		file.write(reinterpret_cast<char*>(&saves_list[i].current_level), sizeof(&saves_list[i].current_level));
		file.write(reinterpret_cast<char*>(&saves_list[i].current_secret_level), sizeof(&saves_list[i].current_secret_level));
		file.write(reinterpret_cast<char*>(&saves_list[i].pisteet), sizeof(&saves_list[i].pisteet));
	}

	file.write(reinterpret_cast<char*>(&last_save), sizeof(last_save));

	file.close();

	return 0;
}
int PK_Load_Records(int i){
	if (strcmp(saves_list[i].episodi," ")!=0) {

		current_episode = saves_list[i].episodi;

		strcpy(pelaajan_nimi, saves_list[i].nimi);
		//jakso = saves_list[i].jakso;
		pisteet = saves_list[i].pisteet;

		//std::copy(std::begin(saves_list[i].collectables_collected), std::end(saves_list[i].collectables_collected), levels)

		for (int j = 0; j < EPISODI_MAX_LEVELS; j++) {
			level_stats.collectables_collected[j] = saves_list[i].collectables_collected[j];
		}

		for (int j = 0; j < EPISODI_MAX_LEVELS; j++) {
			if (levels[j].collectables_index.size() > 0) {
				levels[j].collectables_index.clear();
				level_stats.collectables_index.clear();
			}

			for (int k = 0; k < saves_list[i].collectables_index.size(); k++) {
				levels[j].collectables_index.push_back(saves_list[i].collectables_index[k]);
			}
		}

		level_stats.overall_collectables_collected = saves_list[i].overall_collectables_collected;

		level_stats.current_level = saves_list[i].current_level;
		level_stats.current_secret_level = saves_list[i].current_secret_level;

		std::copy(std::begin(saves_list[i].level_passed), std::end(saves_list[i].level_passed), level_stats.level_passed);
		std::copy(std::begin(saves_list[i].level_secret_passed), std::end(saves_list[i].level_secret_passed), level_stats.level_secret_passed);

		for (int i = 0; i < 100; i++) {
			if (levels[i].is_secret) {
				if ((levels[i].unlock_type == UNLOCK::SCORE && pisteet >= levels[i].unlock_score)) {
					levels[i].secret_shown = true;
				}
				else if (levels[i].unlock_type == UNLOCK::COLLECTABLES && level_stats.overall_collectables_collected >= levels[i].unlock_collectables) {
					levels[i].secret_shown = true;
				}
			}
		}

		PK_Start_Saves();

		//for (int j = 0;j < EPISODI_MAX_LEVELS;j++)
		//	levels[j].lapaisty = tallennukset[i].jakso_lapaisty[j];

		if (PK_Check_File("episodes/" + current_episode + "/gfx/pk2stuff.bmp")) {
			PisteDraw2_Image_Delete(kuva_peli);
			kuva_peli = PisteDraw2_Image_Load(("episodes/" + current_episode + "/gfx/pk2stuff.bmp").c_str(), false);
		}

		PK_Load_Custom_Hud_Text();

		game_next_screen = SCREEN_MAP;

		lataa_peli = i;
		episode_started = false;

		show_collectables = false;
	}

	return 0;
}

int PK_Save_Records(int i){
	saves_list[i].kaytossa = true;
	strcpy(saves_list[i].episodi, current_episode.c_str());
	strcpy(saves_list[i].nimi,pelaajan_nimi);
	//saves_list[i].jakso = jakso;
	saves_list[i].pisteet = pisteet;
	saves_list[i].overall_collectables_collected = level_stats.overall_collectables_collected;
	saves_list[i].current_level = level_stats.current_level;
	saves_list[i].current_secret_level = level_stats.current_secret_level;

	std::copy(std::begin(level_stats.level_passed), std::end(level_stats.level_passed), saves_list[i].level_passed);
	std::copy(std::begin(level_stats.level_secret_passed), std::end(level_stats.level_secret_passed), saves_list[i].level_secret_passed);

	for (int j = 0; j < EPISODI_MAX_LEVELS; j++) {
		saves_list[i].jakso_lapaisty[j] = levels[j].level_number;
		saves_list[i].collectables_collected[j] = level_stats.collectables_collected[j];

		for (size_t k = 0; k < levels[j].collectables_index.size(); k++) {
			saves_list[i].collectables_index.push_back(levels[j].collectables_index[k]);
		}
	}

	PK_Save_All_Records("data/saves.dat");

	return 0;
}

// Sounds
void PK_Play_Sound(int aani, int voimakkuus, int x, int y, int freq, bool random_freq){
	if (!test_mute && aani > -1 && settings.sfx_max_volume > 0 && voimakkuus > 0){
		if (x < Game::camera_x+screen_width && x > Game::camera_x && y < Game::camera_y+screen_height && y > Game::camera_y){
			voimakkuus = voimakkuus / (100 / settings.sfx_max_volume);

			if (voimakkuus > 100)
				voimakkuus = 100;

			if (voimakkuus < 0)
				voimakkuus = 0;

			int pan = Game::camera_x + (screen_width / 2) - x;
			pan *= -2;

			if (random_freq)
				freq = freq + rand()%4000 - rand()%2000;

			int err = PisteSound_PlaySFX(aani,settings.sfx_max_volume, pan, freq);
			if (err)
				printf("PK2     - Error playing sound. Error %i\n", err);
		}
	}
}
void PK_Play_MenuSound(int aani, int voimakkuus){
	if (!test_mute) {
		if (aani > -1 && settings.sfx_max_volume > 0 && voimakkuus > 0) {
			voimakkuus = voimakkuus / (100 / settings.sfx_max_volume);

			if (voimakkuus > 100)
				voimakkuus = 100;

			if (voimakkuus < 0)
				voimakkuus = 0;

			int freq = 22050 + rand() % 5000 - rand() % 5000;

			int err = PisteSound_PlaySFX(aani, settings.sfx_max_volume, 0, freq);
			if (err)
				printf("PK2     - Error playing sound. Error %i\n", err);
		}
	}
}

void PK_Update_Mouse(int x = -1, int y = -1) {
	#ifdef __ANDROID__
    	float x, y;
		PisteInput_GetTouchPos(x, y);

		screen_width * x - PisteDraw2_GetXOffset();
		hiiri_y = screen_height * y;
	#else
		MOUSE mouse = PisteInput_UpdateMouse(game_screen == SCREEN_MAP || game_screen == SCREEN_SCORING, settings.isFullScreen);
		mouse.x -= PisteDraw2_GetXOffset();

		if (mouse.x < 0) mouse.x = 0;
		if (mouse.y < 0) mouse.y = 0;
		if (mouse.x > 640-19) mouse.x = 640-19;
		if (mouse.y > 480-19) mouse.y = 480-19;

		int xx = mouse.x;
		int yy = mouse.y;

		if (x > -1) {
			xx = x;
		}

		if (y > -1) {
			yy = y;
		}

		hiiri_x = xx;
		hiiri_y = yy;
	#endif
}

void PK_Precalculate_SinCos(){
	int i;
	for (i=0; i<360; i++) cos_table[i] = cos(M_PI*2* (i%360)/180)*33;
	for (i=0; i<360; i++) sin_table[i] = sin(M_PI*2* (i%360)/180)*33;
}

int PK_Palikka_Tee_Maskit(){
	BYTE *buffer = NULL;
	DWORD leveys;
	int x,y;
	BYTE color;

	PisteDraw2_DrawImage_Start(kartta->palikat_buffer,*&buffer,(DWORD &)leveys);
	for (int mask=0; mask<BLOCK_MAX_MASKEJA; mask++){
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
	PisteDraw2_DrawImage_End(kartta->palikat_buffer);

	return 0;
}
int PK_Clean_TileBuffer(){
	BYTE *buffer = NULL;
	DWORD leveys;
	int x,y;

	PisteDraw2_DrawImage_Start(kartta->palikat_buffer,*&buffer,(DWORD &)leveys);
	for (y=0;y<480;y++)
		for(x=0;x<320;x++)
			if (buffer[x+y*leveys] == 254)
				buffer[x+y*leveys] = 255;
	PisteDraw2_DrawImage_End(kartta->palikat_buffer);

	return 0;
}
int PK_MenuShadow_Create(int kbuffer, DWORD kleveys, int kkorkeus, int startx){
	BYTE *buffer = NULL;
	DWORD leveys;
	BYTE vari,/* vari2, vari3,*/ vari32;
	DWORD x, mx, my;
	int y;
	double kerroin;


	if (PisteDraw2_DrawImage_Start(kbuffer,*&buffer,(DWORD &)leveys)==1)
		return 1;

	if (kleveys > leveys)
		kleveys = leveys;

	kkorkeus -= 2;
	kleveys  -= 2;

	kleveys += startx - 30;

	kerroin = 3;//2.25;//2

	//for (y=0;y<kkorkeus;y++)
	for (y=35;y<kkorkeus-30;y++)
	{
		my = (y)*leveys;
		//for(x=0;x<kleveys;x++)
		for(x=startx;x<kleveys-30;x++)
		{
			mx = x+my;
			vari   = buffer[mx];

			vari32 = VARI_TURKOOSI;//(vari>>5)<<5;
			vari %= 32;

			if (x == startx || x == kleveys-31 || y == 35 || y == kkorkeus-31)
				vari = int((double)vari / (kerroin / 1.5));//1.25
			else
				vari = int((double)vari / kerroin);//1.25

			vari += vari32;

			buffer[mx] = vari;
		}

		if (kerroin > 1.005)
			kerroin = kerroin - 0.005;
	}

	if (PisteDraw2_DrawImage_End(kbuffer)==1)
		return 1;

	return 0;
}

int PK_Draw_Transparent_Object(int source_buffer, DWORD source_x, DWORD source_y, DWORD source_width, DWORD source_height,
							DWORD target_x, DWORD target_y, int alpha, BYTE colorsum) {
	PD_RECT src = { source_x, source_y, source_width, source_height };
	PD_RECT dst = { target_x, target_y, source_width, source_height };
	int res = PisteDraw2_Image_CutClipTransparent(source_buffer, src, dst, alpha, colorsum);

	return res;
}

void PK_Start_Info(std::string text, int duration){
	if (text != info || info_timer == 0) {
		info = text;
		
		info_timer = duration;
	
	}
}

//==================================================
//(#4) Text
//==================================================

int PK_Wavetext_Draw(std::string text, int fontti, int x, int y){
	int pituus = text.size();
	int vali = 0;
	char kirjain[3] = " \0";
	int ys, xs;

	if (pituus > 0){
		for (int i=0;i<pituus;i++){
			ys = (int)(sin_table[((i+degree)*8)%360])/7;
			xs = (int)(cos_table[((i+degree)*8)%360])/9;
			kirjain[0] = text[i];
			PisteDraw2_Font_Write(FONT_BLACK,kirjain,x+vali-xs+3,y+ys+3);
			vali += PisteDraw2_Font_Write(fontti,kirjain,x+vali-xs,y+ys);
		}
	}
	return vali;
}

int PK_WavetextSlow_Draw(std::string text, int fontti, int x, int y){
	int pituus = strlen(text.c_str());
	int vali = 0;
	char kirjain[3] = " \0";
	int ys, xs;

	// TODO FIX some strings not showing/being cut off
	if (pituus > 0) {
		for (int i = 0; i < pituus; i++) {
			ys = (int)(sin_table[((i+degree)*4)%360])/9;
			xs = (int)(cos_table[((i+degree)*4)%360])/11;
			kirjain[0] = text[i];

			if (settings.lapinakyvat_menutekstit) {
				vali += PisteDraw2_Font_WriteAlpha(fontti, kirjain, x + vali - xs, y + ys, 75);
			} else {
				PisteDraw2_Font_Write(FONT_BLACK,kirjain,x+vali-xs+1,y+ys+1);
				vali += PisteDraw2_Font_Write(fontti,kirjain,x+vali-xs,y+ys);
			}
		}
	}

	return vali;
}

void PK_Fadetext_Init(){
	for (int i=0;i<MAX_FADETEKSTEJA;i++)
		fadetekstit[i].ajastin = 0;
}
void PK_Fadetext_New(int fontti, char *teksti, DWORD x, DWORD y, DWORD ajastin, bool ui){
	fadetekstit[fadeteksti_index].fontti = fontti;
	strcpy(fadetekstit[fadeteksti_index].teksti,teksti);
	fadetekstit[fadeteksti_index].x = x;
	fadetekstit[fadeteksti_index].y = y;
	fadetekstit[fadeteksti_index].ajastin = ajastin;
	fadetekstit[fadeteksti_index].ui = ui;
	fadeteksti_index++;

	if (fadeteksti_index >= MAX_FADETEKSTEJA)
		fadeteksti_index = 0;
}
int  PK_Fadetext_Draw(){
	int pros;
	int x, y;

	for (int i=0; i<MAX_FADETEKSTEJA; i++)
		if (fadetekstit[i].ajastin > 0){
			if (fadetekstit[i].ajastin > 50)
				pros = 100;
			else
				pros = fadetekstit[i].ajastin * 2;

			x = fadetekstit[i].ui ? fadetekstit[i].x : fadetekstit[i].x - Game::camera_x;
			y = fadetekstit[i].ui ? fadetekstit[i].y : fadetekstit[i].y - Game::camera_y;

			if (settings.lapinakyvat_objektit && pros < 100)
				PisteDraw2_Font_WriteAlpha(fadetekstit[i].fontti, fadetekstit[i].teksti, x, y, pros);
			else
				PisteDraw2_Font_Write(fadetekstit[i].fontti, fadetekstit[i].teksti, x, y);

		}
	return 0;
}
void PK_Fadetext_Update(){
	for (int i=0;i<MAX_FADETEKSTEJA;i++)
		if (fadetekstit[i].ajastin > 0){
			fadetekstit[i].ajastin--;

			if (fadetekstit[i].ajastin%2 == 0)
				fadetekstit[i].y--;

			if (fadetekstit[i].x < Game::camera_x || fadetekstit[i].x > Game::camera_x + screen_width ||
				fadetekstit[i].y < Game::camera_y || fadetekstit[i].y > Game::camera_y + screen_height)
				if(!fadetekstit[i].ui) fadetekstit[i].ajastin = 0;
		}
}

//==================================================
//(#5) Particle System
//==================================================

PK2::Particle::Particle(int type, double x, double y, double a, double b, int anim, int time, double weight, int color) {
	this->type = type;
	this->x = x;
	this->y = y;
	this->a = a;
	this->b = b;
	this->anim = anim;
	this->time = time;
	this->weight = weight;
	this->color = color;
}

PK2::Particle::~Particle() {}

void PK2::Particle::draw() {

	alpha = time;
	if (alpha > 100) alpha = 100;

	if (time > 0)
		switch (this->type) {
			case PARTICLE_STAR:        draw_star();      break;
			case PARTICLE_FEATHER:     draw_feather();   break;
			case PARTICLE_DUST_CLOUDS: draw_dust();      break;
			case PARTICLE_LIGHT:       draw_light();     break;
			case PARTICLE_SPARK:       draw_spark();     break;
			case PARTICLE_POINT:       draw_dot();       break;
			case PARTICLE_SMOKE:       draw_smoke();     break;

			case BGPARTICLE_WATERDROP: draw_waterdrop(); break;
			case BGPARTICLE_LEAF1:     draw_leaf1();     break;
			case BGPARTICLE_LEAF2:     draw_leaf2();     break;
			case BGPARTICLE_LEAF3:     draw_leaf3();     break;
			case BGPARTICLE_LEAF4:     draw_leaf4();     break;
			case BGPARTICLE_FLAKE1:    draw_flake1();    break;
			case BGPARTICLE_FLAKE2:    draw_flake2();    break;
			case BGPARTICLE_FLAKE3:    draw_flake3();    break;
			case BGPARTICLE_FLAKE4:    draw_flake4();    break;
		}
}

void PK2::Particle::update() {

	switch (this->type) {
		case PARTICLE_STAR:
		case PARTICLE_FEATHER:
		case PARTICLE_DUST_CLOUDS:
		case PARTICLE_LIGHT:
		case PARTICLE_SPARK:
		case PARTICLE_POINT:
		case PARTICLE_SMOKE: 
			update_fg(); break;
		case BGPARTICLE_WATERDROP: update_waterdrop(); update_bg(); break;
		case BGPARTICLE_LEAF1:     update_leaf1();     update_bg(); break;
		case BGPARTICLE_LEAF2:     update_leaf2();     update_bg(); break;
		case BGPARTICLE_LEAF3:     update_leaf3();     update_bg(); break;
		case BGPARTICLE_LEAF4:     update_leaf4();     update_bg(); break;
		case BGPARTICLE_FLAKE1:    update_flake1();    update_bg(); break;
		case BGPARTICLE_FLAKE2:    update_flake2();    update_bg(); break;
		case BGPARTICLE_FLAKE3:    update_flake3();    update_bg(); break;
		case BGPARTICLE_FLAKE4:    update_flake4();    update_bg(); break;
		default: break;
	}
}

void PK2::Particle::set_type(int type){

	this->type = type;

}

bool PK2::Particle::time_over() {

	return (time == 0);

}

void PK2::Particle::draw_dot() {

	PisteDraw2_ScreenFill((int) (x - Game::camera_x), (int) (y - Game::camera_y), (int) (x - Game::camera_x + 1), (int) (y - Game::camera_y + 1), color+25);

}

void PK2::Particle::draw_star() {

	if (color > 99 || !settings.lapinakyvat_objektit)
		PisteDraw2_Image_CutClip(kuva_peli, (int) (x - Game::camera_x), (int) (y - Game::camera_y), 1, 1, 11, 11);
	else
		PK_Draw_Transparent_Object(kuva_peli, 2, 2, 10, 10, x-Game::camera_x, y-Game::camera_y, alpha, color);

}

void PK2::Particle::draw_hit() {

	int framex = ((degree%12)/3) * 58;
	PisteDraw2_Image_CutClip(kuva_peli,x-Game::camera_x-28+8, y-Game::camera_y-27+8,1+framex,83,1+57+framex,83+55);
}

void PK2::Particle::draw_light() {

	if (settings.lapinakyvat_objektit)
		PK_Draw_Transparent_Object(kuva_peli, 1, 14, 13, 13, x-Game::camera_x, y-Game::camera_y, alpha, color);
	else{
		int vx = (color/32) * 14;
		PisteDraw2_Image_CutClip(kuva_peli,x-Game::camera_x, y-Game::camera_y,1+vx,14+14,14+vx,27+14);
	}

}

void PK2::Particle::draw_spark() {

	if (settings.lapinakyvat_objektit)
		PK_Draw_Transparent_Object(kuva_peli, 99, 14, 7, 7, x-Game::camera_x, y-Game::camera_y, alpha, color);
	else{
		int vx = (color/32) * 8;
		PisteDraw2_Image_CutClip(kuva_peli,x-Game::camera_x, y-Game::camera_y,99+vx,14+14,106+vx,21+14);
	}

}

void PK2::Particle::draw_feather() {

	int xplus = (anim/7) * 21;
	PisteDraw2_Image_CutClip(kuva_peli,x-Game::camera_x,y-Game::camera_y,14+xplus,1,34+xplus,12);
	anim++;
	if (anim > 63)
		anim = 0;

}

void PK2::Particle::draw_smoke() {

	int frame = (anim/7);
	int xplus = (frame%17) * 36;
	int yplus = 0;

	if (anim < 7*30) {

		if (frame > 16)
			yplus = 32;

		PisteDraw2_Image_CutClip(kuva_peli,x-Game::camera_x,y-Game::camera_y,1+xplus,338+yplus,34+xplus,366+yplus);
		anim++;
	}

}

void PK2::Particle::draw_dust() {

	if (alpha > 99 || !settings.lapinakyvat_objektit)
		PisteDraw2_Image_CutClip(kuva_peli,x-Game::camera_x,y-Game::camera_y,226,2,224,49);
	else
		PK_Draw_Transparent_Object(kuva_peli, 226, 2, 18, 19, x-Game::camera_x, y-Game::camera_y, alpha, color);
	PisteDraw2_Image_CutClip(kuva_peli,x-Game::camera_x,y-Game::camera_y,226, 2, 18, 19);

}

void PK2::Particle::draw_waterdrop() {

	int kx = (int)(x-Game::camera_x);
	int ky = (int)(y-Game::camera_y);

	PisteDraw2_ScreenFill(kx,ky,kx+1,ky+4,40+(int)b);

}

void PK2::Particle::draw_leaf1() {

	int kx = (int)(x-Game::camera_x),
		ky = (int)(y-Game::camera_y);

	PisteDraw2_ScreenFill(kx,ky,kx+2,ky+2,96+6+(int)b+(int)(x+y)%10);

}

void PK2::Particle::draw_leaf2() {

	int kx = (int)(x-Game::camera_x),
		ky = (int)(y-Game::camera_y),
		frame = (int(y/10)%4)*23;

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,1+frame,141,21+frame,152);

}

void PK2::Particle::draw_leaf3() {

	int kx = (int)(x-Game::camera_x),
		ky = (int)(y-Game::camera_y),
		frame = (int(y/5)%4)*20;

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,93+frame,141,109+frame,150);

}

void PK2::Particle::draw_leaf4() {

	int kx = (int)(x-Game::camera_x),
		ky = (int)(y-Game::camera_y),
		frame = (int(y/5)%2)*14;

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,173+frame,141,183+frame,150);

}

void PK2::Particle::draw_flake1() {

	int kx = (int)(x-Game::camera_x),
		ky = (int)(y-Game::camera_y);

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,1,155,8,162);

}

void PK2::Particle::draw_flake2() {

	int kx = (int)(x-Game::camera_x),
		ky = (int)(y-Game::camera_y);

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,11,155,16,160);

}

void PK2::Particle::draw_flake3() {

	int kx = (int)(x-Game::camera_x),
		ky = (int)(y-Game::camera_y);

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,19,155,22,158);

}

void PK2::Particle::draw_flake4() {

	int kx = (int)(x-Game::camera_x),
		ky = (int)(y-Game::camera_y);

	PisteDraw2_ScreenFill(kx,ky,kx+2,ky+2,20+(int)b);

}

void PK2::Particle::update_waterdrop() {

	y += b / 3.0 + 2.0;

}

void PK2::Particle::update_leaf1() {

	x += a / 9.0;
	y += b / 9.0;

}

void PK2::Particle::update_leaf2() {

	x += a / 14.0;
	y += b / 9.0;

}

void PK2::Particle::update_leaf3() {

	x += a / 12.0;
	y += b / 9.0;

}

void PK2::Particle::update_leaf4() {

	x += a / 11.0;
	y += b / 9.0;

}

void PK2::Particle::update_flake1() {

	x += sin_table[int(y)%360]/50.0;
	y += b / 7.0;

}

void PK2::Particle::update_flake2() {

	x += sin_table[int(y)%360]/100.0;
	y += b / 8.0;

}

void PK2::Particle::update_flake3() {

	y += b / 8.0;

}

void PK2::Particle::update_flake4() {

	y += b / 9.0;

}

void PK2::Particle::update_fg() {
	if (this->time > 0){
		this->x += this->a;
		this->y += this->b;

		if (this->weight > 0)
			this->b += this->weight;

		this->time--;
	}
}

void PK2::Particle::update_bg() {

	if ( x  >  Game::camera_x + screen_width )
		x  =  Game::camera_x + int(x - Game::camera_x + screen_width) % screen_width;
	if ( x  <  Game::camera_x )
		x  =  Game::camera_x + screen_width - int(Game::camera_x - x) % screen_width;
	if ( y  >  Game::camera_y + screen_height )
		y  =  Game::camera_y + int(y - Game::camera_y + screen_height) % screen_height;
	if ( y  <  Game::camera_y )
		y  =  Game::camera_y + screen_height - int(Game::camera_y - y) % screen_height;
	
}






PK2::Particle_System::Particle_System() {}

PK2::Particle_System::~Particle_System() {

	clear_particles();

}

void PK2::Particle_System::update() {
	
	if (!paused)
		for (Particle* particle : Particles)
			particle->update();

	//Todo - clean "time_over" particles - use deque on Particles

	if (settings.saa_efektit)
		for (Particle* particle : BGParticles)
			particle->update();

}

void PK2::Particle_System::new_particle(int type, double x, double y, double a, double b, int time, double weight, int color) {

	PK2::Particle* particle = new PK2::Particle(type, x, y, a, b, rand()%63, time, weight, color);
	Particles.push_back(particle);

}

void PK2::Particle_System::draw_front_particles() {

	for (Particle* particle : Particles)
		particle->draw();

}

void PK2::Particle_System::draw_bg_particles() {

	if (!settings.saa_efektit) return;

	for (Particle* particle : BGParticles)
		particle->draw();

}

void PK2::Particle_System::load_bg_particles(PK2Kartta* map) {
	int i = 0;

	while (i < nof_bg_particles){
		PK2::Particle* particle = new PK2::Particle(
			BGPARTICLE_NOTHING,   //type
			rand()%screen_width,  //x
			rand()%screen_height, //y
			rand()%10-rand()%10,  //a
			rand()%9+1,           //b
			rand()%63,            //anim
			1,                    //time -- unused
			rand()%10,            //weight
			0);                   //color
		BGParticles.push_back(particle);
		//taustapartikkelit[i].a = rand()%10-rand()%10;
		//taustapartikkelit[i].aika = 1;
		//taustapartikkelit[i].anim = rand()%10;
		//taustapartikkelit[i].b = rand()%9+1;
		//taustapartikkelit[i].type = BGPARTICLE_NOTHING;
		//taustapartikkelit[i].x = rand()%screen_width;
		//taustapartikkelit[i].y = rand()%screen_height;
		//taustapartikkelit[i].paino = rand()%10;
		//taustapartikkelit[i].vari = 0;
		i++;
	}

	if (map->ilma == ILMA_SADE || map->ilma == ILMA_SADEMETSA)
		for( i = 0; i < nof_bg_particles; i++)
			BGParticles[i]->set_type(BGPARTICLE_WATERDROP);

	if (map->ilma == ILMA_METSA || map->ilma == ILMA_SADEMETSA)
		for( i = 0; i < nof_bg_particles / 8; i++)
			BGParticles[i]->set_type(BGPARTICLE_LEAF1 + rand()%4);

	if (map->ilma == ILMA_LUMISADE){
		for( i = 0; i < nof_bg_particles / 2; i++)
			BGParticles[i]->set_type(BGPARTICLE_FLAKE4);
		for( i = 0; i < nof_bg_particles / 3; i++)
			BGParticles[i]->set_type(BGPARTICLE_FLAKE1 + rand()%2+1);//3
	}

}

void PK2::Particle_System::clear_particles() {
	while (Particles.size() > 0) {
		delete Particles.back();
		Particles.pop_back();
	}
	while (BGParticles.size() > 0) {
		delete BGParticles.back();
		BGParticles.pop_back();
	}
}




//==================================================
//(#6) Effects
//==================================================
namespace Effect {

void Feathers(DWORD x, DWORD y) {
	for (int i=0;i<9;i++)//6
		Game::Particles->new_particle(PARTICLE_FEATHER,x+rand()%17-rand()%17,y+rand()%20-rand()%10,
							(rand()%16-rand()%16)/10.0,(45+rand()%45)/100.0,300+rand()%40,0,0);
}

void Splash(DWORD x, DWORD y, BYTE color) {
	/*
	for (int i=0;i<12;i++)
		Game::Particles->new_particle(	PARTICLE_LIGHT,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5,(rand()%7-rand()%7)/3,
							rand()%50+60,0.025,color);*/
	for (int i=0;i<7;i++)
		Game::Particles->new_particle(	PARTICLE_SPARK,x+rand()%17-13,y+rand()%17-13,
							(rand()%5-rand()%5)/4.0,(rand()%4-rand()%7)/3.0,
							rand()%50+40,0.025,color);//0.015

	for (int i=0;i<20;i++)
		Game::Particles->new_particle(	PARTICLE_POINT,x+rand()%17-13,y+rand()%17-13,
							(rand()%5-rand()%5)/4.0,(rand()%2-rand()%7)/3.0,
							rand()%50+40,0.025,color);//0.015

	PK_Play_Sound(loiskahdus_aani, 100, x, y, SOUND_SAMPLERATE, true);
}

void Explosion(DWORD x, DWORD y, BYTE color) {
	int i;

	for (i=0;i<3;i++)
		Game::Particles->new_particle(	PARTICLE_SMOKE,x+rand()%17-32,y+rand()%17,
							0,double((rand()%4)+3) / -10.0,450,0.0,color);

	for (i=0;i<9;i++)//12
		Game::Particles->new_particle(	PARTICLE_LIGHT,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5.0,(rand()%7-rand()%7)/3.0,
							rand()%40+60,0.025,color);

	for (i=0;i<8;i++)//8//10
		Game::Particles->new_particle(	PARTICLE_SPARK,x+rand()%17-13,y+rand()%17-13,
							(rand()%3-rand()%3),//(rand()%7-rand()%7)/5,
							(rand()%7-rand()%7)/3,
							rand()%20+60,0.015,color);//50+60

	for (i=0;i<20;i++)//12
		Game::Particles->new_particle(	PARTICLE_POINT,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5.0,(rand()%7-rand()%7)/3.0,
							rand()%40+60,0.025,color);
}

void Smoke(DWORD x, DWORD y, BYTE color) {
	for (int i=0;i<3;i++)
		Game::Particles->new_particle(	PARTICLE_SMOKE,x+rand()%17-32,y+rand()%17,
							0,double((rand()%3)+3) / -10.0/*-0.3*/,450,0.0,color);
	for (int i=0;i<6;i++)
		Game::Particles->new_particle(	PARTICLE_DUST_CLOUDS,x+rand()%30-rand()%30-10,y+rand()%30-rand()%30+10,
							0,-0.3,rand()%50+60,0,color);
}

void SmokeClouds(DWORD x, DWORD y) {
	for (int i=0;i<5;i++)
		Game::Particles->new_particle(	PARTICLE_SMOKE,x+rand()%17-32,y+rand()%17,
							0,double((rand()%3)+3) / -10.0/*-0.3*/,450,0.0,0);
}

void Stars(DWORD x, DWORD y, BYTE color) {
	for (int i=0;i<5;i++)
		Game::Particles->new_particle(PARTICLE_STAR,x-5, y-5, (rand()%3-rand()%3)/1.5, rand()%3*-1,100,(rand()%5+5)/100.0/* 0.05*/,color);//300

	for (int i=0;i<3;i++)//12
		Game::Particles->new_particle(	PARTICLE_POINT,x-5, y-5, (rand()%3-rand()%3)/1.5, rand()%3*-1,100,(rand()%5+5)/100.0,color);
}

void Destruction(BYTE tehoste, DWORD x, DWORD y) {
	switch (tehoste){
		case TUHOUTUMINEN_HOYHENET			: Effect::Feathers(x, y); break;
		case TUHOUTUMINEN_TAHDET_HARMAA		: Effect::Stars(x,y,0); break;
		case TUHOUTUMINEN_TAHDET_SININEN	: Effect::Stars(x,y,32); break;
		case TUHOUTUMINEN_TAHDET_PUNAINEN	: Effect::Stars(x,y,64); break;
		case TUHOUTUMINEN_TAHDET_VIHREA		: Effect::Stars(x,y,96); break;
		case TUHOUTUMINEN_TAHDET_ORANSSI	: Effect::Stars(x,y,128); break;
		case TUHOUTUMINEN_TAHDET_VIOLETTI	: Effect::Stars(x,y,160); break;
		case TUHOUTUMINEN_TAHDET_TURKOOSI	: Effect::Stars(x,y,192); break;
		case TUHOUTUMINEN_RAJAHDYS_HARMAA	: Effect::Explosion(x,y,0); break;
		case TUHOUTUMINEN_RAJAHDYS_SININEN	: Effect::Explosion(x,y,32); break;
		case TUHOUTUMINEN_RAJAHDYS_PUNAINEN	: Effect::Explosion(x,y,64); break;
		case TUHOUTUMINEN_RAJAHDYS_VIHREA	: Effect::Explosion(x,y,96); break;
		case TUHOUTUMINEN_RAJAHDYS_ORANSSI	: Effect::Explosion(x,y,128); break;
		case TUHOUTUMINEN_RAJAHDYS_VIOLETTI	: Effect::Explosion(x,y,160); break;
		case TUHOUTUMINEN_RAJAHDYS_TURKOOSI	: Effect::Explosion(x,y,192); break;
		case TUHOUTUMINEN_SAVU_HARMAA		: Effect::Smoke(x,y,0); break;
		case TUHOUTUMINEN_SAVU_SININEN		: Effect::Smoke(x,y,32); break;
		case TUHOUTUMINEN_SAVU_PUNAINEN		: Effect::Smoke(x,y,64); break;
		case TUHOUTUMINEN_SAVU_VIHREA		: Effect::Smoke(x,y,96); break;
		case TUHOUTUMINEN_SAVU_ORANSSI		: Effect::Smoke(x,y,128); break;
		case TUHOUTUMINEN_SAVU_VIOLETTI		: Effect::Smoke(x,y,160); break;
		case TUHOUTUMINEN_SAVU_TURKOOSI		: Effect::Smoke(x,y,192); break;
		case TUHOUTUMINEN_SAVUPILVET		: Effect::SmokeClouds(x,y); break;
		default	: break;
	}
}

}

//==================================================
//(#7) Sprite Prototypes
//==================================================

void PK2::SpriteSystem::protot_clear_all() {
	for (int i=0; i< prototypes.size(); i++){
		for (int j=0;j<MAX_AANIA;j++)
			if (prototypes[i].aanet[j] > -1)
				PisteSound_FreeSFX(prototypes[i].aanet[j]);
		//prototypes[i].New();
		
		kartta->prototypes[i], "";
	}

	prototypes.clear();

	next_free_prototype = 0;
}

int  PK2::SpriteSystem::protot_get_sound(std::string path) {
	return PisteSound_LoadSFX(path);
}

int PK2::SpriteSystem::Get_Prototype(std::string filename) {
	//PK2Sprite_Prototype *p = new PK2Sprite_Prototype();

	prototypes.push_back(PK2Sprite_Prototype());
	PK2Sprite_Prototype* p = &prototypes[prototypes.size() - 1];

	int res = p->Load(filename, current_episode);

	if (res == 1) {
		next_free_prototype++;

		return 1;
	}

	/*
	if (prototypes[next_free_prototype].Load(filename, current_episode) == 1) {
		// This fixes the bug that is probably caused by the original editor, but it's a bad solution
		next_free_prototype++;

		return 1;
	}*/

	prototypes[next_free_prototype].indeksi = next_free_prototype;

	std::stringstream ss;

	//Load sounds
	for (int i = 0; i < MAX_AANIA; i++) {
		if (strcmp(prototypes[next_free_prototype].aanitiedostot[i], "") != 0) {
			ss.str("");
			ss << "./episodes/" << current_episode << "/sprites/" << prototypes[next_free_prototype].aanitiedostot[i];

			int sndIndex = -1;
			if ((sndIndex = PisteSound_LoadSFX(ss.str())) != -1) {
				prototypes[next_free_prototype].aanet[i] = sndIndex;
			} else {
				ss.str("");
				ss << "sprites/" << prototypes[next_free_prototype].aanitiedostot[i];

				if ((sndIndex = PisteSound_LoadSFX(ss.str())) != -1) {
					prototypes[next_free_prototype].aanet[i] = sndIndex;
				}
			}
		}
	}

	next_free_prototype++;

	return 0;
}

void PK2::SpriteSystem::protot_get_transformation(int i) {
	int j = 0;
	bool loaded = false;

	if (!prototypes[i].transformation_sprite.empty()){
		while (j< prototypes.size() && !loaded){
			if (j!=i) {
				if (prototypes[i].transformation_sprite == prototypes[j].file) {
					prototypes[i].muutos = j;
					loaded = true;
				}
			}
			j++;
		}

		if (!loaded) {
			if (Get_Prototype(prototypes[i].transformation_sprite) == 0)
				prototypes[i].muutos = next_free_prototype-1; // jokainen lataus kasvattaa next_free_prototype:a
		}
	}
}

void PK2::SpriteSystem::protot_get_bonus(int i) {
	int j = 0;
	bool loaded = false;

	for (int k = 0; k < 5; k++) {
		if (!prototypes[i].bonus_sprite[k].empty()) {
			while (j < prototypes.size() && !loaded) {
				if (j != i) {
					if (prototypes[i].bonus_sprite[k] == prototypes[j].file) {
						prototypes[i].bonus[k] = j;
						loaded = true;
					}
				}

				j++;
			}

			if (!loaded) {
				if (Get_Prototype(prototypes[i].bonus_sprite[k]) == 0)
					prototypes[i].bonus[k] = next_free_prototype - 1;
			}
		}
	}
}

void PK2::SpriteSystem::protot_get_ammo1(int i) {
	int j = 0;
	bool loaded = false;

	if (!prototypes[i].ammus1_sprite.empty()) {
		while (j < prototypes.size() && !loaded){
			if (j!=i){
				if (prototypes[i].ammus1_sprite == prototypes[j].file) {
					prototypes[i].ammus1 = j;
					loaded = true;
				}
			}

			j++;
		}

		if (!loaded){
			char polku[PE_PATH_SIZE];
			strcpy(polku,"sprites/");

			if (Get_Prototype(prototypes[i].ammus1_sprite) == 0)
				prototypes[i].ammus1 = next_free_prototype-1;
		}
	}
}

void PK2::SpriteSystem::protot_get_ammo2(int i) {
	int j = 0;
	bool loaded = false;

	if (!prototypes[i].ammus2_sprite.empty()) {
		while (j<prototypes.size() && !loaded){
			if (j!=i){
				if (prototypes[i].ammus2_sprite == prototypes[j].file){
					prototypes[i].ammus2 = j;
					loaded = true; //Prototype has already loaded
				}
			}

			j++;
		}

		if (!loaded){
			if (Get_Prototype(prototypes[i].ammus2_sprite) == 0)
				prototypes[i].ammus2 = next_free_prototype - 1;
		}
	}
}

int  PK2::SpriteSystem::protot_get_all() {
	char path[PE_PATH_SIZE];
	int viimeinen_proto = 0;

	for (int i=0;i < kartta->prototypes.size(); i++){
		viimeinen_proto = i;
		
		if (Get_Prototype(kartta->prototypes[i]) != 0) {
			PisteLog_Write("PK2Main", "PK2Main\t- Can't load sprite " + std::string(kartta->prototypes[i]) + ". It will not appear.", TYPE::T_ERROR);
			//next_free_prototype++;
		} else {
			//next_free_prototype++;
		}
	}

	next_free_prototype = viimeinen_proto + 1;

	for (int i = 0; i < prototypes.size(); i++){
		protot_get_transformation(i);
		protot_get_bonus(i);
		protot_get_ammo1(i);
		protot_get_ammo2(i);
	}

	return 0;
}

PK2::SpriteSystem::SpriteSystem() {
	sprites.reserve(1000);
	prototypes.reserve(1000);

	//clear();

}

PK2::SpriteSystem::~SpriteSystem() {

}

void PK2::SpriteSystem::add_bg(int index) {
	background_sprites.push_back(index);
	/*for (int i=0;i<MAX_SPRITEJA;i++){
		if (taustaspritet[i] == -1){
			taustaspritet[i] = index;
			return;
		}
	}*/
}

void PK2::SpriteSystem::sort_bg() {
	bool lopeta = false;
	int l = 1;
	int vali;

	while (l < background_sprites.size() && lopeta == false)
	{
		lopeta = true;

		for (int i = 0; i < background_sprites.size() - 1; i++)
		{
			/*
			if (background_sprites[i] == -1 || taustaspritet[i+1] == -1)
				i = MAX_SPRITEJA;
			else
			{*/
				if (sprites[background_sprites[i]].type->pallarx_kerroin > sprites[background_sprites[i+1]].type->pallarx_kerroin)
				{
					vali = background_sprites[i];
					background_sprites[i]   = background_sprites[i+1];
					background_sprites[i+1] = vali;
					lopeta = false;
				}
			//}
		}
		l++;
	}
}

void PK2::SpriteSystem::start_directions() {
	for (int i = 0; i < Game::Sprites->sprites.size(); i++){
		PK2Sprite* spr = &Game::Sprites->sprites[i];

		if (!spr->piilota) {	
			spr->a = 0;

			if (spr->type->Onko_AI(AI_RANDOM_ALOITUSSUUNTA_HORI)){
				while (spr->a == 0) {
					spr->a = ((rand()%2 - rand()%2) * spr->type->max_nopeus) / 3.5;//2;
				}
			}

			if (spr->type->Onko_AI(AI_RANDOM_ALOITUSSUUNTA_VERT)){
				while (spr->b == 0) {
					spr->b = ((rand()%2 - rand()%2) * spr->type->max_nopeus) / 3.5;//2;
				}
			}

			if (spr->type->Onko_AI(AI_ALOITUSSUUNTA_PELAAJAA_KOHTI)){
				if (spr->x < player->x)
					spr->a = spr->type->max_nopeus / 3.5;

				if (spr->x > player->x)
					spr->a = (spr->type->max_nopeus * -1) / 3.5;
			}

			if (spr->type->Onko_AI(AI_ALOITUSSUUNTA_PELAAJAA_KOHTI_VERT)){
				if (spr->y < player->y)
					spr->b = spr->type->max_nopeus / -3.5;

				if (spr->y > player->y)
					spr->b = spr->type->max_nopeus / 3.5;
			}
		}
	}
}

void PK2::SpriteSystem::add(int protoype_id, int is_player, double x, double y, int parent_sprite, bool isbonus) {
	PK2Sprite_Prototype* proto = &prototypes[protoype_id];

	Game::Sprites->sprites.push_back(PK2Sprite(proto, is_player, false, x, y));
	PK2Sprite* spr = &Game::Sprites->sprites[Game::Sprites->sprites.size() - 1];

	spr->originalX = x;
	spr->originalY = y;

	if (isbonus) { //If it is a bonus dropped by enemy
		spr->x += spr->type->leveys;
		spr->y += spr->type->korkeus / 2;
		spr->alku_x = spr->x;
		spr->alku_y = spr->y;
		spr->hyppy_ajastin = 1;
		spr->a = rand() % 2 - rand() % 4;
		spr->isku = 35;//25
	} else {
		spr->x = x + 16 + 1;
		spr->y += spr->type->korkeus / 2;
		spr->alku_x = spr->x;
		spr->alku_y = spr->y;
	}

	if (proto->type == type_TAUSTA)
		add_bg(background_sprites.size() + 1);

	if (parent_sprite != -1)
		spr->emosprite = parent_sprite;
	else
		spr->emosprite = sprites.size() - 1;
}

void PK2::SpriteSystem::add_ammo(int protoype_id, int is_player, double x, double y, int emo) {
	PK2Sprite_Prototype* proto = &prototypes[protoype_id];
	
	sprites.push_back(PK2Sprite(proto, is_player, false, x/*-proto.leveys/2*/, y));
	PK2Sprite& spr = sprites[sprites.size() - 1];

	//spritet[i].x += spritet[i].type->leveys;
	//spritet[i].y += spritet[i].type->korkeus/2;

	if (proto->Onko_AI(AI_HEITTOASE)) {
		if ((int) sprites[emo].a == 0) {
			// If the shooter is a player or the shooter has a speed of zero
			if (sprites[emo].pelaaja == 1 || spr.type->max_nopeus == 0) {
				if (!sprites[emo].flip_x)
					spr.a = spr.type->max_nopeus;
				else
					spr.a = -spr.type->max_nopeus;
			} else { // tai jos kyseess� on vihollinen
				if (!sprites[emo].flip_x)
					spr.a = 1 + rand() % (int)spr.type->max_nopeus;
				else
					spr.a = -1 - rand() % -(int)spr.type->max_nopeus;
			}
		} else {
			if (!sprites[emo].flip_x)
				spr.a = spr.type->max_nopeus + sprites[emo].a;
			else
				spr.a = -spr.type->max_nopeus + sprites[emo].a;

			//spritet[i].a = spritet[emo].a * 1.5;

		}

		spr.hyppy_ajastin = 1;
	} else
		if (proto->Onko_AI(AI_MUNA)) {
			spr.y = sprites[emo].y + 10;
			spr.a = sprites[emo].a / 1.5;
		} else {
			if (!sprites[emo].flip_x)
				spr.a = spr.type->max_nopeus;
			else
				spr.a = -spr.type->max_nopeus;
		}

	if (emo != -1) {
		spr.emosprite = emo;
		spr.vihollinen = sprites[emo].vihollinen;
	} else {
		spr.emosprite = sprites.size() - 1;
	}

	if (proto->type == type_TAUSTA)
		add_bg(sprites.size() - 1);
		//add_bg(i);

	/*bool lisatty = false;
	int i = 0;

	while (!lisatty && i < MAX_SPRITEJA){
		if (spritet[i].piilota){
			spritet[i] = PK2Sprite(proto,is_player,false,x/*-proto.leveys/2,y);

			//spritet[i].x += spritet[i].type->leveys;
			//spritet[i].y += spritet[i].type->korkeus/2;

			if (proto->Onko_AI(AI_HEITTOASE)){
				if ((int)spritet[emo].a == 0){
					// Jos "ampuja" on pelaaja tai ammuksen nopeus on nolla
					if (spritet[emo].pelaaja == 1 || spritet[i].type->max_nopeus == 0){
						if (!spritet[emo].flip_x)
							spritet[i].a = spritet[i].type->max_nopeus;
						else
							spritet[i].a = -spritet[i].type->max_nopeus;
					}
					else{ // tai jos kyseess� on vihollinen
						if (!spritet[emo].flip_x)
							spritet[i].a = 1 + rand()%(int)spritet[i].type->max_nopeus;
						else
							spritet[i].a = -1 - rand()%-(int)spritet[i].type->max_nopeus;
					}
				}
				else{
					if (!spritet[emo].flip_x)
						spritet[i].a = spritet[i].type->max_nopeus + spritet[emo].a;
					else
						spritet[i].a = -spritet[i].type->max_nopeus + spritet[emo].a;

					//spritet[i].a = spritet[emo].a * 1.5;

				}

				spritet[i].hyppy_ajastin = 1;
			}
			else
			if (proto->Onko_AI(AI_MUNA)){
				spritet[i].y = spritet[emo].y+10;
				spritet[i].a = spritet[emo].a / 1.5;
			}
			else{
				if (!spritet[emo].flip_x)
					spritet[i].a = spritet[i].type->max_nopeus;
				else
					spritet[i].a = -spritet[i].type->max_nopeus;
			}

			if (emo != MAX_SPRITEJA){
				spritet[i].emosprite = emo;
				spritet[i].vihollinen = spritet[emo].vihollinen;
			}
			else{
				spritet[i].emosprite = i;
			}

			if (proto->type == type_TAUSTA)
				add_bg(i);

			lisatty = true;
		}
		else
			i++;
	}*/
}

void PK2::SpriteSystem::clear() {
	/*
	int i = 0;
	for (int i = 0; i < prototypes.size(); i++) {
		delete prototypes[i];
		prototypes[i] = nullptr;

		std::cout << "deleted" << std::endl;
	}

	prototypes.clear();
	*/

	sprites.clear();

	background_sprites.clear();

	/*	while (i < MAX_SPRITEJA){
		taustaspritet[i] = -1;
		i++;
	}*/
}

//==================================================
//(#9) Map
//==================================================

int PK_Map_Open(char* nimi) {

	Game::Sprites->protot_clear_all();
	Game::Sprites->clear();

	char polku[PE_PATH_SIZE];
	strcpy(polku, "");
	PK_Load_EpisodeDir(polku);

	if (kartta->Load(nimi, current_episode) == 1) {
		return 1;
	}

	switch1_time = kartta->kytkin1_aika;
	switch2_time = kartta->kytkin2_aika;
	switch3_time = kartta->kytkin3_aika;

	PK_New_Save();

	// TODO Create function in map.cpp to check for correct version
	if (strcmp(kartta->versio, "1.2") == 0 || strcmp(kartta->versio, "1.3") == 0 || strcmp(kartta->versio, "1.4") == 0) {
		if (Game::Sprites->protot_get_all() == 1) {
			return 1;
		}
	}

	PK_Palikka_Tee_Maskit();

	if (PK_Clean_TileBuffer()==1)
		return 1;

	Game::Sprites->player = &Game::Sprites->sprites[0];

	kartta->Place_Sprites();
	kartta->Select_Start();
	avaimia = kartta->Count_Keys();
	kartta->Calculate_Edges();

	Game::Sprites->start_directions();

	Game::Particles->clear_particles();
	Game::Particles->load_bg_particles(kartta);

	int collectables = 0;

	for (int i = 0; i < Game::Sprites->sprites.size(); i++) {
		if (Game::Sprites->sprites[i].type->type == TYPE_COLLECTABLE) {
			collectables++;
		}
	}

	if (collectables > 0) {
		show_collectables = true;
	}

	if (!kartta->musiikki.empty()) {
		std::stringstream ss;
		ss << "episodes/" << current_episode << "/music/" << kartta->musiikki;

		if (!test_mute) {
			if (PisteSound_StartMusic(ss.str()) != 0) {
				ss.str("");
				ss << "music/" << kartta->musiikki;

				if (PisteSound_StartMusic(ss.str()) != 0) {
					ss.str("");
					ss << "Couldn't find music file \"" << kartta->musiikki << "\", trying to load song01.xm";

					PisteLog_Write("PK2Main", ss.str(), TYPE::T_ERROR);

					if (PisteSound_StartMusic("music/song01.xm") != 0) {
						PisteLog_Write("PK2Main", "Couldn't find song01.xm or custom music!", TYPE::T_ERROR);
					}
				}
			}
		}
	}

	return 0;
}

//==================================================
//(#10) Blocks
//==================================================

void     PK_Block_Set_Barriers(PK2BLOCK &palikka) {
	palikka.tausta = false;

	palikka.oikealle	= BLOCK_SEINA;
	palikka.vasemmalle	= BLOCK_SEINA;
	palikka.ylos		= BLOCK_SEINA;
	palikka.alas		= BLOCK_SEINA;

	// Special Floor

	if (palikka.koodi > 139){
		palikka.oikealle	= BLOCK_TAUSTA;
		palikka.vasemmalle	= BLOCK_TAUSTA;
		palikka.ylos		= BLOCK_TAUSTA;
		palikka.alas		= BLOCK_TAUSTA;
	}

	// Lifts

	if (palikka.koodi == BLOCK_HISSI_HORI){
		palikka.vasen += (int)cos_table[degree%360];
		palikka.oikea += (int)cos_table[degree%360];
	}
	if (palikka.koodi == BLOCK_HISSI_VERT){
		palikka.ala += (int)sin_table[degree%360];
		palikka.yla += (int)sin_table[degree%360];
	}

	// Walk-through Floor

	if (palikka.koodi == BLOCK_ESTO_ALAS){
		palikka.oikealle	= BLOCK_TAUSTA;
		palikka.ylos		= BLOCK_TAUSTA;
		palikka.alas		= BLOCK_SEINA;
		palikka.vasemmalle	= BLOCK_TAUSTA;
		palikka.ala -= 27;
	}

	// Hill

	if (palikka.koodi > 49 && palikka.koodi < 60){
		palikka.oikealle	= BLOCK_TAUSTA;
		palikka.ylos		= BLOCK_SEINA;
		palikka.alas		= BLOCK_SEINA;
		palikka.vasemmalle	= BLOCK_TAUSTA;
		palikka.ala += 1;
	}

	// Switches

	if (palikka.koodi >= BLOCK_KYTKIN1 && palikka.koodi <= BLOCK_KYTKIN3){
		palikka.oikealle	= BLOCK_SEINA;
		palikka.ylos		= BLOCK_SEINA;
		palikka.alas		= BLOCK_SEINA;
		palikka.vasemmalle	= BLOCK_SEINA;
	}

	// Switches Affected Floors

	int kytkin1_y = 0,
		kytkin2_y = 0,
		kytkin3_x = 0;

	if (kytkin1 > 0){
		kytkin1_y = 64;

		if (kytkin1 < 64)
			kytkin1_y = kytkin1;

		if (kytkin1 > switch1_time - 64)
			kytkin1_y = switch1_time - kytkin1;
	}

	if (kytkin2 > 0){
		kytkin2_y = 64;

		if (kytkin2 < 64)
			kytkin2_y = kytkin2;

		if (kytkin2 > switch2_time - 64)
			kytkin2_y = switch2_time - kytkin2;
	}

	if (kytkin3 > 0){
		kytkin3_x = 64;

		if (kytkin3 < 64)
			kytkin3_x = kytkin3;

		if (kytkin3 > switch3_time - 64)
			kytkin3_x = switch3_time - kytkin3;
	}


	if (palikka.koodi == BLOCK_KYTKIN2_YLOS){
		palikka.ala -= kytkin2_y/2;
		palikka.yla -= kytkin2_y/2;
	}

	if (palikka.koodi == BLOCK_KYTKIN2_ALAS){
		palikka.ala += kytkin2_y/2;
		palikka.yla += kytkin2_y/2;
	}

	if (palikka.koodi == BLOCK_KYTKIN2){
		palikka.ala += kytkin2_y/2;
		palikka.yla += kytkin2_y/2;
	}

	if (palikka.koodi == BLOCK_KYTKIN3_OIKEALLE){
		palikka.oikea += kytkin3_x/2;
		palikka.vasen += kytkin3_x/2;
		palikka.koodi = BLOCK_HISSI_HORI; // samat idea sivusuuntaan ty�nn�ss�
	}

	if (palikka.koodi == BLOCK_KYTKIN3_VASEMMALLE){
		palikka.oikea -= kytkin3_x/2;
		palikka.vasen -= kytkin3_x/2;
		palikka.koodi = BLOCK_HISSI_HORI; // samat idea sivusuuntaan ty�nn�ss�
	}

	if (palikka.koodi == BLOCK_KYTKIN3){
		palikka.ala += kytkin3_x/2;
		palikka.yla += kytkin3_x/2;
	}

	if (palikka.koodi == BLOCK_KYTKIN1){
		palikka.ala += kytkin1_y/2;
		palikka.yla += kytkin1_y/2;
	}

}

PK2BLOCK PK_Block_Get(int x, int y) {
	PK2BLOCK palikka;

	if (x < 0 || x > PK2KARTTA_KARTTA_LEVEYS || y < 0 || y > PK2KARTTA_KARTTA_LEVEYS) {
		palikka.koodi  = 255;
		palikka.tausta = true;
		palikka.vasen  = x*32;
		palikka.oikea  = x*32+32;
		palikka.yla	   = y*32;
		palikka.ala    = y*32+32;
		palikka.vesi   = false;
		palikka.reuna  = true;
		return palikka;
	}

	BYTE i = kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS];

	if (i<150){ //If it is ground
		palikka = lasketut_palikat[i];
		palikka.vasen  = x*32+lasketut_palikat[i].vasen;
		palikka.oikea  = x*32+32+lasketut_palikat[i].oikea;
		palikka.yla	   = y*32+lasketut_palikat[i].yla;
		palikka.ala    = y*32+32+lasketut_palikat[i].ala;
	}
	else{ //If it is sky - Need to reset
		palikka.koodi  = 255;
		palikka.tausta = true;
		palikka.vasen  = x*32;
		palikka.oikea  = x*32+32;
		palikka.yla	   = y*32;
		palikka.ala    = y*32+32;
		palikka.vesi   = false;

		palikka.vasemmalle = 0;
		palikka.oikealle = 0;
		palikka.ylos = 0;
		palikka.alas = 0;
	}

	i = kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS];

	if (i>131 && i<140)
		palikka.vesi = true;

	palikka.reuna = kartta->reunat[x+y*PK2KARTTA_KARTTA_LEVEYS];


	return palikka;
}

void     PK_Calculate_MovableBlocks_Position() {
	lasketut_palikat[BLOCK_HISSI_HORI].vasen = (int)cos_table[degree%360];
	lasketut_palikat[BLOCK_HISSI_HORI].oikea = (int)cos_table[degree%360];

	lasketut_palikat[BLOCK_HISSI_VERT].ala = (int)sin_table[degree%360];
	lasketut_palikat[BLOCK_HISSI_VERT].yla = (int)sin_table[degree%360];

	int kytkin1_y = 0,
		kytkin2_y = 0,
		kytkin3_x = 0;

	if (kytkin1 > 0)
	{
		kytkin1_y = 64;

		if (kytkin1 < 64)
			kytkin1_y = kytkin1;

		if (kytkin1 > switch1_time - 64)
			kytkin1_y = switch1_time - kytkin1;
	}

	if (kytkin2 > 0)
	{
		kytkin2_y = 64;

		if (kytkin2 < 64)
			kytkin2_y = kytkin2;

		if (kytkin2 > switch2_time - 64)
			kytkin2_y = switch2_time - kytkin2;
	}

	if (kytkin3 > 0)
	{
		kytkin3_x = 64;

		if (kytkin3 < 64)
			kytkin3_x = kytkin3;

		if (kytkin3 > switch3_time - 64)
			kytkin3_x = switch3_time - kytkin3;
	}

	kytkin1_y /= 2;
	kytkin2_y /= 2;
	kytkin3_x /= 2;

	lasketut_palikat[BLOCK_KYTKIN1].ala = kytkin1_y;
	lasketut_palikat[BLOCK_KYTKIN1].yla = kytkin1_y;

	lasketut_palikat[BLOCK_KYTKIN2_YLOS].ala = -kytkin2_y;
	lasketut_palikat[BLOCK_KYTKIN2_YLOS].yla = -kytkin2_y;

	lasketut_palikat[BLOCK_KYTKIN2_ALAS].ala = kytkin2_y;
	lasketut_palikat[BLOCK_KYTKIN2_ALAS].yla = kytkin2_y;

	lasketut_palikat[BLOCK_KYTKIN2].ala = kytkin2_y;
	lasketut_palikat[BLOCK_KYTKIN2].yla = kytkin2_y;

	lasketut_palikat[BLOCK_KYTKIN3_OIKEALLE].oikea = kytkin3_x;
	lasketut_palikat[BLOCK_KYTKIN3_OIKEALLE].vasen = kytkin3_x;
	lasketut_palikat[BLOCK_KYTKIN3_OIKEALLE].koodi = BLOCK_HISSI_HORI;

	lasketut_palikat[BLOCK_KYTKIN3_VASEMMALLE].oikea = -kytkin3_x;
	lasketut_palikat[BLOCK_KYTKIN3_VASEMMALLE].vasen = -kytkin3_x;
	lasketut_palikat[BLOCK_KYTKIN3_VASEMMALLE].koodi = BLOCK_HISSI_HORI;

	lasketut_palikat[BLOCK_KYTKIN3].ala = kytkin3_x;
	lasketut_palikat[BLOCK_KYTKIN3].yla = kytkin3_x;
}

int      PK_Calculate_Tiles() {
	PK2BLOCK palikka;

	for (int i=0;i<150;i++){
		palikka = lasketut_palikat[i];

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

		lasketut_palikat[i] = palikka;
	}

	PK_Calculate_MovableBlocks_Position();

	return 0;
}

//==================================================
//(#11) Gifts
//==================================================



int PK2::GiftSystem::count() {
	return gift_count;
}

int PK2::GiftSystem::get(int i) {
	return list[i];
}

void PK2::GiftSystem::set(int index, int value) {
	list[index] = value;
}

PK2Sprite_Prototype* PK2::GiftSystem::get_protot(int i) {
	return &Game::Sprites->prototypes[list[i]];
}

void PK2::GiftSystem::draw(int i, int x, int y) {
	PK2Sprite_Prototype* prot = &Game::Sprites->prototypes[ list[i] ];
	prot->Piirra(x - prot->leveys / 2, y - prot->korkeus / 2, 0);
}

PK2::GiftSystem::GiftSystem() {
	clean();
}

PK2::GiftSystem::~GiftSystem() {}

void PK2::GiftSystem::clean() {
	for (int i=0;i<MAX_GIFTS;i++)
		list[i] = -1;
	gift_count = 0;
}

bool PK2::GiftSystem::add(int PROTOTYPE_id) {
	int i=0;
	bool lisatty = false;

	std::string text = tekstit->Hae_Teksti(PK_txt.game_newitem);
	
	while (i<MAX_GIFTS && !lisatty)
	{
		if (list[i] == -1)
		{
			lisatty = true;
			list[i] = PROTOTYPE_id;
			
			//strcat(ilmo,protot[PROTOTYPE_id]->nimi);
			PK_Start_Info(text);
			gift_count++;
		}
		i++;
	}
	return lisatty;
}

int  PK2::GiftSystem::use() {
	if (gift_count > 0) {
		Game::Sprites->add(
			list[0], 0,
			Game::Sprites->player->x - Game::Sprites->prototypes[list[0]].leveys,
			Game::Sprites->player->y,
			-1, false);

		for (int i = 0; i < MAX_GIFTS - 1; i++)
			list[i] = list[i+1];

		list[MAX_GIFTS-1] = -1;

		gift_count--;
	}

	return 0;
}

int  PK2::GiftSystem::change_order() {
	if (list[0] == -1)
		return 0;

	int temp = list[0];

	for (int i=0;i<MAX_GIFTS-1;i++)
		list[i] = list[i+1];

	int count = 0;

	while(count < MAX_GIFTS-1 && list[count] != -1)
		count++;

	list[count] = temp;

	return 0;
}

//==================================================
//(#12) Collision System
//==================================================

	double	sprite_x,
			sprite_y,
			sprite_a,
			sprite_b,

			sprite_vasen,
			sprite_oikea,
			sprite_yla,
			sprite_ala;

	int		sprite_leveys,
			sprite_korkeus;

	int		kartta_vasen,
			kartta_yla,
			x = 0,
			y = 0;

	bool	oikealle,
			vasemmalle,
			ylos,
			alas,

			vedessa;

	BYTE   max_nopeus;

void PK_Check_Blocks2(PK2Sprite &sprite, PK2BLOCK &palikka) {

	//left and right
	if (sprite_yla < palikka.ala && sprite_ala-1 > palikka.yla){
		if (sprite_oikea+sprite_a-1 > palikka.vasen && sprite_vasen+sprite_a < palikka.oikea){
			// Tutkitaan onko sprite menossa oikeanpuoleisen palikan sis��n.
			if (sprite_oikea+sprite_a < palikka.oikea){
				// Onko palikka sein�?
				if (palikka.oikealle == BLOCK_SEINA){
					oikealle = false;
					if (palikka.koodi == BLOCK_HISSI_HORI)
						sprite_x = palikka.vasen - sprite_leveys/2;
				}
			}

			if (sprite_vasen+sprite_a > palikka.vasen){
				if (palikka.vasemmalle == BLOCK_SEINA){
					vasemmalle = false;

					if (palikka.koodi == BLOCK_HISSI_HORI)
						sprite_x = palikka.oikea + sprite_leveys/2;

				}
			}
		}
	}

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;

	//ceil and floor

	if (sprite_vasen < palikka.oikea && sprite_oikea-1 > palikka.vasen){
		if (sprite_ala+sprite_b-1 >= palikka.yla && sprite_yla+sprite_b <= palikka.ala){
			if (sprite_ala+sprite_b-1 <= palikka.ala){
				if (palikka.alas == BLOCK_SEINA){
					alas = false;

					if (palikka.koodi == BLOCK_HISSI_VERT)
						sprite_y = palikka.yla - sprite_korkeus /2;

					if (sprite_ala-1 >= palikka.yla && sprite_b >= 0)
						if (palikka.koodi != BLOCK_HISSI_HORI)
							sprite_y = palikka.yla - sprite_korkeus /2;
				}
			}

			if (sprite_yla+sprite_b > palikka.yla){
				if (palikka.ylos == BLOCK_SEINA){
					ylos = false;

					if (sprite_yla < palikka.ala)
						if (palikka.koodi != BLOCK_HISSI_HORI)
							sprite.kyykky = true;
				}
			}
		}
	}
}

void PK_Check_Blocks(PK2Sprite &sprite, PK2BLOCK &palikka) {
	int mask_index;

	//If sprite is in the block
	if (sprite_x <= palikka.oikea && sprite_x >= palikka.vasen && sprite_y <= palikka.ala && sprite_y >= palikka.yla){

		/**********************************************************************/
		/* Examine if block is water background                               */
		/**********************************************************************/
		if (palikka.vesi)
			sprite.vedessa = true;

		/**********************************************************************/
		/* Examine if it touches the fire                                     */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_TULI && kytkin1 == 0 && sprite.isku == 0){
			sprite.saatu_vahinko = 2;
			sprite.saatu_vahinko_type = VAHINKO_TULI;
		}

		/**********************************************************************/
		/* Examine if bloc is hideway                                         */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_PIILO)
			sprite.piilossa = true;

		/**********************************************************************/
		/* Examine if block is the exit                                       */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_LOPETUS && sprite.pelaaja != 0){
			if (!jakso_lapaisty){
				std::string files[] = {
					"hiscore.ogg",
					"hiscore.mp3"
					"hiscore.xm",
				};

				std::string hiscore_music = "music/hiscore.xm";

				for (int i = 0; i < 2; i++) {
					if (PisteUtils_File_Exists(current_episode + "/music/" + files[i])) {
						hiscore_music = current_episode + "/music/" + files[i];

						break;
					}
				}

				if (PisteSound_StartMusic(hiscore_music) != 0){
					PisteLog_Write("PK2Main", "Couldn't find hiscore music!", TYPE::T_ERROR);
				}

				jakso_lapaisty = true;
				
				if (levels[jakso_indeksi_nyt].is_secret) {
					level_stats.current_secret_level++;
					level_stats.level_secret_passed[jakso_indeksi_nyt] = true;
				} else {
					level_stats.level_passed[jakso_indeksi_nyt] = true;
				}

				for (auto& vv : level_stats.collectables_index) {
					levels[jakso_indeksi_nyt].collectables_index.push_back(vv);
				}

				level_stats.collectables_index.clear();

				// TODO Remove
				levels[jakso_indeksi_nyt].passed = true;

				//level_stats.collectables_collected[jakso_indeksi_nyt] = levels[jakso_indeksi_nyt].collectables_collected;
	
				if (levels[jakso_indeksi_nyt].level_number == level_stats.current_level)
					if (!levels[jakso_indeksi_nyt].is_secret) {
						level_stats.current_level++;
						
						// TODO REMOVE
						jakso++; //Increase level
					} else {
						// TODO REMOVE
						jakso = jakso_indeksi_nyt;

						level_stats.current_level = jakso_indeksi_nyt;
					}

				delete checkpoint_data;
				checkpoint_data = nullptr;

				music_volume = settings.music_max_volume;
				music_volume_now = settings.music_max_volume - 1;
			}
		}
	}

	//If sprite is thouching the block
	if (sprite_vasen <= palikka.oikea-4 && sprite_oikea >= palikka.vasen+4 && sprite_yla <= palikka.ala && sprite_ala >= palikka.yla+16){
		/**********************************************************************/
		/* Examine if it touches the fire                                     */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_TULI && kytkin1 == 0 && sprite.isku == 0){
			sprite.saatu_vahinko = 2;
			sprite.saatu_vahinko_type = VAHINKO_TULI;
		}
	}

	//Examine if there is a block on bottom
	if ((palikka.koodi<80 || palikka.koodi>139) && palikka.koodi != BLOCK_ESTO_ALAS && palikka.koodi < 150){
		mask_index = (int)(sprite_x+sprite_a) - palikka.vasen;

		if (mask_index < 0)
			mask_index = 0;

		if (mask_index > 31)
			mask_index = 31;

		palikka.yla += palikkamaskit[palikka.koodi].alas[mask_index];

		if (palikka.yla >= palikka.ala-2)
			palikka.alas = BLOCK_TAUSTA;

		palikka.ala -= palikkamaskit[palikka.koodi].ylos[mask_index];
	}

	//If sprite is thouching the block (again?)
	if (sprite_vasen <= palikka.oikea+2 && sprite_oikea >= palikka.vasen-2 && sprite_yla <= palikka.ala && sprite_ala >= palikka.yla){
		/**********************************************************************/
		/* Examine if it is a key and touches lock wall                       */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_LUKKO && sprite.type->avain){
			kartta->seinat[palikka.vasen/32+(palikka.yla/32)*PK2KARTTA_KARTTA_LEVEYS] = 255;
			kartta->Calculate_Edges();

			sprite.piilota = true;

			if (sprite.type->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU) {
				avaimia--;
				if (avaimia < 1)
					kartta->Open_Locks();
			}

			Effect::Explosion(palikka.vasen+16, palikka.yla+10, 0);
			PK_Play_Sound(avaa_lukko_aani,100, (int)sprite_x, (int)sprite_y, SOUND_SAMPLERATE, false);
		}

		/**********************************************************************/
		/* Make wind effects                                                  */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_VIRTA_VASEMMALLE && vasemmalle)
			sprite_a -= 0.02;

		if (palikka.koodi == BLOCK_VIRTA_OIKEALLE && oikealle)
			sprite_a += 0.02;	//0.05

		/*********************************************************************/
		/* Examine if sprite is on the border to fall                        */
		/*********************************************************************/
		if (palikka.reuna && sprite.hyppy_ajastin <= 0 && sprite_y < palikka.ala && sprite_y > palikka.yla){
			/* && sprite_ala <= palikka.ala+2)*/ // onko sprite tullut reunalle
			if (sprite_vasen > palikka.vasen)
				sprite.reuna_vasemmalla = true;

			if (sprite_oikea < palikka.oikea)
				sprite.reuna_oikealla = true;
		}
	}

	//Examine walls on left and right

	if (sprite_yla < palikka.ala && sprite_ala-1 > palikka.yla) {
		if (sprite_oikea+sprite_a-1 > palikka.vasen && sprite_vasen+sprite_a < palikka.oikea) {
			// Examine whether the sprite going in the right side of the block.
			if (sprite_oikea+sprite_a < palikka.oikea) {
				// Onko palikka sein�?
				if (palikka.oikealle == BLOCK_SEINA) {
					oikealle = false;

					if (palikka.koodi == BLOCK_HISSI_HORI)
						sprite_x = palikka.vasen - sprite_leveys/2;
				}
			}
			// Examine whether the sprite going in the left side of the block.
			if (sprite_vasen+sprite_a > palikka.vasen) {
				if (palikka.vasemmalle == BLOCK_SEINA) {
					vasemmalle = false;

					if (palikka.koodi == BLOCK_HISSI_HORI)
						sprite_x = palikka.oikea + sprite_leveys/2;

				}
			}
		}
	}

	sprite_vasen = sprite_x - sprite_leveys/2;
	sprite_oikea = sprite_x + sprite_leveys/2;

	//Examine walls on up and down

	if (sprite_vasen < palikka.oikea && sprite_oikea-1 > palikka.vasen) { //Remove the left and right blocks
		if (sprite_ala+sprite_b-1 >= palikka.yla && sprite_yla+sprite_b <= palikka.ala) { //Get the up and down blocks
			if (sprite_ala+sprite_b-1 <= palikka.ala) { //Just in the sprite's foot
				if (palikka.alas == BLOCK_SEINA) { //If it is a wall
					alas = false;
					if (palikka.koodi == BLOCK_HISSI_VERT)
						sprite_y = palikka.yla - sprite_korkeus /2;

					if (sprite_ala-1 >= palikka.yla && sprite_b >= 0) {
						//sprite_y -= sprite_ala - palikka.yla;
						if (palikka.koodi != BLOCK_HISSI_HORI) {
							sprite_y = palikka.yla - sprite_korkeus /2;
						}
					}

					if (sprite.kytkinpaino >= 1) { // Sprite can press the buttons
						if (palikka.koodi == BLOCK_KYTKIN1 && kytkin1 == 0) {
							kytkin1 = switch1_time;
							kytkin_tarina = 64;
							PK_Play_Sound(kytkin_aani, 100, (int)sprite_x, (int)sprite_y, SOUND_SAMPLERATE, false);
						}

						if (palikka.koodi == BLOCK_KYTKIN2 && kytkin2 == 0) {
							kytkin2 = switch2_time;
							kytkin_tarina = 64;
							PK_Play_Sound(kytkin_aani, 100, (int)sprite_x, (int)sprite_y, SOUND_SAMPLERATE, false);
						}

						if (palikka.koodi == BLOCK_KYTKIN3 && kytkin3 == 0) {
							kytkin3 = switch3_time;
							kytkin_tarina = 64;
							PK_Play_Sound(kytkin_aani, 100, (int)sprite_x, (int)sprite_y, SOUND_SAMPLERATE, false);
						}
					}

				}
			}

			if (sprite_yla+sprite_b > palikka.yla) {
				if (palikka.ylos == BLOCK_SEINA) {
					ylos = false;

					if (sprite_yla < palikka.ala) {
						if (palikka.koodi == BLOCK_HISSI_VERT && sprite.kyykky) {
							sprite.saatu_vahinko = 2;
							sprite.saatu_vahinko_type = VAHINKO_ISKU;
						}

						if (palikka.koodi != BLOCK_HISSI_HORI) {
							//if (sprite.kyykky)
							//	sprite_y = palikka.ala + sprite_korkeus /2;

							sprite.kyykky = true;
						}
					}
				}
			}
		}
	}
}

void PK_Checkpoint_Create(int x, int y) {
	if (checkpoint_data != nullptr) {
		delete checkpoint_data;
		checkpoint_data = nullptr;
	}

	checkpoint_data = new PK2CheckpointStruct;

	std::copy(std::begin(kartta->taustat), std::end(kartta->taustat), std::begin(checkpoint_data->layer_background));
	std::copy(std::begin(kartta->seinat), std::end(kartta->seinat), std::begin(checkpoint_data->layer_foreground));

	checkpoint_data->sprites = Game::Sprites->sprites;
	//std::copy(std::begin(Game::Sprites->sprites), std::end(Game::Sprites->sprites), std::begin(checkpoint_data->sprites));

	checkpoint_data->player_energy = Game::Sprites->player->energia;

	checkpoint_data->startX = x;
	checkpoint_data->startY = y;

	checkpoint_data->keys = avaimia;
	checkpoint_data->time = sekunti;
	checkpoint_data->timeout = timeout;

	checkpoint_data->score = jakso_pisteet;

	checkpoint_data->switch1_timer = kytkin1;
	checkpoint_data->switch2_timer = kytkin2;
	checkpoint_data->switch3_timer = kytkin3;

	checkpoint_data->collectables_collected = levels[jakso_indeksi_nyt].collectables_collected;

	std::copy(std::begin(Game::Gifts->list), std::end(Game::Gifts->list), std::begin(checkpoint_data->gifts));
	checkpoint_data->gift_amount = Game::Gifts->gift_count;
}

void PK_Checkpoint_Restore() {
	std::copy(std::begin(checkpoint_data->layer_background), std::end(checkpoint_data->layer_background), std::begin(kartta->taustat));
	std::copy(std::begin(checkpoint_data->layer_foreground), std::end(checkpoint_data->layer_foreground), std::begin(kartta->seinat));

	Game::Sprites->clear();
	//Game::Sprites->sprites.clear();
	Game::Sprites->sprites = checkpoint_data->sprites;
	//std::copy(std::begin(checkpoint_data->sprites), std::end(checkpoint_data->sprites), std::begin(Game::Sprites->sprites));

	std::copy(std::begin(checkpoint_data->gifts), std::end(checkpoint_data->gifts), std::begin(Game::Gifts->list));

	Game::Gifts->gift_count = checkpoint_data->gift_amount;

	Game::Sprites->player->energia = checkpoint_data->player_energy;

	Game::Sprites->player->x = checkpoint_data->startX;
	Game::Sprites->player->y = checkpoint_data->startY;

	Game::Sprites->player->a = 0;
	Game::Sprites->player->b = 0;

	Game::camera_x = checkpoint_data->startX;
	Game::camera_y = checkpoint_data->startY;
	Game::dcamera_x = Game::camera_x - screen_width / 2;
	Game::dcamera_y = Game::camera_y - screen_height / 2;

	avaimia = checkpoint_data->keys;
	sekunti = checkpoint_data->time;
	timeout = checkpoint_data->timeout;

	jakso_pisteet = checkpoint_data->score;

	kytkin1 = checkpoint_data->switch1_timer;
	kytkin2 = checkpoint_data->switch2_timer;
	kytkin3 = checkpoint_data->switch3_timer;

	levels[jakso_indeksi_nyt].collectables_collected = checkpoint_data->collectables_collected;
	
	Game::Particles->clear_particles();

	/*
	delete checkpoint_data;
	checkpoint_data = nullptr;*/
}

int PK_Sprite_Movement(int i){
	if (i < 0)
		return -1;

	PK2Sprite *sprite = &Game::Sprites->sprites[i]; 

	if (!sprite->type) {
		return -1;
	}

	sprite_x = sprite->x;
	sprite_y = sprite->y;
	sprite_a = sprite->a;
	sprite_b = sprite->b;

	sprite_leveys = sprite->type->leveys;
	sprite_korkeus = sprite->type->korkeus;

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;
	sprite_yla	 = sprite_y-sprite_korkeus/2;
	sprite_ala	 = sprite_y+sprite_korkeus/2;

	max_nopeus = (BYTE)sprite->type->max_nopeus;

	vedessa = sprite->vedessa;

	x = 0;
	y = 0;

	oikealle	 = true,
	vasemmalle	 = true,
	ylos		 = true,
	alas		 = true;

	kartta_vasen = 0;
	kartta_yla   = 0;

	sprite->kyykky = false;

	sprite->reuna_vasemmalla = false;
	sprite->reuna_oikealla = false;


	/* Pistet��n vauhtia tainnutettuihin spriteihin */
	if (sprite->energia < 1)
		max_nopeus = 3;

	// Calculate the remainder of the sprite towards

	if (sprite->hyokkays1 > 0)
		sprite->hyokkays1--;

	if (sprite->hyokkays2 > 0)
		sprite->hyokkays2--;

	if (sprite->lataus > 0)	// aika kahden ampumisen (munimisen) v�lill�
		sprite->lataus --;

	if (sprite->transformation_ajastin > 0)	// aika muutokseen
		sprite->transformation_ajastin --;

	// Player-sprite and its controls
	bool lisavauhti = true;
	bool hidastus = false;

	PisteInput_Lue_Eventti();

	if (sprite->pelaaja != 0 && sprite->energia > 0){
		/* SLOW WALK */
		if (PisteInput_Keydown(settings.control_walk_slow) || PisteInput_Gamepad_Button(settings.gamepad_walk_slow))
			lisavauhti = false;

		/* ATTACK 1 */
		if ((PisteInput_Keydown(settings.control_attack1) || PisteInput_Gamepad_Button(settings.gamepad_attack1)) && sprite->lataus == 0 && sprite->ammus1 != -1)
			sprite->hyokkays1 = sprite->type->hyokkays1_aika;
		/* ATTACK 2 */
		else if ((PisteInput_Keydown(settings.control_attack2) || PisteInput_Gamepad_Button(settings.gamepad_attack2)) && sprite->lataus == 0 && sprite->ammus2 != -1)
				sprite->hyokkays2 = sprite->type->hyokkays2_aika;

		/* CROUCH */
		sprite->kyykky = false;
		if ((PisteInput_Keydown(settings.control_down) || PisteInput_Gamepad_Button(settings.gamepad_down)) && !sprite->alas) {
			sprite->kyykky = true;
			sprite_yla += sprite_korkeus/1.5;
		}

		double a_lisays = 0;

		/* NAVIGATING TO RIGHT */
		if (PisteInput_Keydown(settings.control_right) || PisteInput_Gamepad_Button(settings.gamepad_right)) {
			a_lisays = 0.04;//0.08;

			if (lisavauhti) {
				if (rand()%20 == 1 && sprite->current_animation == Animation::WALKING) // Draw dust
					Game::Particles->new_particle(PARTICLE_DUST_CLOUDS,sprite_x-8,sprite_ala-8,0.25,-0.25,40,0,0);

				a_lisays += 0.09;//0.05
			}

			if (sprite->alas)
				a_lisays /= 1.5;//2.0

			sprite->flip_x = false;
		}

		/* NAVIGATING TO LEFT */
		if (PisteInput_Keydown(settings.control_left) || PisteInput_Gamepad_Button(settings.gamepad_left)) {
			a_lisays = -0.04;

			if (lisavauhti) {
				if (rand()%20 == 1 && sprite->current_animation == Animation::WALKING) { // Draw dust
					Game::Particles->new_particle(PARTICLE_DUST_CLOUDS,sprite_x-8,sprite_ala-8,-0.25,-0.25,40,0,0);
				}

				a_lisays -= 0.09;
			}

			if (sprite->alas)	// spriten koskettaessa maata kitka vaikuttaa
				a_lisays /= 1.5;//2.0

			sprite->flip_x = true;
		}

		if (sprite->kyykky)	// Slow when couch
			a_lisays /= 10;

		sprite_a += a_lisays;

		/* JUMPING */
		if (sprite->type->paino > 0) {
			if (PisteInput_Keydown(settings.control_jump) || PisteInput_Gamepad_Button(settings.gamepad_jump)) {
				if (!sprite->kyykky) {
					if (sprite->hyppy_ajastin == 0)
						PK_Play_Sound(hyppy_aani, 100, (int)sprite_x, (int)sprite_y,
									  sprite->type->aani_frq, sprite->type->random_frq);

					if (sprite->hyppy_ajastin <= 0)
						sprite->hyppy_ajastin = 1; //10;
				}
			} else {
				if (sprite->hyppy_ajastin > 0 && sprite->hyppy_ajastin < 45)
					sprite->hyppy_ajastin = 55;
			}

			/* tippuminen hiljaa alasp�in */
			if ((PisteInput_Keydown(settings.control_jump) || PisteInput_Gamepad_Button(settings.gamepad_jump)) && sprite->hyppy_ajastin >= 150/*90+20*/ &&
				sprite->type->liitokyky)
				hidastus = true;
		}
		/* MOVING UP AND DOWN */
		else { // if the player sprite-weight is 0 - like birds

			if (PisteInput_Keydown(settings.control_jump) || PisteInput_Gamepad_Button(settings.gamepad_jump))
				sprite_b -= 0.15;

			if (PisteInput_Keydown(settings.control_down) || PisteInput_Gamepad_Button(settings.gamepad_down))
				sprite_b += 0.15;

			sprite->hyppy_ajastin = 0;
		}

		/* AI */
		for (int ai=0;ai < SPRITE_MAX_AI;ai++)
			switch (sprite->type->AI[ai]){
			
			case AI_MUUTOS_JOS_ENERGIAA_ALLE_2:
				if (sprite->type->muutos > -1)
					sprite->AI_Transformation_Jos_Energiaa_Alle_2(Game::Sprites->prototypes[sprite->type->muutos]);
			break;
			
			case AI_MUUTOS_JOS_ENERGIAA_YLI_1:
			if (sprite->type->muutos > -1)
				if (sprite->AI_Transformation_Jos_Energiaa_Yli_1(Game::Sprites->prototypes[sprite->type->muutos])==1)
					Effect::Destruction(TUHOUTUMINEN_SAVU_HARMAA, (DWORD)sprite->x, (DWORD)sprite->y);
			break;
			
			case AI_MUUTOS_AJASTIN:
				if (sprite->type->muutos > -1)
					sprite->AI_Transformation_Ajastin(Game::Sprites->prototypes[sprite->type->muutos]);
			break;
			
			case AI_VAHINGOITTUU_VEDESTA:
				sprite->AI_Vahingoittuu_Vedesta();
			break;
			
			case AI_MUUTOS_JOS_OSUTTU:
				if (sprite->type->muutos > -1)
					sprite->AI_Transformation_Jos_Osuttu(Game::Sprites->prototypes[sprite->type->muutos]);
			break;

			default: break;
			}

		/* It is not acceptable that a player is anything other than the game character */
		if (sprite->type->type != type_PELIHAHMO)
			sprite->energia = 0;
	}

	//Jump
	bool hyppy_maximissa = sprite->hyppy_ajastin >= 90;

	// Jos ollaan hyp�tty / ilmassa:
	if (sprite->hyppy_ajastin > 0) {
		if (sprite->hyppy_ajastin < 50-sprite->type->max_hyppy)
			sprite->hyppy_ajastin = 50-sprite->type->max_hyppy;

		if (sprite->hyppy_ajastin < 10)
			sprite->hyppy_ajastin = 10;

		if (!hyppy_maximissa) {
		// sprite_b = (sprite->type->max_hyppy/2 - sprite->hyppy_ajastin/2)/-2.0;//-4
		   sprite_b = -sin_table[sprite->hyppy_ajastin]/8;//(sprite->type->max_hyppy/2 - sprite->hyppy_ajastin/2)/-2.5;
			if (sprite_b > sprite->type->max_hyppy){
				sprite_b = sprite->type->max_hyppy/10.0;
				sprite->hyppy_ajastin = 90 - sprite->hyppy_ajastin;
			}

		}

		if (sprite->hyppy_ajastin < 180)
			sprite->hyppy_ajastin += 2;
	}

	if (sprite->hyppy_ajastin < 0)
		sprite->hyppy_ajastin++;

	if (sprite_b > 0 && !hyppy_maximissa)
		sprite->hyppy_ajastin = 90;//sprite->type->max_hyppy*2;

	//Hit recovering
	if (sprite->isku > 0)
		sprite->isku --;

	// Gravity effect
	if (sprite->paino != 0 && (sprite->hyppy_ajastin <= 0 || sprite->hyppy_ajastin >= 45))
		sprite_b += sprite->paino/1.25;// + sprite_b/1.5;

	if (hidastus && sprite_b > 0) // If gliding
		sprite_b /= 1.3;//1.5;//3

	// By default, the sprite is not in the water and not hidden
	sprite->vedessa  = false;
	sprite->piilossa = false;

	// Speed limits
	if (sprite_b > 4.0)//4
		sprite_b = 4.0;//4

	if (sprite_b < -4.0)
		sprite_b = -4.0;

	if (sprite_a > max_nopeus)
		sprite_a = max_nopeus;

	if (sprite_a < -max_nopeus)
		sprite_a = -max_nopeus;

	/*****************************************************************************************/
	/* Blocks colision -                                                                     */
	/*****************************************************************************************/

	int palikat_x_lkm,
	    palikat_y_lkm,
	    palikat_lkm;
	DWORD p;

	if (sprite->type->tiletarkistus){ //Find the tiles that the sprite occupies

		palikat_x_lkm = (int)((sprite_leveys) /32)+4; //Number of blocks
		palikat_y_lkm = (int)((sprite_korkeus)/32)+4;

		kartta_vasen = (int)(sprite_vasen)/32;	//Position in tile map
		kartta_yla	 = (int)(sprite_yla)/32;

		for (y=0;y<palikat_y_lkm;y++)
			for (x=0;x<palikat_x_lkm;x++) //For each block, create a array of blocks around the sprite
				palikat[x+(y*palikat_x_lkm)] = PK_Block_Get(kartta_vasen+x-1,kartta_yla+y-1); //x = 0, y = 0

		/*****************************************************************************************/
		/* Going through the blocks around the sprite->                                           */
		/*****************************************************************************************/

		palikat_lkm = palikat_y_lkm*palikat_x_lkm;
		for (y=0;y<palikat_y_lkm;y++){
			for (x=0;x<palikat_x_lkm;x++) {
				p = x+y*palikat_x_lkm;
				if (p<300)// && p>=0)//{
					//if(sprite->pelaaja == 1) printf("%i\n",palikat_lkm);
					PK_Check_Blocks(*sprite, palikat[p]);
				//}
			}
		}
	}
	/*****************************************************************************************/
	/* If the sprite is under water                                                          */
	/*****************************************************************************************/

	if (sprite->vedessa) {

		if (!sprite->type->osaa_uida || sprite->energia < 1) {
			/*
			if (sprite_b > 0)
				sprite_b /= 2.0;

			sprite_b -= (1.5-sprite->paino)/10;*/
			sprite_b /= 2.0;
			sprite_a /= 1.05;

			if (sprite->hyppy_ajastin > 0 && sprite->hyppy_ajastin < 90)
				sprite->hyppy_ajastin--;
		}

		if (rand()%80 == 1)
			Game::Particles->new_particle(PARTICLE_SPARK,sprite_x-4,sprite_y,0,-0.5-rand()%2,rand()%30+30,0,32);
	}

	if (vedessa != sprite->vedessa) { // Sprite comes in or out from water
		Effect::Splash((int)sprite_x,(int)sprite_y,32);
	}

	/*****************************************************************************************/
	/* Sprite weight                                                                         */
	/*****************************************************************************************/

	sprite->paino = sprite->alkupaino;
	sprite->kytkinpaino = sprite->paino;

	if (sprite->energia < 1 && sprite->paino == 0) // Fall when is death
		sprite->paino = 1;

	/*****************************************************************************************/
	/* Sprite collision with other sprites                                                   */
	/*****************************************************************************************/

	int tuhoutuminen;
	double sprite2_yla; // kyykistymiseen liittyv�
	PK2BLOCK spritepalikka;

	PK2Sprite *sprite2;

	//Compare this sprite with every sprite in the game
	for (int sprite_index = 0; sprite_index < Game::Sprites->sprites.size(); sprite_index++) {
		sprite2 = &Game::Sprites->sprites[sprite_index];

		if (sprite_index != i && /*!sprite2->piilota*/sprite2->aktiivinen) {
			if (sprite2->kyykky)
				sprite2_yla = sprite2->type->korkeus / 3;//1.5;
			else
				sprite2_yla = 0;

			if (sprite2->type->este && sprite->type->tiletarkistus) { //If there is a block sprite active

				if (sprite_x-sprite_leveys/2 +sprite_a  <= sprite2->x + sprite2->type->leveys /2 &&
					sprite_x+sprite_leveys/2 +sprite_a  >= sprite2->x - sprite2->type->leveys /2 &&
					sprite_y-sprite_korkeus/2+sprite_b <= sprite2->y + sprite2->type->korkeus/2 &&
					sprite_y+sprite_korkeus/2+sprite_b >= sprite2->y - sprite2->type->korkeus/2)
				{
					spritepalikka.koodi = 0;
					spritepalikka.ala   = (int)sprite2->y + sprite2->type->korkeus/2;
					spritepalikka.oikea = (int)sprite2->x + sprite2->type->leveys/2;
					spritepalikka.vasen = (int)sprite2->x - sprite2->type->leveys/2;
					spritepalikka.yla   = (int)sprite2->y - sprite2->type->korkeus/2;

					spritepalikka.vesi  = false;

					//spritepalikka.koodi = BLOCK_HISSI_VERT;
					/*
					PK_Block_Set_Barriers(spritepalikka);

					if (!sprite->type->este)
					{
						if (!sprite2->type->este_alas)
							spritepalikka.alas		 = BLOCK_TAUSTA;
						if (!sprite2->type->este_ylos)
							spritepalikka.ylos		 = BLOCK_TAUSTA;
						if (!sprite2->type->este_oikealle)
							spritepalikka.oikealle   = BLOCK_TAUSTA;
						if (!sprite2->type->este_vasemmalle)
							spritepalikka.vasemmalle = BLOCK_TAUSTA;
					}
					*/

					PK_Block_Set_Barriers(spritepalikka);

					if (!sprite->type->este){
						if (!sprite2->type->este_alas)
							spritepalikka.alas = BLOCK_TAUSTA;
						if (!sprite2->type->este_ylos)
							spritepalikka.ylos = BLOCK_TAUSTA;
						if (!sprite2->type->este_oikealle)
							spritepalikka.oikealle = BLOCK_TAUSTA;
						if (!sprite2->type->este_vasemmalle)
							spritepalikka.vasemmalle = BLOCK_TAUSTA;
					}

					if (sprite2->a > 0)
						spritepalikka.koodi = BLOCK_HISSI_HORI;

					if (sprite2->b > 0)
						spritepalikka.koodi = BLOCK_HISSI_VERT;

					PK_Check_Blocks2(*sprite, spritepalikka); //Colision sprite and sprite block
				}
			}

			if (sprite_x <= sprite2->x + sprite2->type->leveys /2 &&
				sprite_x >= sprite2->x - sprite2->type->leveys /2 &&
				sprite_y/*yla*/ <= sprite2->y + sprite2->type->korkeus/2 &&
				sprite_y/*ala*/ >= sprite2->y - sprite2->type->korkeus/2 + sprite2_yla)
			{
				// samanmerkkiset spritet vaihtavat suuntaa t�rm�tess��n
				if (sprite->type->indeksi == sprite2->type->indeksi &&
					sprite2->energia > 0/* && sprite->pelaaja == 0*/)
				{
					if (sprite->x < sprite2->x)
						oikealle = false;
					if (sprite->x > sprite2->x)
						vasemmalle = false;
					if (sprite->y < sprite2->y)
						alas = false;
					if (sprite->y > sprite2->y)
						ylos = false;
				}

				if (sprite->type->Onko_AI(AI_NUOLET_VAIKUTTAVAT)) {

					if (sprite2->type->Onko_AI(AI_NUOLI_OIKEALLE)) {
						sprite_a = sprite->type->max_nopeus / 3.5;
						sprite_b = 0;
					}
					else if (sprite2->type->Onko_AI(AI_NUOLI_VASEMMALLE)) {
						sprite_a = sprite->type->max_nopeus / -3.5;
						sprite_b = 0;
					}

					if (sprite2->type->Onko_AI(AI_NUOLI_YLOS)) {
						sprite_b = sprite->type->max_nopeus / -3.5;
						sprite_a = 0;
					}
					else if (sprite2->type->Onko_AI(AI_NUOLI_ALAS)) {
						sprite_b = sprite->type->max_nopeus / 3.5;
						sprite_a = 0;
					}
				}

				/* spritet voivat vaihtaa tietoa pelaajan olinpaikasta */
	/*			if (sprite->pelaaja_x != -1 && sprite2->pelaaja_x == -1)
				{
					sprite2->pelaaja_x = sprite->pelaaja_x + rand()%30 - rand()%30;
					sprite->pelaaja_x = -1;
				} */

				if (sprite->type->type == TYPE_CHECKPOINT) {
					// Only activate a checkpoint once
					if (!sprite->checkpoint_activated) {
						// This is also needed for animation of the checkpoint sprite
						sprite->checkpoint_activated = true;

						PK_Play_Sound(sprite->type->aanet[AANI_TUHOUTUMINEN], 100, (int) sprite->x, (int) sprite->y, sprite->type->aani_frq, sprite->type->random_frq);

						PK_Checkpoint_Create(sprite->x, sprite->y);
					}
				} 

				if (sprite->type->type == TYPE_COLLECTABLE) {
					PK_Play_Sound(sprite->type->aanet[AANI_TUHOUTUMINEN], 100, (int) sprite->x, (int) sprite->y, sprite->type->aani_frq, sprite->type->random_frq);

					sprite->saatu_vahinko = 1;

					level_stats.collectables_collected[jakso_indeksi_nyt]++;

					level_stats.collectables_index.push_back({ sprite->originalX, sprite->originalY });
				}

				if (sprite->type->show_message_on_collision && sprite2->pelaaja == 1) {
					PK_Start_Info(sprite->message, sprite->message_duration);
				}

				if (sprite->vihollinen != sprite2->vihollinen && sprite->emosprite != sprite_index) {
					if (sprite2->type->type != type_TAUSTA &&
						sprite->type->type   != type_TAUSTA &&
						sprite2->type->type != type_TELEPORTTI &&
						sprite2->isku == 0 &&
						sprite->isku == 0 &&
						sprite2->energia > 0 &&
						sprite->energia > 0 &&
						sprite2->saatu_vahinko < 1)
					{

						// Does the second sprite overpower?

						if (sprite2->b > 2 && sprite2->paino >= 0.5 &&
							sprite2->y < sprite_y && !sprite->type->este &&
							sprite->type->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
						{
							//sprite->saatu_vahinko = (int)sprite2->paino;//1;
							sprite->saatu_vahinko = (int)(sprite2->paino+sprite2->b/4);
							sprite->saatu_vahinko_type = VAHINKO_PUDOTUS;
							sprite2->hyppy_ajastin = 1;
						}

						// If there is another sprite damaging
						if (sprite->type->vahinko > 0 && sprite2->type->type != type_BONUS) {
							sprite2->saatu_vahinko        = sprite->type->vahinko;
							sprite2->saatu_vahinko_type = sprite->type->vahinko_type;
							
							if ( !(sprite2->pelaaja && nakymattomyys) ) //If sprite2 isn't a invisible player
								sprite->hyokkays1 = sprite->type->hyokkays1_aika; //Then sprite attack

							// The projectiles are shattered by shock
							if (sprite2->type->type == type_AMMUS) {
								sprite->saatu_vahinko = 1;//sprite2->type->vahinko;
								sprite->saatu_vahinko_type = sprite2->type->vahinko_type;
							}

							if (sprite->type->type == type_AMMUS) {
								sprite->saatu_vahinko = 1;//sprite2->type->vahinko;
								sprite->saatu_vahinko_type = sprite2->type->vahinko_type;
							}
						}
					}
				}

				// lis�t��n spriten painoon sit� koskettavan toisen spriten paino
				if (sprite->paino > 0)
					sprite->kytkinpaino += sprite2->type->paino;

			}
		}
	}

	/*****************************************************************************************/
	/* If the sprite has suffered damage                                                     */
	/*****************************************************************************************/

	// Just fire can damage a invisible player
	if (nakymattomyys > 0 && sprite->saatu_vahinko != 0 && sprite->saatu_vahinko_type != VAHINKO_TULI &&
		sprite == Game::Sprites->player /*i == pelaaja_index*/) {
		sprite->saatu_vahinko = 0;
		sprite->saatu_vahinko_type = VAHINKO_EI;
	}

	if (sprite->saatu_vahinko != 0 && sprite->energia > 0 && sprite->type->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU && sprite->type->type != TYPE_CHECKPOINT) {
		if (sprite->type->suojaus != sprite->saatu_vahinko_type || sprite->type->suojaus == VAHINKO_EI) {
			sprite->energia -= sprite->saatu_vahinko;
			sprite->isku = VAHINKO_AIKA;

			if (sprite->saatu_vahinko_type == VAHINKO_SAHKO)
				sprite->isku *= 6;

			PK_Play_Sound(sprite->type->aanet[AANI_VAHINKO], 100, (int)sprite->x, (int)sprite->y,
						  sprite->type->aani_frq, sprite->type->random_frq);

			if (sprite->type->tuhoutuminen%100 == TUHOUTUMINEN_HOYHENET)
				Effect::Destruction(TUHOUTUMINEN_HOYHENET, (DWORD)sprite->x, (DWORD)sprite->y);

			if (sprite->type->type != type_AMMUS){
				Game::Particles->new_particle(PARTICLE_STAR,sprite_x,sprite_y,-1,-1,60,0.01,128);
				Game::Particles->new_particle(PARTICLE_STAR,sprite_x,sprite_y, 0,-1,60,0.01,128);
				Game::Particles->new_particle(PARTICLE_STAR,sprite_x,sprite_y, 1,-1,60,0.01,128);
			}

			if (sprite->Onko_AI(AI_VAIHDA_KALLOT_JOS_OSUTTU))
				kartta->Change_SkullBlocks();

			if (sprite->Onko_AI(AI_HYOKKAYS_1_JOS_OSUTTU)){
				sprite->hyokkays1 = sprite->type->hyokkays1_aika;
				sprite->lataus = 0;
			}

			if (sprite->Onko_AI(AI_HYOKKAYS_2_JOS_OSUTTU)){
				sprite->hyokkays2 = sprite->type->hyokkays2_aika;
				sprite->lataus = 0;
			}

		}

		sprite->saatu_vahinko = 0;
		sprite->saatu_vahinko_type = VAHINKO_EI;


		/*****************************************************************************************/
		/* If the sprite is destroyed                                                            */
		/*****************************************************************************************/

		if (sprite->energia < 1) {
			tuhoutuminen = sprite->type->tuhoutuminen;

			if (tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU){
				for (int i = 0; i < 5; i++) {
					if (sprite->type->bonus[i] > -1 && sprite->type->bonus_amount[i] > 0)
						if (sprite->type->bonus_aina || rand() % 4 == 1)
							for (int bi = 0; bi < sprite->type->bonus_amount[i]; bi++)
								Game::Sprites->add(sprite->type->bonus[i], 0, sprite_x - 11 + (10 - rand() % 20),
									sprite_ala - 16 - (10 + rand() % 20), i, true);
				}

				if (sprite->Onko_AI(AI_VAIHDA_KALLOT_JOS_TYRMATTY) && !sprite->Onko_AI(AI_VAIHDA_KALLOT_JOS_OSUTTU))
					kartta->Change_SkullBlocks();

				if (tuhoutuminen >= TUHOUTUMINEN_ANIMAATIO)
					tuhoutuminen -= TUHOUTUMINEN_ANIMAATIO;
				else
					sprite->piilota = true;

				Effect::Destruction(tuhoutuminen, (DWORD)sprite->x, (DWORD)sprite->y);
				PK_Play_Sound(sprite->type->aanet[AANI_TUHOUTUMINEN],100, (int)sprite->x, (int)sprite->y,
							  sprite->type->aani_frq, sprite->type->random_frq);

				if (sprite->Onko_AI(AI_UUSI_JOS_TUHOUTUU)) {
					Game::Sprites->add(sprite->type->indeksi,0,sprite->alku_x-sprite->type->leveys, sprite->alku_y-sprite->type->korkeus,i, false);
				} //TODO - does sprite->type->indeksi work

				if (sprite->type->type == type_PELIHAHMO && sprite->type->pisteet != 0){
					char luku[10];
					itoa(sprite->type->pisteet,luku,10);
					PK_Fadetext_New(FONT_BIG,luku,(int)Game::Sprites->sprites[i].x - 8, (int) Game::Sprites->sprites[i].y - 8, 80, false);
					piste_lisays += sprite->type->pisteet;
				}
			} else
				sprite->energia = 1;
		}
	}

	if (sprite->isku == 0)
		sprite->saatu_vahinko_type = VAHINKO_EI;


	/*****************************************************************************************/
	/* Revisions                                                                             */
	/*****************************************************************************************/

	if (!oikealle)
		if (sprite_a > 0)
			sprite_a = 0;

	if (!vasemmalle)
		if (sprite_a < 0)
			sprite_a = 0;

	if (!ylos){
		if (sprite_b < 0)
			sprite_b = 0;

		if (!hyppy_maximissa)
			sprite->hyppy_ajastin = 95;//sprite->type->max_hyppy * 2;
	}

	if (!alas)
		if (sprite_b >= 0){ //If sprite is falling
			if (sprite->hyppy_ajastin > 0){
				if (sprite->hyppy_ajastin >= 90+10){
					PK_Play_Sound(tomahdys_aani,30,(int)sprite_x, (int)sprite_y,
				                  int(25050-sprite->paino*3000),true);

					//Game::Particles->new_particle(	PARTICLE_DUST_CLOUDS,sprite_x+rand()%5-rand()%5-10,sprite_ala+rand()%3-rand()%3,
					//			  0,-0.2,rand()%50+20,0,0);

					if (rand()%7 == 1) {
						Game::Particles->new_particle(PARTICLE_SMOKE,sprite_x+rand()%5-rand()%5-10,sprite_ala+rand()%3-rand()%3,
									  	   0.3,-0.1,450,0,0);
						Game::Particles->new_particle(PARTICLE_SMOKE,sprite_x+rand()%5-rand()%5-10,sprite_ala+rand()%3-rand()%3,
									  	   -0.3,-0.1,450,0,0);
					}

					if (sprite->paino > 1)
						Game::vibration = 34 + int(sprite->paino * 20);
				}

				sprite->hyppy_ajastin = 0;
			}

			sprite_b = 0;
		}

	/*****************************************************************************************/
	/* Set correct values                                                                    */
	/*****************************************************************************************/

	if (sprite_b > 4.0)
		sprite_b = 4.0;

	if (sprite_b < -4.0)
		sprite_b = -4.0;

	if (sprite_a > max_nopeus)
		sprite_a = max_nopeus;

	if (sprite_a < -max_nopeus)
		sprite_a = -max_nopeus;

	if (sprite->energia > sprite->type->energia)
		sprite->energia = sprite->type->energia;

	if (sprite->isku == 0 || sprite->pelaaja == 1) {
		sprite_x += sprite_a;
		sprite_y += sprite_b;
	}

	if (sprite == Game::Sprites->player || sprite->energia < 1) {
		double kitka = 1.04;

		if (kartta->ilma == ILMA_SADE || kartta->ilma == ILMA_SADEMETSA)
			kitka = 1.03;

		if (kartta->ilma == ILMA_LUMISADE)
			kitka = 1.01;

		if (!alas)
			sprite_a /= kitka;
		else
			sprite_a /= 1.03;//1.02//1.05

		sprite_b /= 1.25;
	}

	sprite->x = sprite_x;
	sprite->y = sprite_y;
	sprite->a = sprite_a;
	sprite->b = sprite_b;

	sprite->oikealle = oikealle;
	sprite->vasemmalle = vasemmalle;
	sprite->alas = alas;
	sprite->ylos = ylos;

	/*
	sprite->paino = sprite->type->paino;

	if (sprite->energia < 1 && sprite->paino == 0)
		sprite->paino = 1;*/

	if (sprite->hyppy_ajastin < 0)
		sprite->alas = false;

	//sprite->kyykky   = false;

	/*****************************************************************************************/
	/* AI                                                                                    */
	/*****************************************************************************************/

	//TODO run sprite lua script
	
	if (sprite->pelaaja == 0) {
		for (int ai=0;ai < SPRITE_MAX_AI; ai++)
			switch (sprite->type->AI[ai]) {
				case AI_EI:							ai = SPRITE_MAX_AI; // lopetetaan
													break;
				case AI_KANA:						sprite->AI_Kana();
													break;
				case AI_PIKKUKANA:					sprite->AI_Kana();
													break;
				case AI_SAMMAKKO1:					sprite->AI_Sammakko1();
													break;
				case AI_SAMMAKKO2:					sprite->AI_Sammakko2();
													break;
				case AI_BONUS:						sprite->AI_Bonus();
													break;
				case AI_MUNA:						sprite->AI_Muna();
													break;
				case AI_AMMUS:						sprite->AI_Ammus();
													break;
				case AI_HYPPIJA:					sprite->AI_Hyppija();
													break;
				case AI_PERUS:						sprite->AI_Perus();
													break;
				case AI_NONSTOP:					sprite->AI_NonStop();
													break;
				case AI_KAANTYY_ESTEESTA_HORI:		sprite->AI_Kaantyy_Esteesta_Hori();
													break;
				case AI_KAANTYY_ESTEESTA_VERT:		sprite->AI_Kaantyy_Esteesta_Vert();
													break;
				case AI_VAROO_KUOPPAA:				sprite->AI_Varoo_Kuoppaa();
													break;
				case AI_RANDOM_SUUNNANVAIHTO_HORI:	sprite->AI_Random_Suunnanvaihto_Hori();
													break;
				case AI_RANDOM_KAANTYMINEN:			sprite->AI_Random_Kaantyminen();
													break;
				case AI_RANDOM_HYPPY:				sprite->AI_Random_Hyppy();
													break;
				case AI_SEURAA_PELAAJAA:			if (nakymattomyys == 0)
														sprite->AI_Seuraa_Pelaajaa(Game::Sprites->player);
													break;
				case AI_SEURAA_PELAAJAA_JOS_NAKEE:	if (nakymattomyys == 0)
														sprite->AI_Seuraa_Pelaajaa_Jos_Nakee(Game::Sprites->player);
													break;
				case AI_SEURAA_PELAAJAA_VERT_HORI:	if (nakymattomyys == 0)
														sprite->AI_Seuraa_Pelaajaa_Vert_Hori(Game::Sprites->player);
													break;
				case AI_SEURAA_PELAAJAA_JOS_NAKEE_VERT_HORI:
													if (nakymattomyys == 0)
														sprite->AI_Seuraa_Pelaajaa_Jos_Nakee_Vert_Hori(Game::Sprites->player);
													break;
				case AI_PAKENEE_PELAAJAA_JOS_NAKEE:	if (nakymattomyys == 0)
														sprite->AI_Pakenee_Pelaajaa_Jos_Nakee(Game::Sprites->player);
													break;
				case AI_POMMI:						sprite->AI_Pommi();
													break;
				case AI_HYOKKAYS_1_JOS_OSUTTU:		sprite->AI_Hyokkays_1_Jos_Osuttu();
													break;
				case AI_HYOKKAYS_2_JOS_OSUTTU:		sprite->AI_Hyokkays_2_Jos_Osuttu();
													break;
				case AI_HYOKKAYS_1_NONSTOP:			sprite->AI_Hyokkays_1_Nonstop();
													break;
				case AI_HYOKKAYS_2_NONSTOP:			sprite->AI_Hyokkays_2_Nonstop();
													break;
				case AI_HYOKKAYS_1_JOS_PELAAJA_EDESSA:
													if (nakymattomyys == 0)
														sprite->AI_Hyokkays_1_Jos_Pelaaja_Edessa(Game::Sprites->player);
													break;
				case AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA:
													if (nakymattomyys == 0)
														sprite->AI_Hyokkays_2_Jos_Pelaaja_Edessa(Game::Sprites->player);
													break;
				case AI_HYOKKAYS_1_JOS_PELAAJA_ALAPUOLELLA:
													if (nakymattomyys == 0)
														sprite->AI_Hyokkays_1_Jos_Pelaaja_Alapuolella(Game::Sprites->player);
													break;
				case AI_HYPPY_JOS_PELAAJA_YLAPUOLELLA:
													if (nakymattomyys == 0)
														sprite->AI_Hyppy_Jos_Pelaaja_Ylapuolella(Game::Sprites->player);
													break;
				case AI_VAHINGOITTUU_VEDESTA:		sprite->AI_Vahingoittuu_Vedesta();
													break;
				case AI_TAPA_KAIKKI:				sprite->AI_Tapa_Kaikki();
													break;
				case AI_KITKA_VAIKUTTAA:			sprite->AI_Kitka_Vaikuttaa();
													break;
				case AI_PIILOUTUU:					sprite->AI_Piiloutuu();
													break;
				case AI_PALAA_ALKUUN_X:				sprite->AI_Palaa_Alkuun_X();
													break;
				case AI_PALAA_ALKUUN_Y:				sprite->AI_Palaa_Alkuun_Y();
													break;
				case AI_LIIKKUU_X_COS:				sprite->AI_Liikkuu_X(cos_table[degree%360]);
													break;
				case AI_LIIKKUU_Y_COS:				sprite->AI_Liikkuu_Y(cos_table[degree%360]);
													break;
				case AI_LIIKKUU_X_SIN:				sprite->AI_Liikkuu_X(sin_table[degree%360]);
													break;
				case AI_LIIKKUU_Y_SIN:				sprite->AI_Liikkuu_Y(sin_table[degree%360]);
													break;
				case AI_LIIKKUU_X_COS_NOPEA:		sprite->AI_Liikkuu_X(cos_table[(degree*2)%360]);
													break;
				case AI_LIIKKUU_Y_SIN_NOPEA:		sprite->AI_Liikkuu_Y(sin_table[(degree*2)%360]);
													break;
				case AI_LIIKKUU_X_COS_HIDAS:		sprite->AI_Liikkuu_X(cos_table[(degree/2)%360]);
													break;
				case AI_LIIKKUU_Y_SIN_HIDAS:		sprite->AI_Liikkuu_Y(sin_table[(degree/2)%360]);
													break;
				case AI_LIIKKUU_Y_SIN_VAPAA:		sprite->AI_Liikkuu_Y(sin_table[(sprite->ajastin/2)%360]);
													break;
				case AI_MUUTOS_JOS_ENERGIAA_ALLE_2:	if (sprite->type->muutos > -1)
														sprite->AI_Transformation_Jos_Energiaa_Alle_2(Game::Sprites->prototypes[sprite->type->muutos]);
													break;
				case AI_MUUTOS_JOS_ENERGIAA_YLI_1:	if (sprite->type->muutos > -1)
														if (sprite->AI_Transformation_Jos_Energiaa_Yli_1(Game::Sprites->prototypes[sprite->type->muutos])==1)
															Effect::Destruction(TUHOUTUMINEN_SAVU_HARMAA, (DWORD)sprite->x, (DWORD)sprite->y);
													break;
				case AI_MUUTOS_AJASTIN:				if (sprite->type->muutos > -1) {
														sprite->AI_Transformation_Ajastin(Game::Sprites->prototypes[sprite->type->muutos]);
													}
													break;
				case AI_MUUTOS_JOS_OSUTTU:			if (sprite->type->muutos > -1) {
														sprite->AI_Transformation_Jos_Osuttu(Game::Sprites->prototypes[sprite->type->muutos]);
													}
													break;
				case AI_TELEPORTTI:					if (sprite->AI_Teleportti(i, Game::Sprites->sprites, /*MAX_SPRITEJA*/ Game::Sprites->sprites.size(), *Game::Sprites->player)==1)
													{

														Game::camera_x = (int)Game::Sprites->player->x;
														Game::camera_y = (int)Game::Sprites->player->y;
														Game::dcamera_x = Game::camera_x-screen_width/2;
														Game::dcamera_y = Game::camera_y-screen_height/2;
														PisteDraw2_FadeIn(PD_FADE_NORMAL);
														if (sprite->type->aanet[AANI_HYOKKAYS2] != -1)
															PK_Play_MenuSound(sprite->type->aanet[AANI_HYOKKAYS2], 100);
															//PK_Play_Sound(, 100, Game::camera_x, Game::camera_y, SOUND_SAMPLERATE, false);


													}
													break;
				case AI_KIIPEILIJA:					sprite->AI_Kiipeilija();
													break;
				case AI_KIIPEILIJA2:				sprite->AI_Kiipeilija2();
													break;
				case AI_TUHOUTUU_JOS_EMO_TUHOUTUU:	sprite->AI_Tuhoutuu_Jos_Emo_Tuhoutuu(Game::Sprites->sprites);
													break;

				case AI_TIPPUU_TARINASTA:			sprite->AI_Tippuu_Tarinasta(Game::vibration + kytkin_tarina);
													break;
				case AI_LIIKKUU_ALAS_JOS_KYTKIN1_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(kytkin1,0,1);
													break;
				case AI_LIIKKUU_YLOS_JOS_KYTKIN1_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(kytkin1,0,-1);
													break;
				case AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN1_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(kytkin1,-1,0);
													break;
				case AI_LIIKKUU_OIKEALLE_JOS_KYTKIN1_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(kytkin1,1,0);
													break;
				case AI_LIIKKUU_ALAS_JOS_KYTKIN2_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(kytkin2,0,1);
													break;
				case AI_LIIKKUU_YLOS_JOS_KYTKIN2_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(kytkin2,0,-1);
													break;
				case AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN2_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(kytkin2,-1,0);
													break;
				case AI_LIIKKUU_OIKEALLE_JOS_KYTKIN2_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(kytkin2,1,0);
													break;
				case AI_LIIKKUU_ALAS_JOS_KYTKIN3_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(kytkin3,0,1);
													break;
				case AI_LIIKKUU_YLOS_JOS_KYTKIN3_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(kytkin3,0,-1);
													break;
				case AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN3_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(kytkin3,-1,0);
													break;
				case AI_LIIKKUU_OIKEALLE_JOS_KYTKIN3_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(kytkin3,1,0);
													break;
				case AI_TIPPUU_JOS_KYTKIN1_PAINETTU: sprite->AI_Tippuu_Jos_Kytkin_Painettu(kytkin1);
													break;
				case AI_TIPPUU_JOS_KYTKIN2_PAINETTU: sprite->AI_Tippuu_Jos_Kytkin_Painettu(kytkin2);
													break;
				case AI_TIPPUU_JOS_KYTKIN3_PAINETTU: sprite->AI_Tippuu_Jos_Kytkin_Painettu(kytkin3);
													break;
				case AI_RANDOM_LIIKAHDUS_VERT_HORI:	sprite->AI_Random_Liikahdus_Vert_Hori();
													break;
				case AI_KAANTYY_JOS_OSUTTU:			sprite->AI_Kaantyy_Jos_Osuttu();
													break;
				case AI_EVIL_ONE:					if (sprite->energia < 1) music_volume = 0;
													break;

				case AI_INFO1:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info01));break;
				case AI_INFO2:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info02));break;
				case AI_INFO3:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info03));break;
				case AI_INFO4:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info04));break;
				case AI_INFO5:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info05));break;
				case AI_INFO6:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info06));break;
				case AI_INFO7:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info07));break;
				case AI_INFO8:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info08));break;
				case AI_INFO9:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info09));break;
				case AI_INFO10:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info10));break;
				case AI_INFO11:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info11));break;
				case AI_INFO12:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info12));break;
				case AI_INFO13:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info13));break;
				case AI_INFO14:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info14));break;
				case AI_INFO15:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info15));break;
				case AI_INFO16:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info16));break;
				case AI_INFO17:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info17));break;
				case AI_INFO18:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info18));break;
				case AI_INFO19:						if (sprite->AI_Info(*Game::Sprites->player))	PK_Start_Info(tekstit->Hae_Teksti(PK_txt.info19));break;

				default:							break;
			}
	}

	//if (kaiku == 1 && sprite->type->type == type_AMMUS && sprite->type->vahinko_type == VAHINKO_MELU &&
	//	sprite->type->aanet[AANI_HYOKKAYS1] > -1)
	//	PK_Play_Sound(sprite->type->aanet[AANI_HYOKKAYS1],20, (int)sprite_x, (int)sprite_y,
	//				  sprite->type->aani_frq, sprite->type->random_frq);


	/*****************************************************************************************/
	/* Set game area to sprite                                                               */
	/*****************************************************************************************/

	if (sprite->x < 0)
		sprite->x = 0;

	if (sprite->y < -sprite_korkeus)
		sprite->y = -sprite_korkeus;

	if (sprite->x > PK2KARTTA_KARTTA_LEVEYS*32)
		sprite->x = PK2KARTTA_KARTTA_LEVEYS*32;

	//if(sprite->x != sprite_x) printf("%f, %f\n", sprite->x, sprite_x);

	// If the sprite falls under the lower edge of the map
	if (sprite->y > PK2KARTTA_KARTTA_KORKEUS*32 + sprite_korkeus) {

		sprite->y = PK2KARTTA_KARTTA_KORKEUS*32 + sprite_korkeus;
		sprite->energia = 0;
		sprite->piilota = true;

		if (sprite->kytkinpaino >= 1)
			Game::vibration = 50;
	}

	if (sprite->a > max_nopeus)
		sprite->a = max_nopeus;

	if (sprite->a < -max_nopeus)
		sprite->a = -max_nopeus;


	/*****************************************************************************************/
	/* Attacks 1 and 2                                                                       */
	/*****************************************************************************************/

	// If the sprite is ready and isn't crouching
	if (sprite->lataus == 0 && !sprite->kyykky) {
		// hy�kk�ysaika on "tapissa" mik� tarkoittaa sit�, ett� aloitetaan hy�kk�ys
		if (sprite->hyokkays1 == sprite->type->hyokkays1_aika) {
			// provides recovery time, after which the sprite can attack again
			sprite->lataus = sprite->type->latausaika;
			if(sprite->lataus == 0) sprite->lataus = 5;
			// jos spritelle ei ole m��ritelty omaa latausaikaa ...
			if (sprite->ammus1 > -1 && sprite->type->latausaika == 0)
			// ... ja ammukseen on, otetaan latausaika ammuksesta
				if (Game::Sprites->prototypes[sprite->ammus1].tulitauko > 0)
					sprite->lataus = Game::Sprites->prototypes[sprite->ammus1].tulitauko;

			// soitetaan hy�kk�ys��ni
			PK_Play_Sound(sprite->type->aanet[AANI_HYOKKAYS1],100, (int)sprite_x, (int)sprite_y,
						  sprite->type->aani_frq, sprite->type->random_frq);

			if (sprite->ammus1 > -1) {
				Game::Sprites->add_ammo(sprite->ammus1,0,sprite_x, sprite_y, i);

		//		if (Game::Sprites->protot[sprite->ammus1].aanet[AANI_HYOKKAYS1] > -1)
		//			PK_Play_Sound(Game::Sprites->protot[sprite->ammus1].aanet[AANI_HYOKKAYS1],100, (int)sprite_x, (int)sprite_y,
		//						  sprite->type->aani_frq, sprite->type->random_frq);
			}
		}

		// Sama kuin hy�kk�ys 1:ss�
		if (sprite->hyokkays2 == sprite->type->hyokkays2_aika) {
			sprite->lataus = sprite->type->latausaika;
			if(sprite->lataus == 0) sprite->lataus = 5;
			if (sprite->ammus2 > -1  && sprite->type->latausaika == 0)
				if (Game::Sprites->prototypes[sprite->ammus2].tulitauko > 0)
					sprite->lataus = Game::Sprites->prototypes[sprite->ammus2].tulitauko;

			PK_Play_Sound(sprite->type->aanet[AANI_HYOKKAYS2],100,(int)sprite_x, (int)sprite_y,
						  sprite->type->aani_frq, sprite->type->random_frq);

			if (sprite->ammus2 > -1) {
				Game::Sprites->add_ammo(sprite->ammus2,0,sprite_x, sprite_y, i);

		//		if (Game::Sprites->protot[sprite->ammus2].aanet[AANI_HYOKKAYS1] > -1)
		//			PK_Play_Sound(Game::Sprites->protot[sprite->ammus2].aanet[AANI_HYOKKAYS1],100, (int)sprite_x, (int)sprite_y,
		//						  sprite->type->aani_frq, sprite->type->random_frq);

			}
		}
	}

	// Random sounds
	if (sprite->type->aanet[AANI_RANDOM] != -1 && rand()%200 == 1 && sprite->energia > 0)
		PK_Play_Sound(sprite->type->aanet[AANI_RANDOM],80,(int)sprite_x, (int)sprite_y,
					  sprite->type->aani_frq, sprite->type->random_frq);


	// KEHITYSVAIHEEN APUTOIMINTOJA

	if (Game::Sprites->player->energia < 0) {
		Game::Sprites->player = 0;
	}

	BYTE color;
	DWORD plk;

	if (PisteInput_Keydown(PI_B) && dev_mode) { // Draw bounding box
		
		if (i == 0/*pelaaja_index*/) {

			char lukua[50];
			itoa(palikat[1].yla,lukua,10);
			//gcvt(sprite_a,7,lukua);
			PisteDraw2_Font_Write(FONT_SMALL,lukua,310,50);

		}

		if (sprite->type->tiletarkistus)
			for (x=0;x<palikat_x_lkm;x++) {
				for (y=0;y<palikat_y_lkm;y++) {
					color = 50-x*2-y*2;
					plk = x+y*palikat_x_lkm;

					if (plk > 299)
						plk = 299;

					//if (plk < 0)
					//	plk = 0;

					if (!palikat[plk].tausta)
						color += 32;

					PisteDraw2_ScreenFill(
											palikat[plk].vasen-Game::camera_x,
											palikat[plk].yla-Game::camera_y,
											palikat[plk].oikea-Game::camera_x,
											palikat[plk].ala-Game::camera_y,
											color);
				}
			}

		PisteDraw2_ScreenFill(
								(int)(sprite_vasen-Game::camera_x),
								(int)(sprite_yla-Game::camera_y),
								(int)(sprite_oikea-Game::camera_x),
								(int)(sprite_ala-Game::camera_y),
								30);

	}



	return 0;
}
int PK_Sprite_Bonus_Movement(int i){
	sprite_x = 0;
	sprite_y = 0;
	sprite_a = 0;
	sprite_b = 0;

	sprite_vasen = 0;
	sprite_oikea = 0;
	sprite_yla = 0;
	sprite_ala = 0;

	sprite_leveys  = 0;
	sprite_korkeus = 0;

	kartta_vasen = 0;
	kartta_yla   = 0;

	PK2Sprite *sprite = &Game::Sprites->sprites[i];

	sprite_x = sprite->x;
	sprite_y = sprite->y;
	sprite_a = sprite->a;
	sprite_b = sprite->b;

	sprite_leveys  = sprite->type->leveys;
	sprite_korkeus = sprite->type->korkeus;

	x = 0;
	y = 0;
	oikealle	= true,
	vasemmalle	= true,
	ylos		= true,
	alas		= true;

	vedessa = sprite->vedessa;

	max_nopeus = (int)sprite->type->max_nopeus;

	// Siirret��n varsinaiset muuttujat apumuuttujiin.

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;
	sprite_yla	 = sprite_y-sprite_korkeus/2;
	sprite_ala	 = sprite_y+sprite_korkeus/2;


	if (sprite->isku > 0)
		sprite->isku--;

	if (sprite->lataus > 0)
		sprite->lataus--;

	if (sprite->transformation_ajastin > 0)	// aika muutokseen
		sprite->transformation_ajastin --;

	// Hyppyyn liittyv�t seikat

	if (kytkin_tarina + Game::vibration > 0 && sprite->hyppy_ajastin == 0)
		sprite->hyppy_ajastin = sprite->type->max_hyppy / 2;

	if (sprite->hyppy_ajastin > 0 && sprite->hyppy_ajastin < sprite->type->max_hyppy)
	{
		sprite->hyppy_ajastin ++;
		sprite_b = (sprite->type->max_hyppy - sprite->hyppy_ajastin)/-4.0;//-2
	}

	if (sprite_b > 0)
		sprite->hyppy_ajastin = sprite->type->max_hyppy;



	if (sprite->paino != 0)	// jos bonuksella on paino, tutkitaan ymp�rist�
	{
		// o
		//
		// |  Gravity
		// V

		sprite_b += sprite->paino + sprite_b/1.25;

		if (sprite->vedessa)
		{
			if (sprite_b > 0)
				sprite_b /= 2.0;

			if (rand()%80 == 1)
				Game::Particles->new_particle(PARTICLE_SPARK,sprite_x-4,sprite_y,0,-0.5-rand()%2,rand()%30+30,0,32);
		}

		sprite->vedessa = false;

		sprite->kytkinpaino = sprite->paino;

		/* TOISET SPRITET */

		PK2BLOCK spritepalikka; 

		for (int sprite_index = 0; sprite_index < Game::Sprites->sprites.size(); sprite_index++) {

			PK2Sprite* sprite2 = &Game::Sprites->sprites[sprite_index];
			if (sprite_index != i && !sprite2->piilota) {
				if (sprite2->type->este && sprite->type->tiletarkistus) {
					if (sprite_x-sprite_leveys/2 +sprite_a <= sprite2->x + sprite2->type->leveys /2 &&
						sprite_x+sprite_leveys/2 +sprite_a >= sprite2->x - sprite2->type->leveys /2 &&
						sprite_y-sprite_korkeus/2+sprite_b <= sprite2->y + sprite2->type->korkeus/2 &&
						sprite_y+sprite_korkeus/2+sprite_b >= sprite2->y - sprite2->type->korkeus/2)
					{
						spritepalikka.koodi = 0;
						spritepalikka.ala   = (int)sprite2->y + sprite2->type->korkeus/2;
						spritepalikka.oikea = (int)sprite2->x + sprite2->type->leveys/2;
						spritepalikka.vasen = (int)sprite2->x - sprite2->type->leveys/2;
						spritepalikka.yla   = (int)sprite2->y - sprite2->type->korkeus/2;

						spritepalikka.alas       = BLOCK_SEINA;
						spritepalikka.ylos       = BLOCK_SEINA;
						spritepalikka.oikealle   = BLOCK_SEINA;
						spritepalikka.vasemmalle = BLOCK_SEINA;

						if (!sprite2->type->este_alas)
							spritepalikka.alas		 = BLOCK_TAUSTA;
						if (!sprite2->type->este_ylos)
							spritepalikka.ylos		 = BLOCK_TAUSTA;
						if (!sprite2->type->este_oikealle)
							spritepalikka.oikealle   = BLOCK_TAUSTA;
						if (!sprite2->type->este_vasemmalle)
							spritepalikka.vasemmalle = BLOCK_TAUSTA;


						spritepalikka.vesi  = false;

						PK_Block_Set_Barriers(spritepalikka);
						PK_Check_Blocks2(*sprite, spritepalikka); //Colision bonus and sprite block
					}
				}

				if (sprite_x < sprite2->x + sprite2->type->leveys/2 &&
					sprite_x > sprite2->x - sprite2->type->leveys/2 &&
					sprite_y < sprite2->y + sprite2->type->korkeus/2 &&
					sprite_y > sprite2->y - sprite2->type->korkeus/2 &&
					sprite->isku == 0)
				{
					if (sprite2->type->type != type_BONUS &&
						!(sprite2 == Game::Sprites->player && sprite->type->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU))
						sprite_a += sprite2->a*(rand()%4);

					// lis�t��n spriten painoon sit� koskettavan toisen spriten paino
					sprite->kytkinpaino += sprite2->type->paino;

					// samanmerkkiset spritet vaihtavat suuntaa t�rm�tess��n
					if (sprite->type->indeksi == sprite2->type->indeksi &&
						sprite2->energia > 0)
					{
						if (sprite->x < sprite2->x) {
							sprite2->a += sprite->a / 3.0;
							oikealle = false;
						}
						if (sprite->x > sprite2->x) {
							sprite2->a += sprite->a / 3.0;
							vasemmalle = false;
						}
						/*
						if (sprite->y < spritet[sprite_index].y)
							alas = false;
						if (sprite->y > spritet[sprite_index].y)
							ylos = false;*/
					}

				}
			}
		}

		// Tarkistetaan ettei menn� mihink��n suuntaan liian kovaa.

		if (sprite_b > 4)
			sprite_b = 4;

		if (sprite_b < -4)
			sprite_b = -4;

		if (sprite_a > 3)
			sprite_a = 3;

		if (sprite_a < -3)
			sprite_a = -3;

		// Lasketaan

		int palikat_x_lkm = 0,
			palikat_y_lkm = 0;

		if (sprite->type->tiletarkistus)
		{

			palikat_x_lkm = (int)((sprite_leveys) /32)+4;
			palikat_y_lkm = (int)((sprite_korkeus)/32)+4;

			kartta_vasen = (int)(sprite_vasen)/32;
			kartta_yla	 = (int)(sprite_yla)/32;

			for (y=0;y<palikat_y_lkm;y++)
				for (x=0;x<palikat_x_lkm;x++)
				{
					palikat[x+y*palikat_x_lkm] = PK_Block_Get(kartta_vasen+x-1,kartta_yla+y-1);
				}

			// Tutkitaan t�rm��k� palikkaan

			for (y=0;y<palikat_y_lkm;y++)
				for (x=0;x<palikat_x_lkm;x++)
					PK_Check_Blocks(*sprite, palikat[x+y*palikat_x_lkm]);
			/*
			PK_Check_Blocks_Debug(sprite, palikat[x+y*palikat_x_lkm],
					sprite_x,
					sprite_y,
					sprite_a,
					sprite_b,
					sprite_vasen,
					sprite_oikea,
					sprite_yla,
					sprite_ala,
					sprite_leveys,
					sprite_korkeus,
					kartta_vasen,
					kartta_yla,
					oikealle,
					vasemmalle,
					ylos,
					alas);*/


		}

		if (vedessa != sprite->vedessa)
			Effect::Splash((int)sprite_x,(int)sprite_y,32);


		if (!oikealle)
		{
			if (sprite_a > 0)
				sprite_a = -sprite_a/1.5;
		}

		if (!vasemmalle)
		{
			if (sprite_a < 0)
				sprite_a = -sprite_a/1.5;
		}

		if (!ylos)
		{
			if (sprite_b < 0)
				sprite_b = 0;

			sprite->hyppy_ajastin = sprite->type->max_hyppy;
		}

		if (!alas)
		{
			if (sprite_b >= 0)
			{
				if (sprite->hyppy_ajastin > 0)
				{
					sprite->hyppy_ajastin = 0;
				//	if (/*sprite_b == 4*/!maassa)
				//		PK_Play_Sound(tomahdys_aani,20,(int)sprite_x, (int)sprite_y,
				//				      int(25050-sprite->type->paino*4000),true);
				}

				if (sprite_b > 2)
					sprite_b = -sprite_b/(3+rand()%2);
				else
					sprite_b = 0;
			}
			//sprite_a /= kitka;
			sprite_a /= 1.07;
		}
		else
		{
			sprite_a /= 1.02;
		}

		sprite_b /= 1.5;

		if (sprite_b > 4)
			sprite_b = 4;

		if (sprite_b < -4)
			sprite_b = -4;

		if (sprite_a > 4)
			sprite_a = 4;

		if (sprite_a < -4)
			sprite_a = -4;

		sprite_x += sprite_a;
		sprite_y += sprite_b;

		sprite->x = sprite_x;
		sprite->y = sprite_y;
		sprite->a = sprite_a;
		sprite->b = sprite_b;

		sprite->oikealle = oikealle;
		sprite->vasemmalle = vasemmalle;
		sprite->alas = alas;
		sprite->ylos = ylos;
	}
	else	// jos spriten paino on nolla, tehd��n spritest� "kelluva"
	{
		sprite->y = sprite->alku_y + cos_table[int(degree+(sprite->alku_x+sprite->alku_y))%360] / 3.0;
		sprite_y = sprite->y;
	}

	sprite->paino = sprite->alkupaino;

	int tuhoutuminen;

	// Test if player touches bonus
	if (sprite_x < Game::Sprites->player->x + Game::Sprites->player->type->leveys/2 &&
		sprite_x > Game::Sprites->player->x - Game::Sprites->player->type->leveys/2 &&
		sprite_y < Game::Sprites->player->y + Game::Sprites->player->type->korkeus/2 &&
		sprite_y > Game::Sprites->player->y - Game::Sprites->player->type->korkeus/2 &&
		sprite->isku == 0) {

		// TODO FIX SCORE UPDATE
		// Update the score on the HUD
		if (sprite->energia > 0 && Game::Sprites->player->energia > 0) {
			if (sprite->type->pisteet != 0) {
				/*if (sprite->type->pisteet > 50) {
					piste_lisays += sprite->type->pisteet / 10;
				} else {
					piste_lisays += sprite->type->pisteet;
				}*/

				piste_lisays += sprite->type->pisteet;
				
				char luku[6];
				itoa(sprite->type->pisteet,luku,10);
				if (sprite->type->pisteet >= 50)
					PK_Fadetext_New(FONT_BIG,luku,(int)sprite->x-8,(int)sprite->y-8,100,false);
				else
					PK_Fadetext_New(FONT_SMALL,luku,(int)sprite->x-8,(int)sprite->y-8,100,false);

			}

			// TODO Fix this, when value is negative
			if (sprite->Onko_AI(AI_BONUS_AIKA)) {
				increase_time += sprite->type->latausaika;
			}

			if (sprite->Onko_AI(AI_BONUS_NAKYMATTOMYYS))
				nakymattomyys = sprite->type->latausaika;

			//kartta->spritet[(int)(sprite->alku_x/32) + (int)(sprite->alku_y/32)*PK2KARTTA_KARTTA_LEVEYS] = 255;

			if (sprite->type->vahinko != 0 && sprite->type->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU){
				Game::Sprites->player->energia -= sprite->type->vahinko;
				//if (player->energia > player->type->energia){ //TODO - set correct energy
				//	player->energia = player->type->energia;
				//}
			}

			tuhoutuminen = sprite->type->tuhoutuminen;

			if (tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
			{
				if (tuhoutuminen >= TUHOUTUMINEN_ANIMAATIO)
					tuhoutuminen -= TUHOUTUMINEN_ANIMAATIO;
				else
				{
					if (sprite->type->avain)
					{
						avaimia--;

						if (avaimia < 1)
							kartta->Open_Locks();
					}

					sprite->piilota = true;
				}

				if (sprite->Onko_AI(AI_UUSI_JOS_TUHOUTUU)) {
					double ax, ay;
					ax = sprite->alku_x;//-sprite->type->leveys;
					ay = sprite->alku_y-sprite->type->korkeus/2.0;
					Game::Sprites->add(sprite->type->indeksi,0,ax-17, ay,i, false);
					for (int r=1;r<6;r++)
						Game::Particles->new_particle(PARTICLE_SPARK,ax+rand()%10-rand()%10, ay+rand()%10-rand()%10,0,0,rand()%100,0.1,32);

				}

				for (int i = 0; i < 5; i++) {
					if (sprite->type->bonus[i] != -1)
						Game::Gifts->add(sprite->type->bonus[i]);
				}
				
				if (sprite->type->muutos != -1)
				{
					if (Game::Sprites->prototypes[sprite->type->muutos].AI[0] != AI_BONUS)
					{
						Game::Sprites->player->type = &Game::Sprites->prototypes[sprite->type->muutos];
						Game::Sprites->player->ammus1 = Game::Sprites->player->type->ammus1;
						Game::Sprites->player->ammus2 = Game::Sprites->player->type->ammus2;
						Game::Sprites->player->alkupaino = Game::Sprites->player->type->paino;
						Game::Sprites->player->y -= Game::Sprites->player->type->korkeus/2;
						//PK_Start_Info("pekka has been transformed!");
					}
				}

				if (sprite->type->ammus1 != -1)
				{
					Game::Sprites->player->ammus1 = sprite->type->ammus1;
					PK_Start_Info(tekstit->Hae_Teksti(PK_txt.game_newegg));
				}

				if (sprite->type->ammus2 != -1)
				{
					Game::Sprites->player->ammus2 = sprite->type->ammus2;
					PK_Start_Info(tekstit->Hae_Teksti(PK_txt.game_newdoodle));
				}

				PK_Play_Sound(sprite->type->aanet[AANI_TUHOUTUMINEN],100, (int)sprite->x, (int)sprite->y,
							  sprite->type->aani_frq, sprite->type->random_frq);

				Effect::Destruction(tuhoutuminen, (DWORD)sprite_x, (DWORD)sprite_y);
			}
		}
	}

	for (i=0;i<SPRITE_MAX_AI;i++)
	{
		if (sprite->type->AI[i] == AI_EI)
			break;

		switch (sprite->type->AI[i])
		{
		case AI_BONUS:				sprite->AI_Bonus();break;

		case AI_PERUS:				sprite->AI_Perus();break;

		case AI_MUUTOS_AJASTIN:		if (sprite->type->muutos > -1)
									sprite->AI_Transformation_Ajastin(Game::Sprites->prototypes[sprite->type->muutos]);
									break;

		case AI_TIPPUU_TARINASTA:	sprite->AI_Tippuu_Tarinasta(Game::vibration + kytkin_tarina);
									break;

		default:					break;
		}
	}

	/* The energy doesn't matter that the player is a bonus item */
	if (sprite->pelaaja != 0)
		sprite->energia = 0;

	return 0;
}

int PK_Update_Sprites(){
	debug_active_sprites = 0;
	int i;
	PK2Sprite *sprite = nullptr;

	// TODO Update sprite activation range
	for (i = 0; i < Game::Sprites->sprites.size(); i++){ //Activate sprite if it is on screen
		sprite = &Game::Sprites->sprites[i];
		if (sprite->x < Game::camera_x+640+320 &&//screen_width+screen_width/2 &&
			sprite->x > Game::camera_x-320 &&//screen_width/2 &&
			sprite->y < Game::camera_y+480+240 &&//screen_height+screen_height/2 &&
			sprite->y > Game::camera_y-240)//screen_height/2)
			sprite->aktiivinen = true;
		else
			sprite->aktiivinen = false;

		if (sprite->piilota == true) {
			sprite->aktiivinen = false;
		}
	}

	for (i = 0; i < Game::Sprites->sprites.size(); i++){
		sprite = &Game::Sprites->sprites[i];
		if (sprite->aktiivinen && sprite->type->type != type_TAUSTA){
			if (sprite->type->type == type_BONUS)
				PK_Sprite_Bonus_Movement(i);
			else
				PK_Sprite_Movement(i);

			debug_active_sprites++;
		}
	}

	return 0;
}

int PK_Update_Camera(){


	Game::camera_x = (int)Game::Sprites->player->x-screen_width/2;
	Game::camera_y = (int)Game::Sprites->player->y-screen_height/2;

	/*
	if (!PisteInput_Hiiri_Vasen())
	{
		Game::camera_x = (int)player->x-screen_width/2;
		Game::camera_y = (int)player->y-screen_height/2;
	}
	else
	{
		Game::camera_x += PisteInput_Hiiri_X(0)*5;
		Game::camera_y += PisteInput_Hiiri_Y(0)*5;
	}*/

	if (Game::vibration > 0)
	{
		Game::dcamera_x += (rand()%Game::vibration-rand()%Game::vibration)/5;
		Game::dcamera_y += (rand()%Game::vibration-rand()%Game::vibration)/5;

		Game::vibration--;
	}

	if (kytkin_tarina > 0)
	{
		Game::dcamera_x += (rand()%9-rand()%9);//3
		Game::dcamera_y += (rand()%9-rand()%9);

		kytkin_tarina--;
	}

	if (Game::dcamera_x != Game::camera_x)
		Game::dcamera_a = (Game::camera_x - Game::dcamera_x) / 15;

	if (Game::dcamera_y != Game::camera_y)
		Game::dcamera_b = (Game::camera_y - Game::dcamera_y) / 15;

	if (Game::dcamera_a > 6)
		Game::dcamera_a = 6;

	if (Game::dcamera_a < -6)
		Game::dcamera_a = -6;

	if (Game::dcamera_b > 6)
		Game::dcamera_b = 6;

	if (Game::dcamera_b < -6)
		Game::dcamera_b = -6;

	Game::dcamera_x += Game::dcamera_a;
	Game::dcamera_y += Game::dcamera_b;

	Game::camera_x = (int)Game::dcamera_x;
	Game::camera_y = (int)Game::dcamera_y;

	if (Game::camera_x < 0)
		Game::camera_x = 0;

	if (Game::camera_y < 0)
		Game::camera_y = 0;

	if (Game::camera_x > int(PK2KARTTA_KARTTA_LEVEYS-screen_width/32)*32)
		Game::camera_x = int(PK2KARTTA_KARTTA_LEVEYS-screen_width/32)*32;

	if (Game::camera_y > int(PK2KARTTA_KARTTA_KORKEUS-screen_height/32)*32)
		Game::camera_y = int(PK2KARTTA_KARTTA_KORKEUS-screen_height/32)*32;

	return 0;
}

//==================================================
//(#13) Draw Screen Functions
//==================================================

int PK_Draw_InGame_BGSprites(){
	double xl, yl, alku_x, alku_y, yk;

	for (int in=0; in < Game::Sprites->background_sprites.size(); in++) { //Game::Sprites->sprites.size()
		PK2Sprite& sprite = Game::Sprites->sprites[Game::Sprites->background_sprites[in]];

		if (sprite.type != nullptr) {
			if (!sprite.piilota && sprite.type->type == type_TAUSTA) {
				//Tarkistetaanko onko sprite tai osa siit� kuvassa

				alku_x = sprite.alku_x;
				alku_y = sprite.alku_y;

				if (sprite.type->pallarx_kerroin != 0) {
					xl =  alku_x - Game::camera_x-screen_width/2 - sprite.type->leveys/2;
					xl /= sprite.type->pallarx_kerroin;
					yl =  alku_y - Game::camera_y-screen_height/2 - sprite.type->korkeus/2;
					yk = sprite.type->pallarx_kerroin;///1.5;
					if (yk != 0)
						yl /= yk;


				}
				else
					xl = yl = 0;

				switch(sprite.type->AI[0]) {
				case AI_TAUSTA_KUU					:	yl += screen_height/3+50; break;
				/*case AI_TAUSTA_LIIKKUU_VASEMMALLE	:	if (sprite.a == 0)
															sprite.a = rand()%3;
														sprite.alku_x -= sprite.a;
														if (sprite.piilossa && sprite.alku_x < Game::camera_x)
														{
													  		  sprite.alku_x = Game::camera_x+screen_width+sprite.type->leveys*2;
															  sprite.a = rand()%3;
														}
														break;*/
				case AI_LIIKKUU_X_COS:			sprite.AI_Liikkuu_X(cos_table[degree%360]);
												alku_x = sprite.x;
												alku_y = sprite.y;
												break;
				case AI_LIIKKUU_Y_COS:			sprite.AI_Liikkuu_Y(cos_table[degree%360]);
												alku_x = sprite.x;
												alku_y = sprite.y;
												break;
				case AI_LIIKKUU_X_SIN:			sprite.AI_Liikkuu_X(sin_table[degree%360]);
												alku_x = sprite.x;
												alku_y = sprite.y;
												break;
				case AI_LIIKKUU_Y_SIN:			sprite.AI_Liikkuu_Y(sin_table[degree%360]);
												alku_x = sprite.x;
												alku_y = sprite.y;
												break;
				default: break;
				}

				sprite.x = alku_x-xl;
				sprite.y = alku_y-yl;

				//Tarkistetaanko onko sprite tai osa siit� kuvassa
				if (sprite.x - sprite.type->leveys/2  < Game::camera_x+screen_width &&
					sprite.x + sprite.type->leveys/2  > Game::camera_x &&
					sprite.y - sprite.type->korkeus/2 < Game::camera_y+screen_height &&
					sprite.y + sprite.type->korkeus/2 > Game::camera_y)
				{
					sprite.Piirra(Game::camera_x,Game::camera_y);
					sprite.piilossa = false;

					debug_drawn_sprites++;
				} else {
					if (!paused)
						sprite.Animoi();
					sprite.piilossa = true;
				}

				debug_sprites++;

			}
		}
	}
	return 0;
}
int PK_Draw_InGame_Sprites(){
	debug_sprites = 0;
	debug_drawn_sprites = 0;
	int stars, sx;
	double star_x, star_y;

	for (int i = 0; i < Game::Sprites->sprites.size(); i++){
		// Onko sprite n�kyv�
		PK2Sprite* sprite = &Game::Sprites->sprites[i];
		if (!sprite->piilota && sprite->type->type != type_TAUSTA){
			//Check whether or not sprite is on the screen
			if (sprite->x - sprite->type->leveys/2  < Game::camera_x+screen_width &&
				sprite->x + sprite->type->leveys/2  > Game::camera_x &&
				sprite->y - sprite->type->korkeus/2 < Game::camera_y+screen_height &&
				sprite->y + sprite->type->korkeus/2 > Game::camera_y){

				if (sprite->isku > 0 && sprite->type->type != type_BONUS && sprite->energia < 1) {
					int framex = ((degree%12)/3) * 58;
					double hit_x = sprite->x-8, hit_y = sprite->y-8;
					PisteDraw2_Image_CutClip(kuva_peli,hit_x-Game::camera_x-28+8, hit_y-Game::camera_y-27+8,1+framex,83,1+57+framex,83+55);
				}

				if (nakymattomyys == 0 || (!doublespeed && nakymattomyys%2 == 0) || (doublespeed && nakymattomyys%4 <= 1) || sprite != Game::Sprites->player /*i != pelaaja_index*/)
					sprite->Piirra(Game::camera_x,Game::camera_y);

				if (sprite->energia < 1 && sprite->type->type != type_AMMUS && sprite->type->type != TYPE_CHECKPOINT) {
					sx = (int)sprite->x;
					for (stars=0; stars<3; stars++){
						star_x = sprite->x-8 + (sin_table[((stars*120+degree)*2)%359])/3;
						star_y = sprite->y-18 + (cos_table[((stars*120+degree)*2+sx)%359])/8;
						PisteDraw2_Image_CutClip(kuva_peli,star_x-Game::camera_x, star_y-Game::camera_y,1,1,11,11);
					}
				}

					debug_drawn_sprites++;
			} else{
				if (!paused)
					sprite->Animoi();

				if (sprite->energia < 1) {
					sprite->piilota = true;
				}
			}

			debug_sprites++;
		}
	}
	return 0;
}

int PK_Draw_InGame_DebugInfo(){
	int vali, fy = 70;
	char lukua[20];

	if (settings.isWide)
		PisteDraw2_SetXOffset(80);
	else
		PisteDraw2_SetXOffset(0);

	vali = PisteDraw2_Font_Write(FONT_SMALL,"Sprites: ",10,fy);
	itoa(debug_sprites,lukua,10);
	PisteDraw2_Font_Write(FONT_SMALL,lukua,10+vali,fy);
	fy += 10;

	vali = PisteDraw2_Font_Write(FONT_SMALL,"Active: ",10,fy);
	itoa(debug_active_sprites,lukua,10);
	PisteDraw2_Font_Write(FONT_SMALL,lukua,10+vali,fy);
	fy += 10;

	vali = PisteDraw2_Font_Write(FONT_SMALL,"Animated: ",10,fy);
	itoa(debug_drawn_sprites,lukua,10);
	PisteDraw2_Font_Write(FONT_SMALL,lukua,10+vali,fy);
	fy += 10;

	for (int i=0;i< Game::Sprites->prototypes.size();i++){
		itoa(Game::Sprites->prototypes[i].indeksi,lukua,10);
		PisteDraw2_Font_Write(FONT_SMALL,lukua,410,10+i*10);
		PisteDraw2_Font_Write(FONT_SMALL,Game::Sprites->prototypes[i].file, 430,10+i*10);
		//PisteDraw2_Font_Write(FONT_SMALL,Game::Sprites->protot[i].bonus_sprite,545,10+i*10);
	}

	for (int i=0;i<EPISODI_MAX_LEVELS;i++)
		if (!levels[i].name.empty())
			PisteDraw2_Font_Write(FONT_SMALL,levels[i].name, 0, 240 + i * 10);

	char dluku[50];

	sprintf(dluku, "%.7f", Game::Sprites->player->x); //Player x
	PisteDraw2_Font_Write(FONT_SMALL, dluku, 10, 410);

	sprintf(dluku, "%.7f", Game::Sprites->player->y); //Player y
	PisteDraw2_Font_Write(FONT_SMALL, dluku, 10, 420);

	sprintf(dluku, "%.7f", Game::Sprites->player->b); //Player v-speed
	PisteDraw2_Font_Write(FONT_SMALL, dluku, 10, 430);

	sprintf(dluku, "%.7f", Game::Sprites->player->a); //Player h-speed
	PisteDraw2_Font_Write(FONT_SMALL, dluku, 10, 440);

	PisteDraw2_Font_Write(FONT_SMALL, next_map, 10, 460);

	itoa(Game::Sprites->player->hyppy_ajastin, lukua, 10);
	PisteDraw2_Font_Write(FONT_SMALL, lukua, 270, 460);

	char tpolku[PE_PATH_SIZE] = "";
	PK_Load_EpisodeDir(tpolku);

	PisteDraw2_Font_Write(FONT_SMALL,tpolku,10,470);

	itoa(nakymattomyys,lukua,10);
	PisteDraw2_Font_Write(FONT_SMALL,lukua,610,470);

	itoa(kytkin1, lukua, 10);
	PisteDraw2_Font_Write(FONT_SMALL, lukua, 610, 460);
	itoa(kytkin2, lukua, 10);
	PisteDraw2_Font_Write(FONT_SMALL, lukua, 610, 450);
	itoa(kytkin3, lukua, 10);
	PisteDraw2_Font_Write(FONT_SMALL, lukua, 610, 440);

	PisteDraw2_SetXOffset(0);

	return 0;
}

// TODO Update this
int PK_Draw_InGame_DevKeys() {
	const char* txt0 = "dev mode";
	int char_w = PisteDraw2_Font_Write(FONT_SMALL, txt0, 0, screen_height - 10) / strlen(txt0);
	int char_h = 10;

	const char* help = "h: help";

	if (!PisteInput_Keydown(PI_H)) {
		PisteDraw2_Font_Write(FONT_SMALL, help, screen_width - strlen(help) * char_w, screen_height - 10);
		return 0;
	}

	const char* txt1  = "z: press buttons";
	const char* txt2  = "x: release buttons";
	const char* txt3  = "b: draw bounding box";
	const char* txt4  = "l: open locks";
	const char* txt5  = "k: open skull blocks";
	const char* txt6  = "t: toggle speed";
	const char* txt7  = "g: toggle transparency";
	const char* txt8  = "w: toggle window mode";
	const char* txt9  = "i: toggle debug info";
	const char* txt10 = "u: go up";
	const char* txt11 = "r: back to start";
	const char* txt12 = "v: set invisible";
	const char* txt13 = "e: set energy to max";
	const char* txt14 = "end: end level";
	const char* txt15 = "lshift: set rooster";

	const char* txts[] = { txt15,txt14,txt13,txt12,txt11,txt10,txt9,txt8,txt7,txt6,txt5,txt4,txt3,txt2,txt1 };
	int nof_txt = sizeof(txts) / sizeof(const char*);

	size_t last_size = 0;
	for (int i = 0; i < nof_txt; i++)
		if (strlen(txts[i]) > last_size) last_size = strlen(txts[i]);

	int posx = screen_width - last_size * char_w;
	int posy = screen_height - char_h * nof_txt;

	PisteDraw2_ScreenFill(posx - 4, posy - 4, screen_width, screen_height, 0);
	PisteDraw2_ScreenFill(posx - 2, posy - 2, screen_width, screen_height, 38);
	
	for (int i = 0; i < nof_txt; i++)
		PisteDraw2_Font_Write(FONT_SMALL, txts[i], posx, screen_height - (i+1)*10);

	return 0;
}
int PK_Draw_InGame_BG(){
	int pallarx = (Game::camera_x%(640*3))/3;
	int pallary = (Game::camera_y%(480*3))/3;

	PisteDraw2_ScreenFill(34);//0

	if (kartta->tausta == TAUSTA_STAATTINEN){
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0,0);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640,0);
	}

	if (kartta->tausta == TAUSTA_PALLARX_HORI){
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0   - pallarx,0);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640 - pallarx,0);

		if (screen_width > 640)
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640*2 - pallarx,0);
	}

	if (kartta->tausta == TAUSTA_PALLARX_VERT){
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0,0   - pallary);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0,480 - pallary);

		if (screen_width > 640){
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640,0   - pallary);
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640,480 - pallary);
		}
	}

	if (kartta->tausta == TAUSTA_PALLARX_VERT_JA_HORI){
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0   - pallarx, 0-pallary);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640 - pallarx, 0-pallary);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0   - pallarx, 480-pallary);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640 - pallarx, 480-pallary);

		if (screen_width > 640){
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640*2 - pallarx,0-pallary);
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640*2 - pallarx,480-pallary);
		}
	}

	return 0;
}

int PK_Draw_InGame_Gifts(){
	int x,y;

	y = screen_height-35;//36
	x = item_paneeli_x + 35;//40

	BYTE v1, v2;

	for (int i=0;i<MAX_GIFTS;i++)
		if (Game::Gifts->get(i) != -1){

			if (i == 0) {
				v1 = 31;
				v2 = 16 + 128;
			}
			else {
				v1 = 0;
				v2 = 16;
			}

			Game::Gifts->draw(i, x, y);
			//Game::Gifts->get_protot(i)->Piirra(x-esineet[i]->leveys/2,y-esineet[i]->korkeus/2,0);
			x += 38;
		}


	return 0;
}
int PK_Draw_InGame_Lower_Menu(){
	char luku[15];
	int vali = 0;

	int x, y;

	// Draw time
	if (timeout > 0){
		timeout += increase_time;
		float shown_sec = (float)(timeout * TIME_FPS + sekunti) / 60;
		int min = (int)shown_sec/60,
			sek = (int)shown_sec%60;

		x = screen_width / 2 - 546 / 2 + 342;
		y = screen_height-39;
		PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.game_time),x,y-20);

		itoa(min,luku,10);
		PisteDraw2_Font_Write(        FONT_BLACK,luku,x+1,y+1);
		vali += PisteDraw2_Font_Write(FONT_BIG,luku,x,y);
		vali += PisteDraw2_Font_Write(FONT_SMALL,":",x+vali,y);

		if (increase_time > 0 || increase_time < 0) {
			itoa((int)(increase_time * TIME_FPS) / 60, luku, 10);
			//PK_Fadetext_New(FONT_BIG, luku, x + vali, y, 49, true);
			increase_time = 0;
		}

		if (sek < 10){
			PisteDraw2_Font_Write(        FONT_BLACK,"0",x+vali+1,y+1);
			vali += PisteDraw2_Font_Write(FONT_BIG,"0",x+vali,y);
		}
		itoa(sek,luku,10);

		PisteDraw2_Font_Write(        FONT_BLACK,luku,x+vali+1,y+1);
		vali += PisteDraw2_Font_Write(FONT_BIG,luku,x+vali,y);
	}

	// Draw keys
	if (avaimia > 0){
		x = screen_width / 2 - 546 / 2 + 483;
		y = screen_height-39;
		PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.game_keys),x,y-20);

		itoa(avaimia,luku,10);
		PisteDraw2_Font_Write(FONT_BLACK,luku,x+1,y+1);
		PisteDraw2_Font_Write(FONT_BIG,luku,x,y);
	}

	// Draw Gifts
	if (Game::Gifts->count() > 0 && item_paneeli_x < 10)
		item_paneeli_x++;

	if (Game::Gifts->count() == 0 && item_paneeli_x > -215)
		item_paneeli_x--;

	if (item_paneeli_x > -215)
		PisteDraw2_Image_CutClip(kuva_peli,item_paneeli_x,screen_height-60,
		                        1,216,211,266);
	if (item_paneeli_x > 5)
		PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.game_items),15,screen_height-65);

	PK_Draw_InGame_Gifts();

	return 0;
}

int PK_Draw_InGame_UI(){
	char luku[15];
	int vali = 20;
	int my = 8;

	// Draw Energy
	vali = PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.game_energy),40,my);
	ltoa(Game::Sprites->player->energia,luku,10);
	PisteDraw2_Font_Write(FONT_BLACK,luku,40+vali+1,my+1);
	PisteDraw2_Font_Write(FONT_BIG,luku,40+vali,my);

	// Draw Invisible
	if(nakymattomyys > 0){
		vali = PisteDraw2_Font_Write(FONT_SMALL,"invisible:",40,my+27);
		ltoa(nakymattomyys/60,luku,10);
		PisteDraw2_Font_Write(FONT_BIG,luku,40+vali+1,my+27+1);
		PisteDraw2_Font_Write(FONT_BIG,luku,40+vali,my+27);
	}

	// Draw Score
	if (settings.isWide) {
		vali = PisteDraw2_Font_Write(FONT_SMALL, tekstit->Hae_Teksti(PK_txt.game_score), 230, my);

		ltoa(jakso_pisteet, luku, 10);
		PisteDraw2_Font_Write(FONT_BLACK, luku, 230 + vali + 1, my + 1);
		PisteDraw2_Font_Write(FONT_BIG, luku, 230 + vali, my);
	} else {
		vali = PisteDraw2_Font_Write(FONT_SMALL, tekstit->Hae_Teksti(PK_txt.game_score), 190, my);

		ltoa(jakso_pisteet, luku, 10);
		PisteDraw2_Font_Write(FONT_BLACK, luku, 190 + vali + 1, my + 1);
		PisteDraw2_Font_Write(FONT_BIG, luku, 190 + vali, my);
	}

	// Draw Ammunition
	if (Game::Sprites->player->ammus2 != -1) {
		PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.game_attack1), screen_width-170,my);
		Game::Sprites->prototypes[Game::Sprites->player->ammus2].Piirra(screen_width-170,my+10,0);
	}

	if (Game::Sprites->player->ammus1 != -1) {
		PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.game_attack2), screen_width-90,my);
		Game::Sprites->prototypes[Game::Sprites->player->ammus1].Piirra(screen_width-90,my+10,0);
	}

	// Draw Info
	if (info_timer > 0){
		int ilm_pituus = info.length() * 8 + 8; // 300

		RECT alue = {screen_width/2-(ilm_pituus/2),60,screen_width/2+(ilm_pituus/2),60+20};

		if (info_timer < 20){
			alue.top	+= (20 - info_timer) / 2;
			alue.bottom -= (20 - info_timer) / 2;
		}

		if (info_timer > MAX_ILMOITUKSENNAYTTOAIKA - 20){
			alue.top	+= 10 - (MAX_ILMOITUKSENNAYTTOAIKA - info_timer) / 2;
			alue.bottom -= 10 - (MAX_ILMOITUKSENNAYTTOAIKA - info_timer) / 2;
		}

		PisteDraw2_ScreenFill(alue.left-1,alue.top-1,alue.right+1,alue.bottom+1,51);
		PisteDraw2_ScreenFill(alue.left,alue.top,alue.right,alue.bottom,38);

		// TODO let sprite creators set x and y value?
		if (info_timer >= 100)
			PisteDraw2_Font_Write(FONT_SMALL,info,alue.left+4,alue.top+4);
		else
			PisteDraw2_Font_WriteAlpha(FONT_SMALL,info,alue.left+4,alue.top+4,info_timer);
	}

	// Draw collectables
	if (collectable_settings.show_in_game && show_collectables) {
		if (settings.isWide) {
			PisteDraw2_Font_Write(FONT_SMALL, tekstit->Hae_Teksti(PK_txt.game_collectables), screen_width - 350, my);
		} else {
			PisteDraw2_Font_Write(FONT_SMALL, tekstit->Hae_Teksti(PK_txt.game_collectables), screen_width - 320, my);
		}

		for (int j = 0; j < EPISODI_MAX_LEVELS; j++) {
			for (int i = 0; i < level_stats.collectables_collected[jakso_indeksi_nyt]; i++) {
				if (settings.isWide) {
					PisteDraw2_Image_Clip(collectable_settings.index_game_image, screen_width - 350 + (i * collectable_settings.gameW) + 2, my + 10);
				} else {
					PisteDraw2_Image_Clip(collectable_settings.index_game_image, screen_width - 320 + (i * collectable_settings.gameW) + 2, my + 10);
				}
			}
		}
	}

	return 0;
}
int PK_Draw_InGame(){
	char luku[15];
	int vali = 20;

	if (!skip_frame){

		PK_Draw_InGame_BG();

		if (settings.tausta_spritet)
			PK_Draw_InGame_BGSprites();

		Game::Particles->draw_bg_particles();

		kartta->Piirra_Taustat(Game::camera_x,Game::camera_y,false);

		PK_Draw_InGame_Sprites();

		//PK_Particles_Draw();
		Game::Particles->draw_front_particles();

		kartta->Piirra_Seinat(Game::camera_x,Game::camera_y, false);

		if (settings.nayta_tavarat)
			PK_Draw_InGame_Lower_Menu();

		PK_Fadetext_Draw();

		PK_Draw_InGame_UI();

		if (draw_dubug_info)
			PK_Draw_InGame_DebugInfo();
		else {
			if (dev_mode)
				PK_Draw_InGame_DevKeys();
			if (test_level)
				PisteDraw2_Font_Write(FONT_SMALL, "testing level", 0, 480 - 20);
			if (show_fps) {
				if (fps >= 100)
					vali = PisteDraw2_Font_Write(FONT_SMALL, "fps:", 570, 48);
				else
					vali = PisteDraw2_Font_Write(FONT_SMALL, "fps: ", 570, 48);
				fps = Engine->get_fps();
				itoa((int)fps, luku, 10);
				PisteDraw2_Font_Write(FONT_SMALL, luku, 570 + vali, 48);
			}
		}

		if (paused)
			PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.game_paused),screen_width/2-82,screen_height/2-9);

		if (jakso_lapaisty)
			PK_Wavetext_Draw(tekstit->Hae_Teksti(PK_txt.game_clear),FONT_BIG,screen_width/2-120,screen_height/2-9);
		else
			if (game_over){
				if (Game::Sprites->player->energia < 1) {
					PK_Wavetext_Draw(tekstit->Hae_Teksti(PK_txt.game_ko), FONT_BIG, screen_width / 2 - 90, screen_height / 2 - 9 - 10);
					
					if (checkpoint_data != nullptr) {
						// TODO Add controller support and custom text
						PisteDraw2_Font_Write(FONT_SMALL, "ENTER to restart at checkpoint", screen_width / 2 - 115, screen_height / 2 + 40);
						PisteDraw2_Font_Write(FONT_SMALL, "SPACE to return to map", screen_width / 2 - 85, screen_height / 2 + 60);
					}
				}

				else
					if (timeout < 1 && aikaraja)
						PK_Wavetext_Draw(tekstit->Hae_Teksti(PK_txt.game_timeout),FONT_BIG,screen_width/2-67,screen_height/2-9-10);

				PK_Wavetext_Draw(tekstit->Hae_Teksti(PK_txt.game_tryagain),FONT_BIG,screen_width/2-75,screen_height/2-9+10);
			}
	}
	
	if (skip_frame) Engine->ignore_frame();

	if (doublespeed) skip_frame = !skip_frame;
	else skip_frame = false;

	palikka_animaatio = 1 + palikka_animaatio % 34;

	return 0;
}

int PK_Draw_Cursor(int x,int y){
	#ifndef __ANDROID__

	if(settings.isFullScreen)
		PisteDraw2_Image_CutClip(kuva_peli,x,y,621,461,640,480);

	#endif

	return 0;
}

int  PK_Draw_Menu_Square(int vasen, int yla, int oikea, int ala, BYTE pvari){
	if (episode_started)
		return 0;

	//pvari = 224;

	if (menunelio.left < vasen)
		menunelio.left++;

	if (menunelio.left > vasen)
		menunelio.left--;

	if (menunelio.right < oikea)
		menunelio.right++;

	if (menunelio.right > oikea)
		menunelio.right--;

	if (menunelio.top < yla)
		menunelio.top++;

	if (menunelio.top > yla)
		menunelio.top--;

	if (menunelio.bottom < ala)
		menunelio.bottom++;

	if (menunelio.bottom > ala)
		menunelio.bottom--;

	vasen = (int)menunelio.left;
	oikea = (int)menunelio.right;
	yla	= (int)menunelio.top;
	ala = (int)menunelio.bottom;

	vasen += (int)(sin_table[(degree*2)%359]/2.0);
	oikea += (int)(cos_table[(degree*2)%359]/2.0);
	yla	+= (int)(sin_table[((degree*2)+20)%359]/2.0);
	ala += (int)(cos_table[((degree*2)+40)%359]/2.0);

	//PisteDraw2_ScreenFill(vasen,yla,oikea,ala,38);

	double kerroin_y = (ala - yla) / 19.0;
	double kerroin_x = (oikea - vasen) / 19.0;
	double dbl_y = yla;
	double dbl_x = vasen;
	bool tumma = true;
	int vari = 0;

	for (int y=0;y<19;y++) {

		dbl_x = vasen;

		for (int x=0;x<19;x++) {
			//vari = (x+y) / 6;
			vari = (x/4)+(y/3);
			if (tumma) vari /= 2;

			PisteDraw2_ScreenFill(int(dbl_x),int(dbl_y),int(dbl_x+kerroin_x),int(dbl_y+kerroin_y),pvari+vari);
			dbl_x += kerroin_x;
			tumma = !tumma;
		}
		dbl_y += kerroin_y;
	}

	PisteDraw2_ScreenFill(vasen-1,yla-1,oikea+1,yla+2,0);
	PisteDraw2_ScreenFill(vasen-1,yla-1,vasen+2,ala+1,0);
	PisteDraw2_ScreenFill(vasen-1,ala-2,oikea+1,ala+1,0);
	PisteDraw2_ScreenFill(oikea-2,yla-1,oikea+1,ala+1,0);

	PisteDraw2_ScreenFill(vasen-1+1,yla-1+1,oikea+1+1,yla+2+1,0);
	PisteDraw2_ScreenFill(vasen-1+1,yla-1+1,vasen+2+1,ala+1+1,0);
	PisteDraw2_ScreenFill(vasen-1+1,ala-2+1,oikea+1+1,ala+1+1,0);
	PisteDraw2_ScreenFill(oikea-2+1,yla-1+1,oikea+1+1,ala+1+1,0);

	PisteDraw2_ScreenFill(vasen,yla,oikea,yla+1,153);
	PisteDraw2_ScreenFill(vasen,yla,vasen+1,ala,144);
	PisteDraw2_ScreenFill(vasen,ala-1,oikea,ala,138);
	PisteDraw2_ScreenFill(oikea-1,yla,oikea,ala,138);

	return 0;
}
bool PK_Draw_Menu_Text(bool active, std::string text, int x, int y, int kd = 20) {
	if(!active){
		PK_WavetextSlow_Draw(text, FONT_BIG, x, y);

		return false;
	}

	int length = text.length() * 15;

	// hiiri = mouse
	if ((hiiri_x > x && hiiri_x < x + length && hiiri_y > y && hiiri_y < y + 20) ||
		(menu_valittu_id == menu_valinta_id)) {
		menu_valittu_id = menu_valinta_id;

		if (((PisteInput_Hiiri_Vasen() && hiiri_x > x && hiiri_x < x + length && hiiri_y > y - 2 && hiiri_y < y + 22)
			|| PisteInput_Keydown(PI_SPACE) || PisteInput_Gamepad_Button(PI_CONTROLLER_A))
			&& key_delay == 0) {
				PK_Play_MenuSound(menu_aani, 100);
				
				key_delay = kd;
				
				menu_valinta_id++;

				return true;
		}

		PK_Wavetext_Draw(text, FONT_WHITE, x, y);
	} else {
		PK_WavetextSlow_Draw(text, FONT_BIG, x, y);
	}

	menu_valinta_id++;

	return false;
}
int  PK_Draw_Menu_BoolBox(int x, int y, bool muuttuja, bool active){
	PD_RECT img_src, img_dst = {(DWORD)x,(DWORD)y,0,0};

	if(muuttuja) img_src = {504,124,31,31};
	else img_src = {473,124,31,31};

	if(active){
		PisteDraw2_Image_CutClip(kuva_peli,img_src,img_dst);
	} else{
		PisteDraw2_Image_CutClipTransparent(kuva_peli,img_src,img_dst,50);
		return false;
	}

	if (hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31){
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE) || PisteInput_Gamepad_Button(PI_CONTROLLER_A))
			&& key_delay == 0){

			PK_Play_MenuSound(menu_aani, 100);
			key_delay = 20;
			return true;
		}
	}

	return false;
}
int  PK_Draw_Menu_BackNext(int x, int y){
	int val = 45;

	int randx = rand()%3 - rand()%3;
	int randy = rand()%3 - rand()%3;

	if (menu_valittu_id == menu_valinta_id)
		PisteDraw2_Image_CutClip(kuva_peli,x+randx,y+randy,566,124,566+31,124+31);
	else
		PisteDraw2_Image_CutClip(kuva_peli,x,y,566,124,566+31,124+31);

	if (menu_valittu_id == menu_valinta_id+1)
		PisteDraw2_Image_CutClip(kuva_peli,x+val+randx,y+randy,535,124,535+31,124+31);
	else
		PisteDraw2_Image_CutClip(kuva_peli,x+val,y,535,124,535+31,124+31);

	if ((hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31) || (menu_valittu_id == menu_valinta_id)){
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE) || PisteInput_Gamepad_Button(PI_CONTROLLER_A))
			&& key_delay == 0){
			PK_Play_MenuSound(menu_aani, 100);
			key_delay = 20;
			return 1;
		}
	}

	x += val;

	if ((hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31) || (menu_valittu_id == menu_valinta_id+1)){
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE) || PisteInput_Gamepad_Button(PI_CONTROLLER_A))
			&& key_delay == 0){
			PK_Play_MenuSound(menu_aani, 100);
			key_delay = 20;
			return 2;
		}
	}

	menu_valinta_id += 2;

	return 0;
}

int PK_Draw_Menu_Main(){
	int my = 240;//250;
	#ifdef __ANDROID__
	my = 260;
	#endif

	PK_Draw_Menu_Square(160, 200, 640-180, 410, 224);

	PK_Search_Records("data/saves.dat");

	if (last_save != -1 && !episode_started) {
		if (PK_Draw_Menu_Text(true, tekstit->Hae_Teksti(PK_txt.mainmenu_continue), 180, my)) {
			PK_Load_Records(last_save);
		}

		my += 20;
	}

	if (episode_started) {
		if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_continue),180,my)){
			if ((!game_over && !jakso_lapaisty) || lopetusajastin > 1)
				game_next_screen = SCREEN_GAME;
			else
				game_next_screen = SCREEN_MAP;

		}
		my += 20;
	}

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_new_game),180,my)){
		name_edit = true;
		menu_name_index = strlen(pelaajan_nimi);//   0;
		menu_name_last_mark = ' ';
		menu_nyt = MENU_NAME;
		key_delay = 30;

		if (checkpoint_data != nullptr) {
			delete checkpoint_data;
			checkpoint_data = nullptr;
		}
	}
	my += 20;

	if (episode_started){
		if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_save_game),180,my)){
			menu_nyt = MENU_TALLENNA;
		}
		my += 20;
	}

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_load_game),180,my)){
		menu_nyt = MENU_LOAD;
	}
	my += 20;

	if (PK_Draw_Menu_Text(true,"load language",180,my)){
		menu_nyt = MENU_LANGUAGE;
	}
	my += 20;

	#ifndef __ANDROID__
	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_controls),180,my)){
		menu_nyt = MENU_CONTROLS;
	}
	my += 20;
	#endif

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_graphics),180,my)){
		menu_nyt = MENU_GRAPHICS;
	}
	my += 20;

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_sounds),180,my)){
		menu_nyt = MENU_SOUNDS;
	}
	my += 20;

	#ifndef __ANDROID__
	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_exit),180,my))
		PK_Fade_Quit();
	my += 20;
	#endif
	return 0;
}
int PK_Draw_Menu_Name(){
	int mx; //Text cursor
	char merkki;
	bool hiiri_alueella = false;
	int nameSize = (int) strlen(pelaajan_nimi);

	PK_Draw_Menu_Square(90, 150, 640-90, 480-100, 224);

	if (hiiri_x > 180 && hiiri_x < 180+15*20 && hiiri_y > 255 && hiiri_y < 255+18)
		hiiri_alueella = true; //Mouse is in text

	if (hiiri_alueella && PisteInput_Hiiri_Vasen() && key_delay == 0){
		name_edit = true;
		menu_name_index = (hiiri_x - 180)/15; //Set text cursor with the mouse
		key_delay = 10;
	}

	if (name_edit && key_delay == 0){

		if (PisteInput_Keydown(PI_LEFT)) {
			menu_name_index--;
			key_delay = 8;
		}

		if (PisteInput_Keydown(PI_RIGHT)) {
			menu_name_index++;
			key_delay = 8;
		}
	}

	if (menu_name_index >= 20)
		menu_name_index = 19;

	if (menu_name_index >= nameSize)
		menu_name_index = nameSize;

	if (menu_name_index < 0)
		menu_name_index = 0;


	PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.playermenu_type_name),180,224);

	PisteDraw2_ScreenFill(180 - 2, 255 - 2, 180 + 19 * 15 + 4, 255 + 18 + 4, 0);
	PisteDraw2_ScreenFill(180, 255, 180 + 19 * 15, 255 + 18, 50);

	if (name_edit) { //Draw text cursor
		mx = menu_name_index*15 + 180 + rand()%2; //Text cursor x

		PisteDraw2_ScreenFill(mx-2, 254, mx + 15, 254 + 20 + 3, 0);
		PisteDraw2_ScreenFill(mx, 254, mx + 12, 254 + 20, 96 + 8);
		PisteDraw2_ScreenFill(mx-1, 254, mx + 11, 254 + 20, 96 + 16);
	}

	PK_WavetextSlow_Draw(pelaajan_nimi,FONT_BIG,180,255);
	PisteDraw2_Font_WriteAlpha(FONT_WHITE,pelaajan_nimi,180,255,15);

	merkki = PisteInput_Get_Typed_Char();

	if (merkki != '\0' && (merkki != menu_name_last_mark || key_delay == 0) && name_edit && nameSize < 19) {
		menu_name_last_mark = merkki; // Don't write the same letter many times
		key_delay = 15;

		for(int c = nameSize; c > menu_name_index; c--)
			pelaajan_nimi[c] = pelaajan_nimi[c-1];

		pelaajan_nimi[menu_name_index] = merkki;
		menu_name_index++;
	}

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.playermenu_continue),180,300)) {
		menu_nyt = MENU_EPISODES;
		menu_name_index = 0;
		name_edit = false;
		menu_valittu_id = menu_valinta_id = 1;

		if (episodi_lkm == 1) {
			current_episode = episodit[2];
			
			game_next_screen = SCREEN_MAP;
			episode_started = false;
			PK_New_Game();
		}
	}

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.playermenu_clear),340,300)) {
		memset(pelaajan_nimi,'\0',sizeof(pelaajan_nimi));
		menu_name_index = 0;
	}

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_exit),180,400)) {
		menu_nyt = MENU_MAIN;
		menu_name_index = 0;
		name_edit = false;
	}

	if (key_delay == 0) {
		if (PisteInput_Keydown(PI_DELETE)) {
			for (int c = menu_name_index; c<19; c++)
				pelaajan_nimi[c] = pelaajan_nimi[c + 1];
			pelaajan_nimi[19] = '\0';
			key_delay = 10;
		}

		if (PisteInput_Keydown(PI_BACK) && menu_name_index != 0) {
			for (int c = menu_name_index - 1; c<19; c++)
				pelaajan_nimi[c] = pelaajan_nimi[c + 1];
			pelaajan_nimi[19] = '\0';
			if (pelaajan_nimi[menu_name_index] == '\0') pelaajan_nimi[menu_name_index - 1] = '\0';
			menu_name_index--;
			key_delay = 10;
		}

		if (PisteInput_Keydown(PI_RETURN) && pelaajan_nimi[0] != '\0') {
			key_delay = 10;
			merkki = '\0';
			name_edit = false;
			menu_valittu_id = 1;
		}

		if (PisteInput_Gamepad_Button(PI_CONTROLLER_A) && pelaajan_nimi[0] != '\0') {
			key_delay = 10;
			merkki = '\0';
			name_edit = false;
			menu_valittu_id = 1;
		}
	}

	return 0;
}
int PK_Draw_Menu_Load(){
	int my = 0, vali = 0;
	char tpaikka[100];
	char jaksoc[8];
	char ind[4];

	PK_Draw_Menu_Square(40, 70, 640-40, 410, 70);

	PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.loadgame_title),50,90);
	PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.loadgame_info),50,110);
	my = -20;

	for ( int i = 0; i < MAX_SAVES; i++ ) {
		itoa(i+1,ind,10);
		strcpy(tpaikka,ind);
		strcat(tpaikka,". ");

		strcat(tpaikka, saves_list[i].nimi);

		if (PK_Draw_Menu_Text(true, tpaikka, 100, 150 + my)) {
			PK_Load_Records(i);
		}

		if (strcmp(saves_list[i].episodi," ")!=0) {
			vali = 0;
			vali = PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.loadgame_episode),400,150+my);
			vali += PisteDraw2_Font_Write(FONT_SMALL, saves_list[i].episodi,400+vali,150+my);
			vali = 0;
			vali += PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.loadgame_level),400+vali,160+my);
			itoa(saves_list[i].current_level, jaksoc,10);
			vali += PisteDraw2_Font_Write(FONT_SMALL,jaksoc,400+vali,160+my);
		}

		my += 22;
	}

	my += 20;

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),180,400))
		menu_nyt = MENU_MAIN;

	return 0;
}
int PK_Draw_Menu_Save(){
	int my = 0, vali = 0;
	char tpaikka[100];
	char jaksoc[8];
	char ind[4];

	PK_Draw_Menu_Square(40, 70, 640-40, 410, 224);

	PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.savegame_title),50,90);
	PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.savegame_info),50,110);
	my = -20;

	for (int i = 0; i < MAX_SAVES; i++) {
		itoa(i + 1, ind, 10);
		strcpy(tpaikka, ind);
		strcat(tpaikka, ". ");

		strcat(tpaikka, saves_list[i].nimi);

		if (PK_Draw_Menu_Text(true, tpaikka, 100, 150 + my)) {
			last_save = i;
			PK_Save_Records(i);
		}

		if (strcmp(saves_list[i].episodi," ")!=0)
		{
			vali = 0;
			vali = PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.savegame_episode),400,150+my);
			vali += PisteDraw2_Font_Write(FONT_SMALL, saves_list[i].episodi,400+vali,150+my);
			vali = 0;
			vali += PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.savegame_level),400+vali,160+my);
			itoa(saves_list[i].current_level, jaksoc, 10);
			vali += PisteDraw2_Font_Write(FONT_SMALL,jaksoc,400+vali,160+my);
		}

		my += 22;
	}

	my += 20;

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),180,400))
		menu_nyt = MENU_MAIN;

	return 0;
}

int PK_Draw_Menu_Graphics(){
	bool wasFullScreen, wasFiltered, wasFit, wasWide;
	int my = 150;
	static bool moreOptions = false;

	PK_Draw_Menu_Square(40, 70, 640-40, 410, 224);

	PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.gfx_title),50,90);

	if(moreOptions){
		wasFullScreen = settings.isFullScreen;
		wasFiltered = settings.isFiltered;
		wasFit = settings.isFit;
		wasWide = settings.isWide;

		if (settings.isFullScreen){
			if (PK_Draw_Menu_Text(true,"fullscreen mode is on",180,my)){
				settings.isFullScreen = false;
			}
		} else{
			if (PK_Draw_Menu_Text(true,"fullscreen mode is off",180,my)){
				settings.isFullScreen = true;
			}
		}
		if (PK_Draw_Menu_BoolBox(100, my, settings.isFullScreen, true)) {
			settings.isFullScreen = !settings.isFullScreen;
		}
		my += 30;

		if (settings.isFiltered){
			if (PK_Draw_Menu_Text(true,"bilinear filter is on",180,my)){
				settings.isFiltered = false;
			}
		} else{
			if (PK_Draw_Menu_Text(true,"bilinear filter is off",180,my)){
				settings.isFiltered = true;
			}
		}
		if (PK_Draw_Menu_BoolBox(100, my, settings.isFiltered, true)) {
			settings.isFiltered = !settings.isFiltered;
		}
		my += 30;

		if (settings.isFit){
			if (PK_Draw_Menu_Text(true,"screen fit is on",180,my)){
				settings.isFit = false;
			}
		} else{
			if (PK_Draw_Menu_Text(true,"screen fit is off",180,my)){
				settings.isFit = true;
			}
		}
		if (PK_Draw_Menu_BoolBox(100, my, settings.isFit, true)) {
			settings.isFit = !settings.isFit;
		}
		my += 30;

		bool res_active = true;

		if (settings.isWide) {
			if (PK_Draw_Menu_Text(res_active,"screen size 800x480", 180, my)) {
				settings.isWide = false;
			}
		}
		else {
			if (PK_Draw_Menu_Text(res_active,"screen size 640x480", 180, my)) {
				settings.isWide = true;
			}
		}
		if (PK_Draw_Menu_BoolBox(100, my, settings.isWide, res_active)) {
			settings.isWide = !settings.isWide;
		}
		my += 30;

		if (settings.show_intro) {
			if (PK_Draw_Menu_Text(res_active, "Startup intro is on", 180, my)) {
				settings.show_intro = false;
			}
		} else {
			if (PK_Draw_Menu_Text(res_active, "Startup intro is off", 180, my)) {
				settings.show_intro = true;
			}
		}
		if (PK_Draw_Menu_BoolBox(100, my, settings.show_intro, res_active)) {
			settings.show_intro = !settings.show_intro;
		}
		my += 30;

		//Can add more options here

		if(wasFullScreen != settings.isFullScreen) // If fullscreen changes
			PisteDraw2_FullScreen(settings.isFullScreen);

		if(wasFiltered && !settings.isFiltered) // If filter changes
			PisteDraw2_SetFilter(PD_FILTER_NEAREST);
		if(!wasFiltered && settings.isFiltered)
			PisteDraw2_SetFilter(PD_FILTER_BILINEAR);

		if(wasFit != settings.isFit) // If fit changes
			PisteDraw2_FitScreen(settings.isFit);

		if (wasWide != settings.isWide) {
			screen_width = settings.isWide ? 800 : 640;
			PK2Kartta_Aseta_Ruudun_Mitat(screen_width, screen_height);
			PisteDraw2_ChangeResolution(screen_width,screen_height);
			
			if(episode_started)
				PisteDraw2_ImageFill(kuva_tausta, 0);
			
			if (settings.isWide) PisteDraw2_SetXOffset(80);
			else PisteDraw2_SetXOffset(0);
		}

		if (PK_Draw_Menu_Text(true,"back",100,360)){
			moreOptions = false;
			menu_valittu_id = 0; //Set menu cursor to 0
		}

	}
	else {

		if (settings.lapinakyvat_objektit){
			if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_tfx_on),180,my))
				settings.lapinakyvat_objektit = false;
		} else{
			if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_tfx_off),180,my))
				settings.lapinakyvat_objektit = true;
		}
		if (PK_Draw_Menu_BoolBox(100, my, settings.lapinakyvat_objektit, true)) {
			settings.lapinakyvat_objektit = !settings.lapinakyvat_objektit;
		}
		my += 30;


		if (settings.lapinakyvat_menutekstit){
			if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_tmenus_on),180,my))
				settings.lapinakyvat_menutekstit = false;
		} else{
			if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_tmenus_off),180,my))
				settings.lapinakyvat_menutekstit = true;
		}
		if (PK_Draw_Menu_BoolBox(100, my, settings.lapinakyvat_menutekstit, true)) {
			settings.lapinakyvat_menutekstit = !settings.lapinakyvat_menutekstit;
		}
		my += 30;


		if (settings.nayta_tavarat){
			if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_items_on),180,my))
				settings.nayta_tavarat = false;
		} else{
			if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_items_off),180,my))
				settings.nayta_tavarat = true;
		}
		if (PK_Draw_Menu_BoolBox(100, my, settings.nayta_tavarat, true)) {
			settings.nayta_tavarat = !settings.nayta_tavarat;
		}
		my += 30;


		if (settings.saa_efektit){
			if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_weather_on),180,my))
				settings.saa_efektit = false;
		} else{
			if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_weather_off),180,my))
				settings.saa_efektit = true;
		}
		if (PK_Draw_Menu_BoolBox(100, my, settings.saa_efektit, true)) {
			settings.saa_efektit = !settings.saa_efektit;
		}
		my += 30;


		if (settings.tausta_spritet){
			if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_bgsprites_on),180,my))
				settings.tausta_spritet = false;
		} else{
			if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_bgsprites_off),180,my))
				settings.tausta_spritet = true;
		}
		if (PK_Draw_Menu_BoolBox(100, my, settings.tausta_spritet, true)) {
			settings.tausta_spritet = !settings.tausta_spritet;
		}
		my += 30;


		if (doublespeed){
			if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_speed_double),180,my))
				doublespeed = false;
		} else{
			if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.gfx_speed_normal),180,my))
				doublespeed = true;
		}
		if (PK_Draw_Menu_BoolBox(100, my, doublespeed, true)) {
			doublespeed = !doublespeed;
		}
		my += 30;

		#ifndef __ANDROID__
		if (PK_Draw_Menu_Text(true,"more",100,360)){
			moreOptions = true;
			menu_valittu_id = 0; //Set menu cursor to 0
		}
		#endif

	}

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),180,400)){
		menu_nyt = MENU_MAIN;
		moreOptions = false;
	}

	return 0;
}
int PK_Draw_Menu_Sounds(){
	int my = 0;

	PK_Draw_Menu_Square(40, 70, 640-40, 410, 224);

	PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.sound_title), 50, 90);
	my += 20;

	PisteDraw2_ScreenFill(404, 224 + my, 404 + settings.sfx_max_volume, 244 + my, 0);
	PisteDraw2_ScreenFill(400, 220 + my, 400 + settings.sfx_max_volume, 240 + my, 81);

	PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.sound_sfx_volume), 150, 195 + my);
	my += 20;

	if (settings.sfx_max_volume == 0) {
		if (PK_Draw_Menu_Text(true, "Unmute", 80, 200 + my))
			settings.sfx_max_volume = 64;
	} else {
		if (PK_Draw_Menu_Text(true, "Mute", 80, 200 + my))
			settings.sfx_max_volume = 0;
	}

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.sound_less),180,200+my, 5))
		if (settings.sfx_max_volume > 0)
			settings.sfx_max_volume -= 4;

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.sound_more),180+8*15,200+my, 5))
		if (settings.sfx_max_volume < 100)
			settings.sfx_max_volume += 4;

	if (settings.sfx_max_volume < 0)
		settings.sfx_max_volume = 0;

	if (settings.sfx_max_volume > 100)
		settings.sfx_max_volume = 100;

	my+=40;

	PisteDraw2_ScreenFill(404, 224 + my, 404 + int(settings.music_max_volume * 1.56), 244 + my, 0);
	PisteDraw2_ScreenFill(400, 220 + my, 400 + int(settings.music_max_volume * 1.56), 240 + my, 112);

	PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.sound_music_volume), 200, 195 + my);
	my += 20;

	if (settings.music_max_volume == 0) {
		if (PK_Draw_Menu_Text(true, "Unmute", 80, 200 + my)) {
			settings.music_max_volume = 64;
		}
	} else {
		if (PK_Draw_Menu_Text(true, "Mute", 80, 200 + my)) {
			settings.music_max_volume = 0;
		}
	}

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.sound_less), 180, 200 + my, 5))
		if (settings.music_max_volume > 0)
			settings.music_max_volume -= 2;

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.sound_more), 180 + 8 * 15, 200 + my, 5))
		if (settings.music_max_volume < 64)
			settings.music_max_volume += 2;

	if (settings.music_max_volume < 0)
		settings.music_max_volume = 0;

	if (settings.music_max_volume > 64)
		settings.music_max_volume = 64;

	music_volume = settings.music_max_volume;

	my += 20;

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),180,400))
		menu_nyt = MENU_MAIN;

	return 0;
}
int PK_Draw_Menu_Controls(){
	int my = 0;

	PK_Draw_Menu_Square(40, 70, 640-40, 410, 224);

	PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.controls_title),50,60);

	if (PK_Draw_Menu_Text(true, "Keyboard", 50, 90)) {
		settingsControlKeyboard = true;
		settingsControlGamepad = false;
	}
	
	if (PK_Draw_Menu_Text(true, "Gamepad", 300, 90)) {
		settingsControlKeyboard = false;
		settingsControlGamepad = true;
	}

	my = 40;

	if (menu_lue_kontrollit > 0){
		PisteDraw2_ScreenFill(299,74+my+menu_lue_kontrollit*20,584,94+my+menu_lue_kontrollit*20,0);
		PisteDraw2_ScreenFill(295,70+my+menu_lue_kontrollit*20,580,90+my+menu_lue_kontrollit*20,50);
	}

	if (settingsControlKeyboard) {
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_moveleft), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_moveright), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_jump), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_duck), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_walkslow), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_eggattack), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_doodleattack), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_useitem), 100, 90 + my); my += 20;

		my = 40;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_KeyName(settings.control_left), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_KeyName(settings.control_right), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_KeyName(settings.control_jump), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_KeyName(settings.control_down), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_KeyName(settings.control_walk_slow), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_KeyName(settings.control_attack1), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_KeyName(settings.control_attack2), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_KeyName(settings.control_open_gift), 310, 90 + my); my += 20;

		if (PisteInput_Hiiri_Vasen()) {
			if (hiiri_x > 310 && hiiri_x < 580 && hiiri_y > 130 && hiiri_y < 350 - 20){
				menu_lue_kontrollit = (hiiri_y - 110) / 20;

				if (menu_lue_kontrollit < 0 || menu_lue_kontrollit > 8) {
					menu_lue_kontrollit = 0;
				} else {
					settingsEditSingle = true;
					
					key_delay = 5;
				}
			}
		}

		if (menu_lue_kontrollit == 0) {
			if (PK_Draw_Menu_Text(true, tekstit->Hae_Teksti(PK_txt.controls_edit), 100, 100 + my))
				menu_lue_kontrollit = 1;
				menu_valittu_id = 0; //Set menu cursor to 0
		}

		my += 30;

		if (PK_Draw_Menu_Text(true, tekstit->Hae_Teksti(PK_txt.controls_kbdef), 100, 100 + my)) {
			settings.control_left = PI_LEFT;
			settings.control_right = PI_RIGHT;
			settings.control_jump = PI_UP;
			settings.control_down = PI_DOWN;
			settings.control_walk_slow = PI_RALT;
			settings.control_attack1 = PI_RCONTROL;
			settings.control_attack2 = PI_RSHIFT;
			settings.control_open_gift = PI_SPACE;
			menu_lue_kontrollit = 0;
			menu_valittu_id = 0;
		}
	} else {
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_moveleft), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_moveright), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_jump), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_duck), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_walkslow), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_eggattack), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_doodleattack), 100, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.controls_useitem), 100, 90 + my); my += 20;

		my = 40;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_ButtonName(settings.gamepad_left), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_ButtonName(settings.gamepad_right), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_ButtonName(settings.gamepad_jump), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_ButtonName(settings.gamepad_down), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_ButtonName(settings.gamepad_walk_slow), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_ButtonName(settings.gamepad_attack1), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_ButtonName(settings.gamepad_attack2), 310, 90 + my); my += 20;
		PisteDraw2_Font_Write(FONT_BIG, PisteInput_ButtonName(settings.gamepad_open_gift), 310, 90 + my); my += 20;

		if (PisteInput_Hiiri_Vasen()) {
			if (hiiri_x > 310 && hiiri_x < 580 && hiiri_y > 130 && hiiri_y < 350 - 20) {
				menu_lue_kontrollit = (hiiri_y - 110) / 20;

				if (menu_lue_kontrollit < 0 || menu_lue_kontrollit > 8) {
					menu_lue_kontrollit = 0;
				} else {
					settingsEditSingle = true;

					key_delay = 5;
				}
			}
		}

		if (menu_lue_kontrollit == 0) {
			if (PK_Draw_Menu_Text(true, tekstit->Hae_Teksti(PK_txt.controls_edit), 100, 100 + my))
				menu_lue_kontrollit = 1;
			menu_valittu_id = 0; //Set menu cursor to 0
		}

		my += 30;

		if (PK_Draw_Menu_Text(true, "Gamepad defaults", 100, 100 + my)) {
			settings.gamepad_left = PI_CONTROLLER_LEFT;
			settings.gamepad_right = PI_CONTROLLER_RIGHT;
			settings.gamepad_jump = PI_CONTROLLER_B;
			settings.gamepad_down = PI_CONTROLLER_DOWN;
			settings.gamepad_walk_slow = PI_CONTROLLER_SHOULDER_LEFT;
			settings.gamepad_attack1 = PI_CONTROLLER_A;
			settings.gamepad_attack2 = PI_CONTROLLER_X;
			settings.gamepad_open_gift = PI_CONTROLLER_Y;

			menu_lue_kontrollit = 0;
			menu_valittu_id = 0;
		}
	}

	my += 20;

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),180,400)){
		menu_nyt = MENU_MAIN;
		menu_lue_kontrollit = 0;
		menu_valittu_id = 0;
	}

	BYTE k = 0;

	// TODO clean this up
	if (key_delay == 0 && menu_lue_kontrollit > 0){
		if (settingsControlKeyboard) {
			k = PisteInput_GetKey();

			if (k != 0) {
				switch (menu_lue_kontrollit) {
				case 1: settings.control_left = k; break;
				case 2: settings.control_right = k; break;
				case 3: settings.control_jump = k; break;
				case 4: settings.control_down = k; break;
				case 5: settings.control_walk_slow = k; break;
				case 6: settings.control_attack1 = k; break;
				case 7: settings.control_attack2 = k; break;
				case 8: settings.control_open_gift = k; break;
				default: PK_Play_MenuSound(ammuu_aani, 100); break;
				}

				if (!settingsEditSingle) {
					key_delay = 20;
					menu_lue_kontrollit++;

					settingsEditSingle = false;
				} else {
					menu_lue_kontrollit = 0;

					settingsEditSingle = false;
				}
			}

			if (menu_lue_kontrollit > 8) {
				menu_lue_kontrollit = 0;
				menu_valittu_id = 0;
			}
		} else {
			k = PisteInput_Gamepad_Get_Pressed();

			if (k != 255) {
				switch (menu_lue_kontrollit) {
				case 1: settings.gamepad_left = k; break;
				case 2: settings.gamepad_right = k; break;
				case 3: settings.gamepad_jump = k; break;
				case 4: settings.gamepad_down = k; break;
				case 5: settings.gamepad_walk_slow = k; break;
				case 6: settings.gamepad_attack1 = k; break;
				case 7: settings.gamepad_attack2 = k; break;
				case 8: settings.gamepad_open_gift = k; break;
				default: PK_Play_MenuSound(ammuu_aani, 100); break;
				}

				if (!settingsEditSingle) {
					key_delay = 20;
					menu_lue_kontrollit++;

					settingsEditSingle = false;
				} else {
					menu_lue_kontrollit = 0;

					settingsEditSingle = false;
				}
			}

			if (menu_lue_kontrollit > 8) {
				menu_lue_kontrollit = 0;
				menu_valittu_id = 0;
			}
		}
	}

	my += 20;

	return 0;
}

int PK_Draw_Menu_Episodes(){
	int my = 0;

	PK_Draw_Menu_Square(110, 130, 640-110, 450, 224);

	PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.episodes_choose_episode),50,90);
	my += 80;

	if (episodi_lkm-2 > 10) {
		char luku[20];
		int vali = 90;
		int x = 50,//500,
			y = 50;//300;
		//vali += PisteDraw2_Font_Write(FONT_SMALL,"page:",x,y+40);
		itoa(episodisivu+1,luku,10);
		vali += PisteDraw2_Font_Write(FONT_SMALL,luku,x+vali,y+20);
		vali += PisteDraw2_Font_Write(FONT_SMALL,"/",x+vali,y+20);
		itoa((episodi_lkm/10)+1,luku,10);
		vali += PisteDraw2_Font_Write(FONT_SMALL,luku,x+vali,y+20);

		int nappi = PK_Draw_Menu_BackNext(x,y);

		if (nappi == 1 && episodisivu > 0)
			episodisivu--;

		if (nappi == 2 && (episodisivu*10)+10 < episodi_lkm)
			episodisivu++;
	}

	for (int i=(episodisivu*10)+2;i<(episodisivu*10)+12;i++){
		if (strcmp(episodit[i],"") != 0){
			if (PK_Draw_Menu_Text(true,episodit[i],220,90+my)){
				current_episode = episodit[i];

				PK_Load_InfoText();
				game_next_screen = SCREEN_MAP;
				episode_started = false;
				PK_New_Game();

				if (PK_Check_File("episodes/" + current_episode + "/gfx/pk2stuff.bmp")) {
					PisteDraw2_Image_Delete(kuva_peli);
					kuva_peli = PisteDraw2_Image_Load(("episodes/" + current_episode + "/gfx/pk2stuff.bmp").c_str(), false);
				}
				
				PK_Load_Custom_Hud_Text();

				//PisteDraw2_FadeIn(PD_FADE_NORMAL);
			}
			my += 20;
		}
	}

	/* sivu / kaikki */
	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),180,400)){
		menu_nyt = MENU_MAIN;
		my += 20;
	}
	PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.episodes_get_more),140,440);

	return 0;
}
int PK_Draw_Menu_Language(){
	int my = 0;
	int i;

	PK_Draw_Menu_Square(110, 130, 640-110, 450, 224);

	PisteDraw2_Font_Write(FONT_BIG,"select a language:",50,100);


	for (i=0;i<10;i++){
		if(PK_Draw_Menu_Text(true,langmenulist[i],150,150+my)){
			//printf("Selected %s\n",langmenulist[i]);
			strcpy(settings.kieli,langmenulist[i]);
			PK_Load_Language();
		}
		my += 20;
	}
	my+=180;


	int direction;
	if(totallangs>10){
		direction = PK_Draw_Menu_BackNext(400,my-20);
		if(direction == 1){
			if(langlistindex>0){

				for(i=9;i>0;i--)
					strcpy(langmenulist[i],langmenulist[i-1]);
				strcpy(langmenulist[0],langlist[langlistindex-1]);
				langlistindex--;
			}
		}
		if(direction == 2){
			if(langlistindex<totallangs-10){

				for(i=0;i<9;i++)
					strcpy(langmenulist[i],langmenulist[i+1]);
				strcpy(langmenulist[9],langlist[langlistindex+10]);
				langlistindex++;
			}
		}
	}
	my+=20;
	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.mainmenu_return),130,my)){
		menu_nyt = MENU_MAIN;
	}

	return 0;
}

int PK_Draw_Menu(){
	PisteDraw2_ScreenFill(0);
	PisteDraw2_Image_Clip(kuva_tausta, (episode_started && settings.isWide)? -80 : 0, 0);

	menu_valinta_id = 1;

	switch (menu_nyt)
	{
	case MENU_MAIN     : PK_Draw_Menu_Main();     break;
	case MENU_EPISODES : PK_Draw_Menu_Episodes(); break;
	case MENU_GRAPHICS : PK_Draw_Menu_Graphics(); break;
	case MENU_SOUNDS   : PK_Draw_Menu_Sounds();   break;
	case MENU_CONTROLS : PK_Draw_Menu_Controls(); break;
	case MENU_NAME     : PK_Draw_Menu_Name();     break;
	case MENU_LOAD     : PK_Draw_Menu_Load();     break;
	case MENU_TALLENNA : PK_Draw_Menu_Save();     break;
	case MENU_LANGUAGE : PK_Draw_Menu_Language(); break;
	default            : PK_Draw_Menu_Main();     break;
	}

	PK_Draw_Cursor(hiiri_x,hiiri_y);

	return 0;
}

int PK_Draw_Map_Button(int x, int y, int t){
	int paluu = 0;

	t = t * 25;

	int vilkku = 50 + (int)(sin_table[degree%360]*2);

	if (vilkku < 0)
		vilkku = 0;

	if (hiiri_x > x && hiiri_x < x+17 && hiiri_y > y && hiiri_y < y+17){
		if (key_delay == 0 && (PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE)
													    || PisteInput_Gamepad_Button(PI_CONTROLLER_A))){
			key_delay = 30;
			return 2;
		}

		if (t == 25)
			PK_Draw_Transparent_Object(kuva_peli, 247, 1, 25, 25, x-2, y-2, 60, 96);
		if (t == 0)
			PK_Draw_Transparent_Object(kuva_peli, 247, 1, 25, 25, x-4, y-4, 60, 32);
		if (t == 50)
			PK_Draw_Transparent_Object(kuva_peli, 247, 1, 25, 25, x-4, y-4, 60, 64);

		paluu = 1;
	}

	// Draw egg
	if (t == 25)
		PK_Draw_Transparent_Object(kuva_peli, 247, 1, 25, 25, x-2, y-2, vilkku, 96);

	if (((degree/45)+1)%4==0 || t==0)
		PisteDraw2_Image_CutClip(kuva_peli,x,y,1+t,58,23+t,80);

	return paluu;
}

int PK_Draw_Map() {
	char luku[20];
	int vali = 20;

	std::string st;

	PisteDraw2_ScreenFill(0);
	PisteDraw2_Image_Clip(kuva_tausta, 0, 0);

	PisteDraw2_Font_Write(FONT_BLACK, current_episode, 100+2,72+2);
	PisteDraw2_Font_Write(FONT_BIG, current_episode, 100,72);

	PisteDraw2_Font_Write(FONT_BLACK,tekstit->Hae_Teksti(PK_txt.map_total_score),100+2,92+2);
	vali = PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.map_total_score),100,92);//250,80
	ltoa(pisteet,luku,10);
	PisteDraw2_Font_Write(FONT_BLACK,luku,100+vali+2+15,92+2);
	PisteDraw2_Font_Write(FONT_BIG,luku,100+vali+15,92);

	if (episodipisteet.episode_top_score > 0) {
		vali = PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.map_episode_best_player),360,72);
		PisteDraw2_Font_Write(FONT_SMALL,episodipisteet.episode_top_player,360+vali+10,72);
		vali = PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.map_episode_hiscore),360,92);
		ltoa(episodipisteet.episode_top_score,luku,10);
		PisteDraw2_Font_Write(FONT_BIG,luku,360+vali+15,92);
	}

	vali = PisteDraw2_Font_Write(FONT_SMALL,tekstit->Hae_Teksti(PK_txt.map_next_level),100,120);
	ltoa(jakso,luku,10);
	PisteDraw2_Font_Write(FONT_SMALL,luku,100+vali+15,120);

	//PK_Particles_Draw();

	if (episode_amount == 0) {
		PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.episodes_no_maps),180,290);
	}

	if (PK_Draw_Menu_Text(true, tekstit->Hae_Teksti(PK_txt.mainmenu_return), 100, 430)){
		game_next_screen = SCREEN_MENU;
		menu_nyt = MENU_MAIN;
	}

	int nuppi_x = 0, nuppi_y = 0;
	int type = 0;
	int result;
	int time = 0, min = 0, sek = 0;
	int ikoni;
	int sinx = 0, cosy = 0;
	int pekkaframe = 0;

	/*
	int njakso = episode_amount;
	for (int i=1;i<= episode_amount;i++)
		if (!levels[i].passed && levels[i].level_number < njakso)
			njakso = levels[i].level_number; // Find the first unclear level
	if(jakso < njakso)
		jakso = njakso;*/

	for (int i = 0; i <= episode_amount; i++) {
		if (!levels[i].name.empty()) {
			if (!levels[i].is_secret || (levels[i].is_secret && levels[i].secret_shown)) {
				type = 0;							//0 Gray

				if (levels[i].level_number == level_stats.current_level) {
					type = 1;
				} else if (levels[i].level_number > level_stats.current_level) {
					type = 2;
				} else if (level_stats.level_passed[i]) {
					type = 0;
				}

				if (levels[i].is_secret) {
					if (levels[i].level_number_secret == level_stats.current_secret_level) {
						type = 1;
					} else if (level_stats.level_secret_passed[i]) {
						type = 0;
					} else {
						type = 2;
					}
				}

				if (levels[i].x == 0)
					levels[i].x = 142 + i * 30;

				if (levels[i].y == 0)
					levels[i].y = 270;

				ikoni = levels[i].icon;

				// Draw level icon
				PisteDraw2_Image_CutClip(kuva_peli, levels[i].x - 9, levels[i].y - 14, 1 + (ikoni * 28), 452, 28 + (ikoni * 28), 479);

				// Draw Pekka's head
				if (type == 1 && !levels[i].is_secret) {
					sinx = (int)(sin_table[degree % 360] / 2);
					cosy = (int)(cos_table[degree % 360] / 2);
					
					pekkaframe = 28 * ((degree % 360) / 120);
					
					PisteDraw2_Image_CutClip(kuva_peli, levels[i].x + sinx - 12, levels[i].y - 17 + cosy, 157 + pekkaframe, 46, 181 + pekkaframe, 79);
				}

				result = PK_Draw_Map_Button(levels[i].x - 5, levels[i].y - 10, type);

				// if clicked
				if (result == 2) {
					if (type != 2 || dev_mode) {
						strcpy(next_map, levels[i].file.c_str());
						jakso_indeksi_nyt = i;
						going_to_game = true;
						PisteDraw2_FadeOut(PD_FADE_SLOW);
						music_volume = 0;
						PK_Play_MenuSound(kieku_aani, 90);
					}
					else
						PK_Play_MenuSound(ammuu_aani, 100);
				}

				if (levels[i].is_secret) {
					st = "x" + std::to_string(levels[i].level_number_secret);
					PisteDraw2_Font_Write(FONT_SMALL, st, levels[i].x - 12 + 2, levels[i].y - 29 + 2);
				} else {
					st = std::to_string(levels[i].level_number);
					PisteDraw2_Font_Write(FONT_SMALL, st, levels[i].x - 12 + 2, levels[i].y - 29 + 2);
				}

				// Draw collectables on map
				if (collectable_settings.show_on_map) {
					for (int j = 0; j < EPISODI_MAX_LEVELS; j++) {
						for (int i = 0; i < level_stats.collectables_collected[j]; i++) {
							PisteDraw2_Image_Clip(collectable_settings.index_map_image, levels[j].x + ((level_stats.collectables_collected[j] * collectable_settings.mapW) / 2) - (i * collectable_settings.mapW), levels[j].y + 4);
						}
					}
				}

				/*
				itoa(levels[i].level_number, luku, 10);
				char tmp[20];
				strcat(tmp, "x");
				strcat(tmp, luku);*/
				//PisteDraw2_Font_Write(FONT_SMALL, st, levels[i].x - 12 + 2, levels[i].y - 29 + 2);

				if (result > 0) {

					int info_x = 489 + 3, info_y = 341 - 26;
					
					// TODO figure out values, that make this look good
					// It is width and heigth, not end_x end_y
					// The transparent object has just one color (line) on the palette, the las arg is a power of two
					// that determines witch color is it. It'll sum on the color value
					PK_Draw_Transparent_Object(kuva_peli, info_x - 3, 0, 610 - info_x, 121, info_x, info_y + 26, 60, 128*2);
					//PisteDraw2_Image_CutClip(kuva_peli, info_x - 3, info_y + 26, 473, 0, 607, 121);

					PisteDraw2_Font_Write(FONT_SMALL, levels[i].name, info_x, info_y + 30);

					if (episodipisteet.best_score[i] > 0) {
						PisteDraw2_Font_WriteAlpha(FONT_SMALL, tekstit->Hae_Teksti(PK_txt.map_level_best_player), info_x, info_y + 50, 75);
						PisteDraw2_Font_Write(FONT_SMALL, episodipisteet.top_player[i], info_x, info_y + 62);
						vali = 8 + PisteDraw2_Font_WriteAlpha(FONT_SMALL, tekstit->Hae_Teksti(PK_txt.map_level_hiscore), info_x, info_y + 74, 75);
						ltoa(episodipisteet.best_score[i], luku, 10);
						PisteDraw2_Font_Write(FONT_SMALL, luku, info_x + vali, info_y + 75);
					}

					if (episodipisteet.best_time[i] > 0) { //TODO - Also show if zero
						PisteDraw2_Font_WriteAlpha(FONT_SMALL, tekstit->Hae_Teksti(PK_txt.map_level_fastest_player), info_x, info_y + 98, 75);
						PisteDraw2_Font_Write(FONT_SMALL, episodipisteet.fastest_player[i], info_x, info_y + 110);

						vali = 8 + PisteDraw2_Font_WriteAlpha(FONT_SMALL, tekstit->Hae_Teksti(PK_txt.map_level_best_time), info_x, info_y + 122, 75);

						time = (int)episodipisteet.best_time[i];
						if (time < 0) {
							vali += PisteDraw2_Font_Write(FONT_SMALL, "-", info_x + vali, info_y + 122);
							time = -time;
						}
						
						min = time / 60;
						sek = time % 60;

						itoa(min, luku, 10);
						vali += PisteDraw2_Font_Write(FONT_SMALL, luku, info_x + vali, info_y + 122);
						vali += PisteDraw2_Font_Write(FONT_SMALL, ":", info_x + vali, info_y + 122);
						itoa(sek, luku, 10);
						PisteDraw2_Font_Write(FONT_SMALL, luku, info_x + vali, info_y + 122);
					}
				}
			}
		}
	}
	
	PK_Draw_Cursor(hiiri_x, hiiri_y);

	return 0;
}

int PK_Draw_ScoreCount(){
	char luku[20];
	int vali = 20;
	int my = 0, x, y;
	int kuutio, aste;
	int	vari = 0, kerroin;

	PisteDraw2_ScreenFill(0);
	PisteDraw2_Image_Clip(kuva_tausta, 0, 0);

	for (aste=0;aste<18;aste++) {

		kerroin = (int)(cos_table[(degree+aste*3)%180]);

		x = (int)(sin_table[(degree+aste*10)%360]*2)+kerroin;
		y = (int)(cos_table[(degree+aste*10)%360]*2);//10 | 360 | 2
		//PisteDraw2_Image_Clip(kuva_peli,320+x,240+y,157,46,181,79);
		kuutio = (int)(sin_table[(degree+aste*3)%360]);
		if (kuutio < 0) kuutio = -kuutio;

		PisteDraw2_ScreenFill(320-x,240-y,320-x+kuutio,240-y+kuutio,VARI_TURKOOSI+8);
	}
	for (aste=0;aste<18;aste++) {

		x = (int)(sin_table[(degree+aste*10)%360]*3);
		y = (int)(cos_table[(degree+aste*10)%360]*3);//10 | 360 | 3
		//PisteDraw2_Image_Clip(kuva_peli,320+x,240+y,157,46,181,79);
		kuutio = (int)(sin_table[(degree+aste*2)%360])+18;
		if (kuutio < 0) kuutio = -kuutio;//0;//
		if (kuutio > 100) kuutio = 100;

		//PisteDraw2_ScreenFill(320+x,240+y,320+x+kuutio,240+y+kuutio,VARI_TURKOOSI+10);
		PK_Draw_Transparent_Object(kuva_peli, 247, 1, 25, 25, 320+x, 240+y, kuutio, 32);
	}

	PisteDraw2_Font_Write(FONT_BLACK,tekstit->Hae_Teksti(PK_txt.score_screen_title),100+2,72+2);
	PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.score_screen_title),100,72);
	PisteDraw2_Font_Write(FONT_BLACK,tekstit->Hae_Teksti(PK_txt.score_screen_level_score),100+2,102+2);
	vali = PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.score_screen_level_score),100,102);//250,80
	fake_pisteet = bonuspisteet + aikapisteet + energiapisteet + esinepisteet + pelastuspisteet;
	ltoa(fake_pisteet,luku,10);
	PisteDraw2_Font_Write(FONT_BLACK,luku,400+2,102+2);
	PisteDraw2_Font_Write(FONT_BIG,luku,400,102);
	my = 0;

	PisteDraw2_Font_Write(FONT_BLACK,tekstit->Hae_Teksti(PK_txt.score_screen_bonus_score),100+2,192+2+my);
	vali = PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.score_screen_bonus_score),100,192+my);
	ltoa(bonuspisteet,luku,10);
	PisteDraw2_Font_Write(FONT_BLACK,luku,400+2,192+2+my);
	PisteDraw2_Font_Write(FONT_BIG,luku,400,192+my);
	my += 30;

	PisteDraw2_Font_Write(FONT_BLACK,tekstit->Hae_Teksti(PK_txt.score_screen_time_score),100+2,192+2+my);
	vali = PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.score_screen_time_score),100,192+my);
	ltoa(aikapisteet,luku,10);
	PisteDraw2_Font_Write(FONT_BLACK,luku,400+2,192+2+my);
	PisteDraw2_Font_Write(FONT_BIG,luku,400,192+my);
	my += 30;

	PisteDraw2_Font_Write(FONT_BLACK,tekstit->Hae_Teksti(PK_txt.score_screen_energy_score),100+2,192+2+my);
	vali = PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.score_screen_energy_score),100,192+my);
	ltoa(energiapisteet,luku,10);
	PisteDraw2_Font_Write(FONT_BLACK,luku,400+2,192+2+my);
	PisteDraw2_Font_Write(FONT_BIG,luku,400,192+my);
	my += 30;

	PisteDraw2_Font_Write(FONT_BLACK,tekstit->Hae_Teksti(PK_txt.score_screen_item_score),100+2,192+2+my);
	vali = PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.score_screen_item_score),100,192+my);
	ltoa(esinepisteet,luku,10);
	PisteDraw2_Font_Write(FONT_BLACK,luku,400+2,192+2+my);
	PisteDraw2_Font_Write(FONT_BIG,luku,400,192+my);
	my += 30;

	x = 110;
	y = 192 + my;

	for (int i = 0; i < MAX_GIFTS; i++)
		if (Game::Gifts->get(i) != -1)	{
			//PK2Sprite_Prototype* prot = Game::Gifts->get_protot(i);
			Game::Gifts->draw(i, x, y);
			//prot->Piirra(x - prot->leveys/2, y - prot->korkeus / 2, 0);
			x += 38;
		}

	my += 10;

	if (pistelaskuvaihe >= 4){
		PisteDraw2_Font_Write(FONT_BLACK,tekstit->Hae_Teksti(PK_txt.score_screen_total_score),100+2,192+2+my);
		vali = PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.score_screen_total_score),100,192+my);//250,80
		ltoa(pisteet,luku,10);
		PisteDraw2_Font_Write(FONT_BLACK,luku,400+2,192+2+my);
		PisteDraw2_Font_Write(FONT_BIG,luku,400,192+my);
		my += 25;

		if (jakso_uusi_ennatys) {
			PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.score_screen_new_level_hiscore),100+rand()%2,192+my+rand()%2);
			my += 25;
		}
		if (jakso_uusi_ennatysaika) {
			PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.score_screen_new_level_best_time),100+rand()%2,192+my+rand()%2);
			my += 25;
		}
		if (episodi_uusi_ennatys) {
			PisteDraw2_Font_Write(FONT_BIG,tekstit->Hae_Teksti(PK_txt.score_screen_new_episode_hiscore),100+rand()%2,192+my+rand()%2);
			my += 25;
		}
	}

	if (PK_Draw_Menu_Text(true,tekstit->Hae_Teksti(PK_txt.score_screen_continue),100,430)){
		music_volume = 0;
		siirry_pistelaskusta_karttaan = true;
		PisteDraw2_FadeOut(PD_FADE_NORMAL);
		//game_next_screen = SCREEN_MAP;
	}

	PK_Draw_Cursor(hiiri_x,hiiri_y);

	return 0;
}

int PK_Draw_Intro_Text(char *teksti, int fontti, int x, int y, DWORD alkuaika, DWORD loppuaika){
	int pros = 100;
	if (introlaskuri > alkuaika && introlaskuri < loppuaika) {

		if (introlaskuri - alkuaika < 100)
			pros = introlaskuri - alkuaika;

		if (loppuaika - introlaskuri < 100)
			pros = loppuaika - introlaskuri;

		if (pros > 0) {
			if (pros < 100)
				PisteDraw2_Font_WriteAlpha(fontti,teksti,x,y,pros);
			else
				PisteDraw2_Font_Write(fontti,teksti,x,y);
		}

	}
	return 0;
}
int PK_Draw_Intro(){

	DWORD pistelogo_alku	= 300;
	DWORD pistelogo_loppu	= pistelogo_alku + 500;
	DWORD tekijat_alku		= pistelogo_loppu + 80;
	DWORD tekijat_loppu		= tekijat_alku + 720;
	DWORD testaajat_alku	= tekijat_loppu + 80;
	DWORD testaajat_loppu	= testaajat_alku + 700;
	DWORD kaantaja_alku		= testaajat_loppu + 100;
	DWORD kaantaja_loppu	= kaantaja_alku + 300;

	PisteDraw2_ScreenFill(0);
	PisteDraw2_Image_CutClip(kuva_tausta, 280, 80, 280, 80, 640, 480);

	if ((introlaskuri / 10) % 50 == 0)
		PisteDraw2_Image_CutClip(kuva_tausta,353, 313, 242, 313, 275, 432);

	if (introlaskuri > pistelogo_alku && introlaskuri < pistelogo_loppu) {

		//int x = introlaskuri - pistelogo_alku - 194;
		int kerroin = 120 / (introlaskuri - pistelogo_alku);
		int x = 120 - kerroin;

		if (x > 120)
			x = 120;

		PisteDraw2_Image_CutClip(kuva_tausta,/*120*/x,230, 37, 230, 194, 442);

		PK_Draw_Intro_Text(tekstit->Hae_Teksti(PK_txt.intro_presents), FONT_SMALL, 230, 400, pistelogo_alku, pistelogo_loppu-20);

	}

	if (introlaskuri > tekijat_alku) {
		PK_Draw_Intro_Text(tekstit->Hae_Teksti(PK_txt.intro_a_game_by),FONT_SMALL, 120, 200, tekijat_alku, tekijat_loppu);
		PK_Draw_Intro_Text("janne kivilahti 2003",		            FONT_SMALL, 120, 220, tekijat_alku+20, tekijat_loppu+20);
		PK_Draw_Intro_Text(tekstit->Hae_Teksti(PK_txt.intro_original), FONT_SMALL, 120, 245, tekijat_alku+40, tekijat_loppu+40);
		PK_Draw_Intro_Text("antti suuronen 1998",		            FONT_SMALL, 120, 265, tekijat_alku+50, tekijat_loppu+50);
		PK_Draw_Intro_Text("sdl porting by",		                FONT_SMALL, 120, 290, tekijat_alku+70, tekijat_loppu+70);
		PK_Draw_Intro_Text("samuli tuomola 2010",		            FONT_SMALL, 120, 310, tekijat_alku+80, tekijat_loppu+80);
		PK_Draw_Intro_Text("sdl2 port and bug fixes",               FONT_SMALL, 120, 335, tekijat_alku + 90, tekijat_loppu + 90);
		PK_Draw_Intro_Text("danilo lemos 2017",                     FONT_SMALL, 120, 355, tekijat_alku + 100, tekijat_loppu + 100);
		PK_Draw_Intro_Text("Community Edition",                     FONT_SMALL, 120, 375, tekijat_alku + 110, tekijat_loppu + 110);
		PK_Draw_Intro_Text("Deta 2019",								FONT_SMALL, 120, 395, tekijat_alku + 120, tekijat_loppu + 120);
	}

	if (introlaskuri > testaajat_alku) {
		PK_Draw_Intro_Text(tekstit->Hae_Teksti(PK_txt.intro_tested_by),FONT_SMALL, 120, 230, testaajat_alku, testaajat_loppu);
		PK_Draw_Intro_Text("antti suuronen",			FONT_SMALL, 120, 250, testaajat_alku+10, testaajat_loppu+10);
		PK_Draw_Intro_Text("toni hurskainen",			FONT_SMALL, 120, 260, testaajat_alku+20, testaajat_loppu+20);
		PK_Draw_Intro_Text("juho rytk�nen",				FONT_SMALL, 120, 270, testaajat_alku+30, testaajat_loppu+30);
		PK_Draw_Intro_Text("annukka korja",				FONT_SMALL, 120, 280, testaajat_alku+40, testaajat_loppu+40);
		PK_Draw_Intro_Text(tekstit->Hae_Teksti(PK_txt.intro_thanks_to),FONT_SMALL, 120, 300, testaajat_alku+70, testaajat_loppu+70);
		PK_Draw_Intro_Text("oskari raunio",				FONT_SMALL, 120, 310, testaajat_alku+70, testaajat_loppu+70);
		PK_Draw_Intro_Text("assembly organization",		FONT_SMALL, 120, 320, testaajat_alku+70, testaajat_loppu+70);
	}

	if (introlaskuri > kaantaja_alku) {
		PK_Draw_Intro_Text(tekstit->Hae_Teksti(PK_txt.intro_translation), FONT_SMALL, 120, 230, kaantaja_alku, kaantaja_loppu);
		PK_Draw_Intro_Text(tekstit->Hae_Teksti(PK_txt.intro_translator),  FONT_SMALL, 120, 250, kaantaja_alku+20, kaantaja_loppu+20);
	}
	return 0;
}

int PK_Draw_EndGame_Image(int x, int y, int type, int plus, int rapytys){
	int frm = 0;
	int yk = 0;

	if (type == 1){ // Pekka
		frm = 1;
		if ((degree/10)%10==rapytys) frm = 0;
		yk = (int)sin_table[(degree%360)];
		PisteDraw2_Image_CutClip(kuva_tausta,x+3,y+56, 4, 63, 29, 69);
		if (yk < 0){
			y+=yk;
			frm = 2;
		}
		PisteDraw2_Image_CutClip(kuva_tausta,x,y, 1+frm*35, 1, 32+frm*35, 59);
	}

	if (type == 2){ // kana (katse oikealle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)cos_table[((degree+plus)%360)];
		PisteDraw2_Image_CutClip(kuva_tausta,x+3,y+36, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PisteDraw2_Image_CutClip(kuva_tausta,x,y, 106+frm*37, 1, 139+frm*37, 38);
	}

	if (type == 3){ // kana (katse vasemmalle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)cos_table[((degree+plus)%360)];
		PisteDraw2_Image_CutClip(kuva_tausta,x+3,y+36, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PisteDraw2_Image_CutClip(kuva_tausta,x,y, 106+frm*37, 41, 139+frm*37, 77);
	}

	if (type == 4){ // pikkukana (katse oikealle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)sin_table[(((degree*2)+plus)%360)];
		PisteDraw2_Image_CutClip(kuva_tausta,x+3,y+27, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PisteDraw2_Image_CutClip(kuva_tausta,x,y, 217+frm*29, 1, 243+frm*29, 29);
	}

	if (type == 5){ // pikkukana (katse vasemmalle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)sin_table[(((degree*2)+plus)%360)];
		PisteDraw2_Image_CutClip(kuva_tausta,x,y+27, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PisteDraw2_Image_CutClip(kuva_tausta,x,y, 217+frm*29, 33, 243+frm*29, 61);
	}

	return 0;
}
int PK_Draw_EndGame(){

	DWORD onnittelut_alku	= 300;
	DWORD onnittelut_loppu	= onnittelut_alku + 1000;
	DWORD the_end_alku		= onnittelut_loppu + 80;
	DWORD the_end_loppu		= the_end_alku + 3000;

	PisteDraw2_ScreenFill(0);
	PisteDraw2_Image_CutClip(kuva_tausta,320-233/2,240-233/2, 6, 229, 239, 462);

	PK_Draw_EndGame_Image(345, 244, 3, 30, 2);
	PK_Draw_EndGame_Image(276, 230, 2, 50, 3);
	PK_Draw_EndGame_Image(217, 254, 4, 0, 4);

	PK_Draw_EndGame_Image(305, 240, 1, 0, 1);

	PK_Draw_EndGame_Image(270, 284, 2, 20, 1);
	PK_Draw_EndGame_Image(360, 284, 5, 60, 2);

	if (loppulaskuri > onnittelut_alku) {
		PK_Draw_Intro_Text(tekstit->Hae_Teksti(PK_txt.end_congratulations), FONT_BIG, 220, 380, onnittelut_alku, onnittelut_loppu);
		PK_Draw_Intro_Text(tekstit->Hae_Teksti(PK_txt.end_chickens_saved), FONT_SMALL, 220, 402, onnittelut_alku+30, onnittelut_loppu+30);
	}
	if (loppulaskuri > the_end_alku) {
		PK_Draw_Intro_Text(tekstit->Hae_Teksti(PK_txt.end_the_end), FONT_BIG, 280, 190, the_end_alku, the_end_loppu);
	}

	return 0;
}

//==================================================
//(#14) Android UI
//==================================================

void PK_UI_Activate(bool set){
	PisteInput_ActiveGui(gui_menu, set);
	PisteInput_ActiveGui(gui_arr, set);
	PisteInput_ActiveGui(gui_left, set);
	PisteInput_ActiveGui(gui_right, set);
	PisteInput_ActiveGui(gui_up, set);
	PisteInput_ActiveGui(gui_down, set);
	PisteInput_ActiveGui(gui_doodle, set);
	PisteInput_ActiveGui(gui_egg, set);
	PisteInput_ActiveGui(gui_gift, set);
	PisteInput_ActiveGui(gui_tab, set);
}
void PK_UI_Change(int ui_mode){
	switch(ui_mode){
		case UI_TOUCH_TO_START:
			PisteInput_ActiveGui(gui_touch, true);
			PK_UI_Activate(false);
		break;
		case UI_CURSOR:
			PisteInput_ActiveGui(gui_touch, false);
			PK_UI_Activate(false);
		break;
		case UI_GAME_BUTTONS:
			PisteInput_ActiveGui(gui_touch, true);
			PK_UI_Activate(true);
		break;
	}
}
void PK_UI_Load(){
	static DWORD escape = PI_ESCAPE;
	static DWORD tab    = PI_TAB;
	static DWORD enter  = PI_RETURN;

	int circ_size = 200;
	int alpha = 170;

	gui_touch = PisteInput_CreateGui(0,0,1920,1080,alpha,"", &enter);

	gui_menu = PisteInput_CreateGui(50,130,circ_size,circ_size,alpha,"android/menu.png", &escape);
	gui_arr = PisteInput_CreateGui(50,650,388,256,alpha,"android/arrow.png", NULL);
	gui_left = PisteInput_CreateGui(50,650,388/2,256,alpha,"", &settings.control_left);
	gui_right = PisteInput_CreateGui(50+388/2,650,388/2,256,alpha,"",  &settings.control_right);
	gui_up = PisteInput_CreateGui(1630,650,circ_size,circ_size,alpha,"android/up.png", &settings.control_jump);
	gui_down = PisteInput_CreateGui(1420,700,circ_size,circ_size,alpha,"android/down.png", &settings.control_down);
	gui_doodle = PisteInput_CreateGui(1630,450,circ_size,circ_size,alpha,"android/doodle.png", &settings.control_attack2);
	gui_egg = PisteInput_CreateGui(1420,500,circ_size,circ_size,alpha,"android/egg.png", &settings.control_attack1);
	gui_gift = PisteInput_CreateGui(1630,250,circ_size,circ_size,alpha,"android/gift.png", &settings.control_open_gift);
	gui_tab = PisteInput_CreateGui(0,930,530,150,alpha,"", &tab);
}

//==================================================
//(#15) Main frames
//==================================================

int PK_MainScreen_Intro(){
	PK_Draw_Intro();

	degree = 1 + degree % 360;

	introlaskuri++;

	if (siirry_introsta_menuun && !PisteDraw2_IsFading()){
		game_next_screen = SCREEN_MENU;
		episode_started = false;
	}
	if (key_delay > 0) key_delay--;
	if (key_delay == 0)
		if (PisteInput_Keydown(PI_RETURN) || PisteInput_Keydown(PI_SPACE) || PisteInput_Gamepad_Button(PI_CONTROLLER_A) || introlaskuri == 3500){
			siirry_introsta_menuun = true;
			PisteDraw2_FadeOut(PD_FADE_SLOW);
		}

	return 0;
}
int PK_MainScreen_ScoreCount(){
	PK_Update_Mouse();

	PK_Draw_ScoreCount();

	degree = 1 + degree % 360;

	// PISTELASKU

	// TODO Display collectables, like gifts?
	int energia = Game::Sprites->player->energia;

	if (pistelaskudelay == 0){
		if (bonuspisteet < jakso_pisteet){
			pistelaskuvaihe = 1;
			pistelaskudelay = 0;
			bonuspisteet += 10;

			if (degree%7==1)
				PK_Play_MenuSound(pistelaskuri_aani, 70);

			if (bonuspisteet >= jakso_pisteet){
				bonuspisteet = jakso_pisteet;
				pistelaskudelay = 50;
			}

		} else if (timeout > 0){
			pistelaskuvaihe = 2;
			pistelaskudelay = 0;
			aikapisteet+=5;
			timeout--;

			if (degree%10==1)
				PK_Play_MenuSound(pistelaskuri_aani, 70);

			if (timeout == 0)
				pistelaskudelay = 50;

		} else if (Game::Sprites->player->energia > 0){
			pistelaskuvaihe = 3;
			pistelaskudelay = 10;
			energiapisteet+=300;
			Game::Sprites->player->energia--;

			PK_Play_MenuSound(pistelaskuri_aani, 70);

		} else if (Game::Gifts->count() > 0){
			int count = Game::Gifts->count();

			pistelaskuvaihe = 4;
			pistelaskudelay = 30;
			for (int i = 0; i < MAX_GIFTS; i++)
				if (Game::Gifts->get(i) != -1) {
					esinepisteet += Game::Gifts->get_protot(i)->pisteet + 500;
					//esineet[i] = NULL;

					Game::Gifts->set(i, -1);

					PK_Play_MenuSound(hyppy_aani, 100);
					break;
				}

			count--;
		}
		else pistelaskuvaihe = 5;
	}

	if (pistelaskudelay > 0)
		pistelaskudelay--;

	if (siirry_pistelaskusta_karttaan && !PisteDraw2_IsFading()){
		/*tarkistetaan oliko viimeinen jakso*/

		if (jakso_indeksi_nyt == EPISODI_MAX_LEVELS-1) { // ihan niin kuin joku tekisi n�in monta jaksoa...
			game_next_screen = SCREEN_END;
		}
		else if (levels[jakso_indeksi_nyt+1].level_number == -1)
			    game_next_screen = SCREEN_END;
		else // jos ei niin palataan karttaan...
		{
			game_next_screen = SCREEN_MAP;

			//episode_started = false;
			menu_nyt = MENU_MAIN;
		}
	}

	if (key_delay > 0)
		key_delay--;

	if (key_delay == 0){
		if (PisteInput_Keydown(PI_RETURN) && pistelaskuvaihe == 5){
			siirry_pistelaskusta_karttaan = true;
			music_volume = 0;
			PisteDraw2_FadeOut(PD_FADE_NORMAL);
			key_delay = 20;
		}

		if (PisteInput_Keydown(PI_RETURN) && pistelaskuvaihe < 5){
			pistelaskuvaihe = 5;
			bonuspisteet = jakso_pisteet;
			aikapisteet += timeout*5;
			timeout = 0;
			energiapisteet += Game::Sprites->player->energia * 300;
			Game::Sprites->player->energia = 0;
			for (int i=0;i<MAX_GIFTS;i++)
				if (Game::Gifts->get(i) != -1)
					esinepisteet += Game::Gifts->get_protot(i)->pisteet + 500;
			
			Game::Gifts->clean(); //TODO esineita = 0;

			key_delay = 20;
		}
	}

	return 0;
}
int PK_MainScreen_Map(){
	PK_Draw_Map();

	PK_Update_Mouse();

	degree = 1 + degree % 360;

	if (going_to_game && !PisteDraw2_IsFading()) {
		game_next_screen = SCREEN_GAME;

		episode_started = false;

		//Draw "loading" text
		PisteDraw2_SetXOffset(0);
		PisteDraw2_ScreenFill(0);
		PisteDraw2_Font_Write(FONT_BIG, tekstit->Hae_Teksti(PK_txt.game_loading), screen_width / 2 - 82, screen_height / 2 - 9);
		PisteDraw2_FadeOut(0);
	}

	if (key_delay > 0)
		key_delay--;

	return 0;
}
int PK_MainScreen_Menu(){

	if (!name_edit && key_delay == 0 && menu_lue_kontrollit == 0) {
		if (PisteInput_Keydown(PI_UP) || PisteInput_Keydown(PI_LEFT) ||
			PisteInput_Gamepad_Button(PI_CONTROLLER_LEFT) || (PisteInput_Gamepad_Button(PI_CONTROLLER_UP))){
			menu_valittu_id--;

			if (menu_valittu_id < 1)
				menu_valittu_id = menu_valinta_id-1;
			
			key_delay = 15;
		}

		if (PisteInput_Keydown(PI_DOWN) || PisteInput_Keydown(PI_RIGHT) ||
			PisteInput_Gamepad_Button(PI_CONTROLLER_RIGHT) || (PisteInput_Gamepad_Button(PI_CONTROLLER_DOWN))) {
			menu_valittu_id++;

			if (menu_valittu_id > menu_valinta_id-1)
				menu_valittu_id = 1;

			key_delay = 15;
			//hiiri_y += 3;
		}
	}

	if (name_edit || menu_lue_kontrollit > 0){
		menu_valittu_id = 0;
	}

	if (menu_nyt != MENU_NAME){
		name_edit = false;
	}
	int menu_ennen = menu_nyt;

	PK_Draw_Menu();

	if (menu_nyt != menu_ennen)
		menu_valittu_id = 0;

	degree = 1 + degree % 360;

	if (doublespeed)
		degree = 1 + degree % 360;

	if (key_delay > 0)
		key_delay--;

	return 0;
}
int PK_MainScreen_InGame(){

	PK2Kartta_Animoi(degree, palikka_animaatio/7, kytkin1, kytkin2, kytkin3, false);
	PK_Update_Camera();

	Game::Particles->update();

	if (!paused){
		if (!jakso_lapaisty && (!aikaraja || timeout > 0))
			PK_Update_Sprites();
		PK_Fadetext_Update();
	}

	PK_Draw_InGame();

	PK_Calculate_MovableBlocks_Position();

	if (!paused){
		degree = 1 + degree%359;

		if (kytkin1 > 0)
			kytkin1 --;

		if (kytkin2 > 0)
			kytkin2 --;

		if (kytkin3 > 0)
			kytkin3 --;

		if (info_timer > 0)
			info_timer--;

		if (piste_lisays > 0){
			jakso_pisteet++;
			piste_lisays--;
		}

		if (aikaraja && !jakso_lapaisty){
			if (sekunti > 0)
				sekunti --;
			else{
				sekunti = TIME_FPS;
				if (timeout > 0)
					timeout--;
				else
					game_over = true;
			}
		}

		if (nakymattomyys > 0)
			nakymattomyys--;
	}

	if (Game::Sprites->player->energia < 1 && !game_over){
		game_over = true;
		key_delay = 20;
	}

	if (key_delay > 0)
		key_delay--;

	if (jakso_lapaisty || game_over) {
		if (lopetusajastin > 1) {
			lopetusajastin--;
		}

		if (lopetusajastin == 0) {
			lopetusajastin = 320;//2000;
		}

		if (PisteInput_Keydown(settings.control_attack1) || PisteInput_Keydown(settings.control_attack2) || PisteInput_Keydown(PI_RETURN) ||
			PisteInput_Gamepad_Button(settings.gamepad_attack1) || PisteInput_Gamepad_Button(settings.gamepad_attack2)) {
			if (lopetusajastin > 2 && lopetusajastin < 300/*1900*/ && key_delay == 0) {
				lopetusajastin = 2;
			}
		}

		if (jakso_lapaisty && lopetusajastin == 2) {
			PisteDraw2_FadeOut(PD_FADE_NORMAL);
			//music_volume = 0;
		}
	}

	// TODO Figure something out with the SPACE and map stuff
	if (lopetusajastin == 1 && !PisteDraw2_IsFading()) {
		// If level was beaten, go to scoring
		if (jakso_lapaisty) {
			game_next_screen = SCREEN_SCORING;
		} else if (checkpoint_data == nullptr) {
			// If not, start go to the map screen
			game_next_screen = SCREEN_MAP;
		}
	}

	if (checkpoint_data != nullptr && !jakso_lapaisty && Game::Sprites->player->energia <= 0) {
		if (PisteInput_Keydown(PI_SPACE) || PisteInput_Gamepad_Button(settings.gamepad_attack2)) { // Going back to the map
			delete checkpoint_data;
			checkpoint_data = nullptr;

			if (!levels[jakso_indeksi_nyt].passed) {
				level_stats.collectables_collected[jakso_indeksi_nyt] = 0;
				levels[jakso_indeksi_nyt].collectables_collected = 0;
			}

			PisteDraw2_FadeOut(PD_FADE_NORMAL);

			game_next_screen = SCREEN_MAP;

			key_delay = 120;
		} else if (PisteInput_Keydown(PI_RETURN) || PisteInput_Gamepad_Button(settings.gamepad_attack1)) {
			game_over = false;

			PisteDraw2_FadeIn(PD_FADE_FAST);
			PK_Checkpoint_Restore();
		}
	}

	if (key_delay == 0){
		if ((PisteInput_Keydown(settings.control_open_gift) || PisteInput_Gamepad_Button(settings.gamepad_open_gift)) && Game::Sprites->player->energia > 0) {
			Game::Gifts->use();
			key_delay = 10;
		}
		if (PisteInput_Keydown(PI_P) && !jakso_lapaisty){
			paused = !paused;
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_DELETE))
			Game::Sprites->player->energia = 0;
		if (PisteInput_Keydown(PI_TAB)){
			Game::Gifts->change_order();
			key_delay = 10;
		}
		if (!dev_mode)
			if (PisteInput_Keydown(PI_I)) {
				show_fps = !show_fps;
				key_delay = 20;
			}
		if (PisteInput_Keydown(PI_F)) {
			show_fps = !show_fps;
			key_delay = 20;
		}
	}

	if (dev_mode){ //Debug
		if (key_delay == 0) {
			if (PisteInput_Keydown(PI_Z)) {
				if (kytkin1 < switch1_time - 64) kytkin1 = SWITCH_TIME_DEFAULT;
				if (kytkin2 < switch2_time - 64) kytkin2 = SWITCH_TIME_DEFAULT;
				if (kytkin3 < switch3_time - 64) kytkin3 = SWITCH_TIME_DEFAULT;
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_X)) {
				if (kytkin1 > 64) kytkin1 = 64;
				if (kytkin2 > 64) kytkin2 = 64;
				if (kytkin3 > 64) kytkin3 = 64;
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_T)) {
				doublespeed = !doublespeed;
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_G)) {
				settings.lapinakyvat_objektit = !settings.lapinakyvat_objektit;
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_L)) {
				avaimia = 0;
				kartta->Open_Locks();
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_K)) {
				kartta->Change_SkullBlocks();
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_W)) {
				settings.isFullScreen = !settings.isFullScreen;
				PisteDraw2_FullScreen(settings.isFullScreen);
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_I)) {
				draw_dubug_info = !draw_dubug_info;
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_R)) {
				kartta->Select_Start();
				Game::Sprites->player->energia = 10;
				key_delay = 20;
			}
			if (PisteInput_Keydown(PI_END)) {
				key_delay = 20;
				if (PisteSound_StartMusic("music/hiscore.xm") != 0){
					PK2_error = true;
					PK2_error_msg = "Can't find hiscore.xm";
				}
				jakso_lapaisty = true;
				levels[jakso_indeksi_nyt].passed = true;
				if (levels[jakso_indeksi_nyt].level_number == jakso)
					jakso++;
				music_volume = settings.music_max_volume;
				music_volume_now = settings.music_max_volume - 1;
			}
			if (PisteInput_Keydown(PI_LSHIFT)/* && key_delay == 0*/) {
				//key_delay = 20;
				for (int r = 1; r<6; r++)
					//Game::Particles->new_particle(PARTICLE_SPARK, player->x + rand() % 10 - rand() % 10, player->y + rand() % 10 - rand() % 10, 0, 0, rand() % 100, 0.1, 32);
					Game::Particles->new_particle(PARTICLE_SPARK, Game::Sprites->player->x + rand() % 10 - rand() % 10, Game::Sprites->player->y + rand() % 10 - rand() % 10, 0, 0, rand() % 100, 0.1, 32);
				//Game::Sprites->player = PK2Sprite(Game::Sprites->prototypes[PROTOTYPE_KANA], 1, false, Game::Sprites->player->x, Game::Sprites->player->y);
			}
		}
		if (PisteInput_Keydown(PI_U))
			Game::Sprites->player->b = -10;
		if (PisteInput_Keydown(PI_E))
			Game::Sprites->player->energia = Game::Sprites->player->type->energia;
		if (PisteInput_Keydown(PI_V))
			nakymattomyys = 3000;
	}

	return 0;
}
int PK_MainScreen_End(){

	PK_Draw_EndGame();

	degree = 1 + degree % 360;

	loppulaskuri++;
	introlaskuri = loppulaskuri; // introtekstej� varten

	if (siirry_lopusta_menuun && !PisteDraw2_IsFading())
	{
		game_next_screen = SCREEN_MENU;
		menu_nyt = MENU_MAIN;
		episode_started = false;
	}

	if (key_delay > 0)
		key_delay--;

	if (key_delay == 0)
	{
		if (PisteInput_Keydown(PI_RETURN) || PisteInput_Keydown(PI_SPACE) || PisteInput_Gamepad_Button(PI_CONTROLLER_A))
		{
			siirry_lopusta_menuun = true;
			music_volume = 0;
			PisteDraw2_FadeOut(PD_FADE_SLOW);
		}
	}

	return 0;
}

void PK_Load_Collectable_Settings() {
	// Only load once
	if (collectable_settings.map_image.empty() || collectable_settings.game_image.empty()) {
		std::ifstream ifs;

		std::string file = "episodes/" + current_episode + "/data/episode.dat";

		ifs.open(file.c_str(), std::ios::binary);

		if (ifs.fail()) {
			file = "data/episode.dat";

			ifs.open(file.c_str(), std::ios::binary);
		}

		if (ifs.fail()) {
			PisteLog_Write("PK2Main", "Couldn't find collectable settings!", TYPE::T_INFO);
		} else {
			ifs.read(reinterpret_cast<char *>(&collectable_settings.show_on_map), sizeof(bool));
			ifs.read(reinterpret_cast<char *>(&collectable_settings.show_in_game), sizeof(bool));

			int len = 0;
			ifs.read(reinterpret_cast<char *>(&len), sizeof(int));

			char* tmp = new char[len + 1];
			for (int i = 0; i < len; i++) {
				ifs.read(&tmp[i], sizeof(char));
			}

			tmp[len] = '\0';
			collectable_settings.map_image = tmp;

			delete[] tmp;

			len = 0;
			ifs.read(reinterpret_cast<char *>(&len), sizeof(int));

			tmp = new char[len + 1];
			for (int i = 0; i < len; i++) {
				ifs.read(&tmp[i], sizeof(char));
			}

			tmp[len] = '\0';
			collectable_settings.game_image = tmp;

			delete[] tmp;

			ifs.read(reinterpret_cast<char *>(&collectable_settings.mapX), sizeof(int));
			ifs.read(reinterpret_cast<char *>(&collectable_settings.mapY), sizeof(int));
			ifs.read(reinterpret_cast<char *>(&collectable_settings.mapW), sizeof(int));
			ifs.read(reinterpret_cast<char *>(&collectable_settings.mapH), sizeof(int));

			ifs.read(reinterpret_cast<char *>(&collectable_settings.gameX), sizeof(int));
			ifs.read(reinterpret_cast<char *>(&collectable_settings.gameY), sizeof(int));
			ifs.read(reinterpret_cast<char *>(&collectable_settings.gameW), sizeof(int));
			ifs.read(reinterpret_cast<char *>(&collectable_settings.gameH), sizeof(int));

			std::string image_file = "episodes/" + current_episode + "/sprites/" + collectable_settings.map_image;

			if (!PisteUtils_File_Exists(image_file)) {
				image_file = "sprites/" + collectable_settings.map_image;
			}

			int img = PisteDraw2_Image_Load(image_file);

			if (img != -1) {
				collectable_settings.index_map_image = PisteDraw2_Image_Cut(img, collectable_settings.mapX, collectable_settings.mapY, collectable_settings.mapW, collectable_settings.mapH);
			}

			image_file = "episodes/" + current_episode + "/sprites/" + collectable_settings.game_image;

			if (!PisteUtils_File_Exists(image_file)) {
				image_file = "sprites/" + collectable_settings.game_image;
			}

			img = PisteDraw2_Image_Load(image_file);

			if (img != -1) {
				collectable_settings.index_game_image = PisteDraw2_Image_Cut(img, collectable_settings.gameX, collectable_settings.gameY, collectable_settings.gameW, collectable_settings.gameH);
			}
		}
	}
}

int PK_MainScreen_Change() {

	PisteDraw2_FadeIn(PD_FADE_NORMAL);

	// First start
	if (game_next_screen == SCREEN_BASIC_FORMAT)
	{
		PK_UI_Change(UI_TOUCH_TO_START);
		strcpy(pelaajan_nimi, tekstit->Hae_Teksti(PK_txt.player_default_name));
		srand((unsigned)time(NULL));
		
		current_episode = "";
		strcpy(next_map, "untitle1.map");

		jakso = 1;

		if (!precalculated_sincos)
		{
			PK_Precalculate_SinCos();
			PK2Kartta_Cos_Sin(cos_table, sin_table);
			precalculated_sincos = true;
		}

		PK_Fadetext_Init();

		PK2Kartta_Aseta_Ruudun_Mitat(screen_width, screen_height);

		kartta = new PK2Kartta();
		Game::Particles = new PK2::Particle_System();
		Game::Sprites = new PK2::SpriteSystem();
		Game::Gifts = new PK2::GiftSystem();

		if (!settings.isFiltered)
			PisteDraw2_SetFilter(PD_FILTER_NEAREST);
		if (settings.isFiltered)
			PisteDraw2_SetFilter(PD_FILTER_BILINEAR);

		PisteDraw2_FitScreen(settings.isFit);
		
		if (test_windowed) {
			PisteDraw2_FullScreen(false);
		} else {
			PisteDraw2_FullScreen(settings.isFullScreen);
		}

		PisteDraw2_ChangeResolution(settings.isWide ? 800 : 640, 480);

		PisteDraw2_Image_Delete(kuva_peli);
		kuva_peli = PisteDraw2_Image_Load("gfx/pk2stuff.bmp", false);

		PK_Load_Font();

		PK_Search_Episode();
		PK_Start_Saves();
		PK_Search_File();

		PisteDraw2_ScreenFill(0);

		if ((kytkin_aani = PisteSound_LoadSFX("sfx/switch3.wav")) == -1)
		{
			PK2_error = true;
			PK2_error_msg = "Can't find switch3.wav";
		}

		if ((hyppy_aani = PisteSound_LoadSFX("sfx/jump4.wav")) == -1)
		{
			PK2_error = true;
			PK2_error_msg = "Can't find jump4.wav";
		}

		if ((loiskahdus_aani = PisteSound_LoadSFX("sfx/splash.wav")) == -1)
		{
			PK2_error = true;
			PK2_error_msg = "Can't find splash.wav";
		}

		if ((avaa_lukko_aani = PisteSound_LoadSFX("sfx/openlock.wav")) == -1)
		{
			PK2_error = true;
			PK2_error_msg = "Can't find openlock.wav";
		}

		if ((menu_aani = PisteSound_LoadSFX("sfx/menu2.wav")) == -1)
		{
			PK2_error = true;
			PK2_error_msg = "Can't find menu2.wav";
		}

		if ((ammuu_aani = PisteSound_LoadSFX("sfx/moo.wav")) == -1)
		{
			PK2_error = true;
			PK2_error_msg = "Can't find moo.wav";
		}

		if ((kieku_aani = PisteSound_LoadSFX("sfx/doodle.wav")) == -1)
		{
			PK2_error = true;
			PK2_error_msg = "Can't find doodle.wav";
		}

		if ((tomahdys_aani = PisteSound_LoadSFX("sfx/pump.wav")) == -1)
		{
			PK2_error = true;
			PK2_error_msg = "Can't find pump.wav";
		}

		if ((pistelaskuri_aani = PisteSound_LoadSFX("sfx/counter.wav")) == -1)
		{
			PK2_error = true;
			PK2_error_msg = "Can't find counter.wav";
		}

		PisteDraw2_FadeIn(PD_FADE_SLOW);

		PK_Calculate_Tiles();

		Game::Gifts->clean();

		PisteDraw2_Image_Delete(kuva_tausta);
		kuva_tausta = PisteDraw2_Image_Load("gfx/menu.bmp", true);

		PK_Empty_Records();

		PK_Search_Records("data/saves.dat");
	}

	// Start map
	if (game_next_screen == SCREEN_MAP) {
		PK_UI_Change(UI_CURSOR);
		if (settings.isWide)
			PisteDraw2_SetXOffset(80);
		else
			PisteDraw2_SetXOffset(0);
		PisteDraw2_ScreenFill(0);

		if (!episode_started)
		{
			if (lataa_peli != -1)
			{
				PK_Search_File();

				for (int j = 0; j < EPISODI_MAX_LEVELS; j++) {
					levels[j].passed = saves_list[lataa_peli].jakso_lapaisty[j];
					
					level_stats.collectables_collected[j] = saves_list[lataa_peli].collectables_collected[j];
				}

				for (int i = 0; i < 100; i++) {
					if (levels[i].is_secret) {
						if ((levels[i].unlock_type == UNLOCK::SCORE && pisteet >= levels[i].unlock_score) || (levels[i].unlock_type == UNLOCK::COLLECTABLES && level_stats.overall_collectables_collected >= levels[i].unlock_collectables)) {
							levels[i].secret_shown = true;
						}
					}
				}

				lataa_peli = -1;
				episode_started = true;
				game_over = true;
				jakso_lapaisty = true;
				lopetusajastin = 0;
			}
			else
			{
				PK_Start_Saves(); // jos ladataan peli, asetetaan l�p�istyarvot jaksoille aikaisemmin
				PK_Search_File();
			}
			char topscoretiedosto[PE_PATH_SIZE] = "scores.dat";
			PK_EpisodeScore_Open(topscoretiedosto);
		}

		// TODO Optimization - Only load this stuff once?
		/* Loading map background ... */
		char mapkuva[PE_PATH_SIZE] = "map.bmp";
		PK_Load_EpisodeDir(mapkuva);

		PisteDraw2_Image_Delete(kuva_tausta);
		kuva_tausta = PisteDraw2_Image_Load(mapkuva, true);
		if (kuva_tausta == -1)
			kuva_tausta = PisteDraw2_Image_Load("gfx/map.bmp", true);

		std::string mapmusa = "";
		std::string mapStr[] = {
			"episodes/" + current_episode + "/map.mp3",
			"episodes/" + current_episode + "/map.ogg",
			"episodes/" + current_episode + "/map.xm",
			"episodes/" + current_episode + "/map.mod",
			"episodes/" + current_episode + "/map.it",
			"episodes/" + current_episode + "/map.s3m",
			"music/map.xm",
			"music/map.mod",
			"music/map.it",
			"music/map.s3m",
			"music/map.mp3",
			"music/map.ogg"
		};

		for (std::string s : mapStr) {
			if (PK_Check_File(s)) {
				mapmusa = s;
				break;
			}
		}

		// TODO Optimization - Only load this once
		char mm[255];
		strcpy(mm, mapmusa.c_str());
		PisteSound_StartMusic(mm);

		music_volume = settings.music_max_volume;

		going_to_game = false;

		// TODO Only load this stuff once
		PK_Load_Collectable_Settings();

		PisteDraw2_FadeIn(PD_FADE_NORMAL);
	}

	// Start menu
	if (game_next_screen == SCREEN_MENU)
	{
		PK_UI_Change(UI_CURSOR);
		if (settings.isWide)
			PisteDraw2_SetXOffset(80);
		else
			PisteDraw2_SetXOffset(0);
		PK_Search_Episode();

		if (!episode_started)
		{
			PisteDraw2_Image_Delete(kuva_tausta);
			kuva_tausta = PisteDraw2_Image_Load("gfx/menu.bmp", true);
			PisteSound_StartMusic("music/song09.xm"); //theme.xm
			music_volume = settings.music_max_volume;
		}
		else
		{
			int w, h;
			PisteDraw2_Image_GetSize(kuva_tausta, w, h);
			if (w != screen_width)
			{
				PisteDraw2_Image_Delete(kuva_tausta);
				kuva_tausta = PisteDraw2_Image_New(screen_width, screen_height);
			}
			PisteDraw2_Image_Snapshot(kuva_tausta); //TODO - take snapshot without text and cursor
			PK_MenuShadow_Create(kuva_tausta, 640, 480, settings.isWide? 110 : 30);
		}

		menunelio.left = 320 - 5;
		menunelio.top = 240 - 5;
		menunelio.right = 320 + 5;
		menunelio.bottom = 240 + 5;

		PisteDraw2_ScreenFill(0);
		menu_valittu_id = 1;
	}

	// Start game
	if (game_next_screen == SCREEN_GAME)
	{
		PK_UI_Change(UI_GAME_BUTTONS);
		PisteDraw2_SetXOffset(0);

		if (levels[jakso_indeksi_nyt].passed)
			uusinta = true;
		else
			uusinta = false;

		if (!episode_started)
		{
			jakso_lapaisty = false;

			Game::Gifts->clean(); //Reset gifts
			Game::Sprites->clear(); //Reset sprites
			Game::Sprites->protot_clear_all(); //Reset prototypes

			if (PK_Map_Open(next_map) == 1) {
				PisteLog_Write("PK2Main", "Couldn't open map " + std::string(next_map), TYPE::T_ERROR);
			}

			PK_Fadetext_Init(); //Reset fade text

			Game::Gifts->clean();
			episode_started = true;
			music_volume = settings.music_max_volume;
			degree = 0;
			item_paneeli_x = -215;
			piste_lisays = 0;

			// Kinda weird and lazy solution, but I'm tired and this is the best I can think of right now lol
			for (int i = 0; i < Game::Sprites->sprites.size(); i++) {
				for (size_t j = 0; j < levels[jakso_indeksi_nyt].collectables_index.size(); j++) {
					//if (Game::Sprites->spritet[i]. == TYPE_COLLECTABLE) {
						PK2Vector2D* v = &levels[jakso_indeksi_nyt].collectables_index[j];

						if (Game::Sprites->sprites[i].originalX == v->x && Game::Sprites->sprites[i].originalY == v->y) {
							Game::Sprites->sprites[i].piilota = true;
						}
					//}
				}
			}

			// Restore level from checkpoint, if it has been reached
			if (checkpoint_data != nullptr) {
				PK_Checkpoint_Restore();
			}

			PK_Calculate_Tiles();
		} else {
			degree = degree_temp;
		}
	}

	// Start pontuation
	if (game_next_screen == SCREEN_SCORING) {
		PK_UI_Change(UI_CURSOR);
		if (settings.isWide)
			PisteDraw2_SetXOffset(80);
		else
			PisteDraw2_SetXOffset(0);
		PisteDraw2_ScreenFill(0);

		PisteDraw2_Image_Delete(kuva_tausta);
		kuva_tausta = PisteDraw2_Image_Load("gfx/menu.bmp", true);
		PK_MenuShadow_Create(kuva_tausta, 640, 480, 30);

		jakso_uusi_ennatys = false;
		jakso_uusi_ennatysaika = false;
		episodi_uusi_ennatys = false;

		// Lasketaan pelaajan kokonaispisteet etuk�teen
		DWORD temp_pisteet = 0;
		temp_pisteet += jakso_pisteet;
		temp_pisteet += timeout * 5;
		temp_pisteet += Game::Sprites->player->energia * 300;
		for (int i = 0; i < MAX_GIFTS; i++)
			if (Game::Gifts->get(i) != -1)
				temp_pisteet += Game::Gifts->get_protot(i)->pisteet + 500;

		//if (levels[jakso_indeksi_nyt].lapaisty)
		//if (levels[jakso_indeksi_nyt].jarjestys == jakso-1)
		pisteet += temp_pisteet;

		if (uusinta)
			pisteet -= temp_pisteet;

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
		vertailun_tulos = PK_EpisodeScore_Compare(jakso_indeksi_nyt, temp_pisteet, kartta->aika - timeout, false);
		if (vertailun_tulos > 0)
		{
			PK_EpisodeScore_Save(pisteet_tiedosto);
		}

		/* Tutkitaan onko pelaaja rikkonut episodin piste-enn�tyksen */
		vertailun_tulos = PK_EpisodeScore_Compare(0, pisteet, 0, true);
		if (vertailun_tulos > 0)
			PK_EpisodeScore_Save(pisteet_tiedosto);

		music_volume = settings.music_max_volume;

		siirry_pistelaskusta_karttaan = false;

		level_stats.overall_collectables_collected += level_stats.collectables_collected[jakso_indeksi_nyt];
		//levels[jakso_indeksi_nyt].collectables_collected = 0;
		
		for (int i = 0; i < EPISODI_MAX_LEVELS; i++) {
			if (levels[i].is_secret) {
				if ((levels[i].unlock_type == UNLOCK::SCORE && pisteet >= levels[i].unlock_score) || (levels[i].unlock_type == UNLOCK::COLLECTABLES && level_stats.overall_collectables_collected >= levels[i].unlock_collectables)) {
					levels[i].secret_shown = true;
				}
			}
		}

		PisteDraw2_FadeIn(PD_FADE_FAST);
	}

	// Start intro
	if (game_next_screen == SCREEN_INTRO)
	{
		//PisteLog_Kirjoita("- Initializing intro screen\n");
		PK_UI_Change(UI_TOUCH_TO_START);
		if (settings.isWide)
			PisteDraw2_SetXOffset(80);
		else
			PisteDraw2_SetXOffset(0);
		PisteDraw2_ScreenFill(0);

		//PisteLog_Kirjoita("  - Loading picture: gfx/intro.bmp\n");
		PisteDraw2_Image_Delete(kuva_tausta);
		kuva_tausta = PisteDraw2_Image_Load("gfx/intro.bmp", true);

		//PisteLog_Kirjoita("  - Loading music: music/INTRO.XM\n");

		if (PisteSound_StartMusic("music/intro.xm") != 0)
		{
			PK2_error = true;
			PK2_error_msg = "Can't load intro.xm";
		}

		music_volume = settings.music_max_volume;

		introlaskuri = 0;
		siirry_pistelaskusta_karttaan = false;

		PisteDraw2_FadeIn(PD_FADE_FAST);
	}

	// Start ending
	if (game_next_screen == SCREEN_END)
	{
		PK_UI_Change(UI_TOUCH_TO_START);
		if (settings.isWide)
			PisteDraw2_SetXOffset(80);
		else
			PisteDraw2_SetXOffset(0);
		PisteDraw2_ScreenFill(0);
		PisteDraw2_Image_Delete(kuva_tausta);
		kuva_tausta = PisteDraw2_Image_Load("gfx/ending.bmp", true);

		if (PisteSound_StartMusic("music/intro.xm") != 0)
		{
			PK2_error = true;
			PK2_error_msg = "Can't load intro.xm";
		}

		music_volume = settings.music_max_volume;

		loppulaskuri = 0;
		siirry_lopusta_menuun = false;
		episode_started = false;

		PisteDraw2_FadeIn(PD_FADE_FAST);
	}

	game_screen = game_next_screen;

	return 0;
}


//The game loop
int PK_MainScreen() {

	if (game_next_screen != game_screen) PK_MainScreen_Change();
	
	PK_Update_Mouse();
	
	switch (game_screen){
		case SCREEN_GAME    : PK_MainScreen_InGame();     break;
		case SCREEN_MENU    : PK_MainScreen_Menu();       break;
		case SCREEN_MAP     : PK_MainScreen_Map();        break;
		case SCREEN_SCORING : PK_MainScreen_ScoreCount(); break;
		case SCREEN_INTRO   : PK_MainScreen_Intro();      break;
		case SCREEN_END     : PK_MainScreen_End();        break;
		default             : PK_Fade_Quit();                  break;
	}

	// Update music volume
	bool update = false;
	if (music_volume != music_volume_now)
		update = true;

	if (update) {
		if (settings.music_max_volume > 64)
			settings.music_max_volume = 64;

		if (settings.music_max_volume < 0)
			settings.music_max_volume = 0;

		if (music_volume > settings.music_max_volume)
			music_volume = settings.music_max_volume;

		if (music_volume_now < music_volume)
			music_volume_now++;

		if (music_volume_now > music_volume)
			music_volume_now--;

		if (music_volume_now > 64)
			music_volume_now = 64;

		if (music_volume_now < 0)
			music_volume_now = 0;

		PisteSound_SetMusicVolume(music_volume_now);
	}

	if (test_mute) {
		PisteSound_SetMusicVolume(0);
	}

	static bool wasPressed = false;

	bool skipped = !skip_frame && doublespeed; // If is in double speed and don't skip this frame, so the last frame was skipped, and it wasn't drawn
	if ((PisteInput_Keydown(PI_ESCAPE) | PisteInput_Gamepad_Button(PI_CONTROLLER_START)) && key_delay == 0 && !skipped){ //Don't activate menu whith a not drawn screen
		if(test_level)
			PK_Fade_Quit();
		else{
			if (menu_nyt != MENU_MAIN || game_screen != SCREEN_MENU){
				game_next_screen = SCREEN_MENU;
				menu_nyt = MENU_MAIN;
				degree_temp = degree;
			}
			else if (game_screen == SCREEN_MENU && !wasPressed && (PisteInput_Keydown(PI_ESCAPE) | PisteInput_Gamepad_Button(PI_CONTROLLER_START)) && menu_lue_kontrollit == 0){ // Just pressed escape in menu
				if(menu_valittu_id == menu_valinta_id-1)
					PK_Fade_Quit();
				else {
					menu_valittu_id = menu_valinta_id-1; // Set to "exit" option
					//window_activated = false;
					//PisteInput_ActivateWindow(false);
				}
			}
		}
	}

	wasPressed = PisteInput_Keydown(PI_ESCAPE) | PisteInput_Gamepad_Button(PI_CONTROLLER_START);

	if (closing_game && !PisteDraw2_IsFading() || PK2_error)
		Engine->stop();
	
	return 0;
}

//==================================================
//(#16) Process Functions
//==================================================

void PK_Start_Test(std::string arg){
	if (arg.empty()) return;

	int pos = arg.find("/");
	std::string episode = arg.substr(0, pos);
	std::string map = arg.substr(pos, arg.length() - 1);

	// TODO Load whole episode?

	current_episode = episode;
	strcpy(next_map, map.c_str());

	PK_Load_InfoText();
	PK_New_Game();
}

void PK_Unload(){
	if (!unload){
		PisteSound_StopMusic();

		Game::Sprites->protot_clear_all();
		Game::Sprites->clear();

		delete kartta;
		delete tekstit;
		delete Game::Particles;
		delete Game::Sprites;
		delete Game::Gifts;
		unload = true;
	}
}

void PK_Fade_Quit() {
	if(!closing_game) PisteDraw2_FadeOut(PD_FADE_FAST);
	closing_game = true;
	music_volume = 0;
}

void quit(int ret) {
	delete checkpoint_data;
	checkpoint_data = nullptr;

	Game::Sprites->protot_clear_all();

	PK_Settings_Save("data/settings.ini");
	PK_Unload();

	PisteLog_Quit();

	delete Engine;
	Engine = nullptr;

	if (!ret) printf("Exited correctely\n");
	//exit(ret);
}

int main(int argc, char *argv[]) {
	char* test_path = NULL;
	bool path_set = false;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "version") == 0) {
			printf(PK2_VERSION);
			printf("\n");
			exit(0);
		}

		if (strcmp(argv[i], "dev") == 0) {
			dev_mode = true;
			continue;
		}

		if (strcmp(argv[i], "test") == 0) {
			if (argc <= i + 1) {
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "No level to test...", "No .map file provided to test!", nullptr);
				//printf("Please set a level to test\n");
				exit(1);
			} else {
				test_level = true;

				test_path = argv[i + 1];
				continue;
			}
		}

		if (strcmp(argv[i], "windowed") == 0) {
			test_windowed = true;
		}

		if (strcmp(argv[i], "nosound") == 0) {
			test_mute = true;
		}

		if (strcmp(argv[i], "path") == 0) {
			if (argc <= i + 1) {
				printf("Please set a path\n");
				exit(1);
			}
			else {
				printf("PK2    - Path set to %s\n", argv[i + 1]);
				if (chdir(argv[i + 1]))
					path_set = true;
				continue;
			}
		}

		if (strcmp(argv[i], "fps") == 0) {
			show_fps = true;
			continue;
		}

	}

	PisteLog_Init(true);

	if(!path_set)
		PisteUtils_Setcwd();
	strcpy(tyohakemisto,".");

	PK_Settings_Open("data/settings.ini");

	#ifdef __ANDROID__
	screen_width = 800;
	#endif

	Engine = new Piste::Game(screen_width, screen_height, GAME_NAME, "gfx/icon.bmp");

	if (!Engine->is_ready()) {
		printf("PK2    - Failed to init PisteEngine.\n");
		return 0;
	}

	if (dev_mode) Engine->set_debug(true);

	tekstit = new PisteLanguage();

	if (!PK_Load_Language()){
		PisteLog_Write("PK2Main", "Couldn't find language file: " + std::string(settings.kieli), TYPE::T_ERROR);

		strcpy(settings.kieli,"english.txt");
		if(!PK_Load_Language()){
			PisteLog_Write("PK2Main", "Couldn't find default language file", TYPE::T_ERROR);

			return 0;
		}
	}

	PK_MainScreen_Change();

	#ifdef __ANDROID__
	PK_UI_Load();
	#endif

	if (settings.show_intro) {
		game_next_screen = SCREEN_INTRO;
	} else {
		game_next_screen = SCREEN_MENU;
	}

	if (dev_mode)
		game_next_screen = SCREEN_MENU;
	if (test_level && test_path != nullptr) {
		game_next_screen = SCREEN_GAME;
		
		PK_Start_Test(test_path);
	}

	Engine->loop(PK_MainScreen); //The game loop calls PK_MainScreen().

	if(PK2_error){
		printf("PK2    - Error!\n");
		PisteUtils_Show_Error(PK2_error_msg);
	}
	
	quit(0);

	return 0;
}
