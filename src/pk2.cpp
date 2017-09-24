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
//Known bugs:
// -There is no colision between player and sprites wall
// -Sounds volume and frequency don't work
// -Set controlls
// -Menu background
//
//TODO:
// -Draw movable blocks outside screen
//#########################

#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif

#include "PisteEngine.h"
#include "map.h"
#include "sprite.h"

using namespace std;

#define GAME_NAME "Pekka Kana 2"

#ifndef _WIN32
void itoa(int n, char s[], int radix){
	sprintf(s, "%i", n);
}
void ltoa(long n, char s[], int radix){
	sprintf(s, "%i", (int)n);
}
#endif

//#### Constants

const int MAX_ESINEITA = 4;
const int MAX_TALLENNUKSIA = 10;

//Blocks
const BYTE BLOCK_TAUSTA		= 0;
const BYTE BLOCK_SEINA		= 1;
const BYTE BLOCK_MAKI_OIKEA_YLOS = 2;
const BYTE BLOCK_MAKI_VASEN_YLOS = 3;
const BYTE BLOCK_MAKI_OIKEA_ALAS = 4;
const BYTE BLOCK_MAKI_VASEN_ALAS = 5;
const BYTE BLOCK_MAKI_YLOS = 6;
const BYTE BLOCK_MAKI_ALAS = 7;

const BYTE BLOCK_MAX_MASKEJA = 150;

struct PK2BLOCK{
	BYTE		koodi;
	bool		tausta;
	BYTE		vasemmalle, oikealle, ylos, alas;
	int			vasen, oikea, yla, ala;
	bool		vesi;
	bool		reuna;
};

struct PK2BLOCKMASKI{
	short int	ylos[32];
	short int	alas[32];
	short int	vasemmalle[32];
	short int	oikealle[32];
};

//Particles
const BYTE PARTIKKELI_EI_MIKAAN = 0;
const BYTE PARTIKKELI_TAHTI	    = 1;
const BYTE PARTIKKELI_HOYHEN	  = 2;
const BYTE PARTIKKELI_POLYPILVI = 3;
const BYTE PARTIKKELI_VALO		  = 4;
const BYTE PARTIKKELI_KIPINA	  = 5;
const BYTE PARTIKKELI_PISTE	    = 6;
const BYTE PARTIKKELI_SAVUPILVI = 7;
const BYTE PARTIKKELI_TAHTI2	  = 8;

const BYTE TAUSTAPARTIKKELI_VESIPISARA = 1;
const BYTE TAUSTAPARTIKKELI_LEHTI1		= 2;
const BYTE TAUSTAPARTIKKELI_LEHTI2		= 3;
const BYTE TAUSTAPARTIKKELI_LEHTI3		= 4;
const BYTE TAUSTAPARTIKKELI_LEHTI4		= 5;
const BYTE TAUSTAPARTIKKELI_HIUTALE1	= 6;
const BYTE TAUSTAPARTIKKELI_HIUTALE2	= 7;
const BYTE TAUSTAPARTIKKELI_HIUTALE3	= 8;
const BYTE TAUSTAPARTIKKELI_HIUTALE4	= 9;

const int MAX_PARTIKKELEITA = 300;
const int MAX_TAUSTAPARTIKKELEITA = 200;
const int MAX_FADETEKSTEJA  = 40;


struct PK2PARTIKKELI{
	int			tyyppi;
	double		x;
	double		y;
	double		a;
	double		b;
	int			anim;
	int			aika;
	double		paino;
	int			vari;
};

PK2PARTIKKELI partikkelit[MAX_PARTIKKELEITA];
int partikkeli_index = 0;

PK2PARTIKKELI taustapartikkelit[MAX_TAUSTAPARTIKKELEITA];
int taustapartikkeli_index = 0;

//Episode
const int EPISODI_MAX_JAKSOJA = 50;
const int MAX_EPISODEJA	= 300;

const int MAX_ILMOITUKSENNAYTTOAIKA = 700;

//PK2EPISODESCORES
struct PK2EPISODIPISTEET{
	// parhaat pisteet kullekin jaksolle episodissa..
	DWORD pisteet[EPISODI_MAX_JAKSOJA];
	// eniten pisteit� ker�nneet pelaajat kussakin jaksossa episodissa..
	char top_pelaajat[EPISODI_MAX_JAKSOJA][20];
	// parhaat ajat kullekin jaksolle episodissa..
	DWORD ajat[EPISODI_MAX_JAKSOJA];
	// nopeimmat pelaajat kussakin jaksossa episodissa..
	char nopeimmat_pelaajat[EPISODI_MAX_JAKSOJA][20];

	DWORD episodin_top_pisteet;
	char  episodin_top_pelaaja[20];
};

//Screen
const int TILA_EI_ALUSTETTU = 0;
const int TILA_PERUSALUSTUS = 1;
const int TILA_INTRO = 2;
const int TILA_MENUT = 3;
const int TILA_KARTTA= 4;
const int TILA_PELI  = 5;
const int TILA_PISTELASKU = 6;
const int TILA_LOPPU = 7;

//Menu
const BYTE MENU_PAAVALIKKO = 0;
const BYTE MENU_EPISODIT   = 1;
const BYTE MENU_KONTROLLIT = 2;
const BYTE MENU_GRAFIIKKA  = 3;
const BYTE MENU_AANET      = 4;
const BYTE MENU_NIMI		= 5;
const BYTE MENU_LATAA		= 6;
const BYTE MENU_TALLENNA	= 7;
const BYTE MENU_LANGUAGE   = 8;

//Sound
const int AANET_STEREO			= 1;
const int AANET_SAMPLERATE		= 22050;
const int AANET_BITRATE			= 8;

//#### Structs
//PK2??
struct PK2JAKSO{
	char	tiedosto[_MAX_PATH];
	char	nimi[40];
	int		x,y;
	int		jarjestys;
	bool	lapaisty;
	int		ikoni;
};

//PK2FADETEXT
struct PK2FADETEKSTI{
	char teksti[20];
	int fontti;
	int x,y,ajastin;
};

//PK2RECORDING ??
struct PK2TALLENNUS{
	int   jakso;
	char  episodi[_MAX_PATH];
	char  nimi[20];
	bool  kaytossa;
	bool  jakso_lapaisty[EPISODI_MAX_JAKSOJA];
	DWORD pisteet;
};

//PK2SETTINGS
struct PK2ASETUKSET{
	// versio
	double versio;
	// alustetaan jos ei ole ladattu
	bool ladattu;
	// kieli
	char kieli[_MAX_PATH];
	// grafiikka
	int ruudun_korkeus;
	int ruudun_leveys;
	bool lapinakyvat_objektit;
	bool lapinakyvat_menutekstit;
	bool saa_efektit;
	bool nayta_tavarat;
	bool tausta_spritet;
	// kontrollit
	int  kontrolli_vasemmalle;
	int  kontrolli_oikealle;
	int  kontrolli_hyppy;
	int  kontrolli_alas;
	int  kontrolli_juoksu;
	int  kontrolli_hyokkays1;
	int  kontrolli_hyokkays2;
	int  kontrolli_kayta_esine;
	// audio
	bool musiikki;
	bool aanet;
};



//#### Global Variables

int RUUDUN_LEVEYS				= 640;
int RUUDUN_KORKEUS				= 480;

int KARTANPIIRTO_LEVEYS   = 800;
int KARTANPIIRTO_KORKEUS  = 480;
bool RAJAA_KARTANPIIRTOALUE = true;

bool huijaukset = false;

bool PK2_virhe				= false;// jos t�m� muuttuu todeksi niin ohjelma lopetetaan

bool window_closed	= false;// onko ikkuna kiinni
bool lopeta_peli = false;

bool running = true;
bool unload = false;
bool taulut_laskettu = false;

//ASETUKSET
PK2ASETUKSET asetukset;

bool alkusetup = true;

//INFO MUUTTUJAT
bool	piirra_infot = false;
int		info_spriteja = 0;
int		info_spriteja_piirretty = 0;
int		info_spriteja_aktiivisena = 0;

char	viesti[60];

//KARTTA
PK2Kartta *kartta;
char seuraava_kartta[_MAX_PATH];

//SPRITET JA NIIDEN PROTOTYYPIT
PK2Sprite_Prototyyppi protot[MAX_PROTOTYYPPEJA];

PK2Sprite spritet[MAX_SPRITEJA];

int taustaspritet[MAX_SPRITEJA];

int seuraava_vapaa_proto = 0;

PK2Sprite_Prototyyppi *esineet[MAX_ESINEITA];
int esineita = 0;

//PALIKAT JA MASKIT
PK2BLOCK	palikat[300];

PK2BLOCK	lasketut_palikat[150];//150

PK2BLOCKMASKI palikkamaskit[BLOCK_MAX_MASKEJA];

//Fade Text
PK2FADETEKSTI fadetekstit[MAX_FADETEKSTEJA];
int fadeteksti_index = 0;

//Screen Buffers
int  kuva_peli = -1;
int  kuva_peli_sysmem = -1;
int  kuva_tausta = -1;

//Fonts
int fontti1;
int fontti2;
int fontti3;
int fontti4;
int fontti5;

//Controls
int hiiri_x = 10;
int hiiri_y = 10;
double menukursori_x = RUUDUN_LEVEYS / 2;
double menukursori_y = RUUDUN_KORKEUS / 2;
double menukursori_xk = 0;
double menukursori_yk = 0;

int  kontrolli_vasemmalle		= PI_LEFT;
int  kontrolli_oikealle			= PI_RIGHT;
int  kontrolli_hyppy			= PI_UP;
int  kontrolli_alas				= PI_DOWN;
int  kontrolli_juoksu			= PI_RALT;
int  kontrolli_hyokkays1		= PI_RCONTROL;
int  kontrolli_hyokkays2		= PI_RSHIFT;
int  kontrolli_kayta_esine		= PI_SPACE;
int  key_delay = 0;

//KAMERAN KORDINAATIT
int kamera_x = 0;
int kamera_y = 0;
double dkamera_x = 0;
double dkamera_y = 0;
double dkamera_a = 0;
double dkamera_b = 0;

//JAKSO JA EPISODI
int	jakso = 1;
int jaksoja = 1;
int episodi_lkm = 0;
int jakso_indeksi_nyt = 1;
char episodit[MAX_EPISODEJA][_MAX_PATH];
char episodi[_MAX_PATH];
int  episodisivu = 0;
PK2JAKSO jaksot[EPISODI_MAX_JAKSOJA];
bool jakso_lapaisty = false;
bool uusinta = false;
bool peli_ohi = false;
DWORD lopetusajastin = 0;
DWORD jakso_pisteet = 0;
DWORD fake_pisteet = 0;

//PELAAJA
int pelaaja_index = 0;
DWORD pisteet = 0;
DWORD piste_lisays = 0;
char pelaajan_nimi[20] = " ";

bool nimiedit = false;

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

int aanenvoimakkuus = 90;

//MUSIIKKI
int   musiikin_voimakkuus = 64;
int   musiikin_max_voimakkuus = 64;
int   musiikin_voimakkuus_nyt = 64;

//TALLENNUKSET
PK2TALLENNUS tallennukset[MAX_TALLENNUKSIA];
int lataa_peli = -1;

//MUUTA
double cos_table[360];
double sin_table[360];

int degree = 0,
	degree_temp = 0;

int avaimia = 0;

DWORD aika = 0;
int sekunti = 0;
bool aikaraja = false;

int jaristys = 0;
int kytkin_tarina = 0;

int item_paneeli_x = 10;

int ilmoitus_ajastin = 0;
char ilmoitus[80] = " ";

//PISTEIDEN LASKEMINEN
PK2EPISODIPISTEET episodipisteet;

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
char tyohakemisto[_MAX_PATH];
bool paused = false;
int pelin_tila = TILA_EI_ALUSTETTU;
int pelin_seuraava_tila = TILA_PERUSALUSTUS;
bool peli_kesken = false;
bool siirry_kartasta_peliin = false;
bool siirry_pistelaskusta_karttaan = false;

int nakymattomyys = 0;

//INTRO
DWORD introlaskuri = 0;
bool siirry_introsta_menuun = false;

//LOPPURUUTU
DWORD loppulaskuri = 0;
bool siirry_lopusta_menuun = false;

//GRAFIIKKA
bool tuplanopeus = false;
bool skip_frame = false;

//Menus
int menu_nyt = MENU_PAAVALIKKO;
int menu_lue_kontrollit = 0;
int menu_nimi_index = 0;
char menu_nimi_ed_merkki = '\0';
int menu_valittu_id = 0;
int menu_valinta_id = 1;
RECT menunelio;

//Framerate
int   fps = 0;
int   fps_laskuri = 0;
bool  fps_nayta = false;

//LANGUAGE AND TEXTS OF THE GAME
PisteLanguage *tekstit;
char langlist[60][_MAX_PATH];
char langmenulist[10][_MAX_PATH];
int langlistindex = 0;
int totallangs = 0;
int txt_setup_options,
	txt_setup_videomodes,
	txt_setup_music_and_sounds,
	txt_setup_music,
	txt_setup_sounds,
	txt_setup_language,
	txt_setup_play,
	txt_setup_exit,

	txt_intro_presents,
	txt_intro_a_game_by,
	txt_intro_original,
	txt_intro_tested_by,
	txt_intro_thanks_to,
	txt_intro_translation,
	txt_intro_translator,

	txt_mainmenu_new_game,
	txt_mainmenu_continue,
	txt_mainmenu_load_game,
	txt_mainmenu_save_game,
	txt_mainmenu_controls,
	txt_mainmenu_graphics,
	txt_mainmenu_sounds,
	txt_mainmenu_exit,

	txt_mainmenu_return,

	txt_loadgame_title,
	txt_loadgame_info,
	txt_loadgame_episode,
	txt_loadgame_level,

	txt_savegame_title,
	txt_savegame_info,
	txt_savegame_episode,
	txt_savegame_level,

	txt_controls_title,
	txt_controls_moveleft,
	txt_controls_moveright,
	txt_controls_jump,
	txt_controls_duck,
	txt_controls_walkslow,
	txt_controls_eggattack,
	txt_controls_doodleattack,
	txt_controls_useitem,
	txt_controls_edit,
	txt_controls_kbdef,
	txt_controls_gp4def,
	txt_controls_gp6def,

	txt_gfx_title,
	txt_gfx_tfx_on,
	txt_gfx_tfx_off,
	txt_gfx_tmenus_on,
	txt_gfx_tmenus_off,
	txt_gfx_items_on,
	txt_gfx_items_off,
	txt_gfx_weather_on,
	txt_gfx_weather_off,
	txt_gfx_bgsprites_on,
	txt_gfx_bgsprites_off,
	txt_gfx_speed_normal,
	txt_gfx_speed_double,

	txt_sound_title,
	txt_sound_sfx_volume,
	txt_sound_music_volume,
	txt_sound_more,
	txt_sound_less,

	txt_playermenu_type_name,
	txt_playermenu_continue,
	txt_playermenu_clear,
	txt_player_default_name,

	txt_episodes_choose_episode,
	txt_episodes_no_maps,
	txt_episodes_get_more,

	txt_map_total_score,
	txt_map_next_level,
	txt_map_episode_best_player,
	txt_map_episode_hiscore,
	txt_map_level_best_player,
	txt_map_level_hiscore,
	txt_map_level_fastest_player,
	txt_map_level_best_time,

	txt_score_screen_title,
	txt_score_screen_level_score,
	txt_score_screen_bonus_score,
	txt_score_screen_time_score,
	txt_score_screen_energy_score,
	txt_score_screen_item_score,
	txt_score_screen_total_score,
	txt_score_screen_new_level_hiscore,
	txt_score_screen_new_level_best_time,
	txt_score_screen_new_episode_hiscore,
	txt_score_screen_continue,

	txt_game_score,
	txt_game_time,
	txt_game_energy,
	txt_game_items,
	txt_game_attack1,
	txt_game_attack2,
	txt_game_keys,
	txt_game_clear,
	txt_game_timebonus,
	txt_game_ko,
	txt_game_timeout,
	txt_game_tryagain,
	txt_game_locksopen,
	txt_game_newdoodle,
	txt_game_newegg,
	txt_game_newitem,
	txt_game_loading,
	txt_game_paused,

	txt_end_congratulations,
	txt_end_chickens_saved,
	txt_end_the_end,

	txt_info01,
	txt_info02,
	txt_info03,
	txt_info04,
	txt_info05,
	txt_info06,
	txt_info07,
	txt_info08,
	txt_info09,
	txt_info10,
	txt_info11,
	txt_info12,
	txt_info13,
	txt_info14,
	txt_info15,
	txt_info16,
	txt_info17,
	txt_info18,
	txt_info19;


//#### Prototypes

int  PK_Kartta_Aseta_Spritet();
void PK_Esineet_Alusta();
void PK_Lisaa_Episodin_Hakemisto(char *tiedosto);





//#### Functions

//PK_Check_File
bool PK_Onko_File(char *filename){
	struct stat st;
	bool ret = (stat(filename, &st) == 0);
	if(!ret) printf("Asked about non-existing file: %s\n", filename);
	return ret;
}
//PK_Settings_Start
void PK_Asetukset_Alusta(){
	asetukset.ladattu = false;
	strcpy(asetukset.kieli,"english.txt");

	asetukset.ruudun_leveys = 640;
	asetukset.ruudun_korkeus = 480;
	asetukset.lapinakyvat_objektit = true;
	asetukset.lapinakyvat_menutekstit = false;
	asetukset.saa_efektit = true;
	asetukset.nayta_tavarat = true;
	asetukset.tausta_spritet = true;

	asetukset.aanet = true;
	asetukset.musiikki = true;

	asetukset.kontrolli_vasemmalle		= PI_LEFT;
	asetukset.kontrolli_oikealle		= PI_RIGHT;
	asetukset.kontrolli_hyppy			= PI_UP;
	asetukset.kontrolli_alas			= PI_DOWN;
	asetukset.kontrolli_juoksu			= PI_RALT;
	asetukset.kontrolli_hyokkays1		= PI_RCONTROL;
	asetukset.kontrolli_hyokkays2		= PI_RSHIFT;
	asetukset.kontrolli_kayta_esine		= PI_SPACE;
}
//PK_Settings_Open
int PK_Asetukset_Lataa(char *filename){
	ifstream *tiedosto = new ifstream(filename, ios::binary);
	char versio[4];

	if (tiedosto->fail()){
		delete (tiedosto);
		PK_Asetukset_Alusta();
		return 1;
	}

	tiedosto->read((char*)versio, 4);

	if (strcmp(versio,"1.0") == 0)
		tiedosto->read((char*)&asetukset, sizeof(asetukset));

	delete (tiedosto);

	asetukset.ladattu = true;
	kontrolli_vasemmalle	= asetukset.kontrolli_vasemmalle;
	kontrolli_oikealle		= asetukset.kontrolli_oikealle;
	kontrolli_hyppy			= asetukset.kontrolli_hyppy;
	kontrolli_alas			= asetukset.kontrolli_alas;
	kontrolli_juoksu		= asetukset.kontrolli_juoksu;
	kontrolli_hyokkays1		= asetukset.kontrolli_hyokkays1;
	kontrolli_hyokkays2		= asetukset.kontrolli_hyokkays2;
	kontrolli_kayta_esine	= asetukset.kontrolli_kayta_esine;

	return 0;
}
//PK_Settings_Save
int PK_Asetukset_Tallenna(char *filename){
	asetukset.ruudun_korkeus	= RUUDUN_KORKEUS;
	asetukset.ruudun_leveys		= RUUDUN_LEVEYS;

	asetukset.kontrolli_vasemmalle		= kontrolli_vasemmalle;
	asetukset.kontrolli_oikealle		= kontrolli_oikealle;
	asetukset.kontrolli_hyppy			= kontrolli_hyppy;
	asetukset.kontrolli_alas			= kontrolli_alas;
	asetukset.kontrolli_juoksu			= kontrolli_juoksu;
	asetukset.kontrolli_hyokkays1		= kontrolli_hyokkays1;
	asetukset.kontrolli_hyokkays2		= kontrolli_hyokkays2;
	asetukset.kontrolli_kayta_esine		= kontrolli_kayta_esine;

	ofstream *tiedosto = new ofstream(filename, ios::binary);
	tiedosto->write ("1.0", 4);
	tiedosto->write ((char *)&asetukset, sizeof (asetukset));

	delete (tiedosto);

	return 0;
}
//PK_EpisodeScore_Start
void PK_Episodipisteet_Alusta(){
	for (int i=0;i<EPISODI_MAX_JAKSOJA;i++){
		episodipisteet.pisteet[i] = 0;
		episodipisteet.ajat[i] = 0;
		strcpy(episodipisteet.top_pelaajat[i]," ");
		strcpy(episodipisteet.nopeimmat_pelaajat[i]," ");
	}

	episodipisteet.episodin_top_pisteet = 0;
	strcpy(episodipisteet.episodin_top_pelaaja," ");
}
//PK_EpisodeScore_Compare
int PK_Episodipisteet_Vertaa(int jakso, DWORD episteet, DWORD aika, bool loppupisteet){
	int paluu = 0;
	if (!loppupisteet) {
		if (episteet > episodipisteet.pisteet[jakso]) {
			strcpy(episodipisteet.top_pelaajat[jakso],pelaajan_nimi);
			episodipisteet.pisteet[jakso] = episteet;
			jakso_uusi_ennatys = true;
			paluu++;
		}
		if ((aika < episodipisteet.ajat[jakso] || episodipisteet.ajat[jakso] == 0) && kartta->aika > 0) {
			strcpy(episodipisteet.nopeimmat_pelaajat[jakso],pelaajan_nimi);
			episodipisteet.ajat[jakso] = aika;
			jakso_uusi_ennatysaika = true;
			paluu++;
		}
	}
	else {
		if (episteet > episodipisteet.episodin_top_pisteet) {
		    episodipisteet.episodin_top_pisteet = episteet;
			strcpy(episodipisteet.episodin_top_pelaaja,pelaajan_nimi);
			episodi_uusi_ennatys = true;
			paluu++;
		}
	}
	return paluu;
}
//PK_EpisodeScore_Open
int PK_Episodipisteet_Lataa(char *filename){
	PK_Lisaa_Episodin_Hakemisto(filename);

	ifstream *tiedosto = new ifstream(filename, ios::binary);
	char versio[4];

	if (tiedosto->fail()){
		delete (tiedosto);
		PK_Episodipisteet_Alusta();
		return 1;
	}

	tiedosto->read ((char *)versio, 4);

	if (strcmp(versio,"1.0") == 0) {
		tiedosto->read ((char *)&episodipisteet, sizeof (episodipisteet));
	}

	delete (tiedosto);

	return 0;
}
//PK_EpisodeScore_Save
int PK_Episodipisteet_Tallenna(char *filename){
	PK_Lisaa_Episodin_Hakemisto(filename);

	ofstream *tiedosto = new ofstream(filename, ios::binary);
	tiedosto->write ("1.0", 4);
	tiedosto->write ((char *)&episodipisteet, sizeof (episodipisteet));
	delete (tiedosto);
	return 0;
}
//PK_Load_InfoText
void PK_Lataa_Lisainfot(){
	PisteLanguage* temp;
	char infofile[_MAX_PATH] = "infosign.txt";
	char otsikko[] = "info00";
	int indeksi1, indeksi2, i;

	temp = new PisteLanguage();
	PK_Lisaa_Episodin_Hakemisto(infofile);

	if (PK_Onko_File(infofile)){
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
//PK_Load_Language
bool PK_Lataa_Kieli(){
	char tiedosto[_MAX_PATH];
	int i;

	strcpy(tiedosto,"language/");

	if(totallangs == 0){
		totallangs = PisteUtils_Scandir(".txt", tiedosto, langlist, 60);
		for(i=0;i<10;i++)
			strcpy(langmenulist[i],langlist[i]);
	}

	strcat(tiedosto,asetukset.kieli);

	if (!tekstit->Read_File(tiedosto)){
		//PK2_virhe = true;
		return false;
	}
	// Aloitusikkuna
	txt_setup_options			= tekstit->Hae_Indeksi("setup options");
	txt_setup_videomodes		= tekstit->Hae_Indeksi("setup video modes");
	txt_setup_music_and_sounds	= tekstit->Hae_Indeksi("setup music & sounds");
	txt_setup_music				= tekstit->Hae_Indeksi("setup music");
	txt_setup_sounds			= tekstit->Hae_Indeksi("setup sounds");
	txt_setup_language			= tekstit->Hae_Indeksi("setup language");
	txt_setup_play				= tekstit->Hae_Indeksi("setup play");
	txt_setup_exit				= tekstit->Hae_Indeksi("setup exit");

	// Intro
	txt_intro_presents			= tekstit->Hae_Indeksi("intro presents");
	txt_intro_a_game_by			= tekstit->Hae_Indeksi("intro a game by");
	txt_intro_original			= tekstit->Hae_Indeksi("intro original character design");
	txt_intro_tested_by			= tekstit->Hae_Indeksi("intro tested by");
	txt_intro_thanks_to			= tekstit->Hae_Indeksi("intro thanks to");
	txt_intro_translation		= tekstit->Hae_Indeksi("intro translation");
	txt_intro_translator		= tekstit->Hae_Indeksi("intro translator");

	// P��valikko
	txt_mainmenu_new_game		= tekstit->Hae_Indeksi("main menu new game");
	txt_mainmenu_continue		= tekstit->Hae_Indeksi("main menu continue");
	txt_mainmenu_load_game		= tekstit->Hae_Indeksi("main menu load game");
	txt_mainmenu_save_game		= tekstit->Hae_Indeksi("main menu save game");
	txt_mainmenu_controls		= tekstit->Hae_Indeksi("main menu controls");
	txt_mainmenu_graphics		= tekstit->Hae_Indeksi("main menu graphics");
	txt_mainmenu_sounds			= tekstit->Hae_Indeksi("main menu sounds");
	txt_mainmenu_exit			= tekstit->Hae_Indeksi("main menu exit game");

	txt_mainmenu_return			= tekstit->Hae_Indeksi("back to main menu");

	// Lataus
	txt_loadgame_title			= tekstit->Hae_Indeksi("load menu title");
	txt_loadgame_info			= tekstit->Hae_Indeksi("load menu info");
	txt_loadgame_episode		= tekstit->Hae_Indeksi("load menu episode");
	txt_loadgame_level			= tekstit->Hae_Indeksi("load menu level");

	// Tallennus
	txt_savegame_title			= tekstit->Hae_Indeksi("save menu title");
	txt_savegame_info			= tekstit->Hae_Indeksi("save menu info");
	txt_savegame_episode		= tekstit->Hae_Indeksi("save menu episode");
	txt_savegame_level			= tekstit->Hae_Indeksi("save menu level");

	// Kontrollit
	txt_controls_title			= tekstit->Hae_Indeksi("controls menu title");
	txt_controls_moveleft		= tekstit->Hae_Indeksi("controls menu move left");
	txt_controls_moveright		= tekstit->Hae_Indeksi("controls menu move right");
	txt_controls_jump			= tekstit->Hae_Indeksi("controls menu jump");
	txt_controls_duck			= tekstit->Hae_Indeksi("controls menu duck");
	txt_controls_walkslow		= tekstit->Hae_Indeksi("controls menu walk slow");
	txt_controls_eggattack		= tekstit->Hae_Indeksi("controls menu egg attack");
	txt_controls_doodleattack	= tekstit->Hae_Indeksi("controls menu doodle attack");
	txt_controls_useitem		= tekstit->Hae_Indeksi("controls menu use item");
	txt_controls_edit			= tekstit->Hae_Indeksi("controls menu edit");
	txt_controls_kbdef			= tekstit->Hae_Indeksi("controls menu keyboard def");
	txt_controls_gp4def			= tekstit->Hae_Indeksi("controls menu gamepad4");
	txt_controls_gp6def			= tekstit->Hae_Indeksi("controls menu gamepad6");

	txt_gfx_title				= tekstit->Hae_Indeksi("graphics menu title");
	txt_gfx_tfx_on				= tekstit->Hae_Indeksi("graphics menu transparency fx on");
	txt_gfx_tfx_off				= tekstit->Hae_Indeksi("graphics menu transparency fx off");
	txt_gfx_tmenus_on			= tekstit->Hae_Indeksi("graphics menu menus are transparent");
	txt_gfx_tmenus_off			= tekstit->Hae_Indeksi("graphics menu menus are not transparent");
	txt_gfx_items_on			= tekstit->Hae_Indeksi("graphics menu item bar is visible");
	txt_gfx_items_off			= tekstit->Hae_Indeksi("graphics menu item bar is not visible");
	txt_gfx_weather_on			= tekstit->Hae_Indeksi("graphics menu weather fx on");
	txt_gfx_weather_off			= tekstit->Hae_Indeksi("graphics menu weather fx off");
	txt_gfx_bgsprites_on		= tekstit->Hae_Indeksi("graphics menu bg sprites on");
	txt_gfx_bgsprites_off		= tekstit->Hae_Indeksi("graphics menu bg sprites off");
	txt_gfx_speed_normal		= tekstit->Hae_Indeksi("graphics menu game speed normal");
	txt_gfx_speed_double		= tekstit->Hae_Indeksi("graphics menu game speed double");

	txt_sound_title				= tekstit->Hae_Indeksi("sounds menu title");
	txt_sound_sfx_volume		= tekstit->Hae_Indeksi("sounds menu sfx volume");
	txt_sound_music_volume		= tekstit->Hae_Indeksi("sounds menu music volume");
	txt_sound_more				= tekstit->Hae_Indeksi("sounds menu more");
	txt_sound_less				= tekstit->Hae_Indeksi("sounds menu less");

	txt_playermenu_type_name	= tekstit->Hae_Indeksi("player screen type your name");
	txt_playermenu_continue		= tekstit->Hae_Indeksi("player screen continue");
	txt_playermenu_clear		= tekstit->Hae_Indeksi("player screen clear");
	txt_player_default_name		= tekstit->Hae_Indeksi("player default name");

	txt_episodes_choose_episode	= tekstit->Hae_Indeksi("episode menu choose episode");
	txt_episodes_no_maps		= tekstit->Hae_Indeksi("episode menu no maps");
	txt_episodes_get_more		= tekstit->Hae_Indeksi("episode menu get more episodes at");

	txt_map_total_score			= tekstit->Hae_Indeksi("map screen total score");
	txt_map_next_level			= tekstit->Hae_Indeksi("map screen next level");
	txt_map_episode_best_player	= tekstit->Hae_Indeksi("episode best player");
	txt_map_episode_hiscore		= tekstit->Hae_Indeksi("episode hiscore");
	txt_map_level_best_player	= tekstit->Hae_Indeksi("level best player");
	txt_map_level_hiscore		= tekstit->Hae_Indeksi("level hiscore");
	txt_map_level_fastest_player= tekstit->Hae_Indeksi("level fastest player");
	txt_map_level_best_time		= tekstit->Hae_Indeksi("level best time");

	txt_score_screen_title			= tekstit->Hae_Indeksi("score screen title");
	txt_score_screen_level_score	= tekstit->Hae_Indeksi("score screen level score");
	txt_score_screen_bonus_score	= tekstit->Hae_Indeksi("score screen bonus score");
	txt_score_screen_time_score		= tekstit->Hae_Indeksi("score screen time score");
	txt_score_screen_energy_score	= tekstit->Hae_Indeksi("score screen energy score");
	txt_score_screen_item_score		= tekstit->Hae_Indeksi("score screen item score");
	txt_score_screen_total_score	= tekstit->Hae_Indeksi("score screen total score");
	txt_score_screen_new_level_hiscore	= tekstit->Hae_Indeksi("score screen new level hiscore");
	txt_score_screen_new_level_best_time= tekstit->Hae_Indeksi("score screen new level best time");
	txt_score_screen_new_episode_hiscore= tekstit->Hae_Indeksi("score screen new episode hiscore");
	txt_score_screen_continue		= tekstit->Hae_Indeksi("score screen continue");

	txt_game_score				= tekstit->Hae_Indeksi("score");
	txt_game_time				= tekstit->Hae_Indeksi("game time");
	txt_game_energy				= tekstit->Hae_Indeksi("energy");
	txt_game_items				= tekstit->Hae_Indeksi("items");
	txt_game_attack1			= tekstit->Hae_Indeksi("attack 1");
	txt_game_attack2			= tekstit->Hae_Indeksi("attack 2");
	txt_game_keys				= tekstit->Hae_Indeksi("keys");
	txt_game_clear				= tekstit->Hae_Indeksi("level clear");
	txt_game_timebonus			= tekstit->Hae_Indeksi("time bonus");
	txt_game_ko					= tekstit->Hae_Indeksi("knocked out");
	txt_game_timeout			= tekstit->Hae_Indeksi("time out");
	txt_game_tryagain			= tekstit->Hae_Indeksi("try again");
	txt_game_locksopen			= tekstit->Hae_Indeksi("locks open");
	txt_game_newdoodle			= tekstit->Hae_Indeksi("new doodle attack");
	txt_game_newegg				= tekstit->Hae_Indeksi("new egg attack");
	txt_game_newitem			= tekstit->Hae_Indeksi("new item");
	txt_game_loading			= tekstit->Hae_Indeksi("loading");
	txt_game_paused				= tekstit->Hae_Indeksi("game paused");

	txt_end_congratulations	= tekstit->Hae_Indeksi("end congratulations");
	txt_end_chickens_saved	= tekstit->Hae_Indeksi("end chickens saved");
	txt_end_the_end			= tekstit->Hae_Indeksi("end the end");

	txt_info01					= tekstit->Hae_Indeksi("info01");
	txt_info02					= tekstit->Hae_Indeksi("info02");
	txt_info03					= tekstit->Hae_Indeksi("info03");
	txt_info04					= tekstit->Hae_Indeksi("info04");
	txt_info05					= tekstit->Hae_Indeksi("info05");
	txt_info06					= tekstit->Hae_Indeksi("info06");
	txt_info07					= tekstit->Hae_Indeksi("info07");
	txt_info08					= tekstit->Hae_Indeksi("info08");
	txt_info09					= tekstit->Hae_Indeksi("info09");
	txt_info10					= tekstit->Hae_Indeksi("info10");
	txt_info11					= tekstit->Hae_Indeksi("info11");
	txt_info12					= tekstit->Hae_Indeksi("info12");
	txt_info13					= tekstit->Hae_Indeksi("info13");
	txt_info14					= tekstit->Hae_Indeksi("info14");
	txt_info15					= tekstit->Hae_Indeksi("info15");
	txt_info16					= tekstit->Hae_Indeksi("info16");
	txt_info17					= tekstit->Hae_Indeksi("info17");
	txt_info18					= tekstit->Hae_Indeksi("info18");
	txt_info19					= tekstit->Hae_Indeksi("info19");

	return true;
}
//PK_Load_WorkingDir
void PK_Lisaa_Tyohakemisto(char *tiedosto){
	char uusi_tiedosto[255];

	strcpy(uusi_tiedosto, tyohakemisto);
	strcat(uusi_tiedosto, "/");
	strcat(uusi_tiedosto, tiedosto);
	strcpy(tiedosto, uusi_tiedosto);
}
//PK_Load_EpisodeDir
void PK_Lisaa_Episodin_Hakemisto(char *tiedosto){
	char uusi_tiedosto[255];

	strcpy(uusi_tiedosto, tyohakemisto);
	strcat(uusi_tiedosto, "/episodes/");
	strcat(uusi_tiedosto, episodi);
	strcat(uusi_tiedosto, "/");
	strcat(uusi_tiedosto, tiedosto);
	strcpy(tiedosto, uusi_tiedosto);
}
//PK_New_Game
void PK_Uusi_Peli(){
	pisteet = 0;
	jakso = 1;
}
//PK_Save_New ??
void PK_Uusi_Jakso(){
	aika = kartta->aika;

	if (aika > 0)
		aikaraja = true;
	else
		aikaraja = false;

	lopetusajastin = 0;

	sekunti = 1000;
	jakso_pisteet = 0;
	peli_ohi = false;
	jakso_lapaisty = false;
	kytkin1 = 0;
	kytkin2 = 0;
	kytkin3 = 0;
	kytkin_tarina = 0;
	jaristys = 0;

	paused = false;

	ilmoitus_ajastin = 0;

	nakymattomyys = 0;
}
//PK_Save_Start ??
void PK_Jaksot_Alusta(){
	for (int i=0;i<EPISODI_MAX_JAKSOJA;i++){
		strcpy(jaksot[i].nimi,"");
		strcpy(jaksot[i].tiedosto,"");
		jaksot[i].x = 0;
		jaksot[i].y = 0;
		jaksot[i].jarjestys = -1;
		jaksot[i].lapaisty = false;
		jaksot[i].ikoni = 0;
	}
}
//PK_Search_Dir -- PisteUtils_Search_Dir
void PK_Jaksot_Hae(){
	int i=0;
	char hakemisto[_MAX_PATH];
	char list[EPISODI_MAX_JAKSOJA][_MAX_PATH];
	for (int j = 0; j < EPISODI_MAX_JAKSOJA; j++)
		memset(list[j], '\0', _MAX_PATH);

	PK2Kartta *temp = new PK2Kartta();

	strcpy(hakemisto,"");
	PK_Lisaa_Episodin_Hakemisto(hakemisto);
	jaksoja = PisteUtils_Scandir(".map", hakemisto, list, EPISODI_MAX_JAKSOJA);

	for (i=0;i<=jaksoja;i++){
		strcpy(jaksot[i].tiedosto,list[i]);
		if (temp->Lataa_Pelkat_Tiedot(hakemisto,jaksot[i].tiedosto) == 0){
			strcpy(jaksot[i].nimi, temp->nimi);
			jaksot[i].x = temp->x;//   142 + i*35;
			jaksot[i].y = temp->y;// 270;
			jaksot[i].jarjestys = temp->jakso;
			jaksot[i].ikoni = temp->ikoni;
		}
	}

	PK2JAKSO jakso;

	bool lopeta = false;

	while (!lopeta){
		lopeta = true;

		for (i=0;i<jaksoja;i++){
			if (jaksot[i].jarjestys > jaksot[i+1].jarjestys){
				jakso = jaksot[i];
				jaksot[i] = jaksot[i+1];
				jaksot[i+1] = jakso;
				lopeta = false;
			}
		}
	}
	delete temp;
}
//PK_Episode_Compare ?? (Piste_Utils)
int PK_Episodit_Vertaa(char *a, char *b){
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
//PK_Episode_Order ?? (Piste_Utils)
int PK_Episodit_Aakkosta(){
	DWORD i,t;
	char temp[_MAX_PATH] = "";
	bool tehty;

	if (episodi_lkm > 1) {

		for (i=episodi_lkm-1;i>=0;i--) {

			tehty = true;

			//for (t=0;t<i;t++) {
			for (t=2;t<i+2;t++) {
				if (PK_Episodit_Vertaa(episodit[t],episodit[t+1]) == 1) {
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
//PK_Episode_Search
int PK_Episodit_Hae(){
	int i;
	char hakemisto[_MAX_PATH];

	for (i=0;i<MAX_EPISODEJA;i++)
		strcpy(episodit[i],"");

	strcpy(hakemisto,"episodes/");

	episodi_lkm = PisteUtils_Scandir("/", hakemisto, episodit, MAX_EPISODEJA) - 2;

	PK_Episodit_Aakkosta();

	return 0;
}
//PK_Records_Empty
int PK_Tallennukset_Tyhjenna(){
	for (int i = 0;i < MAX_TALLENNUKSIA;i++){
		tallennukset[i].kaytossa = false;
		strcpy(tallennukset[i].episodi," ");
		strcpy(tallennukset[i].nimi,"empty");
		tallennukset[i].jakso = 0;
		tallennukset[i].pisteet = 0;
		for (int j = 0;j < EPISODI_MAX_JAKSOJA;j++)
			tallennukset[i].jakso_lapaisty[j] = false;
	}

	return 0;
}
//PK_Records_Search
int PK_Tallennukset_Hae_Kaikki(char *filename){
	char versio[2];
	char lkmc[8];
	int lkm, i;

	ifstream *tiedosto = new ifstream(filename, ios::binary);

	if (tiedosto->fail()){
		delete (tiedosto);
		PK_Tallennukset_Tyhjenna();
		return 1;
	}

	PK_Tallennukset_Tyhjenna();

	tiedosto->read(versio,	sizeof(versio));

	if (strcmp(versio,"1")==0){
		tiedosto->read(lkmc, sizeof(lkmc));
		lkm = atoi(lkmc);

		for (i=0;i<lkm;i++)
			tiedosto->read ((char *)&tallennukset[i], sizeof (tallennukset[i]));
	}

	delete (tiedosto);

	return 0;
}
//PK_Records_SaveAll
int PK_Tallennukset_Tallenna_Kaikki(char *filename){
	char versio[2] = "1";
	char lkm[8];

	itoa(MAX_TALLENNUKSIA,lkm,10);

	ofstream *tiedosto = new ofstream(filename, ios::binary);
	tiedosto->write(versio,	sizeof(versio));
	tiedosto->write(lkm,	sizeof(lkm));
	for (int i=0;i< MAX_TALLENNUKSIA;i++)
	{
		tiedosto->write((char *)&tallennukset[i],	sizeof(tallennukset[i]));
	}

	delete (tiedosto);

	return 0;
}
//PK_Records_Load
int PK_Tallennukset_Lataa(int i){
	if (strcmp(tallennukset[i].episodi," ")!=0){
		strcpy(episodi,tallennukset[i].episodi);
		strcpy(pelaajan_nimi, tallennukset[i].nimi);
		jakso = tallennukset[i].jakso;
		pisteet = tallennukset[i].pisteet;

		PK_Jaksot_Alusta();

		//for (int j = 0;j < EPISODI_MAX_JAKSOJA;j++)
		//	jaksot[j].lapaisty = tallennukset[i].jakso_lapaisty[j];

		pelin_seuraava_tila = TILA_KARTTA;
		lataa_peli = i;
		peli_kesken = false;
	}

	return 0;
}
//PK_Records_Save
int PK_Tallennukset_Tallenna(int i){
	tallennukset[i].kaytossa = true;
	strcpy(tallennukset[i].episodi, episodi);
	strcpy(tallennukset[i].nimi,pelaajan_nimi);
	tallennukset[i].jakso = jakso;
	tallennukset[i].pisteet = pisteet;

	for (int j = 0;j < EPISODI_MAX_JAKSOJA;j++)
		tallennukset[i].jakso_lapaisty[j] = jaksot[j].lapaisty;

	PK_Tallennukset_Tallenna_Kaikki("data/saves.dat");

	return 0;
}
//PK_Play_Sound
void PK_Soita_Aani(int aani, int voimakkuus, int x, int y, int freq, bool random_freq){
	if (aani > -1 && aanenvoimakkuus > 0 && voimakkuus > 0){
		if (x < kamera_x+RUUDUN_LEVEYS && x > kamera_x && y < kamera_y+RUUDUN_KORKEUS && y > kamera_y){
			voimakkuus = voimakkuus / (100 / aanenvoimakkuus);

			if (voimakkuus > 100)
				voimakkuus = 100;

			if (voimakkuus < 0)
				voimakkuus = 0;

			//int pan = kamera_x + (RUUDUN_LEVEYS / 2) - x;

			if (random_freq)
				freq = freq + rand()%4000 - rand()%2000;

			PisteSound_PlaySFX(aani,aanenvoimakkuus, freq);
		}
	}
}
//PK_Play_MenuSound
void PK_Soita_Aani_Menu(int aani, int voimakkuus){
	if (aani > -1 && aanenvoimakkuus > 0 && voimakkuus > 0){
		voimakkuus = voimakkuus / (100 / aanenvoimakkuus);

		if (voimakkuus > 100)
			voimakkuus = 100;

		if (voimakkuus < 0)
			voimakkuus = 0;

		int freq = 22050 + rand()%5000 - rand()%5000;

		PisteSound_PlaySFX(aani, aanenvoimakkuus, freq);
	}
}
//PK_Calculate_Tables
void PK_Laske_Taulut(){
	int i;
	for (i=0; i<360; i++) cos_table[i] = cos(3.1415*2* (i%360)/180)*33;
	for (i=0; i<360; i++) sin_table[i] = sin(3.1415*2* (i%360)/180)*33;
}
//PK_Calculate_MovableBlockPosition
void PK_Palikka_Paivita_Lasketut_Palikat(){
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

		if (kytkin1 > KYTKIN_ALOITUSARVO-64)
			kytkin1_y = KYTKIN_ALOITUSARVO - kytkin1;
	}

	if (kytkin2 > 0)
	{
		kytkin2_y = 64;

		if (kytkin2 < 64)
			kytkin2_y = kytkin2;

		if (kytkin2 > KYTKIN_ALOITUSARVO-64)
			kytkin2_y = KYTKIN_ALOITUSARVO - kytkin2;
	}

	if (kytkin3 > 0)
	{
		kytkin3_x = 64;

		if (kytkin3 < 64)
			kytkin3_x = kytkin3;

		if (kytkin3 > KYTKIN_ALOITUSARVO-64)
			kytkin3_x = KYTKIN_ALOITUSARVO - kytkin3;
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
//PK_Tiles_Calculate
int PK_Palikka_Laske_Palikat(){
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

	PK_Palikka_Paivita_Lasketut_Palikat();

	return 0;
}
//PK_Tiles_DoMasks ??
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
//Remove colors from blocks buffer ???
int PK_Poista_Vari_254_Palikoista(){
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
//Borrão ?
int PK_Sumenna_Kuva(int kbuffer, DWORD kleveys, int kkorkeus){
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

	kerroin = 3;//2.25;//2

	//for (y=0;y<kkorkeus;y++)
	for (y=35;y<kkorkeus-30;y++)
	{
		my = (y)*leveys;
		//for(x=0;x<kleveys;x++)
		for(x=30;x<kleveys-30;x++)
		{
			mx = x+my;
			vari   = buffer[mx];

			vari32 = VARI_TURKOOSI;//(vari>>5)<<5;
			vari %= 32;

			if (x == 30 || x == kleveys-31 || y == 35 || y == kkorkeus-31)
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
//Copy the screen to bg buffer
int PK_Kopioi_Pelitilanne_Taustakuvaksi(){

	BYTE *screen_buff = NULL, *bg_buff = NULL;
	DWORD screen_w, bg_w;
	int x,y;

	PisteDraw2_DrawScreen_Start(*&screen_buff,(DWORD &)screen_w);
	PisteDraw2_DrawImage_Start(kuva_tausta,*&bg_buff,(DWORD &)bg_w);

	for (x=0;x<640;x++)
		for (y=0;y<480;y++)
			bg_buff[x+y*bg_w] = 0; //TODO - BG

	PisteDraw2_DrawImage_End(kuva_tausta);
	PisteDraw2_DrawScreen_End();

	return 0;
}

void PK_Kartta_Laske_Reunat(){
	BYTE tile1, tile2, tile3;
	bool reuna = false;

	memset(kartta->reunat, false, sizeof(kartta->reunat));

	for (int x=1;x<PK2KARTTA_KARTTA_LEVEYS-1;x++)
		for (int y=0;y<PK2KARTTA_KARTTA_KORKEUS-1;y++)
		{
			reuna = false;

			tile1 = kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS];

			if (tile1 > BLOCK_LOPETUS)
				kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS] = 255;

			tile2 = kartta->seinat[x+(y+1)*PK2KARTTA_KARTTA_LEVEYS];

			if (tile1 > 79 || tile1 == BLOCK_ESTO_ALAS) tile1 = 1; else tile1 = 0;
			if (tile2 > 79) tile2 = 1; else tile2 = 0;

			if (tile1 == 1 && tile2 == 1)
			{
				tile1 = kartta->seinat[x+1+(y+1)*PK2KARTTA_KARTTA_LEVEYS];
				tile2 = kartta->seinat[x-1+(y+1)*PK2KARTTA_KARTTA_LEVEYS];

				if (tile1 < 80  && !(tile1 < 60 && tile1 > 49)) tile1 = 1; else tile1 = 0;
				if (tile2 < 80  && !(tile2 < 60 && tile2 > 49)) tile2 = 1; else tile2 = 0;

				if (tile1 == 1)
				{
					tile3 = kartta->seinat[x+1+y*PK2KARTTA_KARTTA_LEVEYS];
					if (tile3 > 79 || (tile3 < 60 && tile3 > 49) || tile3 == BLOCK_ESTO_ALAS)
						reuna = true;
				}

				if (tile2 == 1)
				{
					tile3 = kartta->seinat[x-1+y*PK2KARTTA_KARTTA_LEVEYS];
					if (tile3 > 79 || (tile3 < 60 && tile3 > 49) || tile3 == BLOCK_ESTO_ALAS)
						reuna = true;
				}

				if (reuna)
				{
					kartta->reunat[x+y*PK2KARTTA_KARTTA_LEVEYS] = true;
					//kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS] = 49;
				}
			}
		}
}

int PK_Piirra_LaineTeksti(char *teksti, int fontti, int x, int y){
	int pituus = strlen(teksti);
	int vali = 0;
	char kirjain[3] = " \0";
	int ys, xs;

	if (pituus > 0)
	{
		for (int i=0;i<pituus;i++)
		{
			ys = (int)(sin_table[((i+degree)*8)%360])/7;
			xs = (int)(cos_table[((i+degree)*8)%360])/9;
			kirjain[0] = teksti[i];
			PisteDraw2_Font_Write(fontti4,kirjain,x+vali-xs+3,y+ys+3);
			vali += PisteDraw2_Font_Write(fontti,kirjain,x+vali-xs,y+ys);
		}
	}
	return vali;
}

int PK_Piirra_LaineTeksti_Hidas(char *teksti, int fontti, int x, int y){
	int pituus = strlen(teksti);
	int vali = 0;
	char kirjain[3] = " \0";
	int ys, xs;

	if (pituus > 0){
		for (int i=0;i<pituus;i++){
			ys = (int)(sin_table[((i+degree)*4)%360])/9;
			xs = (int)(cos_table[((i+degree)*4)%360])/11;
			kirjain[0] = teksti[i];

			if (asetukset.lapinakyvat_menutekstit)
				vali += PisteDraw2_Font_WriteAlpha(fontti,kirjain,x+vali-xs,y+ys,75);
			else{
				PisteDraw2_Font_Write(fontti4,kirjain,x+vali-xs+1,y+ys+1);
				vali += PisteDraw2_Font_Write(fontti,kirjain,x+vali-xs,y+ys);
			}


		}
	}
	return vali;
}
int PK_Piirra_Lapinakyva_Objekti(int lahde_buffer, DWORD lahde_x, DWORD lahde_y, DWORD lahde_leveys, DWORD lahde_korkeus,
						 DWORD kohde_x, DWORD kohde_y, int pros, BYTE vari){
	PD_RECT src = {lahde_x, lahde_y, lahde_leveys, lahde_korkeus};
	PD_RECT dst = {kohde_x, kohde_y, lahde_leveys, lahde_korkeus};
	PisteDraw2_Image_CutClipTransparent(lahde_buffer, src, dst, pros, vari);
	return 0;
}

void PK_Uusi_Ilmoitus(char *teksti){
	if (strcmp(teksti,ilmoitus) != 0 || ilmoitus_ajastin == 0)
	{
		strcpy(ilmoitus,teksti);
		ilmoitus_ajastin = MAX_ILMOITUKSENNAYTTOAIKA;
	}
}

void PK_Alusta_Fadetekstit(){
	for (int i=0;i<MAX_FADETEKSTEJA;i++)
		fadetekstit[i].ajastin = 0;
}

void PK_Fadeteksti_Uusi(int fontti, char *teksti, DWORD x, DWORD y, DWORD ajastin){
	fadetekstit[fadeteksti_index].fontti = fontti;
	strcpy(fadetekstit[fadeteksti_index].teksti,teksti);
	fadetekstit[fadeteksti_index].x = x;
	fadetekstit[fadeteksti_index].y = y;
	fadetekstit[fadeteksti_index].ajastin = ajastin;
	fadeteksti_index++;

	if (fadeteksti_index >= MAX_FADETEKSTEJA)
		fadeteksti_index = 0;
}

int PK_Fadetekstit_Piirra(){
	int pros;

	for (int i=0;i<MAX_FADETEKSTEJA;i++)
	{
		if (fadetekstit[i].ajastin > 0)
		{
			if (fadetekstit[i].ajastin > 50)
				pros = 100;
			else
				pros = fadetekstit[i].ajastin * 2;

			if (asetukset.lapinakyvat_objektit && pros < 100)
			{
				if (PisteDraw2_Font_WriteAlpha( fadetekstit[i].fontti,fadetekstit[i].teksti,
														fadetekstit[i].x-kamera_x, fadetekstit[i].y-kamera_y, pros)==-1)
					return 1;
			}
			else
				PisteDraw2_Font_Write( fadetekstit[i].fontti,fadetekstit[i].teksti,
									 fadetekstit[i].x-kamera_x, fadetekstit[i].y-kamera_y);

		}
	}
	return 0;
}

void PK_Fadetekstit(){
	for (int i=0;i<MAX_FADETEKSTEJA;i++)
	{
		if (fadetekstit[i].ajastin > 0)
		{
			fadetekstit[i].ajastin--;

			if (fadetekstit[i].ajastin%2 == 0)
				fadetekstit[i].y--;

			if (fadetekstit[i].x < kamera_x || fadetekstit[i].x > kamera_x + RUUDUN_LEVEYS ||
				fadetekstit[i].y < kamera_y || fadetekstit[i].y > kamera_y + RUUDUN_KORKEUS)
				fadetekstit[i].ajastin = 0;
		}
	}
}

//==================================================
//Particle System
//==================================================

int PK_Piirra_Piste(DWORD x, DWORD y, int pros, BYTE vari){
	PisteDraw2_ScreenFill(x-kamera_x, y-kamera_y,x-kamera_x+1,y-kamera_y+1,vari+25);

	return 0;
}

int PK_Piirra_Tahti(DWORD x, DWORD y, int pros, BYTE vari){
	if (pros > 99 || !asetukset.lapinakyvat_objektit)
		PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x, y-kamera_y,1,1,11,11);
	else
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 2, 2, 10, 10, x-kamera_x, y-kamera_y, pros, vari);

	return 0;
}

int PK_Piirra_Iskuvalays(DWORD x, DWORD y){
	int framex = ((degree%12)/3) * 58;
	PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x-28+8, y-kamera_y-27+8,1+framex,83,1+57+framex,83+55);//140<-83
	return 0;
}

int PK_Piirra_Valo(DWORD x, DWORD y, int pros, BYTE vari){
	if (asetukset.lapinakyvat_objektit)
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 1, 14, 13, 13, x-kamera_x, y-kamera_y, pros, vari);
	else{
		int vx = (vari/32) * 14;
		PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x, y-kamera_y,1+vx,14+14,14+vx,27+14);
	}

	return 0;
}

int PK_Piirra_Kipina(DWORD x, DWORD y, int pros, BYTE vari){
	if (asetukset.lapinakyvat_objektit)
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 99, 14, 7, 7, x-kamera_x, y-kamera_y, pros, vari);
	else{
		int vx = (vari/32) * 8;
		PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x, y-kamera_y,99+vx,14+14,106+vx,21+14);
	}

	return 0;
}

void PK_Piirra_Hoyhen(int x, int y, int &anim){
	int xplus = (anim/7) * 21;
	PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x,y-kamera_y,14+xplus,1,34+xplus,12);
	anim++;
	if (anim > 63)
		anim = 0;
}

void PK_Piirra_Savupilvi(int x, int y, int &anim){
	int frame = (anim/7);
	int xplus = (frame%17) * 36;
	int yplus = 0;

	if (anim < 7*30) {

		if (frame > 16)
			yplus = 32;

		PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x,y-kamera_y,1+xplus,338+yplus,34+xplus,366+yplus);
		anim++;
	}

	//if (anim > 63)
	//	anim = 0;
}

void PK_Piirra_Polypilvi(DWORD x, DWORD y, int pros, BYTE vari){
	if (pros > 99 || !asetukset.lapinakyvat_objektit)
		PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x,y-kamera_y,226,2,224,49);
	else
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 226, 2, 18, 19, x-kamera_x, y-kamera_y, pros, vari);
	PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x,y-kamera_y,226, 2, 18, 19); //Tirar isso
}
//PK_Particles_Clear
int PK_Partikkelit_Tyhjenna(){
	int i = 0;

	partikkeli_index = 0;

	while (i < MAX_PARTIKKELEITA){
		partikkelit[i].a = 0;
		partikkelit[i].aika = 0;
		partikkelit[i].anim = 0;
		partikkelit[i].b = 0;
		partikkelit[i].tyyppi = PARTIKKELI_EI_MIKAAN;
		partikkelit[i].x = 0;
		partikkelit[i].y = 0;
		partikkelit[i].paino = 0;
		partikkelit[i].vari = 0;
		i++;
	}

	return 0;
}
//PK_Particles_New
void PK_Partikkeli_Uusi(int tyyppi, double x, double y, double a, double b, int aika, double paino, int vari){ //Create particles
	if (x < kamera_x+RUUDUN_LEVEYS+10 && x > kamera_x-10 &&
		y < kamera_y+RUUDUN_KORKEUS+10 && y > kamera_y-10){
		if (partikkeli_index >= MAX_PARTIKKELEITA)
			partikkeli_index = 0;

		partikkelit[partikkeli_index].a = a;
		partikkelit[partikkeli_index].aika = aika;
		partikkelit[partikkeli_index].anim = rand()%63;
		partikkelit[partikkeli_index].b = b;
		partikkelit[partikkeli_index].tyyppi = tyyppi;
		partikkelit[partikkeli_index].x = x;
		partikkelit[partikkeli_index].y = y;
		partikkelit[partikkeli_index].paino = paino;
		partikkelit[partikkeli_index].vari = vari;

		partikkeli_index++;
	}
}
//PK_Particles_Draw
int	PK_Partikkelit_Piirra(){
	int pros;
	int piirtoleveys, piirtokorkeus;

	if (RAJAA_KARTANPIIRTOALUE){
		piirtoleveys  = KARTANPIIRTO_LEVEYS;
		piirtokorkeus = KARTANPIIRTO_KORKEUS;
	}
	else{
		piirtoleveys  = RUUDUN_LEVEYS;
		piirtokorkeus = RUUDUN_KORKEUS;
	}

	for (int i=0;i<MAX_PARTIKKELEITA;i++){
		if (partikkelit[i].aika > 0){
			if (partikkelit[i].x < kamera_x+piirtoleveys &&
				partikkelit[i].x > kamera_x &&
				partikkelit[i].y < kamera_y+piirtokorkeus &&
				partikkelit[i].y > kamera_y) {
				if (partikkelit[i].aika < 100)
					pros = partikkelit[i].aika;
				else
					pros = 100;

				switch (partikkelit[i].tyyppi){
				case PARTIKKELI_TAHTI		:	PK_Piirra_Tahti((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break; //Star
				case PARTIKKELI_HOYHEN		:	PK_Piirra_Hoyhen((int)partikkelit[i].x,(int)partikkelit[i].y,partikkelit[i].anim);
												break; //Feather
				case PARTIKKELI_POLYPILVI	:	PK_Piirra_Polypilvi((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break; //Dust
				case PARTIKKELI_SAVUPILVI	:	PK_Piirra_Savupilvi((int)partikkelit[i].x,(int)partikkelit[i].y, partikkelit[i].anim);
												break; //Smoke
				case PARTIKKELI_VALO		:	PK_Piirra_Valo((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break; //Light
				case PARTIKKELI_KIPINA		:	PK_Piirra_Kipina((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break; //Spark
				case PARTIKKELI_PISTE		:	PK_Piirra_Piste((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break; //Score
				default						:	break;
				}
			}
		}
		else
			partikkelit[i].aika = 0;
	}
	return 0;
}
//PK_Particles_Update
int	PK_Partikkelit(){
	for (int i=0;i<MAX_PARTIKKELEITA;i++){
		if (partikkelit[i].aika > 0){
			partikkelit[i].x += partikkelit[i].a;
			partikkelit[i].y += partikkelit[i].b;

			if (partikkelit[i].paino > 0)
				partikkelit[i].b += partikkelit[i].paino;

			partikkelit[i].aika--;
		}
	}
	return 0;
}
//PK_Particles_Empty
int PK_Taustapartikkelit_Tyhjenna(){
	int i = 0;

	taustapartikkeli_index = 0;

	//kartta->ilma = ILMA_SADE;

	while (i < MAX_TAUSTAPARTIKKELEITA)
	{
		taustapartikkelit[i].a = rand()%10-rand()%10;
		taustapartikkelit[i].aika = 1;
		taustapartikkelit[i].anim = rand()%10;
		taustapartikkelit[i].b = rand()%9+1;
		taustapartikkelit[i].tyyppi = PARTIKKELI_EI_MIKAAN;
		taustapartikkelit[i].x = rand()%KARTANPIIRTO_LEVEYS;
		taustapartikkelit[i].y = rand()%KARTANPIIRTO_KORKEUS;
		taustapartikkelit[i].paino = rand()%10;
		taustapartikkelit[i].vari = 0;
		i++;
	}

	if (kartta->ilma == ILMA_SADE || kartta->ilma == ILMA_SADEMETSA)
		for(i=0;i < MAX_TAUSTAPARTIKKELEITA;i++)
		{
			taustapartikkelit[i].tyyppi = TAUSTAPARTIKKELI_VESIPISARA;
		}

	if (kartta->ilma == ILMA_METSA || kartta->ilma == ILMA_SADEMETSA)
		for(i=0;i < MAX_TAUSTAPARTIKKELEITA/8;i++)
		{
			taustapartikkelit[i].tyyppi = TAUSTAPARTIKKELI_LEHTI1 + rand()%4;
		}

	if (kartta->ilma == ILMA_LUMISADE){

		for(i=0;i < MAX_TAUSTAPARTIKKELEITA/2;i++)
		{
			taustapartikkelit[i].tyyppi = TAUSTAPARTIKKELI_HIUTALE4;
		}
		for(i=0;i < MAX_TAUSTAPARTIKKELEITA/3;i++)
		{
			taustapartikkelit[i].tyyppi = TAUSTAPARTIKKELI_HIUTALE1 + rand()%2+1;//3
		}
	}
	return 0;
}

void PK_Taustapartikkelit_Vesipisara(PK2PARTIKKELI &p){
	p.y += p.b / 3.0 + 2.0;

	int kx = (int)(p.x-kamera_x);
	int ky = (int)(p.y-kamera_y);

	PisteDraw2_ScreenFill(kx,ky,kx+1,ky+4,40+(int)p.b);
};

void PK_Taustapartikkelit_Lehti1(PK2PARTIKKELI &p){
	p.x += p.a / 9.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw2_ScreenFill(kx,ky,kx+2,ky+2,96+6+(int)p.b+(int)(p.x+p.y)%10);
};

void PK_Taustapartikkelit_Lehti2(PK2PARTIKKELI &p){
	p.x += p.a / 14.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y),
		frame = (int(p.y/10)%4)*23;

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,1+frame,141,21+frame,152);
};

void PK_Taustapartikkelit_Lehti3(PK2PARTIKKELI &p){
	p.x += p.a / 12.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y),
		frame = (int(p.y/5)%4)*20;

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,93+frame,141,109+frame,150);

};

void PK_Taustapartikkelit_Lehti4(PK2PARTIKKELI &p){
	p.x += p.a / 11.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y),
		frame = (int(p.y/5)%2)*14;

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,173+frame,141,183+frame,150);
};

void PK_Taustapartikkelit_Hiutale1(PK2PARTIKKELI &p){
	//p.x += p.a / 11.0;
	p.x += sin_table[int(p.y)%360]/50.0;
	p.y += p.b / 7.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,1,155,8,162);
};

void PK_Taustapartikkelit_Hiutale2(PK2PARTIKKELI &p){
	//p.x += p.a / 11.0;
	p.x += sin_table[int(p.y)%360]/100.0;
	p.y += p.b / 8.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,11,155,16,160);
};

void PK_Taustapartikkelit_Hiutale3(PK2PARTIKKELI &p){
	//p.x += p.a / 11.0;
	p.y += p.b / 8.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,19,155,22,158);
};

void PK_Taustapartikkelit_Hiutale4(PK2PARTIKKELI &p){
	//p.x += p.a / 9.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw2_ScreenFill(kx,ky,kx+2,ky+2,20+(int)p.b);
};

int	PK_Taustapartikkelit(){

	for (int i=0;i<MAX_TAUSTAPARTIKKELEITA;i++)
	{
		if (taustapartikkelit[i].tyyppi != PARTIKKELI_EI_MIKAAN)
		{
			switch(taustapartikkelit[i].tyyppi)
			{
				case TAUSTAPARTIKKELI_VESIPISARA : PK_Taustapartikkelit_Vesipisara(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_LEHTI1	 : PK_Taustapartikkelit_Lehti1(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_LEHTI2	 : PK_Taustapartikkelit_Lehti2(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_LEHTI3	 : PK_Taustapartikkelit_Lehti3(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_LEHTI4	 : PK_Taustapartikkelit_Lehti4(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_HIUTALE1	 : PK_Taustapartikkelit_Hiutale1(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_HIUTALE2	 : PK_Taustapartikkelit_Hiutale2(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_HIUTALE3	 : PK_Taustapartikkelit_Hiutale3(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_HIUTALE4	 : PK_Taustapartikkelit_Hiutale4(taustapartikkelit[i]);break;
				default : taustapartikkelit[i].tyyppi = PARTIKKELI_EI_MIKAAN;
			}

			if (taustapartikkelit[i].x  >  kamera_x + KARTANPIIRTO_LEVEYS)
				taustapartikkelit[i].x  =  kamera_x + int(taustapartikkelit[i].x - kamera_x + KARTANPIIRTO_LEVEYS) % KARTANPIIRTO_LEVEYS;

			if (taustapartikkelit[i].x  <  kamera_x)
				taustapartikkelit[i].x  =  kamera_x + KARTANPIIRTO_LEVEYS - int(kamera_x - taustapartikkelit[i].x) % KARTANPIIRTO_LEVEYS;

			if (taustapartikkelit[i].y  >  kamera_y + KARTANPIIRTO_KORKEUS)
				taustapartikkelit[i].y  =  kamera_y + int(taustapartikkelit[i].y - kamera_y + KARTANPIIRTO_KORKEUS) % KARTANPIIRTO_KORKEUS;

			if (taustapartikkelit[i].y  <  kamera_y)
				taustapartikkelit[i].y  =  kamera_y + KARTANPIIRTO_KORKEUS - int(kamera_y - taustapartikkelit[i].y) % KARTANPIIRTO_KORKEUS;
		}
	}
	return 0;
}

void PK_Tehoste_Hoyhenet(DWORD x, DWORD y){
	for (int i=0;i<9;i++)//6
		PK_Partikkeli_Uusi(	PARTIKKELI_HOYHEN,x+rand()%17-rand()%17,y+rand()%20-rand()%10,
							(rand()%16-rand()%16)/10.0,(45+rand()%45)/100.0,300+rand()%40,0,0);
}

void PK_Tehoste_Loiskahdus(DWORD x, DWORD y, BYTE vari){
	/*
	for (int i=0;i<12;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_VALO,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5,(rand()%7-rand()%7)/3,
							rand()%50+60,0.025,vari);*/
	for (int i=0;i<7;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_KIPINA,x+rand()%17-13,y+rand()%17-13,
							(rand()%5-rand()%5)/4.0,(rand()%4-rand()%7)/3.0,
							rand()%50+40,0.025,vari);//0.015

	for (int i=0;i<20;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_PISTE,x+rand()%17-13,y+rand()%17-13,
							(rand()%5-rand()%5)/4.0,(rand()%2-rand()%7)/3.0,
							rand()%50+40,0.025,vari);//0.015

	PK_Soita_Aani(loiskahdus_aani, 100, x, y, AANET_SAMPLERATE, true);
}

void PK_Tehoste_Rajahdys(DWORD x, DWORD y, BYTE vari){
	int i;

	for (i=0;i<3;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_SAVUPILVI,x+rand()%17-32,y+rand()%17,
							0,double((rand()%4)+3) / -10.0,450,0.0,vari);

	for (i=0;i<9;i++)//12
		PK_Partikkeli_Uusi(	PARTIKKELI_VALO,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5.0,(rand()%7-rand()%7)/3.0,
							rand()%40+60,0.025,vari);

	for (i=0;i<8;i++)//8//10
		PK_Partikkeli_Uusi(	PARTIKKELI_KIPINA,x+rand()%17-13,y+rand()%17-13,
							(rand()%3-rand()%3),//(rand()%7-rand()%7)/5,
							(rand()%7-rand()%7)/3,
							rand()%20+60,0.015,vari);//50+60

	for (i=0;i<20;i++)//12
		PK_Partikkeli_Uusi(	PARTIKKELI_PISTE,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5.0,(rand()%7-rand()%7)/3.0,
							rand()%40+60,0.025,vari);
}

void PK_Tehoste_Savu(DWORD x, DWORD y, BYTE vari){
	for (int i=0;i<3;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_SAVUPILVI,x+rand()%17-32,y+rand()%17,
							0,double((rand()%3)+3) / -10.0/*-0.3*/,450,0.0,vari);
	for (int i=0;i<6;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_POLYPILVI,x+rand()%30-rand()%30-10,y+rand()%30-rand()%30+10,
							0,-0.3,rand()%50+60,0,vari);
}

void PK_Tehoste_Savupilvet(DWORD x, DWORD y){
	for (int i=0;i<5;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_SAVUPILVI,x+rand()%17-32,y+rand()%17,
							0,double((rand()%3)+3) / -10.0/*-0.3*/,450,0.0,0);
}

void PK_Tehoste_Tahdet(DWORD x, DWORD y, BYTE vari){
	for (int i=0;i<5;i++)
		PK_Partikkeli_Uusi(PARTIKKELI_TAHTI,x-5, y-5, (rand()%3-rand()%3)/1.5, rand()%3*-1,100,(rand()%5+5)/100.0/* 0.05*/,vari);//300

	for (int i=0;i<3;i++)//12
		PK_Partikkeli_Uusi(	PARTIKKELI_PISTE,x-5, y-5, (rand()%3-rand()%3)/1.5, rand()%3*-1,100,(rand()%5+5)/100.0,vari);
}

void PK_Tehosteet(BYTE tehoste, DWORD x, DWORD y){
	switch (tehoste){
	case TUHOUTUMINEN_HOYHENET			: PK_Tehoste_Hoyhenet(x, y); break;
	case TUHOUTUMINEN_TAHDET_HARMAA		: PK_Tehoste_Tahdet(x,y,0);break;
	case TUHOUTUMINEN_TAHDET_SININEN	: PK_Tehoste_Tahdet(x,y,32);break;
	case TUHOUTUMINEN_TAHDET_PUNAINEN	: PK_Tehoste_Tahdet(x,y,64);break;
	case TUHOUTUMINEN_TAHDET_VIHREA		: PK_Tehoste_Tahdet(x,y,96);break;
	case TUHOUTUMINEN_TAHDET_ORANSSI	: PK_Tehoste_Tahdet(x,y,128);break;
	case TUHOUTUMINEN_TAHDET_VIOLETTI	: PK_Tehoste_Tahdet(x,y,160);break;
	case TUHOUTUMINEN_TAHDET_TURKOOSI	: PK_Tehoste_Tahdet(x,y,192);break;
	case TUHOUTUMINEN_RAJAHDYS_HARMAA	: PK_Tehoste_Rajahdys(x,y,0);break;
	case TUHOUTUMINEN_RAJAHDYS_SININEN	: PK_Tehoste_Rajahdys(x,y,32);break;
	case TUHOUTUMINEN_RAJAHDYS_PUNAINEN	: PK_Tehoste_Rajahdys(x,y,64);break;
	case TUHOUTUMINEN_RAJAHDYS_VIHREA	: PK_Tehoste_Rajahdys(x,y,96);break;
	case TUHOUTUMINEN_RAJAHDYS_ORANSSI	: PK_Tehoste_Rajahdys(x,y,128);break;
	case TUHOUTUMINEN_RAJAHDYS_VIOLETTI	: PK_Tehoste_Rajahdys(x,y,160);break;
	case TUHOUTUMINEN_RAJAHDYS_TURKOOSI	: PK_Tehoste_Rajahdys(x,y,192);break;
	case TUHOUTUMINEN_SAVU_HARMAA		: PK_Tehoste_Savu(x,y,0);break;
	case TUHOUTUMINEN_SAVU_SININEN		: PK_Tehoste_Savu(x,y,32);break;
	case TUHOUTUMINEN_SAVU_PUNAINEN		: PK_Tehoste_Savu(x,y,64);break;
	case TUHOUTUMINEN_SAVU_VIHREA		: PK_Tehoste_Savu(x,y,96);break;
	case TUHOUTUMINEN_SAVU_ORANSSI		: PK_Tehoste_Savu(x,y,128);break;
	case TUHOUTUMINEN_SAVU_VIOLETTI		: PK_Tehoste_Savu(x,y,160);break;
	case TUHOUTUMINEN_SAVU_TURKOOSI		: PK_Tehoste_Savu(x,y,192);break;
	case TUHOUTUMINEN_SAVUPILVET		: PK_Tehoste_Savupilvet(x,y);break;
	default	: break;
	}
}

//Map sets

//PK_Map_Change_Skull_Blocks
int PK_Kartta_Vaihda_Kallopalikat(){
	BYTE seina, tausta;
	DWORD x,y;

	for (x=0; x<PK2KARTTA_KARTTA_LEVEYS; x++)
		for (y=0; y<PK2KARTTA_KARTTA_KORKEUS; y++){
			seina =  kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS];
			tausta = kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS];

			if (seina == BLOCK_KALLOSEINA){
				kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS] = 255;
				if (tausta != BLOCK_KALLOSEINA)
					PK_Tehoste_Savupilvet(x*32+6,y*32+6);

			}

			if (tausta == BLOCK_KALLOTAUSTA && seina == 255)
				kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS] = BLOCK_KALLOSEINA;
		}

	jaristys = 90;//60

	//PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_game_locksopen));

	PK_Kartta_Laske_Reunat();

	return 0;
}
//Map_Open_Locks
int PK_Kartta_Avaa_Lukot(){
	BYTE palikka;
	DWORD x,y;

	avaimia = 0;
	for (x=0; x<PK2KARTTA_KARTTA_LEVEYS; x++)
		for (y=0; y<PK2KARTTA_KARTTA_KORKEUS; y++){
			palikka = kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS];
			if (palikka == BLOCK_LUKKO){
				kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS] = 255;
				PK_Tehoste_Savupilvet(x*32+6,y*32+6);
			}
		}

	jaristys = 90;//60

	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_game_locksopen));

	PK_Kartta_Laske_Reunat();

	return 0;
}
//Map_Count_Keys ?
int PK_Kartta_Laske_Avaimet(){
	BYTE sprite;
	DWORD x;

	avaimia = 0;

	for (x=0; x<PK2KARTTA_KARTTA_KOKO; x++){
		sprite = kartta->spritet[x];
		if (sprite != 255)
			if (protot[sprite].avain && protot[sprite].tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
				avaimia++;
	}

	return 0;
}
//Map_Search_start
int PK_Kartta_Etsi_Alku(){
	double  alku_x = 320,
			alku_y = 196;
	int		alkujen_maara = 0, alku = 0,
			x, y;

	for (x=0; x<PK2KARTTA_KARTTA_KOKO; x++)
		if (kartta->seinat[x] == BLOCK_ALOITUS)
			alkujen_maara ++;

	if (alkujen_maara > 0){
		alku = rand()%alkujen_maara + 1;
		alkujen_maara = 1;

		for (x=0; x < PK2KARTTA_KARTTA_LEVEYS; x++)
			for (y=0; y < PK2KARTTA_KARTTA_KORKEUS; y++)
				if (kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS] == BLOCK_ALOITUS){
					if (alkujen_maara == alku){
						alku_x = x*32;
						alku_y = y*32;
					}

					alkujen_maara ++;
				}
	}

	spritet[pelaaja_index].x = alku_x+spritet[pelaaja_index].tyyppi->leveys/2;
	spritet[pelaaja_index].y = alku_y-spritet[pelaaja_index].tyyppi->korkeus/2;

	kamera_x = (int)spritet[pelaaja_index].x;
	kamera_y = (int)spritet[pelaaja_index].y;
	dkamera_x = kamera_x;
	dkamera_y = kamera_y;

	return 0;
}

//==================================================
//Sprite Prototypes
//==================================================

//Empty the prototypes
void PK_Prototyyppi_Tyhjenna(){
	for (int i=0; i<MAX_PROTOTYYPPEJA; i++){
		for (int j=0;j<MAX_AANIA;j++)
			if (protot[i].aanet[j] > -1)
				PisteSound_FreeSFX(protot[i].aanet[j]);
		protot[i].Uusi();
		strcpy(kartta->protot[i],"");
	}

	seuraava_vapaa_proto = 0;
}

int PK_Prototyyppi_Lataa_Aani(char *polku, char *tiedosto){
	char aanitiedosto[255];
	if (strcmp(tiedosto,"")!=0){
		strcpy(aanitiedosto,polku);
		strcat(aanitiedosto,tiedosto);
		return PisteSound_LoadSFX(aanitiedosto);
	}

	return -1;
}
//Load sprite prototypes
int PK_Prototyyppi_Lataa_Vanha(char *polku, char *tiedosto){
	char aanipolku[255];
	char testipolku[255];
	strcpy(aanipolku,polku);

	//Check if have space
	if (seuraava_vapaa_proto < MAX_PROTOTYYPPEJA){
		if (protot[seuraava_vapaa_proto].Lataa(polku, tiedosto) == 1)
			return 1;
		protot[seuraava_vapaa_proto].indeksi = seuraava_vapaa_proto;
	}
	else return 2;

	//Load sounds
	for (int i=0;i<MAX_AANIA;i++){

		if (strcmp(protot[seuraava_vapaa_proto].aanitiedostot[i],"")!=0){

			strcpy(testipolku,aanipolku);
			strcat(testipolku,"/");
			strcat(testipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);

			if (PK_Onko_File(testipolku))
				protot[seuraava_vapaa_proto].aanet[i] = PK_Prototyyppi_Lataa_Aani(aanipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);
			else{
			//if (protot[seuraava_vapaa_proto].aanet[i] == -1) {
				//strcpy(aanipolku,"sprites/");
				getcwd(aanipolku, _MAX_PATH);
				strcat(aanipolku,"/sprites/");

				strcpy(testipolku,aanipolku);
				strcat(testipolku,"/");
				strcat(testipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);

				if (PK_Onko_File(testipolku))
					protot[seuraava_vapaa_proto].aanet[i] = PK_Prototyyppi_Lataa_Aani(aanipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);
			}
		}
		//if (protot[seuraava_vapaa_proto].aanet[i] == -1)
		//	return 3;
	}

	//PisteLog_Kirjoita("  - Loading completed. \n");

	seuraava_vapaa_proto++;

	return 0;
}

void PK_Prototyyppi_Aseta_Muutos_Sprite(int i){
	int j = 0;
	bool loytyi = false;

	if (strcmp(protot[i].muutos_sprite,"")!=0){
		while (j<MAX_PROTOTYYPPEJA && !loytyi){
			if (j!=i) {
				if (strcmp(protot[i].muutos_sprite,protot[j].tiedosto)==0){
					protot[i].muutos = j;
					loytyi = true;
				}
			}
			j++;
		}

		if (!loytyi) {
			char polku[_MAX_PATH];
			strcpy(polku,"sprites/");
			//PK_Lisaa_Episodin_Hakemisto(polku);

			if (PK_Prototyyppi_Lataa_Vanha(polku, protot[i].muutos_sprite)==0)
				protot[i].muutos = seuraava_vapaa_proto-1; // jokainen lataus kasvattaa seuraava_vapaa_proto:a
		}
	}
}

void PK_Prototyyppi_Aseta_Bonus_Sprite(int i){
	int j = 0;
	bool loytyi = false;

	if (strcmp(protot[i].bonus_sprite,"")!=0){
		while (j<MAX_PROTOTYYPPEJA && !loytyi){
			if (j!=i){
				if (strcmp(protot[i].bonus_sprite,protot[j].tiedosto)==0){
					protot[i].bonus = j;
					loytyi = true;
				}
			}

			j++;
		}

		if (!loytyi){
			char polku[_MAX_PATH];
			strcpy(polku,"sprites/");
			//PK_Lisaa_Episodin_Hakemisto(polku);

			if (PK_Prototyyppi_Lataa_Vanha(polku, protot[i].bonus_sprite)==0)
				protot[i].bonus = seuraava_vapaa_proto-1;
		}
	}
}

void PK_Prototyyppi_Aseta_Ammus1_Sprite(int i){
	int j = 0;
	bool loytyi = false;

	if (strcmp(protot[i].ammus1_sprite,"")!=0){
		while (j<MAX_PROTOTYYPPEJA && !loytyi){
			if (j!=i){
				if (strcmp(protot[i].ammus1_sprite,protot[j].tiedosto)==0){
					protot[i].ammus1 = j;
					loytyi = true;
				}
			}

			j++;
		}

		if (!loytyi){
			char polku[_MAX_PATH];
			strcpy(polku,"sprites/");
			//PK_Lisaa_Episodin_Hakemisto(polku);


			if (PK_Prototyyppi_Lataa_Vanha(polku, protot[i].ammus1_sprite)==0)
				protot[i].ammus1 = seuraava_vapaa_proto-1;
		}
	}
}

void PK_Prototyyppi_Aseta_Ammus2_Sprite(int i){
	int j = 0;
	bool loytyi = false;

	if (strcmp(protot[i].ammus2_sprite,"")!=0){
		while (j<MAX_PROTOTYYPPEJA && !loytyi){
			if (j!=i){
				if (strcmp(protot[i].ammus2_sprite,protot[j].tiedosto)==0){
					protot[i].ammus2 = j;
					loytyi = true;
				}
			}

			j++;
		}

		if (!loytyi){
			char polku[_MAX_PATH];
			strcpy(polku,"sprites/");
			//PK_Lisaa_Episodin_Hakemisto(polku);

			if (PK_Prototyyppi_Lataa_Vanha(polku, protot[i].ammus2_sprite)==0)
				protot[i].ammus2 = seuraava_vapaa_proto-1;
		}
	}
}

int PK_Prototyyppi_Lataa_Kaikki(){
	char polku[_MAX_PATH];
	int viimeinen_proto;

	for (int i=0;i < MAX_PROTOTYYPPEJA;i++){
		if (strcmp(kartta->protot[i],"") != 0){
			viimeinen_proto = i;
			strcpy(polku,"");
			PK_Lisaa_Episodin_Hakemisto(polku);
			//PK_Lisaa_Episodin_Hakemisto(polku);

			if (PK_Prototyyppi_Lataa_Vanha(polku,kartta->protot[i])==1){
				strcpy(polku,"");
				strcpy(polku,"sprites/");
				//PK_Lisaa_Episodin_Hakemisto(polku);
				if (PK_Prototyyppi_Lataa_Vanha(polku,kartta->protot[i])==1)
					seuraava_vapaa_proto++;
					//return -1;
			}
		}
		else
			seuraava_vapaa_proto++;
	}

	seuraava_vapaa_proto = viimeinen_proto+1;

	for (int i=0;i<MAX_PROTOTYYPPEJA;i++){
		PK_Prototyyppi_Aseta_Muutos_Sprite(i);
		PK_Prototyyppi_Aseta_Bonus_Sprite(i);
		PK_Prototyyppi_Aseta_Ammus1_Sprite(i);
		PK_Prototyyppi_Aseta_Ammus2_Sprite(i);
	}

	return 0;
}

void PK_Sprite_Maaraa_Alkusuunnat(){
	for (int i=0;i<MAX_SPRITEJA;i++){
		if (pelaaja_index >= 0 && pelaaja_index < MAX_SPRITEJA && !spritet[i].piilota){
			spritet[i].a = 0;

			if (spritet[i].tyyppi->Onko_AI(AI_RANDOM_ALOITUSSUUNTA_HORI)){
				while (spritet[i].a == 0) {
					spritet[i].a = ((rand()%2 - rand()%2) * spritet[i].tyyppi->max_nopeus) / 3.5;//2;
				}
			}

			if (spritet[i].tyyppi->Onko_AI(AI_RANDOM_ALOITUSSUUNTA_VERT)){
				while (spritet[i].b == 0) {
					spritet[i].b = ((rand()%2 - rand()%2) * spritet[i].tyyppi->max_nopeus) / 3.5;//2;
				}
			}

			if (spritet[i].tyyppi->Onko_AI(AI_ALOITUSSUUNTA_PELAAJAA_KOHTI)){

				if (spritet[i].x < spritet[pelaaja_index].x)
					spritet[i].a = spritet[i].tyyppi->max_nopeus / 3.5;

				if (spritet[i].x > spritet[pelaaja_index].x)
					spritet[i].a = (spritet[i].tyyppi->max_nopeus * -1) / 3.5;
			}

			if (spritet[i].tyyppi->Onko_AI(AI_ALOITUSSUUNTA_PELAAJAA_KOHTI_VERT)){

				if (spritet[i].y < spritet[pelaaja_index].y)
					spritet[i].b = spritet[i].tyyppi->max_nopeus / -3.5;

				if (spritet[i].y > spritet[pelaaja_index].y)
					spritet[i].b = spritet[i].tyyppi->max_nopeus / 3.5;
			}
		}
	}
}

int PK_Kartta_Lataa(char *nimi){
	PK_Esineet_Alusta();

	PK_Prototyyppi_Tyhjenna();

	char polku[_MAX_PATH];
	strcpy(polku,"");
	PK_Lisaa_Episodin_Hakemisto(polku);

	//PisteLog_Kirjoita("- Trying to load a map: ");
	//PisteLog_Kirjoita(polku);
	//PisteLog_Kirjoita(nimi);
	//PisteLog_Kirjoita("\n");

	if (kartta->Lataa(polku, nimi) == 1){
		//PisteLog_Kirjoita("  - Loading failed. \n");
		return 1;
	}

	PK_Uusi_Jakso();

	if (strcmp(kartta->versio,"1.2") == 0 || strcmp(kartta->versio,"1.3") == 0)
		if (PK_Prototyyppi_Lataa_Kaikki()==1)
			return 1;

	PK_Palikka_Tee_Maskit();

	if (PK_Poista_Vari_254_Palikoista()==1)
		return 1;

	PK_Kartta_Aseta_Spritet();
	PK_Kartta_Etsi_Alku();
	PK_Sprite_Maaraa_Alkusuunnat();
	PK_Kartta_Laske_Avaimet();
	PK_Partikkelit_Tyhjenna();
	PK_Taustapartikkelit_Tyhjenna();
	PK_Kartta_Laske_Reunat();

	if (strcmp(kartta->musiikki,"")!=0){
		char biisi[_MAX_PATH] = "";
		PK_Lisaa_Episodin_Hakemisto(biisi);
		strcat(biisi,kartta->musiikki);
		if (PisteSound_StartMusic(biisi)!=0){
			strcpy(biisi,"music/");
			strcat(biisi,kartta->musiikki);

			if (PisteSound_StartMusic(biisi)!=0)
				if (PisteSound_StartMusic("music/song01.xm")!=0)
					PK2_virhe = true;
		}
	}
	return 0;
}

void PK_Palikka_Este(PK2BLOCK &palikka){
	palikka.tausta = false;

	palikka.oikealle	= BLOCK_SEINA;
	palikka.vasemmalle	= BLOCK_SEINA;
	palikka.ylos		= BLOCK_SEINA;
	palikka.alas		= BLOCK_SEINA;

	// Erikoislattiat

	if (palikka.koodi > 139){
		palikka.oikealle	= BLOCK_TAUSTA;
		palikka.vasemmalle	= BLOCK_TAUSTA;
		palikka.ylos		= BLOCK_TAUSTA;
		palikka.alas		= BLOCK_TAUSTA;
	}

	// Hissit

	if (palikka.koodi == BLOCK_HISSI_HORI){
		palikka.vasen += (int)cos_table[degree%360];
		palikka.oikea += (int)cos_table[degree%360];
	}

	if (palikka.koodi == BLOCK_HISSI_VERT){
		palikka.ala += (int)sin_table[degree%360];
		palikka.yla += (int)sin_table[degree%360];
	}

	// L�pik�velt�v� lattia

	if (palikka.koodi == BLOCK_ESTO_ALAS){
		palikka.oikealle	= BLOCK_TAUSTA;
		palikka.ylos		= BLOCK_TAUSTA;
		palikka.alas		= BLOCK_SEINA;
		palikka.vasemmalle	= BLOCK_TAUSTA;
		palikka.ala -= 27;
	}

	// M�et

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

	// Lattiat joihin kytkimet vaikuttavat

	int kytkin1_y = 0,
		kytkin2_y = 0,
		kytkin3_x = 0;

	if (kytkin1 > 0){
		kytkin1_y = 64;

		if (kytkin1 < 64)
			kytkin1_y = kytkin1;

		if (kytkin1 > KYTKIN_ALOITUSARVO-64)
			kytkin1_y = KYTKIN_ALOITUSARVO - kytkin1;
	}

	if (kytkin2 > 0){
		kytkin2_y = 64;

		if (kytkin2 < 64)
			kytkin2_y = kytkin2;

		if (kytkin2 > KYTKIN_ALOITUSARVO-64)
			kytkin2_y = KYTKIN_ALOITUSARVO - kytkin2;
	}

	if (kytkin3 > 0){
		kytkin3_x = 64;

		if (kytkin3 < 64)
			kytkin3_x = kytkin3;

		if (kytkin3 > KYTKIN_ALOITUSARVO-64)
			kytkin3_x = KYTKIN_ALOITUSARVO - kytkin3;
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
//Get the block from x and y
PK2BLOCK PK_Palikka_Tutki(int x, int y){
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

int PK_Sprite_Lisaa_Taustasprite(int index){
	for (int i=0;i<MAX_SPRITEJA;i++){
		if (taustaspritet[i] == -1){
			taustaspritet[i] = index;
			return 0;
		}
	}
	return 1;
}

void PK_Sprite_Jarjesta_Taustaspritet(){
	bool lopeta = false;
	int l = 1;
	int vali;

	while (l < MAX_SPRITEJA && lopeta == false)
	{
		lopeta = true;

		for (int i=0;i<MAX_SPRITEJA-1;i++)
		{
			if (taustaspritet[i] == -1 || taustaspritet[i+1] == -1)
				i = MAX_SPRITEJA;
			else
			{
				if (spritet[taustaspritet[i]].tyyppi->pallarx_kerroin > spritet[taustaspritet[i+1]].tyyppi->pallarx_kerroin)
				{
					vali = taustaspritet[i];
					taustaspritet[i]   = taustaspritet[i+1];
					taustaspritet[i+1] = vali;
					lopeta = false;
				}
			}
		}
		l++;
	}
}

int PK_Sprite_Lisaa(PK2Sprite_Prototyyppi &proto, int pelaaja, double x, double y, int emo){
	bool lisatty = false;
	int i = 0;

	while (!lisatty && i < MAX_SPRITEJA)
	{
		if (spritet[i].piilota)
		{
			spritet[i] = PK2Sprite(&proto,pelaaja,false,x,y);

			spritet[i].x = x + 16 + 1;
			spritet[i].y += spritet[i].tyyppi->korkeus/2;
			spritet[i].alku_x = spritet[i].x;
			spritet[i].alku_y = spritet[i].y;

			if (proto.tyyppi == TYYPPI_TAUSTA)
				PK_Sprite_Lisaa_Taustasprite(i);

			if (emo != MAX_SPRITEJA)
			{
				spritet[i].emosprite = emo;
			}
			else
			{
				spritet[i].emosprite = i;
			}

			lisatty = true;
		}
		else
			i++;
	}

	return 0;
}

int PK_Sprite_Lisaa_Bonus(PK2Sprite_Prototyyppi &proto, int pelaaja, double x, double y, int emo){
	bool lisatty = false;
	int i = 0;

	while (!lisatty && i < MAX_SPRITEJA)
	{
		if (spritet[i].piilota)
		{
			spritet[i] = PK2Sprite(&proto,pelaaja,false,x,y);

			spritet[i].x += spritet[i].tyyppi->leveys;
			spritet[i].y += spritet[i].tyyppi->korkeus/2;
			spritet[i].alku_x = spritet[i].x;
			spritet[i].alku_y = spritet[i].y;
			spritet[i].hyppy_ajastin = 1;//-1 - rand()%3;
			//spritet[i].b = rand()%2 - rand()%2;
			spritet[i].a = rand()%2 - rand()%4;
			spritet[i].isku = 35;//25

			if (proto.tyyppi == TYYPPI_TAUSTA)
				PK_Sprite_Lisaa_Taustasprite(i);

			if (emo != MAX_SPRITEJA)
			{
				spritet[i].emosprite = emo;
			}
			else
			{
				spritet[i].emosprite = i;
			}

			lisatty = true;
		}
		else
			i++;
	}

	return 0;
}

int PK_Sprite_Lisaa_Ammus(PK2Sprite_Prototyyppi &proto, int pelaaja, double x, double y, int emo){
	bool lisatty = false;
	int i = 0;

	while (!lisatty && i < MAX_SPRITEJA)
	{
		if (spritet[i].piilota)
		{
			spritet[i] = PK2Sprite(&proto,pelaaja,false,x/*-proto.leveys/2*/,y);

			//spritet[i].x += spritet[i].tyyppi->leveys;
			//spritet[i].y += spritet[i].tyyppi->korkeus/2;

			if (proto.Onko_AI(AI_HEITTOASE))
			{
				if ((int)spritet[emo].a == 0)
				{
					// Jos "ampuja" on pelaaja tai ammuksen nopeus on nolla
					if (spritet[emo].pelaaja == 1 || spritet[i].tyyppi->max_nopeus == 0)
					{
						if (!spritet[emo].flip_x)
							spritet[i].a = spritet[i].tyyppi->max_nopeus;
						else
							spritet[i].a = -spritet[i].tyyppi->max_nopeus;
					}
					else // tai jos kyseess� on vihollinen
					{
						if (!spritet[emo].flip_x)
							spritet[i].a = 1 + rand()%(int)spritet[i].tyyppi->max_nopeus;
						else
							spritet[i].a = -1 - rand()%-(int)spritet[i].tyyppi->max_nopeus;
					}
				}
				else
				{
					if (!spritet[emo].flip_x)
						spritet[i].a = spritet[i].tyyppi->max_nopeus + spritet[emo].a;
					else
						spritet[i].a = -spritet[i].tyyppi->max_nopeus + spritet[emo].a;

					//spritet[i].a = spritet[emo].a * 1.5;

				}

				spritet[i].hyppy_ajastin = 1;
			}
			else
			if (proto.Onko_AI(AI_MUNA))
			{
				spritet[i].y = spritet[emo].y+10;
				spritet[i].a = spritet[emo].a / 1.5;
			}
			else
			{
				if (!spritet[emo].flip_x)
					spritet[i].a = spritet[i].tyyppi->max_nopeus;
				else
					spritet[i].a = -spritet[i].tyyppi->max_nopeus;
			}

			if (emo != MAX_SPRITEJA)
			{
				spritet[i].emosprite = emo;
				spritet[i].vihollinen = spritet[emo].vihollinen;
			}
			else
			{
				spritet[i].emosprite = i;
			}

			if (proto.tyyppi == TYYPPI_TAUSTA)
				PK_Sprite_Lisaa_Taustasprite(i);

			lisatty = true;
		}
		else
			i++;
	}

	return 0;
}

int PK_Sprite_Tyhjenna(){
	int i = 0;

	while (i < MAX_SPRITEJA)
	{
		spritet[i] = PK2Sprite();
		taustaspritet[i] = -1;
		i++;
	}

	return 0;
}

int PK_Kartta_Aseta_Spritet(){
	PK_Sprite_Tyhjenna();

	PK_Sprite_Lisaa(protot[kartta->pelaaja_sprite], 1, 0, 0, MAX_SPRITEJA);

	int sprite;

	for (int x=0;x<PK2KARTTA_KARTTA_LEVEYS;x++)
	{
		for (int y=0;y<PK2KARTTA_KARTTA_KORKEUS;y++)
		{
			sprite = kartta->spritet[x+y*PK2KARTTA_KARTTA_LEVEYS];

			if (sprite != 255 && protot[sprite].korkeus > 0)
			{
				PK_Sprite_Lisaa(protot[sprite],	0, x*32,y*32-protot[sprite].korkeus+32, MAX_SPRITEJA);
			}
		}
	}

	PK_Sprite_Jarjesta_Taustaspritet();

	return 0;
}

int PK_Piirra_Bonukset(){
	int bonus;
	int px = 0,
		py = 0;

	for (int x=0;x<RUUDUN_LEVEYS/32;x++)
	{
		for (int y=0;y<RUUDUN_KORKEUS/32;y++)
		{
			bonus = kartta->spritet[x+kamera_x/32+(y+kamera_y/32)*PK2KARTTA_KARTTA_LEVEYS];

			if (bonus != 255 && protot[bonus].tyyppi == TYYPPI_BONUS && protot[bonus].paino <= 0)
			{
				px = x*32+protot[bonus].leveys/2  -kamera_x%32;
				py = y*32-protot[bonus].korkeus+32-kamera_y%32;
				py += (int)cos_table[(degree+(x)*20)%360] / 3;
				protot[bonus].Piirra(px,py,0);
			}
		}
	}
	return 0;
}

void PK_Esineet_Alusta(){
	for (int i=0;i<MAX_ESINEITA;i++)
		esineet[i] = NULL;
}

int PK_Esineet_Piirra(){
	int x,y;

	y = KARTANPIIRTO_KORKEUS-35;//36
	x = item_paneeli_x + 35;//40

	BYTE v1, v2;

	esineita = 0;

	for (int i=0;i<MAX_ESINEITA;i++)
		if (esineet[i] != NULL)
		{
			esineita++;

			if (i == 0)
			{
				v1 = 31;
				v2 = 16+128;
			}
			else
			{
				v1 = 0;
				v2 = 16;

			}

			if (false)
			{

				PisteDraw2_ScreenFill(	x-esineet[i]->leveys/2+4,
														y-esineet[i]->korkeus/2+4,
														x+esineet[i]->leveys/2+4,
														y+esineet[i]->korkeus/2+4,
														0);

				PisteDraw2_ScreenFill(	x-esineet[i]->leveys/2-1,
														y-esineet[i]->korkeus/2-1,
														x+esineet[i]->leveys/2+1,
														y+esineet[i]->korkeus/2+1,
														v1);

				PisteDraw2_ScreenFill(	x-esineet[i]->leveys/2+1,
														y-esineet[i]->korkeus/2+1,
														x+esineet[i]->leveys/2-1,
														y+esineet[i]->korkeus/2-1,
														v2);

			}


			esineet[i]->Piirra(x-esineet[i]->leveys/2,y-esineet[i]->korkeus/2,0);
			x += 38;
		}


	return 0;
}

bool PK_Esineet_Lisaa(PK2Sprite_Prototyyppi *proto){
	int i=0;
	bool lisatty = false;

	char ilmo[80];
	strcpy(ilmo,tekstit->Hae_Teksti(txt_game_newitem));  //"a new item: ";

	while (i<MAX_ESINEITA && !lisatty)
	{
		if (esineet[i] == NULL)
		{
			lisatty = true;
			esineet[i] = proto;
			//strcat(ilmo,proto->nimi);
			PK_Uusi_Ilmoitus(ilmo);
		}
		i++;
	}
	return lisatty;
}

int PK_Esineet_Kayta(){
	if (esineet[0] != NULL)
		PK_Sprite_Lisaa(*esineet[0],0,spritet[pelaaja_index].x-esineet[0]->leveys,spritet[pelaaja_index].y,MAX_SPRITEJA/*pelaaja_index*/);

	for (int i=0;i<MAX_ESINEITA-1;i++)
		esineet[i] = esineet[i+1];

	esineet[MAX_ESINEITA-1] = NULL;

	return 0;
}

int PK_Esineet_Vaihda(){
	if (esineet[0] == NULL)
		return 0;

	PK2Sprite_Prototyyppi *temp = esineet[0];

	for (int i=0;i<MAX_ESINEITA-1;i++)
		esineet[i] = esineet[i+1];

	int lkm = 0;

	while(lkm < MAX_ESINEITA-1 && esineet[lkm] != NULL)
		lkm++;

	esineet[lkm] = temp;

	return 0;
}

//Sprite Movement

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

//Colision with the block sprite
void PK_Tutki_Seina2(PK2Sprite &sprite, PK2BLOCK &palikka){

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
//Examine blocks
void PK_Tutki_Seina(PK2Sprite &sprite, PK2BLOCK &palikka){
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
			sprite.saatu_vahinko_tyyppi = VAHINKO_TULI;
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
				if (PisteSound_StartMusic("music/hiscore.xm")!=0)
					PK2_virhe = true;
				jakso_lapaisty = true;
				jaksot[jakso_indeksi_nyt].lapaisty = true;
				if (jaksot[jakso_indeksi_nyt].jarjestys >= jakso)
					jakso++; //Increase level
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
			sprite.saatu_vahinko_tyyppi = VAHINKO_TULI;
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
		if (palikka.koodi == BLOCK_LUKKO && sprite.tyyppi->avain){
			kartta->seinat[palikka.vasen/32+(palikka.yla/32)*PK2KARTTA_KARTTA_LEVEYS] = 255;
			PK_Kartta_Laske_Reunat();

			sprite.piilota = true;

			if (sprite.tyyppi->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
			{
				avaimia--;
				if (avaimia < 1)
					PK_Kartta_Avaa_Lukot();
			}

			PK_Tehoste_Rajahdys(palikka.vasen+16, palikka.yla+10, 0);
			PK_Soita_Aani(avaa_lukko_aani,100, (int)sprite_x, (int)sprite_y, AANET_SAMPLERATE, false);
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

	if (sprite_yla < palikka.ala && sprite_ala-1 > palikka.yla)
	{
		if (sprite_oikea+sprite_a-1 > palikka.vasen && sprite_vasen+sprite_a < palikka.oikea)
		{
			// Examine whether the sprite going in the right side of the block.
			if (sprite_oikea+sprite_a < palikka.oikea)
			{
				// Onko palikka sein�?
				if (palikka.oikealle == BLOCK_SEINA)
				{
					oikealle = false;

					if (palikka.koodi == BLOCK_HISSI_HORI)
						sprite_x = palikka.vasen - sprite_leveys/2;
				}
			}
			// Examine whether the sprite going in the left side of the block.
			if (sprite_vasen+sprite_a > palikka.vasen)
			{
				if (palikka.vasemmalle == BLOCK_SEINA)
				{
					vasemmalle = false;

					if (palikka.koodi == BLOCK_HISSI_HORI)
						sprite_x = palikka.oikea + sprite_leveys/2;

				}
			}
		}
	}

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;

	//Examine walls on up and down

	if (sprite_vasen < palikka.oikea && sprite_oikea-1 > palikka.vasen) //Remove the left and right blocks
	{
		if (sprite_ala+sprite_b-1 >= palikka.yla && sprite_yla+sprite_b <= palikka.ala) //Remove the up and down blocks
		{
			if (sprite_ala+sprite_b-1 <= palikka.ala) //Just in the sprite's foot
			{
				if (palikka.alas == BLOCK_SEINA) //If it is a wall
				{
					alas = false;
					if (palikka.koodi == BLOCK_HISSI_VERT)
						sprite_y = palikka.yla - sprite_korkeus /2;

					if (sprite_ala-1 >= palikka.yla && sprite_b >= 0)
					{
						//sprite_y -= sprite_ala - palikka.yla;
						if (palikka.koodi != BLOCK_HISSI_HORI)
						{
							sprite_y = palikka.yla - sprite_korkeus /2;
						}
					}

					if (sprite.kytkinpaino >= 1)
					{
						if (palikka.koodi == BLOCK_KYTKIN1 && kytkin1 == 0)
						{
							kytkin1 = KYTKIN_ALOITUSARVO;
							kytkin_tarina = 64;
							PK_Soita_Aani(kytkin_aani, 100, (int)sprite_x, (int)sprite_y, AANET_SAMPLERATE, false);
						}

						if (palikka.koodi == BLOCK_KYTKIN2 && kytkin2 == 0)
						{
							kytkin2 = KYTKIN_ALOITUSARVO;
							kytkin_tarina = 64;
							PK_Soita_Aani(kytkin_aani, 100, (int)sprite_x, (int)sprite_y, AANET_SAMPLERATE, false);
						}

						if (palikka.koodi == BLOCK_KYTKIN3 && kytkin3 == 0)
						{
							kytkin3 = KYTKIN_ALOITUSARVO;
							kytkin_tarina = 64;
							PK_Soita_Aani(kytkin_aani, 100, (int)sprite_x, (int)sprite_y, AANET_SAMPLERATE, false);
						}

						//if (sprite_b > 3 && sprite.paino >= 2)
						//	kytkin_tarina = 5;
					}

				}
			}

			if (sprite_yla+sprite_b > palikka.yla)
			{
				if (palikka.ylos == BLOCK_SEINA)
				{
					ylos = false;

					if (sprite_yla < palikka.ala)
					{
						if (palikka.koodi == BLOCK_HISSI_VERT && sprite.kyykky)
						{
							sprite.saatu_vahinko = 2;
							sprite.saatu_vahinko_tyyppi = VAHINKO_ISKU;
						}

						if (palikka.koodi != BLOCK_HISSI_HORI)
						{
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

int PK_Sprite_Liikuta(int i){

	PK2Sprite &sprite = spritet[i]; //address of sprite = address of spritet[i] (if change sprite, change spritet[i])

	if (i >= MAX_SPRITEJA || i < 0)
		return -1;

	if (!spritet[i].tyyppi)
		return -1;

	sprite_x = sprite.x;
	sprite_y = sprite.y;
	sprite_a = sprite.a;
	sprite_b = sprite.b;

	sprite_leveys  = sprite.tyyppi->leveys;
	sprite_korkeus = sprite.tyyppi->korkeus;

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;
	sprite_yla	 = sprite_y-sprite_korkeus/2;
	sprite_ala	 = sprite_y+sprite_korkeus/2;

	max_nopeus = (BYTE)sprite.tyyppi->max_nopeus;

	vedessa = sprite.vedessa;

	x = 0;
	y = 0;

	oikealle	 = true,
	vasemmalle	 = true,
	ylos		 = true,
	alas		 = true;

	kartta_vasen = 0;
	kartta_yla   = 0;

	sprite.kyykky = false;

	sprite.reuna_vasemmalla = false;
	sprite.reuna_oikealla = false;


	/* Pistet��n vauhtia tainnutettuihin spriteihin */
	if (sprite.energia < 1/* && sprite.tyyppi->max_nopeus == 0*/)
		max_nopeus = 3;

	// Calculate the remainder of the sprite towards

	if (sprite.hyokkays1 > 0)
		sprite.hyokkays1--;

	if (sprite.hyokkays2 > 0)
		sprite.hyokkays2--;

	if (sprite.lataus > 0)	// aika kahden ampumisen (munimisen) v�lill�
		sprite.lataus --;

	if (sprite.muutos_ajastin > 0)	// aika muutokseen
		sprite.muutos_ajastin --;

	/*****************************************************************************************/
	/* Player-sprite and its controls                                                        */
	/*****************************************************************************************/

	bool lisavauhti = true;
	bool hidastus = false;

	PisteInput_Lue_Eventti();
	if (sprite.pelaaja != 0 && sprite.energia > 0)
	{
		/* WALK */
		if (PisteInput_Lue_Kontrolli(kontrolli_juoksu))
			lisavauhti = false;

		/* ATTACK 1 */
		if (PisteInput_Lue_Kontrolli(kontrolli_hyokkays1) && sprite.lataus == 0 && sprite.ammus1 != -1) {
			sprite.hyokkays1 = sprite.tyyppi->hyokkays1_aika;
		}
		/* ATTACK 2 */
		else {
			if (PisteInput_Lue_Kontrolli(kontrolli_hyokkays2) && sprite.lataus == 0 && sprite.ammus2 != -1)
				sprite.hyokkays2 = sprite.tyyppi->hyokkays2_aika;
		}

		/* CROUCH */
		sprite.kyykky = false;

		if (PisteInput_Lue_Kontrolli(kontrolli_alas) && !sprite.alas)
		{
			sprite.kyykky = true;
			sprite_yla += sprite_korkeus/1.5;
		}

		double a_lisays = 0;

		/* NAVIGATING TO RIGHT */
		if (PisteInput_Lue_Kontrolli(kontrolli_oikealle))
		{
			a_lisays = 0.04;//0.08;

			if (lisavauhti)
			{
				if (rand()%20 == 1 && sprite.animaatio_index == ANIMAATIO_KAVELY) //rand()%30
					PK_Partikkeli_Uusi(PARTIKKELI_POLYPILVI,sprite_x-8,sprite_ala-8,0.25,-0.25,40,0,0);

				a_lisays += 0.09;//0.05
			}

			if (sprite.alas)
				a_lisays /= 1.5;//2.0

			sprite.flip_x = false;
		}

		/* NAVIGATING TO LEFT */
		if (PisteInput_Lue_Kontrolli(kontrolli_vasemmalle))
		{
			a_lisays = -0.04;

			if (lisavauhti)
			{
				if (rand()%20 == 1 && sprite.animaatio_index == ANIMAATIO_KAVELY)
				{
					PK_Partikkeli_Uusi(PARTIKKELI_POLYPILVI,sprite_x-8,sprite_ala-8,-0.25,-0.25,40,0,0);
				}

				a_lisays -= 0.09;
			}

			if (sprite.alas)	// spriten koskettaessa maata kitka vaikuttaa
				a_lisays /= 1.5;//2.0

			sprite.flip_x = true;
		}

		if (sprite.kyykky)	// Slow when couch

			a_lisays /= 10;

		sprite_a += a_lisays;

		/* JUMPING */
		if (sprite.tyyppi->paino > 0)
		{
			if (PisteInput_Lue_Kontrolli(kontrolli_hyppy))
			{
				if (!sprite.kyykky)
				{
					if (sprite.hyppy_ajastin == 0)
						PK_Soita_Aani(hyppy_aani, 100, (int)sprite_x, (int)sprite_y,
									  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

					if (sprite.hyppy_ajastin <= 0)
						sprite.hyppy_ajastin = 1; //10;
				}
			}
			else
			{
				/*
				if (sprite.hyppy_ajastin > 0 && sprite.hyppy_ajastin < 55)
					sprite.hyppy_ajastin = 55; */

				if (sprite.hyppy_ajastin > 0 && sprite.hyppy_ajastin < 45)//40
					sprite.hyppy_ajastin = 55;
			}

			/* tippuminen hiljaa alasp�in */
			if (PisteInput_Lue_Kontrolli(kontrolli_hyppy) && sprite.hyppy_ajastin >= 150/*90+20*/ &&
				sprite.tyyppi->liitokyky)
				hidastus = true;
		}
		/* MOVING UP AND DOWN */
		else{ // if the player sprite-weight is 0 - like birds

			if (PisteInput_Lue_Kontrolli(kontrolli_hyppy))
				sprite_b -= 0.15;

			if (PisteInput_Lue_Kontrolli(kontrolli_alas))
				sprite_b += 0.15;

			sprite.hyppy_ajastin = 0;
		}

		/* AI */
		for (int ai=0;ai < SPRITE_MAX_AI;ai++)
			switch (sprite.tyyppi->AI[ai]){
			case AI_MUUTOS_JOS_ENERGIAA_ALLE_2:	if (sprite.tyyppi->muutos > -1)
													sprite.AI_Muutos_Jos_Energiaa_Alle_2(protot[sprite.tyyppi->muutos]);
												break;
			case AI_MUUTOS_JOS_ENERGIAA_YLI_1:	if (sprite.tyyppi->muutos > -1)
												{
													if (sprite.AI_Muutos_Jos_Energiaa_Yli_1(protot[sprite.tyyppi->muutos])==1)
														PK_Tehosteet(TUHOUTUMINEN_SAVU_HARMAA, (DWORD)sprite.x, (DWORD)sprite.y);
												}
												break;
			case AI_MUUTOS_AJASTIN:				if (sprite.tyyppi->muutos > -1)
													sprite.AI_Muutos_Ajastin(protot[sprite.tyyppi->muutos]);
												break;
			case AI_VAHINGOITTUU_VEDESTA:		sprite.AI_Vahingoittuu_Vedesta();break;

			case AI_MUUTOS_JOS_OSUTTU:			if (sprite.tyyppi->muutos > -1) {
													sprite.AI_Muutos_Jos_Osuttu(protot[sprite.tyyppi->muutos]);
												}
												break;
			default:			break;
			}

		/* It is not acceptable that a player is anything other than the game character */
		if (sprite.tyyppi->tyyppi != TYYPPI_PELIHAHMO)
			sprite.energia = 0;
	}

	/*****************************************************************************************/
	/* Jump                                                                                  */
	/*****************************************************************************************/

	bool hyppy_maximissa = sprite.hyppy_ajastin >= 90;

	// Jos ollaan hyp�tty / ilmassa:
	if (sprite.hyppy_ajastin > 0)
	{
		if (sprite.hyppy_ajastin < 50-sprite.tyyppi->max_hyppy)
			sprite.hyppy_ajastin = 50-sprite.tyyppi->max_hyppy;

		if (sprite.hyppy_ajastin < 10)
			sprite.hyppy_ajastin = 10;

		if (!hyppy_maximissa){
		// sprite_b = (sprite.tyyppi->max_hyppy/2 - sprite.hyppy_ajastin/2)/-2.0;//-4
		   sprite_b = -sin_table[sprite.hyppy_ajastin]/8;//(sprite.tyyppi->max_hyppy/2 - sprite.hyppy_ajastin/2)/-2.5;
			if (sprite_b > sprite.tyyppi->max_hyppy){
				sprite_b = sprite.tyyppi->max_hyppy/10.0;
				sprite.hyppy_ajastin = 90 - sprite.hyppy_ajastin;
			}

		}

		if (sprite.hyppy_ajastin < 180)
			sprite.hyppy_ajastin += 2;
	}

	if (sprite.hyppy_ajastin < 0)
		sprite.hyppy_ajastin++;

	if (sprite_b > 0 && !hyppy_maximissa)
		sprite.hyppy_ajastin = 90;//sprite.tyyppi->max_hyppy*2;

	/*****************************************************************************************/
	/* Hit recovering                                                                        */
	/*****************************************************************************************/

	if (sprite.isku > 0)
		sprite.isku --;

	/*****************************************************************************************/
	/* Gravity effect                                                                        */
	/*****************************************************************************************/

	if (sprite.paino != 0 && (sprite.hyppy_ajastin <= 0 || sprite.hyppy_ajastin >= 45))
		sprite_b += sprite.paino/1.25;// + sprite_b/1.5;

	if (hidastus && sprite_b > 0) // If gliding
		sprite_b /= 1.3;//1.5;//3

	/*****************************************************************************************/
	/* By default, the sprite is not in the water and not hidden                             */
	/*****************************************************************************************/

	sprite.vedessa  = false;
	sprite.piilossa = false;

	/*****************************************************************************************/
	/* Speed limits                                                                          */
	/*****************************************************************************************/

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

	if (sprite.tyyppi->tiletarkistus){ //Find the tiles that the sprite occupies

		palikat_x_lkm = (int)((sprite_leveys) /32)+4; //Number of blocks
		palikat_y_lkm = (int)((sprite_korkeus)/32)+4;

		kartta_vasen = (int)(sprite_vasen)/32;	//Position in tile map
		kartta_yla	 = (int)(sprite_yla)/32;

		for (y=0;y<palikat_y_lkm;y++)
			for (x=0;x<palikat_x_lkm;x++) //For each block, create a array of blocks around the sprite
				palikat[x+(y*palikat_x_lkm)] = PK_Palikka_Tutki(kartta_vasen+x-1,kartta_yla+y-1); //x = 0, y = 0

		/*****************************************************************************************/
		/* Going through the blocks around the sprite.                                           */
		/*****************************************************************************************/

		palikat_lkm = palikat_y_lkm*palikat_x_lkm;
		for (y=0;y<palikat_y_lkm;y++){
			for (x=0;x<palikat_x_lkm;x++) {
				p = x+y*palikat_x_lkm;
				if (p<300 && p>=0)//{
					//if(sprite.pelaaja == 1) printf("%i\n",palikat_lkm);
					PK_Tutki_Seina(sprite, palikat[p]);
				//}
			}
		}
	}
	/*****************************************************************************************/
	/* If the sprite is under water                                                          */
	/*****************************************************************************************/

	if (sprite.vedessa)
	{

		if (!sprite.tyyppi->osaa_uida || sprite.energia < 1)
		{
			/*
			if (sprite_b > 0)
				sprite_b /= 2.0;

			sprite_b -= (1.5-sprite.paino)/10;*/
			sprite_b /= 2.0;
			sprite_a /= 1.05;

			if (sprite.hyppy_ajastin > 0 && sprite.hyppy_ajastin < 90)
				sprite.hyppy_ajastin--;
		}

		if (rand()%80 == 1)
			PK_Partikkeli_Uusi(PARTIKKELI_KIPINA,sprite_x-4,sprite_y,0,-0.5-rand()%2,rand()%30+30,0,32);
	}

	if (vedessa != sprite.vedessa) // Sprite comes in or out from water
	{
		PK_Tehoste_Loiskahdus((int)sprite_x,(int)sprite_y,32);
	}

	/*****************************************************************************************/
	/* Sprite weight                                                                         */
	/*****************************************************************************************/

	sprite.paino = sprite.alkupaino;
	sprite.kytkinpaino = sprite.paino;

	if (sprite.energia < 1 && sprite.paino == 0)
		sprite.paino = 1;

	/*****************************************************************************************/
	/* Sprite collision with other sprites                                                   */
	/*****************************************************************************************/

	int tuhoutuminen;
	double sprite2_yla; // kyykistymiseen liittyv�
	PK2BLOCK spritepalikka;

	PK2Sprite *sprite2;

	for (int sprite_index = 0; sprite_index < MAX_SPRITEJA; sprite_index++)
	{
		sprite2 = &spritet[sprite_index];

		if (sprite_index != i && /*!sprite2->piilota*/sprite2->aktiivinen)
		{
			if (sprite2->kyykky)
				sprite2_yla = sprite2->tyyppi->korkeus / 3;//1.5;
			else
				sprite2_yla = 0;

			if (sprite2->tyyppi->este && sprite.tyyppi->tiletarkistus)
			{

				if (sprite_x-sprite_leveys/2 +sprite_a  <= sprite2->x + sprite2->tyyppi->leveys /2 &&
					sprite_x+sprite_leveys/2 +sprite_a  >= sprite2->x - sprite2->tyyppi->leveys /2 &&
					sprite_y-sprite_korkeus/2+sprite_b <= sprite2->y + sprite2->tyyppi->korkeus/2 &&
					sprite_y+sprite_korkeus/2+sprite_b >= sprite2->y - sprite2->tyyppi->korkeus/2)
				{
					spritepalikka.koodi = 0;
					spritepalikka.ala   = (int)sprite2->y + sprite2->tyyppi->korkeus/2;
					spritepalikka.oikea = (int)sprite2->x + sprite2->tyyppi->leveys/2;
					spritepalikka.vasen = (int)sprite2->x - sprite2->tyyppi->leveys/2;
					spritepalikka.yla   = (int)sprite2->y - sprite2->tyyppi->korkeus/2;

					spritepalikka.vesi  = false;

					//spritepalikka.koodi = BLOCK_HISSI_VERT;
					/*
					PK_Palikka_Este(spritepalikka);

					if (!sprite.tyyppi->este)
					{
						if (!sprite2->tyyppi->este_alas)
							spritepalikka.alas		 = BLOCK_TAUSTA;
						if (!sprite2->tyyppi->este_ylos)
							spritepalikka.ylos		 = BLOCK_TAUSTA;
						if (!sprite2->tyyppi->este_oikealle)
							spritepalikka.oikealle   = BLOCK_TAUSTA;
						if (!sprite2->tyyppi->este_vasemmalle)
							spritepalikka.vasemmalle = BLOCK_TAUSTA;
					}
					*/

					if (sprite2->a > 0)
						spritepalikka.koodi = BLOCK_HISSI_HORI;

					if (sprite2->b > 0)
						spritepalikka.koodi = BLOCK_HISSI_VERT;

					PK_Palikka_Este(spritepalikka); //TODO - Errado
					PK_Tutki_Seina2(sprite, spritepalikka); //Colision sprite and sprite block
				}
			}

			if (sprite_x <= sprite2->x + sprite2->tyyppi->leveys /2 &&
				sprite_x >= sprite2->x - sprite2->tyyppi->leveys /2 &&
				sprite_y/*yla*/ <= sprite2->y + sprite2->tyyppi->korkeus/2 &&
				sprite_y/*ala*/ >= sprite2->y - sprite2->tyyppi->korkeus/2 + sprite2_yla)
			{
				// samanmerkkiset spritet vaihtavat suuntaa t�rm�tess��n
				if (sprite.tyyppi->indeksi == sprite2->tyyppi->indeksi &&
					sprite2->energia > 0/* && sprite.pelaaja == 0*/)
				{
					if (sprite.x < sprite2->x)
						oikealle = false;
					if (sprite.x > sprite2->x)
						vasemmalle = false;
					if (sprite.y < sprite2->y)
						alas = false;
					if (sprite.y > sprite2->y)
						ylos = false;
				}

				if (sprite.tyyppi->Onko_AI(AI_NUOLET_VAIKUTTAVAT)) {

					if (sprite2->tyyppi->Onko_AI(AI_NUOLI_OIKEALLE)) {
						sprite_a = sprite.tyyppi->max_nopeus / 3.5;
						sprite_b = 0;
					}
					else if (sprite2->tyyppi->Onko_AI(AI_NUOLI_VASEMMALLE)) {
						sprite_a = sprite.tyyppi->max_nopeus / -3.5;
						sprite_b = 0;
					}

					if (sprite2->tyyppi->Onko_AI(AI_NUOLI_YLOS)) {
						sprite_b = sprite.tyyppi->max_nopeus / -3.5;
						sprite_a = 0;
					}
					else if (sprite2->tyyppi->Onko_AI(AI_NUOLI_ALAS)) {
						sprite_b = sprite.tyyppi->max_nopeus / 3.5;
						sprite_a = 0;
					}
				}

				/* spritet voivat vaihtaa tietoa pelaajan olinpaikasta */
	/*			if (sprite.pelaaja_x != -1 && sprite2->pelaaja_x == -1)
				{
					sprite2->pelaaja_x = sprite.pelaaja_x + rand()%30 - rand()%30;
					sprite.pelaaja_x = -1;
				} */


				if (sprite.vihollinen != sprite2->vihollinen && sprite.emosprite != sprite_index)
				{
					if (sprite2->tyyppi->tyyppi != TYYPPI_TAUSTA &&
						sprite.tyyppi->tyyppi   != TYYPPI_TAUSTA &&
						sprite2->tyyppi->tyyppi != TYYPPI_TELEPORTTI &&
						sprite2->isku == 0 &&
						sprite.isku == 0 &&
						sprite2->energia > 0 &&
						sprite.energia > 0 &&
						sprite2->saatu_vahinko < 1)
					{

						// Tippuuko toinen sprite p��lle?

						if (sprite2->b > 2 && sprite2->paino >= 0.5 &&
							sprite2->y < sprite_y && !sprite.tyyppi->este &&
							sprite.tyyppi->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
						{
							//sprite.saatu_vahinko = (int)sprite2->paino;//1;
							sprite.saatu_vahinko = (int)(sprite2->paino+sprite2->b/4);
							sprite.saatu_vahinko_tyyppi = VAHINKO_PUDOTUS;
							sprite2->hyppy_ajastin = 1;
						}

						// Is there another sprite damaging

						if (sprite.tyyppi->vahinko > 0 && sprite2->tyyppi->tyyppi != TYYPPI_BONUS)
						{
							sprite2->saatu_vahinko		  = sprite.tyyppi->vahinko;
							sprite2->saatu_vahinko_tyyppi = sprite.tyyppi->vahinko_tyyppi;
							sprite.hyokkays1 = sprite.tyyppi->hyokkays1_aika;

							// Ammukset hajoavat t�rm�yksest�

							if (sprite2->tyyppi->tyyppi == TYYPPI_AMMUS)
							{
								sprite.saatu_vahinko = 1;//sprite2->tyyppi->vahinko;
								sprite.saatu_vahinko_tyyppi = sprite2->tyyppi->vahinko_tyyppi;
							}

							if (sprite.tyyppi->tyyppi == TYYPPI_AMMUS)
							{
								sprite.saatu_vahinko = 1;//sprite2->tyyppi->vahinko;
								sprite.saatu_vahinko_tyyppi = sprite2->tyyppi->vahinko_tyyppi;
							}
						}
					}
				}

				// lis�t��n spriten painoon sit� koskettavan toisen spriten paino
				if (sprite.paino > 0)
					sprite.kytkinpaino += spritet[sprite_index].tyyppi->paino;

			}
		}
	}

	/*****************************************************************************************/
	/* If the sprite has suffered damage                                                     */
	/*****************************************************************************************/

	// jos n�kym�t�n, ei saa damagea kuin tulesta.
	if (nakymattomyys > 0 && sprite.saatu_vahinko != 0 && sprite.saatu_vahinko_tyyppi != VAHINKO_TULI &&
		i == pelaaja_index) {
		sprite.saatu_vahinko = 0;
		sprite.saatu_vahinko_tyyppi = VAHINKO_EI;
	}



	if (sprite.saatu_vahinko != 0 && sprite.energia > 0 && sprite.tyyppi->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
	{
		if (sprite.tyyppi->suojaus != sprite.saatu_vahinko_tyyppi || sprite.tyyppi->suojaus == VAHINKO_EI)
		{
			sprite.energia -= sprite.saatu_vahinko;
			sprite.isku = VAHINKO_AIKA;

			if (sprite.saatu_vahinko_tyyppi == VAHINKO_SAHKO)
				sprite.isku *= 6;

			PK_Soita_Aani(sprite.tyyppi->aanet[AANI_VAHINKO], 100, (int)sprite.x, (int)sprite.y,
						  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

			if (sprite.tyyppi->tuhoutuminen%100 == TUHOUTUMINEN_HOYHENET)
			{
				PK_Tehosteet(TUHOUTUMINEN_HOYHENET, (DWORD)sprite.x, (DWORD)sprite.y);
			}

			if (sprite.tyyppi->tyyppi != TYYPPI_AMMUS)
			{
				PK_Partikkeli_Uusi(PARTIKKELI_TAHTI,sprite_x,sprite_y,-1,-1,60,0.01,128);
				PK_Partikkeli_Uusi(PARTIKKELI_TAHTI,sprite_x,sprite_y, 0,-1,60,0.01,128);
				PK_Partikkeli_Uusi(PARTIKKELI_TAHTI,sprite_x,sprite_y, 1,-1,60,0.01,128);
			}

			if (sprite.Onko_AI(AI_VAIHDA_KALLOT_JOS_OSUTTU))
				PK_Kartta_Vaihda_Kallopalikat();

			if (sprite.Onko_AI(AI_HYOKKAYS_1_JOS_OSUTTU))
			{
				sprite.hyokkays1 = sprite.tyyppi->hyokkays1_aika;
				sprite.lataus = 0;
			}

			if (sprite.Onko_AI(AI_HYOKKAYS_2_JOS_OSUTTU))
			{
				sprite.hyokkays2 = sprite.tyyppi->hyokkays2_aika;
				sprite.lataus = 0;
			}

		}

		sprite.saatu_vahinko = 0;
		sprite.saatu_vahinko_tyyppi = VAHINKO_EI;


		/*****************************************************************************************/
		/* If the sprite is destroyed                                                            */
		/*****************************************************************************************/

		if (sprite.energia < 1)
		{
			tuhoutuminen = sprite.tyyppi->tuhoutuminen;

			if (tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
			{
				if (sprite.tyyppi->bonus > -1 && sprite.tyyppi->bonusten_lkm > 0)
					if (sprite.tyyppi->bonus_aina || rand()%4 == 1)
						for (int bi=0; bi<sprite.tyyppi->bonusten_lkm; bi++)
							PK_Sprite_Lisaa_Bonus(protot[sprite.tyyppi->bonus],0,sprite_x-11+(10-rand()%20),
											  sprite_ala-16-(10+rand()%20), i);

				if (sprite.Onko_AI(AI_VAIHDA_KALLOT_JOS_TYRMATTY) && !sprite.Onko_AI(AI_VAIHDA_KALLOT_JOS_OSUTTU))
					PK_Kartta_Vaihda_Kallopalikat();

				if (tuhoutuminen >= TUHOUTUMINEN_ANIMAATIO)
					tuhoutuminen -= TUHOUTUMINEN_ANIMAATIO;
				else
					sprite.piilota = true;

				PK_Tehosteet(tuhoutuminen, (DWORD)sprite.x, (DWORD)sprite.y);
				PK_Soita_Aani(sprite.tyyppi->aanet[AANI_TUHOUTUMINEN],100, (int)sprite.x, (int)sprite.y,
							  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

				if (sprite.Onko_AI(AI_UUSI_JOS_TUHOUTUU)) {
					PK_Sprite_Lisaa(*sprite.tyyppi,0,sprite.alku_x-sprite.tyyppi->leveys, sprite.alku_y-sprite.tyyppi->korkeus,i);
				}

				if (sprite.tyyppi->tyyppi == TYYPPI_PELIHAHMO && sprite.tyyppi->pisteet != 0)
				{
					char luku[10];
					itoa(sprite.tyyppi->pisteet,luku,10);
					PK_Fadeteksti_Uusi(fontti2,luku,(int)spritet[i].x-8,(int)spritet[i].y-8,80);
					piste_lisays += sprite.tyyppi->pisteet;
				}
			}
			else
				sprite.energia = 1;
		}
	}

	if (sprite.isku == 0)
		sprite.saatu_vahinko_tyyppi = VAHINKO_EI;


	/*****************************************************************************************/
	/* Revisions                                                                             */
	/*****************************************************************************************/

	if (!oikealle)
	{
		if (sprite_a > 0)
			sprite_a = 0;
	}

	if (!vasemmalle)
	{
		if (sprite_a < 0)
			sprite_a = 0;
	}

	if (!ylos)
	{
		if (sprite_b < 0)
			sprite_b = 0;

		if (!hyppy_maximissa)
			sprite.hyppy_ajastin = 95;//sprite.tyyppi->max_hyppy * 2;
	}

	if (!alas)
	{
		if (sprite_b >= 0) //If sprite is falling
		{
			if (sprite.hyppy_ajastin > 0)
			{
				if (sprite.hyppy_ajastin >= 90+10)
				{
					PK_Soita_Aani(tomahdys_aani,30,(int)sprite_x, (int)sprite_y,
				                  int(25050-sprite.paino*3000),true);

					//PK_Partikkeli_Uusi(	PARTIKKELI_POLYPILVI,sprite_x+rand()%5-rand()%5-10,sprite_ala+rand()%3-rand()%3,
					//			  0,-0.2,rand()%50+20,0,0);

					if (rand()%7 == 1) {
						PK_Partikkeli_Uusi(PARTIKKELI_SAVUPILVI,sprite_x+rand()%5-rand()%5-10,sprite_ala+rand()%3-rand()%3,
									  	   0.3,-0.1,450,0,0);
						PK_Partikkeli_Uusi(PARTIKKELI_SAVUPILVI,sprite_x+rand()%5-rand()%5-10,sprite_ala+rand()%3-rand()%3,
									  	   -0.3,-0.1,450,0,0);
					}

					if (sprite.paino > 1)
						jaristys = 34 + int(sprite.paino * 20);
				}

				sprite.hyppy_ajastin = 0;
			}

			sprite_b = 0;
		}

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

	if (sprite.energia > sprite.tyyppi->energia)
		sprite.energia = sprite.tyyppi->energia;

	if (sprite.isku == 0 || sprite.pelaaja == 1)
	{
		sprite_x += sprite_a;
		sprite_y += sprite_b;
	}

	if (i == pelaaja_index || sprite.energia < 1)
	{
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

	sprite.x = sprite_x;
	sprite.y = sprite_y;
	sprite.a = sprite_a;
	sprite.b = sprite_b;

	sprite.oikealle = oikealle;
	sprite.vasemmalle = vasemmalle;
	sprite.alas = alas;
	sprite.ylos = ylos;

	/*
	sprite.paino = sprite.tyyppi->paino;

	if (sprite.energia < 1 && sprite.paino == 0)
		sprite.paino = 1;*/

	if (sprite.hyppy_ajastin < 0)
		sprite.alas = false;

	//sprite.kyykky   = false;

	/*****************************************************************************************/
	/* AI                                                                                    */
	/*****************************************************************************************/

	if (sprite.pelaaja == 0)
	{
		for (int ai=0;ai < SPRITE_MAX_AI; ai++)
			switch (sprite.tyyppi->AI[ai])
			{
			case AI_EI:							ai = SPRITE_MAX_AI; // lopetetaan
												break;
			case AI_KANA:						sprite.AI_Kana();
												break;
			case AI_PIKKUKANA:					sprite.AI_Kana();
												break;
			case AI_SAMMAKKO1:					sprite.AI_Sammakko1();
												break;
			case AI_SAMMAKKO2:					sprite.AI_Sammakko2();
												break;
			case AI_BONUS:						sprite.AI_Bonus();
												break;
			case AI_MUNA:						sprite.AI_Muna();
												break;
			case AI_AMMUS:						sprite.AI_Ammus();
												break;
			case AI_HYPPIJA:					sprite.AI_Hyppija();
												break;
			case AI_PERUS:						sprite.AI_Perus();
												break;
			case AI_NONSTOP:					sprite.AI_NonStop();
												break;
			case AI_KAANTYY_ESTEESTA_HORI:		sprite.AI_Kaantyy_Esteesta_Hori();
												break;
			case AI_KAANTYY_ESTEESTA_VERT:		sprite.AI_Kaantyy_Esteesta_Vert();
												break;
			case AI_VAROO_KUOPPAA:				sprite.AI_Varoo_Kuoppaa();
												break;
			case AI_RANDOM_SUUNNANVAIHTO_HORI:	sprite.AI_Random_Suunnanvaihto_Hori();
												break;
			case AI_RANDOM_KAANTYMINEN:			sprite.AI_Random_Kaantyminen();
												break;
			case AI_RANDOM_HYPPY:				sprite.AI_Random_Hyppy();
												break;
			case AI_SEURAA_PELAAJAA:			if (nakymattomyys == 0)
													sprite.AI_Seuraa_Pelaajaa(spritet[pelaaja_index]);
												break;
			case AI_SEURAA_PELAAJAA_JOS_NAKEE:	if (nakymattomyys == 0)
													sprite.AI_Seuraa_Pelaajaa_Jos_Nakee(spritet[pelaaja_index]);
												break;
			case AI_SEURAA_PELAAJAA_VERT_HORI:	if (nakymattomyys == 0)
													sprite.AI_Seuraa_Pelaajaa_Vert_Hori(spritet[pelaaja_index]);
												break;
			case AI_SEURAA_PELAAJAA_JOS_NAKEE_VERT_HORI:
												if (nakymattomyys == 0)
													sprite.AI_Seuraa_Pelaajaa_Jos_Nakee_Vert_Hori(spritet[pelaaja_index]);
												break;
			case AI_PAKENEE_PELAAJAA_JOS_NAKEE:	if (nakymattomyys == 0)
													sprite.AI_Pakenee_Pelaajaa_Jos_Nakee(spritet[pelaaja_index]);
												break;
			case AI_POMMI:						sprite.AI_Pommi();
												break;
			case AI_HYOKKAYS_1_JOS_OSUTTU:		sprite.AI_Hyokkays_1_Jos_Osuttu();
												break;
			case AI_HYOKKAYS_2_JOS_OSUTTU:		sprite.AI_Hyokkays_2_Jos_Osuttu();
												break;
			case AI_HYOKKAYS_1_NONSTOP:			sprite.AI_Hyokkays_1_Nonstop();
												break;
			case AI_HYOKKAYS_2_NONSTOP:			sprite.AI_Hyokkays_2_Nonstop();
												break;
			case AI_HYOKKAYS_1_JOS_PELAAJA_EDESSA:
												if (nakymattomyys == 0)
													sprite.AI_Hyokkays_1_Jos_Pelaaja_Edessa(spritet[pelaaja_index]);
												break;
			case AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA:
												if (nakymattomyys == 0)
													sprite.AI_Hyokkays_2_Jos_Pelaaja_Edessa(spritet[pelaaja_index]);
												break;
			case AI_HYOKKAYS_1_JOS_PELAAJA_ALAPUOLELLA:
												if (nakymattomyys == 0)
													sprite.AI_Hyokkays_1_Jos_Pelaaja_Alapuolella(spritet[pelaaja_index]);
												break;
			case AI_HYPPY_JOS_PELAAJA_YLAPUOLELLA:
												if (nakymattomyys == 0)
													sprite.AI_Hyppy_Jos_Pelaaja_Ylapuolella(spritet[pelaaja_index]);
												break;
			case AI_VAHINGOITTUU_VEDESTA:		sprite.AI_Vahingoittuu_Vedesta();
												break;
			case AI_TAPA_KAIKKI:				sprite.AI_Tapa_Kaikki();
												break;
			case AI_KITKA_VAIKUTTAA:			sprite.AI_Kitka_Vaikuttaa();
												break;
			case AI_PIILOUTUU:					sprite.AI_Piiloutuu();
												break;
			case AI_PALAA_ALKUUN_X:				sprite.AI_Palaa_Alkuun_X();
												break;
			case AI_PALAA_ALKUUN_Y:				sprite.AI_Palaa_Alkuun_Y();
												break;
			case AI_LIIKKUU_X_COS:				sprite.AI_Liikkuu_X(cos_table[degree%360]);
												break;
			case AI_LIIKKUU_Y_COS:				sprite.AI_Liikkuu_Y(cos_table[degree%360]);
												break;
			case AI_LIIKKUU_X_SIN:				sprite.AI_Liikkuu_X(sin_table[degree%360]);
												break;
			case AI_LIIKKUU_Y_SIN:				sprite.AI_Liikkuu_Y(sin_table[degree%360]);
												break;
			case AI_LIIKKUU_X_COS_NOPEA:		sprite.AI_Liikkuu_X(cos_table[(degree*2)%360]);
												break;
			case AI_LIIKKUU_Y_SIN_NOPEA:		sprite.AI_Liikkuu_Y(sin_table[(degree*2)%360]);
												break;
			case AI_LIIKKUU_X_COS_HIDAS:		sprite.AI_Liikkuu_X(cos_table[(degree/2)%360]);
												break;
			case AI_LIIKKUU_Y_SIN_HIDAS:		sprite.AI_Liikkuu_Y(sin_table[(degree/2)%360]);
												break;
			case AI_LIIKKUU_Y_SIN_VAPAA:		sprite.AI_Liikkuu_Y(sin_table[(sprite.ajastin/2)%360]);
												break;
			case AI_MUUTOS_JOS_ENERGIAA_ALLE_2:	if (sprite.tyyppi->muutos > -1)
													sprite.AI_Muutos_Jos_Energiaa_Alle_2(protot[sprite.tyyppi->muutos]);
												break;
			case AI_MUUTOS_JOS_ENERGIAA_YLI_1:	if (sprite.tyyppi->muutos > -1)
													if (sprite.AI_Muutos_Jos_Energiaa_Yli_1(protot[sprite.tyyppi->muutos])==1)
														PK_Tehosteet(TUHOUTUMINEN_SAVU_HARMAA, (DWORD)sprite.x, (DWORD)sprite.y);
												break;
			case AI_MUUTOS_AJASTIN:				if (sprite.tyyppi->muutos > -1) {
													sprite.AI_Muutos_Ajastin(protot[sprite.tyyppi->muutos]);
												}
												break;
			case AI_MUUTOS_JOS_OSUTTU:			if (sprite.tyyppi->muutos > -1) {
													sprite.AI_Muutos_Jos_Osuttu(protot[sprite.tyyppi->muutos]);
												}
												break;
			case AI_TELEPORTTI:					if (sprite.AI_Teleportti(i, spritet, MAX_SPRITEJA, spritet[pelaaja_index])==1)
												{

													kamera_x = (int)spritet[pelaaja_index].x;
													kamera_y = (int)spritet[pelaaja_index].y;
													dkamera_x = kamera_x-KARTANPIIRTO_LEVEYS/2;
													dkamera_y = kamera_y-KARTANPIIRTO_KORKEUS/2;
													PisteDraw2_FadeIn(PD_FADE_NORMAL);
													if (sprite.tyyppi->aanet[AANI_HYOKKAYS2] != -1)
														PK_Soita_Aani_Menu(sprite.tyyppi->aanet[AANI_HYOKKAYS2], 100);
														//PK_Soita_Aani(, 100, kamera_x, kamera_y, AANET_SAMPLERATE, false);


												}
												break;
			case AI_KIIPEILIJA:					sprite.AI_Kiipeilija();
												break;
			case AI_KIIPEILIJA2:				sprite.AI_Kiipeilija2();
												break;
			case AI_TUHOUTUU_JOS_EMO_TUHOUTUU:	sprite.AI_Tuhoutuu_Jos_Emo_Tuhoutuu(spritet);
												break;

			case AI_TIPPUU_TARINASTA:			sprite.AI_Tippuu_Tarinasta(jaristys + kytkin_tarina);
												break;
			case AI_LIIKKUU_ALAS_JOS_KYTKIN1_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin1,0,1);
												break;
			case AI_LIIKKUU_YLOS_JOS_KYTKIN1_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin1,0,-1);
												break;
			case AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN1_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin1,-1,0);
												break;
			case AI_LIIKKUU_OIKEALLE_JOS_KYTKIN1_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin1,1,0);
												break;
			case AI_LIIKKUU_ALAS_JOS_KYTKIN2_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin2,0,1);
												break;
			case AI_LIIKKUU_YLOS_JOS_KYTKIN2_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin2,0,-1);
												break;
			case AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN2_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin2,-1,0);
												break;
			case AI_LIIKKUU_OIKEALLE_JOS_KYTKIN2_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin2,1,0);
												break;
			case AI_LIIKKUU_ALAS_JOS_KYTKIN3_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin3,0,1);
												break;
			case AI_LIIKKUU_YLOS_JOS_KYTKIN3_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin3,0,-1);
												break;
			case AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN3_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin3,-1,0);
												break;
			case AI_LIIKKUU_OIKEALLE_JOS_KYTKIN3_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin3,1,0);
												break;
			case AI_TIPPUU_JOS_KYTKIN1_PAINETTU: sprite.AI_Tippuu_Jos_Kytkin_Painettu(kytkin1);
												break;
			case AI_TIPPUU_JOS_KYTKIN2_PAINETTU: sprite.AI_Tippuu_Jos_Kytkin_Painettu(kytkin2);
												break;
			case AI_TIPPUU_JOS_KYTKIN3_PAINETTU: sprite.AI_Tippuu_Jos_Kytkin_Painettu(kytkin3);
												break;
			case AI_RANDOM_LIIKAHDUS_VERT_HORI:	sprite.AI_Random_Liikahdus_Vert_Hori();
												break;
			case AI_KAANTYY_JOS_OSUTTU:			sprite.AI_Kaantyy_Jos_Osuttu();
												break;
			case AI_EVIL_ONE:					if (sprite.energia < 1) musiikin_voimakkuus = 0;
												break;

			case AI_INFO1:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info01));break;
			case AI_INFO2:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info02));break;
			case AI_INFO3:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info03));break;
			case AI_INFO4:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info04));break;
			case AI_INFO5:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info05));break;
			case AI_INFO6:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info06));break;
			case AI_INFO7:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info07));break;
			case AI_INFO8:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info08));break;
			case AI_INFO9:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info09));break;
			case AI_INFO10:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info10));break;
			case AI_INFO11:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info11));break;
			case AI_INFO12:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info12));break;
			case AI_INFO13:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info13));break;
			case AI_INFO14:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info14));break;
			case AI_INFO15:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info15));break;
			case AI_INFO16:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info16));break;
			case AI_INFO17:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info17));break;
			case AI_INFO18:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info18));break;
			case AI_INFO19:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info19));break;

			default:							break;
			}
	}

	//if (kaiku == 1 && sprite.tyyppi->tyyppi == TYYPPI_AMMUS && sprite.tyyppi->vahinko_tyyppi == VAHINKO_MELU &&
	//	sprite.tyyppi->aanet[AANI_HYOKKAYS1] > -1)
	//	PK_Soita_Aani(sprite.tyyppi->aanet[AANI_HYOKKAYS1],20, (int)sprite_x, (int)sprite_y,
	//				  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);


	/*****************************************************************************************/
	/* Set game area to sprite                                                               */
	/*****************************************************************************************/

	if (sprite.x < 0)
		sprite.x = 0;

	if (sprite.y < -sprite_korkeus)
		sprite.y = -sprite_korkeus;

	if (sprite.x > PK2KARTTA_KARTTA_LEVEYS*32)
		sprite.x = PK2KARTTA_KARTTA_LEVEYS*32;

	// If the sprite falls under the lower edge of the map
	if (sprite.y > PK2KARTTA_KARTTA_KORKEUS*32+sprite_korkeus)
	{
		sprite.y = PK2KARTTA_KARTTA_KORKEUS*32+sprite_korkeus;
		sprite.energia = 0;
		sprite.piilota = true;

		if (sprite.kytkinpaino >= 1)
			jaristys = 50;
	}

	if (sprite.a > max_nopeus)
		sprite.a = max_nopeus;

	if (sprite.a < -max_nopeus)
		sprite.a = -max_nopeus;


	/*****************************************************************************************/
	/* Attacks 1 and 2                                                                       */
	/*****************************************************************************************/

	// If the sprite is ready and isn't crouching
	if (sprite.lataus == 0 && !sprite.kyykky)
	{
		// hy�kk�ysaika on "tapissa" mik� tarkoittaa sit�, ett� aloitetaan hy�kk�ys
		if (sprite.hyokkays1 == sprite.tyyppi->hyokkays1_aika)
		{
			// provides recovery time, after which the sprite can attack again
			sprite.lataus = sprite.tyyppi->latausaika;
			if(sprite.lataus == 0) sprite.lataus = 5;
			// jos spritelle ei ole m��ritelty omaa latausaikaa ...
			if (sprite.ammus1 > -1 && sprite.tyyppi->latausaika == 0)
			// ... ja ammukseen on, otetaan latausaika ammuksesta
				if (protot[sprite.ammus1].tulitauko > 0)
					sprite.lataus = protot[sprite.ammus1].tulitauko;

			// soitetaan hy�kk�ys��ni
			PK_Soita_Aani(sprite.tyyppi->aanet[AANI_HYOKKAYS1],100, (int)sprite_x, (int)sprite_y,
						  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

			if (sprite.ammus1 > -1)
			{
				PK_Sprite_Lisaa_Ammus(protot[sprite.ammus1],0,sprite_x, sprite_y, i);

		//		if (protot[sprite.ammus1].aanet[AANI_HYOKKAYS1] > -1)
		//			PK_Soita_Aani(protot[sprite.ammus1].aanet[AANI_HYOKKAYS1],100, (int)sprite_x, (int)sprite_y,
		//						  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);
			}
		}

		// Sama kuin hy�kk�ys 1:ss�
		if (sprite.hyokkays2 == sprite.tyyppi->hyokkays2_aika)
		{
			sprite.lataus = sprite.tyyppi->latausaika;
			if(sprite.lataus == 0) sprite.lataus = 5;
			if (sprite.ammus2 > -1  && sprite.tyyppi->latausaika == 0)
				if (protot[sprite.ammus2].tulitauko > 0)
					sprite.lataus = protot[sprite.ammus2].tulitauko;

			PK_Soita_Aani(sprite.tyyppi->aanet[AANI_HYOKKAYS2],100,(int)sprite_x, (int)sprite_y,
						  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

			if (sprite.ammus2 > -1)
			{
				PK_Sprite_Lisaa_Ammus(protot[sprite.ammus2],0,sprite_x, sprite_y, i);

		//		if (protot[sprite.ammus2].aanet[AANI_HYOKKAYS1] > -1)
		//			PK_Soita_Aani(protot[sprite.ammus2].aanet[AANI_HYOKKAYS1],100, (int)sprite_x, (int)sprite_y,
		//						  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

			}
		}
	}

	// Random sounds
	if (sprite.tyyppi->aanet[AANI_RANDOM] != -1 && rand()%200 == 1 && sprite.energia > 0)
		PK_Soita_Aani(sprite.tyyppi->aanet[AANI_RANDOM],80,(int)sprite_x, (int)sprite_y,
					  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);


	// KEHITYSVAIHEEN APUTOIMINTOJA

	BYTE color;
	DWORD plk;

	if (PisteInput_Keydown(PI_B))//PisteInput_Keydown(PI_B) && huijaukset)
	{

		if (i == pelaaja_index)
		{

			char lukua[50];
			itoa(palikat[1].yla,lukua,10);
			//gcvt(sprite_a,7,lukua);
			PisteDraw2_Font_Write(fontti1,lukua,310,50);

		}

		if (sprite.tyyppi->tiletarkistus)
			for (x=0;x<palikat_x_lkm;x++)
			{
				for (y=0;y<palikat_y_lkm;y++)
				{
					color = 50-x*2-y*2;
					plk = x+y*palikat_x_lkm;

					if (plk > 299)
						plk = 299;

					if (plk < 0)
						plk = 0;

					if (!palikat[plk].tausta)
						color += 32;

					PisteDraw2_ScreenFill(
											palikat[plk].vasen-kamera_x,
											palikat[plk].yla-kamera_y,
											palikat[plk].oikea-kamera_x,
											palikat[plk].ala-kamera_y,
											color);
				}
			}

		PisteDraw2_ScreenFill(
								(int)(sprite_vasen-kamera_x),
								(int)(sprite_yla-kamera_y),
								(int)(sprite_oikea-kamera_x),
								(int)(sprite_ala-kamera_y),
								30);

	}



	return 0;
}

int PK_Sprite_Liikuta_Bonus(int i){
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

	PK2Sprite &sprite = spritet[i];

	sprite_x = sprite.x;
	sprite_y = sprite.y;
	sprite_a = sprite.a;
	sprite_b = sprite.b;

	sprite_leveys  = sprite.tyyppi->leveys;
	sprite_korkeus = sprite.tyyppi->korkeus;

	x = 0;
	y = 0;
	oikealle	= true,
	vasemmalle	= true,
	ylos		= true,
	alas		= true;

	vedessa = sprite.vedessa;

	max_nopeus = (int)sprite.tyyppi->max_nopeus;

	// Siirret��n varsinaiset muuttujat apumuuttujiin.

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;
	sprite_yla	 = sprite_y-sprite_korkeus/2;
	sprite_ala	 = sprite_y+sprite_korkeus/2;


	if (sprite.isku > 0)
		sprite.isku--;

	if (sprite.lataus > 0)
		sprite.lataus--;

	if (sprite.muutos_ajastin > 0)	// aika muutokseen
		sprite.muutos_ajastin --;

	// Hyppyyn liittyv�t seikat

	if (kytkin_tarina + jaristys > 0 && sprite.hyppy_ajastin == 0)
		sprite.hyppy_ajastin = sprite.tyyppi->max_hyppy / 2;

	if (sprite.hyppy_ajastin > 0 && sprite.hyppy_ajastin < sprite.tyyppi->max_hyppy)
	{
		sprite.hyppy_ajastin ++;
		sprite_b = (sprite.tyyppi->max_hyppy - sprite.hyppy_ajastin)/-4.0;//-2
	}

	if (sprite_b > 0)
		sprite.hyppy_ajastin = sprite.tyyppi->max_hyppy;



	if (sprite.paino != 0)	// jos bonuksella on paino, tutkitaan ymp�rist�
	{
		// o
		//
		// |  Gravity
		// V

		sprite_b += sprite.paino + sprite_b/1.25;

		if (sprite.vedessa)
		{
			if (sprite_b > 0)
				sprite_b /= 2.0;

			if (rand()%80 == 1)
				PK_Partikkeli_Uusi(PARTIKKELI_KIPINA,sprite_x-4,sprite_y,0,-0.5-rand()%2,rand()%30+30,0,32);
		}

		sprite.vedessa = false;

		sprite.kytkinpaino = sprite.paino;

		/* TOISET SPRITET */

		PK2BLOCK spritepalikka;

		for (int sprite_index = 0; sprite_index < MAX_SPRITEJA; sprite_index++)
		{
			if (sprite_index != i && !spritet[sprite_index].piilota)
			{
				if (spritet[sprite_index].tyyppi->este && sprite.tyyppi->tiletarkistus)
				{
					if (sprite_x-sprite_leveys/2 +sprite_a <= spritet[sprite_index].x + spritet[sprite_index].tyyppi->leveys /2 &&
						sprite_x+sprite_leveys/2 +sprite_a >= spritet[sprite_index].x - spritet[sprite_index].tyyppi->leveys /2 &&
						sprite_y-sprite_korkeus/2+sprite_b <= spritet[sprite_index].y + spritet[sprite_index].tyyppi->korkeus/2 &&
						sprite_y+sprite_korkeus/2+sprite_b >= spritet[sprite_index].y - spritet[sprite_index].tyyppi->korkeus/2)
					{
						spritepalikka.koodi = 0;
						spritepalikka.ala   = (int)spritet[sprite_index].y + spritet[sprite_index].tyyppi->korkeus/2;
						spritepalikka.oikea = (int)spritet[sprite_index].x + spritet[sprite_index].tyyppi->leveys/2;
						spritepalikka.vasen = (int)spritet[sprite_index].x - spritet[sprite_index].tyyppi->leveys/2;
						spritepalikka.yla   = (int)spritet[sprite_index].y - spritet[sprite_index].tyyppi->korkeus/2;

						spritepalikka.alas       = BLOCK_SEINA;
						spritepalikka.ylos       = BLOCK_SEINA;
						spritepalikka.oikealle   = BLOCK_SEINA;
						spritepalikka.vasemmalle = BLOCK_SEINA;

						if (!spritet[sprite_index].tyyppi->este_alas)
							spritepalikka.alas		 = BLOCK_TAUSTA;
						if (!spritet[sprite_index].tyyppi->este_ylos)
							spritepalikka.ylos		 = BLOCK_TAUSTA;
						if (!spritet[sprite_index].tyyppi->este_oikealle)
							spritepalikka.oikealle   = BLOCK_TAUSTA;
						if (!spritet[sprite_index].tyyppi->este_vasemmalle)
							spritepalikka.vasemmalle = BLOCK_TAUSTA;


						spritepalikka.vesi  = false;

						PK_Palikka_Este(spritepalikka);

						PK_Tutki_Seina2(sprite, spritepalikka); //Colision bonus and sprite block
					}
				}

				if (sprite_x < spritet[sprite_index].x + spritet[sprite_index].tyyppi->leveys/2 &&
					sprite_x > spritet[sprite_index].x - spritet[sprite_index].tyyppi->leveys/2 &&
					sprite_y < spritet[sprite_index].y + spritet[sprite_index].tyyppi->korkeus/2 &&
					sprite_y > spritet[sprite_index].y - spritet[sprite_index].tyyppi->korkeus/2 &&
					sprite.isku == 0)
				{
					if (spritet[sprite_index].tyyppi->tyyppi != TYYPPI_BONUS &&
						!(sprite_index == pelaaja_index && sprite.tyyppi->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU))
						sprite_a += spritet[sprite_index].a*(rand()%4);

					// lis�t��n spriten painoon sit� koskettavan toisen spriten paino
					sprite.kytkinpaino += spritet[sprite_index].tyyppi->paino;

					// samanmerkkiset spritet vaihtavat suuntaa t�rm�tess��n
					if (sprite.tyyppi->indeksi == spritet[sprite_index].tyyppi->indeksi &&
						spritet[sprite_index].energia > 0)
					{
						if (sprite.x < spritet[sprite_index].x)
						{
							spritet[sprite_index].a += sprite.a / 3.0;
							oikealle = false;
						}
						if (sprite.x > spritet[sprite_index].x)
						{
							spritet[sprite_index].a += sprite.a / 3.0;
							vasemmalle = false;
						}
						/*
						if (sprite.y < spritet[sprite_index].y)
							alas = false;
						if (sprite.y > spritet[sprite_index].y)
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

		if (sprite.tyyppi->tiletarkistus)
		{

			palikat_x_lkm = (int)((sprite_leveys) /32)+4;
			palikat_y_lkm = (int)((sprite_korkeus)/32)+4;

			kartta_vasen = (int)(sprite_vasen)/32;
			kartta_yla	 = (int)(sprite_yla)/32;

			for (y=0;y<palikat_y_lkm;y++)
				for (x=0;x<palikat_x_lkm;x++)
				{
					palikat[x+y*palikat_x_lkm] = PK_Palikka_Tutki(kartta_vasen+x-1,kartta_yla+y-1);
				}

			// Tutkitaan t�rm��k� palikkaan

			for (y=0;y<palikat_y_lkm;y++)
				for (x=0;x<palikat_x_lkm;x++)
					PK_Tutki_Seina(sprite, palikat[x+y*palikat_x_lkm]);
			/*
			PK_Tutki_Seina_Debug(sprite, palikat[x+y*palikat_x_lkm],
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

		if (vedessa != sprite.vedessa)
			PK_Tehoste_Loiskahdus((int)sprite_x,(int)sprite_y,32);


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

			sprite.hyppy_ajastin = sprite.tyyppi->max_hyppy;
		}

		if (!alas)
		{
			if (sprite_b >= 0)
			{
				if (sprite.hyppy_ajastin > 0)
				{
					sprite.hyppy_ajastin = 0;
				//	if (/*sprite_b == 4*/!maassa)
				//		PK_Soita_Aani(tomahdys_aani,20,(int)sprite_x, (int)sprite_y,
				//				      int(25050-sprite.tyyppi->paino*4000),true);
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

		sprite.x = sprite_x;
		sprite.y = sprite_y;
		sprite.a = sprite_a;
		sprite.b = sprite_b;

		sprite.oikealle = oikealle;
		sprite.vasemmalle = vasemmalle;
		sprite.alas = alas;
		sprite.ylos = ylos;
	}
	else	// jos spriten paino on nolla, tehd��n spritest� "kelluva"
	{
		sprite.y = sprite.alku_y + cos_table[int(degree+(sprite.alku_x+sprite.alku_y))%360] / 3.0;
		sprite_y = sprite.y;
	}

	sprite.paino = sprite.alkupaino;

	int tuhoutuminen;

	// Test if player touches bonus
	if (sprite_x < spritet[pelaaja_index].x + spritet[pelaaja_index].tyyppi->leveys/2 &&
		sprite_x > spritet[pelaaja_index].x - spritet[pelaaja_index].tyyppi->leveys/2 &&
		sprite_y < spritet[pelaaja_index].y + spritet[pelaaja_index].tyyppi->korkeus/2 &&
		sprite_y > spritet[pelaaja_index].y - spritet[pelaaja_index].tyyppi->korkeus/2 &&
		sprite.isku == 0)
	{
		if (sprite.energia > 0 && spritet[pelaaja_index].energia > 0)
		{
			if (sprite.tyyppi->pisteet != 0)
			{
				piste_lisays += sprite.tyyppi->pisteet;
				char luku[6];
				itoa(sprite.tyyppi->pisteet,luku,10);
				if (sprite.tyyppi->pisteet >= 50)
					PK_Fadeteksti_Uusi(fontti2,luku,(int)sprite.x-8,(int)sprite.y-8,100);
				else
					PK_Fadeteksti_Uusi(fontti1,luku,(int)sprite.x-8,(int)sprite.y-8,100);

			}

			if (sprite.Onko_AI(AI_BONUS_AIKA))
				aika += sprite.tyyppi->latausaika;

			if (sprite.Onko_AI(AI_BONUS_NAKYMATTOMYYS))
				nakymattomyys = sprite.tyyppi->latausaika;

			//kartta->spritet[(int)(sprite.alku_x/32) + (int)(sprite.alku_y/32)*PK2KARTTA_KARTTA_LEVEYS] = 255;

			if (sprite.tyyppi->vahinko != 0 && sprite.tyyppi->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
				spritet[pelaaja_index].energia -= sprite.tyyppi->vahinko;

			tuhoutuminen = sprite.tyyppi->tuhoutuminen;

			if (tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
			{
				if (tuhoutuminen >= TUHOUTUMINEN_ANIMAATIO)
					tuhoutuminen -= TUHOUTUMINEN_ANIMAATIO;
				else
				{
					if (sprite.tyyppi->avain)
					{
						avaimia--;

						if (avaimia < 1)
							PK_Kartta_Avaa_Lukot();
					}

					sprite.piilota = true;
				}

				if (sprite.Onko_AI(AI_UUSI_JOS_TUHOUTUU)) {
					double ax, ay;
					ax = sprite.alku_x;//-sprite.tyyppi->leveys;
					ay = sprite.alku_y-sprite.tyyppi->korkeus/2.0;
					PK_Sprite_Lisaa(*sprite.tyyppi,0,ax-17, ay,i);
					for (int r=1;r<6;r++)
						PK_Partikkeli_Uusi(PARTIKKELI_KIPINA,ax+rand()%10-rand()%10, ay+rand()%10-rand()%10,0,0,rand()%100,0.1,32);

				}

				if (sprite.tyyppi->bonus  != -1)
					PK_Esineet_Lisaa(&protot[sprite.tyyppi->bonus]);

				if (sprite.tyyppi->muutos != -1)
				{
					if (protot[sprite.tyyppi->muutos].AI[0] != AI_BONUS)
					{
						spritet[pelaaja_index].tyyppi = &protot[sprite.tyyppi->muutos];
						spritet[pelaaja_index].ammus1 = spritet[pelaaja_index].tyyppi->ammus1;
						spritet[pelaaja_index].ammus2 = spritet[pelaaja_index].tyyppi->ammus2;
						spritet[pelaaja_index].alkupaino = spritet[pelaaja_index].tyyppi->paino;
						spritet[pelaaja_index].y -= spritet[pelaaja_index].tyyppi->korkeus/2;
						//PK_Uusi_Ilmoitus("pekka has been transformed!");
					}
				}

				if (sprite.tyyppi->ammus1 != -1)
				{
					spritet[pelaaja_index].ammus1 = sprite.tyyppi->ammus1;
					PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_game_newegg));
				}

				if (sprite.tyyppi->ammus2 != -1)
				{
					spritet[pelaaja_index].ammus2 = sprite.tyyppi->ammus2;
					PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_game_newdoodle));
				}

				PK_Soita_Aani(sprite.tyyppi->aanet[AANI_TUHOUTUMINEN],100, (int)sprite.x, (int)sprite.y,
							  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

				PK_Tehosteet(tuhoutuminen, (DWORD)sprite_x, (DWORD)sprite_y);
			}
		}
	}

	for (i=0;i<SPRITE_MAX_AI;i++)
	{
		if (sprite.tyyppi->AI[i] == AI_EI)
			break;

		switch (sprite.tyyppi->AI[i])
		{
		case AI_BONUS:				sprite.AI_Bonus();break;

		case AI_PERUS:				sprite.AI_Perus();break;

		case AI_MUUTOS_AJASTIN:		if (sprite.tyyppi->muutos > -1)
									sprite.AI_Muutos_Ajastin(protot[sprite.tyyppi->muutos]);
									break;

		case AI_TIPPUU_TARINASTA:	sprite.AI_Tippuu_Tarinasta(jaristys + kytkin_tarina);
									break;

		default:					break;
		}
	}

	/* Ei k�y p�ins�, ett� pelaaja on bonusesine */
	if (sprite.pelaaja != 0)
		sprite.energia = 0;

	return 0;
}
//If the screen changes
int PK_Alusta_Tilat(){
	if (pelin_seuraava_tila != pelin_tila){
		if (pelin_seuraava_tila == TILA_PERUSALUSTUS){
			strcpy(pelaajan_nimi,tekstit->Hae_Teksti(txt_player_default_name));
			srand((unsigned)time(NULL));
			strcpy(viesti,"no message");
			strcpy(episodi,"");
			strcpy(seuraava_kartta,"untitle1.map");

			jakso = 1;

			if (!taulut_laskettu){
				//PisteLog_Kirjoita("  - Precalculating... \n");
				PK_Laske_Taulut();

				PK2Kartta_Cos_Sin(cos_table, sin_table);
				taulut_laskettu = true;
			}

			PK_Alusta_Fadetekstit();

			if (KARTANPIIRTO_LEVEYS > RUUDUN_LEVEYS)
				KARTANPIIRTO_LEVEYS = RUUDUN_LEVEYS;

			if (KARTANPIIRTO_KORKEUS > RUUDUN_KORKEUS)
				KARTANPIIRTO_KORKEUS = RUUDUN_KORKEUS;

			if (!RAJAA_KARTANPIIRTOALUE){

				KARTANPIIRTO_LEVEYS = RUUDUN_LEVEYS;
				KARTANPIIRTO_KORKEUS = RUUDUN_KORKEUS;
			}

			PK2Kartta_Aseta_Ruudun_Mitat(KARTANPIIRTO_LEVEYS,KARTANPIIRTO_KORKEUS);

			kartta = new PK2Kartta();


			PisteDraw2_Image_Delete(kuva_peli); //Delete if there is a image allocated
			kuva_peli = PisteDraw2_Image_Load("gfx/pk2stuff.bmp", false);

			PisteDraw2_Image_Delete(kuva_peli_sysmem);
			kuva_peli_sysmem = PisteDraw2_Image_Load("gfx/pk2stuff.bmp", false);

			int ind_font = 0,
				ind_path = 0;

			ind_path = tekstit->Hae_Indeksi("font path");

			ind_font = tekstit->Hae_Indeksi("font small font");
			if (ind_path == -1 || ind_font == -1) {
				if ((fontti1 = PisteDraw2_Font_Create("language/fonts/","ScandicSmall.txt")) == -1){
					PK2_virhe = true;
				}
			}
			else {
				if ((fontti1 = PisteDraw2_Font_Create(tekstit->Hae_Teksti(ind_path),tekstit->Hae_Teksti(ind_font))) == -1){
					PK2_virhe = true;
				}
			}

			ind_font = tekstit->Hae_Indeksi("font big font normal");
			if (ind_path == -1 || ind_font == -1) {
				if ((fontti2 = PisteDraw2_Font_Create("language/fonts/","ScandicBig1.txt")) == -1){
				PK2_virhe = true;
				}
			}
			else {
				if ((fontti2 = PisteDraw2_Font_Create(tekstit->Hae_Teksti(ind_path),tekstit->Hae_Teksti(ind_font))) == -1){
				PK2_virhe = true;
				//PisteLog_Kirjoita("    - Loading font ");
				//PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_path));
				//PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_font));
				//PisteLog_Kirjoita(" failed!\n");
				}
			}

			ind_font = tekstit->Hae_Indeksi("font big font hilite");
			if (ind_path == -1 || ind_font == -1) {
				if ((fontti3 = PisteDraw2_Font_Create("language/fonts/","ScandicBig2.txt")) == -1){
				PK2_virhe = true;
				}
			}
			else {
				if ((fontti3 = PisteDraw2_Font_Create(tekstit->Hae_Teksti(ind_path),tekstit->Hae_Teksti(ind_font))) == -1){
				PK2_virhe = true;
				//PisteLog_Kirjoita("    - Loading font ");
				//PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_path));
				//PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_font));
				//PisteLog_Kirjoita(" failed!\n");
				}
			}

			ind_font = tekstit->Hae_Indeksi("font big font shadow");
			if (ind_path == -1 || ind_font == -1) {
				if ((fontti4 = PisteDraw2_Font_Create("language/fonts/","ScandicBig3.txt")) == -1){
				PK2_virhe = true;
				}
			}
			else {
				if ((fontti4 = PisteDraw2_Font_Create(tekstit->Hae_Teksti(ind_path),tekstit->Hae_Teksti(ind_font))) == -1){
				PK2_virhe = true;
				//PisteLog_Kirjoita("    - Loading font ");
				//PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_path));
				//PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_font));
				//PisteLog_Kirjoita(" failed!\n");
				}
			}

			if ((fontti2 = PisteDraw2_Font_Create("language/fonts/","ScandicBig1.txt")) == -1){
				PK2_virhe = true;
			}

			if ((fontti3 = PisteDraw2_Font_Create("language/fonts/","ScandicBig2.txt")) == -1){
				PK2_virhe = true;
			}

			if ((fontti4 = PisteDraw2_Font_Create("language/fonts/","ScandicBig3.txt")) == -1){
				PK2_virhe = true;
			}

			PK_Sprite_Tyhjenna();

			PK_Episodit_Hae();
			PK_Jaksot_Alusta();
			PK_Jaksot_Hae();

			PisteDraw2_ScreenFill(0);

			//PisteLog_Kirjoita("  - Loading basic sound fx \n");

			if ((kytkin_aani = PisteSound_LoadSFX("sfx/switch3.wav"))==-1)
				PK2_virhe = true;

			if ((hyppy_aani  = PisteSound_LoadSFX("sfx/jump4.wav"))==-1)
				PK2_virhe = true;

			if ((loiskahdus_aani  = PisteSound_LoadSFX("sfx/splash.wav"))==-1)
				PK2_virhe = true;

			if ((avaa_lukko_aani  = PisteSound_LoadSFX("sfx/openlock.wav"))==-1)
				PK2_virhe = true;

			if ((menu_aani  = PisteSound_LoadSFX("sfx/menu2.wav"))==-1)
				PK2_virhe = true;

			if ((ammuu_aani  = PisteSound_LoadSFX("sfx/moo.wav"))==-1)
				PK2_virhe = true;

			if ((kieku_aani  = PisteSound_LoadSFX("sfx/doodle.wav"))==-1)
				PK2_virhe = true;

			if ((tomahdys_aani  = PisteSound_LoadSFX("sfx/pump.wav"))==-1)
				PK2_virhe = true;

			if ((pistelaskuri_aani = PisteSound_LoadSFX("sfx/counter.wav"))==-1)
				PK2_virhe = true;

			if (PK2_virhe);
				//PisteLog_Kirjoita("  - Loading one or more basic sound fx failed!\n");

			//PisteWait_Start();

			PisteDraw2_FadeIn(PD_FADE_SLOW);

			//PisteLog_Kirjoita("  - Calculating tiles. \n");
			PK_Palikka_Laske_Palikat();

			PK_Esineet_Alusta();

			//PisteLog_Kirjoita("  - Loading background picture \n");
			PisteDraw2_Image_Delete(kuva_tausta);
			kuva_tausta = PisteDraw2_Image_Load("gfx/menu.bmp",true);

			PK_Tallennukset_Tyhjenna();

			//PisteLog_Kirjoita("  - Loading saves \n");
			PK_Tallennukset_Hae_Kaikki("data/saves.dat");

			//PisteLog_Kirjoita("  - PisteSound sounds on \n");
			//PisteSound_Aanet_Paalla(asetukset.aanet);

			//PisteLog_Kirjoita("- Initializing basic stuff completed \n");
		}

		// KARTAN ALUSTUS
		if (pelin_seuraava_tila == TILA_KARTTA){
			//PisteLog_Kirjoita("- Initializing map screen \n");

			PisteDraw2_ScreenFill(0);
			//PisteDraw2_Font_Write(fontti2,"loading...",RUUDUN_LEVEYS/2-82,RUUDUN_KORKEUS/2-9);

			if (!peli_kesken)
			{
				if (lataa_peli != -1)
				{
					//PisteLog_Kirjoita("  - Loading a saved game \n");
					PK_Jaksot_Hae();

					for (int j = 0;j < EPISODI_MAX_JAKSOJA;j++)
						 jaksot[j].lapaisty = tallennukset[lataa_peli].jakso_lapaisty[j];

					lataa_peli = -1;
					peli_kesken = true;
					peli_ohi = true;
					jakso_lapaisty = true;
					lopetusajastin = 0;

				}
				else
				{
					//PisteLog_Kirjoita("  - Starting a new game \n");
					PK_Jaksot_Alusta();	// jos ladataan peli, asetetaan l�p�istyarvot jaksoille aikaisemmin
					PK_Jaksot_Hae();
				}

				//PisteLog_Kirjoita("  - Loading top scores \n");
				char topscoretiedosto[_MAX_PATH] = "scores.dat";
				PK_Episodipisteet_Lataa(topscoretiedosto);
			}

			/* Ladataan kartan taustakuva ...*/
			char mapkuva[_MAX_PATH] = "map.bmp";
			PK_Lisaa_Episodin_Hakemisto(mapkuva);
			//PisteLog_Kirjoita("  - Loading map picture ");
			//PisteLog_Kirjoita(mapkuva);
			//PisteLog_Kirjoita(" from episode folder \n");

			PisteDraw2_Image_Delete(kuva_tausta);
			kuva_tausta = PisteDraw2_Image_Load(mapkuva,true);
			if (kuva_tausta == -1)
				kuva_tausta = PisteDraw2_Image_Load("gfx/map.bmp",true);

			/* Ladataan kartan musiikki ...*/
			char mapmusa[_MAX_PATH] = "map.xm"; //TODO - (Reduce char lenght)
			if(!PK_Onko_File(mapmusa)){
				strcpy(mapmusa,"map.mod");
				PK_Lisaa_Episodin_Hakemisto(mapmusa);

				if (!PK_Onko_File(mapmusa)) {
					strcpy(mapmusa,"map.it");
					PK_Lisaa_Episodin_Hakemisto(mapmusa);

					if (!PK_Onko_File(mapmusa)) {
						strcpy(mapmusa,"map.s3m");
						PK_Lisaa_Episodin_Hakemisto(mapmusa);

						if (!PK_Onko_File(mapmusa)) {
							strcpy(mapmusa,"music/map.xm");
						}
					}
				}
			}

			PisteSound_StartMusic(mapmusa);

			musiikin_voimakkuus = musiikin_max_voimakkuus; //TODO - Set music volume

			siirry_kartasta_peliin = false;

			PisteDraw2_FadeIn(PD_FADE_SLOW);
		}

		// MENUJEN ALUSTUS
		if (pelin_seuraava_tila == TILA_MENUT){
			//PisteLog_Kirjoita("- Initializing menu screen \n");

			PK_Kopioi_Pelitilanne_Taustakuvaksi();
			PK_Sumenna_Kuva(kuva_tausta, 640, 480);

			PisteDraw2_ScreenFill(0);

			PK_Episodit_Hae();

			if (!peli_kesken){
				PisteDraw2_Image_Delete(kuva_tausta);
				kuva_tausta = PisteDraw2_Image_Load("gfx/menu.bmp",true);
				PisteSound_StartMusic("music/song09.xm");//theme.xm
				musiikin_voimakkuus = musiikin_max_voimakkuus;
			}

			menunelio.left = 320-5;
			menunelio.top = 240-5;
			menunelio.right = 320+5;
			menunelio.bottom = 240+5;

			PisteDraw2_FadeIn(PD_FADE_NORMAL);

			//PisteWait_Start();

			menu_valittu_id = 1;

			//PisteLog_Kirjoita("- Initializing menu screen completed\n");
		}

		// UUDEN JAKSON ALUSTUS
		if (pelin_seuraava_tila == TILA_PELI){
			//PisteLog_Kirjoita("- Initializing a new level \n");

			PisteDraw2_ScreenFill(0);
			PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_game_loading),RUUDUN_LEVEYS/2-82,RUUDUN_KORKEUS/2-9);

			if (jaksot[jakso_indeksi_nyt].lapaisty)
				uusinta = true;
			else
				uusinta = false;

			if (!peli_kesken)
			{
				jakso_lapaisty = false;

				PK_Sprite_Tyhjenna();
				//PisteLog_Kirjoita("  - Loading map \n");
				if (PK_Kartta_Lataa(seuraava_kartta) == 1) {
					PK2_virhe = true;
					//PisteLog_Kirjoita("  - Loading map failed!\n");
				}

				//PisteLog_Kirjoita("  - Doing tile calcualtions...\n");
				PK_Palikka_Laske_Palikat();

				//PisteLog_Kirjoita("  - Initializing items \n");
				PK_Esineet_Alusta();
				peli_kesken = true;
				musiikin_voimakkuus = musiikin_max_voimakkuus;
				degree = 0;
				item_paneeli_x = -215;
				piste_lisays = 0;
			}
			else
			{
				degree = degree_temp;
			}

			//PisteWait_Start();
			//fps_aika = PisteWait_Get();

			//PisteLog_Kirjoita("- Initializing a new level complete\n");
		}

		// PISTELASKUN ALUSTUS
		if (pelin_seuraava_tila == TILA_PISTELASKU){

			PisteDraw2_ScreenFill(0);

			PisteDraw2_Image_Delete(kuva_tausta);
			kuva_tausta = PisteDraw2_Image_Load("gfx/menu.bmp",true);
			PK_Sumenna_Kuva(kuva_tausta, 640, 480);

			//if (PisteSound_StartMusic("music/hiscore.xm")!=0)
			//	PK2_virhe = true;

			jakso_uusi_ennatys = false;
			jakso_uusi_ennatysaika = false;
			episodi_uusi_ennatys = false;

			// Lasketaan pelaajan kokonaispisteet etuk�teen
			DWORD temp_pisteet = 0;
			temp_pisteet += jakso_pisteet;
			temp_pisteet += aika*5;
			temp_pisteet += spritet[pelaaja_index].energia * 300;
			for (int i=0;i<MAX_ESINEITA;i++)
				if (esineet[i] != NULL)
					temp_pisteet += esineet[i]->pisteet + 500;

			//if (jaksot[jakso_indeksi_nyt].lapaisty)
			//if (jaksot[jakso_indeksi_nyt].jarjestys == jakso-1)
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

			char pisteet_tiedosto[_MAX_PATH] = "scores.dat";
			int vertailun_tulos;

			/* Tutkitaan onko pelaajarikkonut kent�n piste- tai nopeusenn�tyksen */
			vertailun_tulos = PK_Episodipisteet_Vertaa(jakso_indeksi_nyt,temp_pisteet,kartta->aika-aika,false);
			if (vertailun_tulos > 0) {
				PK_Episodipisteet_Tallenna(pisteet_tiedosto);
			}

			/* Tutkitaan onko pelaaja rikkonut episodin piste-enn�tyksen */
			vertailun_tulos = PK_Episodipisteet_Vertaa(0,pisteet,0,true);
			if (vertailun_tulos > 0)
				PK_Episodipisteet_Tallenna(pisteet_tiedosto);

			musiikin_voimakkuus = musiikin_max_voimakkuus;

			siirry_pistelaskusta_karttaan = false;

			//PisteDraw2_FadeIn(PD_FADE_FAST);

			//PisteWait_Start();
		}

		// INTRON ALUSTUS
		if (pelin_seuraava_tila == TILA_INTRO){
			//PisteLog_Kirjoita("- Initializing intro screen\n");

			PisteDraw2_ScreenFill(0);

			//PisteLog_Kirjoita("  - Loading picture: gfx/intro.bmp\n");
			PisteDraw2_Image_Delete(kuva_tausta);
			kuva_tausta = PisteDraw2_Image_Load("gfx/intro.bmp",true);

			//PisteLog_Kirjoita("  - Loading music: music/INTRO.XM\n");

			if (PisteSound_StartMusic("music/intro.xm")!=0)
				PK2_virhe = true;

			musiikin_voimakkuus = musiikin_max_voimakkuus;

			introlaskuri = 0;
			siirry_pistelaskusta_karttaan = false;

			//PisteDraw2_FadeIn(PD_FADE_FAST);

			//PisteWait_Start();

			//PisteLog_Kirjoita("- Initializing intro screen complete\n");
		}

		if (pelin_seuraava_tila == TILA_LOPPU){

			PisteDraw2_ScreenFill(0);
			PisteDraw2_Image_Delete(kuva_tausta);
			kuva_tausta = PisteDraw2_Image_Load("gfx/ending.bmp",true);

			if (PisteSound_StartMusic("music/intro.xm")!=0)
				PK2_virhe = true;

			musiikin_voimakkuus = musiikin_max_voimakkuus;

			loppulaskuri = 0;
			siirry_lopusta_menuun = false;
			peli_kesken = false;

			//PisteDraw2_FadeIn(PD_FADE_FAST);

			//PisteWait_Start();
		}


		PisteDraw2_FadeIn(PD_FADE_NORMAL);

		pelin_tila = pelin_seuraava_tila;

	}
	return 0;
}

int PK_Spritet(){
	info_spriteja_aktiivisena = 0;

	//bool aktiivinen;
	int i;

	for (i=0;i<MAX_SPRITEJA;i++) //If it is on screen
	{
		// Onko sprite l�hell� tapahtumien keskipistett�? Jos on, niin aktivoidaan.
		if (spritet[i].x < kamera_x+640+320 &&//KARTANPIIRTO_LEVEYS+KARTANPIIRTO_LEVEYS/2 &&
			spritet[i].x > kamera_x-320 &&//KARTANPIIRTO_LEVEYS/2 &&
			spritet[i].y < kamera_y+480+240 &&//KARTANPIIRTO_KORKEUS+KARTANPIIRTO_KORKEUS/2 &&
			spritet[i].y > kamera_y-240)//KARTANPIIRTO_KORKEUS/2)
			spritet[i].aktiivinen = true;
		else
			spritet[i].aktiivinen = false;

		if (spritet[i].piilota == true)
			spritet[i].aktiivinen = false;
	}

	for (i=0;i<MAX_SPRITEJA;i++) //1
	{
		if (spritet[i].aktiivinen && spritet[i].tyyppi->tyyppi != TYYPPI_TAUSTA)
		{
			if (spritet[i].tyyppi->tyyppi == TYYPPI_BONUS)
				PK_Sprite_Liikuta_Bonus(i);
			else
				PK_Sprite_Liikuta(i);

			info_spriteja_aktiivisena++;
		}
	}

	return 0;
}

int PK_Spritet_Piirra_Taustat(){
	double xl, yl, alku_x, alku_y, yk;
	int i;


	for (int in=0;in<MAX_SPRITEJA;in++)
	{
		i = taustaspritet[in];
		// Onko sprite n�kyv�
		if (spritet[i].tyyppi != NULL && i != -1)
		{
			if (!spritet[i].piilota && spritet[i].tyyppi->tyyppi == TYYPPI_TAUSTA)
			{
				//Tarkistetaanko onko sprite tai osa siit� kuvassa

				alku_x = spritet[i].alku_x;
				alku_y = spritet[i].alku_y;

				if (spritet[i].tyyppi->pallarx_kerroin != 0)
				{
					xl =  alku_x - kamera_x-KARTANPIIRTO_LEVEYS/2 - spritet[i].tyyppi->leveys/2;
					xl /= spritet[i].tyyppi->pallarx_kerroin;
					yl =  alku_y - kamera_y-KARTANPIIRTO_KORKEUS/2 - spritet[i].tyyppi->korkeus/2;
					yk = spritet[i].tyyppi->pallarx_kerroin;///1.5;
					if (yk != 0)
						yl /= yk;


				}
				else
					xl = yl = 0;

				switch(spritet[i].tyyppi->AI[0])
				{
				case AI_TAUSTA_KUU					:	yl += KARTANPIIRTO_KORKEUS/3+50; break;
				/*case AI_TAUSTA_LIIKKUU_VASEMMALLE	:	if (spritet[i].a == 0)
															spritet[i].a = rand()%3;
														spritet[i].alku_x -= spritet[i].a;
														if (spritet[i].piilossa && spritet[i].alku_x < kamera_x)
														{
													  		  spritet[i].alku_x = kamera_x+RUUDUN_LEVEYS+spritet[i].tyyppi->leveys*2;
															  spritet[i].a = rand()%3;
														}
														break;*/
				case AI_LIIKKUU_X_COS:			spritet[i].AI_Liikkuu_X(cos_table[degree%360]);
												alku_x = spritet[i].x;
												alku_y = spritet[i].y;
												break;
				case AI_LIIKKUU_Y_COS:			spritet[i].AI_Liikkuu_Y(cos_table[degree%360]);
												alku_x = spritet[i].x;
												alku_y = spritet[i].y;
												break;
				case AI_LIIKKUU_X_SIN:			spritet[i].AI_Liikkuu_X(sin_table[degree%360]);
												alku_x = spritet[i].x;
												alku_y = spritet[i].y;
												break;
				case AI_LIIKKUU_Y_SIN:			spritet[i].AI_Liikkuu_Y(sin_table[degree%360]);
												alku_x = spritet[i].x;
												alku_y = spritet[i].y;
												break;
				default: break;
				}

				spritet[i].x = alku_x-xl;
				spritet[i].y = alku_y-yl;

				//Tarkistetaanko onko sprite tai osa siit� kuvassa
				if (spritet[i].x - spritet[i].tyyppi->leveys/2  < kamera_x+RUUDUN_LEVEYS &&
					spritet[i].x + spritet[i].tyyppi->leveys/2  > kamera_x &&
					spritet[i].y - spritet[i].tyyppi->korkeus/2 < kamera_y+RUUDUN_KORKEUS &&
					spritet[i].y + spritet[i].tyyppi->korkeus/2 > kamera_y)
				{
					spritet[i].Piirra(kamera_x,kamera_y);
					spritet[i].piilossa = false;

					info_spriteja_piirretty++;
				}
				else
				{
					if (!paused)
						spritet[i].Animoi();
					spritet[i].piilossa = true;
				}

				info_spriteja++;

			}
		}
	}
	return 0;
}

int PK_Spritet_Piirra(){
	info_spriteja = 0;
	info_spriteja_piirretty = 0;
	int stars, sx;
	double star_x, star_y;

	for (int i=0;i<MAX_SPRITEJA;i++)
	{
		// Onko sprite n�kyv�
		if (!spritet[i].piilota && spritet[i].tyyppi->tyyppi != TYYPPI_TAUSTA)
		{
			//Tarkistetaanko onko sprite tai osa siit� kuvassa
			if (spritet[i].x - spritet[i].tyyppi->leveys/2  < kamera_x+RUUDUN_LEVEYS &&
				spritet[i].x + spritet[i].tyyppi->leveys/2  > kamera_x &&
				spritet[i].y - spritet[i].tyyppi->korkeus/2 < kamera_y+RUUDUN_KORKEUS &&
				spritet[i].y + spritet[i].tyyppi->korkeus/2 > kamera_y)
			{
			if (spritet[i].isku > 0 && spritet[i].tyyppi->tyyppi != TYYPPI_BONUS && spritet[i].energia < 1)
				PK_Piirra_Iskuvalays((DWORD)spritet[i].x-8,(DWORD)spritet[i].y-8);


			if (nakymattomyys == 0 || degree%2 == 0 || i != pelaaja_index)
				spritet[i].Piirra(kamera_x,kamera_y);
			//else
			//	spritet[i].Piirra(kamera_x,kamera_y);

				if (spritet[i].energia < 1 && spritet[i].tyyppi->tyyppi != TYYPPI_AMMUS)
				{
					sx = (int)spritet[i].x;
					for (stars=0; stars<3; stars++)
					{
						star_x = spritet[i].x-8 + (sin_table[((stars*120+degree)*2)%359])/3;
						star_y = spritet[i].y-18 + (cos_table[((stars*120+degree)*2+sx)%359])/8;
						PK_Piirra_Tahti((DWORD)star_x,(DWORD)star_y,100,128);
					}
				}

				info_spriteja_piirretty++;
			}
			else
			{
				if (!paused)
					spritet[i].Animoi();

				if (spritet[i].energia < 1)
					spritet[i].piilota = true;
			}

			info_spriteja++;
		}
	}
	return 0;
}

int PK_Kamera(){


	kamera_x = (int)spritet[pelaaja_index].x-KARTANPIIRTO_LEVEYS/2;
	kamera_y = (int)spritet[pelaaja_index].y-KARTANPIIRTO_KORKEUS/2;

	/*
	if (!PisteInput_Hiiri_Vasen())
	{
		kamera_x = (int)spritet[pelaaja_index].x-KARTANPIIRTO_LEVEYS/2;
		kamera_y = (int)spritet[pelaaja_index].y-KARTANPIIRTO_KORKEUS/2;
	}
	else
	{
		kamera_x += PisteInput_Hiiri_X(0)*5;
		kamera_y += PisteInput_Hiiri_Y(0)*5;
	}*/

	if (jaristys > 0)
	{
		dkamera_x += (rand()%jaristys-rand()%jaristys)/5;
		dkamera_y += (rand()%jaristys-rand()%jaristys)/5;

		jaristys--;
	}

	if (kytkin_tarina > 0)
	{
		dkamera_x += (rand()%9-rand()%9);//3
		dkamera_y += (rand()%9-rand()%9);

		kytkin_tarina--;
	}

	if (dkamera_x != kamera_x)
		dkamera_a = (kamera_x - dkamera_x) / 15;

	if (dkamera_y != kamera_y)
		dkamera_b = (kamera_y - dkamera_y) / 15;

	if (dkamera_a > 6)
		dkamera_a = 6;

	if (dkamera_a < -6)
		dkamera_a = -6;

	if (dkamera_b > 6)
		dkamera_b = 6;

	if (dkamera_b < -6)
		dkamera_b = -6;

	dkamera_x += dkamera_a;
	dkamera_y += dkamera_b;

	kamera_x = (int)dkamera_x;
	kamera_y = (int)dkamera_y;

	if (kamera_x < 0)
		kamera_x = 0;

	if (kamera_y < 0)
		kamera_y = 0;

	if (kamera_x > int(PK2KARTTA_KARTTA_LEVEYS-KARTANPIIRTO_LEVEYS/32)*32)
		kamera_x = int(PK2KARTTA_KARTTA_LEVEYS-KARTANPIIRTO_LEVEYS/32)*32;

	if (kamera_y > int(PK2KARTTA_KARTTA_KORKEUS-KARTANPIIRTO_KORKEUS/32)*32)
		kamera_y = int(PK2KARTTA_KARTTA_KORKEUS-KARTANPIIRTO_KORKEUS/32)*32;

	return 0;
}

//==================================================
//Draw Functions (Piirra)
//==================================================
int PK_Piirra_Info(){
	int fy, vali;
	char lukua[20];

	fy = 35;

	vali = PisteDraw2_Font_Write(fontti1,"spriteja: ",10,fy);
	itoa(info_spriteja,lukua,10);
	PisteDraw2_Font_Write(fontti1,lukua,10+vali,fy);
	fy += 10;

	vali = PisteDraw2_Font_Write(fontti1,"aktiivisia: ",10,fy);
	itoa(info_spriteja_aktiivisena,lukua,10);
	PisteDraw2_Font_Write(fontti1,lukua,10+vali,fy);
	fy += 10;

	vali = PisteDraw2_Font_Write(fontti1,"piirretty: ",10,fy);
	itoa(info_spriteja_piirretty,lukua,10);
	PisteDraw2_Font_Write(fontti1,lukua,10+vali,fy);
	fy += 10;

	for (int i=0;i<40;i++)
	{
		itoa(protot[i].indeksi,lukua,10);
		PisteDraw2_Font_Write(fontti1,lukua,410,10+i*10);
		PisteDraw2_Font_Write(fontti1,protot[i].tiedosto,430,10+i*10);
		PisteDraw2_Font_Write(fontti1,protot[i].bonus_sprite,545,10+i*10);
	}

	PisteDraw2_Font_Write(fontti1,viesti,10,80);

	for (int i=0;i<EPISODI_MAX_JAKSOJA;i++)
	{
		if (strcmp(jaksot[i].nimi,"")!=0)
			PisteDraw2_Font_Write(fontti1,jaksot[i].nimi,0,240+i*10);
	}

	PisteDraw2_Font_Write(fontti1,seuraava_kartta,10,450);


	char dluku[50];

	gcvt(spritet[pelaaja_index].x,7,dluku);
	PisteDraw2_Font_Write(fontti1,dluku,10,420);
	gcvt(spritet[pelaaja_index].y,7,dluku);
	PisteDraw2_Font_Write(fontti1,dluku,10,430);
	gcvt(spritet[pelaaja_index].b,7,dluku);
	PisteDraw2_Font_Write(fontti1,dluku,10,440);

	itoa(spritet[pelaaja_index].hyppy_ajastin,lukua,10);
	PisteDraw2_Font_Write(fontti1,lukua,270,450);

	char tpolku[_MAX_PATH] = "";
	PK_Lisaa_Episodin_Hakemisto(tpolku);

	PisteDraw2_Font_Write(fontti1,tpolku,10,470);

	itoa(nakymattomyys,lukua,10);
	PisteDraw2_Font_Write(fontti1,lukua,610,470);

	return 0;
}

int PK_Piirra_Tausta(){
	int pallarx = (kamera_x%1920)/3;
	int pallary = (kamera_y%1440)/3;

	int pallarx2 = (kamera_x%1280)/2;
	int pallary2 = (kamera_y%960)/2;

	PisteDraw2_ScreenFill(34);//0


	if (kartta->tausta == TAUSTA_STAATTINEN)
	{
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0,0);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640,0);
	}

	if (kartta->tausta == TAUSTA_PALLARX_HORI)
	{
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0 - pallarx,0);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640 - pallarx,0);

		if (KARTANPIIRTO_LEVEYS > 640)
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,1280 - pallarx,0);
	}

	if (kartta->tausta == TAUSTA_PALLARX_VERT)
	{
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0,0 - pallary);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0,480 - pallary);

		if (KARTANPIIRTO_LEVEYS > 640)
		{
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640,0 - pallary);
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640,480 - pallary);
		}
	}

	if (kartta->tausta == TAUSTA_PALLARX_VERT_JA_HORI)
	{
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0 - pallarx,	0-pallary);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640 - pallarx,0-pallary);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0 - pallarx,	480-pallary);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640 - pallarx,480-pallary);

		if (KARTANPIIRTO_LEVEYS > 640)
		{
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,1280 - pallarx,0-pallary);
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,1280 - pallarx,480-pallary);
		}
	}

	/*pilvet 1*/
	/*
	PisteDraw2_Image_Clip(tausta_extra,0-pallarx2,0-pallary2);
	PisteDraw2_Image_Clip(tausta_extra,640-pallarx2,0-pallary2);
	PisteDraw2_Image_Clip(tausta_extra,0-pallarx2,480-pallary2);
	PisteDraw2_Image_Clip(tausta_extra,640-pallarx2,480-pallary2);
	*/
	/*pilvet 2
	pallarx2 = (int)((kamera_x%1600)/2.5);

	PisteDraw2_Image_Clip(tausta_extra,0-pallarx2,95,0,332,640,382);
	PisteDraw2_Image_Clip(tausta_extra,640-pallarx2,95,0,332,640,382);

	pallarx2 = (int)((kamera_x%1280)/2);

	PisteDraw2_Image_Clip(tausta_extra,0-pallarx2,65,0,186,640,263);
	PisteDraw2_Image_Clip(tausta_extra,640-pallarx2,65,0,186,640,263);

	pallarx2 = (int)((kamera_x%960)/1.5);

	PisteDraw2_Image_Clip(tausta_extra,0-pallarx2,0,0,0,640,109);
	PisteDraw2_Image_Clip(tausta_extra,640-pallarx2,0,0,0,640,109);
	*/
	return 0;
}

int PK_Piirra_Alapaneeli(){

	if (esineita > 0 && item_paneeli_x < 10)
		item_paneeli_x++;

	if (esineita == 0 && item_paneeli_x > -215)
		item_paneeli_x--;

	if (item_paneeli_x > -215)
		PisteDraw2_Image_CutClip(kuva_peli,item_paneeli_x,KARTANPIIRTO_KORKEUS-60,
		                        1,216,211,266);
	if (item_paneeli_x > 5)
			PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_items),15,KARTANPIIRTO_KORKEUS-65);


	char luku[15];
	int vali = 0;

	int x, y;

	//////////////
	// piirr� aika
	//////////////
	if (aika > 0)
	{
		int min = aika/60,
			sek = aika%60;


		x = RUUDUN_LEVEYS / 2 - 546 / 2 + 342;
		y = KARTANPIIRTO_KORKEUS-39;
		PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_time),x,y-20);


		itoa(min,luku,10);
		PisteDraw2_Font_Write(        fontti4,luku,x+1,y+1);
		vali += PisteDraw2_Font_Write(fontti2,luku,x,y);
		vali += PisteDraw2_Font_Write(fontti1,":",x+vali,y);
		if (sek < 10)
		{
			PisteDraw2_Font_Write(        fontti4,"0",x+vali+1,y+1);
			vali += PisteDraw2_Font_Write(fontti2,"0",x+vali,y);
		}
		itoa(sek,luku,10);

		PisteDraw2_Font_Write(        fontti4,luku,x+vali+1,y+1);
		vali += PisteDraw2_Font_Write(fontti2,luku,x+vali,y);
	}

	/////////////////
	// piirr� avaimet
	/////////////////
	if (avaimia > 0)
	{
		x = RUUDUN_LEVEYS / 2 - 546 / 2 + 483;
		y = KARTANPIIRTO_KORKEUS-39;
		PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_keys),x,y-20);

		itoa(avaimia,luku,10);
		PisteDraw2_Font_Write(fontti4,luku,x+1,y+1);
		PisteDraw2_Font_Write(fontti2,luku,x,y);
	}

	/////////////////
	// piirr� esineet
	/////////////////
	PK_Esineet_Piirra();

	return 0;
}

int PK_Piirra_Peli_Ylaosa(){
	char luku[15];
	int vali = 20;

	vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_energy),40,10);
	ltoa(spritet[pelaaja_index].energia,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,40+vali+1,10+1);
	PisteDraw2_Font_Write(fontti2,luku,40+vali,10);

	vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_score),230,10);
	ltoa(jakso_pisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,230+vali+1,10+1);
	PisteDraw2_Font_Write(fontti2,luku,230+vali,10);

	if (spritet[pelaaja_index].ammus2 != -1)
	{
		//itoa(ammuksia2,luku,10);
		PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_attack1), KARTANPIIRTO_LEVEYS-170,10);
		protot[spritet[pelaaja_index].ammus2].Piirra(KARTANPIIRTO_LEVEYS-170,20,0);
		//PisteDraw2_Font_Write(fontti2,luku,RUUDUN_LEVEYS-35,50);
	}

	if (spritet[pelaaja_index].ammus1 != -1)
	{
		//itoa(ammuksia1,luku,10);
		PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_attack2), KARTANPIIRTO_LEVEYS-90,25);
		protot[spritet[pelaaja_index].ammus1].Piirra(KARTANPIIRTO_LEVEYS-90,35,0);
		//PisteDraw2_Font_Write(fontti2,luku,RUUDUN_LEVEYS-35,10);
	}


	if (ilmoitus_ajastin > 0)
	{
		int ilm_pituus = strlen(ilmoitus) * 8 + 8; // 300

		RECT alue = {RUUDUN_LEVEYS/2-(ilm_pituus/2),60,RUUDUN_LEVEYS/2+(ilm_pituus/2),60+20};

		if (ilmoitus_ajastin < 20)
		{
			alue.top	+= (20 - ilmoitus_ajastin) / 2;
			alue.bottom -= (20 - ilmoitus_ajastin) / 2;
		}

		if (ilmoitus_ajastin > MAX_ILMOITUKSENNAYTTOAIKA - 20)
		{
			alue.top	+= 10 - (MAX_ILMOITUKSENNAYTTOAIKA - ilmoitus_ajastin) / 2;
			alue.bottom -= 10 - (MAX_ILMOITUKSENNAYTTOAIKA - ilmoitus_ajastin) / 2;
		}

		PisteDraw2_ScreenFill(alue.left-1,alue.top-1,alue.right+1,alue.bottom+1,51);
		PisteDraw2_ScreenFill(alue.left,alue.top,alue.right,alue.bottom,38);

		if (ilmoitus_ajastin >= 100)
			PisteDraw2_Font_Write(fontti1,ilmoitus,alue.left+4,alue.top+4);
		else
			PisteDraw2_Font_WriteAlpha(fontti1,ilmoitus,alue.left+4,alue.top+4,ilmoitus_ajastin);
	}

	return 0;
}

int PK_Piirra_Peli(){
	char luku[15];
	int vali = 20;

	PK_Piirra_Tausta();

	PK2Kartta_Animoi(degree,palikka_animaatio/7,kytkin1,kytkin2,kytkin3,false);

	if (!skip_frame)
		if (asetukset.tausta_spritet)
			PK_Spritet_Piirra_Taustat();

	if (asetukset.saa_efektit)
		PK_Taustapartikkelit();

	if (!skip_frame)
		kartta->Piirra_Taustat(kamera_x,kamera_y,false);

	if (!paused)
	{
		PK_Partikkelit();
		if (!jakso_lapaisty && (!aikaraja || aika > 0))
			PK_Spritet();
		PK_Fadetekstit();
	}

	if (!skip_frame)
		PK_Spritet_Piirra();

	//PK_Piirra_Bonukset();

	if (!skip_frame)
		PK_Partikkelit_Piirra();

	if (!skip_frame)
		kartta->Piirra_Seinat(kamera_x,kamera_y, false);

	PK_Fadetekstit_Piirra();

	if (asetukset.nayta_tavarat)
		PK_Piirra_Alapaneeli();

	PK_Piirra_Peli_Ylaosa();

	/*
	//////////////
	// piirr� aika
	//////////////
	int min = aika/60,
		sek = aika%60;
	vali = PisteDraw2_Font_Write(fontti2,"time ",272,RUUDUN_KORKEUS-20);
	itoa(min,luku,10);
	vali += PisteDraw2_Font_Write(fontti2,luku,272+vali,RUUDUN_KORKEUS-20);
	vali += PisteDraw2_Font_Write(fontti1,":",272+vali,RUUDUN_KORKEUS-15);

	if (sek < 10)
		vali += PisteDraw2_Font_Write(fontti2,"0",272+vali,RUUDUN_KORKEUS-20);

	itoa(sek,luku,10);
	vali += PisteDraw2_Font_Write(fontti2,luku,272+vali,RUUDUN_KORKEUS-20);

	/////////////////
	// piirr� avaimet
	/////////////////
	vali = PisteDraw2_Font_Write(fontti2,"keys ",450,RUUDUN_KORKEUS-20);
	itoa(avaimia,luku,10);
	PisteDraw2_Font_Write(fontti2,luku,450+vali,RUUDUN_KORKEUS-20);

	/////////////////
	// piirr� esineet
	/////////////////
	PK_Esineet_Piirra();
	*/
	if (piirra_infot)
		PK_Piirra_Info();

	///////////////////
	// piirr� framerate
	///////////////////
	if (fps_nayta)
	{
		vali = PisteDraw2_Font_Write(fontti1,"fps: ",570,48);
		itoa(fps,luku,10);
		PisteDraw2_Font_Write(fontti1,luku,570+vali,48);
	}

	///////////////////
	// piirr� pause
	///////////////////
	if (paused)
		PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_game_paused),KARTANPIIRTO_LEVEYS/2-82,KARTANPIIRTO_KORKEUS/2-9);

	////////////////////////
	// piirr� jakso l�p�isty
	////////////////////////
	if (jakso_lapaisty)
	{
		PK_Piirra_LaineTeksti(tekstit->Hae_Teksti(txt_game_clear),fontti2,KARTANPIIRTO_LEVEYS/2-120,KARTANPIIRTO_KORKEUS/2-9);

		/*
		if (aikaraja)
		{
			vali = PK_Piirra_LaineTeksti(tekstit->Hae_Teksti(txt_game_timebonus),fontti2,KARTANPIIRTO_LEVEYS/2-120,KARTANPIIRTO_KORKEUS/2+20);
			itoa(fake_pisteet,luku,10);
			PK_Piirra_LaineTeksti(luku,fontti2,KARTANPIIRTO_LEVEYS/2-120+vali,KARTANPIIRTO_KORKEUS/2+20);
		}*/

	}
	else
		if (peli_ohi)
		{
			if (spritet[pelaaja_index].energia < 1)
			{
				PK_Piirra_LaineTeksti(tekstit->Hae_Teksti(txt_game_ko),fontti2,KARTANPIIRTO_LEVEYS/2-90,KARTANPIIRTO_KORKEUS/2-9-10);
			}
			else
				if (aika < 1 && aikaraja)
					PK_Piirra_LaineTeksti(tekstit->Hae_Teksti(txt_game_timeout),fontti2,KARTANPIIRTO_LEVEYS/2-67,KARTANPIIRTO_KORKEUS/2-9-10);

			PK_Piirra_LaineTeksti(tekstit->Hae_Teksti(txt_game_tryagain),fontti2,KARTANPIIRTO_LEVEYS/2-75,KARTANPIIRTO_KORKEUS/2-9+10);
		}


	if (tuplanopeus)
		skip_frame = !skip_frame;
	else
		skip_frame = false;

	if (!skip_frame)
	{
		//PisteWait_Wait(0);//10


		//PisteWait_Start();
	}

	palikka_animaatio = 1 + palikka_animaatio % 34;

	return 0;
}

void PK_Piirra_Kursori(int x,int y){
	PisteDraw2_Image_CutClip(kuva_peli,x,y,621,461,640,480);
}

int PK_Piirra_Menu_Nelio(int vasen, int yla, int oikea, int ala, BYTE pvari){
	if (peli_kesken)
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

bool PK_Piirra_Menut_Valinta(char *teksti, int x, int y){
	int pituus = strlen(teksti)*15;

	if ((hiiri_x > x && hiiri_x < x+pituus && hiiri_y > y && hiiri_y < y+15) ||
		(menu_valittu_id == menu_valinta_id))
	{
		menu_valittu_id = menu_valinta_id;

		if ((
			(PisteInput_Hiiri_Vasen() && hiiri_x > x && hiiri_x < x+pituus && hiiri_y > y && hiiri_y < y+15)
			|| PisteInput_Keydown(PI_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1))
			&& key_delay == 0)
		{
			PK_Soita_Aani_Menu(menu_aani, 100);
			key_delay = 20;
			return true;
		}

		PK_Piirra_LaineTeksti(teksti, fontti3, x, y);
	}
	else
		PK_Piirra_LaineTeksti_Hidas(teksti, fontti2, x, y);
		//PisteDraw2_Font_Write(fontti2,teksti,x,y);

	menu_valinta_id++;

	return false;
}

int PK_Piirra_Menut_Valintalaatikko(int x, int y, bool muuttuja){
	if (!muuttuja)
		PisteDraw2_Image_CutClip(kuva_peli,x,y,473,124,473+31,124+31);
	else
		PisteDraw2_Image_CutClip(kuva_peli,x,y,504,124,504+31,124+31);

	if ((hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31))/* ||
		(menu_valittu_id == menu_valinta_id))*/
	{
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1))
			&& key_delay == 0)
		{
			PK_Soita_Aani_Menu(menu_aani, 100);
			key_delay = 20;
			return true;
		}
	}

	//menu_valinta_id++;

	return false;
}

int PK_Piirra_Menut_Saatolaatikko(int x, int y){
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

	if ((hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31) || (menu_valittu_id == menu_valinta_id))
	{
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1))
			&& key_delay == 0)
		{
			PK_Soita_Aani_Menu(menu_aani, 100);
			key_delay = 20;
			return 1;
		}
	}

	x += val;

	if ((hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31) || (menu_valittu_id == menu_valinta_id+1))
	{
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1))
			&& key_delay == 0)
		{
			PK_Soita_Aani_Menu(menu_aani, 100);
			key_delay = 20;
			return 2;
		}
	}

	menu_valinta_id += 2;

	return 0;
}

int PK_Piirra_Menut_PaaValikko(){
	int my = 240;//250;

	PK_Piirra_Menu_Nelio(160, 200, 640-180, 410, 224);

	if (peli_kesken)
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_continue),180,my))
		{
			if ((!peli_ohi && !jakso_lapaisty) || lopetusajastin > 1)
				pelin_seuraava_tila = TILA_PELI;
			else
				pelin_seuraava_tila = TILA_KARTTA;

		}
		my += 20;
	}

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_new_game),180,my))
	{
		nimiedit = true;
		menu_nimi_index = strlen(pelaajan_nimi);//   0;
		menu_nimi_ed_merkki = ' ';
		menu_nyt = MENU_NIMI;
		key_delay = 30;
	}
	my += 20;

	if (peli_kesken)
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_save_game),180,my))
		{
			menu_nyt = MENU_TALLENNA;
		}
		my += 20;
	}

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_load_game),180,my))
	{
		menu_nyt = MENU_LATAA;
	}
	my += 20;

	if (PK_Piirra_Menut_Valinta("load language",180,my))
	{
		menu_nyt = MENU_LANGUAGE;
	}
	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_controls),180,my))
	{
		menu_nyt = MENU_KONTROLLIT;
	}
	my += 20;

	if (PK_Piirra_Menut_Valinta("full screen",180,my))
	{
		PisteDraw2_FullScreen();
	}
	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_graphics),180,my))
	{
		menu_nyt = MENU_GRAFIIKKA;
	}
	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_sounds),180,my))
	{
		menu_nyt = MENU_AANET;
	}
	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_exit),180,my))
	{
		lopeta_peli = true;
		PisteDraw2_FadeOut(PD_FADE_NORMAL);
		musiikin_voimakkuus = 0;
	}
	my += 20;

	return 0;
}

int PK_Piirra_Menut_Nimi(){
	int my = 0,
		mx = menu_nimi_index*15 + 180 + rand()%2;
	char merkki;
	bool hiiri_alueella = false;

	PK_Piirra_Menu_Nelio(90, 150, 640-90, 480-100, 224);

	if (hiiri_x > 180 && hiiri_x < 180+15*20 && hiiri_y > 255 && hiiri_y < 255+18)
		hiiri_alueella = true;

	if (hiiri_alueella && PisteInput_Hiiri_Vasen() && key_delay == 0)
	{
		nimiedit = true;
		menu_nimi_index = (hiiri_x - 180)/15;
		key_delay = 10;
	}

	if (nimiedit && key_delay == 0) {

		if (PisteInput_Keydown(PI_LEFT)) {
			menu_nimi_index--;
			key_delay = 8;
		}

		if (PisteInput_Keydown(PI_RIGHT)) {
			menu_nimi_index++;
			key_delay = 8;
		}
	}

	if (menu_nimi_index >= 19)
		menu_nimi_index = 18;

	if (menu_nimi_index >= (int)strlen(pelaajan_nimi))
		menu_nimi_index = (int)strlen(pelaajan_nimi);

	if (menu_nimi_index < 0)
		menu_nimi_index = 0;

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_playermenu_type_name),180,224);

	PisteDraw2_ScreenFill(180-2,255-2,180+20*15+4,255+18+4,0);
	PisteDraw2_ScreenFill(180,255,180+20*15,255+18,50);
	if (nimiedit) {
		PisteDraw2_ScreenFill(mx-2,254,mx+16+3,254+20+3,0);
		PisteDraw2_ScreenFill(mx-1,254,mx+16,254+20,96+16);
		PisteDraw2_ScreenFill(mx+12,254,mx+16,254+20,96+8);
	}

	PK_Piirra_LaineTeksti_Hidas(pelaajan_nimi,fontti2,180,255);
	PisteDraw2_Font_WriteAlpha(fontti3,pelaajan_nimi,180,255,15);

	merkki = PisteInput_Lue_Nappaimisto();

	if (PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1) && key_delay == 0 && nimiedit) {
		nimiedit = false;
	}

	if (merkki != '\0' && (merkki != menu_nimi_ed_merkki || key_delay == 0) && nimiedit)
	{
		menu_nimi_ed_merkki = merkki;

		if (merkki == PI_RETURN) {
			merkki = '\0';
			nimiedit = false;
			menu_valittu_id = 1;
		}

		key_delay = 15;
		pelaajan_nimi[menu_nimi_index] = merkki;
		menu_nimi_index++;

		//if (hiiri_alueella)
		//	hiiri_x += 15;
	}

	if (key_delay == 0)
	{
		if (PisteInput_Keydown(PI_DELETE) || PisteInput_Keydown(PI_BACK))
		{
			for (int c=menu_nimi_index;c<19;c++)
				pelaajan_nimi[c] = pelaajan_nimi[c+1];
				pelaajan_nimi[19] = '\0';
				key_delay = 10;
		}

		if (PisteInput_Keydown(PI_BACK))
		{
			key_delay = 10;
			menu_nimi_index--;
		//if (hiiri_alueella)
		//	hiiri_x -= 15;
		}
	}


	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_playermenu_continue),180,300))
	{
		menu_nyt = MENU_EPISODIT;
		menu_nimi_index = 0;
		nimiedit = false;
		menu_valittu_id = menu_valinta_id = 1;

		if (episodi_lkm == 1) {
			strcpy(episodi,episodit[2]);
			pelin_seuraava_tila = TILA_KARTTA;
			peli_kesken = false;
			PK_Uusi_Peli();
		}

	}

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_playermenu_clear),340,300))
	{
		memset(pelaajan_nimi,'\0',sizeof(pelaajan_nimi));
		strcpy(pelaajan_nimi," ");
		menu_nimi_index = 0;
	}


	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_exit),180,400))
	{
		menu_nyt = MENU_PAAVALIKKO;
		menu_nimi_index = 0;
		nimiedit = false;
	}

	return 0;
}

int PK_Piirra_Menut_Lataa(){
	int my = 0, vali = 0;
	char tpaikka[100];
	char jaksoc[8];
	char ind[4];

	PK_Piirra_Menu_Nelio(40, 70, 640-40, 410, 70);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_loadgame_title),50,90);
	PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_loadgame_info),50,110);
	my = -20;

	for (int i=0;i<MAX_TALLENNUKSIA;i++)
	{
		itoa(i+1,ind,10);
		strcpy(tpaikka,ind);
		strcat(tpaikka,". ");

		strcat(tpaikka,tallennukset[i].nimi);

		if (PK_Piirra_Menut_Valinta(tpaikka,100,150+my))
			PK_Tallennukset_Lataa(i);

		if (strcmp(tallennukset[i].episodi," ")!=0)
		{
			vali = 0;
			vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_loadgame_episode),400,150+my);
			vali += PisteDraw2_Font_Write(fontti1,tallennukset[i].episodi,400+vali,150+my);
			vali = 0;
			vali += PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_loadgame_level),400+vali,160+my);
			itoa(tallennukset[i].jakso,jaksoc,10);
			vali += PisteDraw2_Font_Write(fontti1,jaksoc,400+vali,160+my);
		}

		my += 22;
	}

	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_return),180,400))
		menu_nyt = MENU_PAAVALIKKO;

	return 0;
}

int PK_Piirra_Menut_Tallenna(){
	int my = 0, vali = 0;
	char tpaikka[100];
	char jaksoc[8];
	char ind[4];

	PK_Piirra_Menu_Nelio(40, 70, 640-40, 410, 224);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_savegame_title),50,90);
	PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_savegame_info),50,110);
	my = -20;

	for (int i=0;i<MAX_TALLENNUKSIA;i++)
	{
		itoa(i+1,ind,10);
		strcpy(tpaikka,ind);
		strcat(tpaikka,". ");

		strcat(tpaikka,tallennukset[i].nimi);

		if (PK_Piirra_Menut_Valinta(tpaikka,100,150+my))
			PK_Tallennukset_Tallenna(i);

		if (strcmp(tallennukset[i].episodi," ")!=0)
		{
			vali = 0;
			vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_savegame_episode),400,150+my);
			vali += PisteDraw2_Font_Write(fontti1,tallennukset[i].episodi,400+vali,150+my);
			vali = 0;
			vali += PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_savegame_level),400+vali,160+my);
			itoa(tallennukset[i].jakso,jaksoc,10);
			vali += PisteDraw2_Font_Write(fontti1,jaksoc,400+vali,160+my);
		}

		my += 22;
	}

	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_return),180,400))
		menu_nyt = MENU_PAAVALIKKO;

	return 0;
}

int PK_Piirra_Menut_Grafiikka(){
	int my = 0;

	PK_Piirra_Menu_Nelio(40, 70, 640-40, 410, 224);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_gfx_title),50,90);
	my += 150;

	if (asetukset.lapinakyvat_objektit)
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_gfx_tfx_on),180,my))
			asetukset.lapinakyvat_objektit = false;
	}
	else
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_gfx_tfx_off),180,my))
			asetukset.lapinakyvat_objektit = true;
	}

	if (PK_Piirra_Menut_Valintalaatikko(100, my, asetukset.lapinakyvat_objektit)) {
		asetukset.lapinakyvat_objektit = !asetukset.lapinakyvat_objektit;
	}

	my += 30;

	if (asetukset.lapinakyvat_menutekstit)
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_gfx_tmenus_on),180,my))
			asetukset.lapinakyvat_menutekstit = false;
	}
	else
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_gfx_tmenus_off),180,my))
			asetukset.lapinakyvat_menutekstit = true;
	}

	if (PK_Piirra_Menut_Valintalaatikko(100, my, asetukset.lapinakyvat_menutekstit)) {
		asetukset.lapinakyvat_menutekstit = !asetukset.lapinakyvat_menutekstit;
	}

	my += 30;

	if (asetukset.nayta_tavarat)
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_gfx_items_on),180,my))
			asetukset.nayta_tavarat = false;
	}
	else
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_gfx_items_off),180,my))
			asetukset.nayta_tavarat = true;
	}

	if (PK_Piirra_Menut_Valintalaatikko(100, my, asetukset.nayta_tavarat)) {
		asetukset.nayta_tavarat = !asetukset.nayta_tavarat;
	}

	my += 30;

	if (asetukset.saa_efektit)
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_gfx_weather_on),180,my))
			asetukset.saa_efektit = false;
	}
	else
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_gfx_weather_off),180,my))
			asetukset.saa_efektit = true;
	}

	if (PK_Piirra_Menut_Valintalaatikko(100, my, asetukset.saa_efektit)) {
		asetukset.saa_efektit = !asetukset.saa_efektit;
	}

	my += 30;

	if (asetukset.tausta_spritet)
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_gfx_bgsprites_on),180,my))
			asetukset.tausta_spritet = false;
	}
	else
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_gfx_bgsprites_off),180,my))
			asetukset.tausta_spritet = true;
	}

	if (PK_Piirra_Menut_Valintalaatikko(100, my, asetukset.tausta_spritet)) {
		asetukset.tausta_spritet = !asetukset.tausta_spritet;
	}

	my += 30;

	if (tuplanopeus)
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_gfx_speed_double),180,my))
			tuplanopeus = false;
	}
	else
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_gfx_speed_normal),180,my))
			tuplanopeus = true;
	}

	if (PK_Piirra_Menut_Valintalaatikko(100, my, tuplanopeus)) {
		tuplanopeus = !tuplanopeus;
	}

	my += 30;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_return),180,400))
		menu_nyt = MENU_PAAVALIKKO;

	return 0;
}

int PK_Piirra_Menut_Aanet(){
	int my = 0;

	PK_Piirra_Menu_Nelio(40, 70, 640-40, 410, 224);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_sound_title),50,90);
	my += 20;

	PisteDraw2_ScreenFill(404,224+my,404+aanenvoimakkuus,244+my,0);
	PisteDraw2_ScreenFill(400,220+my,400+aanenvoimakkuus,240+my,81);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_sound_sfx_volume),180,200+my);
	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_sound_less),180,200+my))
		if (aanenvoimakkuus > 0)
			aanenvoimakkuus -= 5;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_sound_more),180+8*15,200+my))
		if (aanenvoimakkuus < 100)
			aanenvoimakkuus += 5;

	if (aanenvoimakkuus < 0)
		aanenvoimakkuus = 0;

	if (aanenvoimakkuus > 100)
		aanenvoimakkuus = 100;

	my+=40;

	PisteDraw2_ScreenFill(404,224+my,404+int(musiikin_max_voimakkuus*1.56),244+my,0);
	PisteDraw2_ScreenFill(400,220+my,400+int(musiikin_max_voimakkuus*1.56),240+my,112);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_sound_music_volume),180,200+my);
	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_sound_less),180,200+my))
		if (musiikin_max_voimakkuus > 0)
			musiikin_max_voimakkuus -= 4;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_sound_more),180+8*15,200+my))
		if (musiikin_max_voimakkuus < 64)
			musiikin_max_voimakkuus += 4;

	if (musiikin_max_voimakkuus < 0)
		musiikin_max_voimakkuus = 0;

	if (musiikin_max_voimakkuus > 64)
		musiikin_max_voimakkuus = 64;

	musiikin_voimakkuus = musiikin_max_voimakkuus;

	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_return),180,400))
		menu_nyt = MENU_PAAVALIKKO;

	return 0;
}

int PK_Piirra_Menut_Kontrollit(){
	int my = 0;

	PK_Piirra_Menu_Nelio(40, 70, 640-40, 410, 224);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_title),50,90);

	my = 40;

	if (menu_lue_kontrollit > 0)
	{
		PisteDraw2_ScreenFill(299,74+my+menu_lue_kontrollit*20,584,94+my+menu_lue_kontrollit*20,0);
		PisteDraw2_ScreenFill(295,70+my+menu_lue_kontrollit*20,580,90+my+menu_lue_kontrollit*20,50);
	}

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_moveleft),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_moveright),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_jump),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_duck),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_walkslow),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_eggattack),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_doodleattack),100,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_controls_useitem),100,90+my);my+=20;

	my = 40;
	PisteDraw2_Font_Write(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_vasemmalle),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_oikealle),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_hyppy),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_alas),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_juoksu),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_hyokkays1),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_hyokkays2),310,90+my);my+=20;
	PisteDraw2_Font_Write(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_kayta_esine),310,90+my);my+=20;

	/*
	if (hiiri_x > 310 && hiiri_x < 580 && hiiri_y > 130 && hiiri_y < my-20)
	{
		menu_lue_kontrollit = (hiiri_y - 120) / 20;

		if (menu_lue_kontrollit < 0 || menu_lue_kontrollit > 8)
			menu_lue_kontrollit = 0;
		else
			key_delay = 25;


	}*/

	if (menu_lue_kontrollit == 0)
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_controls_edit),100,90+my))
			menu_lue_kontrollit = 1;
	}

	my += 30;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_controls_kbdef),100,90+my))
	{
		kontrolli_vasemmalle	= PI_LEFT;
		kontrolli_oikealle		= PI_RIGHT;
		kontrolli_hyppy			= PI_UP;
		kontrolli_alas			= PI_DOWN;
		kontrolli_juoksu		= PI_RALT;
		kontrolli_hyokkays1		= PI_RCONTROL;
		kontrolli_hyokkays2		= PI_RSHIFT;
		kontrolli_kayta_esine	= PI_SPACE;
		menu_lue_kontrollit = 0;
	}

	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_controls_gp4def),100,90+my))
	{
		kontrolli_vasemmalle	= PI_OHJAIN1_VASEMMALLE;
		kontrolli_oikealle		= PI_OHJAIN1_OIKEALLE;
		kontrolli_hyppy			= PI_OHJAIN1_YLOS;
		kontrolli_alas			= PI_OHJAIN1_ALAS;
		kontrolli_juoksu		= PI_OHJAIN1_NAPPI2;
		kontrolli_hyokkays1		= PI_OHJAIN1_NAPPI1;
		kontrolli_hyokkays2		= PI_OHJAIN1_NAPPI3;
		kontrolli_kayta_esine	= PI_OHJAIN1_NAPPI4;
		menu_lue_kontrollit = 0;
	}

	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_controls_gp6def),100,90+my))
	{
		kontrolli_vasemmalle	= PI_OHJAIN1_VASEMMALLE;
		kontrolli_oikealle		= PI_OHJAIN1_OIKEALLE;
		kontrolli_hyppy			= PI_OHJAIN1_YLOS;//PI_OHJAIN1_NAPPI1;
		kontrolli_alas			= PI_OHJAIN1_ALAS;
		kontrolli_juoksu		= PI_OHJAIN1_NAPPI2;
		kontrolli_hyokkays1		= PI_OHJAIN1_NAPPI1;
		kontrolli_hyokkays2		= PI_OHJAIN1_NAPPI4;
		kontrolli_kayta_esine	= PI_OHJAIN1_NAPPI6;
		menu_lue_kontrollit = 0;
	}

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_return),180,400))
	{
		menu_nyt = MENU_PAAVALIKKO;
		menu_lue_kontrollit = 0;
	}

	BYTE k = 0;

	if (key_delay == 0 && menu_lue_kontrollit > 0)
	{
		k = PisteInput_Lue_Kontrolli();

		if (k != 0)
		{
			switch(menu_lue_kontrollit)
			{
				case 1 : kontrolli_vasemmalle	= k;break;
				case 2 : kontrolli_oikealle		= k;break;
				case 3 : kontrolli_hyppy		= k;break;
				case 4 : kontrolli_alas			= k;break;
				case 5 : kontrolli_juoksu		= k;break;
				case 6 : kontrolli_hyokkays1	= k;break;
				case 7 : kontrolli_hyokkays2	= k;break;
				case 8 : kontrolli_kayta_esine	= k;break;
				default: PK_Soita_Aani_Menu(ammuu_aani,100);break;
			}

			key_delay = 20;
			menu_lue_kontrollit++;
		}

		if (menu_lue_kontrollit > 8) {
			menu_lue_kontrollit = 0;
			menu_valittu_id = 1;
		}
	}

	my += 20;

	return 0;
}

int PK_Piirra_Menut_Episodit(){
	int my = 0;

	PK_Piirra_Menu_Nelio(110, 130, 640-110, 450, 224);

	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_episodes_choose_episode),50,90);
	my += 80;

	if (episodi_lkm-2 > 10) {
		char luku[20];
		int vali = 90;
		int x = 50,//500,
			y = 50;//300;
		//vali += PisteDraw2_Font_Write(fontti1,"page:",x,y+40);
		itoa(episodisivu+1,luku,10);
		vali += PisteDraw2_Font_Write(fontti1,luku,x+vali,y+20);
		vali += PisteDraw2_Font_Write(fontti1,"/",x+vali,y+20);
		itoa((episodi_lkm/10)+1,luku,10);
		vali += PisteDraw2_Font_Write(fontti1,luku,x+vali,y+20);

		int nappi = PK_Piirra_Menut_Saatolaatikko(x,y);

		if (nappi == 1 && episodisivu > 0)
			episodisivu--;

		if (nappi == 2 && (episodisivu*10)+10 < episodi_lkm)
			episodisivu++;
	}

	for (int i=(episodisivu*10)+2;i<(episodisivu*10)+12;i++){
		if (strcmp(episodit[i],"") != 0){
			if (PK_Piirra_Menut_Valinta(episodit[i],220,90+my)){
				strcpy(episodi,episodit[i]);
				PK_Lataa_Lisainfot();
				pelin_seuraava_tila = TILA_KARTTA;
				peli_kesken = false;
				PK_Uusi_Peli();
				//PisteDraw2_FadeIn(PD_FADE_NORMAL);
			}
			my += 20;
		}
	}

	/* sivu / kaikki */
	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_return),180,400)){
		menu_nyt = MENU_PAAVALIKKO;
		my += 20;
	}
	PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_episodes_get_more),140,440);

	return 0;
}
//PK_Draw_Language_Menu
int PK_Piirra_Menut_Language(){
	int my = 0;
	int i;
	//char temp[10];

	PK_Piirra_Menu_Nelio(110, 130, 640-110, 450, 224);

	PisteDraw2_Font_Write(fontti2,"select a language:",50,100);


	for (i=0;i<10;i++){
		if(PK_Piirra_Menut_Valinta(langmenulist[i],150,150+my)){
			//printf("Selected %s\n",langmenulist[i]);
			strcpy(asetukset.kieli,langmenulist[i]);
			PK_Lataa_Kieli();
		}
		my += 20;
	}
	my+=180;


	char* next = "next";

	if(totallangs>10){
		if (PK_Piirra_Menut_Valinta(next,530-(strlen(next)*16/*font weight*/),my)){
			if(langlistindex<totallangs-10){

				for(i=0;i<9;i++)
					strcpy(langmenulist[i],langmenulist[i+1]);
				strcpy(langmenulist[9],langlist[langlistindex+10]);
				langlistindex++;
			}

		}
		if (PK_Piirra_Menut_Valinta("previous",130,my)){
			if(langlistindex>0){

				for(i=9;i>0;i--)
					strcpy(langmenulist[i],langmenulist[i-1]);
				strcpy(langmenulist[0],langlist[langlistindex-1]);
				langlistindex--;
			}
		}
	}
	my+=20;
	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_return),130,my)){
		menu_nyt = MENU_PAAVALIKKO;
	}

	return 0;
}
//PK_Draw_Menu
int PK_Piirra_Menut(){
	PisteDraw2_ScreenFill(0);
	PisteDraw2_Image_Clip(kuva_tausta,0,0);

	menu_valinta_id = 1;

	switch (menu_nyt)
	{
	case MENU_PAAVALIKKO	: PK_Piirra_Menut_PaaValikko(); break;
	case MENU_EPISODIT		: PK_Piirra_Menut_Episodit();   break;
	case MENU_GRAFIIKKA		: PK_Piirra_Menut_Grafiikka();  break;
	case MENU_AANET			: PK_Piirra_Menut_Aanet();		break;
	case MENU_KONTROLLIT	: PK_Piirra_Menut_Kontrollit(); break;
	case MENU_NIMI			: PK_Piirra_Menut_Nimi();		break;
	case MENU_LATAA			: PK_Piirra_Menut_Lataa();		break;
	case MENU_TALLENNA		: PK_Piirra_Menut_Tallenna();	break;
	case MENU_LANGUAGE		: PK_Piirra_Menut_Language();	break;
	default					: PK_Piirra_Menut_PaaValikko(); break;
	}

	//PK_Partikkelit_Piirra();

	PK_Piirra_Kursori(hiiri_x,hiiri_y);

	//PisteWait_Wait(0);//10

	//if (PisteDraw2_IsFading())
	//	PisteDraw2_FadeIn(PD_FADE_NORMAL);


	//PisteWait_Start();

	return 0;
}

int PK_Piirra_Nuppi(int x, int y, int t){

	int paluu = 0;

	t = t * 25;

	int vilkku = 50 + (int)(sin_table[degree%360]*2);

	if (vilkku < 0)
		vilkku = 0;

	//PisteDraw2_Image_Clip(kuva_peli,x,y,1+t,58,23+t,80);

	if (hiiri_x > x && hiiri_x < x+17 && hiiri_y > y && hiiri_y < y+17)
	{
		if (key_delay == 0 && (PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE)
													    || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1)))
		{
			key_delay = 30;
			return 2;
		}

		if (t == 25)
			PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 247, 1, 25, 25, x-2, y-2, 60, 96);
		if (t == 0)
			PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 247, 1, 25, 25, x-4, y-4, 60, 32);
		if (t == 50)
			PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 247, 1, 25, 25, x-4, y-4, 60, 64);

		paluu = 1;
	}

	if (t == 25)
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 247, 1, 25, 25, x-2, y-2, vilkku, 96);

	if (((degree/45)+1)%4==0 || t==0)
		PisteDraw2_Image_CutClip(kuva_peli,x,y,1+t,58,23+t,80);

	return paluu;
}
//PK_Draw_Map
int PK_Piirra_Kartta(){
	char luku[20];
	int vali = 20;

	PisteDraw2_ScreenFill(0);
	PisteDraw2_Image_Clip(kuva_tausta,0,0);

	PisteDraw2_Font_Write(fontti4,episodi,100+2,72+2);
	PisteDraw2_Font_Write(fontti2,episodi,100,72);

	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_map_total_score),100+2,92+2);
	vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_map_total_score),100,92);//250,80
	ltoa(pisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,100+vali+2+15,92+2);
	PisteDraw2_Font_Write(fontti2,luku,100+vali+15,92);

	if (episodipisteet.episodin_top_pisteet > 0) {
		vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_map_episode_best_player),360,72);
		PisteDraw2_Font_Write(fontti1,episodipisteet.episodin_top_pelaaja,360+vali+10,72);
		vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_map_episode_hiscore),360,92);
		ltoa(episodipisteet.episodin_top_pisteet,luku,10);
		PisteDraw2_Font_Write(fontti2,luku,360+vali+15,92);
	}

	vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_map_next_level),100,120);
	ltoa(jakso,luku,10);
	PisteDraw2_Font_Write(fontti1,luku,100+vali+15,120);

		//PK_Partikkelit_Piirra();

	if (jaksoja == 0) {
		PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_episodes_no_maps),180,290);
	}

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_return),100,430)){
		pelin_seuraava_tila = TILA_MENUT;
		menu_nyt = MENU_PAAVALIKKO;
	}

	int nuppi_x = 0, nuppi_y = 0;
	int tyyppi = 0;
	int paluu;
	int min = 0, sek = 0;
	int ikoni;
	int sinx = 0, cosy = 0;
	int pekkaframe = 0;


	for (int i=0;i<=jaksoja;i++)
	{
		if (strcmp(jaksot[i].nimi,"")!=0 && jaksot[i].jarjestys > 0)
		{
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

			//PisteDraw2_Image_Clip(kuva_peli,jaksot[i].x-4,jaksot[i].y-4-30,1+(ikoni*27),452,27+(ikoni*27),478);
			PisteDraw2_Image_CutClip(kuva_peli,jaksot[i].x-9,jaksot[i].y-14,1+(ikoni*28),452,28+(ikoni*28),479);

			if (tyyppi==1)
			{
				//PisteDraw2_Image_Clip(kuva_peli,jaksot[i].x-30,jaksot[i].y-4,157,46,181,79);
				sinx = (int)(sin_table[degree%360]/2);
				cosy = (int)(cos_table[degree%360]/2);
				pekkaframe = 28*((degree%360)/120);
				PisteDraw2_Image_CutClip(kuva_peli,jaksot[i].x+sinx-12,jaksot[i].y-17+cosy,157+pekkaframe,46,181+pekkaframe,79);
			}

			paluu = PK_Piirra_Nuppi(jaksot[i].x-5, jaksot[i].y-10, tyyppi);

			// jos klikattu
			if (paluu == 2)
			{
				if (tyyppi < 2 || (huijaukset && tyyppi > 0))//== 1
				{
					strcpy(seuraava_kartta,jaksot[i].tiedosto);
					//jakso = i;
					jakso_indeksi_nyt = i;
					siirry_kartasta_peliin = true;
					PisteDraw2_FadeOut(PD_FADE_SLOW);
					musiikin_voimakkuus = 0;
					PK_Soita_Aani_Menu(kieku_aani,90);
				}
				else
					PK_Soita_Aani_Menu(ammuu_aani,100);
			}

			itoa(jaksot[i].jarjestys,luku,10);
			PisteDraw2_Font_Write(fontti1,luku,jaksot[i].x-12+2,jaksot[i].y-29+2);
			//PisteDraw2_Font_Write(fontti2,luku,jaksot[i].x+3,jaksot[i].y-27);


			if (paluu > 0) {

				int info_x = 489+3, info_y = 341-26;
				/*
				PisteDraw2_ScreenFill(jaksot[i].x-3, jaksot[i].y+26,jaksot[i].x + 130, jaksot[i].y+26+120,1);
				PisteDraw2_ScreenFill(jaksot[i].x-2, jaksot[i].y+27,jaksot[i].x + 129, jaksot[i].y+25+120,43);
				PisteDraw2_ScreenFill(jaksot[i].x-1, jaksot[i].y+28,jaksot[i].x + 128, jaksot[i].y+24+120,38);*/
				PisteDraw2_Image_CutClip(kuva_peli,info_x-3,info_y+26,473,0,607,121);
				PisteDraw2_Font_Write(fontti1,jaksot[i].nimi,info_x,info_y+30);

				if (episodipisteet.pisteet[i] > 0) {
					PisteDraw2_Font_WriteAlpha(fontti1,tekstit->Hae_Teksti(txt_map_level_best_player),info_x,info_y+50,75);
					PisteDraw2_Font_Write(fontti1,episodipisteet.top_pelaajat[i],info_x,info_y+62);
					vali = 8 + PisteDraw2_Font_WriteAlpha(fontti1,tekstit->Hae_Teksti(txt_map_level_hiscore),info_x,info_y+74,75);
					ltoa(episodipisteet.pisteet[i],luku,10);
					PisteDraw2_Font_Write(fontti1,luku,info_x+vali,info_y+75);
				}

				if (episodipisteet.ajat[i] > 0) {
					PisteDraw2_Font_WriteAlpha(fontti1,tekstit->Hae_Teksti(txt_map_level_fastest_player),info_x,info_y+98,75);
					PisteDraw2_Font_Write(fontti1,episodipisteet.nopeimmat_pelaajat[i],info_x,info_y+110);

					vali = 8 + PisteDraw2_Font_WriteAlpha(fontti1,tekstit->Hae_Teksti(txt_map_level_best_time),info_x,info_y+122,75);
					min = episodipisteet.ajat[i]/60;
					sek = episodipisteet.ajat[i]%60;

					itoa(min,luku,10);
					vali += PisteDraw2_Font_Write(fontti1,luku,info_x+vali,info_y+122);
					vali += PisteDraw2_Font_Write(fontti1,":",info_x+vali,info_y+122);
					itoa(sek,luku,10);
					PisteDraw2_Font_Write(fontti1,luku,info_x+vali,info_y+122);
				}
			}
		}
	}

	PK_Piirra_Kursori(hiiri_x,hiiri_y);

	//PisteWait_Wait(0);//10

	//PisteWait_Start();

	return 0;
}

int PK_Piirra_Pistelasku(){
	char luku[20];
	int vali = 20;
	int my = 0, x, y;
	int kuutio, aste;
	int	vari = 0, kerroin;

	PisteDraw2_ScreenFill(0);
	PisteDraw2_Image_Clip(kuva_tausta,0,0);

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
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 247, 1, 25, 25, 320+x, 240+y, kuutio, 32);
	}

	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_title),100+2,72+2);
	PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_title),100,72);
	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_level_score),100+2,102+2);
	vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_level_score),100,102);//250,80
	fake_pisteet = bonuspisteet + aikapisteet + energiapisteet + esinepisteet + pelastuspisteet;
	ltoa(fake_pisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,400+2,102+2);
	PisteDraw2_Font_Write(fontti2,luku,400,102);
	my = 0;

	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_bonus_score),100+2,192+2+my);
	vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_bonus_score),100,192+my);
	ltoa(bonuspisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,400+2,192+2+my);
	PisteDraw2_Font_Write(fontti2,luku,400,192+my);
	my += 30;

	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_time_score),100+2,192+2+my);
	vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_time_score),100,192+my);
	ltoa(aikapisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,400+2,192+2+my);
	PisteDraw2_Font_Write(fontti2,luku,400,192+my);
	my += 30;

	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_energy_score),100+2,192+2+my);
	vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_energy_score),100,192+my);
	ltoa(energiapisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,400+2,192+2+my);
	PisteDraw2_Font_Write(fontti2,luku,400,192+my);
	my += 30;

	PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_item_score),100+2,192+2+my);
	vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_item_score),100,192+my);
	ltoa(esinepisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,400+2,192+2+my);
	PisteDraw2_Font_Write(fontti2,luku,400,192+my);
	my += 30;

	x = 110;
	y = 192 + my;

	for (int i=0;i<MAX_ESINEITA;i++)
		if (esineet[i] != NULL)	{
			esineet[i]->Piirra(x-esineet[i]->leveys/2,y-esineet[i]->korkeus/2,0);
			x += 38;
		}

	my += 10;

	if (pistelaskuvaihe >= 4){
		PisteDraw2_Font_Write(fontti4,tekstit->Hae_Teksti(txt_score_screen_total_score),100+2,192+2+my);
		vali = PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_total_score),100,192+my);//250,80
		ltoa(pisteet,luku,10);
		PisteDraw2_Font_Write(fontti4,luku,400+2,192+2+my);
		PisteDraw2_Font_Write(fontti2,luku,400,192+my);
		my += 25;

		if (jakso_uusi_ennatys) {
			PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_new_level_hiscore),100+rand()%2,192+my+rand()%2);
			my += 25;
		}
		if (jakso_uusi_ennatysaika) {
			PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_new_level_best_time),100+rand()%2,192+my+rand()%2);
			my += 25;
		}
		if (episodi_uusi_ennatys) {
			PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_score_screen_new_episode_hiscore),100+rand()%2,192+my+rand()%2);
			my += 25;
		}
	}

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_score_screen_continue),100,430)){
		musiikin_voimakkuus = 0;
		siirry_pistelaskusta_karttaan = true;
		PisteDraw2_FadeOut(PD_FADE_SLOW);
		//pelin_seuraava_tila = TILA_KARTTA;
	}

	PK_Piirra_Kursori(hiiri_x,hiiri_y);

	return 0;
}
//PK_Draw_Intro_Text
void PK_Piirra_Intro_Teksti(char *teksti, int fontti, int x, int y, DWORD alkuaika, DWORD loppuaika){
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
}
//PK_Draw_Intro
int PK_Piirra_Intro(){
	DWORD pistelogo_alku	= 300;
	DWORD pistelogo_loppu	= pistelogo_alku + 500;
	DWORD tekijat_alku		= pistelogo_loppu + 80;
	DWORD tekijat_loppu		= tekijat_alku + 720;
	DWORD testaajat_alku	= tekijat_loppu + 80;
	DWORD testaajat_loppu	= testaajat_alku + 700;
	DWORD kaantaja_alku		= testaajat_loppu + 100;
	DWORD kaantaja_loppu	= kaantaja_alku + 300;

	PisteDraw2_ScreenFill(0);

	PisteDraw2_Image_CutClip(kuva_tausta,280,80, 280, 80, 640, 480);

	if ((introlaskuri / 10) % 50 == 0)
		PisteDraw2_Image_CutClip(kuva_tausta,353, 313, 242, 313, 275, 432);

	if (introlaskuri > pistelogo_alku && introlaskuri < pistelogo_loppu) {

		//int x = introlaskuri - pistelogo_alku - 194;
		int kerroin = 120 / (introlaskuri - pistelogo_alku);
		int x = 120 - kerroin;

		if (x > 120)
			x = 120;

		PisteDraw2_Image_CutClip(kuva_tausta,/*120*/x,230, 37, 230, 194, 442);

		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_presents), fontti1, 230, 400, pistelogo_alku, pistelogo_loppu-20);

	}

	if (introlaskuri > tekijat_alku) {
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_a_game_by),fontti1, 120, 230, tekijat_alku, tekijat_loppu);
		PK_Piirra_Intro_Teksti("janne kivilahti",			fontti1, 120, 250, tekijat_alku+20, tekijat_loppu+20);
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_original), fontti1, 120, 275, tekijat_alku+40, tekijat_loppu+40);
		PK_Piirra_Intro_Teksti("antti suuronen 1998",		fontti1, 120, 295, tekijat_alku+50, tekijat_loppu+50);
		//PK_Piirra_Intro_Teksti("sdl porting by",		fontti1, 120, 320, tekijat_alku+70, tekijat_loppu+70);
		//PK_Piirra_Intro_Teksti("samuli tuomola 2010",		fontti1, 120, 340, tekijat_alku+80, tekijat_loppu+80);
	}

	if (introlaskuri > testaajat_alku) {
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_tested_by),fontti1, 120, 230, testaajat_alku, testaajat_loppu);
		PK_Piirra_Intro_Teksti("antti suuronen",			fontti1, 120, 250, testaajat_alku+10, testaajat_loppu+10);
		PK_Piirra_Intro_Teksti("toni hurskainen",			fontti1, 120, 260, testaajat_alku+20, testaajat_loppu+20);
		PK_Piirra_Intro_Teksti("juho rytk�nen",				fontti1, 120, 270, testaajat_alku+30, testaajat_loppu+30);
		PK_Piirra_Intro_Teksti("annukka korja",				fontti1, 120, 280, testaajat_alku+40, testaajat_loppu+40);
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_thanks_to),fontti1, 120, 300, testaajat_alku+70, testaajat_loppu+70);
		PK_Piirra_Intro_Teksti("oskari raunio",				fontti1, 120, 310, testaajat_alku+70, testaajat_loppu+70);
		PK_Piirra_Intro_Teksti("assembly organization",		fontti1, 120, 320, testaajat_alku+70, testaajat_loppu+70);
	}

	if (introlaskuri > kaantaja_alku) {
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_translation), fontti1, 120, 230, kaantaja_alku, kaantaja_loppu);
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_translator),  fontti1, 120, 250, kaantaja_alku+20, kaantaja_loppu+20);
	}
	return 0;
}

int PK_Piirra_Loppu_Hahmo(int x, int y, int tyyppi, int plus, int rapytys){
	int frm = 0;
	int yk = 0;

	if (tyyppi == 1){ // Pekka
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

	if (tyyppi == 2){ // kana (katse oikealle)
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

	if (tyyppi == 3){ // kana (katse vasemmalle)
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

	if (tyyppi == 4){ // pikkukana (katse oikealle)
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

	if (tyyppi == 5){ // pikkukana (katse vasemmalle)
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
//PK_Draw_EndGame
int PK_Piirra_Loppu(){

	DWORD onnittelut_alku	= 300;
	DWORD onnittelut_loppu	= onnittelut_alku + 1000;
	DWORD the_end_alku		= onnittelut_loppu + 80;
	DWORD the_end_loppu		= the_end_alku + 3000;

	PisteDraw2_ScreenFill(0);
	PisteDraw2_Image_CutClip(kuva_tausta,320-233/2,240-233/2, 6, 229, 239, 462);

	PK_Piirra_Loppu_Hahmo(345, 244, 3, 30, 2);
	PK_Piirra_Loppu_Hahmo(276, 230, 2, 50, 3);
	PK_Piirra_Loppu_Hahmo(217, 254, 4, 0, 4);

	PK_Piirra_Loppu_Hahmo(305, 240, 1, 0, 1);

	PK_Piirra_Loppu_Hahmo(270, 284, 2, 20, 1);
	PK_Piirra_Loppu_Hahmo(360, 284, 5, 60, 2);

	if (loppulaskuri > onnittelut_alku) {
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_end_congratulations), fontti2, 220, 380, onnittelut_alku, onnittelut_loppu);
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_end_chickens_saved), fontti1, 220, 402, onnittelut_alku+30, onnittelut_loppu+30);
	}
	if (loppulaskuri > the_end_alku) {
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_end_the_end), fontti2, 280, 190, the_end_alku, the_end_loppu);
	}

	return 0;
}

//==================================================
//Main frames
//==================================================

int PK_Main_Intro(){
	PK_Piirra_Intro();

	degree = 1 + degree % 360;

	introlaskuri++;

	if (siirry_introsta_menuun && PisteDraw2_IsFading()){
		pelin_seuraava_tila = TILA_MENUT;
		peli_kesken = false;
	}
	if (key_delay > 0) key_delay--;
	if (key_delay == 0)
		if (PisteInput_Keydown(PI_RETURN) || PisteInput_Keydown(PI_SPACE) || introlaskuri == 2500){
			siirry_introsta_menuun = true;
			PisteDraw2_FadeOut(PD_FADE_SLOW);
		}

	return 0;
}

int PK_Main_Pistelasku(){
	PK_Piirra_Pistelasku();

	degree = 1 + degree % 360;

	// PISTELASKU

	int energia = spritet[pelaaja_index].energia;

	if (pistelaskudelay == 0)
		if (bonuspisteet < jakso_pisteet){
			pistelaskuvaihe = 1;
			pistelaskudelay = 0;
			bonuspisteet += 10;

			if (degree%7==1)
				PK_Soita_Aani_Menu(pistelaskuri_aani, 70);

			if (bonuspisteet >= jakso_pisteet){
				bonuspisteet = jakso_pisteet;
				pistelaskudelay = 50;
			}

		}
		else if (aika > 0){
			pistelaskuvaihe = 2;
			pistelaskudelay = 0;
			aikapisteet+=5;
			aika--;

			if (degree%10==1)
				PK_Soita_Aani_Menu(pistelaskuri_aani, 70);

			if (aika == 0)
				pistelaskudelay = 50;

		}
		else if (spritet[pelaaja_index].energia > 0){
			pistelaskuvaihe = 3;
			pistelaskudelay = 10;
			energiapisteet+=300;
			spritet[pelaaja_index].energia--;

			PK_Soita_Aani_Menu(pistelaskuri_aani, 70);

		}
		else if (esineita > 0){
			pistelaskuvaihe = 4;
			pistelaskudelay = 30;
			for (int i=0;i<MAX_ESINEITA;i++)
				if (esineet[i] != NULL)
				{
					esinepisteet += esineet[i]->pisteet + 500;
					esineet[i] = NULL;
					PK_Soita_Aani_Menu(hyppy_aani, 100);
					break;
				}
		}
		else pistelaskuvaihe = 5;

	if (pistelaskudelay > 0)
		pistelaskudelay--;

	if (siirry_pistelaskusta_karttaan && PisteDraw2_IsFading()){
		/*tarkistetaan oliko viimeinen jakso*/

		if (jakso_indeksi_nyt == EPISODI_MAX_JAKSOJA-1) { // ihan niin kuin joku tekisi n�in monta jaksoa...
			pelin_seuraava_tila = TILA_LOPPU;
		}
		else if (jaksot[jakso_indeksi_nyt+1].jarjestys == -1)
			    pelin_seuraava_tila = TILA_LOPPU;
		else // jos ei niin palataan karttaan...
		{
			pelin_seuraava_tila = TILA_KARTTA;
			//peli_kesken = false;
			menu_nyt = MENU_PAAVALIKKO;
		}
	}

	if (key_delay > 0)
		key_delay--;

	if (key_delay == 0){
		if (PisteInput_Keydown(PI_RETURN) && pistelaskuvaihe == 5){
			siirry_pistelaskusta_karttaan = true;
			musiikin_voimakkuus = 0;
			PisteDraw2_FadeOut(PD_FADE_SLOW);
			key_delay = 20;
		}

		if (PisteInput_Keydown(PI_RETURN) && pistelaskuvaihe < 5){
			pistelaskuvaihe = 5;
			bonuspisteet = jakso_pisteet;
			aikapisteet += aika*5;
			aika = 0;
			energiapisteet += spritet[pelaaja_index].energia * 300;
			spritet[pelaaja_index].energia = 0;
			for (int i=0;i<MAX_ESINEITA;i++)
				if (esineet[i] != NULL)
					esinepisteet += esineet[i]->pisteet + 500;
			esineita = 0;

			key_delay = 20;
		}
	}

	return 0;
}

int PK_Main_Kartta(){
	PK_Piirra_Kartta();

	degree = 1 + degree % 360;

	if (siirry_kartasta_peliin && PisteDraw2_IsFading())
	{
		pelin_seuraava_tila = TILA_PELI;
		//strcpy(seuraava_kartta,jaksot[i].tiedosto);
		peli_kesken = false;//true;
		//PisteDraw2_FadeIn(PD_FADE_NORMAL);
	}

	if (key_delay > 0)
		key_delay--;

	if (key_delay == 0)
	{
		/*
		if (PisteInput_Keydown(PI_SPACE))
		{
			siirry_kartasta_peliin = true;
			//pelin_seuraava_tila = TILA_PELI;
			PisteDraw2_FadeOut(PD_FADE_SLOW);
		}*/
	}

	return 0;
}

int PK_Main_Menut(){

	if (!nimiedit && key_delay == 0 && menu_lue_kontrollit == 0) {
		if (PisteInput_Keydown(PI_UP) || PisteInput_Keydown(PI_LEFT) ||
			PisteInput_Ohjain_X(PI_PELIOHJAIN_1) < 0 || PisteInput_Ohjain_Y(PI_PELIOHJAIN_1) < 0){
			menu_valittu_id--;

			if (menu_valittu_id < 1)
				menu_valittu_id = menu_valinta_id-1;

			key_delay = 15;
		}

		if (PisteInput_Keydown(PI_DOWN) || PisteInput_Keydown(PI_RIGHT) ||
			PisteInput_Ohjain_X(PI_PELIOHJAIN_1) > 0 || PisteInput_Ohjain_Y(PI_PELIOHJAIN_1) > 0){
			menu_valittu_id++;

			if (menu_valittu_id > menu_valinta_id-1)
				menu_valittu_id = 1;

			key_delay = 15;
			//hiiri_y += 3;
		}
	}

	if (nimiedit || menu_lue_kontrollit > 0){
		menu_valittu_id = 0;
	}

	if (menu_nyt != MENU_NIMI){
		nimiedit = false;
	}
	int menu_ennen = menu_nyt;

	PK_Piirra_Menut();

	if (menu_nyt != menu_ennen)
		menu_valittu_id = 1;

	degree = 1 + degree % 360;

	if (tuplanopeus)
		degree = 1 + degree % 360;

	if (key_delay > 0)
		key_delay--;

	return 0;
}

int PK_Main_Peli(){
	PK_Kamera();
	PK_Piirra_Peli();
	PK_Palikka_Paivita_Lasketut_Palikat();

	if (!paused){
		degree = 1 + degree%359;

		if (kytkin1 > 0)
			kytkin1 --;

		if (kytkin2 > 0)
			kytkin2 --;

		if (kytkin3 > 0)
			kytkin3 --;

		if (ilmoitus_ajastin > 0)
			ilmoitus_ajastin--;

		if (piste_lisays > 0)
		{
			jakso_pisteet++;
			piste_lisays--;
		}

		if (aikaraja && !jakso_lapaisty)
		{
			if (sekunti > 0)
			{
				sekunti -= 10;
			}
			else
			{
				sekunti = 1000;

				if (aika > 0)
				{
					aika--;
				}
				else
				{
					peli_ohi = true;
					//PisteDraw2_FadeOut(PD_FADE_SLOW);
				}
			}
		}

		if (nakymattomyys > 0)
			nakymattomyys--;

	}

	if (spritet[pelaaja_index].energia < 1 && !peli_ohi){
		peli_ohi = true;
		key_delay = 50;
	}

	if (key_delay > 0)
		key_delay--;

	if (jakso_lapaisty || peli_ohi){
		if (lopetusajastin > 1)
			lopetusajastin--;

		if (lopetusajastin == 0)
			lopetusajastin = 800;//2000;

		if (PisteInput_Lue_Kontrolli(kontrolli_hyokkays1) || PisteInput_Lue_Kontrolli(kontrolli_hyokkays2) ||
			PisteInput_Lue_Kontrolli(kontrolli_hyppy) || PisteInput_Keydown(PI_RETURN))
			if (lopetusajastin > 2 && lopetusajastin < 600/*1900*/ && key_delay == 0)
				lopetusajastin = 2;

		if (lopetusajastin == 2)
		{
			PisteDraw2_FadeOut(PD_FADE_NORMAL);
			//musiikin_voimakkuus = 0;
		}
	}
	if (lopetusajastin == 1 && PisteDraw2_IsFading()){
		if (jakso_lapaisty) pelin_seuraava_tila = TILA_PISTELASKU;
		else pelin_seuraava_tila = TILA_KARTTA;
	}

	if (key_delay == 0){
		if (PisteInput_Lue_Kontrolli(kontrolli_kayta_esine) && spritet[pelaaja_index].energia > 0){
			PK_Esineet_Kayta();
			key_delay = 10;
		}
		if (PisteInput_Keydown(PI_P) && !jakso_lapaisty){
			paused = !paused;
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_DELETE))
			spritet[pelaaja_index].energia = 0;
		if (PisteInput_Keydown(PI_I)){
			fps_nayta = !fps_nayta;
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_TAB)){
			PK_Esineet_Vaihda();
			key_delay = 10;
		}
	}

	if (key_delay == 0 && huijaukset){ //Debug
		if (PisteInput_Keydown(PI_Z)){
			kytkin1 = KYTKIN_ALOITUSARVO;
			kytkin2 = KYTKIN_ALOITUSARVO;
			kytkin3 = KYTKIN_ALOITUSARVO;
			key_delay = 10;
		}
		if (PisteInput_Keydown(PI_G)){
			asetukset.lapinakyvat_objektit = !asetukset.lapinakyvat_objektit;
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_L)){
			PK_Kartta_Avaa_Lukot();
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_K)){
			PK_Kartta_Vaihda_Kallopalikat();
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_F)){
			fps_nayta = !fps_nayta;
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_I)){
			piirra_infot = !piirra_infot;
			key_delay = 10;
		}
		if (PisteInput_Keydown(PI_R)){
			PK_Kartta_Etsi_Alku();
			spritet[pelaaja_index].energia = 10;
			key_delay = 10;
		}
		if (PisteInput_Keydown(PI_END)){
			if (PisteSound_StartMusic("music/hiscore.xm")!=0)
				PK2_virhe = true;

			key_delay = 20;
			jakso_lapaisty = true;
			jaksot[jakso_indeksi_nyt].lapaisty = true;
			jakso++;
		}
		if (PisteInput_Keydown(PI_E))
			spritet[pelaaja_index].energia = 3;
		if (PisteInput_Keydown(PI_LSHIFT)){
			for (int r=1;r<6;r++)
				PK_Partikkeli_Uusi(PARTIKKELI_KIPINA,spritet[pelaaja_index].x+rand()%10-rand()%10, spritet[pelaaja_index].y+rand()%10-rand()%10,0,0,rand()%100,0.1,32);
			spritet[pelaaja_index] = PK2Sprite(&protot[PROTOTYYPPI_KANA],1,false,spritet[pelaaja_index].x, spritet[pelaaja_index].y);
		}
	}

	return 0;
}

int PK_Main_Loppu(){

	PK_Piirra_Loppu();

	degree = 1 + degree % 360;

	loppulaskuri++;
	introlaskuri = loppulaskuri; // introtekstej� varten

	if (siirry_lopusta_menuun && PisteDraw2_IsFading())
	{
		pelin_seuraava_tila = TILA_MENUT;
		menu_nyt = MENU_PAAVALIKKO;
		peli_kesken = false;
	}

	if (key_delay > 0)
		key_delay--;

	if (key_delay == 0)
	{
		if (PisteInput_Keydown(PI_RETURN) || PisteInput_Keydown(PI_SPACE))
		{
			siirry_lopusta_menuun = true;
			musiikin_voimakkuus = 0;
			PisteDraw2_FadeOut(PD_FADE_SLOW);
		}
	}

	return 0;
}

int PK_Main(){
	PK_Alusta_Tilat();

	if (window_closed/*depr*/){
		running = false;
		return 0;
	}

	MOUSE hiiri = PisteInput_Hiiri();
	hiiri_x = hiiri.x;
	hiiri_y = hiiri.y;

	//Move mouse with keys
	if(pelin_tila != TILA_MENUT){
		hiiri_x += PisteInput_Ohjain_X(PI_PELIOHJAIN_1)/30;
		hiiri_y += PisteInput_Ohjain_Y(PI_PELIOHJAIN_1)/30;

		if (PisteInput_Keydown(PI_LEFT)) hiiri_x -= 3;
		if (PisteInput_Keydown(PI_RIGHT)) hiiri_x += 3;
		if (PisteInput_Keydown(PI_UP)) hiiri_y -= 3;
		if (PisteInput_Keydown(PI_DOWN)) hiiri_y += 3;
	}

	if (hiiri_x < 0) hiiri_x = 0;
	if (hiiri_y < 0) hiiri_y = 0;
	if (hiiri_x > 640-19) hiiri_x = 640-19;
	if (hiiri_y > 480-19) hiiri_y = 480-19;

	switch (pelin_tila){
	case TILA_PELI		: PK_Main_Peli();		break;
	case TILA_MENUT		: PK_Main_Menut();		break;
	case TILA_KARTTA	: PK_Main_Kartta();		break;
	case TILA_PISTELASKU: PK_Main_Pistelasku();	break;
	case TILA_INTRO		: PK_Main_Intro();		break;
	case TILA_LOPPU		: PK_Main_Loppu();		break;
	default				: lopeta_peli = true;	break;
	}

	// MUSIIKIN ��NENS��T�
	bool saada = false;

	if (musiikin_voimakkuus != musiikin_voimakkuus_nyt)
		saada = true;

	if (musiikin_max_voimakkuus > 64)
		musiikin_max_voimakkuus = 64;

	if (musiikin_max_voimakkuus < 0)
		musiikin_max_voimakkuus = 0;

	if (musiikin_voimakkuus > musiikin_max_voimakkuus)
		musiikin_voimakkuus = musiikin_max_voimakkuus;

	if (musiikin_voimakkuus_nyt < musiikin_voimakkuus)
		musiikin_voimakkuus_nyt++;

	if (musiikin_voimakkuus_nyt > musiikin_voimakkuus)
		musiikin_voimakkuus_nyt--;

	if (musiikin_voimakkuus_nyt > 64)
		musiikin_voimakkuus_nyt = 64;

	if (musiikin_voimakkuus_nyt < 0)
		musiikin_voimakkuus_nyt = 0;

	if (saada && asetukset.musiikki)
		PisteSound_SetMusicVolume(musiikin_voimakkuus_nyt);

	if (PisteInput_Keydown(PI_ESCAPE) && key_delay == 0){
		if (menu_nyt != MENU_PAAVALIKKO || pelin_tila != TILA_MENUT){
			pelin_seuraava_tila = TILA_MENUT;
			menu_nyt = MENU_PAAVALIKKO;
			key_delay = 70;
			degree_temp = degree;
		}
		else if (pelin_tila == TILA_MENUT){
			lopeta_peli = true;
			musiikin_voimakkuus = 0;
			PisteDraw2_FadeOut(PD_FADE_FAST);
		}
	}
	if (lopeta_peli && PisteDraw2_IsFading())
		window_closed = true;
	running = !PK2_virhe;
	return 0;
}

int PK_Reload(){

	if (unload) {
		pelin_seuraava_tila = TILA_PERUSALUSTUS;
		pelin_tila = TILA_EI_ALUSTETTU;

		if (PK_Alusta_Tilat() != 0)
			return 1;

		pelin_seuraava_tila = TILA_MENUT;

		//PK_Alusta_Tilat();

		unload = false;
	}
	return 0;
}

int PK_Unload(){
	if (!unload){
		PisteSound_StopMusic();
		delete kartta;
		delete tekstit;
		unload = true;
	}
	return 0;
}

int main(int argc, char *argv[]){

	Piste_Init(RUUDUN_LEVEYS, RUUDUN_KORKEUS, GAME_NAME);
	//Piste_SetFPS(60);

	//Game works in ../res
	chdir("../res");
	strcpy(tyohakemisto,".");

	for(int i=0; i<argc; i++){
		if (strcmp(argv[i], "dev" ) == 0) huijaukset = true;
		if (strcmp(argv[i], "nolimits" ) == 0) RAJAA_KARTANPIIRTOALUE = false;
		if (strcmp(argv[i], "debug" ) == 0);
	}

	PK_Asetukset_Lataa("data/settings.ini");

	tekstit = new PisteLanguage();

	if (!PK_Lataa_Kieli()){
		printf("Could not find %s!\n",asetukset.kieli);
		strcpy(asetukset.kieli,"english.txt");
		if(!PK_Lataa_Kieli()){
			printf("Could not find the default language file!\n");
			return 0;
		}
	}

	PK_Alusta_Tilat();

	if (huijaukset)
		pelin_seuraava_tila = TILA_MENUT;
	else
		pelin_seuraava_tila = TILA_INTRO;

	Piste_Loop(running, *PK_Main);

	if(PK2_virhe)
		printf("Error!\n");

	PK_Asetukset_Tallenna("data/settings.ini");

	PK_Unload();
	Piste_Quit();

	return 0;
}
