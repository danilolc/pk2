/* Pekka Kana 2 v1.2 

Muutoshistoria:

v1.2

- episodiruudun jakaminen useaan osaan: 10 episodia per ruutu
- Musiikin lataus myös episodihakemistosta.
- Kartan musiikin lataus myös episodihakemistosta
- Spritejen lataus episodihakemistosta
- Lumiefekti
- episodit aakkosjärjestyksessä
- Esineiden swappaaminen tabista
- Pistelaskuruudussa laskennan skippaus ENTER:llä
- Max spriteja 800
- Fonttien lataus erillisestä tiedostosta, joka on määritelty kielitiedostossa
- Datan luku kartan ulkopuolelta -bugi korjattu
- Optimoidut spriterutiinit.
- Lokin kirjoitus lisäys
- Scrollbar kielivalinnoissa
- menujen selaus näppäimistöllä ja gamepadilla
- Lisätty spriteille tekoälyjä
- Staattinen alue, jolla spritet aktiivisia (-320 -> +320 : -240 -> +240)
- Pisteidenlaskentabugi fixattu
- näkymättömyysbonus
- uudelleen ilmestyvät spritet
- nuolispritet
- alkup. infojen korvaus infosign.txt -tiedostotsta

*/


/* PRE DEFINITIONS ----------------------------------------------------------------------------*/

#define WIN32_LEAN_AND_MEAN  
#define INITGUID
//#define NDEBUG

/* INCLUDES -----------------------------------------------------------------------------------*/

#include <tchar.h>
#include <windows.h>   
#include <windowsx.h> 
#include <fstream.h>
#include <mmsystem.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <direct.h>
#include <io.h>
#include "D:\Visual Studio\MyProjects\PisteEngine\PisteInput.h"
#include "D:\Visual Studio\MyProjects\PisteEngine\PisteDraw.h"
#include "D:\Visual Studio\MyProjects\PisteEngine\PisteWait.h"
#include "D:\Visual Studio\MyProjects\PisteEngine\PisteSound.h"
#include "D:\Visual Studio\MyProjects\PisteEngine\PisteLog.h"
#include "D:\Visual Studio\MyProjects\PisteLanguage\PisteLanguage.h"
#include "PK2Map.h"
#include "PK2Sprite.h"
#include "resource.h"
#include "D:\C\midas\include\midasdll.h"


/* TYYPPIMÄÄRITTELYT ---------------------------------------------------------------------------*/

typedef unsigned short USHORT;
typedef unsigned short WORD; 
typedef unsigned long  DWORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

// ******* H U I J A U K S E T *****************

bool huijaukset = false;

// ******* H U I J A U K S E T *****************

const int MAX_SPRITEJA = 800;//600;
//const int MAX_PROTOTYYPPEJA = 100;
const int MAX_ESINEITA = 4;
const int MAX_TALLENNUKSIA = 10;

const UCHAR PALIKKA_TAUSTA		= 0;
const UCHAR PALIKKA_SEINA		= 1;
const UCHAR PALIKKA_MAKI_OIKEA_YLOS = 2;
const UCHAR PALIKKA_MAKI_VASEN_YLOS = 3;
const UCHAR PALIKKA_MAKI_OIKEA_ALAS = 4;
const UCHAR PALIKKA_MAKI_VASEN_ALAS = 5;
const UCHAR PALIKKA_MAKI_YLOS = 6;
const UCHAR PALIKKA_MAKI_ALAS = 7;

const UCHAR PALIKKA_MAX_MASKEJA = 150;

const UCHAR PARTIKKELI_EI_MIKAAN= 0;
const UCHAR PARTIKKELI_TAHTI	= 1;
const UCHAR PARTIKKELI_HOYHEN	= 2;
const UCHAR PARTIKKELI_POLYPILVI= 3;
const UCHAR PARTIKKELI_VALO		= 4;
const UCHAR PARTIKKELI_KIPINA	= 5;
const UCHAR PARTIKKELI_PISTE	= 6;
const UCHAR PARTIKKELI_SAVUPILVI= 7;
const UCHAR PARTIKKELI_TAHTI2	= 8;

const UCHAR TAUSTAPARTIKKELI_VESIPISARA = 1;
const UCHAR TAUSTAPARTIKKELI_LEHTI1		= 2;
const UCHAR TAUSTAPARTIKKELI_LEHTI2		= 3;
const UCHAR TAUSTAPARTIKKELI_LEHTI3		= 4;
const UCHAR TAUSTAPARTIKKELI_LEHTI4		= 5;
const UCHAR TAUSTAPARTIKKELI_HIUTALE1	= 6;
const UCHAR TAUSTAPARTIKKELI_HIUTALE2	= 7;
const UCHAR TAUSTAPARTIKKELI_HIUTALE3	= 8;
const UCHAR TAUSTAPARTIKKELI_HIUTALE4	= 9;

const int MAX_PARTIKKELEITA = 300;//200;
const int MAX_TAUSTAPARTIKKELEITA = 200;
const int MAX_FADETEKSTEJA  = 40;

const int EPISODI_MAX_JAKSOJA = 50;
const int MAX_EPISODEJA	= 300;

const int MAX_ILMOITUKSENNAYTTOAIKA = 700;

const int TILA_EI_ALUSTETTU = 0;
const int TILA_PERUSALUSTUS = 1;
const int TILA_INTRO = 2;
const int TILA_MENUT = 3;
const int TILA_KARTTA= 4;
const int TILA_PELI  = 5;
const int TILA_PISTELASKU = 6;
const int TILA_LOPPU = 7;

const UCHAR MENU_PAAVALIKKO = 0;
const UCHAR MENU_EPISODIT   = 1;
const UCHAR MENU_KONTROLLIT = 2;
const UCHAR MENU_GRAFIIKKA  = 3;
const UCHAR MENU_AANET      = 4;
const UCHAR MENU_NIMI		= 5;
const UCHAR MENU_LATAA		= 6;
const UCHAR MENU_TALLENNA	= 7;

struct PK2JAKSO
{
	char	tiedosto[_MAX_PATH];
	char	nimi[40];
	int		x,y;
	int		jarjestys;
	bool	lapaisty;
	int		ikoni;
};

struct PK2PALIKKA
{
	UCHAR		koodi;
	bool		tausta;
	UCHAR		vasemmalle, oikealle, ylos, alas;
	int			vasen, oikea, yla, ala;
	bool		vesi;
	bool		reuna;
};

struct PK2PALIKKAMASKI
{
	short int	ylos[32];
	short int	alas[32];
	short int	vasemmalle[32];
	short int	oikealle[32];
};

struct PK2PARTIKKELI
{
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

struct PK2FADETEKSTI
{
	char teksti[20];
	int fontti;
	int x,y,ajastin;
};

struct PK2TALLENNUS
{
	int   jakso;
	char  episodi[_MAX_PATH];
	char  nimi[20];
	bool  kaytossa;
	bool  jakso_lapaisty[EPISODI_MAX_JAKSOJA];
	DWORD pisteet;
};

struct PK2ASETUKSET
{
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
	UCHAR  kontrolli_vasemmalle;
	UCHAR  kontrolli_oikealle;
	UCHAR  kontrolli_hyppy;
	UCHAR  kontrolli_alas;
	UCHAR  kontrolli_juoksu;
	UCHAR  kontrolli_hyokkays1;
	UCHAR  kontrolli_hyokkays2;
	UCHAR  kontrolli_kayta_esine;
	// audio
	bool musiikki;
	bool aanet;
};

struct PK2EPISODIPISTEET
{
	// parhaat pisteet kullekin jaksolle episodissa..
	DWORD pisteet[EPISODI_MAX_JAKSOJA];
	// eniten pisteitä keränneet pelaajat kussakin jaksossa episodissa..
	char top_pelaajat[EPISODI_MAX_JAKSOJA][20];
	// parhaat ajat kullekin jaksolle episodissa..
	DWORD ajat[EPISODI_MAX_JAKSOJA];
	// nopeimmat pelaajat kussakin jaksossa episodissa..
	char nopeimmat_pelaajat[EPISODI_MAX_JAKSOJA][20];

	DWORD episodin_top_pisteet;
	char  episodin_top_pelaaja[20];
};

/* MÄÄRITTELYT --------------------------------------------------------------------------------*/

#define WINDOW_CLASS_NAME "WINCLASS1"
#define	GAME_NAME		  "PEKKA KANA 2 (v1.2)"

// 320x240 | 512x384 | 640x480 | 800x600 | 1024x768 | 1152x864 | 1280x1024

int RUUDUN_LEVEYS				= 800;	//640
int RUUDUN_KORKEUS				= 600;	//480
const int RUUDUN_BPP			= 8;	
const int MAX_COLORS_PALETTE	= 256;	

int MARGINAALI_HORI				= 0;
int MARGINAALI_VERT				= 0;

const int AANET_STEREO			= 1;
const int AANET_SAMPLERATE		= 22050;
const int AANET_BITRATE			= 8;

int KARTANPIIRTO_LEVEYS   = 800;
int KARTANPIIRTO_KORKEUS  = 480;
bool RAJAA_KARTANPIIRTOALUE = true;

/* GLOBAALIT MUUTTUJAT ---------------------------------------------------------------------------*/

HWND      ikkunan_kahva			= NULL; // pääikkunan kahva
HINSTANCE hinstance_app			= NULL; // hinstance?
HDC       global_dc				= NULL; // global dc?

bool ikkuna_aktiivinen		= true;		// onko ikkuna aktiivinen vai minimoitu?

bool PK2_virhe				= false;// jos tämä muuttuu todeksi niin ohjelma lopetetaan
char PK2_virhe_viesti[2]	= " ";  // ei vielä (kunnolla) käytössä

bool window_closed				= false;// onko ikkuna kiinni
bool lopeta_peli = false;

bool unload = false;
bool taulut_laskettu = false;

// ASETUKSET

PK2ASETUKSET asetukset;

// ALKU-SETUPIN MUUTTUJAT

HWND	hSetupIkkuna,
		hRB_640x480,
		hRB_800x600,
		hRB_1024x768,
		hBT_Play,
		hBT_Music,
		hBT_Exit,
		hCB_Music,
		hCB_SFX,
		hCMB_Languages;
		
const int	ID_RB		  = 100,
			ID_BT_Play	  = 500,
			ID_BT_Music   = 600,
			ID_BT_Exit    = 700,
			ID_CB_Music   = 800,
			ID_CB_SFX     = 900,
			ID_CMB_Languages = 1000;

bool alkusetup = true;

// INFO MUUTTUJAT

bool	piirra_infot = false;
int		info_spriteja = 0;
int		info_spriteja_piirretty = 0;
int		info_spriteja_aktiivisena = 0;

char	viesti[60];

// KARTTA

PK2Kartta *kartta;
char seuraava_kartta[_MAX_PATH];

// SPRITET JA NIIDEN PROTOTYYPIT

PK2Sprite_Prototyyppi protot[MAX_PROTOTYYPPEJA];

PK2Sprite spritet[MAX_SPRITEJA];

int taustaspritet[MAX_SPRITEJA];

int seuraava_vapaa_proto = 0;

PK2Sprite_Prototyyppi *esineet[MAX_ESINEITA];
int esineita = 0;

// PALIKAT JA MASKIT

PK2PALIKKA	palikat[300];

PK2PALIKKA	lasketut_palikat[150];//150

PK2PALIKKAMASKI palikkamaskit[PALIKKA_MAX_MASKEJA];

// PARTIKKELIT

PK2PARTIKKELI partikkelit[MAX_PARTIKKELEITA];
int partikkeli_index = 0;

PK2PARTIKKELI taustapartikkelit[MAX_TAUSTAPARTIKKELEITA];
int taustapartikkeli_index = 0;

// FADE-TEKSTIT
PK2FADETEKSTI fadetekstit[MAX_FADETEKSTEJA];
int fadeteksti_index = 0;

// KUVABUFFERIT
int  kuva_peli;
int  kuva_peli_sysmem;
int  kuva_tausta;

// FONTIT
int fontti1;
int fontti2;
int fontti3;
int fontti4;
int fontti5;

// KONTROLLIT
int hiiri_x = 10;
int hiiri_y = 10;
double menukursori_x = RUUDUN_LEVEYS / 2;
double menukursori_y = RUUDUN_KORKEUS / 2;
double menukursori_xk = 0;
double menukursori_yk = 0;

UCHAR  kontrolli_vasemmalle		= PI_KB_LEFT;
UCHAR  kontrolli_oikealle		= PI_KB_RIGHT;
UCHAR  kontrolli_hyppy			= PI_KB_UP;
UCHAR  kontrolli_alas			= PI_KB_DOWN;
UCHAR  kontrolli_juoksu			= PI_KB_RALT;
UCHAR  kontrolli_hyokkays1		= PI_KB_RCONTROL;
UCHAR  kontrolli_hyokkays2		= PI_KB_RSHIFT;
UCHAR  kontrolli_kayta_esine	= PI_KB_SPACE;

int key_delay = 0;

// KAMERAN KORDINAATIT

int kamera_x = 0;
int kamera_y = 0;
double dkamera_x = 0;
double dkamera_y = 0;
double dkamera_a = 0;
double dkamera_b = 0;

// JAKSO JA EPISODI

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

// PELAAJA

int pelaaja_index = 0;
DWORD pisteet = 0;
DWORD piste_lisays = 0;
char pelaajan_nimi[20] = " ";

bool nimiedit = false;

// PALIKOIHIN LIITTYVÄT AJASTIMET

int kytkin1 = 0, kytkin2 = 0, kytkin3 = 0;
int palikka_animaatio = 0;

// ÄÄNIEFEKTIT

int kytkin_aani,
	hyppy_aani,
	loiskahdus_aani,
	avaa_lukko_aani,
	menu_aani,
	ammuu_aani,
	kieku_aani,
	tomahdys_aani,
	pistelaskuri_aani;


int sprite_aanet[50]; // spritejen käyttämät äänibufferit

int aanenvoimakkuus = 90;

// MUSIIKKI
int   musiikin_voimakkuus = 64;
int   musiikin_max_voimakkuus = 64;
int   musiikin_voimakkuus_nyt = 64;

// TAUSTAKUVA

//int tausta_extra;

// TALLENNUKSET

PK2TALLENNUS tallennukset[MAX_TALLENNUKSIA];
int lataa_peli = -1;


// MUUTA

double cos_table[360];
double sin_table[360];
UCHAR  varitaulu31[256];
UCHAR  varitaulu_harmaa[256];
UCHAR  pros_taulu[32][100];
UCHAR  varit_taulu[256][256][100];

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

// PISTEIDEN LASKEMINEN

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

// PELIN MUUTTUJAT

char tyohakemisto[_MAX_PATH];
bool pause = false;
int pelin_tila = TILA_EI_ALUSTETTU;
int pelin_seuraava_tila = TILA_PERUSALUSTUS;
bool peli_kesken = false;
bool siirry_kartasta_peliin = false;
bool siirry_pistelaskusta_karttaan = false;

int nakymattomyys = 0;


// INTRO
DWORD introlaskuri = 0;
bool siirry_introsta_menuun = false;

// LOPPURUUTU
DWORD loppulaskuri = 0;
bool siirry_lopusta_menuun = false;

// GRAFIIKKA

bool tuplanopeus = false;
bool skip_frame = false;


// MENUT

int menu_nyt = MENU_PAAVALIKKO;
int menu_lue_kontrollit = 0;
int menu_nimi_index = 0;
char menu_nimi_ed_merkki = '\0';
int menu_valittu_id = 0;
int menu_valinta_id = 1;

RECT menunelio;

// MIDAS

MIDASmodule module;
MIDASmodulePlayHandle playHandle;
bool MIDASvirhe   = false;
bool modi_ladattu = false;
bool musiikki_soi = false;
char nykyinen_kappale[255] = "";
UCHAR musiikki_max_voimakkuus = 64;

// FRAMERATE

int   fps = 0;
int   fps_laskuri = 0;
DWORD fps_aika = 0;
bool  fps_nayta = false;

// KIELI JA PELIN TEKSTIT

PisteLanguage *tekstit;

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




/* PROTOTYYPIT --------------------------------------------------------------------------------*/

int  PK_Kartta_Aseta_Spritet();
void PK_Esineet_Alusta();
void PK_Lisaa_Episodin_Hakemisto(char *tiedosto);

/* PELI ---------------------------------------------------------------------------------------*/

bool PK_Onko_File(char *filename)
{
	ifstream *tiedosto = new ifstream(filename, ios::binary | ios::nocreate);
	
	if (tiedosto->fail())
	{
		delete (tiedosto);
		return false;
	}
	
	delete (tiedosto);
	return true;
}

// ASETUKSET

void PK_Asetukset_Alusta()
{
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

	asetukset.kontrolli_vasemmalle		= PI_KB_LEFT;
	asetukset.kontrolli_oikealle		= PI_KB_RIGHT;
	asetukset.kontrolli_hyppy			= PI_KB_UP;
	asetukset.kontrolli_alas			= PI_KB_DOWN;
	asetukset.kontrolli_juoksu			= PI_KB_RALT;
	asetukset.kontrolli_hyokkays1		= PI_KB_RCONTROL;
	asetukset.kontrolli_hyokkays2		= PI_KB_RSHIFT;
	asetukset.kontrolli_kayta_esine		= PI_KB_SPACE;
}

int PK_Asetukset_Lataa(char *filename)
{
	PisteLog_Kirjoita("- Trying to load settings from a file.\n");

	ifstream *tiedosto = new ifstream(filename, ios::binary | ios::nocreate);
	char versio[4];
	
	if (tiedosto->fail())
	{
		PisteLog_Kirjoita("  - No setting file found. Creating default settings.\n");
		delete (tiedosto);
		PK_Asetukset_Alusta();
		return 1;
	}

	tiedosto->read ((char *)versio, 4);

	if (strcmp(versio,"1.0") == 0)
	{
		PisteLog_Kirjoita("- Found a version 1.0 settings file.\n");
		tiedosto->read ((char *)&asetukset, sizeof (asetukset));
	}

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

int PK_Asetukset_Tallenna(char *filename)
{
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

// PISTEET

void PK_Episodipisteet_Alusta()
{
	for (int i=0;i<EPISODI_MAX_JAKSOJA;i++) {
		episodipisteet.pisteet[i] = 0;
		episodipisteet.ajat[i] = 0;
		strcpy(episodipisteet.top_pelaajat[i]," ");
		strcpy(episodipisteet.nopeimmat_pelaajat[i]," ");
	}

	episodipisteet.episodin_top_pisteet = 0;
	strcpy(episodipisteet.episodin_top_pelaaja," ");
}

int PK_Episodipisteet_Vertaa(int jakso, DWORD episteet, DWORD aika, bool loppupisteet)
{
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

int PK_Episodipisteet_Lataa(char *filename)
{
	PK_Lisaa_Episodin_Hakemisto(filename);
	
	ifstream *tiedosto = new ifstream(filename, ios::binary | ios::nocreate);
	char versio[4];
	
	if (tiedosto->fail())
	{
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

int PK_Episodipisteet_Tallenna(char *filename)
{
	PK_Lisaa_Episodin_Hakemisto(filename);
	
	ofstream *tiedosto = new ofstream(filename, ios::binary);
	tiedosto->write ("1.0", 4);
	tiedosto->write ((char *)&episodipisteet, sizeof (episodipisteet));
	delete (tiedosto);
	return 0;	
}

// KIELI JA TEKSTIT

void PK_Lataa_Lisainfot()
{
	PisteLanguage *temp = new PisteLanguage();
	char infofile[_MAX_PATH] = "infosign.txt";
	int indeksi1 = 0;
	int indeksi2 = 0;
	char otsikko[255];

	PK_Lisaa_Episodin_Hakemisto(infofile);

	if (PK_Onko_File(infofile)) {
		if (temp->Lue_Tiedosto(infofile)) {

			for (int i = 0;i<19; i++) {
				switch(i+1){
				case 1:  strcpy(otsikko,"info01");break;
				case 2:  strcpy(otsikko,"info02");break;
				case 3:  strcpy(otsikko,"info03");break;
				case 4:  strcpy(otsikko,"info04");break;
				case 5:  strcpy(otsikko,"info05");break;
				case 6:  strcpy(otsikko,"info06");break;
				case 7:  strcpy(otsikko,"info07");break;
				case 8:  strcpy(otsikko,"info08");break;
				case 9:  strcpy(otsikko,"info09");break;
				case 10: strcpy(otsikko,"info10");break;
				case 11: strcpy(otsikko,"info11");break;
				case 12: strcpy(otsikko,"info12");break;
				case 13: strcpy(otsikko,"info13");break;
				case 14: strcpy(otsikko,"info14");break;
				case 15: strcpy(otsikko,"info15");break;
				case 16: strcpy(otsikko,"info16");break;
				case 17: strcpy(otsikko,"info17");break;
				case 18: strcpy(otsikko,"info18");break;
				case 19: strcpy(otsikko,"info19");break;
				default: break;
				}

				if ((indeksi1 = tekstit->Hae_Indeksi(otsikko)) != -1 && 
					(indeksi2 = temp->Hae_Indeksi(otsikko)) != -1) {
					tekstit->Korvaa_Teksti(indeksi1,temp->Hae_Teksti(indeksi2));
				}
			}
		}
	}

	delete (temp);
}

bool PK_Lataa_Kieli(char *tiedosto)
{
	PisteLog_Kirjoita("- Trying to load a language file: ");
	PisteLog_Kirjoita(tiedosto);
	PisteLog_Kirjoita(" \n");
	
	if (!tekstit->Lue_Tiedosto(tiedosto))
	{
		PisteLog_Kirjoita("  - Loading file failed.\n");
		PK2_virhe = true;
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

	// Päävalikko
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

	PisteLog_Kirjoita("  - Loading file completed.\n");

	return true;
}


// MIDAS

int PK_Midas_Alusta()
{
	if (asetukset.musiikki)
	{
		PisteLog_Kirjoita("  - MIDASstartup \n");
		MIDASstartup();
	
		PisteLog_Kirjoita("  - MIDASloadConfig(\"data\\msetup.ini\") \n");
		if (!MIDASloadConfig("data\\msetup.ini")) {
			PisteLog_Kirjoita("  - data\\msetup.ini not found, showing MIDASconfig window \n");
			if ( !MIDASconfig() )
			{
				PisteLog_Kirjoita("  - MIDASconfig failed! \n");
				MIDASvirhe = true;			
				return (1);
			}
			else {
				PisteLog_Kirjoita("  - MIDASsaveConfig \n");
				MIDASsaveConfig("data\\msetup.ini");
			}
		}

	
		MIDASsetOption(MIDAS_OPTION_DSOUND_HWND, (DWORD) ikkunan_kahva);
		MIDASsetOption(MIDAS_OPTION_DSOUND_MODE, MIDAS_DSOUND_FORCE_STREAM);
		MIDASsetOption(MIDAS_OPTION_DSOUND_OBJECT, (DWORD) PisteSound_Get_DirectSound());
		MIDASinit();
		MIDASstartBackgroundPlay(0);
	}

	return 0;
}

void PK_Midas_Lopeta()
{
	if (asetukset.musiikki)
	{
		if (/*!MIDASvirhe*/true)
		{
			if (modi_ladattu)
			{
				if (musiikki_soi)
					MIDASstopModule(playHandle);
				MIDASfreeModule(module);
			}
		}

		MIDASstopBackgroundPlay();

		MIDASclose();

	}
}

int PK_Midas_Pysayta()
{
	if (asetukset.musiikki)
	{
		if (musiikki_soi)
		{
			if (playHandle != NULL)
				MIDASstopModule(playHandle);
		
			musiikki_soi = false;
		}	
	}
	
	return 0;
}

int PK_Midas_Soita()
{
	if (asetukset.musiikki)
	{

		if (!musiikki_soi)
		{
			playHandle = MIDASplayModule(module, TRUE);
			musiikki_soi = true;
		}	
	}
	
	return(0);
}

int PK_Midas_Lataa_Musiikki(char *filename)
{
	PisteLog_Kirjoita("  - MIDAS: Loading music file: ");
	PisteLog_Kirjoita(filename);
	PisteLog_Kirjoita("\n");

	if (asetukset.musiikki) {
		if (!MIDASvirhe && strcmp(filename,nykyinen_kappale) != 0) {
			
			if (!PK_Onko_File(filename)) {
				PisteLog_Kirjoita("  - File not found. \n");
				return 2;
			}
			
			strcpy(nykyinen_kappale, filename);
			
			if (modi_ladattu) {
				PK_Midas_Pysayta();
				MIDASfreeModule(module);
				modi_ladattu = false;
			}
			
			if (!(module = MIDASloadModule(filename))){
				PisteLog_Kirjoita("  - Loading file failed! \n");
				return 1;
			}
			
			playHandle = MIDASplayModule(module, TRUE);
			modi_ladattu = true;
			musiikki_soi = true;

			if (!(MIDASsetMusicVolume(playHandle, musiikin_max_voimakkuus))){
				PisteLog_Kirjoita("  - Setting music volume failed! \n");
				return 1;
			}
		}
	}
	return(0);
}

void PK_Lisaa_Tyohakemisto(char *tiedosto)
{
	char uusi_tiedosto[255];
	
	strcpy(uusi_tiedosto, tyohakemisto);
	strcat(uusi_tiedosto, "\\");
	strcat(uusi_tiedosto, tiedosto);
	strcpy(tiedosto, uusi_tiedosto); 	

}

void PK_Lisaa_Episodin_Hakemisto(char *tiedosto)
{
	char uusi_tiedosto[255];

	strcpy(uusi_tiedosto, tyohakemisto);
	strcat(uusi_tiedosto, "\\episodes\\");
	strcat(uusi_tiedosto, episodi);
	strcat(uusi_tiedosto, "\\");
	strcat(uusi_tiedosto, tiedosto);
	strcpy(tiedosto, uusi_tiedosto);
}

void PK_Uusi_Peli()
{
	pisteet = 0;
	jakso = 1;
}

void PK_Uusi_Jakso()
{
	aika = kartta->aika;

	if (aika > 0)
		aikaraja = true;
	else
		aikaraja = false;

	lopetusajastin = 0;

	sekunti = 1000;
	jakso_pisteet = 0;//pisteet;
	peli_ohi = false;
	jakso_lapaisty = false;
	kytkin1 = 0;
	kytkin2 = 0;
	kytkin3 = 0;
	kytkin_tarina = 0;
	jaristys = 0;

	pause = false;

	ilmoitus_ajastin = 0;

	nakymattomyys = 0;
}

void PK_Jaksot_Alusta()
{
	for (int i=0;i<EPISODI_MAX_JAKSOJA;i++)
	{
		strcpy(jaksot[i].nimi,"");
		strcpy(jaksot[i].tiedosto,"");
		jaksot[i].x = 0;
		jaksot[i].y = 0;
		jaksot[i].jarjestys = -1;
		jaksot[i].lapaisty = false;
		jaksot[i].ikoni = 0;
	}
}

void PK_Jaksot_Hae()
{
	char hakemisto[_MAX_PATH];
	PK2Kartta *temp = new PK2Kartta();
	strcpy(hakemisto,"");
	PK_Lisaa_Episodin_Hakemisto(hakemisto);
	strcat(hakemisto,"*.map");

    struct _finddata_t map_file;
    long hFile;
	
	int i=0;

	if((hFile = _findfirst(hakemisto, &map_file )) == -1L )
       strcpy(viesti,"no maps found");
	else
	{
		strcpy(jaksot[i].tiedosto, map_file.name);
		i++;
	}

	while (i<EPISODI_MAX_JAKSOJA && _findnext( hFile, &map_file ) == 0)
	{
		strcpy(jaksot[i].tiedosto, map_file.name);
		i++;
	}

	_findclose( hFile );

	strcpy(hakemisto,"");
	PK_Lisaa_Episodin_Hakemisto(hakemisto);

	jaksoja = i;

	for (i=0;i<=jaksoja;i++)
	{
		if (temp->Lataa_Pelkat_Tiedot(hakemisto,jaksot[i].tiedosto) == 0)
		{
			strcpy(jaksot[i].nimi, temp->nimi);
			jaksot[i].x = temp->x;//   142 + i*35;
			jaksot[i].y = temp->y;// 270;
			jaksot[i].jarjestys = temp->jakso;
			jaksot[i].ikoni = temp->ikoni;
		}
	}

	PK2JAKSO jakso;

	bool lopeta = false;

	while (!lopeta)
	{
		lopeta = true;

		for (i=0;i<jaksoja;i++)
		{
			if (jaksot[i].jarjestys > jaksot[i+1].jarjestys )
			{
				jakso = jaksot[i];
				jaksot[i] = jaksot[i+1];
				jaksot[i+1] = jakso;
				lopeta = false;
			}
		}
	}
	delete temp;
}

int PK_Episodit_Vertaa(char *a, char *b)
{
	int apituus = strlen(a);
	int bpituus = strlen(b);
	int looppi = apituus;

	if (bpituus < apituus)
		looppi = bpituus;

	strlwr(a);
	strlwr(b);

	for (int i=0;i<looppi;i++)
	{
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

int PK_Episodit_Aakkosta(void)
{
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

int PK_Episodit_Hae()
{
	for (int i=0;i<MAX_EPISODEJA;i++)
		strcpy(episodit[i],"");
	
	char hakemisto[_MAX_PATH];
	strcpy(hakemisto,"episodes\\");
	strcat(hakemisto,"*.");

    struct _finddata_t map_file;
    long hFile;

	i = 0;

	episodi_lkm = 0;

	if((hFile = _findfirst(hakemisto, &map_file )) == -1L )
       return 1;
	else
	{
		strcpy(episodit[i], map_file.name);
		i++;
	}

	while (i<MAX_EPISODEJA && _findnext( hFile, &map_file ) == 0)
	{
		strcpy(episodit[i], map_file.name);
		i++;
	}

	_findclose( hFile );

	episodi_lkm = i - 2;

	PK_Episodit_Aakkosta();

	return 0;
}

int PK_Tallennukset_Tyhjenna(void)
{
	for (int i = 0;i < MAX_TALLENNUKSIA;i++)
	{
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

int PK_Tallennukset_Hae_Kaikki(char *filename)
{
	char versio[2];
	char lkmc[8];
	int lkm, i;
	 
	ifstream *tiedosto = new ifstream(filename, ios::binary);
	
	if (tiedosto->fail())
	{
		delete (tiedosto);
		PK_Tallennukset_Tyhjenna();
		return 1;
	}

	PK_Tallennukset_Tyhjenna();	

	tiedosto->read(versio,	sizeof(versio));

	if (strcmp(versio,"1")==0)
	{
		tiedosto->read(lkmc, sizeof(lkmc));
		lkm = atoi(lkmc);

		for (i=0;i<lkm;i++)
			tiedosto->read ((char *)&tallennukset[i], sizeof (tallennukset[i]));
	}

	delete (tiedosto);

	return 0;
}

int PK_Tallennukset_Tallenna_Kaikki(char *filename)
{
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

int PK_Tallennukset_Lataa(int i)
{
	if (strcmp(tallennukset[i].episodi," ")!=0)
	{
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

int PK_Tallennukset_Tallenna(int i)
{
	tallennukset[i].kaytossa = true;
	strcpy(tallennukset[i].episodi, episodi);
	strcpy(tallennukset[i].nimi,pelaajan_nimi);
	tallennukset[i].jakso = jakso;
	tallennukset[i].pisteet = pisteet;
	
	for (int j = 0;j < EPISODI_MAX_JAKSOJA;j++) 
		tallennukset[i].jakso_lapaisty[j] = jaksot[j].lapaisty;	

	PK_Tallennukset_Tallenna_Kaikki("data\\saves.dat");

	return 0;
}

void PK_Soita_Aani(int aani, int voimakkuus, int x, int y, int freq, bool random_freq)
{
	if (aani > -1 && aanenvoimakkuus > 0 && voimakkuus > 0)
	{
		if (x < kamera_x+RUUDUN_LEVEYS && x > kamera_x && y < kamera_y+RUUDUN_KORKEUS && y > kamera_y)
		{
			voimakkuus = voimakkuus / (100 / aanenvoimakkuus);

			if (voimakkuus > 100)
				voimakkuus = 100;

			if (voimakkuus < 0)
				voimakkuus = 0;			

			int pan = kamera_x + (RUUDUN_LEVEYS / 2) - x;

			if (random_freq)
				freq = freq + rand()%4000 - rand()%2000;

			//if (PisteSound_SFX_Soita(aani,aanenvoimakkuus) == PS_VIRHE)
			//	PK2_virhe = true;
			
			if (PisteSound_SFX_Soita(aani,aanenvoimakkuus, pan*-2, freq) == PS_VIRHE)
				PK2_virhe = true;
		}
	}
}

void PK_Soita_Aani_Menu(int aani, int voimakkuus)
{
	if (aani > -1 && aanenvoimakkuus > 0 && voimakkuus > 0)
	{
		voimakkuus = voimakkuus / (100 / aanenvoimakkuus);

		if (voimakkuus > 100)
			voimakkuus = 100;

		if (voimakkuus < 0)
			voimakkuus = 0;
		
		int freq = 22050 + rand()%5000 - rand()%5000;

		if (PisteSound_SFX_Soita(aani, aanenvoimakkuus, 0, freq) == PS_VIRHE)
			PK2_virhe = true;
	}
}

void PK_Laske_Taulut(void)
{
	int i;
	
	for (i=0;i<256;i++)
	{
		if (i<32)
			varitaulu31[i] = i;
		else
			varitaulu31[i] = 31;
	}

	for (i=0;i<256;i++)
		varitaulu_harmaa[i] = i%32;
	
	for (int ci=0; ci<360; ci++) 
		cos_table[ci] = cos(3.1415*2* (ci%360)/180)*33;
    
	for (int si=0; si<360; si++) 
		sin_table[si] = sin(3.1415*2* (si%360)/180)*33;

	for (i=0;i<32;i++)
		for (int pros =0;pros < 100;pros++)
			pros_taulu[i][pros] = (i*pros)/100;
	
	int c, v1, v2, pros;

	for (v1 = 0; v1 < 256; v1++)
		for (v2 = 0; v2 < 256; v2++)
			for (pros = 0; pros < 100; pros++)
			{
				c  = v1 % 32;
				c  = (c*pros)/100;
				c += v2 % 32;
				if (c > 31)
					c = 31;
				varit_taulu[v1][v2][pros] = c;
			}
}

void PK_Palikka_Paivita_Lasketut_Palikat(void)
{
	lasketut_palikat[PALIKKA_HISSI_HORI].vasen = (int)cos_table[degree%360];
	lasketut_palikat[PALIKKA_HISSI_HORI].oikea = (int)cos_table[degree%360];

	lasketut_palikat[PALIKKA_HISSI_VERT].ala = (int)sin_table[degree%360];
	lasketut_palikat[PALIKKA_HISSI_VERT].yla = (int)sin_table[degree%360];

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

	lasketut_palikat[PALIKKA_KYTKIN1].ala = kytkin1_y;
	lasketut_palikat[PALIKKA_KYTKIN1].yla = kytkin1_y;

	lasketut_palikat[PALIKKA_KYTKIN2_YLOS].ala = -kytkin2_y;
	lasketut_palikat[PALIKKA_KYTKIN2_YLOS].yla = -kytkin2_y;		
	
	lasketut_palikat[PALIKKA_KYTKIN2_ALAS].ala = kytkin2_y;
	lasketut_palikat[PALIKKA_KYTKIN2_ALAS].yla = kytkin2_y;

	lasketut_palikat[PALIKKA_KYTKIN2].ala = kytkin2_y;
	lasketut_palikat[PALIKKA_KYTKIN2].yla = kytkin2_y;

	lasketut_palikat[PALIKKA_KYTKIN3_OIKEALLE].oikea = kytkin3_x;
	lasketut_palikat[PALIKKA_KYTKIN3_OIKEALLE].vasen = kytkin3_x;		
	lasketut_palikat[PALIKKA_KYTKIN3_OIKEALLE].koodi = PALIKKA_HISSI_HORI;
	
	lasketut_palikat[PALIKKA_KYTKIN3_VASEMMALLE].oikea = -kytkin3_x;
	lasketut_palikat[PALIKKA_KYTKIN3_VASEMMALLE].vasen = -kytkin3_x;
	lasketut_palikat[PALIKKA_KYTKIN3_VASEMMALLE].koodi = PALIKKA_HISSI_HORI;

	lasketut_palikat[PALIKKA_KYTKIN3].ala = kytkin3_x;
	lasketut_palikat[PALIKKA_KYTKIN3].yla = kytkin3_x;

}

int PK_Palikka_Laske_Palikat(void)
{
	PK2PALIKKA palikka;
	
	for (int i=0;i<150;i++)
	{
		palikka = lasketut_palikat[i];

		palikka.vasen  = 0;
		palikka.oikea  = 0;//32
		palikka.yla	   = 0;
		palikka.ala    = 0;//32

		palikka.koodi  = i;
	
		if ((i < 80 || i > 139) && i != 255)
		{
			palikka.tausta = false;
			
			palikka.oikealle	= PALIKKA_SEINA;
			palikka.vasemmalle	= PALIKKA_SEINA;
			palikka.ylos		= PALIKKA_SEINA;
			palikka.alas		= PALIKKA_SEINA;	

			// Erikoislattiat

			if (i > 139)
			{
				palikka.oikealle	= PALIKKA_TAUSTA;
				palikka.vasemmalle	= PALIKKA_TAUSTA;
				palikka.ylos		= PALIKKA_TAUSTA;
				palikka.alas		= PALIKKA_TAUSTA;
			}

			// Läpikäveltävä lattia

			if (i == PALIKKA_ESTO_ALAS)
			{
				palikka.oikealle	= PALIKKA_TAUSTA;
				palikka.ylos		= PALIKKA_TAUSTA;
				palikka.alas		= PALIKKA_SEINA;
				palikka.vasemmalle	= PALIKKA_TAUSTA;
				palikka.ala -= 27;
			}

			// Mäet

			if (i > 49 && i < 60)
			{
				palikka.oikealle	= PALIKKA_TAUSTA;
				palikka.ylos		= PALIKKA_SEINA;
				palikka.alas		= PALIKKA_SEINA;
				palikka.vasemmalle	= PALIKKA_TAUSTA;
				palikka.ala += 1;
			}

			// Kytkimet

			if (i >= PALIKKA_KYTKIN1 && i <= PALIKKA_KYTKIN3)
			{
				palikka.oikealle	= PALIKKA_SEINA;
				palikka.ylos		= PALIKKA_SEINA;
				palikka.alas		= PALIKKA_SEINA;
				palikka.vasemmalle	= PALIKKA_SEINA;		
			}
		}
		else
		{
			palikka.tausta = true;
	
			palikka.oikealle	= PALIKKA_TAUSTA;
			palikka.vasemmalle	= PALIKKA_TAUSTA;
			palikka.ylos		= PALIKKA_TAUSTA;
			palikka.alas		= PALIKKA_TAUSTA;
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

int PK_Palikka_Tee_Maskit(void)
{
	UCHAR *buffer = NULL;
	DWORD leveys;
	int x,y;
	UCHAR color;

	PisteDraw_Piirto_Aloita(kartta->palikat_buffer,*&buffer,(DWORD &)leveys);

	for (int mask=0; mask<PALIKKA_MAX_MASKEJA; mask++)
	{
		for (x=0; x<32; x++)
		{
			y=0;
			while (y<31 && (color = buffer[x+(mask%10)*32 + (y+(mask/10)*32)*leveys])==255)
			{
				y++;
			}

			palikkamaskit[mask].alas[x] = y;
			
		}
		
		for (x=0; x<32; x++)
		{
			y=31;
			while (y>=0 && (color = buffer[x+(mask%10)*32 + (y+(mask/10)*32)*leveys])==255)
			{
				y--;
			}

			palikkamaskit[mask].ylos[x] = 31-y;
			
		}	
/*
		for (y=0; y<32; y++)
		{
			x=0;
			while (x<31 && (color = buffer[x+(mask%10)*32 + (y+(mask/10)*32)*leveys])==255)
			{
				x++;
			}

			palikkamaskit[mask].oikealle[y] = x;
			//buffer[x+(mask%10)*32 + (y+(mask/10)*32)*leveys] = 0;
			
		}*/
	}

	PisteDraw_Piirto_Lopeta(kartta->palikat_buffer);
	
	return 0;
}

int PK_Poista_Vari_254_Palikoista(void)
{
	UCHAR *buffer = NULL;
	DWORD leveys;
	int x,y;

	if (PisteDraw_Piirto_Aloita(kartta->palikat_buffer,*&buffer,(DWORD &)leveys)==1)
		return 1;

	for (y=0;y<480;y++)
		for(x=0;x<320;x++)
			if (buffer[x+y*leveys] == 254)
				buffer[x+y*leveys] = 255;

	if (PisteDraw_Piirto_Lopeta(kartta->palikat_buffer)==1)
		return 1;

	return 0;
}

void PK_Aseta_Rajaus(void)
{
	if (RAJAA_KARTANPIIRTOALUE)
		PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,MARGINAALI_HORI,MARGINAALI_VERT,
								 KARTANPIIRTO_LEVEYS+MARGINAALI_HORI,KARTANPIIRTO_KORKEUS+MARGINAALI_VERT);
	else
		PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);
}

int PK_Sumenna_Kuva(int kbuffer, DWORD kleveys, int kkorkeus)
{
	UCHAR *buffer = NULL;
	DWORD leveys;
	UCHAR vari,/* vari2, vari3,*/ vari32;
	DWORD x, mx, my;
	int y;
	double kerroin;

	if (PisteDraw_Piirto_Aloita(kbuffer,*&buffer,(DWORD &)leveys)==1)
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

	if (PisteDraw_Piirto_Lopeta(kbuffer)==1)
		return 1;
			
	return 0;
}

int PK_Kopioi_Pelitilanne_Taustakuvaksi(void)
{

	UCHAR *buffer1 = NULL,
		  *buffer2 = NULL;
	DWORD leveys1, leveys2;
	int x,y;

	if (PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER,*&buffer1,(DWORD &)leveys1)==1)
		return 1;

	if (PisteDraw_Piirto_Aloita(kuva_tausta,*&buffer2,(DWORD &)leveys2)==1)
		return 1;

	for (y=0;y<478;y++)
		for(x=0;x<638;x++)
			buffer2[x+y*leveys2] = buffer1[(x+MARGINAALI_HORI)+(y+MARGINAALI_VERT)*leveys1];

	if (PisteDraw_Piirto_Lopeta(kuva_tausta)==1)
	{
		PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER);
		return 1;
	}
			
	if (PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER)==1)
		return 1;	

	return 0;
}

void PK_Kartta_Laske_Reunat()
{
	UCHAR tile1, tile2, tile3;
	bool reuna = false;
	
	memset(kartta->reunat, false, sizeof(kartta->reunat));

	for (int x=1;x<PK2KARTTA_KARTTA_LEVEYS-1;x++)
		for (int y=0;y<PK2KARTTA_KARTTA_KORKEUS-1;y++)
		{
			reuna = false;

			tile1 = kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS];

			if (tile1 > PALIKKA_LOPETUS)
				kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS] = 255;

			tile2 = kartta->seinat[x+(y+1)*PK2KARTTA_KARTTA_LEVEYS];

			if (tile1 > 79 || tile1 == PALIKKA_ESTO_ALAS) tile1 = 1; else tile1 = 0;
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
					if (tile3 > 79 || (tile3 < 60 && tile3 > 49) || tile3 == PALIKKA_ESTO_ALAS) 
						reuna = true;
				}

				if (tile2 == 1)
				{
					tile3 = kartta->seinat[x-1+y*PK2KARTTA_KARTTA_LEVEYS];
					if (tile3 > 79 || (tile3 < 60 && tile3 > 49) || tile3 == PALIKKA_ESTO_ALAS) 
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

int PK_Piirra_LaineTeksti(char *teksti, int fontti, int x, int y)
{
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
			PisteDraw_Font_Kirjoita(fontti4,kirjain,PD_TAUSTABUFFER,x+vali-xs+3,y+ys+3);
			vali += PisteDraw_Font_Kirjoita(fontti,kirjain,PD_TAUSTABUFFER,x+vali-xs,y+ys);
		}
	}
	return vali;
}

int PK_Piirra_LaineTeksti_Hidas(char *teksti, int fontti, int x, int y)
{
	int pituus = strlen(teksti);
	int vali = 0;
	char kirjain[3] = " \0";
	int ys, xs;

	if (pituus > 0)
	{
		for (int i=0;i<pituus;i++)
		{
			ys = (int)(sin_table[((i+degree)*4)%360])/9;
			xs = (int)(cos_table[((i+degree)*4)%360])/11;
			kirjain[0] = teksti[i];
			
			if (asetukset.lapinakyvat_menutekstit)
				vali += PisteDraw_Font_Kirjoita_Lapinakyva(fontti,kirjain,PD_TAUSTABUFFER,x+vali-xs,y+ys,75);
			else
			{
				PisteDraw_Font_Kirjoita(fontti4,kirjain,PD_TAUSTABUFFER,x+vali-xs+1,y+ys+1);
				vali += PisteDraw_Font_Kirjoita(fontti,kirjain,PD_TAUSTABUFFER,x+vali-xs,y+ys);
			}
			
			
		}
	}
	return vali;
}

int PK_Piirra_Lapinakyva_Objekti2(int lahde_buffer, DWORD lahde_x, DWORD lahde_y, DWORD lahde_leveys, DWORD lahde_korkeus,
						 DWORD kohde_x, DWORD kohde_y, int pros, UCHAR vari)
{
	if (pros < 0)
		pros = 0;

	if (pros > 99)
		pros = 99;
	
	int vasen = kohde_x,
		oikea = kohde_x + lahde_leveys,
		yla = kohde_y,
		ala = kohde_y + lahde_korkeus;
	 
	if (vasen < 16)
		vasen = 16;

	if (oikea > RUUDUN_LEVEYS-16)
		oikea = RUUDUN_LEVEYS-16;

	if (yla < 16)
		yla = 16;

	if (ala > RUUDUN_KORKEUS-16)
		ala = RUUDUN_KORKEUS-16;	

	vasen -= kohde_x;
	oikea -= kohde_x;
	yla -= kohde_y;
	ala -= kohde_y;

	if (vasen < oikea && yla < ala)
	{
		
		UCHAR *buffer_lahde = NULL, *buffer_kohde = NULL;
		DWORD leveys_buffer_lahde, leveys_buffer_kohde;
		int color1, color2, 
			lahde_kursori, kohde_kursori,
			px, py;

		PisteDraw_Piirto_Aloita(lahde_buffer,    *&buffer_lahde, (DWORD &)leveys_buffer_lahde);
		PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer_kohde, (DWORD &)leveys_buffer_kohde);

		for (py = yla; py < ala; py++)
		{
			lahde_kursori = (py+lahde_y)*leveys_buffer_lahde + lahde_x;
			
			//lahde_kursori =  py+lahde_y;
			//lahde_kursori *= leveys_buffer_lahde;
			//lahde_kursori += lahde_x;

			kohde_kursori = (py+kohde_y)*leveys_buffer_kohde + kohde_x;
			//kohde_kursori =  py+kohde_y;
			//kohde_kursori *= leveys_buffer_kohde;
			//kohde_kursori += kohde_x;
			
			for (px = vasen; px < oikea; px++)
			{
				color1 = buffer_lahde[px+lahde_kursori];
				
				if (color1 != 255)
				{
					color2 = buffer_kohde[px+kohde_kursori];
					color1 %= 32;
					color2 %= 32;
					//color1 = varitaulu_harmaa[color1];
					//color2 = varitaulu_harmaa[color2];
					//color1 = pros_taulu[color1][pros];
					color1 = (color1*pros)/100;
					color1 += color2;
					
					if (color1 > 31)
						color1 = 31;

					//color1 = varitaulu31[color1]; // asettaa värin arvoon 31 jos suurempi kuin 31
					//color1 += vari;

					buffer_kohde[px+kohde_kursori] = color1+vari;
				}
			}
		}
		
		PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER);
		PisteDraw_Piirto_Lopeta(lahde_buffer);
	}
	else
		return 1;

	return 0;
}
int PK_Piirra_Lapinakyva_Objekti3(int lahde_buffer, DWORD lahde_x, DWORD lahde_y, DWORD lahde_leveys, DWORD lahde_korkeus,
						 DWORD kohde_x, DWORD kohde_y, int pros, UCHAR vari)
{
	if (pros < 0)
		pros = 0;

	if (pros > 99)
		pros = 99;
	
	int vasen = kohde_x,
		oikea = kohde_x + lahde_leveys,
		yla = kohde_y,
		ala = kohde_y + lahde_korkeus;
	 
	if (vasen < 16)
		vasen = 16;

	if (oikea > RUUDUN_LEVEYS-16)
		oikea = RUUDUN_LEVEYS-16;

	if (yla < 16)
		yla = 16;

	if (ala > RUUDUN_KORKEUS-16)
		ala = RUUDUN_KORKEUS-16;	

	vasen -= kohde_x;
	oikea -= kohde_x;
	yla -= kohde_y;
	ala -= kohde_y;

	if (vasen < oikea && yla < ala && pros > 0)
	{
		UCHAR *buffer_lahde = NULL, *buffer_kohde = NULL;
		DWORD leveys_buffer_lahde, leveys_buffer_kohde;
		UCHAR color1, color2; 
		DWORD lahde_kursori, kohde_kursori;
		int   px, py;

		PisteDraw_Piirto_Aloita(lahde_buffer,    *&buffer_lahde, (DWORD &)leveys_buffer_lahde);
		PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer_kohde, (DWORD &)leveys_buffer_kohde);

		for (py = yla; py < ala; py++)
		{
			lahde_kursori = (py+lahde_y)*leveys_buffer_lahde + lahde_x;
			kohde_kursori = (py+kohde_y)*leveys_buffer_kohde + kohde_x;
			
			for (px = vasen; px < oikea; px++)
			{
				color1 = buffer_lahde[px+lahde_kursori];

				if (color1 != 255)
				{
					
					color2 =  buffer_kohde[px+kohde_kursori];
					//color1 =  varitaulu_harmaa[color1];//color1 % 32
					color1 =  color1 % 32;
					color1 =  pros_taulu[color1][pros];//(color1*pros)/100;
					//color1 += varitaulu_harmaa[color2];//color2 % 32
					color1 += color2 % 32;
					color1 =  varitaulu31[color1]+vari; // asettaa värin arvoon 31 jos suurempi kuin 31

					buffer_kohde[px+kohde_kursori] = color1;
				}
			}
		}
		
		PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER);
		PisteDraw_Piirto_Lopeta(lahde_buffer);
	}
	else
		return 1;

	return 0;
}

int PK_Piirra_Lapinakyva_Objekti(int lahde_buffer, DWORD lahde_x, DWORD lahde_y, DWORD lahde_leveys, DWORD lahde_korkeus,
						 DWORD kohde_x, DWORD kohde_y, int pros, UCHAR vari)
{
	kohde_x += MARGINAALI_HORI;
	kohde_y += MARGINAALI_VERT;
	
	int vasen = kohde_x,
		oikea = kohde_x + lahde_leveys,
		yla = kohde_y,
		ala = kohde_y + lahde_korkeus;

	if (pros > 99)
		pros = 99;	

	if (vasen < MARGINAALI_HORI)
		vasen = MARGINAALI_HORI;

	if (yla < MARGINAALI_VERT)
		yla = MARGINAALI_VERT;

	
	if (RAJAA_KARTANPIIRTOALUE)	{
		if (oikea > KARTANPIIRTO_LEVEYS+MARGINAALI_HORI)
			oikea = KARTANPIIRTO_LEVEYS+MARGINAALI_HORI;
		if (ala > KARTANPIIRTO_KORKEUS+MARGINAALI_VERT)
			ala = KARTANPIIRTO_KORKEUS+MARGINAALI_VERT;	
	}
	else {
		if (oikea > RUUDUN_LEVEYS)
			oikea = RUUDUN_LEVEYS;
		if (ala > RUUDUN_KORKEUS)
			ala = RUUDUN_KORKEUS;	
	}

	vasen -= kohde_x;
	oikea -= kohde_x;
	yla   -= kohde_y;
	ala   -= kohde_y;

	if (vasen > oikea || yla > ala || pros < 1 || pros > 99)
		return 1;

	UCHAR *buffer_lahde = NULL, 
		  *buffer_kohde = NULL,
		   color1,
		   *pcolor2;
	DWORD leveys_buffer_lahde, 
		  leveys_buffer_kohde,
		  lahde_kursori, 
		  kohde_kursori;
		  //kohde_kursori2;
	int   px, 
		  py;

	PisteDraw_Piirto_Aloita(lahde_buffer,    *&buffer_lahde, (DWORD &)leveys_buffer_lahde);
	PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer_kohde, (DWORD &)leveys_buffer_kohde);

	lahde_kursori = lahde_y*leveys_buffer_lahde + lahde_x;
	kohde_kursori = kohde_y*leveys_buffer_kohde + kohde_x;

	for (py = yla; py < ala; py++) {
		for (px = vasen; px < oikea; px++) {

			color1 = buffer_lahde[px+lahde_kursori];

			if (color1 != 255) {
				pcolor2  = &buffer_kohde[px+kohde_kursori];
				*pcolor2 = varit_taulu[color1][*pcolor2][pros] + vari;
			} 

		}
		
		lahde_kursori += leveys_buffer_lahde;
		kohde_kursori += leveys_buffer_kohde;
	}
	
	PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER);
	PisteDraw_Piirto_Lopeta(lahde_buffer);

	return 0;
}

int PK_Piirra_Lapinakyva_Objekti4(DWORD lahde_x, DWORD lahde_y, DWORD lahde_leveys, DWORD lahde_korkeus,
						 DWORD kohde_x, DWORD kohde_y, int pros, UCHAR vari, 
						 UCHAR *buffer_lahde, UCHAR *buffer_kohde, DWORD leveys_buffer_lahde, DWORD leveys_buffer_kohde)
{
	kohde_x += MARGINAALI_HORI;
	kohde_y += MARGINAALI_VERT;
	
	int vasen = kohde_x,
		oikea = kohde_x + lahde_leveys,
		yla = kohde_y,
		ala = kohde_y + lahde_korkeus;

	if (pros > 99)
		pros = 99;	

	if (vasen < MARGINAALI_HORI)
		vasen = MARGINAALI_HORI;

	if (yla < MARGINAALI_VERT)
		yla = MARGINAALI_VERT;

	
	if (RAJAA_KARTANPIIRTOALUE)	{
		if (oikea > KARTANPIIRTO_LEVEYS+MARGINAALI_HORI)
			oikea = KARTANPIIRTO_LEVEYS+MARGINAALI_HORI;
		if (ala > KARTANPIIRTO_KORKEUS+MARGINAALI_VERT)
			ala = KARTANPIIRTO_KORKEUS+MARGINAALI_VERT;	
	}
	else {
		if (oikea > RUUDUN_LEVEYS)
			oikea = RUUDUN_LEVEYS;
		if (ala > RUUDUN_KORKEUS)
			ala = RUUDUN_KORKEUS;	
	}

	vasen -= kohde_x;
	oikea -= kohde_x;
	yla   -= kohde_y;
	ala   -= kohde_y;

	if (vasen > oikea || yla > ala || pros < 1 || pros > 99)
		return 1;

	UCHAR color1,
		  *pcolor2;
	DWORD lahde_kursori, 
		  kohde_kursori;
	int   px, 
		  py;

	lahde_kursori = lahde_y*leveys_buffer_lahde + lahde_x;
	kohde_kursori = kohde_y*leveys_buffer_kohde + kohde_x;

	for (py = yla; py < ala; py++) {
		for (px = vasen; px < oikea; px++) {

			color1 = buffer_lahde[px+lahde_kursori];

			if (color1 != 255) {
				pcolor2  = &buffer_kohde[px+kohde_kursori];
				*pcolor2 = varit_taulu[color1][*pcolor2][pros] + vari;
			}
		}
		
		lahde_kursori += leveys_buffer_lahde;
		kohde_kursori += leveys_buffer_kohde;
	}

	
	return 0;
}

int PK_Piirra_Lapinakyva_Piste(DWORD kohde_x, DWORD kohde_y, int pros, UCHAR vari)
{
	kohde_x += MARGINAALI_HORI;
	kohde_y += MARGINAALI_VERT;
	
	if (pros > 99)
		pros = 99;	

	if (RAJAA_KARTANPIIRTOALUE)
	{
		if (kohde_x > (DWORD)KARTANPIIRTO_LEVEYS  || kohde_x < (DWORD)MARGINAALI_HORI || 
			kohde_y > (DWORD)KARTANPIIRTO_KORKEUS || kohde_y < (DWORD)MARGINAALI_VERT)
			return 1;
	}
	else
	{
		if (kohde_x > (DWORD)RUUDUN_LEVEYS  || kohde_x < (DWORD)MARGINAALI_HORI || 
			kohde_y > (DWORD)RUUDUN_KORKEUS || kohde_y < (DWORD)MARGINAALI_VERT)
			return 1;	
	}

	UCHAR *buffer_kohde = NULL,
		   color1, 
		   *color2;
	DWORD leveys_buffer_kohde;

	PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer_kohde, (DWORD &)leveys_buffer_kohde);

	color1 = buffer_kohde[kohde_y*leveys_buffer_kohde + kohde_x];

	if (color1 != 255)
	{
		color2  = &buffer_kohde[kohde_y*leveys_buffer_kohde + kohde_x];
		*color2 = varit_taulu[color1][*color2][pros] + vari;
	}
	
	PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER);

	return 0;
}

int PK_Piirra_Varjo(int lahde_buffer, DWORD lahde_x, DWORD lahde_y, DWORD lahde_leveys, DWORD lahde_korkeus,
						 DWORD kohde_x, DWORD kohde_y)
{
	int vasen = kohde_x,
		oikea = kohde_x + lahde_leveys,
		yla = kohde_y,
		ala = kohde_y + lahde_korkeus;
	 
	if (vasen < 16)
		vasen = 16;

	if (oikea > RUUDUN_LEVEYS-16)
		oikea = RUUDUN_LEVEYS-16;

	if (yla < 16)
		yla = 16;

	if (ala > RUUDUN_KORKEUS-16)
		ala = RUUDUN_KORKEUS-16;	

	vasen -= kohde_x;
	oikea -= kohde_x;
	yla -= kohde_y;
	ala -= kohde_y;

	if (vasen < oikea && yla < ala)
	{
		
		UCHAR *buffer_lahde = NULL, *buffer_kohde = NULL;
		DWORD leveys_buffer_lahde, leveys_buffer_kohde;
		int color, 
			lahde_kursori, kohde_kursori;

		PisteDraw_Piirto_Aloita(lahde_buffer,    *&buffer_lahde, (DWORD &)leveys_buffer_lahde);
		PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer_kohde, (DWORD &)leveys_buffer_kohde);

		for (int py = yla; py < ala; py++)
		{
			lahde_kursori = (py+lahde_y)*leveys_buffer_lahde + lahde_x;
			kohde_kursori = (py+kohde_y)*leveys_buffer_kohde + kohde_x;
			
			for (int px = vasen; px < oikea; px++)
			{
				color = buffer_lahde[px+lahde_kursori];
				
				if (color != 255)
				{
					color = buffer_kohde[px+kohde_kursori];
					color %= 32;
					color /= 2;
					
					buffer_kohde[px+kohde_kursori] = color;
				}
			}
		}
		
		PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER);
		PisteDraw_Piirto_Lopeta(lahde_buffer);
	}
	else
		return 1;

	return 0;
}

void PK_Uusi_Ilmoitus(char *teksti)
{
	if (strcmp(teksti,ilmoitus) != 0 || ilmoitus_ajastin == 0)
	{
		strcpy(ilmoitus,teksti);
		ilmoitus_ajastin = MAX_ILMOITUKSENNAYTTOAIKA;
	}
}

void PK_Alusta_Fadetekstit()
{
	for (int i=0;i<MAX_FADETEKSTEJA;i++)
	{
		fadetekstit[i].ajastin = 0;
	}
}

void PK_Fadeteksti_Uusi(int fontti, char *teksti, DWORD x, DWORD y, DWORD ajastin)
{
	fadetekstit[fadeteksti_index].fontti = fontti;
	strcpy(fadetekstit[fadeteksti_index].teksti,teksti);
	fadetekstit[fadeteksti_index].x = x;
	fadetekstit[fadeteksti_index].y = y;
	fadetekstit[fadeteksti_index].ajastin = ajastin;
	fadeteksti_index++;
	
	if (fadeteksti_index >= MAX_FADETEKSTEJA)
		fadeteksti_index = 0;
}

int PK_Fadetekstit_Piirra()
{
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
				if (PisteDraw_Font_Kirjoita_Lapinakyva( fadetekstit[i].fontti,fadetekstit[i].teksti,PD_TAUSTABUFFER,
														fadetekstit[i].x-kamera_x, fadetekstit[i].y-kamera_y, pros)==PD_VIRHE)
					return 1;
			}
			else
				PisteDraw_Font_Kirjoita( fadetekstit[i].fontti,fadetekstit[i].teksti,PD_TAUSTABUFFER,
									 fadetekstit[i].x-kamera_x, fadetekstit[i].y-kamera_y);

		}
	}
	return 0;
}

void PK_Fadetekstit()
{
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

int PK_Piirra_Piste(DWORD x, DWORD y, int pros, UCHAR vari)
{
	//if (pros > 99 || !asetukset.lapinakyvat_objektit)
		PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,x-kamera_x, y-kamera_y,x-kamera_x+1,y-kamera_y+1,vari+25);
	//else
	//	PK_Piirra_Lapinakyva_Piste(x-kamera_x, y-kamera_y, pros * 8, vari);

	return 0;
}

int PK_Piirra_Tahti(DWORD x, DWORD y, int pros, UCHAR vari)
{
	if (pros > 99 || !asetukset.lapinakyvat_objektit)
		PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x-kamera_x, y-kamera_y,1,1,11,11);
	else
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 2, 2, 10, 10, x-kamera_x, y-kamera_y, pros, vari);

	return 0;
}

int PK_Piirra_Iskuvalays(DWORD x, DWORD y)
{
	int framex = ((degree%12)/3) * 58; 
	PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x-kamera_x-28+8, y-kamera_y-27+8,1+framex,83,1+57+framex,83+55);//140<-83
	return 0;
}

int PK_Piirra_Valo(DWORD x, DWORD y, int pros, UCHAR vari)
{
	if (asetukset.lapinakyvat_objektit)
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 1, 14, 13, 13, x-kamera_x, y-kamera_y, pros, vari);
	else
	{
		int vx = (vari/32) * 14;
		PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x-kamera_x, y-kamera_y,1+vx,14+14,14+vx,27+14);
	}

	return 0;
}

int PK_Piirra_Kipina(DWORD x, DWORD y, int pros, UCHAR vari)
{
	if (asetukset.lapinakyvat_objektit)
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 99, 14, 7, 7, x-kamera_x, y-kamera_y, pros, vari);
	else
	{
		int vx = (vari/32) * 8;
		PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x-kamera_x, y-kamera_y,99+vx,14+14,106+vx,21+14);
	}

	return 0;
}

void PK_Piirra_Hoyhen(int x, int y, int &anim)
{
	int xplus = (anim/7) * 21;
	PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x-kamera_x,y-kamera_y,14+xplus,1,34+xplus,12);
	anim++;
	if (anim > 63)
		anim = 0;
}

void PK_Piirra_Savupilvi(int x, int y, int &anim)
{
	int frame = (anim/7);
	int xplus = (frame%17) * 36;
	int yplus = 0;

	if (anim < 7*30) {
	
		if (frame > 16)
			yplus = 32;

		PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x-kamera_x,y-kamera_y,1+xplus,338+yplus,34+xplus,366+yplus);
		anim++;
	}
	
	//if (anim > 63)
	//	anim = 0;
}

void PK_Piirra_Polypilvi(DWORD x, DWORD y, int pros, UCHAR vari)
{
	if (pros > 99 || !asetukset.lapinakyvat_objektit)
		PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x-kamera_x,y-kamera_y,226,2,224,49);
	else
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 226, 2, 18, 19, x-kamera_x, y-kamera_y, pros, vari);
}

int PK_Partikkelit_Tyhjenna()
{
	int i = 0;
	
	partikkeli_index = 0;

	while (i < MAX_PARTIKKELEITA)
	{
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

void PK_Partikkeli_Uusi(int tyyppi, double x, double y, double a, double b, int aika, double paino, int vari)
{
	if (x < kamera_x+RUUDUN_LEVEYS+10 && x > kamera_x-10 &&
		y < kamera_y+RUUDUN_KORKEUS+10 && y > kamera_y-10)
	{
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

int	PK_Partikkelit_Piirra()
{
	int pros;
	int piirtoleveys, piirtokorkeus;

	if (RAJAA_KARTANPIIRTOALUE)
	{
		piirtoleveys  = KARTANPIIRTO_LEVEYS;
		piirtokorkeus = KARTANPIIRTO_KORKEUS;
	}
	else
	{
		piirtoleveys  = RUUDUN_LEVEYS;
		piirtokorkeus = RUUDUN_KORKEUS;
	}

	for (int i=0;i<MAX_PARTIKKELEITA;i++)
	{
		if (partikkelit[i].aika > 0)
		{
			if (partikkelit[i].x < kamera_x+piirtoleveys && 
				partikkelit[i].x > kamera_x &&
				partikkelit[i].y < kamera_y+piirtokorkeus && 
				partikkelit[i].y > kamera_y) 
			{
				if (partikkelit[i].aika < 100)
					pros = partikkelit[i].aika;
				else
					pros = 100;
				
				switch (partikkelit[i].tyyppi)
				{
				case PARTIKKELI_TAHTI		:	PK_Piirra_Tahti((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break;
				case PARTIKKELI_HOYHEN		:	PK_Piirra_Hoyhen((int)partikkelit[i].x,(int)partikkelit[i].y,partikkelit[i].anim);
												break;
				case PARTIKKELI_POLYPILVI	:	PK_Piirra_Polypilvi((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break;
				case PARTIKKELI_SAVUPILVI	:	PK_Piirra_Savupilvi((int)partikkelit[i].x,(int)partikkelit[i].y, partikkelit[i].anim);
												break;
				case PARTIKKELI_VALO		:	PK_Piirra_Valo((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break;
				case PARTIKKELI_KIPINA		:	PK_Piirra_Kipina((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break;
				case PARTIKKELI_PISTE		:	PK_Piirra_Piste((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break;
				default						:	break;
				}
			}
		}
		else
			partikkelit[i].aika = 0;
	}
	return 0;
}

int	PK_Partikkelit()
{
	for (int i=0;i<MAX_PARTIKKELEITA;i++)
	{
		if (partikkelit[i].aika > 0)
		{
			partikkelit[i].x += partikkelit[i].a;
			partikkelit[i].y += partikkelit[i].b;
			
			if (partikkelit[i].paino > 0)
				partikkelit[i].b += partikkelit[i].paino;

			partikkelit[i].aika--;
		}
	}
	return 0;
}

int PK_Taustapartikkelit_Tyhjenna()
{
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

void PK_Taustapartikkelit_Vesipisara(PK2PARTIKKELI &p)
{
//	p.x += p.a / 3;
	p.y += p.b / 3.0 + 2.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,kx,ky,kx+1,ky+4,40+(int)p.b);
};

void PK_Taustapartikkelit_Lehti1(PK2PARTIKKELI &p)
{
	p.x += p.a / 9.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,kx,ky,kx+2,ky+2,96+6+(int)p.b+(int)(p.x+p.y)%10);
};

void PK_Taustapartikkelit_Lehti2(PK2PARTIKKELI &p)
{
	p.x += p.a / 14.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y),
		frame = (int(p.y/10)%4)*23;

	PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,kx,ky,1+frame,141,21+frame,152);
};

void PK_Taustapartikkelit_Lehti3(PK2PARTIKKELI &p)
{
	p.x += p.a / 12.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y),
		frame = (int(p.y/5)%4)*20;

	PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,kx,ky,93+frame,141,109+frame,150);

};

void PK_Taustapartikkelit_Lehti4(PK2PARTIKKELI &p)
{
	p.x += p.a / 11.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y),
		frame = (int(p.y/5)%2)*14;

	PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,kx,ky,173+frame,141,183+frame,150);
};

void PK_Taustapartikkelit_Hiutale1(PK2PARTIKKELI &p)
{
	//p.x += p.a / 11.0;
	p.x += sin_table[int(p.y)%360]/50.0;
	p.y += p.b / 7.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,kx,ky,1,155,8,162);
};

void PK_Taustapartikkelit_Hiutale2(PK2PARTIKKELI &p)
{
	//p.x += p.a / 11.0;
	p.x += sin_table[int(p.y)%360]/100.0;
	p.y += p.b / 8.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,kx,ky,11,155,16,160);
};

void PK_Taustapartikkelit_Hiutale3(PK2PARTIKKELI &p)
{
	//p.x += p.a / 11.0;
	p.y += p.b / 8.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,kx,ky,19,155,22,158);
};

void PK_Taustapartikkelit_Hiutale4(PK2PARTIKKELI &p)
{
	//p.x += p.a / 9.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,kx,ky,kx+2,ky+2,20+(int)p.b);
};

int	PK_Taustapartikkelit()
{
	PK_Aseta_Rajaus();
	//PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,KARTANPIIRTO_LEVEYS,KARTANPIIRTO_KORKEUS);

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

void PK_Tehoste_Hoyhenet(DWORD x, DWORD y)
{
	for (int i=0;i<9;i++)//6
		PK_Partikkeli_Uusi(	PARTIKKELI_HOYHEN,x+rand()%17-rand()%17,y+rand()%20-rand()%10,
							(rand()%16-rand()%16)/10.0,(45+rand()%45)/100.0,300+rand()%40,0,0);
}

void PK_Tehoste_Loiskahdus(DWORD x, DWORD y, UCHAR vari)
{
	/*
	for (int i=0;i<12;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_VALO,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5,(rand()%7-rand()%7)/3,
							rand()%50+60,0.025,vari);*/
	for (int i=0;i<7;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_KIPINA,x+rand()%17-13,y+rand()%17-13,
							(rand()%5-rand()%5)/4.0,(rand()%4-rand()%7)/3.0,
							rand()%50+40,0.025,vari);//0.015

	for (i=0;i<20;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_PISTE,x+rand()%17-13,y+rand()%17-13,
							(rand()%5-rand()%5)/4.0,(rand()%2-rand()%7)/3.0,
							rand()%50+40,0.025,vari);//0.015

	PK_Soita_Aani(loiskahdus_aani, 100, x, y, AANET_SAMPLERATE, true);
}

void PK_Tehoste_Rajahdys(DWORD x, DWORD y, UCHAR vari)
{
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

void PK_Tehoste_Savu(DWORD x, DWORD y, UCHAR vari)
{
	for (int i=0;i<3;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_SAVUPILVI,x+rand()%17-32,y+rand()%17,
							0,double((rand()%3)+3) / -10.0/*-0.3*/,450,0.0,vari);
	for (i=0;i<6;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_POLYPILVI,x+rand()%30-rand()%30-10,y+rand()%30-rand()%30+10,
							0,-0.3,rand()%50+60,0,vari);
}

void PK_Tehoste_Savupilvet(DWORD x, DWORD y)
{
	for (int i=0;i<5;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_SAVUPILVI,x+rand()%17-32,y+rand()%17,
							0,double((rand()%3)+3) / -10.0/*-0.3*/,450,0.0,0);
}

void PK_Tehoste_Tahdet(DWORD x, DWORD y, UCHAR vari)
{
	for (int i=0;i<5;i++)
		PK_Partikkeli_Uusi(PARTIKKELI_TAHTI,x-5, y-5, (rand()%3-rand()%3)/1.5, rand()%3*-1,100,(rand()%5+5)/100.0/* 0.05*/,vari);//300
	
	for (i=0;i<3;i++)//12
		PK_Partikkeli_Uusi(	PARTIKKELI_PISTE,x-5, y-5, (rand()%3-rand()%3)/1.5, rand()%3*-1,100,(rand()%5+5)/100.0,vari);
}

void PK_Tehosteet(UCHAR tehoste, DWORD x, DWORD y)
{
	switch (tehoste)
	{
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

int PK_Kartta_Vaihda_Kallopalikat()
{
	UCHAR seina, tausta;
	DWORD x,y;
	
	for (x=0; x<PK2KARTTA_KARTTA_LEVEYS; x++)
		for (y=0; y<PK2KARTTA_KARTTA_KORKEUS; y++)
		{
			seina =  kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS];
			tausta = kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS];
			
			if (seina == PALIKKA_KALLOSEINA)
			{
				kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS] = 255;
				if (tausta != PALIKKA_KALLOSEINA)
					PK_Tehoste_Savupilvet(x*32+6,y*32+6);

			}

			if (tausta == PALIKKA_KALLOTAUSTA && seina == 255)
			{
				kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS] = PALIKKA_KALLOSEINA;
			}
		}

	jaristys = 90;//60

	//PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_game_locksopen));

	PK_Kartta_Laske_Reunat();
				
	return 0;
}

int PK_Kartta_Avaa_Lukot()
{
	UCHAR palikka;
	DWORD x,y;
	
	avaimia = 0;
	for (x=0; x<PK2KARTTA_KARTTA_LEVEYS; x++)
		for (y=0; y<PK2KARTTA_KARTTA_KORKEUS; y++)
		{
			palikka = kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS];
			if (palikka == PALIKKA_LUKKO)
			{
				kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS] = 255;
				PK_Tehoste_Savupilvet(x*32+6,y*32+6);
			}
		}

	jaristys = 90;//60

	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_game_locksopen));

	PK_Kartta_Laske_Reunat();
				
	return 0;
}

int PK_Kartta_Laske_Avaimet()
{
	UCHAR sprite;
	DWORD x;
	
	avaimia = 0;

	for (x=0; x<PK2KARTTA_KARTTA_KOKO; x++)
	{
		sprite = kartta->spritet[x];
		if (sprite != 255)
			if (protot[sprite].avain && protot[sprite].tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
				avaimia++;
	}
				
	return 0;
}

int PK_Kartta_Etsi_Alku()
{
	double  alku_x = 320,
			alku_y = 196;
	int		alkujen_maara = 0, alku = 0, 
			x, y;
	
	for (x=0; x<PK2KARTTA_KARTTA_KOKO; x++)
		if (kartta->seinat[x] == PALIKKA_ALOITUS)
			alkujen_maara ++;
				
	if (alkujen_maara > 0)
	{
		alku = rand()%alkujen_maara + 1;
		alkujen_maara = 1;

		for (x=0; x < PK2KARTTA_KARTTA_LEVEYS; x++)
			for (y=0; y < PK2KARTTA_KARTTA_KORKEUS; y++)
				if (kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS] == PALIKKA_ALOITUS)
				{
					if (alkujen_maara == alku)
					{
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

void PK_Prototyyppi_Tyhjenna()
{
	for (int i=0; i<MAX_PROTOTYYPPEJA; i++)
	{
		for (int j=0;j<MAX_AANIA;j++)
			if (protot[i].aanet[j] > -1)
				PisteSound_SFX_Tuhoa(protot[i].aanet[j]);

		protot[i].Uusi();
		strcpy(kartta->protot[i],"");
	}

	seuraava_vapaa_proto = 0;
}

int PK_Prototyyppi_Lataa_Aani(char *polku, char *tiedosto)
{
	if (strcmp(tiedosto,"")!=0)
	{
	
		char aanitiedosto[255];

		strcpy(aanitiedosto,polku);
		strcat(aanitiedosto,tiedosto);
	
		int i = PisteSound_SFX_Uusi(aanitiedosto);
	
		if (i == PS_VIRHE)
		{
			PK2_virhe = true;
			return -1;
		}
		else
			return i;
	}
	
	return -1;

}

int PK_Prototyyppi_Lataa_Vanha(char *polku, char *tiedosto)
{
	char aanipolku[255];
	char testipolku[255];
	strcpy(aanipolku,polku);

	PisteLog_Kirjoita("- Loading sprite: ");	
	PisteLog_Kirjoita(polku);	
	PisteLog_Kirjoita(tiedosto);
	PisteLog_Kirjoita("\n");

	if (seuraava_vapaa_proto < MAX_PROTOTYYPPEJA)
	{
		if (protot[seuraava_vapaa_proto].Lataa(polku, tiedosto) == 1)
		{
			PisteLog_Kirjoita("  - Loading failed\n");
			return 1;
		}
		protot[seuraava_vapaa_proto].indeksi = seuraava_vapaa_proto;
	}
	else {
		PisteLog_Kirjoita("  - Error: Maximum limit of sprite types exceeded! \n");
		return 2;
	}

	for (int i=0;i<MAX_AANIA;i++) {
		
		if (strcmp(protot[seuraava_vapaa_proto].aanitiedostot[i],"")!=0) {
			/*PisteLog_Kirjoita("  - Loading sound: ");
			PisteLog_Kirjoita(aanipolku);
			PisteLog_Kirjoita(protot[seuraava_vapaa_proto].aanitiedostot[i]);
			PisteLog_Kirjoita("\n");*/
			
			strcpy(testipolku,aanipolku);
			strcat(testipolku,"\\");
			strcat(testipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);

			if (PK_Onko_File(testipolku))
				protot[seuraava_vapaa_proto].aanet[i] = PK_Prototyyppi_Lataa_Aani(aanipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);
			else {
			//if (protot[seuraava_vapaa_proto].aanet[i] == -1) {
				//strcpy(aanipolku,"sprites\\");
				_getcwd(aanipolku, _MAX_PATH);
				strcat(aanipolku,"\\sprites\\");

				strcpy(testipolku,aanipolku);
				strcat(testipolku,"\\");
				strcat(testipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);

				if (PK_Onko_File(testipolku))
					protot[seuraava_vapaa_proto].aanet[i] = PK_Prototyyppi_Lataa_Aani(aanipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);
			}
		}
		//if (protot[seuraava_vapaa_proto].aanet[i] == -1)
		//	return 3;
	}

	PisteLog_Kirjoita("  - Loading completed. \n");
	
	seuraava_vapaa_proto++;
	
	return 0;
}

void PK_Prototyyppi_Aseta_Muutos_Sprite(int i)
{
	int j = 0;
	bool loytyi = false;
	
	if (strcmp(protot[i].muutos_sprite,"")!=0) {
		while (j<MAX_PROTOTYYPPEJA && !loytyi) {
			if (j!=i) {
				if (strcmp(protot[i].muutos_sprite,protot[j].tiedosto)==0) {
					protot[i].muutos = j;
					loytyi = true;
				}
			}
			j++;
		}
		
		if (!loytyi) {
			char polku[_MAX_PATH];
			strcpy(polku,"sprites\\");
			//PK_Lisaa_Episodin_Hakemisto(polku);

			if (PK_Prototyyppi_Lataa_Vanha(polku, protot[i].muutos_sprite)==0)
				protot[i].muutos = seuraava_vapaa_proto-1; // jokainen lataus kasvattaa seuraava_vapaa_proto:a
		}
	}
}

void PK_Prototyyppi_Aseta_Bonus_Sprite(int i)
{
	int j = 0;
	bool loytyi = false;
	
	if (strcmp(protot[i].bonus_sprite,"")!=0)
	{
		while (j<MAX_PROTOTYYPPEJA && !loytyi)
		{
			if (j!=i)
			{
				if (strcmp(protot[i].bonus_sprite,protot[j].tiedosto)==0)
				{
					protot[i].bonus = j;
					loytyi = true;
				}
			}

			j++;
		}
		
		if (!loytyi)
		{
			char polku[_MAX_PATH];
			strcpy(polku,"sprites\\");
			//PK_Lisaa_Episodin_Hakemisto(polku);

			if (PK_Prototyyppi_Lataa_Vanha(polku, protot[i].bonus_sprite)==0)
				protot[i].bonus = seuraava_vapaa_proto-1;
		}
	}
}

void PK_Prototyyppi_Aseta_Ammus1_Sprite(int i)
{
	int j = 0;
	bool loytyi = false;
	
	if (strcmp(protot[i].ammus1_sprite,"")!=0)
	{
		while (j<MAX_PROTOTYYPPEJA && !loytyi)
		{
			if (j!=i)
			{
				if (strcmp(protot[i].ammus1_sprite,protot[j].tiedosto)==0)
				{
					protot[i].ammus1 = j;
					loytyi = true;
				}
			}

			j++;
		}
		
		if (!loytyi)
		{
			char polku[_MAX_PATH];
			strcpy(polku,"sprites\\");
			//PK_Lisaa_Episodin_Hakemisto(polku);
			

			if (PK_Prototyyppi_Lataa_Vanha(polku, protot[i].ammus1_sprite)==0)
				protot[i].ammus1 = seuraava_vapaa_proto-1;
		}
	}
}

void PK_Prototyyppi_Aseta_Ammus2_Sprite(int i)
{
	int j = 0;
	bool loytyi = false;
	
	if (strcmp(protot[i].ammus2_sprite,"")!=0)
	{
		while (j<MAX_PROTOTYYPPEJA && !loytyi)
		{
			if (j!=i)
			{
				if (strcmp(protot[i].ammus2_sprite,protot[j].tiedosto)==0)
				{
					protot[i].ammus2 = j;
					loytyi = true;
				}
			}

			j++;
		}
		
		if (!loytyi)
		{
			char polku[_MAX_PATH];
			strcpy(polku,"sprites\\");
			//PK_Lisaa_Episodin_Hakemisto(polku);

			if (PK_Prototyyppi_Lataa_Vanha(polku, protot[i].ammus2_sprite)==0)
				protot[i].ammus2 = seuraava_vapaa_proto-1;
		}
	}
}

int PK_Prototyyppi_Lataa_Kaikki()
{
	char polku[_MAX_PATH];
	int viimeinen_proto;
	
	for (int i=0;i < MAX_PROTOTYYPPEJA;i++)
	{
		if (strcmp(kartta->protot[i],"") != 0)
		{
			viimeinen_proto = i;
			strcpy(polku,"");
			PK_Lisaa_Episodin_Hakemisto(polku);
			//PK_Lisaa_Episodin_Hakemisto(polku);

			if (PK_Prototyyppi_Lataa_Vanha(polku,kartta->protot[i])==1)
			{
				strcpy(polku,"");
				strcpy(polku,"sprites\\");
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

	for (i=0;i<MAX_PROTOTYYPPEJA;i++)
	{
		PK_Prototyyppi_Aseta_Muutos_Sprite(i);
		PK_Prototyyppi_Aseta_Bonus_Sprite(i);
		PK_Prototyyppi_Aseta_Ammus1_Sprite(i);
		PK_Prototyyppi_Aseta_Ammus2_Sprite(i);
	}

	return 0;
}

void PK_Sprite_Maaraa_Alkusuunnat(void)
{
	for (int i=0;i<MAX_SPRITEJA;i++)
	{
		if (pelaaja_index >= 0 && pelaaja_index < MAX_SPRITEJA && !spritet[i].piilota)
		{
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
/*
			for (int ai=0;ai<SPRITE_MAX_AI;ai++)
			{
				if (spritet[i].tyyppi->AI[ai] == AI_RANDOM_ALOITUSSUUNTA_HORI)
				{
					while (spritet[i].a == 0)
						spritet[i].a = ((rand()%2 - rand()%2) * spritet[i].tyyppi->max_nopeus) / 2;
				}

				if (spritet[i].tyyppi->AI[ai] == AI_ALOITUSSUUNTA_PELAAJAA_KOHTI)
				{
					if (spritet[i].x < spritet[pelaaja_index].x)
						spritet[i].a = spritet[i].tyyppi->max_nopeus / 3.5;
		
					if (spritet[i].x > spritet[pelaaja_index].x)
						spritet[i].a = (spritet[i].tyyppi->max_nopeus * -1) / 3.5;
				}
			}*/
		}
	}
}

int PK_Kartta_Lataa(char *nimi)
{
	PK_Esineet_Alusta();
	
	PK_Prototyyppi_Tyhjenna();

	char polku[_MAX_PATH];
	strcpy(polku,"");
	PK_Lisaa_Episodin_Hakemisto(polku);

	PisteLog_Kirjoita("- Trying to load a map: ");	
	PisteLog_Kirjoita(polku);	
	PisteLog_Kirjoita(nimi);
	PisteLog_Kirjoita("\n");
	
	if (kartta->Lataa(polku, nimi) == 1) {
		PisteLog_Kirjoita("  - Loading failed. \n");
		return 1;
	}

	PK_Uusi_Jakso();

	if (strcmp(kartta->versio,"1.2") == 0 || strcmp(kartta->versio,"1.3") == 0)
	{
		if (PK_Prototyyppi_Lataa_Kaikki()==1)
			return 1;
	}
	
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

	if (strcmp(kartta->musiikki,"")!=0)
	{
		char biisi[_MAX_PATH] = "";
		PK_Lisaa_Episodin_Hakemisto(biisi);
		strcat(biisi,kartta->musiikki);
		
		if (PK_Midas_Lataa_Musiikki(biisi)!=0)
		{
			strcpy(biisi,"music\\");
			strcat(biisi,kartta->musiikki);

			if (PK_Midas_Lataa_Musiikki(biisi)!=0)
				if (PK_Midas_Lataa_Musiikki("music\\song01.xm")!=0)
					PK2_virhe = true;
		}
	}

	PisteLog_Kirjoita("  - Loading map completed. \n");

	return 0;
}


void PK_Palikka_Este(PK2PALIKKA &palikka)
{
	palikka.tausta = false;
	
	palikka.oikealle	= PALIKKA_SEINA;
	palikka.vasemmalle	= PALIKKA_SEINA;
	palikka.ylos		= PALIKKA_SEINA;
	palikka.alas		= PALIKKA_SEINA;	

	// Erikoislattiat

	if (palikka.koodi > 139)
	{
		palikka.oikealle	= PALIKKA_TAUSTA;
		palikka.vasemmalle	= PALIKKA_TAUSTA;
		palikka.ylos		= PALIKKA_TAUSTA;
		palikka.alas		= PALIKKA_TAUSTA;
	}

	// Hissit

	if (palikka.koodi == PALIKKA_HISSI_HORI)
	{
		palikka.vasen += (int)cos_table[degree%360];
		palikka.oikea += (int)cos_table[degree%360];
	}

	if (palikka.koodi == PALIKKA_HISSI_VERT)
	{
		palikka.ala += (int)sin_table[degree%360];
		palikka.yla += (int)sin_table[degree%360];
	}

	// Läpikäveltävä lattia

	if (palikka.koodi == PALIKKA_ESTO_ALAS)
	{
		palikka.oikealle	= PALIKKA_TAUSTA;
		palikka.ylos		= PALIKKA_TAUSTA;
		palikka.alas		= PALIKKA_SEINA;
		palikka.vasemmalle	= PALIKKA_TAUSTA;
		palikka.ala -= 27;
	}

	// Mäet

	if (palikka.koodi > 49 && palikka.koodi < 60)
	{
		palikka.oikealle	= PALIKKA_TAUSTA;
		palikka.ylos		= PALIKKA_SEINA;
		palikka.alas		= PALIKKA_SEINA;
		palikka.vasemmalle	= PALIKKA_TAUSTA;
		palikka.ala += 1;
	}

	// Kytkimet

	if (palikka.koodi >= PALIKKA_KYTKIN1 && palikka.koodi <= PALIKKA_KYTKIN3)
	{
		palikka.oikealle	= PALIKKA_SEINA;
		palikka.ylos		= PALIKKA_SEINA;
		palikka.alas		= PALIKKA_SEINA;
		palikka.vasemmalle	= PALIKKA_SEINA;		
	}

	// Lattiat joihin kytkimet vaikuttavat

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


	if (palikka.koodi == PALIKKA_KYTKIN2_YLOS)
	{
		palikka.ala -= kytkin2_y/2;
		palikka.yla -= kytkin2_y/2;		
	}

	if (palikka.koodi == PALIKKA_KYTKIN2_ALAS)
	{
		palikka.ala += kytkin2_y/2;
		palikka.yla += kytkin2_y/2;		
	}

	if (palikka.koodi == PALIKKA_KYTKIN2)
	{
		palikka.ala += kytkin2_y/2;
		palikka.yla += kytkin2_y/2;		
	}

	if (palikka.koodi == PALIKKA_KYTKIN3_OIKEALLE)
	{
		palikka.oikea += kytkin3_x/2;
		palikka.vasen += kytkin3_x/2;
		palikka.koodi = PALIKKA_HISSI_HORI; // samat idea sivusuuntaan työnnössä
	}

	if (palikka.koodi == PALIKKA_KYTKIN3_VASEMMALLE)
	{
		palikka.oikea -= kytkin3_x/2;
		palikka.vasen -= kytkin3_x/2;
		palikka.koodi = PALIKKA_HISSI_HORI; // samat idea sivusuuntaan työnnössä
	}

	if (palikka.koodi == PALIKKA_KYTKIN3)
	{
		palikka.ala += kytkin3_x/2;
		palikka.yla += kytkin3_x/2;		
	}

	if (palikka.koodi == PALIKKA_KYTKIN1)
	{
		palikka.ala += kytkin1_y/2;
		palikka.yla += kytkin1_y/2;		
	}

}

PK2PALIKKA PK_Palikka_Tutki(int x, int y)
{
	PK2PALIKKA palikka;

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

	UCHAR i = kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS];

	if (i<150)
	{
		palikka = lasketut_palikat[i];
		palikka.vasen  = x*32+lasketut_palikat[i].vasen;
		palikka.oikea  = x*32+32+lasketut_palikat[i].oikea;
		palikka.yla	   = y*32+lasketut_palikat[i].yla;
		palikka.ala    = y*32+32+lasketut_palikat[i].ala;	 
	}
	else
	{
		palikka.koodi  = 255;
		palikka.tausta = true;
		palikka.vasen  = x*32;
		palikka.oikea  = x*32+32;
		palikka.yla	   = y*32;
		palikka.ala    = y*32+32;
		palikka.vesi   = false;
	}

	i = kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS];
	
	if (i>131 && i<140)
		palikka.vesi = true;

	palikka.reuna = kartta->reunat[x+y*PK2KARTTA_KARTTA_LEVEYS];

	return palikka;
}

int PK_Sprite_Lisaa_Taustasprite(int index)
{
	for (int i=0;i<MAX_SPRITEJA;i++)
	{
		if (taustaspritet[i] == -1)
		{
			taustaspritet[i] = index;
			return 0;
		}
	}
	return 1;
}

void PK_Sprite_Jarjesta_Taustaspritet(void)
{
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

int PK_Sprite_Lisaa(PK2Sprite_Prototyyppi &proto, int pelaaja, double x, double y, int emo)
{
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

int PK_Sprite_Lisaa_Bonus(PK2Sprite_Prototyyppi &proto, int pelaaja, double x, double y, int emo)
{
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

int PK_Sprite_Lisaa_Ammus(PK2Sprite_Prototyyppi &proto, int pelaaja, double x, double y, int emo)
{
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
					else // tai jos kyseessä on vihollinen
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

int PK_Sprite_Tyhjenna()
{
	int i = 0;
	
	while (i < MAX_SPRITEJA)
	{
		spritet[i] = PK2Sprite();
		taustaspritet[i] = -1;
		i++;
	}

	return 0;
}

int PK_Kartta_Aseta_Spritet()
{
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

int PK_Piirra_Bonukset(void)
{
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

void PK_Esineet_Alusta()
{
	for (int i=0;i<MAX_ESINEITA;i++)
		esineet[i] = NULL;
}

int PK_Esineet_Piirra()
{
	int x,y;
	PK_Aseta_Rajaus();
	//PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);

	y = KARTANPIIRTO_KORKEUS-35;//36
	x = item_paneeli_x + 35;//40

	UCHAR v1, v2;

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
				
				PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,	x-esineet[i]->leveys/2+4,
														y-esineet[i]->korkeus/2+4,
														x+esineet[i]->leveys/2+4,
														y+esineet[i]->korkeus/2+4,
														0);
				
				PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,	x-esineet[i]->leveys/2-1,
														y-esineet[i]->korkeus/2-1,
														x+esineet[i]->leveys/2+1,
														y+esineet[i]->korkeus/2+1,
														v1);

				PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,	x-esineet[i]->leveys/2+1,
														y-esineet[i]->korkeus/2+1,
														x+esineet[i]->leveys/2-1,
														y+esineet[i]->korkeus/2-1,
														v2);
			
			}


			esineet[i]->Piirra(x-esineet[i]->leveys/2,y-esineet[i]->korkeus/2,0);
			x += 38;
		}
	
	PK_Aseta_Rajaus();
	//PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);
	
	return 0;
}

bool PK_Esineet_Lisaa(PK2Sprite_Prototyyppi *proto)
{
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

int PK_Esineet_Kayta()
{
	if (esineet[0] != NULL)
		PK_Sprite_Lisaa(*esineet[0],0,spritet[pelaaja_index].x-esineet[0]->leveys,spritet[pelaaja_index].y,MAX_SPRITEJA/*pelaaja_index*/);

	for (int i=0;i<MAX_ESINEITA-1;i++)
		esineet[i] = esineet[i+1];

	esineet[MAX_ESINEITA-1] = NULL;

	return 0;
}

int PK_Esineet_Vaihda()
{
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

void PK_Tutki_Seina(PK2Sprite &sprite, 
					PK2PALIKKA &palikka, 	
					double	&sprite_x,
					double	&sprite_y,
					double	&sprite_a,
					double	&sprite_b,
					double	&sprite_vasen,
					double	&sprite_oikea,
					double	&sprite_yla,
					double	&sprite_ala,
					int		&sprite_leveys,
					int		&sprite_korkeus,
					int		&kartta_vasen,
					int		&kartta_yla,
					bool	&oikealle,
					bool	&vasemmalle,
					bool	&ylos,
					bool	&alas)
{

	// tarkistetaan onko palikka vasen tai oikea seinä.
	if (sprite_yla < palikka.ala && sprite_ala-1 > palikka.yla)
	{
		if (sprite_oikea+sprite_a-1 > palikka.vasen && sprite_vasen+sprite_a < palikka.oikea)
		{
			// Tutkitaan onko sprite menossa oikeanpuoleisen palikan sisään.
			if (sprite_oikea+sprite_a < palikka.oikea)
			{
				// Onko palikka seinä?
				if (palikka.oikealle == PALIKKA_SEINA)
				{
					oikealle = false;

					if (palikka.koodi == PALIKKA_HISSI_HORI)
						sprite_x = palikka.vasen - sprite_leveys/2;
				}
			}
			
			if (sprite_vasen+sprite_a > palikka.vasen)
			{
				if (palikka.vasemmalle == PALIKKA_SEINA)
				{
					vasemmalle = false;
					
					if (palikka.koodi == PALIKKA_HISSI_HORI)
						sprite_x = palikka.oikea + sprite_leveys/2;

				}
			}
		}
	}

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;				

	//...tarkistetaan, onko palikka lattia tai katto.

	if (sprite_vasen < palikka.oikea && sprite_oikea-1 > palikka.vasen)
	{
		if (sprite_ala+sprite_b-1 >= palikka.yla && sprite_yla+sprite_b <= palikka.ala)
		{
			if (sprite_ala+sprite_b-1 <= palikka.ala)
			{
				if (palikka.alas == PALIKKA_SEINA)
				{
					alas = false;
					
					if (palikka.koodi == PALIKKA_HISSI_VERT)
						sprite_y = palikka.yla - sprite_korkeus /2;

					if (sprite_ala-1 >= palikka.yla && sprite_b >= 0)
					{
						if (palikka.koodi != PALIKKA_HISSI_HORI)
							sprite_y = palikka.yla - sprite_korkeus /2;
					}
				}
			}
			
			if (sprite_yla+sprite_b > palikka.yla)
			{
				if (palikka.ylos == PALIKKA_SEINA)
				{
					ylos = false;
					
					if (sprite_yla < palikka.ala)
					{
						if (palikka.koodi != PALIKKA_HISSI_HORI)
							sprite.kyykky = true;
					}
				}
			}
		}
	}
}


/*######################### LIIKUTA #################################*/

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

	//		palikat_x_lkm,
	//		palikat_y_lkm,
			
			x = 0, 
			y = 0;

	bool	oikealle,
			vasemmalle,
			ylos,
			alas,

			vedessa;

	UCHAR   max_nopeus;


void PK_Tutki_Seina(PK2Sprite &sprite, PK2PALIKKA &palikka)
{

	int mask_index;

	if (sprite_x <= palikka.oikea && sprite_x >= palikka.vasen && sprite_y <= palikka.ala && sprite_y >= palikka.yla)
	{
		
		/**********************************************************************/
		/* Tutkitaan onko seinän tausta vesitausta                            */
		/**********************************************************************/
		if (palikka.vesi)
		{
			sprite.vedessa = true;
		}
		
		/**********************************************************************/
		/* Tutkitaan onko seinä tuliansa, kytkin ylhäällä ja sprite valmis    */
		/**********************************************************************/		
		if (palikka.koodi == PALIKKA_TULI && kytkin1 == 0 && sprite.isku == 0)
		{
			sprite.saatu_vahinko = 2;
			sprite.saatu_vahinko_tyyppi = VAHINKO_TULI;
		}
		
		/**********************************************************************/
		/* Tutkitaan onko seinä piilopaikka                                   */
		/**********************************************************************/
		if (palikka.koodi == PALIKKA_PIILO)
			sprite.piilossa = true;
		
		/**********************************************************************/
		/* Tutkitaan onko seinä exit                                          */
		/**********************************************************************/
		if (palikka.koodi == PALIKKA_LOPETUS && sprite.pelaaja != 0)
		{
			if (!jakso_lapaisty)
			{
				if (PK_Midas_Lataa_Musiikki("music\\hiscore.xm")!=0)
					PK2_virhe = true;
				jakso_lapaisty = true;
				jaksot[jakso_indeksi_nyt].lapaisty = true;
				/*
				pisteet += jakso_pisteet;
				pisteet += aika*5;
				fake_pisteet = pisteet;
				*/
				if (jaksot[jakso_indeksi_nyt].jarjestys >= jakso)
					jakso++;
			}
		}
		/*
		if (palikka.koodi == PALIKKA_VIRTA_YLOS)
		{
			//sprite_b -= 0.25;
		}*/
	}

	if (sprite_vasen <= palikka.oikea-4 && sprite_oikea >= palikka.vasen+4 && sprite_yla <= palikka.ala && sprite_ala >= palikka.yla+16)
	{
		/**********************************************************************/
		/* Tutkitaan onko seinä tuliansa, kytkin ylhäällä ja sprite valmis    */
		/**********************************************************************/		
		if (palikka.koodi == PALIKKA_TULI && kytkin1 == 0 && sprite.isku == 0)
		{
			sprite.saatu_vahinko = 2;
			sprite.saatu_vahinko_tyyppi = VAHINKO_TULI;
		}
	}

	/* Tutkitaan onko kyseessä seinä joka blokkaa alaspäin menevät*/
	if ((palikka.koodi<80 || palikka.koodi>139) && palikka.koodi != PALIKKA_ESTO_ALAS && palikka.koodi < 150)
	{
		mask_index = (int)(sprite_x+sprite_a) - palikka.vasen;
	
		if (mask_index < 0)
			mask_index = 0;

		if (mask_index > 31)
			mask_index = 31;

		palikka.yla += palikkamaskit[palikka.koodi].alas[mask_index];
		
		if (palikka.yla >= palikka.ala-2)
			palikka.alas = PALIKKA_TAUSTA;
		
		palikka.ala -= palikkamaskit[palikka.koodi].ylos[mask_index];
		
	}

	if (sprite_vasen <= palikka.oikea+2 && sprite_oikea >= palikka.vasen-2 && sprite_yla <= palikka.ala && sprite_ala >= palikka.yla)
	{
		/**********************************************************************/
		/* Tutkitaan onko sprite avain ja seinä lukko                         */
		/**********************************************************************/ 
		if (palikka.koodi == PALIKKA_LUKKO && sprite.tyyppi->avain)
		{
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
		/* Tutkitaan onko sprite joutunut virtaukseen (liukuhihna, tuuli yms.)*/
		/**********************************************************************/		
		if (palikka.koodi == PALIKKA_VIRTA_VASEMMALLE && vasemmalle)
			sprite_a -= 0.02;
	
		if (palikka.koodi == PALIKKA_VIRTA_OIKEALLE && oikealle)
			sprite_a += 0.02;	//0.05
		
		/*********************************************************************/
		/* Tutkitaan onko sprite tullut reunalle josta se voi pudota         */
		/*********************************************************************/
		if (palikka.reuna && sprite.hyppy_ajastin <= 0 && sprite_y < palikka.ala && sprite_y > palikka.yla)
			/* && sprite_ala <= palikka.ala+2)*/ // onko sprite tullut reunalle
		{
			if (sprite_vasen > palikka.vasen)
				sprite.reuna_vasemmalla = true;
					
			if (sprite_oikea < palikka.oikea)
				sprite.reuna_oikealla = true;
		}	
	}

	// tarkistetaan onko palikka vasen tai oikea seinä.
	if (sprite_yla < palikka.ala && sprite_ala-1 > palikka.yla)
	{
		if (sprite_oikea+sprite_a-1 > palikka.vasen && sprite_vasen+sprite_a < palikka.oikea)
		{
			// Tutkitaan onko sprite menossa oikeanpuoleisen palikan sisään.
			if (sprite_oikea+sprite_a < palikka.oikea)
			{
				// Onko palikka seinä?
				if (palikka.oikealle == PALIKKA_SEINA)
				{
					oikealle = false;

					if (palikka.koodi == PALIKKA_HISSI_HORI)
						sprite_x = palikka.vasen - sprite_leveys/2;
				}
			}
			
			if (sprite_vasen+sprite_a > palikka.vasen)
			{
				if (palikka.vasemmalle == PALIKKA_SEINA)
				{
					vasemmalle = false;
					
					if (palikka.koodi == PALIKKA_HISSI_HORI)
						sprite_x = palikka.oikea + sprite_leveys/2;

				}
			}
		}
	}

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;				

	//...tarkistetaan, onko palikka lattia tai katto.

	if (sprite_vasen < palikka.oikea && sprite_oikea-1 > palikka.vasen)
	{
		if (sprite_ala+sprite_b-1 >= palikka.yla && sprite_yla+sprite_b <= palikka.ala)
		{
			if (sprite_ala+sprite_b-1 <= palikka.ala)
			{
				if (palikka.alas == PALIKKA_SEINA)
				{
					alas = false;
					
					if (palikka.koodi == PALIKKA_HISSI_VERT)
						sprite_y = palikka.yla - sprite_korkeus /2;

					if (sprite_ala-1 >= palikka.yla && sprite_b >= 0)
					{
						//sprite_y -= sprite_ala - palikka.yla;
						if (palikka.koodi != PALIKKA_HISSI_HORI)
						{
							sprite_y = palikka.yla - sprite_korkeus /2;
						}
					}

					if (sprite.kytkinpaino >= 1)
					{
						if (palikka.koodi == PALIKKA_KYTKIN1 && kytkin1 == 0)
						{
							kytkin1 = KYTKIN_ALOITUSARVO;
							kytkin_tarina = 64;
							PK_Soita_Aani(kytkin_aani, 100, (int)sprite_x, (int)sprite_y, AANET_SAMPLERATE, false);
						}
					
						if (palikka.koodi == PALIKKA_KYTKIN2 && kytkin2 == 0)
						{
							kytkin2 = KYTKIN_ALOITUSARVO;
							kytkin_tarina = 64;
							PK_Soita_Aani(kytkin_aani, 100, (int)sprite_x, (int)sprite_y, AANET_SAMPLERATE, false);
						}
					
						if (palikka.koodi == PALIKKA_KYTKIN3 && kytkin3 == 0)
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
				if (palikka.ylos == PALIKKA_SEINA)
				{
					ylos = false;
					
					if (sprite_yla < palikka.ala)
					{
						if (palikka.koodi == PALIKKA_HISSI_VERT && sprite.kyykky)
						{
							sprite.saatu_vahinko = 2;
							sprite.saatu_vahinko_tyyppi = VAHINKO_ISKU;
						}
						
						if (palikka.koodi != PALIKKA_HISSI_HORI)
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

void PK_Tutki_Seina_Debug(PK2Sprite &sprite, 
						  PK2PALIKKA &palikka, 	
					double  &sprite_x,
					double	&sprite_y,
					double	&sprite_a,
					double	&sprite_b,
					double	&sprite_vasen,
					double	&sprite_oikea,
					double	&sprite_yla,
					double	&sprite_ala,
					int		&sprite_leveys,
					int		&sprite_korkeus,
					int		&kartta_vasen,
					int		&kartta_yla,
					bool	&oikealle,
					bool	&vasemmalle,
					bool	&ylos,
					bool	&alas)
{
	int mask_index = 0;


	if (sprite_x <= palikka.oikea && sprite_x >= palikka.vasen && sprite_y <= palikka.ala && sprite_y >= palikka.yla)
	{
		
		/**********************************************************************/
		/* Tutkitaan onko seinän tausta vesitausta                            */
		/**********************************************************************/
		if (palikka.vesi)
		{
			sprite.vedessa = true;
		}
		
		/**********************************************************************/
		/* Tutkitaan onko seinä tuliansa, kytkin ylhäällä ja sprite valmis    */
		/**********************************************************************/		
/*		if (palikka.koodi == PALIKKA_TULI && kytkin1 == 0 && sprite.isku == 0)
		{
			sprite.saatu_vahinko = 2;
			sprite.saatu_vahinko_tyyppi = VAHINKO_TULI;
		}
		
		/**********************************************************************/
		/* Tutkitaan onko seinä piilopaikka                                   */
		/**********************************************************************/
		if (palikka.koodi == PALIKKA_PIILO)
			sprite.piilossa = true;
		
		/**********************************************************************/
		/* Tutkitaan onko seinä exit                                          */
		/**********************************************************************/
		if (palikka.koodi == PALIKKA_LOPETUS && sprite.pelaaja != 0)
		{
			if (!jakso_lapaisty)
			{
				if (PK_Midas_Lataa_Musiikki("music\\hiscore.xm")!=0)
					PK2_virhe = true;
				jakso_lapaisty = true;
				jaksot[jakso_indeksi_nyt].lapaisty = true;
				jakso++;
			}
		}
		/*
		if (palikka.koodi == PALIKKA_VIRTA_YLOS)
		{
			//sprite_b -= 0.25;
		}*/
	}

	if (sprite_vasen <= palikka.oikea-4 && sprite_oikea >= palikka.vasen+4 && sprite_yla <= palikka.ala && sprite_ala >= palikka.yla+16)
	{
		/**********************************************************************/
		/* Tutkitaan onko seinä tuliansa, kytkin ylhäällä ja sprite valmis    */
		/**********************************************************************/		
		if (palikka.koodi == PALIKKA_TULI && kytkin1 == 0 && sprite.isku == 0)
		{
			sprite.saatu_vahinko = 2;
			sprite.saatu_vahinko_tyyppi = VAHINKO_TULI;
		}
	}

	if ((palikka.koodi<80 || palikka.koodi>139) && palikka.koodi != PALIKKA_ESTO_ALAS && palikka.koodi < 150)
	{
		mask_index = (int)(sprite_x+sprite_a) - palikka.vasen;
	
		if (mask_index < 0)
			mask_index = 0;

		if (mask_index > 31)
			mask_index = 31;

		palikka.yla += palikkamaskit[palikka.koodi].alas[mask_index];
		
		if (palikka.yla >= palikka.ala-2)
			palikka.alas = PALIKKA_TAUSTA;
		
		palikka.ala -= palikkamaskit[palikka.koodi].ylos[mask_index];
		
	}

	if (sprite_vasen <= palikka.oikea+2 && sprite_oikea >= palikka.vasen-2 && sprite_yla <= palikka.ala && sprite_ala >= palikka.yla)
	{
		/**********************************************************************/
		/* Tutkitaan onko sprite avain ja seinä lukko                         */
		/**********************************************************************/ 
		if (palikka.koodi == PALIKKA_LUKKO && sprite.tyyppi->avain)
		{
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
		/* Tutkitaan onko sprite joutunut virtaukseen (liukuhihna, tuuli yms.)*/
		/**********************************************************************/		
		if (palikka.koodi == PALIKKA_VIRTA_VASEMMALLE && vasemmalle)
			sprite_a -= 0.02;
	
		if (palikka.koodi == PALIKKA_VIRTA_OIKEALLE && oikealle)
			sprite_a += 0.02;	//0.05
		
		/*********************************************************************/
		/* Tutkitaan onko sprite tullut reunalle josta se voi pudota         */
		/*********************************************************************/
		if (palikka.reuna && sprite.hyppy_ajastin <= 0 && sprite_y < palikka.ala && sprite_y > palikka.yla)
			/* && sprite_ala <= palikka.ala+2)*/ // onko sprite tullut reunalle
		{
			if (sprite_vasen > palikka.vasen)
				sprite.reuna_vasemmalla = true;
					
			if (sprite_oikea < palikka.oikea)
				sprite.reuna_oikealla = true;
		}	
	}

	// tarkistetaan onko palikka vasen tai oikea seinä.
	if (sprite_yla < palikka.ala && sprite_ala-1 > palikka.yla)
	{
		if (sprite_oikea+sprite_a-1 > palikka.vasen && sprite_vasen+sprite_a < palikka.oikea)
		{
			// Tutkitaan onko sprite menossa oikeanpuoleisen palikan sisään.
			if (sprite_oikea+sprite_a < palikka.oikea)
			{
				// Onko palikka seinä?
				if (palikka.oikealle == PALIKKA_SEINA)
				{
					oikealle = false;

					if (palikka.koodi == PALIKKA_HISSI_HORI)
						sprite_x = palikka.vasen - sprite_leveys/2;
				}
			}
			
			if (sprite_vasen+sprite_a > palikka.vasen)
			{
				if (palikka.vasemmalle == PALIKKA_SEINA)
				{
					vasemmalle = false;
					
					if (palikka.koodi == PALIKKA_HISSI_HORI)
						sprite_x = palikka.oikea + sprite_leveys/2;

				}
			}
		}
	}

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;				

	//...tarkistetaan, onko palikka lattia tai katto.

	if (sprite_vasen < palikka.oikea && sprite_oikea-1 > palikka.vasen)
	{
		if (sprite_ala+sprite_b-1 >= palikka.yla && sprite_yla+sprite_b <= palikka.ala)
		{
			if (sprite_ala+sprite_b-1 <= palikka.ala)
			{
				if (palikka.alas == PALIKKA_SEINA)
				{
					alas = false;
					
					if (palikka.koodi == PALIKKA_HISSI_VERT)
						sprite_y = palikka.yla - sprite_korkeus /2;

					if (sprite_ala-1 >= palikka.yla && sprite_b >= 0)
					{
						//sprite_y -= sprite_ala - palikka.yla;
						if (palikka.koodi != PALIKKA_HISSI_HORI)
						{
							sprite_y = palikka.yla - sprite_korkeus /2;
						}
					}

					if (sprite.kytkinpaino >= 1)
					{
						if (palikka.koodi == PALIKKA_KYTKIN1 && kytkin1 == 0)
						{
							kytkin1 = KYTKIN_ALOITUSARVO;
							kytkin_tarina = 64;
							PK_Soita_Aani(kytkin_aani, 100, (int)sprite_x, (int)sprite_y, AANET_SAMPLERATE, false);
						}
					
						if (palikka.koodi == PALIKKA_KYTKIN2 && kytkin2 == 0)
						{
							kytkin2 = KYTKIN_ALOITUSARVO;
							kytkin_tarina = 64;
							PK_Soita_Aani(kytkin_aani, 100, (int)sprite_x, (int)sprite_y, AANET_SAMPLERATE, false);
						}
					
						if (palikka.koodi == PALIKKA_KYTKIN3 && kytkin3 == 0)
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
				if (palikka.ylos == PALIKKA_SEINA)
				{
					ylos = false;
					
					if (sprite_yla < palikka.ala)
					{
						if (palikka.koodi == PALIKKA_HISSI_VERT && sprite.kyykky)
						{
							sprite.saatu_vahinko = 2;
							sprite.saatu_vahinko_tyyppi = VAHINKO_ISKU;
						}
						
						if (palikka.koodi != PALIKKA_HISSI_HORI)
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

int PK_Sprite_Liikuta(int i)
{
	
	if (i >= MAX_SPRITEJA || i < 0)
		return -1;

	if (!spritet[i].tyyppi)
		return -1;
/*	
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

	UCHAR   max_nopeus;
*/	
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

	x = 0;
	y = 0;

	oikealle	= false;
	vasemmalle  = false;
	ylos		= false;
	alas		= false;

	vedessa = false;

	max_nopeus = 0;	

	// Muuttujat
	/*
	PK2Sprite &sprite = spritet[i];

	double	sprite_x = sprite.x,
			sprite_y = sprite.y,
			sprite_a = sprite.a,
			sprite_b = sprite.b,

			sprite_vasen,
			sprite_oikea,
			sprite_yla,
			sprite_ala;

	int		sprite_leveys  = sprite.tyyppi->leveys,
			sprite_korkeus = sprite.tyyppi->korkeus;

	int		kartta_vasen,
			kartta_yla,

			palikat_x_lkm,
			palikat_y_lkm,
			
			x = 0, 
			y = 0;

	bool	oikealle	= true,
			vasemmalle	= true,
			ylos		= true,
			alas		= true;

	bool	vedessa = sprite.vedessa;

	UCHAR   max_nopeus = sprite.tyyppi->max_nopeus;
	*/

	if (spritet[i].tyyppi == NULL)
		return 1;

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

	sprite.kyykky = false;//sprite.piilossa;	//spriteä ei oletusarvoisesti kyykytetä

	sprite.reuna_vasemmalla = false;
	sprite.reuna_oikealla = false;

	// Siirretään varsinaiset muuttujat apumuuttujiin.

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;
	sprite_yla	 = sprite_y-sprite_korkeus/2;
	sprite_ala	 = sprite_y+sprite_korkeus/2;

	max_nopeus = (UCHAR)sprite.tyyppi->max_nopeus;

	/* Pistetään vauhtia tainnutettuihin spriteihin */
	if (sprite.energia < 1/* && sprite.tyyppi->max_nopeus == 0*/)
		max_nopeus = 3;

	// Lasketaan spriten jäljellä olevaa hyökkäystä

	if (sprite.hyokkays1 > 0)
		sprite.hyokkays1--;

	if (sprite.hyokkays2 > 0)
		sprite.hyokkays2--;

	if (sprite.lataus > 0)	// aika kahden ampumisen (munimisen) välillä
		sprite.lataus --;

	if (sprite.muutos_ajastin > 0)	// aika muutokseen
		sprite.muutos_ajastin --;

	/*****************************************************************************************/
	/* Pelaaja-sprite ja sen kontrollit                                                      */
	/*****************************************************************************************/

	bool lisavauhti = true;
	bool hidastus = false;
	

	if (sprite.pelaaja != 0 && sprite.energia > 0)
	{
		/* KÄVELY */
		if (PisteInput_Lue_Kontrolli(kontrolli_juoksu))
			lisavauhti = false;		

		/* HYÖKKÄYS 1 */
		if (PisteInput_Lue_Kontrolli(kontrolli_hyokkays1) && sprite.lataus == 0 && sprite.ammus1 != -1) {
			sprite.hyokkays1 = sprite.tyyppi->hyokkays1_aika;  
		}
		/* HYÖKKÄYS 2 */
		else {
			if (PisteInput_Lue_Kontrolli(kontrolli_hyokkays2) && sprite.lataus == 0 && sprite.ammus2 != -1)
				sprite.hyokkays2 = sprite.tyyppi->hyokkays2_aika;
		}

		/* KYYKISTYMINEN */
		sprite.kyykky = false;

		if (PisteInput_Lue_Kontrolli(kontrolli_alas) && !sprite.alas)
		{
			sprite.kyykky = true;
			sprite_yla += sprite_korkeus/1.5;
		}

		double a_lisays = 0;
		
		/* LIIKKUMINEN OIKEALLE */
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
		
		/* LIIKKUMINEN VASEMMALLE */
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
			
		if (sprite.kyykky)	// kyykyssä vauhti hiljenee
			a_lisays /= 10;

		sprite_a += a_lisays;

		/* HYPPÄÄMINEN */
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

			/* tippuminen hiljaa alaspäin */
			if (PisteInput_Lue_Kontrolli(kontrolli_hyppy) && sprite.hyppy_ajastin >= 150/*90+20*/ &&
				sprite.tyyppi->liitokyky)
				hidastus = true;
/*
			if (hidastus && sprite_b > 0)
			{
				sprite_b /= 4;//3
			}*/
		}
		/* LIIKKUMINEN YLÖS JA ALAS */
		else // jos pelaaja-spriten paino on 0 - esim. lintuna
		{
			if (PisteInput_Lue_Kontrolli(kontrolli_hyppy))
			{
				sprite_b -= 0.15;
			}
			
			if (PisteInput_Lue_Kontrolli(kontrolli_alas))
			{
				sprite_b += 0.15;
			}

			sprite.hyppy_ajastin = 0;
		}

		/* Tekoälyt, jotka koskevat myös pelaajaa */
		for (int ai=0;ai < SPRITE_MAX_AI;ai++)
			switch (sprite.tyyppi->AI[ai])
			{
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

		/* Ei käy päinsä, että pelaaja on jokin muu kuin pelihahmo */
		if (sprite.tyyppi->tyyppi != TYYPPI_PELIHAHMO)
			sprite.energia = 0;	
	}

	/*****************************************************************************************/
	/* Hyppy                                                                                 */
	/*****************************************************************************************/

	bool hyppy_maximissa = sprite.hyppy_ajastin >= 90;
	
	// Jos ollaan hypätty / ilmassa:
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
	/* Iskusta toipuminen                                                                    */
	/*****************************************************************************************/

	if (sprite.isku > 0)	// kuinka kauan sprite toipuu iskusta
		sprite.isku --;

	/*****************************************************************************************/
	/* Painovoiman vaikutus                                                                  */
	/*****************************************************************************************/
	
	if (sprite.paino != 0 && (sprite.hyppy_ajastin <= 0 || sprite.hyppy_ajastin >= 45))
		sprite_b += sprite.paino/1.25;// + sprite_b/1.5;

	if (hidastus && sprite_b > 0)
	{
		sprite_b /= 1.3;//1.5;//3
	}

	/*****************************************************************************************/
	/* Oletuksena sprite ei ole vedessä eikä piilossa                                        */
	/*****************************************************************************************/

	sprite.vedessa  = false;
	sprite.piilossa = false;

	/*****************************************************************************************/
	/* Nopeusrajoitukset                                                                     */
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
	/* Törmäykset palikoihin                                                                 */
	/*****************************************************************************************/

	int palikat_x_lkm,
		palikat_y_lkm,
		palikat_lkm;
	DWORD p;

	if (sprite.tyyppi->tiletarkistus)
	{

		palikat_x_lkm = (int)((sprite_leveys) /32)+4;
		palikat_y_lkm = (int)((sprite_korkeus)/32)+4;

		kartta_vasen = (int)(sprite_vasen)/32;
		kartta_yla	 = (int)(sprite_yla)/32;

		// LUETAAN PALIKAT KARTASTA JA TUTKITAAN NE

		//memset(palikat,255,sizeof(palikat));
/*
		for (y=0;y<palikat_y_lkm;y++)
			for (x=0;x<palikat_x_lkm;x++)
			{
				p = x+y*palikat_x_lkm;
				
				if (p<300 && p>=0)
					palikat[p] = PK_Palikka_Tutki(kartta_vasen+x-1,kartta_yla+y-1);
			}
/**/
		for (y=0;y<palikat_y_lkm;y++)
			for (x=0;x<palikat_x_lkm;x++)
			{
				palikat[x+(y*palikat_x_lkm)] = PK_Palikka_Tutki(kartta_vasen+x-1,kartta_yla+y-1);
			}
/**/
		/*****************************************************************************************/
		/* Käydään läpi spriten ympärillä olevat palikat                                         */
		/*****************************************************************************************/

		palikat_lkm = palikat_y_lkm*palikat_x_lkm;

		for (y=0;y<palikat_y_lkm;y++)
			for (x=0;x<palikat_x_lkm;x++) {			
				p = x+y*palikat_x_lkm;
				if (p<300 && p>=0)
					PK_Tutki_Seina(sprite, palikat[p]);
				/*
				PK_Tutki_Seina_Debug(sprite, palikat[p], 	
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
		
	}
	/*****************************************************************************************/
	/* Jos sprite on veden alla                                                              */
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

	if (vedessa != sprite.vedessa) // spriten joutuu veteen tai tulee pois vedestä
	{
		PK_Tehoste_Loiskahdus((int)sprite_x,(int)sprite_y,32); 
	}

	/*****************************************************************************************/
	/* Spriten peruspaino                                                                    */
	/*****************************************************************************************/	
	
	sprite.paino = sprite.alkupaino;
	sprite.kytkinpaino = sprite.paino;

	if (sprite.energia < 1 && sprite.paino == 0)
		sprite.paino = 1;	

	/*****************************************************************************************/
	/* Spriten törmäys toisiin spriteihin                                                    */
	/*****************************************************************************************/

	int tuhoutuminen;
	double sprite2_yla; // kyykistymiseen liittyvä
	PK2PALIKKA spritepalikka;

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

					//spritepalikka.koodi = PALIKKA_HISSI_VERT;

					PK_Palikka_Este(spritepalikka);

					if (!sprite.tyyppi->este)
					{
						if (!sprite2->tyyppi->este_alas)
							spritepalikka.alas		 = PALIKKA_TAUSTA;
						if (!sprite2->tyyppi->este_ylos)
							spritepalikka.ylos		 = PALIKKA_TAUSTA;
						if (!sprite2->tyyppi->este_oikealle)
							spritepalikka.oikealle   = PALIKKA_TAUSTA;
						if (!sprite2->tyyppi->este_vasemmalle)
							spritepalikka.vasemmalle = PALIKKA_TAUSTA;
					}
					
					if (sprite2->a > 0)
						spritepalikka.koodi = PALIKKA_HISSI_HORI;

					if (sprite2->b > 0)
						spritepalikka.koodi = PALIKKA_HISSI_VERT;


					PK_Tutki_Seina(sprite, spritepalikka, sprite_x, sprite_y, sprite_a, sprite_b,
							   sprite_vasen, sprite_oikea, sprite_yla, sprite_ala, sprite_leveys, sprite_korkeus,
							   kartta_vasen, kartta_yla, 
							   oikealle, vasemmalle, ylos, alas);
				}
			}			
			
			if (sprite_x <= sprite2->x + sprite2->tyyppi->leveys /2 &&
				sprite_x >= sprite2->x - sprite2->tyyppi->leveys /2 &&
				sprite_y/*yla*/ <= sprite2->y + sprite2->tyyppi->korkeus/2 &&
				sprite_y/*ala*/ >= sprite2->y - sprite2->tyyppi->korkeus/2 + sprite2_yla)
			{
				// samanmerkkiset spritet vaihtavat suuntaa törmätessään
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
					
						// Tippuuko toinen sprite päälle?

						if (sprite2->b > 2 && sprite2->paino >= 0.5 &&
							sprite2->y < sprite_y && !sprite.tyyppi->este &&
							sprite.tyyppi->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
						{
							//sprite.saatu_vahinko = (int)sprite2->paino;//1;
							sprite.saatu_vahinko = (int)(sprite2->paino+sprite2->b/4);
							sprite.saatu_vahinko_tyyppi = VAHINKO_PUDOTUS;
							sprite2->hyppy_ajastin = 1;
						}

						// Onko toinen sprite vahingollinen

						if (sprite.tyyppi->vahinko > 0 && sprite2->tyyppi->tyyppi != TYYPPI_BONUS)
						{
							sprite2->saatu_vahinko		  = sprite.tyyppi->vahinko;
							sprite2->saatu_vahinko_tyyppi = sprite.tyyppi->vahinko_tyyppi;
							sprite.hyokkays1 = sprite.tyyppi->hyokkays1_aika;
			
							// Ammukset hajoavat törmäyksestä
							
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
				
				// lisätään spriten painoon sitä koskettavan toisen spriten paino
				if (sprite.paino > 0)
					sprite.kytkinpaino += spritet[sprite_index].tyyppi->paino;

			}
		}
	}

	/*****************************************************************************************/
	/* Jos sprite on kärsinyt vahinkoa                                                       */
	/*****************************************************************************************/

	// jos näkymätön, ei saa damagea kuin tulesta.
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
		/* Jos sprite tuhoutuu                                                                   */
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
	/* Tarkistukset                                                                          */                      
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
		if (sprite_b >= 0)
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
	/* Apumuuttujien sijoitus spriten muuttujiin                                             */                      
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
	/* Tekoälyt                                                                              */
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
													PisteDraw_Fade_Paletti_In(PD_FADE_NORMAALI);
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
	/* Ei päästetä spriteä ulos pelialueelta                                                 */
	/*****************************************************************************************/

	if (sprite.x < 0)
		sprite.x = 0;
	
	if (sprite.y < -sprite_korkeus)
		sprite.y = -sprite_korkeus;

	if (sprite.x > PK2KARTTA_KARTTA_LEVEYS*32)
		sprite.x = PK2KARTTA_KARTTA_LEVEYS*32;
	
	// Jos sprite tippuu kartan alareunan ali
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
	/* Hyökkäykset 1 ja 2                                                                    */
	/*****************************************************************************************/

	// sprite on valmis hyökkäykseen ja ei ole kyykyssä
	if (sprite.lataus == 0 && !sprite.kyykky)
	{
		// hyökkäysaika on "tapissa" mikä tarkoittaa sitä, että aloitetaan hyökkäys
		if (sprite.hyokkays1 == sprite.tyyppi->hyokkays1_aika)
		{
			// määrätään palautumisaika, jonka jälkeen sprite voi hyökätä uudestaan
			sprite.lataus = sprite.tyyppi->latausaika;

			// jos spritelle ei ole määritelty omaa latausaikaa ...
			if (sprite.ammus1 > -1 && sprite.tyyppi->latausaika == 0)
			// ... ja ammukseen on, otetaan latausaika ammuksesta	
				if (protot[sprite.ammus1].tulitauko > 0)
					sprite.lataus = protot[sprite.ammus1].tulitauko;
			
			// soitetaan hyökkäysääni
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

		// Sama kuin hyökkäys 1:ssä
		if (sprite.hyokkays2 == sprite.tyyppi->hyokkays2_aika)
		{
			sprite.lataus = sprite.tyyppi->latausaika;
			
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

	// Random äänet
	if (sprite.tyyppi->aanet[AANI_RANDOM] != -1 && rand()%200 == 1 && sprite.energia > 0)
		PK_Soita_Aani(sprite.tyyppi->aanet[AANI_RANDOM],80,(int)sprite_x, (int)sprite_y, 
					  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);


	// KEHITYSVAIHEEN APUTOIMINTOJA

	UCHAR color;
	DWORD plk;

	if (PisteInput_Keydown(DIK_B) && huijaukset)
	{
		
		if (i == pelaaja_index)
		{
			
			char lukua[50];
			itoa(palikat[1].yla,lukua,10);
			//_gcvt(sprite_a,7,lukua);
			PisteDraw_Font_Kirjoita(fontti1,lukua,PD_TAUSTABUFFER,310,50);
			
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
					
					PisteDraw_Buffer_Tayta( PD_TAUSTABUFFER,
											palikat[plk].vasen-kamera_x,
											palikat[plk].yla-kamera_y,
											palikat[plk].oikea-kamera_x,
											palikat[plk].ala-kamera_y,
											color);
				}
			}
		
		PisteDraw_Buffer_Tayta( PD_TAUSTABUFFER,
								(int)(sprite_vasen-kamera_x),
								(int)(sprite_yla-kamera_y),
								(int)(sprite_oikea-kamera_x),
								(int)(sprite_ala-kamera_y),
								30);
	
	}



	return 0;
}



int PK_Sprite_Liikuta2(int i)
{
	
	if (i >= MAX_SPRITEJA || i < 0)
		return -1;

	if (!spritet[i].tyyppi)
		return -1;
/*	
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

	UCHAR   max_nopeus;
*/	
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

	x = 0;
	y = 0;

	oikealle	= false;
	vasemmalle  = false;
	ylos		= false;
	alas		= false;

	vedessa = false;

	max_nopeus = 0;	

	// Muuttujat
	/*
	PK2Sprite &sprite = spritet[i];

	double	sprite_x = sprite.x,
			sprite_y = sprite.y,
			sprite_a = sprite.a,
			sprite_b = sprite.b,

			sprite_vasen,
			sprite_oikea,
			sprite_yla,
			sprite_ala;

	int		sprite_leveys  = sprite.tyyppi->leveys,
			sprite_korkeus = sprite.tyyppi->korkeus;

	int		kartta_vasen,
			kartta_yla,

			palikat_x_lkm,
			palikat_y_lkm,
			
			x = 0, 
			y = 0;

	bool	oikealle	= true,
			vasemmalle	= true,
			ylos		= true,
			alas		= true;

	bool	vedessa = sprite.vedessa;

	UCHAR   max_nopeus = sprite.tyyppi->max_nopeus;
	*/

	if (spritet[i].tyyppi == NULL)
		return 1;

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

	sprite.kyykky = false;//sprite.piilossa;	//spriteä ei oletusarvoisesti kyykytetä

	sprite.reuna_vasemmalla = false;
	sprite.reuna_oikealla = false;

	// Siirretään varsinaiset muuttujat apumuuttujiin.

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;
	sprite_yla	 = sprite_y-sprite_korkeus/2;
	sprite_ala	 = sprite_y+sprite_korkeus/2;

	max_nopeus = (UCHAR)sprite.tyyppi->max_nopeus;

	/* Pistetään vauhtia tainnutettuihin spriteihin */
	if (sprite.energia < 1/* && sprite.tyyppi->max_nopeus == 0*/)
		max_nopeus = 3;

	// Lasketaan spriten jäljellä olevaa hyökkäystä

	if (sprite.hyokkays1 > 0)
		sprite.hyokkays1--;

	if (sprite.hyokkays2 > 0)
		sprite.hyokkays2--;

	if (sprite.lataus > 0)	// aika kahden ampumisen (munimisen) välillä
		sprite.lataus --;

	/*****************************************************************************************/
	/* Pelaaja-sprite ja sen kontrollit                                                      */
	/*****************************************************************************************/

	bool lisavauhti = true;
	bool hidastus = false;
	

	if (sprite.pelaaja != 0 && sprite.energia > 0)
	{
		/* KÄVELY */
		if (PisteInput_Lue_Kontrolli(kontrolli_juoksu))
			lisavauhti = false;		

		/* HYÖKKÄYS 1 */
		if (PisteInput_Lue_Kontrolli(kontrolli_hyokkays1) && sprite.lataus == 0 && sprite.ammus1 != -1)
			sprite.hyokkays1 = sprite.tyyppi->hyokkays1_aika;  

		/* HYÖKKÄYS 2 */
		if (PisteInput_Lue_Kontrolli(kontrolli_hyokkays2) && sprite.lataus == 0 && sprite.ammus2 != -1)
			sprite.hyokkays2 = sprite.tyyppi->hyokkays2_aika;

		/* KYYKISTYMINEN */
		sprite.kyykky = false;

		if (PisteInput_Lue_Kontrolli(kontrolli_alas) && !sprite.alas)
		{
			sprite.kyykky = true;
			sprite_yla += sprite_korkeus/1.5;
		}

		double a_lisays = 0;
		
		/* LIIKKUMINEN OIKEALLE */
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
		
		/* LIIKKUMINEN VASEMMALLE */
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
			
		if (sprite.kyykky)	// kyykyssä vauhti hiljenee
			a_lisays /= 10;

		sprite_a += a_lisays;

		/* HYPPÄÄMINEN */
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
						sprite.hyppy_ajastin = 1;	
				}
			}
			else
			{
				if (sprite.hyppy_ajastin > 0 && sprite.hyppy_ajastin < sprite.tyyppi->max_hyppy/2) // /2
					sprite.hyppy_ajastin = sprite.tyyppi->max_hyppy/2;// /2
			}

			/* tippuminen hiljaa alaspäin */
			if (PisteInput_Lue_Kontrolli(kontrolli_hyppy) && sprite.hyppy_ajastin > sprite.tyyppi->max_hyppy*2+20 &&
				sprite.tyyppi->liitokyky)
				hidastus = true;

			if (hidastus && sprite_b > 0)
			{
				sprite_b /= 4;//3
			}
		}
		/* LIIKKUMINEN YLÖS JA ALAS */
		else // jos pelaaja-spriten paino on 0 - esim. lintuna
		{
			if (PisteInput_Lue_Kontrolli(kontrolli_hyppy))
			{
				sprite_b -= 0.15;
			}
			
			if (PisteInput_Lue_Kontrolli(kontrolli_alas))
			{
				sprite_b += 0.15;
			}

			sprite.hyppy_ajastin = 0;
		}

		/* Tekoälyt, jotka koskevat myös pelaajaa */
		for (int ai=0;ai < SPRITE_MAX_AI;ai++)
			switch (sprite.tyyppi->AI[ai])
			{
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
			default:			break;
			}

		/* Ei käy päinsä, että pelaaja on jokin muu kuin pelihahmo */
		if (sprite.tyyppi->tyyppi != TYYPPI_PELIHAHMO)
			sprite.energia = 0;	
	}

	/*****************************************************************************************/
	/* Hyppy                                                                                 */
	/*****************************************************************************************/
/*  vanha hyppy
	bool hyppy_maximissa = sprite.hyppy_ajastin >= sprite.tyyppi->max_hyppy*2;

	// Jos ollaan hypätty / ilmassa:
	if (sprite.hyppy_ajastin > 0)
	{
		if (!hyppy_maximissa)
		// sprite_b = (sprite.tyyppi->max_hyppy/2 - sprite.hyppy_ajastin/2)/-2.0;//-4
		   sprite_b = (sprite.tyyppi->max_hyppy/2 - sprite.hyppy_ajastin/2)/-2.5;

		if (sprite.hyppy_ajastin < 100)
			sprite.hyppy_ajastin ++;	
	}	

	if (sprite.hyppy_ajastin < 0)
		sprite.hyppy_ajastin++;

	if (sprite_b > 0 && !hyppy_maximissa)
		sprite.hyppy_ajastin = sprite.tyyppi->max_hyppy*2;
*/
	bool hyppy_maximissa = sprite.hyppy_ajastin >= 45;

	// Jos ollaan hypätty / ilmassa:
	if (sprite.hyppy_ajastin > 0)
	{
		if (!hyppy_maximissa)
		// sprite_b = (sprite.tyyppi->max_hyppy/2 - sprite.hyppy_ajastin/2)/-2.0;//-4
		   sprite_b -= sin_table[sprite.hyppy_ajastin]/3;//(sprite.tyyppi->max_hyppy/2 - sprite.hyppy_ajastin/2)/-2.5;

		if (sprite.hyppy_ajastin < 90)
			sprite.hyppy_ajastin ++;	
	}	

	if (sprite.hyppy_ajastin < 0)
		sprite.hyppy_ajastin++;

	if (sprite_b > 0 && !hyppy_maximissa)
		sprite.hyppy_ajastin = 45;//sprite.tyyppi->max_hyppy*2;

	/*****************************************************************************************/
	/* Iskusta toipuminen                                                                    */
	/*****************************************************************************************/

	if (sprite.isku > 0)	// kuinka kauan sprite toipuu iskusta
		sprite.isku --;

	/*****************************************************************************************/
	/* Painovoiman vaikutus                                                                  */
	/*****************************************************************************************/
	
	if (sprite.paino != 0)
		sprite_b += sprite.paino + sprite_b/1.25;

	/*****************************************************************************************/
	/* Oletuksena sprite ei ole vedessä eikä piilossa                                        */
	/*****************************************************************************************/

	sprite.vedessa  = false;
	sprite.piilossa = false;

	/*****************************************************************************************/
	/* Nopeusrajoitukset                                                                     */
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
	/* Törmäykset palikoihin                                                                 */
	/*****************************************************************************************/

	int palikat_x_lkm,
		palikat_y_lkm,
		palikat_lkm;
	DWORD p;

	if (sprite.tyyppi->tiletarkistus)
	{

		palikat_x_lkm = (int)((sprite_leveys) /32)+4;
		palikat_y_lkm = (int)((sprite_korkeus)/32)+4;

		kartta_vasen = (int)(sprite_vasen)/32;
		kartta_yla	 = (int)(sprite_yla)/32;

		// LUETAAN PALIKAT KARTASTA JA TUTKITAAN NE

		//memset(palikat,255,sizeof(palikat));
/*
		for (y=0;y<palikat_y_lkm;y++)
			for (x=0;x<palikat_x_lkm;x++)
			{
				p = x+y*palikat_x_lkm;
				
				if (p<300 && p>=0)
					palikat[p] = PK_Palikka_Tutki(kartta_vasen+x-1,kartta_yla+y-1);
			}
/**/
		for (int py=0;py<palikat_y_lkm;py++)
			for (int px=0;px<palikat_x_lkm;px++)
			{
				palikat[px+(py*palikat_x_lkm)] = PK_Palikka_Tutki(kartta_vasen+px-1,kartta_yla+py-1);
			}
/**/
		/*****************************************************************************************/
		/* Käydään läpi spriten ympärillä olevat palikat                                         */
		/*****************************************************************************************/

		palikat_lkm = palikat_y_lkm*palikat_x_lkm;

		for (y=0;y<palikat_y_lkm;y++)
			for (x=0;x<palikat_x_lkm;x++) {			
				p = x+y*palikat_x_lkm;
				
				/* debuggausta...
				if (i==pelaaja_index) {
					char luku[20];
					itoa(palikat[p].koodi,luku,10);
					PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,10+x*50,60+y*10);
				}*/

				if (p<300 && p>=0)
					PK_Tutki_Seina(sprite, palikat[p]);
				/*
				PK_Tutki_Seina_Debug(sprite, palikat[p], 	
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
		
	}
	/*****************************************************************************************/
	/* Jos sprite on veden alla                                                              */
	/*****************************************************************************************/

	if (sprite.vedessa)
	{
		
		if (!sprite.tyyppi->osaa_uida || sprite.energia < 1)
		{
			if (sprite_b > 0)
				sprite_b /= 2.0;
		
			sprite_b -= (1.5-sprite.paino)/10;
			sprite_a /= 1.05;
		}

		if (rand()%80 == 1)
			PK_Partikkeli_Uusi(PARTIKKELI_KIPINA,sprite_x-4,sprite_y,0,-0.5-rand()%2,rand()%30+30,0,32);
	}

	if (vedessa != sprite.vedessa) // spriten joutuu veteen tai tulee pois vedestä
	{
		PK_Tehoste_Loiskahdus((int)sprite_x,(int)sprite_y,32); 
	}

	/*****************************************************************************************/
	/* Spriten peruspaino                                                                    */
	/*****************************************************************************************/	
	
	sprite.paino = sprite.alkupaino;
	sprite.kytkinpaino = sprite.paino;

	if (sprite.energia < 1 && sprite.paino == 0)
		sprite.paino = 1;	

	/*****************************************************************************************/
	/* Spriten törmäys toisiin spriteihin                                                    */
	/*****************************************************************************************/

	int tuhoutuminen;
	double sprite2_yla; // kyykistymiseen liittyvä
	PK2PALIKKA spritepalikka;

	PK2Sprite *sprite2;

	for (int sprite_index = 0; sprite_index < MAX_SPRITEJA; sprite_index++)
	{
		sprite2 = &spritet[sprite_index];
		
		if (sprite_index != i && !sprite2->piilota)
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

					//spritepalikka.koodi = PALIKKA_HISSI_VERT;

					PK_Palikka_Este(spritepalikka);

					if (!sprite.tyyppi->este)
					{
						if (!sprite2->tyyppi->este_alas)
							spritepalikka.alas		 = PALIKKA_TAUSTA;
						if (!sprite2->tyyppi->este_ylos)
							spritepalikka.ylos		 = PALIKKA_TAUSTA;
						if (!sprite2->tyyppi->este_oikealle)
							spritepalikka.oikealle   = PALIKKA_TAUSTA;
						if (!sprite2->tyyppi->este_vasemmalle)
							spritepalikka.vasemmalle = PALIKKA_TAUSTA;
					}
					
					if (sprite2->a > 0)
						spritepalikka.koodi = PALIKKA_HISSI_HORI;

					if (sprite2->b > 0)
						spritepalikka.koodi = PALIKKA_HISSI_VERT;


					PK_Tutki_Seina(sprite, spritepalikka, sprite_x, sprite_y, sprite_a, sprite_b,
							   sprite_vasen, sprite_oikea, sprite_yla, sprite_ala, sprite_leveys, sprite_korkeus,
							   kartta_vasen, kartta_yla, 
							   oikealle, vasemmalle, ylos, alas);
				}
			}			
			
			if (sprite_x <= sprite2->x + sprite2->tyyppi->leveys /2 &&
				sprite_x >= sprite2->x - sprite2->tyyppi->leveys /2 &&
				sprite_y/*yla*/ <= sprite2->y + sprite2->tyyppi->korkeus/2 &&
				sprite_y/*ala*/ >= sprite2->y - sprite2->tyyppi->korkeus/2 + sprite2_yla)
			{
				// samanmerkkiset spritet vaihtavat suuntaa törmätessään
				if (sprite.tyyppi->indeksi == sprite2->tyyppi->indeksi &&
					sprite2->energia > 0 && sprite.pelaaja == 0)
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
					
						// Tippuuko toinen sprite päälle?

						if (sprite2->b > 2 && sprite2->paino >= 0.5 &&
							sprite2->y < sprite_y && !sprite.tyyppi->este &&
							sprite.tyyppi->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
						{
							//sprite.saatu_vahinko = (int)sprite2->paino;//1;
							sprite.saatu_vahinko = (int)(sprite2->paino+sprite2->b/4);
							sprite.saatu_vahinko_tyyppi = VAHINKO_PUDOTUS;
							sprite2->hyppy_ajastin = 1;
						}

						// Onko toinen sprite vahingollinen

						if (sprite.tyyppi->vahinko > 0 && sprite2->tyyppi->tyyppi != TYYPPI_BONUS)
						{
							sprite2->saatu_vahinko		  = sprite.tyyppi->vahinko;
							sprite2->saatu_vahinko_tyyppi = sprite.tyyppi->vahinko_tyyppi;
							sprite.hyokkays1 = sprite.tyyppi->hyokkays1_aika;
			
							// Ammukset hajoavat törmäyksestä
							
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
				
				// lisätään spriten painoon sitä koskettavan toisen spriten paino
				if (sprite.paino > 0)
					sprite.kytkinpaino += spritet[sprite_index].tyyppi->paino;

			}
		}
	}

	/*****************************************************************************************/
	/* Jos sprite on kärsinyt vahinkoa                                                       */
	/*****************************************************************************************/

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
		/* Jos sprite tuhoutuu                                                                   */
		/*****************************************************************************************/

		if (sprite.energia < 1)
		{
			tuhoutuminen = sprite.tyyppi->tuhoutuminen;

			if (tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
			{
				if (sprite.tyyppi->bonus > -1 && sprite.tyyppi->bonusten_lkm > 0)
					if (/*sprite.Onko_AI(AI_BONUS)*/sprite.tyyppi->bonus_aina || rand()%4 == 1)
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
	/* Tarkistukset                                                                          */                      
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
			sprite.hyppy_ajastin = sprite.tyyppi->max_hyppy * 2;
	}

	if (!alas)
	{
		if (sprite_b >= 0)
		{
			if (sprite.hyppy_ajastin > 0)
			{
				if (sprite.hyppy_ajastin > sprite.tyyppi->max_hyppy * 2 + 10)
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
	/* Apumuuttujien sijoitus spriten muuttujiin                                             */                      
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
		double kitka = 1.04;//1.04

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
	/* Tekoälyt                                                                              */
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
			case AI_SEURAA_PELAAJAA:			sprite.AI_Seuraa_Pelaajaa(spritet[pelaaja_index]);
												break;
			case AI_SEURAA_PELAAJAA_JOS_NAKEE:	sprite.AI_Seuraa_Pelaajaa_Jos_Nakee(spritet[pelaaja_index]);
												break;
			case AI_SEURAA_PELAAJAA_VERT_HORI:	sprite.AI_Seuraa_Pelaajaa_Vert_Hori(spritet[pelaaja_index]);
												break;
			case AI_SEURAA_PELAAJAA_JOS_NAKEE_VERT_HORI:	sprite.AI_Seuraa_Pelaajaa_Jos_Nakee_Vert_Hori(spritet[pelaaja_index]);
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
			case AI_HYOKKAYS_1_JOS_PELAAJA_EDESSA:		sprite.AI_Hyokkays_1_Jos_Pelaaja_Edessa(spritet[pelaaja_index]);
												break;
			case AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA:		sprite.AI_Hyokkays_2_Jos_Pelaaja_Edessa(spritet[pelaaja_index]);
												break;
			case AI_HYOKKAYS_1_JOS_PELAAJA_ALAPUOLELLA:	sprite.AI_Hyokkays_1_Jos_Pelaaja_Alapuolella(spritet[pelaaja_index]);
												break;
			case AI_HYPPY_JOS_PELAAJA_YLAPUOLELLA:		sprite.AI_Hyppy_Jos_Pelaaja_Ylapuolella(spritet[pelaaja_index]);
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
			case AI_MUUTOS_AJASTIN:				if (sprite.tyyppi->muutos > -1) 
													sprite.AI_Muutos_Ajastin(protot[sprite.tyyppi->muutos]);
												break;
			case AI_TELEPORTTI:					if (sprite.AI_Teleportti(i, spritet, MAX_SPRITEJA, spritet[pelaaja_index])==1)
												{
													
													kamera_x = (int)spritet[pelaaja_index].x;
													kamera_y = (int)spritet[pelaaja_index].y;
													dkamera_x = kamera_x-KARTANPIIRTO_LEVEYS/2;
													dkamera_y = kamera_y-KARTANPIIRTO_KORKEUS/2;
													PisteDraw_Fade_Paletti_In(PD_FADE_NORMAALI);
													if (sprite.tyyppi->aanet[AANI_HYOKKAYS2] != -1)
														PK_Soita_Aani_Menu(sprite.tyyppi->aanet[AANI_HYOKKAYS2], 100);
														//PK_Soita_Aani(, 100, kamera_x, kamera_y, AANET_SAMPLERATE, false);

													
												}
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
	/* Ei päästetä spriteä ulos pelialueelta                                                 */
	/*****************************************************************************************/

	if (sprite.x < 0)
		sprite.x = 0;
	
	if (sprite.y < -sprite_korkeus)
		sprite.y = -sprite_korkeus;

	if (sprite.x > PK2KARTTA_KARTTA_LEVEYS*32)
		sprite.x = PK2KARTTA_KARTTA_LEVEYS*32;
	
	// Jos sprite tippuu kartan alareunan ali
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
	/* Hyökkäykset 1 ja 2                                                                    */
	/*****************************************************************************************/

	// sprite on valmis hyökkäykseen ja ei ole kyykyssä
	if (sprite.lataus == 0 && !sprite.kyykky)
	{
		// hyökkäysaika on "tapissa" mikä tarkoittaa sitä, että aloitetaan hyökkäys
		if (sprite.hyokkays1 == sprite.tyyppi->hyokkays1_aika)
		{
			// määrätään palautumisaika, jonka jälkeen sprite voi hyökätä uudestaan
			sprite.lataus = sprite.tyyppi->latausaika;

			// jos spritelle ei ole määritelty omaa latausaikaa ...
			if (sprite.ammus1 > -1 && sprite.tyyppi->latausaika == 0)
			// ... ja ammukseen on, otetaan latausaika ammuksesta	
				if (protot[sprite.ammus1].tulitauko > 0)
					sprite.lataus = protot[sprite.ammus1].tulitauko;
			
			// soitetaan hyökkäysääni
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

		// Sama kuin hyökkäys 1:ssä
		if (sprite.hyokkays2 == sprite.tyyppi->hyokkays2_aika)
		{
			sprite.lataus = sprite.tyyppi->latausaika;
			
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

	// Random äänet
	if (sprite.tyyppi->aanet[AANI_RANDOM] != -1 && rand()%200 == 1 && sprite.energia > 0)
		PK_Soita_Aani(sprite.tyyppi->aanet[AANI_RANDOM],80,(int)sprite_x, (int)sprite_y, 
					  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);


	// KEHITYSVAIHEEN APUTOIMINTOJA

	UCHAR color;
	DWORD plk;

	if (PisteInput_Keydown(DIK_B) && huijaukset)
	{
		
		if (i == pelaaja_index)
		{
			
			char lukua[50];
			itoa(palikat[1].yla,lukua,10);
			//_gcvt(sprite_a,7,lukua);
			PisteDraw_Font_Kirjoita(fontti1,lukua,PD_TAUSTABUFFER,310,50);
			
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
					
					PisteDraw_Buffer_Tayta( PD_TAUSTABUFFER,
											palikat[plk].vasen-kamera_x,
											palikat[plk].yla-kamera_y,
											palikat[plk].oikea-kamera_x,
											palikat[plk].ala-kamera_y,
											color);
				}
			}
		
		PisteDraw_Buffer_Tayta( PD_TAUSTABUFFER,
								(int)(sprite_vasen-kamera_x),
								(int)(sprite_yla-kamera_y),
								(int)(sprite_oikea-kamera_x),
								(int)(sprite_ala-kamera_y),
								30);
	
	}



	return 0;
}

int PK_Sprite_Liikuta_Bonus(int i)
{
	// Muuttujat
/*
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

	//		palikat_x_lkm,
	//		palikat_y_lkm,
			
			x = 0, 
			y = 0;

	bool	oikealle,
			vasemmalle,
			ylos,
			alas,

			vedessa;

	UCHAR   max_nopeus;
*/
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
	
	/*
	PK2Sprite &sprite = spritet[i];
	
	double	sprite_x = sprite.x,
			sprite_y = sprite.y,
			sprite_a = sprite.a,
			sprite_b = sprite.b,

			sprite_vasen,
			sprite_oikea,
			sprite_yla,
			sprite_ala;

	int		sprite_leveys  = sprite.tyyppi->leveys,
			sprite_korkeus = sprite.tyyppi->korkeus;

	int		kartta_vasen,
			kartta_yla,

			palikat_x_lkm,
			palikat_y_lkm,
			
			x = 0, 
			y = 0;

	bool	oikealle	= true,
			vasemmalle	= true,
			ylos		= true,
			alas		= true;

	bool	vedessa = sprite.vedessa;
*/
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

	// Siirretään varsinaiset muuttujat apumuuttujiin.

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

	// Hyppyyn liittyvät seikat

	if (kytkin_tarina + jaristys > 0 && sprite.hyppy_ajastin == 0)
		sprite.hyppy_ajastin = sprite.tyyppi->max_hyppy / 2;

	if (sprite.hyppy_ajastin > 0 && sprite.hyppy_ajastin < sprite.tyyppi->max_hyppy)
	{
		sprite.hyppy_ajastin ++; 
		sprite_b = (sprite.tyyppi->max_hyppy - sprite.hyppy_ajastin)/-4.0;//-2
	}	

	if (sprite_b > 0)
		sprite.hyppy_ajastin = sprite.tyyppi->max_hyppy;



	if (sprite.paino != 0)	// jos bonuksella on paino, tutkitaan ympäristö
	{
		// o
		//
		// |  Painovoima
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

		PK2PALIKKA spritepalikka;

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
					
						spritepalikka.alas       = PALIKKA_SEINA;
						spritepalikka.ylos       = PALIKKA_SEINA;
						spritepalikka.oikealle   = PALIKKA_SEINA;
						spritepalikka.vasemmalle = PALIKKA_SEINA;
					
						if (!spritet[sprite_index].tyyppi->este_alas)
							spritepalikka.alas		 = PALIKKA_TAUSTA;
						if (!spritet[sprite_index].tyyppi->este_ylos)
							spritepalikka.ylos		 = PALIKKA_TAUSTA;
						if (!spritet[sprite_index].tyyppi->este_oikealle)
							spritepalikka.oikealle   = PALIKKA_TAUSTA;
						if (!spritet[sprite_index].tyyppi->este_vasemmalle)
							spritepalikka.vasemmalle = PALIKKA_TAUSTA;

						
						spritepalikka.vesi  = false;
							
						PK_Palikka_Este(spritepalikka);

						PK_Tutki_Seina(sprite, spritepalikka, sprite_x, sprite_y, sprite_a, sprite_b,
								   sprite_vasen, sprite_oikea, sprite_yla, sprite_ala, sprite_leveys, sprite_korkeus,
								   kartta_vasen, kartta_yla, 
								   oikealle, vasemmalle, ylos, alas);
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

					// lisätään spriten painoon sitä koskettavan toisen spriten paino
					sprite.kytkinpaino += spritet[sprite_index].tyyppi->paino;

					// samanmerkkiset spritet vaihtavat suuntaa törmätessään
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

		// Tarkistetaan ettei mennä mihinkään suuntaan liian kovaa.

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

			// Tutkitaan törmääkö palikkaan

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
	else	// jos spriten paino on nolla, tehdään spritestä "kelluva"
	{
		sprite.y = sprite.alku_y + cos_table[int(degree+(sprite.alku_x+sprite.alku_y))%360] / 3.0;
		sprite_y = sprite.y;
	}

	sprite.paino = sprite.alkupaino;

	int tuhoutuminen;
	//////////////////////////////////////////
	// tutkitaan koskettaako bonus pelaajaa //
	//////////////////////////////////////////
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
//					ammuksia1 += sprite.tyyppi->bonusten_lkm;
				}

				if (sprite.tyyppi->ammus2 != -1)
				{
					spritet[pelaaja_index].ammus2 = sprite.tyyppi->ammus2;
					PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_game_newdoodle));
//					ammuksia2 += sprite.tyyppi->bonusten_lkm;
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
	
	/* Ei käy päinsä, että pelaaja on bonusesine */
	if (sprite.pelaaja != 0)
		sprite.energia = 0;

	return 0;
}

void PK_Aseta_Marginaalit(bool hori, bool vert, int leveys, int korkeus)
{
	if (RAJAA_KARTANPIIRTOALUE)
	{
		if (RUUDUN_LEVEYS > leveys && hori)
			MARGINAALI_HORI = (RUUDUN_LEVEYS - leveys) / 2;
		else
			MARGINAALI_HORI = 0;
			
		if (RUUDUN_KORKEUS > korkeus && vert)
			MARGINAALI_VERT = (RUUDUN_KORKEUS - korkeus) / 2;
		else
			MARGINAALI_VERT = 0;

		PisteDraw_Aseta_Marginaali_Vasen(MARGINAALI_HORI);
		PisteDraw_Aseta_Marginaali_Yla(MARGINAALI_VERT);
	}
}

int PK_Alusta_Tilat()
{
	if (pelin_seuraava_tila != pelin_tila)
	{
		
		// IHAN ENSIMMÄINEN PERUSALUSTUS
		
		if (pelin_seuraava_tila == TILA_PERUSALUSTUS)
		{
			PisteLog_Kirjoita("- Initializing basic stuff \n");
			//PK_Lataa_Kieli("language\\suomi.txt");
			
			strcpy(pelaajan_nimi,tekstit->Hae_Teksti(txt_player_default_name));

			srand( (unsigned)time( NULL ) );
			
			_getcwd(tyohakemisto, _MAX_PATH);

			PisteLog_Kirjoita("  - Work directory: ");
			PisteLog_Kirjoita(tyohakemisto);
			PisteLog_Kirjoita("\n");
			
			strcpy(viesti,"no message");

			strcpy(episodi,"");
			//strcpy(episodin_hakemisto,"");

			strcpy(seuraava_kartta,"untitle1.map");

			jakso = 1;

			if (!taulut_laskettu) {
				PisteLog_Kirjoita("  - Precalculating... \n");
				PK_Laske_Taulut();

				PK2Kartta_Cos_Sin(cos_table, sin_table);
				taulut_laskettu = true;
			}
			
			PK_Alusta_Fadetekstit();

			if (KARTANPIIRTO_LEVEYS > RUUDUN_LEVEYS)
				KARTANPIIRTO_LEVEYS = RUUDUN_LEVEYS;
			
			if (KARTANPIIRTO_KORKEUS > RUUDUN_KORKEUS)
				KARTANPIIRTO_KORKEUS = RUUDUN_KORKEUS;

			if (!RAJAA_KARTANPIIRTOALUE)
			{

				KARTANPIIRTO_LEVEYS = RUUDUN_LEVEYS;
				KARTANPIIRTO_KORKEUS = RUUDUN_KORKEUS;
			}
			else
			{
				/*
				if (RUUDUN_LEVEYS > 640)
					KARTANPIIRTO_LEVEYS = 640;
				else
					KARTANPIIRTO_LEVEYS = RUUDUN_LEVEYS;

				if (RUUDUN_KORKEUS > 480)
					KARTANPIIRTO_KORKEUS = 480;
				else
					KARTANPIIRTO_KORKEUS = RUUDUN_KORKEUS;
					*/
			}

			PK2Kartta_Aseta_Ruudun_Mitat(KARTANPIIRTO_LEVEYS,KARTANPIIRTO_KORKEUS);

			PisteLog_Kirjoita("  - Initializing PisteSound \n");
			if ((PisteSound_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &) hinstance_app, 
				AANET_STEREO, AANET_SAMPLERATE, AANET_BITRATE)) == PD_VIRHE) {
				PK2_virhe = true;
				PisteLog_Kirjoita("  - Initializing PisteSound failed!\n");
			}

			PisteLog_Kirjoita("  - Initializing Midas \n");
			PK_Midas_Alusta();

			PisteLog_Kirjoita("  - Initializing PisteInput \n");
			if ((PisteInput_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &)hinstance_app)) == PI_VIRHE) {
				PK2_virhe = true;
				PisteLog_Kirjoita("  - Initializing PisteInput failed!\n");
			}

			PisteLog_Kirjoita("  - Initializing PisteDraw \n");
			if ((PisteDraw_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &) hinstance_app, 
				RUUDUN_LEVEYS, RUUDUN_KORKEUS, RUUDUN_BPP, MAX_COLORS_PALETTE)) == PD_VIRHE) {
				PK2_virhe = true;
				PisteLog_Kirjoita("  - Initializing PisteDraw failed!\n");
			}

			PisteLog_Kirjoita("  - Creating an empty map class \n");
			kartta = new PK2Kartta();

			PisteLog_Kirjoita("  - Creating a buffer for game's stuff bitmap \n");
			if ((kuva_peli = PisteDraw_Buffer_Uusi(640,480, true, 255)) == PD_VIRHE){
				PK2_virhe = true;
				PisteLog_Kirjoita("  - Creating buffer failed!\n");
				return -1;
			}

			if ((kuva_peli_sysmem = PisteDraw_Buffer_Uusi(640,480, false, 255)) == PD_VIRHE){
				PK2_virhe = true;
				PisteLog_Kirjoita("  - Creating buffer failed!\n");
				return -1;
			}

			PisteLog_Kirjoita("  - Loading pk2stuff.pcx \n");
			if (PisteDraw_Lataa_Kuva(kuva_peli,"gfx\\pk2stuff.bmp", true) == PD_VIRHE){
				PK2_virhe = true;
				PisteLog_Kirjoita("  - Loading pk2stuff.bmp failed!\n");
				return -1;
			}

			if (PisteDraw_Lataa_Kuva(kuva_peli_sysmem,"gfx\\pk2stuff.bmp", true) == PD_VIRHE){
				PK2_virhe = true;
				PisteLog_Kirjoita("  - Loading pk2stuff.bmp failed!\n");
				return -1;
			}

			// FONTIT

			//if ((fontti1 = PisteDraw_Font_Uusi(kuva_peli,1,456,8,8,52)) == PD_VIRHE)
			//	PK2_virhe = true;

			PisteLog_Kirjoita("  - Loading fonts \n");

			int ind_font = 0,
				ind_path = 0;
			
			ind_path = tekstit->Hae_Indeksi("font path");

			ind_font = tekstit->Hae_Indeksi("font small font");
			if (ind_path == -1 || ind_font == -1) {
				if ((fontti1 = PisteDraw_Font_Uusi("language\\fonts\\","ScandicSmall.txt")) == PD_VIRHE){
					PK2_virhe = true;
				}
			}
			else {
				if ((fontti1 = PisteDraw_Font_Uusi(tekstit->Hae_Teksti(ind_path),tekstit->Hae_Teksti(ind_font))) == PD_VIRHE){
				PK2_virhe = true;
				PisteLog_Kirjoita("    - Loading font ");
				PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_path));
				PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_font));
				PisteLog_Kirjoita(" failed!\n");
				}
			}

			ind_font = tekstit->Hae_Indeksi("font big font normal");
			if (ind_path == -1 || ind_font == -1) {
				if ((fontti2 = PisteDraw_Font_Uusi("language\\fonts\\","ScandicBig1.txt")) == PD_VIRHE){
				PK2_virhe = true;
				}
			}
			else {
				if ((fontti2 = PisteDraw_Font_Uusi(tekstit->Hae_Teksti(ind_path),tekstit->Hae_Teksti(ind_font))) == PD_VIRHE){
				PK2_virhe = true;
				PisteLog_Kirjoita("    - Loading font ");
				PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_path));
				PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_font));
				PisteLog_Kirjoita(" failed!\n");
				}
			}

			ind_font = tekstit->Hae_Indeksi("font big font hilite");
			if (ind_path == -1 || ind_font == -1) {
				if ((fontti3 = PisteDraw_Font_Uusi("language\\fonts\\","ScandicBig2.txt")) == PD_VIRHE){
				PK2_virhe = true;
				}
			}
			else {
				if ((fontti3 = PisteDraw_Font_Uusi(tekstit->Hae_Teksti(ind_path),tekstit->Hae_Teksti(ind_font))) == PD_VIRHE){
				PK2_virhe = true;
				PisteLog_Kirjoita("    - Loading font ");
				PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_path));
				PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_font));
				PisteLog_Kirjoita(" failed!\n");
				}
			}

			ind_font = tekstit->Hae_Indeksi("font big font shadow");
			if (ind_path == -1 || ind_font == -1) {
				if ((fontti4 = PisteDraw_Font_Uusi("language\\fonts\\","ScandicBig3.txt")) == PD_VIRHE){
				PK2_virhe = true;
				}
			}
			else {
				if ((fontti4 = PisteDraw_Font_Uusi(tekstit->Hae_Teksti(ind_path),tekstit->Hae_Teksti(ind_font))) == PD_VIRHE){
				PK2_virhe = true;
				PisteLog_Kirjoita("    - Loading font ");
				PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_path));
				PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_font));
				PisteLog_Kirjoita(" failed!\n");
				}
			}
/*			

			if ((fontti2 = PisteDraw_Font_Uusi("language\\fonts\\","ScandicBig1.txt")) == PD_VIRHE){
				PK2_virhe = true;
			}
			
			if ((fontti3 = PisteDraw_Font_Uusi("language\\fonts\\","ScandicBig2.txt")) == PD_VIRHE){
				PK2_virhe = true;
			}
			
			if ((fontti4 = PisteDraw_Font_Uusi("language\\fonts\\","ScandicBig3.txt")) == PD_VIRHE){
				PK2_virhe = true;
			}

/*
			if ((fontti1 = PisteDraw_Font_Uusi(kuva_peli,1,413,8,10,52)) == PD_VIRHE)
				PK2_virhe = true;

			//if ((fontti2 = PisteDraw_Font_Uusi(kuva_peli,1,425,15,18,42)) == PD_VIRHE)
			//	PK2_virhe = true;

			if ((fontti2 = PisteDraw_Font_Uusi(kuva_peli,0,268,15,21,42)) == PD_VIRHE)
				PK2_virhe = true;

			if ((fontti3 = PisteDraw_Font_Uusi(kuva_peli,0,290,15,21,42)) == PD_VIRHE)
				PK2_virhe = true;

			if ((fontti4 = PisteDraw_Font_Uusi(kuva_peli,0,312,15,21,42)) == PD_VIRHE)
				PK2_virhe = true;
*/
			PK_Sprite_Tyhjenna();

			PK_Episodit_Hae();
			PK_Jaksot_Alusta();
			PK_Jaksot_Hae();

			PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,0);

			PisteLog_Kirjoita("  - Loading basic sound fx \n");

			if ((kytkin_aani = PisteSound_SFX_Uusi("sfx\\switch3.wav"))==PS_VIRHE)
				PK2_virhe = true;

			if ((hyppy_aani  = PisteSound_SFX_Uusi("sfx\\jump4.wav"))==PS_VIRHE)
				PK2_virhe = true;

			if ((loiskahdus_aani  = PisteSound_SFX_Uusi("sfx\\splash.wav"))==PS_VIRHE)
				PK2_virhe = true;

			if ((avaa_lukko_aani  = PisteSound_SFX_Uusi("sfx\\openlock.wav"))==PS_VIRHE)
				PK2_virhe = true;

			if ((menu_aani  = PisteSound_SFX_Uusi("sfx\\menu2.wav"))==PS_VIRHE)
				PK2_virhe = true;

			if ((ammuu_aani  = PisteSound_SFX_Uusi("sfx\\moo.wav"))==PS_VIRHE)
				PK2_virhe = true;

			if ((kieku_aani  = PisteSound_SFX_Uusi("sfx\\doodle.wav"))==PS_VIRHE)
				PK2_virhe = true;

			if ((tomahdys_aani  = PisteSound_SFX_Uusi("sfx\\pump.wav"))==PS_VIRHE)
				PK2_virhe = true;

			if ((pistelaskuri_aani = PisteSound_SFX_Uusi("sfx\\counter.wav"))==PS_VIRHE)
				PK2_virhe = true;
			
			if (PK2_virhe)
				PisteLog_Kirjoita("  - Loading one or more basic sound fx failed!\n");

			PisteWait_Start();

			PisteDraw_Fade_Paletti_In(PD_FADE_HIDAS);

			PisteLog_Kirjoita("  - Calculating tiles. \n");
			PK_Palikka_Laske_Palikat();

			PK_Esineet_Alusta();

			PisteLog_Kirjoita("  - Loading background picture \n");
			kuva_tausta = PisteDraw_Buffer_Uusi(640,480,true,255);
			PisteDraw_Lataa_Kuva(kuva_tausta,"gfx\\menu.bmp",true);

			PK_Tallennukset_Tyhjenna();

			PisteLog_Kirjoita("  - Loading saves \n");
			PK_Tallennukset_Hae_Kaikki("data\\saves.dat");

			PisteLog_Kirjoita("  - PisteSound sounds on \n");
			PisteSound_Aanet_Paalla(asetukset.aanet);
			
			PisteLog_Kirjoita("- Initializing basic stuff completed \n");
		}

		// KARTAN ALUSTUS

		if (pelin_seuraava_tila == TILA_KARTTA)
		{
			PisteLog_Kirjoita("- Initializing map screen \n");
			PK_Aseta_Marginaalit(true, true, 640, 480);
			
			PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,0);
			PisteDraw_Paivita_Naytto();
			//PisteDraw_Font_Kirjoita(fontti2,"loading...",PD_TAUSTABUFFER,RUUDUN_LEVEYS/2-82,RUUDUN_KORKEUS/2-9);

			if (!peli_kesken)
			{
				if (lataa_peli != -1)
				{
					PisteLog_Kirjoita("  - Loading a saved game \n");
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
					PisteLog_Kirjoita("  - Starting a new game \n");
					PK_Jaksot_Alusta();	// jos ladataan peli, asetetaan läpäistyarvot jaksoille aikaisemmin
					PK_Jaksot_Hae();
				}

				PisteLog_Kirjoita("  - Loading top scores \n");
				char topscoretiedosto[_MAX_PATH] = "scores.dat";
				PK_Episodipisteet_Lataa(topscoretiedosto);
			}
			
			/* Ladataan kartan taustakuva ...*/
			char mapkuva[_MAX_PATH] = "map.bmp";
			PK_Lisaa_Episodin_Hakemisto(mapkuva);
			PisteLog_Kirjoita("  - Loading map picture ");
			PisteLog_Kirjoita(mapkuva);
			PisteLog_Kirjoita(" from episode folder \n");

			if (PisteDraw_Lataa_Kuva(kuva_tausta,mapkuva,true)==PD_VIRHE) { // Jos episodilla ei omaa...
				PisteLog_Kirjoita("  - Loading map picture map.bmp from gfx folder \n");
				if (PisteDraw_Lataa_Kuva(kuva_tausta,"gfx\\map.bmp",true)==PD_VIRHE) { // ladataan oletuskuva.
					PisteLog_Kirjoita("  - Loading map picture failed!\n");
				}
			}

			/* Ladataan kartan musiikki ...*/
			char mapmusa[_MAX_PATH] = "map.xm";
			PK_Lisaa_Episodin_Hakemisto(mapmusa);

			if (!PK_Onko_File(mapmusa)) {
				strcpy(mapmusa,"map.mod");
				PK_Lisaa_Episodin_Hakemisto(mapmusa);

				if (!PK_Onko_File(mapmusa)) {
					strcpy(mapmusa,"map.it");
					PK_Lisaa_Episodin_Hakemisto(mapmusa);

					if (!PK_Onko_File(mapmusa)) {
						strcpy(mapmusa,"map.s3m");
						PK_Lisaa_Episodin_Hakemisto(mapmusa);
					}
				}
			}

			PisteLog_Kirjoita("  - Loading map music ");
			PisteLog_Kirjoita(mapmusa);
			PisteLog_Kirjoita(" from episode folder \n");


			if (PK_Midas_Lataa_Musiikki(mapmusa)!=0) {
				PisteLog_Kirjoita("  - Loading map music map.xm from music folder \n");
				if (PK_Midas_Lataa_Musiikki("music\\map.xm")!=0) {
					PisteLog_Kirjoita("  - Loading map music failed!\n");
					PK2_virhe = true;
				}
			}
			
			musiikin_voimakkuus = musiikin_max_voimakkuus;

			siirry_kartasta_peliin = false;

			PisteDraw_Fade_Paletti_In(PD_FADE_HIDAS);

			PisteWait_Start();
		}

		// MENUJEN ALUSTUS

		if (pelin_seuraava_tila == TILA_MENUT)
		{
			PisteLog_Kirjoita("- Initializing menu screen \n");
			
			PK_Aseta_Marginaalit(true, true, 640, 480);
			
			//PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,0,0);
			PisteLog_Kirjoita("  - Copying game situation as background image \n");
			PK_Kopioi_Pelitilanne_Taustakuvaksi();
			PK_Sumenna_Kuva(kuva_tausta, 640, 480);
			//PK_Kopioi_Pelitilanne_Taustakuvaksi();
			
			PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,0);
			PisteDraw_Paivita_Naytto();

			PisteLog_Kirjoita("  - Loading episodes \n");
			PK_Episodit_Hae();

			if (!peli_kesken)
			{
				PisteLog_Kirjoita("  - Loading menu background image: gfx\\menu.bmp\n");
				PisteDraw_Lataa_Kuva(kuva_tausta,"gfx\\menu.bmp",true);
				//PK_Sumenna_Kuva(kuva_tausta, 640, 480);

				PisteLog_Kirjoita("  - Loading menu background music: music\\song09.xm \n");
				PK_Midas_Lataa_Musiikki("music\\song09.xm");//theme.xm
				musiikin_voimakkuus = musiikin_max_voimakkuus;
			}			

			//PisteDraw_Lataa_Kuva(kuva_tausta,"gfx\\menu.bmp",false);
			menunelio.left = 320-5;
			menunelio.top = 240-5;
			menunelio.right = 320+5;
			menunelio.bottom = 240+5;

			PisteDraw_Fade_Paletti_In(PD_FADE_NORMAALI);

			PisteWait_Start();

			menu_valittu_id = 1;

			PisteLog_Kirjoita("- Initializing menu screen completed\n");
		}

		// UUDEN JAKSON ALUSTUS

		if (pelin_seuraava_tila == TILA_PELI)
		{
			PisteLog_Kirjoita("- Initializing a new level \n");
			PK_Aseta_Marginaalit(false, false, 0, 0);
			
			PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,0);
			PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_game_loading),PD_TAUSTABUFFER,RUUDUN_LEVEYS/2-82,RUUDUN_KORKEUS/2-9);
			PisteDraw_Paivita_Naytto();
			PisteDraw_Reset_Paletti();

			if (jaksot[jakso_indeksi_nyt].lapaisty)
				uusinta = true;
			else
				uusinta = false;
			
			if (!peli_kesken)
			{
				jakso_lapaisty = false;
				
				PK_Sprite_Tyhjenna();
				PisteLog_Kirjoita("  - Loading map \n");
				if (PK_Kartta_Lataa(seuraava_kartta) == 1) {
					PK2_virhe = true;
					PisteLog_Kirjoita("  - Loading map failed!\n");
				}

				PisteLog_Kirjoita("  - Doing tile calcualtions...\n");
				PK_Palikka_Laske_Palikat();

				PisteLog_Kirjoita("  - Initializing items \n");
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

			PisteWait_Start();
			fps_aika = PisteWait_Get();

			PK_Aseta_Marginaalit(true, true, 800, 480);

			PisteLog_Kirjoita("- Initializing a new level complete\n");
		}

		// PISTELASKUN ALUSTUS

		if (pelin_seuraava_tila == TILA_PISTELASKU)
		{
			
			PK_Aseta_Marginaalit(true, true, 640, 480);
			
			PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,0);
			PisteDraw_Paivita_Naytto();

			PisteDraw_Lataa_Kuva(kuva_tausta,"gfx\\menu.bmp",true);
			PK_Sumenna_Kuva(kuva_tausta, 640, 480);

			if (PK_Midas_Lataa_Musiikki("music\\hiscore.xm")!=0)
				PK2_virhe = true;

			jakso_uusi_ennatys = false;
			jakso_uusi_ennatysaika = false;
			episodi_uusi_ennatys = false;

			// Lasketaan pelaajan kokonaispisteet etukäteen
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
			
			/* Tutkitaan onko pelaajarikkonut kentän piste- tai nopeusennätyksen */
			vertailun_tulos = PK_Episodipisteet_Vertaa(jakso_indeksi_nyt,temp_pisteet,kartta->aika-aika,false);
			if (vertailun_tulos > 0) {
				PK_Episodipisteet_Tallenna(pisteet_tiedosto);
			}
			
			/* Tutkitaan onko pelaaja rikkonut episodin piste-ennätyksen */
			vertailun_tulos = PK_Episodipisteet_Vertaa(0,pisteet,0,true);
			if (vertailun_tulos > 0)
				PK_Episodipisteet_Tallenna(pisteet_tiedosto);

			musiikin_voimakkuus = musiikin_max_voimakkuus;

			siirry_pistelaskusta_karttaan = false;

			//PisteDraw_Fade_Paletti_In(PD_FADE_NOPEA);

			PisteWait_Start();
		}

		// INTRON ALUSTUS

		if (pelin_seuraava_tila == TILA_INTRO)
		{
			PisteLog_Kirjoita("- Initializing intro screen\n");

			PK_Aseta_Marginaalit(true, true, 640, 480);
			
			PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,0);
			PisteDraw_Paivita_Naytto();

			PisteLog_Kirjoita("  - Loading picture: gfx\\intro.bmp\n");

			PisteDraw_Lataa_Kuva(kuva_tausta,"gfx\\intro.bmp",true);

			PisteLog_Kirjoita("  - Loading music: music\\intro.xm\n");

			if (PK_Midas_Lataa_Musiikki("music\\intro.xm")!=0)
				PK2_virhe = true;

			musiikin_voimakkuus = musiikin_max_voimakkuus;

			introlaskuri = 0;
			siirry_pistelaskusta_karttaan = false;

			//PisteDraw_Fade_Paletti_In(PD_FADE_NOPEA);

			PisteWait_Start();

			PisteLog_Kirjoita("- Initializing intro screen complete\n");
		}

		if (pelin_seuraava_tila == TILA_LOPPU)
		{
			
			PK_Aseta_Marginaalit(true, true, 640, 480);
			
			PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,0);
			PisteDraw_Paivita_Naytto();

			PisteDraw_Lataa_Kuva(kuva_tausta,"gfx\\ending.bmp",true);

			if (PK_Midas_Lataa_Musiikki("music\\intro.xm")!=0)
				PK2_virhe = true;

			musiikin_voimakkuus = musiikin_max_voimakkuus;

			loppulaskuri = 0;
			siirry_lopusta_menuun = false;
			peli_kesken = false;

			//PisteDraw_Fade_Paletti_In(PD_FADE_NOPEA);

			PisteWait_Start();
		}


		PisteDraw_Fade_Paletti_In(PD_FADE_NORMAALI);

		pelin_tila = pelin_seuraava_tila;

		
	}
	return 0;
}
/*
int PK_Init(void)
{

	_getcwd(tyohakemisto, _MAX_PATH);
	
	strcpy(viesti,"no message");

	jakso = 1;
	strcpy(episodi,"demo");
	strcpy(episodin_hakemisto,"episodes\\demo");

	srand( (unsigned)time( NULL ) );
	
	PK_Laske_Taulut();

	PK2Kartta_Cos_Sin(cos_table, sin_table);
	
	PK_Alusta_Fadetekstit();

	if (RAJAA_KARTANPIIRTOALUE)
	{
		if (RUUDUN_LEVEYS > 640)
			KARTANPIIRTO_LEVEYS = 640;
		else
			KARTANPIIRTO_LEVEYS = RUUDUN_LEVEYS;

		if (RUUDUN_KORKEUS > 480)
			KARTANPIIRTO_KORKEUS = 480;
		else
			KARTANPIIRTO_KORKEUS = RUUDUN_KORKEUS;
	}
	else
	{
		KARTANPIIRTO_LEVEYS = RUUDUN_LEVEYS;
		KARTANPIIRTO_KORKEUS = RUUDUN_KORKEUS;
	}
	

	//PK2Kartta_Aseta_Ruudun_Mitat(RUUDUN_LEVEYS,RUUDUN_KORKEUS);
	PK2Kartta_Aseta_Ruudun_Mitat(KARTANPIIRTO_LEVEYS,KARTANPIIRTO_KORKEUS);

	if ((PisteSound_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &) hinstance_app, 
								  AANET_STEREO, AANET_SAMPLERATE, AANET_BITRATE)) == PD_VIRHE)
		PK2_virhe = true;	

	PK_Midas_Alusta();

	if ((PisteInput_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &)hinstance_app)) == PI_VIRHE)
		PK2_virhe = true;

	if ((PisteDraw_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &) hinstance_app, 
								  RUUDUN_LEVEYS, RUUDUN_KORKEUS, RUUDUN_BPP, MAX_COLORS_PALETTE)) == PD_VIRHE)
		PK2_virhe = true;

	kartta = new PK2Kartta();

	if ((kuva_peli = PisteDraw_Buffer_Uusi(640,480, true, 255)) == PD_VIRHE)
		PK2_virhe = true;

	if (PisteDraw_Lataa_Kuva(kuva_peli,"gfx\\pk2stuff.bmp", true) == PD_VIRHE)
		PK2_virhe = true;

	if ((fontti1 = PisteDraw_Font_Uusi(kuva_peli,1,456,8,8,52)) == PD_VIRHE)
		PK2_virhe = true;

	//if ((fontti2 = PisteDraw_Font_Uusi(kuva_peli,1,446,14,9,45)) == PD_VIRHE)
	//	PK2_virhe = true;

	if ((fontti2 = PisteDraw_Font_Uusi(kuva_peli,1,425,15,18,42)) == PD_VIRHE)
		PK2_virhe = true;

	PK_Sprite_Tyhjenna();

	PK_Jaksot_Alusta();
	PK_Jaksot_Hae();

	if (PK_Kartta_Lataa("untitle1.map") == 1)
		PK2_virhe = true;

	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,0);

	

	if ((kytkin_aani = PisteSound_SFX_Uusi("sfx\\switch.wav"))==PS_VIRHE)
		PK2_virhe = true;

	if ((hyppy_aani  = PisteSound_SFX_Uusi("sfx\\jump2.wav"))==PS_VIRHE)
		PK2_virhe = true;

	if ((loiskahdus_aani  = PisteSound_SFX_Uusi("sfx\\splash.wav"))==PS_VIRHE)
		PK2_virhe = true;

	if ((avaa_lukko_aani  = PisteSound_SFX_Uusi("sfx\\openlock.wav"))==PS_VIRHE)
		PK2_virhe = true;

	//PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,30,639,450);

//	if ((tausta_extra = PisteDraw_Buffer_Uusi(640,480, true, 255)) == PD_VIRHE)
//		PK2_virhe = true;

//	if ((PisteDraw_Lataa_Kuva(tausta_extra,"pilvet2.bmp",false)) == PD_VIRHE)
//		PK2_virhe = true;

	PisteWait_Start();

	PisteDraw_Fade_Paletti_In(PD_FADE_HIDAS);

	PK_Palikka_Laske_Palikat();

	PK_Esineet_Alusta();

	return 0;
}
*/
int PK_Spritet(void)
{
	info_spriteja_aktiivisena = 0;
	
	//bool aktiivinen;
	int i;

	for (i=0;i<MAX_SPRITEJA;i++)
	{
		// Onko sprite lähellä tapahtumien keskipistettä? Jos on, niin aktivoidaan.
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

	for (i=0;i<MAX_SPRITEJA;i++)
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

int PK_Spritet_Piirra_Taustat()
{
	double xl, yl, alku_x, alku_y, yk;
	int i;
	
	
	for (int in=0;in<MAX_SPRITEJA;in++)
	{	
		i = taustaspritet[in]; 
		// Onko sprite näkyvä
		if (spritet[i].tyyppi != NULL && i != -1)
		{
			if (!spritet[i].piilota && spritet[i].tyyppi->tyyppi == TYYPPI_TAUSTA)
			{
				//Tarkistetaanko onko sprite tai osa siitä kuvassa

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
				
				//Tarkistetaanko onko sprite tai osa siitä kuvassa
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
					if (!pause)
						spritet[i].Animoi();
					spritet[i].piilossa = true;
				}
			
				info_spriteja++;

			}
		}
	}
	return 0;
}

int PK_Spritet_Piirra()
{
	info_spriteja = 0;
	info_spriteja_piirretty = 0;
	int stars, sx;
	double star_x, star_y;
	
	for (int i=0;i<MAX_SPRITEJA;i++)
	{	
		// Onko sprite näkyvä
		if (!spritet[i].piilota && spritet[i].tyyppi->tyyppi != TYYPPI_TAUSTA)
		{
			//Tarkistetaanko onko sprite tai osa siitä kuvassa
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
				if (!pause)
					spritet[i].Animoi();

				if (spritet[i].energia < 1)
					spritet[i].piilota = true;
			}
		
			info_spriteja++;
		}
	}	
	return 0;
}

int PK_Kamera(void)
{


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

int PK_Piirra_Info(void)
{
	int fy, vali;
	char lukua[20]; 

	fy = 35;

	vali = PisteDraw_Font_Kirjoita(fontti1,"spriteja: ",PD_TAUSTABUFFER,10,fy);
	itoa(info_spriteja,lukua,10);
	PisteDraw_Font_Kirjoita(fontti1,lukua,PD_TAUSTABUFFER,10+vali,fy);
	fy += 10;

	vali = PisteDraw_Font_Kirjoita(fontti1,"aktiivisia: ",PD_TAUSTABUFFER,10,fy);
	itoa(info_spriteja_aktiivisena,lukua,10);
	PisteDraw_Font_Kirjoita(fontti1,lukua,PD_TAUSTABUFFER,10+vali,fy);
	fy += 10;

	vali = PisteDraw_Font_Kirjoita(fontti1,"piirretty: ",PD_TAUSTABUFFER,10,fy);
	itoa(info_spriteja_piirretty,lukua,10);
	PisteDraw_Font_Kirjoita(fontti1,lukua,PD_TAUSTABUFFER,10+vali,fy);
	fy += 10;
/*
	char tiedosto[255];

	strcpy(tiedosto,"sprites\\esim.spr");

	PK_Lisaa_Episodin_Hakemisto(tiedosto);

	PisteDraw_Font_Kirjoita(fontti1,tiedosto,PD_TAUSTABUFFER,30,450);
*/
	for (int i=0;i<40;i++)
	{
		itoa(protot[i].indeksi,lukua,10);
		PisteDraw_Font_Kirjoita(fontti1,lukua,PD_TAUSTABUFFER,410,10+i*10);
		PisteDraw_Font_Kirjoita(fontti1,protot[i].tiedosto,PD_TAUSTABUFFER,430,10+i*10);
		PisteDraw_Font_Kirjoita(fontti1,protot[i].bonus_sprite,PD_TAUSTABUFFER,545,10+i*10);
	}

	PisteDraw_Font_Kirjoita(fontti1,viesti,PD_TAUSTABUFFER,10,80);

	for (i=0;i<EPISODI_MAX_JAKSOJA;i++)
	{
		if (strcmp(jaksot[i].nimi,"")!=0)
			PisteDraw_Font_Kirjoita(fontti1,jaksot[i].nimi,PD_TAUSTABUFFER,30,240+i*10);
	}

	PisteDraw_Font_Kirjoita(fontti1,seuraava_kartta,PD_TAUSTABUFFER,10,450);


	char dluku[50];

	_gcvt(spritet[pelaaja_index].x,7,dluku);
	PisteDraw_Font_Kirjoita(fontti1,dluku,PD_TAUSTABUFFER,10,420);
	_gcvt(spritet[pelaaja_index].y,7,dluku);
	PisteDraw_Font_Kirjoita(fontti1,dluku,PD_TAUSTABUFFER,10,430);
	_gcvt(spritet[pelaaja_index].b,7,dluku);
	PisteDraw_Font_Kirjoita(fontti1,dluku,PD_TAUSTABUFFER,10,440);
	//_gcvt(spritet[pelaaja_index].b,7,dluku);
	//PisteDraw_Font_Kirjoita(fontti1,dluku,PD_TAUSTABUFFER,10,450);
	//itoa(spritet[pelaaja_index].tyyppi->korkeus,lukua,10);
	//PisteDraw_Font_Kirjoita(fontti1,lukua,PD_TAUSTABUFFER,10,440);
	/*
	x = (int)((spritet[pelaaja_index].x+spritet[pelaaja_index].a)/32);
	itoa(x,lukua,10);
	PisteDraw_Font_Kirjoita(fontti1,lukua,PD_TAUSTABUFFER,10,10);

	y = (int)(spritet[pelaaja_index].y/32);
	itoa(y,lukua,10);
	PisteDraw_Font_Kirjoita(fontti1,lukua,PD_TAUSTABUFFER,10,25);

	p = kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS];
	itoa(p,lukua,10);
	PisteDraw_Font_Kirjoita(fontti1,lukua,PD_TAUSTABUFFER,10,40);
*/
	itoa(spritet[pelaaja_index].hyppy_ajastin,lukua,10);
	PisteDraw_Font_Kirjoita(fontti1,lukua,PD_TAUSTABUFFER,270,450);
/*
	vali = 0;
	for (int p=177;p<185;p++)	{
		itoa(p,lukua,10);
		PisteDraw_Font_Kirjoita(fontti1,lukua,PD_TAUSTABUFFER,10+vali,450) ;		
		_gcvt(sin_table[p],2,dluku);
		vali += PisteDraw_Font_Kirjoita(fontti1,dluku,PD_TAUSTABUFFER,10+vali,460) + 4;
	}
*/	
	char tpolku[_MAX_PATH] = "";
	PK_Lisaa_Episodin_Hakemisto(tpolku);

	PisteDraw_Font_Kirjoita(fontti1,tpolku,PD_TAUSTABUFFER,10,470);

	itoa(nakymattomyys,lukua,10);
	PisteDraw_Font_Kirjoita(fontti1,lukua,PD_TAUSTABUFFER,610,470);

	return 0;
}

int PK_Piirra_Tausta(void)
{
	int pallarx = (kamera_x%1920)/3;
	int pallary = (kamera_y%1440)/3;

	int pallarx2 = (kamera_x%1280)/2;
	int pallary2 = (kamera_y%960)/2;

	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,34);//0

	PK_Aseta_Rajaus();
	//if (RAJAA_KARTANPIIRTOALUE)
	//	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,MARGINAALI_HORI,MARGINAALI_VERT,
	//							 KARTANPIIRTO_LEVEYS+MARGINAALI_HORI,KARTANPIIRTO_KORKEUS+MARGINAALI_VERT);
	//else
	//	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);	
/*
	if (RAJAA_KARTANPIIRTOALUE)
		PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,16,16,KARTANPIIRTO_LEVEYS-16,KARTANPIIRTO_KORKEUS-16);
	else
		PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,16,16,RUUDUN_LEVEYS-16,RUUDUN_KORKEUS-16);
*/
	if (kartta->tausta == TAUSTA_STAATTINEN)
	{
		PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,0,0);
		PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,640,0);
	}

	if (kartta->tausta == TAUSTA_PALLARX_HORI)
	{
		PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,0 - pallarx,0);
		PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,640 - pallarx,0);
		
		if (KARTANPIIRTO_LEVEYS > 640)
			PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,1280 - pallarx,0);
	}

	if (kartta->tausta == TAUSTA_PALLARX_VERT)
	{
		PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,0,0 - pallary);
		PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,0,480 - pallary);
		
		if (KARTANPIIRTO_LEVEYS > 640)
		{
			PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,640,0 - pallary);
			PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,640,480 - pallary);
		}
	}

	if (kartta->tausta == TAUSTA_PALLARX_VERT_JA_HORI)
	{
		PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,0 - pallarx,	0-pallary);
		PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,640 - pallarx,0-pallary);
		PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,0 - pallarx,	480-pallary);
		PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,640 - pallarx,480-pallary);
		
		if (KARTANPIIRTO_LEVEYS > 640)
		{
			PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,1280 - pallarx,0-pallary);
			PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,1280 - pallarx,480-pallary);
		}
	}
	
	/*pilvet 1*/
	/*
	PisteDraw_Buffer_Flip_Nopea(tausta_extra,PD_TAUSTABUFFER,0-pallarx2,0-pallary2);
	PisteDraw_Buffer_Flip_Nopea(tausta_extra,PD_TAUSTABUFFER,640-pallarx2,0-pallary2);
	PisteDraw_Buffer_Flip_Nopea(tausta_extra,PD_TAUSTABUFFER,0-pallarx2,480-pallary2);
	PisteDraw_Buffer_Flip_Nopea(tausta_extra,PD_TAUSTABUFFER,640-pallarx2,480-pallary2);
	*/
	/*pilvet 2
	pallarx2 = (int)((kamera_x%1600)/2.5);

	PisteDraw_Buffer_Flip_Nopea(tausta_extra,PD_TAUSTABUFFER,0-pallarx2,95,0,332,640,382);
	PisteDraw_Buffer_Flip_Nopea(tausta_extra,PD_TAUSTABUFFER,640-pallarx2,95,0,332,640,382);
	
	pallarx2 = (int)((kamera_x%1280)/2);

	PisteDraw_Buffer_Flip_Nopea(tausta_extra,PD_TAUSTABUFFER,0-pallarx2,65,0,186,640,263);
	PisteDraw_Buffer_Flip_Nopea(tausta_extra,PD_TAUSTABUFFER,640-pallarx2,65,0,186,640,263);
	
	pallarx2 = (int)((kamera_x%960)/1.5);
	
	PisteDraw_Buffer_Flip_Nopea(tausta_extra,PD_TAUSTABUFFER,0-pallarx2,0,0,0,640,109);
	PisteDraw_Buffer_Flip_Nopea(tausta_extra,PD_TAUSTABUFFER,640-pallarx2,0,0,0,640,109);
	*/
	return 0;
}

int PK_Piirra_Alapaneeli()
{
	PK_Aseta_Rajaus();
	//PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);

	if (esineita > 0 && item_paneeli_x < 10)
		item_paneeli_x++;

	if (esineita == 0 && item_paneeli_x > -215)
		item_paneeli_x--;

	if (item_paneeli_x > -215)
		PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,item_paneeli_x,KARTANPIIRTO_KORKEUS-60,
		                        1,216,211,266);
	if (item_paneeli_x > 5)
			PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_game_items),PD_TAUSTABUFFER,15,KARTANPIIRTO_KORKEUS-65);
 

	char luku[15];
	int vali = 0;
	
	int x, y;

	//////////////
	// piirrä aika
	//////////////
	if (aika > 0)
	{
		int min = aika/60, 
			sek = aika%60;


		x = RUUDUN_LEVEYS / 2 - 546 / 2 + 342;
		y = KARTANPIIRTO_KORKEUS-39;
		PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_game_time),PD_TAUSTABUFFER,x,y-20);


		itoa(min,luku,10);
		PisteDraw_Font_Kirjoita(        fontti4,luku,PD_TAUSTABUFFER,x+1,y+1);
		vali += PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,x,y);
		vali += PisteDraw_Font_Kirjoita(fontti1,":",PD_TAUSTABUFFER,x+vali,y);
		if (sek < 10)
		{
			PisteDraw_Font_Kirjoita(        fontti4,"0",PD_TAUSTABUFFER,x+vali+1,y+1);
			vali += PisteDraw_Font_Kirjoita(fontti2,"0",PD_TAUSTABUFFER,x+vali,y);
		}
		itoa(sek,luku,10);
		
		PisteDraw_Font_Kirjoita(        fontti4,luku,PD_TAUSTABUFFER,x+vali+1,y+1);
		vali += PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,x+vali,y);
	}
	
	/////////////////
	// piirrä avaimet
	/////////////////
	if (avaimia > 0)
	{
		x = RUUDUN_LEVEYS / 2 - 546 / 2 + 483;
		y = KARTANPIIRTO_KORKEUS-39;
		PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_game_keys),PD_TAUSTABUFFER,x,y-20);

		itoa(avaimia,luku,10);
		PisteDraw_Font_Kirjoita(fontti4,luku,PD_TAUSTABUFFER,x+1,y+1);
		PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,x,y);
	}
	
	/////////////////
	// piirrä esineet
	/////////////////
	PK_Esineet_Piirra();

	return 0;
}

int PK_Piirra_Peli_Ylaosa(void)
{
	char luku[15];
	int vali = 20;

	vali = PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_game_energy),PD_TAUSTABUFFER,40,10);
	ltoa(spritet[pelaaja_index].energia,luku,10);
	PisteDraw_Font_Kirjoita(fontti4,luku,PD_TAUSTABUFFER,40+vali+1,10+1);
	PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,40+vali,10);
	
	vali = PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_game_score),PD_TAUSTABUFFER,230,10);
	ltoa(jakso_pisteet,luku,10);
	PisteDraw_Font_Kirjoita(fontti4,luku,PD_TAUSTABUFFER,230+vali+1,10+1);
	PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,230+vali,10);	
/*
	vali = PisteDraw_Font_Kirjoita(fontti1,"level: ",PD_TAUSTABUFFER,450,10);
	ltoa(kartta->jakso,luku,10);
	PisteDraw_Font_Kirjoita(fontti4,luku,PD_TAUSTABUFFER,450+vali+1,10+1);
	PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,450+vali,10);
*/
		
	if (spritet[pelaaja_index].ammus2 != -1)
	{
		//itoa(ammuksia2,luku,10);
		PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_game_attack1), PD_TAUSTABUFFER, KARTANPIIRTO_LEVEYS-170,10);
		protot[spritet[pelaaja_index].ammus2].Piirra(KARTANPIIRTO_LEVEYS-170/*+MARGINAALI_HORI*/,/*MARGINAALI_VERT+*/20,0);
		//PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,RUUDUN_LEVEYS-35,50);
	}		

	if (spritet[pelaaja_index].ammus1 != -1)
	{
		//itoa(ammuksia1,luku,10);
		PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_game_attack2), PD_TAUSTABUFFER, KARTANPIIRTO_LEVEYS-90,10+15);
		protot[spritet[pelaaja_index].ammus1].Piirra(KARTANPIIRTO_LEVEYS-90/*+MARGINAALI_HORI*/,/*MARGINAALI_VERT+*/20+15,0);
		//PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,RUUDUN_LEVEYS-35,10);
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

		PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,alue.left-1,alue.top-1,alue.right+1,alue.bottom+1,51);
		PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,alue.left,alue.top,alue.right,alue.bottom,38);		

		if (ilmoitus_ajastin >= 100)
			PisteDraw_Font_Kirjoita(fontti1,ilmoitus,PD_TAUSTABUFFER,alue.left+4,alue.top+4);
		else
			PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,ilmoitus,PD_TAUSTABUFFER,alue.left+4,alue.top+4,ilmoitus_ajastin);
	}

	return 0;
}

int PK_Piirra_Peli(void)
{
	char luku[15];
	int vali = 20;
	
	if (!skip_frame)
		PK_Piirra_Tausta();

	PK2Kartta_Animoi(degree,palikka_animaatio/7,kytkin1,kytkin2,kytkin3,false);

	if (!skip_frame)
		if (asetukset.tausta_spritet)
			PK_Spritet_Piirra_Taustat();

	if (asetukset.saa_efektit)
		PK_Taustapartikkelit();

	PK_Aseta_Rajaus();

	if (!skip_frame)
		kartta->Piirra_Taustat(kamera_x,kamera_y,false);

	if (!pause)
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

	PK_Aseta_Rajaus();
	/*
	if (RAJAA_KARTANPIIRTOALUE)
		PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,MARGINAALI_HORI,MARGINAALI_VERT,
								 KARTANPIIRTO_LEVEYS+MARGINAALI_HORI,KARTANPIIRTO_KORKEUS+MARGINAALI_VERT);
	else
		PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);

*/
	if (!skip_frame)
		kartta->Piirra_Seinat(kamera_x,kamera_y, false);

	PK_Fadetekstit_Piirra();

	if (asetukset.nayta_tavarat)
		PK_Piirra_Alapaneeli();

	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);
	//PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,639,479);

	PK_Piirra_Peli_Ylaosa();
	
/*
	vali = PisteDraw_Font_Kirjoita(fontti2,"episode ",PD_TAUSTABUFFER,20,RUUDUN_KORKEUS-16);
	PisteDraw_Font_Kirjoita(fontti2,episodi,PD_TAUSTABUFFER,20+vali,RUUDUN_KORKEUS-16);
*/
	/*
	//////////////
	// piirrä aika
	//////////////
	int min = aika/60, 
		sek = aika%60; 
	vali = PisteDraw_Font_Kirjoita(fontti2,"time ",PD_TAUSTABUFFER,272,RUUDUN_KORKEUS-20);
	itoa(min,luku,10);
	vali += PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,272+vali,RUUDUN_KORKEUS-20);
	vali += PisteDraw_Font_Kirjoita(fontti1,":",PD_TAUSTABUFFER,272+vali,RUUDUN_KORKEUS-15);

	if (sek < 10)
		vali += PisteDraw_Font_Kirjoita(fontti2,"0",PD_TAUSTABUFFER,272+vali,RUUDUN_KORKEUS-20);
	
	itoa(sek,luku,10);
	vali += PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,272+vali,RUUDUN_KORKEUS-20);
	
	/////////////////
	// piirrä avaimet
	/////////////////
	vali = PisteDraw_Font_Kirjoita(fontti2,"keys ",PD_TAUSTABUFFER,450,RUUDUN_KORKEUS-20);
	itoa(avaimia,luku,10);
	PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,450+vali,RUUDUN_KORKEUS-20);
	
	/////////////////
	// piirrä esineet
	/////////////////
	PK_Esineet_Piirra();
	*/
	if (piirra_infot)
		PK_Piirra_Info();
	
	///////////////////
	// piirrä framerate
	///////////////////
	if (fps_nayta)
	{
		vali = PisteDraw_Font_Kirjoita(fontti1,"fps: ",PD_TAUSTABUFFER,570,48);
		itoa(fps,luku,10);
		PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,570+vali,48);
	}
	
	///////////////////
	// piirrä pause
	///////////////////
	if (pause)
		PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_game_paused),PD_TAUSTABUFFER,KARTANPIIRTO_LEVEYS/2-82,KARTANPIIRTO_KORKEUS/2-9);
	
	////////////////////////
	// piirrä jakso läpäisty
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

		//PisteDraw_Font_Kirjoita(fontti2,"game over!",PD_TAUSTABUFFER,RUUDUN_LEVEYS/2-75,RUUDUN_KORKEUS/2-9);
	/*
	if (PisteInput_Keydown(DIK_S))
		PK_Sumenna_Kuva(PD_TAUSTABUFFER, 640, 480);
		*/

	if (tuplanopeus)
		skip_frame = !skip_frame;
	else
		skip_frame = false;

	if (!skip_frame)
	{
		PisteWait_Wait(0);//10

		PisteDraw_Paivita_Naytto();

		PisteWait_Start();
	}

	palikka_animaatio = 1 + palikka_animaatio % 34;

	return 0;
}

void PK_Piirra_Kursori(int x,int y)
{
	PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x,y,621,461,640,480);
}

int PK_Piirra_Menu_Nelio(int vasen, int yla, int oikea, int ala, UCHAR pvari)
{
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

	//PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen,yla,oikea,ala,38);
/*
	double kerroin_y = (ala - yla) / 6.0;
	double dbl_y = yla;

	for (int y=0;y<6;y++) {
		
		PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen,int(dbl_y),oikea,int(dbl_y+kerroin_y),224+y);
		dbl_y += kerroin_y;
	}*/

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
			if (tumma)
				vari /= 2;
			
			PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,int(dbl_x),int(dbl_y),int(dbl_x+kerroin_x),int(dbl_y+kerroin_y),pvari+vari);
			dbl_x += kerroin_x;
			tumma = !tumma;
		}
		dbl_y += kerroin_y;
	}

	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen-1,yla-1,oikea+1,yla+2,0);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen-1,yla-1,vasen+2,ala+1,0);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen-1,ala-2,oikea+1,ala+1,0);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,oikea-2,yla-1,oikea+1,ala+1,0);	

	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen-1+1,yla-1+1,oikea+1+1,yla+2+1,0);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen-1+1,yla-1+1,vasen+2+1,ala+1+1,0);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen-1+1,ala-2+1,oikea+1+1,ala+1+1,0);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,oikea-2+1,yla-1+1,oikea+1+1,ala+1+1,0);

	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen,yla,oikea,yla+1,153);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen,yla,vasen+1,ala,144);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen,ala-1,oikea,ala,138);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,oikea-1,yla,oikea,ala,138);

	return 0;
}


bool PK_Piirra_Menut_Valinta(char *teksti, int x, int y)
{
	int pituus = strlen(teksti)*15;

	if ((hiiri_x > x && hiiri_x < x+pituus && hiiri_y > y && hiiri_y < y+15) || 
		(menu_valittu_id == menu_valinta_id))
	{
		menu_valittu_id = menu_valinta_id;

		if ((
			(PisteInput_Hiiri_Vasen() && hiiri_x > x && hiiri_x < x+pituus && hiiri_y > y && hiiri_y < y+15)
			|| PisteInput_Keydown(DIK_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1)) 
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
		//PisteDraw_Font_Kirjoita(fontti2,teksti,PD_TAUSTABUFFER,x,y);

	menu_valinta_id++;

	return false;
}

int PK_Piirra_Menut_Valintalaatikko(int x, int y, bool muuttuja)
{
	if (!muuttuja)
		PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x,y,473,124,473+31,124+31);
	else
		PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x,y,504,124,504+31,124+31);

	if ((hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31))/* ||
		(menu_valittu_id == menu_valinta_id))*/
	{
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(DIK_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1)) 
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

int PK_Piirra_Menut_Saatolaatikko(int x, int y)
{
	int val = 45;

	int randx = rand()%3 - rand()%3;
	int randy = rand()%3 - rand()%3;

	if (menu_valittu_id == menu_valinta_id)
		PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x+randx,y+randy,566,124,566+31,124+31);
	else
		PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x,y,566,124,566+31,124+31);

	if (menu_valittu_id == menu_valinta_id+1)
		PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x+val+randx,y+randy,535,124,535+31,124+31);
	else
		PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x+val,y,535,124,535+31,124+31);
	
	if ((hiiri_x > x && hiiri_x < x+30 && hiiri_y > y && hiiri_y < y+31) || (menu_valittu_id == menu_valinta_id))
	{
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(DIK_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1)) 
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
		if ((PisteInput_Hiiri_Vasen() || PisteInput_Keydown(DIK_SPACE) || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1)) 
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

int PK_Piirra_Menut_PaaValikko()
{
	int my = 0;

	PK_Piirra_Menu_Nelio(160, 200, 640-180, 410, 224);

	if (peli_kesken)
	{
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_continue),180,250+my)) 
		{
			if ((!peli_ohi && !jakso_lapaisty) || lopetusajastin > 1) 
				pelin_seuraava_tila = TILA_PELI;
			else
				pelin_seuraava_tila = TILA_KARTTA;

		}
		my += 20;
	}
	
	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_new_game),180,250+my))
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
		if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_save_game),180,250+my))
		{
			menu_nyt = MENU_TALLENNA;
		}
		my += 20;
	}

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_load_game),180,250+my))
	{
		menu_nyt = MENU_LATAA;
	}
	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_controls),180,250+my))
	{
		menu_nyt = MENU_KONTROLLIT;
	}
	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_graphics),180,250+my))
	{
		menu_nyt = MENU_GRAFIIKKA;
	}
	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_sounds),180,250+my))
	{
		menu_nyt = MENU_AANET;
	}
	my += 20;

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_exit),180,250+my))
	{
		lopeta_peli = true;
		PisteDraw_Fade_Paletti_Out(PD_FADE_NORMAALI);
		musiikin_voimakkuus = 0;
	}
	my += 20;

	return 0;
}

int PK_Piirra_Menut_Nimi()
{
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

		if (PisteInput_Keydown(DIK_LEFT)) {
			menu_nimi_index--;
			key_delay = 8;
		}

		if (PisteInput_Keydown(DIK_RIGHT)) {
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

	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_playermenu_type_name),PD_TAUSTABUFFER,180,224);
	
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,180-2,255-2,180+20*15+4,255+18+4,0);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,180,255,180+20*15,255+18,50);
	if (nimiedit) {
		PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,mx-2,254,mx+16+3,254+20+3,0);
		PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,mx-1,254,mx+16,254+20,96+16);
		PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,mx+12,254,mx+16,254+20,96+8);
	}

	PK_Piirra_LaineTeksti_Hidas(pelaajan_nimi,fontti2,180,255);
	PisteDraw_Font_Kirjoita_Lapinakyva(fontti3,pelaajan_nimi,PD_TAUSTABUFFER,180,255,15);

	merkki = PisteInput_Lue_Nappaimisto();

	if (PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1) && key_delay == 0 && nimiedit) {
		nimiedit = false;
	}

	if (merkki != '\0' && (merkki != menu_nimi_ed_merkki || key_delay == 0) && nimiedit)
	{
		menu_nimi_ed_merkki = merkki;

		if (merkki == '\n') {
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
		if (PisteInput_Keydown(DIK_DELETE) || PisteInput_Lue_Kontrolli() == PI_KB_BACKSPACE)
		{
			for (int c=menu_nimi_index;c<19;c++)
				pelaajan_nimi[c] = pelaajan_nimi[c+1];
				pelaajan_nimi[19] = '\0';
				key_delay = 10;
		}
		
		if (PisteInput_Lue_Kontrolli() == PI_KB_BACKSPACE)
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

int PK_Piirra_Menut_Lataa()
{
	int my = 0, vali = 0;
	char tpaikka[100];
	char jaksoc[8];
	char ind[4];

	PK_Piirra_Menu_Nelio(40, 70, 640-40, 410, 70);

	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_loadgame_title),PD_TAUSTABUFFER,50,90);
	PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_loadgame_info),PD_TAUSTABUFFER,50,110);
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
			vali = PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_loadgame_episode),PD_TAUSTABUFFER,400,150+my);
			vali += PisteDraw_Font_Kirjoita(fontti1,tallennukset[i].episodi,PD_TAUSTABUFFER,400+vali,150+my);
			vali = 0;
			vali += PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_loadgame_level),PD_TAUSTABUFFER,400+vali,160+my);
			itoa(tallennukset[i].jakso,jaksoc,10);
			vali += PisteDraw_Font_Kirjoita(fontti1,jaksoc,PD_TAUSTABUFFER,400+vali,160+my);
		}

		my += 22;
	}

	my += 20;
	
	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_return),180,400)) 
		menu_nyt = MENU_PAAVALIKKO;
	
	return 0;
}

int PK_Piirra_Menut_Tallenna()
{
	int my = 0, vali = 0;
	char tpaikka[100];
	char jaksoc[8];
	char ind[4];

	PK_Piirra_Menu_Nelio(40, 70, 640-40, 410, 224);

	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_savegame_title),PD_TAUSTABUFFER,50,90);
	PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_savegame_info),PD_TAUSTABUFFER,50,110);
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
			vali = PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_savegame_episode),PD_TAUSTABUFFER,400,150+my);
			vali += PisteDraw_Font_Kirjoita(fontti1,tallennukset[i].episodi,PD_TAUSTABUFFER,400+vali,150+my);
			vali = 0;
			vali += PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_savegame_level),PD_TAUSTABUFFER,400+vali,160+my);
			itoa(tallennukset[i].jakso,jaksoc,10);
			vali += PisteDraw_Font_Kirjoita(fontti1,jaksoc,PD_TAUSTABUFFER,400+vali,160+my);
		}

		my += 22;
	}

	my += 20;
	
	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_return),180,400)) 
		menu_nyt = MENU_PAAVALIKKO;
	
	return 0;
}

int PK_Piirra_Menut_Grafiikka()
{
	int my = 0;

	PK_Piirra_Menu_Nelio(40, 70, 640-40, 410, 224);

	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_gfx_title),PD_TAUSTABUFFER,50,90);
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

int PK_Piirra_Menut_Aanet()
{
	int my = 0;

	PK_Piirra_Menu_Nelio(40, 70, 640-40, 410, 224);

	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_sound_title),PD_TAUSTABUFFER,50,90);
	my += 20;

	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,404,224+my,404+aanenvoimakkuus,244+my,0);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,400,220+my,400+aanenvoimakkuus,240+my,81);
	
	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_sound_sfx_volume),PD_TAUSTABUFFER,180,200+my);
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
	
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,404,224+my,404+int(musiikin_max_voimakkuus*1.56),244+my,0);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,400,220+my,400+int(musiikin_max_voimakkuus*1.56),240+my,112);
	
	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_sound_music_volume),PD_TAUSTABUFFER,180,200+my);
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

int PK_Piirra_Menut_Kontrollit()
{
	int my = 0;
	
	PK_Piirra_Menu_Nelio(40, 70, 640-40, 410, 224);

	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_controls_title),PD_TAUSTABUFFER,50,90);

	my = 40;

	if (menu_lue_kontrollit > 0)
	{
		PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,299,74+my+menu_lue_kontrollit*20,584,94+my+menu_lue_kontrollit*20,0);
		PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,295,70+my+menu_lue_kontrollit*20,580,90+my+menu_lue_kontrollit*20,50);
	}

	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_controls_moveleft),PD_TAUSTABUFFER,100,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_controls_moveright),PD_TAUSTABUFFER,100,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_controls_jump),PD_TAUSTABUFFER,100,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_controls_duck),PD_TAUSTABUFFER,100,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_controls_walkslow),PD_TAUSTABUFFER,100,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_controls_eggattack),PD_TAUSTABUFFER,100,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_controls_doodleattack),PD_TAUSTABUFFER,100,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_controls_useitem),PD_TAUSTABUFFER,100,90+my);my+=20;

	my = 40;
	PisteDraw_Font_Kirjoita(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_vasemmalle),PD_TAUSTABUFFER,310,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_oikealle),PD_TAUSTABUFFER,310,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_hyppy),PD_TAUSTABUFFER,310,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_alas),PD_TAUSTABUFFER,310,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_juoksu),PD_TAUSTABUFFER,310,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_hyokkays1),PD_TAUSTABUFFER,310,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_hyokkays2),PD_TAUSTABUFFER,310,90+my);my+=20;
	PisteDraw_Font_Kirjoita(fontti2,PisteInput_Lue_Kontrollin_Nimi(kontrolli_kayta_esine),PD_TAUSTABUFFER,310,90+my);my+=20;	

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
		kontrolli_vasemmalle	= PI_KB_LEFT;
		kontrolli_oikealle		= PI_KB_RIGHT;
		kontrolli_hyppy			= PI_KB_UP;
		kontrolli_alas			= PI_KB_DOWN;
		kontrolli_juoksu		= PI_KB_RALT;
		kontrolli_hyokkays1		= PI_KB_RCONTROL;
		kontrolli_hyokkays2		= PI_KB_RSHIFT;
		kontrolli_kayta_esine	= PI_KB_SPACE;
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

	UCHAR k = 0;

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

int PK_Piirra_Menut_Episodit()
{
	int my = 0;

	PK_Piirra_Menu_Nelio(110, 130, 640-110, 450, 224);

	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_episodes_choose_episode),PD_TAUSTABUFFER,50,90);
	my += 80;

	if (episodi_lkm-2 > 10) {
		char luku[20];
		int vali = 90;
		int x = 50,//500,
			y = 50;//300;

		//vali += PisteDraw_Font_Kirjoita(fontti1,"page:",PD_TAUSTABUFFER,x,y+40);
		itoa(episodisivu+1,luku,10);
		vali += PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,x+vali,y+20);
		vali += PisteDraw_Font_Kirjoita(fontti1,"/",PD_TAUSTABUFFER,x+vali,y+20);
		itoa((episodi_lkm/10)+1,luku,10);
		vali += PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,x+vali,y+20);

		int nappi = PK_Piirra_Menut_Saatolaatikko(x,y);

		if (nappi == 1 && episodisivu > 0)
			episodisivu--;
		
		if (nappi == 2 && (episodisivu*10)+10 < episodi_lkm)
			episodisivu++;
	}

	for (int i=(episodisivu*10)+2;i<(episodisivu*10)+12;i++)
	{
		if (strcmp(episodit[i],"") != 0)
		{
			if (PK_Piirra_Menut_Valinta(episodit[i],220,90+my))
			{
				strcpy(episodi,episodit[i]);
				PK_Lataa_Lisainfot();
				pelin_seuraava_tila = TILA_KARTTA;
				peli_kesken = false;
				PK_Uusi_Peli();
				//PisteDraw_Fade_Paletti_In(PD_FADE_NORMAALI);
			}
			my += 20;
		}
	}

	/* sivu / kaikki */
/*	if (episodi_lkm-2 > 10) {
		char luku[20];
		int vali = 0;
		int x = 500,
			y = 300;


		//vali += PisteDraw_Font_Kirjoita(fontti1,"page:",PD_TAUSTABUFFER,x,y+40);
		itoa(episodisivu+1,luku,10);
		vali += PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,x+vali,y+40);
		vali += PisteDraw_Font_Kirjoita(fontti1,"/",PD_TAUSTABUFFER,x+vali,y+40);
		itoa((episodi_lkm/10)+1,luku,10);
		vali += PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,x+vali,y+40);
		/*
		itoa((episodisivu*10)+1,luku,10);
		vali += PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,500,340);
		vali += PisteDraw_Font_Kirjoita(fontti1,"-",PD_TAUSTABUFFER,500+vali,340);

		if ((episodisivu*10)+10 <= episodi_lkm-2)
			itoa((episodisivu*10)+10,luku,10);
		else
			itoa(episodi_lkm-2,luku,10);
		

		vali += PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,500+vali,340);
		vali += PisteDraw_Font_Kirjoita(fontti1,"/",PD_TAUSTABUFFER,500+vali,340);
		itoa(episodi_lkm-2,luku,10);
		vali += PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,500+vali,340);
		*/
/*
		int nappi = PK_Piirra_Menut_Saatolaatikko(x,y);

		if (nappi == 1 && episodisivu > 0)
			episodisivu--;
		
		if (nappi == 2 && (episodisivu*10)+10 < episodi_lkm)
			episodisivu++;
*/
/*	}

	/*
	char luku[20];
	itoa(episodi_lkm,luku,10);
	PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,100,240);
	*/

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_return),180,400)) 
	{
		menu_nyt = MENU_PAAVALIKKO;
		my += 20;
	}

	PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_episodes_get_more),PD_TAUSTABUFFER,140,440);

	return 0;
}

int PK_Piirra_Menut()
{
	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);
	
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,0);
	
	PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,0,0);

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
	default					: PK_Piirra_Menut_PaaValikko(); break;
	}

	//PK_Partikkelit_Piirra();

	PK_Piirra_Kursori(hiiri_x,hiiri_y);

	PisteWait_Wait(0);//10

	//if (PisteDraw_Fade_Paletti_Valmis())
	//	PisteDraw_Fade_Paletti_In(PD_FADE_NORMAALI);

	PisteDraw_Paivita_Naytto();

	PisteWait_Start();

	return 0;
}

int PK_Piirra_Nuppi(int x, int y, int t)
{
	
	int paluu = 0;
	
	t = t * 25;

	int vilkku = 50 + (int)(sin_table[degree%360]*2);

	if (vilkku < 0)
		vilkku = 0;
	
	//PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x,y,1+t,58,23+t,80);

	if (hiiri_x > x && hiiri_x < x+17 && hiiri_y > y && hiiri_y < y+17)
	{
		if (key_delay == 0 && (PisteInput_Hiiri_Vasen() || PisteInput_Keydown(DIK_SPACE) 
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
		PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,x,y,1+t,58,23+t,80);	

	return paluu;
}

int PK_Piirra_Kartta()
{
	char luku[20];
	int vali = 20;

	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);
	
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,0);

	PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,0,0);

	PisteDraw_Font_Kirjoita(fontti4,episodi,PD_TAUSTABUFFER,100+2,72+2);
	PisteDraw_Font_Kirjoita(fontti2,episodi,PD_TAUSTABUFFER,100,72);

	PisteDraw_Font_Kirjoita(fontti4,tekstit->Hae_Teksti(txt_map_total_score),PD_TAUSTABUFFER,100+2,92+2);
	vali = PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_map_total_score),PD_TAUSTABUFFER,100,92);//250,80
	ltoa(pisteet,luku,10);
	PisteDraw_Font_Kirjoita(fontti4,luku,PD_TAUSTABUFFER,100+vali+2+15,92+2);
	PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,100+vali+15,92);

	if (episodipisteet.episodin_top_pisteet > 0) {
		vali = PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_map_episode_best_player),PD_TAUSTABUFFER,360,72);
		PisteDraw_Font_Kirjoita(fontti1,episodipisteet.episodin_top_pelaaja,PD_TAUSTABUFFER,360+vali+10,72);
		vali = PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_map_episode_hiscore),PD_TAUSTABUFFER,360,92);
		ltoa(episodipisteet.episodin_top_pisteet,luku,10);	
		PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,360+vali+15,92);
	}
	
	vali = PisteDraw_Font_Kirjoita(fontti1,tekstit->Hae_Teksti(txt_map_next_level),PD_TAUSTABUFFER,100,120);
	ltoa(jakso,luku,10);
	PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,100+vali+15,120);

		//PK_Partikkelit_Piirra();

	if (jaksoja == 0) {
		PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_episodes_no_maps),PD_TAUSTABUFFER,180,290);
	}

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_mainmenu_return),100,430)) 
	{
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
				tyyppi = 1;						//1 vihreä
			if (jaksot[i].jarjestys > jakso)
				tyyppi = 2;						//2 oranssi
			if (jaksot[i].lapaisty)
				tyyppi = 0;

			if (jaksot[i].x == 0)
				jaksot[i].x = 142+i*30;

			if (jaksot[i].y == 0)
				jaksot[i].y = 270;			

			ikoni = jaksot[i].ikoni;

			//PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,jaksot[i].x-4,jaksot[i].y-4-30,1+(ikoni*27),452,27+(ikoni*27),478);
			PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,jaksot[i].x-9,jaksot[i].y-14,1+(ikoni*28),452,28+(ikoni*28),479);

			if (tyyppi==1)
			{
				//PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,jaksot[i].x-30,jaksot[i].y-4,157,46,181,79);
				sinx = (int)(sin_table[degree%360]/2);
				cosy = (int)(cos_table[degree%360]/2);
				pekkaframe = 28*((degree%360)/120);
				PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,jaksot[i].x+sinx-12,jaksot[i].y-17+cosy,157+pekkaframe,46,181+pekkaframe,79);
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
					PisteDraw_Fade_Paletti_Out(PD_FADE_HIDAS);
					musiikin_voimakkuus = 0;
					PK_Soita_Aani_Menu(kieku_aani,90);
				}
				else
					PK_Soita_Aani_Menu(ammuu_aani,100);
			}
			
			itoa(jaksot[i].jarjestys,luku,10);
			PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,jaksot[i].x-12+2,jaksot[i].y-29+2);
			//PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,jaksot[i].x+3,jaksot[i].y-27);
			

			if (paluu > 0) {
				
				int info_x = 489+3, info_y = 341-26;
				/*
				PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,jaksot[i].x-3, jaksot[i].y+26,jaksot[i].x + 130, jaksot[i].y+26+120,1);
				PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,jaksot[i].x-2, jaksot[i].y+27,jaksot[i].x + 129, jaksot[i].y+25+120,43);
				PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,jaksot[i].x-1, jaksot[i].y+28,jaksot[i].x + 128, jaksot[i].y+24+120,38);*/
				PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,info_x-3,info_y+26,473,0,607,121);
				PisteDraw_Font_Kirjoita(fontti1,jaksot[i].nimi,PD_TAUSTABUFFER,info_x,info_y+30);
				
				if (episodipisteet.pisteet[i] > 0) {
					PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,tekstit->Hae_Teksti(txt_map_level_best_player),PD_TAUSTABUFFER,info_x,info_y+50,75);
					PisteDraw_Font_Kirjoita(fontti1,episodipisteet.top_pelaajat[i],PD_TAUSTABUFFER,info_x,info_y+62);
					vali = 8 + PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,tekstit->Hae_Teksti(txt_map_level_hiscore),PD_TAUSTABUFFER,info_x,info_y+74,75);
					ltoa(episodipisteet.pisteet[i],luku,10);
					PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,info_x+vali,info_y+75);
				}

				if (episodipisteet.ajat[i] > 0) {
					PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,tekstit->Hae_Teksti(txt_map_level_fastest_player),PD_TAUSTABUFFER,info_x,info_y+98,75);
					PisteDraw_Font_Kirjoita(fontti1,episodipisteet.nopeimmat_pelaajat[i],PD_TAUSTABUFFER,info_x,info_y+110);
					
					vali = 8 + PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,tekstit->Hae_Teksti(txt_map_level_best_time),PD_TAUSTABUFFER,info_x,info_y+122,75);
					min = episodipisteet.ajat[i]/60;
					sek = episodipisteet.ajat[i]%60;

					itoa(min,luku,10);
					vali += PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,info_x+vali,info_y+122);
					vali += PisteDraw_Font_Kirjoita(fontti1,":",PD_TAUSTABUFFER,info_x+vali,info_y+122);
					itoa(sek,luku,10);
					PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,info_x+vali,info_y+122);
				}
			}
		}
	}
	
	PK_Piirra_Kursori(hiiri_x,hiiri_y);

	PisteWait_Wait(0);//10

	PisteDraw_Paivita_Naytto();

	PisteWait_Start();

	return 0;
}

int PK_Piirra_Pistelasku()
{
	char luku[20];
	int vali = 20;
	int my = 0, x, y;
	int kuutio, aste;
	int	vari = 0, kerroin;

	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);
	
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,0);

	PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,0,0);

	for (aste=0;aste<18;aste++) {
		
		kerroin = (int)(cos_table[(degree+aste*3)%180]);

		x = (int)(sin_table[(degree+aste*10)%360]*2)+kerroin;
		y = (int)(cos_table[(degree+aste*10)%360]*2);//10 | 360 | 2
		//PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,320+x,240+y,157,46,181,79);
		kuutio = (int)(sin_table[(degree+aste*3)%360]);
		if (kuutio < 0) kuutio = -kuutio;
		
		PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,320-x,240-y,320-x+kuutio,240-y+kuutio,VARI_TURKOOSI+8);
	}

//	UCHAR *buffer_lahde = NULL, 
//		  *buffer_kohde = NULL;
//	DWORD leveys_buffer_lahde, 
//		  leveys_buffer_kohde;

//	PisteDraw_Piirto_Aloita(kuva_peli,    *&buffer_lahde, (DWORD &)leveys_buffer_lahde);
//	PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer_kohde, (DWORD &)leveys_buffer_kohde);

	for (aste=0;aste<18;aste++) {
		
		x = (int)(sin_table[(degree+aste*10)%360]*3);
		y = (int)(cos_table[(degree+aste*10)%360]*3);//10 | 360 | 3
		//PisteDraw_Buffer_Flip_Nopea(kuva_peli,PD_TAUSTABUFFER,320+x,240+y,157,46,181,79);
		kuutio = (int)(sin_table[(degree+aste*2)%360])+18;
		if (kuutio < 0) kuutio = -kuutio;//0;//
		if (kuutio > 100) kuutio = 100;
		
		//PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,320+x,240+y,320+x+kuutio,240+y+kuutio,VARI_TURKOOSI+10);
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 247, 1, 25, 25, 320+x, 240+y, kuutio, 32);

//		PK_Piirra_Lapinakyva_Objekti3(kuva_peli, 247, 1, 25, 25, 320+x, 240+y, kuutio, 32);
//

//		PK_Piirra_Lapinakyva_Objekti4(247, 1, 25, 25, 320+x, 240+y, kuutio, 32,
//									 buffer_lahde, buffer_kohde, leveys_buffer_lahde, leveys_buffer_kohde);
	
	}

//	PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER);
//	PisteDraw_Piirto_Lopeta(kuva_peli);


	PisteDraw_Font_Kirjoita(fontti4,tekstit->Hae_Teksti(txt_score_screen_title),PD_TAUSTABUFFER,100+2,72+2);
	PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_score_screen_title),PD_TAUSTABUFFER,100,72);
	PisteDraw_Font_Kirjoita(fontti4,tekstit->Hae_Teksti(txt_score_screen_level_score),PD_TAUSTABUFFER,100+2,102+2);
	vali = PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_score_screen_level_score),PD_TAUSTABUFFER,100,102);//250,80
	fake_pisteet = bonuspisteet + aikapisteet + energiapisteet + esinepisteet + pelastuspisteet;
	ltoa(fake_pisteet,luku,10);
	PisteDraw_Font_Kirjoita(fontti4,luku,PD_TAUSTABUFFER,400+2,102+2);
	PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,400,102);
	my = 0;

	PisteDraw_Font_Kirjoita(fontti4,tekstit->Hae_Teksti(txt_score_screen_bonus_score),PD_TAUSTABUFFER,100+2,192+2+my);
	vali = PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_score_screen_bonus_score),PD_TAUSTABUFFER,100,192+my);		
	ltoa(bonuspisteet,luku,10);
	PisteDraw_Font_Kirjoita(fontti4,luku,PD_TAUSTABUFFER,400+2,192+2+my);
	PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,400,192+my);
	my += 30;

	PisteDraw_Font_Kirjoita(fontti4,tekstit->Hae_Teksti(txt_score_screen_time_score),PD_TAUSTABUFFER,100+2,192+2+my);
	vali = PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_score_screen_time_score),PD_TAUSTABUFFER,100,192+my);		
	ltoa(aikapisteet,luku,10);
	PisteDraw_Font_Kirjoita(fontti4,luku,PD_TAUSTABUFFER,400+2,192+2+my);
	PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,400,192+my);	
	my += 30;

	PisteDraw_Font_Kirjoita(fontti4,tekstit->Hae_Teksti(txt_score_screen_energy_score),PD_TAUSTABUFFER,100+2,192+2+my);
	vali = PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_score_screen_energy_score),PD_TAUSTABUFFER,100,192+my);		
	ltoa(energiapisteet,luku,10);
	PisteDraw_Font_Kirjoita(fontti4,luku,PD_TAUSTABUFFER,400+2,192+2+my);
	PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,400,192+my);
	my += 30;

	PisteDraw_Font_Kirjoita(fontti4,tekstit->Hae_Teksti(txt_score_screen_item_score),PD_TAUSTABUFFER,100+2,192+2+my);
	vali = PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_score_screen_item_score),PD_TAUSTABUFFER,100,192+my);		
	ltoa(esinepisteet,luku,10);
	PisteDraw_Font_Kirjoita(fontti4,luku,PD_TAUSTABUFFER,400+2,192+2+my);
	PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,400,192+my);
	my += 30;

	
	
/*
	ltoa(fake_pisteet,luku,10);
	PisteDraw_Font_Kirjoita(fontti4,luku,PD_TAUSTABUFFER,400+2,192+2);
	PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,400,192);	
*/

	x = 110;
	y = 192 + my;

	for (int i=0;i<MAX_ESINEITA;i++)
		if (esineet[i] != NULL)	{
			esineet[i]->Piirra(x-esineet[i]->leveys/2,y-esineet[i]->korkeus/2,0);
			x += 38;
		}

	my += 10;

	if (pistelaskuvaihe >= 4)
	{
		PisteDraw_Font_Kirjoita(fontti4,tekstit->Hae_Teksti(txt_score_screen_total_score),PD_TAUSTABUFFER,100+2,192+2+my);
		vali = PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_score_screen_total_score),PD_TAUSTABUFFER,100,192+my);//250,80
		ltoa(pisteet,luku,10);
		PisteDraw_Font_Kirjoita(fontti4,luku,PD_TAUSTABUFFER,400+2,192+2+my);
		PisteDraw_Font_Kirjoita(fontti2,luku,PD_TAUSTABUFFER,400,192+my);
		my += 25;
		
		if (jakso_uusi_ennatys) {
			PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_score_screen_new_level_hiscore),PD_TAUSTABUFFER,100+rand()%2,192+my+rand()%2);
			my += 25;
		}
		if (jakso_uusi_ennatysaika) {
			PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_score_screen_new_level_best_time),PD_TAUSTABUFFER,100+rand()%2,192+my+rand()%2);
			my += 25;
		}
		if (episodi_uusi_ennatys) {
			PisteDraw_Font_Kirjoita(fontti2,tekstit->Hae_Teksti(txt_score_screen_new_episode_hiscore),PD_TAUSTABUFFER,100+rand()%2,192+my+rand()%2);
			my += 25;
		}
	}

	if (PK_Piirra_Menut_Valinta(tekstit->Hae_Teksti(txt_score_screen_continue),100,430)) 
	{
		musiikin_voimakkuus = 0;
		siirry_pistelaskusta_karttaan = true;
		PisteDraw_Fade_Paletti_Out(PD_FADE_HIDAS);
		//pelin_seuraava_tila = TILA_KARTTA;
	}
	
	PK_Piirra_Kursori(hiiri_x,hiiri_y);

	PisteWait_Wait(0);//10

	PisteDraw_Paivita_Naytto();

	PisteWait_Start();

	return 0;
}

void PK_Piirra_Intro_Teksti(char *teksti, int fontti, int x, int y, DWORD alkuaika, DWORD loppuaika)
{
	int pros = 100;
	if (introlaskuri > alkuaika && introlaskuri < loppuaika) {
		
		if (introlaskuri - alkuaika < 100)
			pros = introlaskuri - alkuaika;

		if (loppuaika - introlaskuri < 100)
			pros = loppuaika - introlaskuri;		

		if (pros > 0) {
			if (pros < 100)
				PisteDraw_Font_Kirjoita_Lapinakyva(fontti,teksti,PD_TAUSTABUFFER,x,y,pros);
			else
				PisteDraw_Font_Kirjoita(fontti,teksti,PD_TAUSTABUFFER,x,y);
		}

	}	
}

int PK_Piirra_Intro()
{
	DWORD pistelogo_alku	= 300;
	DWORD pistelogo_loppu	= pistelogo_alku + 500;
	DWORD tekijat_alku		= pistelogo_loppu + 80;
	DWORD tekijat_loppu		= tekijat_alku + 700;
	DWORD testaajat_alku	= tekijat_loppu + 80;
	DWORD testaajat_loppu	= testaajat_alku + 700;
	DWORD kaantaja_alku		= testaajat_loppu + 100;
	DWORD kaantaja_loppu	= kaantaja_alku + 300;
	DWORD midas_alku		= kaantaja_loppu + 100;
	DWORD midas_loppu		= midas_alku + 300;
	
	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);
	
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,0);

	PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,280,80, 280, 80, 640, 480);

	if ((introlaskuri / 10) % 50 == 0)
		PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,353, 313, 242, 313, 275, 432);

	if (introlaskuri > pistelogo_alku && introlaskuri < pistelogo_loppu) {

		//int x = introlaskuri - pistelogo_alku - 194;
		int kerroin = 120 / (introlaskuri - pistelogo_alku);
		int x = 120 - kerroin;

		if (x > 120) 
			x = 120;
		
		PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,/*120*/x,230, 37, 230, 194, 442);

		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_presents), fontti1, 230, 400, pistelogo_alku, pistelogo_loppu-20);

	}
	
	if (introlaskuri > tekijat_alku) {
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_a_game_by),fontti1, 120, 230, tekijat_alku, tekijat_loppu);
		PK_Piirra_Intro_Teksti("janne kivilahti",			fontti1, 120, 250, tekijat_alku+20, tekijat_loppu+20);
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_original), fontti1, 120, 275, tekijat_alku+40, tekijat_loppu+40);
		PK_Piirra_Intro_Teksti("antti suuronen 1998",		fontti1, 120, 295, tekijat_alku+50, tekijat_loppu+50);
	}

	if (introlaskuri > testaajat_alku) {
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_tested_by),fontti1, 120, 230, testaajat_alku, testaajat_loppu);
		PK_Piirra_Intro_Teksti("antti suuronen",			fontti1, 120, 250, testaajat_alku+10, testaajat_loppu+10);
		PK_Piirra_Intro_Teksti("toni hurskainen",			fontti1, 120, 260, testaajat_alku+20, testaajat_loppu+20);
		PK_Piirra_Intro_Teksti("juho rytkönen",				fontti1, 120, 270, testaajat_alku+30, testaajat_loppu+30);
		PK_Piirra_Intro_Teksti("annukka korja",				fontti1, 120, 280, testaajat_alku+40, testaajat_loppu+40);
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_thanks_to),fontti1, 120, 300, testaajat_alku+70, testaajat_loppu+70);
		PK_Piirra_Intro_Teksti("oskari raunio",				fontti1, 120, 310, testaajat_alku+70, testaajat_loppu+70);
		PK_Piirra_Intro_Teksti("assembly organization",		fontti1, 120, 320, testaajat_alku+70, testaajat_loppu+70);
	}

	if (introlaskuri > kaantaja_alku) {
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_translation), fontti1, 120, 230, kaantaja_alku, kaantaja_loppu);
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_translator),  fontti1, 120, 250, kaantaja_alku+20, kaantaja_loppu+20);
	}

	if (introlaskuri > midas_alku) {
		PK_Piirra_Intro_Teksti("housemarque sound system", fontti1, 120, 230, midas_alku, midas_loppu);
		PK_Piirra_Intro_Teksti("petteri kangaslampi",  fontti1, 120, 250, midas_alku+20, midas_loppu+20);
		PK_Piirra_Intro_Teksti("jarno paananen",  fontti1, 120, 270, midas_alku+40, midas_loppu+40);
	}


	
	PisteWait_Wait(0);

	PisteDraw_Paivita_Naytto();

	PisteWait_Start();

	return 0;
}

int PK_Piirra_Loppu_Hahmo(int x, int y, int tyyppi, int plus, int rapytys)
{
	int frm = 0;
	int yk = 0;
	
	if (tyyppi == 1) { // Pekka
		frm = 1;
		if ((degree/10)%10==rapytys) frm = 0;
		yk = (int)sin_table[(degree%360)];
		PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,x+3,y+56, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,x,y, 1+frm*35, 1, 32+frm*35, 59);
	}

	if (tyyppi == 2) { // kana (katse oikealle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)cos_table[((degree+plus)%360)];
		PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,x+3,y+36, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,x,y, 106+frm*37, 1, 139+frm*37, 38);
	}

	if (tyyppi == 3) { // kana (katse vasemmalle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)cos_table[((degree+plus)%360)];
		PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,x+3,y+36, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,x,y, 106+frm*37, 41, 139+frm*37, 77);
	}	

	if (tyyppi == 4) { // pikkukana (katse oikealle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)sin_table[(((degree*2)+plus)%360)];
		PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,x+3,y+27, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,x,y, 217+frm*29, 1, 243+frm*29, 29);
	}

	if (tyyppi == 5) { // pikkukana (katse vasemmalle)
		frm = 0;
		if ((degree/10)%10==rapytys) frm = 1;
		yk = (int)sin_table[(((degree*2)+plus)%360)];
		PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,x,y+27, 4, 63, 29, 69);
		if (yk < 0) {
			y+=yk;
			frm = 2;
		}
		PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,x,y, 217+frm*29, 33, 243+frm*29, 61);
	}

	return 0;
}

int PK_Piirra_Loppu()
{
	
	DWORD onnittelut_alku	= 300;
	DWORD onnittelut_loppu	= onnittelut_alku + 1000;
	DWORD the_end_alku		= onnittelut_loppu + 80;
	DWORD the_end_loppu		= the_end_alku + 3000;
	/*
	DWORD testaajat_alku	= tekijat_loppu + 80;
	DWORD testaajat_loppu	= testaajat_alku + 700;
	DWORD kaantaja_alku		= testaajat_loppu + 100;
	DWORD kaantaja_loppu	= kaantaja_alku + 300;
	*/
	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);
	
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,0);

	PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,320-233/2,240-233/2, 6, 229, 239, 462);
	
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
	/*	
	if (introlaskuri > tekijat_alku) {
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_a_game_by),fontti1, 120, 230, tekijat_alku, tekijat_loppu);
		PK_Piirra_Intro_Teksti("janne kivilahti",			fontti1, 120, 250, tekijat_alku+20, tekijat_loppu+20);
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_original), fontti1, 120, 275, tekijat_alku+40, tekijat_loppu+40);
		PK_Piirra_Intro_Teksti("antti suuronen 1998",		fontti1, 120, 295, tekijat_alku+50, tekijat_loppu+50);
	}

	if (introlaskuri > testaajat_alku) {
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_tested_by),fontti1, 120, 230, testaajat_alku, testaajat_loppu);
		PK_Piirra_Intro_Teksti("antti suuronen",			fontti1, 120, 250, testaajat_alku+10, testaajat_loppu+10);
		PK_Piirra_Intro_Teksti("toni hurskainen",			fontti1, 120, 260, testaajat_alku+20, testaajat_loppu+20);
		PK_Piirra_Intro_Teksti("juho rytkönen",				fontti1, 120, 270, testaajat_alku+30, testaajat_loppu+30);
		PK_Piirra_Intro_Teksti("annukka korja",				fontti1, 120, 280, testaajat_alku+40, testaajat_loppu+40);
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_thanks_to),fontti1, 120, 300, testaajat_alku+70, testaajat_loppu+70);
		PK_Piirra_Intro_Teksti("oskari raunio",				fontti1, 120, 310, testaajat_alku+70, testaajat_loppu+70);
		PK_Piirra_Intro_Teksti("assembly organization",		fontti1, 120, 320, testaajat_alku+70, testaajat_loppu+70);
	}

	if (introlaskuri > kaantaja_alku) {
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_translation), fontti1, 120, 230, kaantaja_alku, kaantaja_loppu);
		PK_Piirra_Intro_Teksti(tekstit->Hae_Teksti(txt_intro_translator),  fontti1, 120, 250, kaantaja_alku+20, kaantaja_loppu+20);
	}

	if ((introlaskuri / 10) % 50 == 0)
		PisteDraw_Buffer_Flip_Nopea(kuva_tausta,PD_TAUSTABUFFER,353, 313, 242, 313, 275, 432);
*/	
	PisteWait_Wait(0);

	PisteDraw_Paivita_Naytto();

	PisteWait_Start();

	return 0;
}

/* MUUT RUTIINIT -------------------------------------------------------*/

int PK_Main_Intro(void)
{

	hiiri_x += PisteInput_Hiiri_X(0);
	hiiri_y += PisteInput_Hiiri_Y(0);

	hiiri_x += PisteInput_Ohjain_X(PI_PELIOHJAIN_1)/30;
	hiiri_y += PisteInput_Ohjain_Y(PI_PELIOHJAIN_1)/30;

	if (PisteInput_Keydown(DIK_LEFT))
		hiiri_x -= 3;

	if (PisteInput_Keydown(DIK_RIGHT))
		hiiri_x += 3;

	if (PisteInput_Keydown(DIK_UP))
		hiiri_y -= 3;

	if (PisteInput_Keydown(DIK_DOWN))
		hiiri_y += 3;

	if (hiiri_x < 0)
		hiiri_x = 0;

	if (hiiri_y < 0)
		hiiri_y = 0;

	if (hiiri_x > 640-19)
		hiiri_x = 640-19;

	if (hiiri_y > 480-19)
		hiiri_y = 480-19;
	
	PK_Piirra_Intro();

	degree = 1 + degree % 360;

	introlaskuri++;

	if (siirry_introsta_menuun && PisteDraw_Fade_Paletti_Valmis())
	{
		pelin_seuraava_tila = TILA_MENUT;
		peli_kesken = false;
	}

	if (key_delay > 0)
		key_delay--;

	if (key_delay == 0)
	{
		if (PisteInput_Keydown(DIK_RETURN) || PisteInput_Keydown(DIK_SPACE) || introlaskuri == 3500)
		{
			siirry_introsta_menuun = true;
			PisteDraw_Fade_Paletti_Out(PD_FADE_HIDAS);
		}
	}

	return 0;
}


int PK_Main_Pistelasku(void)
{

	hiiri_x += PisteInput_Hiiri_X(0);
	hiiri_y += PisteInput_Hiiri_Y(0);

	hiiri_x += PisteInput_Ohjain_X(PI_PELIOHJAIN_1)/30;
	hiiri_y += PisteInput_Ohjain_Y(PI_PELIOHJAIN_1)/30;

	if (PisteInput_Keydown(DIK_LEFT))
		hiiri_x -= 3;

	if (PisteInput_Keydown(DIK_RIGHT))
		hiiri_x += 3;

	if (PisteInput_Keydown(DIK_UP))
		hiiri_y -= 3;

	if (PisteInput_Keydown(DIK_DOWN))
		hiiri_y += 3;

	if (hiiri_x < 0)
		hiiri_x = 0;

	if (hiiri_y < 0)
		hiiri_y = 0;

	if (hiiri_x > 640-19)
		hiiri_x = 640-19;

	if (hiiri_y > 480-19)
		hiiri_y = 480-19;
	
	PK_Piirra_Pistelasku();

	degree = 1 + degree % 360;

	// PISTELASKU

	int energia = spritet[pelaaja_index].energia;
	
	if (pistelaskudelay == 0)
		if (bonuspisteet < jakso_pisteet)
		{
			pistelaskuvaihe = 1;
			pistelaskudelay = 0;
			bonuspisteet += 10;
			
			if (degree%7==1)
				PK_Soita_Aani_Menu(pistelaskuri_aani, 70);
			
			if (bonuspisteet >= jakso_pisteet) {
				bonuspisteet = jakso_pisteet;
				pistelaskudelay = 50;
			}
			
		}
		else if (aika > 0)
		{
			pistelaskuvaihe = 2;
			pistelaskudelay = 0;
			aikapisteet+=5;
			aika--;

			if (degree%10==1)
				PK_Soita_Aani_Menu(pistelaskuri_aani, 70);			
			
			if (aika == 0) 
				pistelaskudelay = 50;
			
		}
		else if (spritet[pelaaja_index].energia > 0)
		{
			pistelaskuvaihe = 3;
			pistelaskudelay = 10;
			energiapisteet+=300;
			spritet[pelaaja_index].energia--;
			
			PK_Soita_Aani_Menu(pistelaskuri_aani, 70);
		
		}
		else if (esineita > 0)
		{
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

	if (siirry_pistelaskusta_karttaan && PisteDraw_Fade_Paletti_Valmis())
	{
		/*tarkistetaan oliko viimeinen jakso*/
		
		if (jakso_indeksi_nyt == EPISODI_MAX_JAKSOJA-1) { // ihan niin kuin joku tekisi näin monta jaksoa...
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

	if (key_delay == 0)
	{
		if (PisteInput_Keydown(DIK_RETURN) && pistelaskuvaihe == 5)
		{
			siirry_pistelaskusta_karttaan = true;
			musiikin_voimakkuus = 0;
			PisteDraw_Fade_Paletti_Out(PD_FADE_HIDAS);
			key_delay = 20;
		}
		
		if (PisteInput_Keydown(DIK_RETURN) && pistelaskuvaihe < 5)
		{
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

int PK_Main_Kartta(void)
{

	hiiri_x += PisteInput_Hiiri_X(0);
	hiiri_y += PisteInput_Hiiri_Y(0);

	hiiri_x += PisteInput_Ohjain_X(PI_PELIOHJAIN_1)/30;
	hiiri_y += PisteInput_Ohjain_Y(PI_PELIOHJAIN_1)/30;

	if (PisteInput_Keydown(DIK_LEFT))
		hiiri_x -= 3;

	if (PisteInput_Keydown(DIK_RIGHT))
		hiiri_x += 3;

	if (PisteInput_Keydown(DIK_UP))
		hiiri_y -= 3;

	if (PisteInput_Keydown(DIK_DOWN))
		hiiri_y += 3;

	if (hiiri_x < 0)
		hiiri_x = 0;

	if (hiiri_y < 0)
		hiiri_y = 0;

	if (hiiri_x > 640-19)
		hiiri_x = 640-19;

	if (hiiri_y > 480-19)
		hiiri_y = 480-19;
	
	PK_Piirra_Kartta();

	degree = 1 + degree % 360;

	if (siirry_kartasta_peliin && PisteDraw_Fade_Paletti_Valmis())
	{
		pelin_seuraava_tila = TILA_PELI;
		//strcpy(seuraava_kartta,jaksot[i].tiedosto);
		peli_kesken = false;//true;
		//PisteDraw_Fade_Paletti_In(PD_FADE_NORMAALI);
	}

	if (key_delay > 0)
		key_delay--;

	if (key_delay == 0)
	{
		/*
		if (PisteInput_Keydown(DIK_RETURN))
		{
			siirry_kartasta_peliin = true;
			//pelin_seuraava_tila = TILA_PELI;
			PisteDraw_Fade_Paletti_Out(PD_FADE_HIDAS);
		}*/
	}

	return 0;
}

int PK_Main_Menut(void)
{

	hiiri_x += PisteInput_Hiiri_X(0);
	hiiri_y += PisteInput_Hiiri_Y(0);
/*
	hiiri_x += PisteInput_Ohjain_X(PI_PELIOHJAIN_1)/30;
	hiiri_y += PisteInput_Ohjain_Y(PI_PELIOHJAIN_1)/30;
*/
	if (!nimiedit && key_delay == 0 && menu_lue_kontrollit == 0) {
/*
		if (PisteInput_Keydown(DIK_LEFT));
			//hiiri_x -= 3;

		if (PisteInput_Keydown(DIK_RIGHT));
			//hiiri_x += 3;
*/
		if (PisteInput_Keydown(DIK_UP) || PisteInput_Keydown(DIK_LEFT) ||
			PisteInput_Ohjain_X(PI_PELIOHJAIN_1) < 0 || PisteInput_Ohjain_Y(PI_PELIOHJAIN_1) < 0){
			menu_valittu_id--;

			if (menu_valittu_id < 1)
				menu_valittu_id = menu_valinta_id-1;

			key_delay = 15;
			//hiiri_y -= 3;
		}

		if (PisteInput_Keydown(DIK_DOWN) || PisteInput_Keydown(DIK_RIGHT) ||
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

	if (hiiri_x < 0)
		hiiri_x = 0;

	if (hiiri_y < 0)
		hiiri_y = 0;

	if (hiiri_x > 640-19)
		hiiri_x = 640-19;

	if (hiiri_y > 480-19)
		hiiri_y = 480-19;

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

int PK_Main_Peli(void)
{
	PK_Kamera();
	
	PK_Piirra_Peli();
	
	PK_Palikka_Paivita_Lasketut_Palikat();

	if (!pause)
	{
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
					//PisteDraw_Fade_Paletti_Out(PD_FADE_HIDAS);
				}
			}
		}

		if (nakymattomyys > 0)
			nakymattomyys--;

	}
	
	// framelaskurit

	fps_laskuri++;

	if (PisteWait_Get() >= fps_aika+1000)
	{
		fps = fps_laskuri;
		fps_laskuri = 0;
		fps_aika = PisteWait_Get();
	}

	if (spritet[pelaaja_index].energia < 1 && !peli_ohi)
	{
		peli_ohi = true;
		key_delay = 50;
	}

	if (key_delay > 0)
		key_delay--;

	if (jakso_lapaisty || peli_ohi)
	{
		if (lopetusajastin > 1)
			lopetusajastin--;
		
		if (lopetusajastin == 0)
			lopetusajastin = 800;//2000;

		if (PisteInput_Lue_Kontrolli(kontrolli_hyokkays1) || PisteInput_Lue_Kontrolli(kontrolli_hyokkays2) ||
			PisteInput_Lue_Kontrolli(kontrolli_hyppy) || PisteInput_Keydown(DIK_RETURN))
			if (lopetusajastin > 2 && lopetusajastin < 600/*1900*/ && key_delay == 0)
				lopetusajastin = 2;

		if (lopetusajastin == 2)
		{
			PisteDraw_Fade_Paletti_Out(PD_FADE_NORMAALI);
			//musiikin_voimakkuus = 0;
		}
	}
/*
	if (jakso_lapaisty)
	{
		if (aika > 0)
		{
			fake_pisteet+=5;
			aika--;
		}
	}
*/
	if (lopetusajastin == 1 && PisteDraw_Fade_Paletti_Valmis())
	{
		if (jakso_lapaisty)
			pelin_seuraava_tila = TILA_PISTELASKU;//TILA_KARTTA;
		else
			pelin_seuraava_tila = TILA_KARTTA;
	}

	if (key_delay == 0)
	{

		if (PisteInput_Lue_Kontrolli(kontrolli_kayta_esine) && spritet[pelaaja_index].energia > 0)
		{
			PK_Esineet_Kayta();
			key_delay = 10;
		}

		if (PisteInput_Keydown(DIK_P) && !jakso_lapaisty)
		{
			pause = !pause;
			key_delay = 20;
		}	

		if (PisteInput_Keydown(DIK_DELETE))
			spritet[pelaaja_index].energia = 0;	

		
		if (PisteInput_Keydown(DIK_I))
		{
			fps_nayta = !fps_nayta;
			key_delay = 20;
		}

		if (PisteInput_Keydown(DIK_TAB))
		{
			PK_Esineet_Vaihda();
			key_delay = 10;
		}

	}

	if (key_delay == 0 && huijaukset)
	{

		if (PisteInput_Keydown(DIK_Z))
		{
			kytkin1 = KYTKIN_ALOITUSARVO;
			kytkin2 = KYTKIN_ALOITUSARVO;
			kytkin3 = KYTKIN_ALOITUSARVO;
			key_delay = 10;
		}

		if (PisteInput_Keydown(DIK_G))
		{
			asetukset.lapinakyvat_objektit = !asetukset.lapinakyvat_objektit;
			key_delay = 20;
		}

		if (PisteInput_Keydown(DIK_L))
		{
			PK_Kartta_Avaa_Lukot();
			key_delay = 20;
		}

		if (PisteInput_Keydown(DIK_K))
		{
			PK_Kartta_Vaihda_Kallopalikat();
			key_delay = 20;
		}

		if (PisteInput_Keydown(DIK_F))
		{
			fps_nayta = !fps_nayta;
			key_delay = 20;
		}

		if (PisteInput_Keydown(DIK_I))
		{
			piirra_infot = !piirra_infot;
			key_delay = 10;
		}

		if (PisteInput_Keydown(DIK_R))
		{
			PK_Kartta_Etsi_Alku();
			spritet[pelaaja_index].energia = 10;
			key_delay = 10;
		}

		if (PisteInput_Keydown(DIK_END))
		{
			if (PK_Midas_Lataa_Musiikki("music\\hiscore.xm")!=0)
				PK2_virhe = true;
			
			key_delay = 20;
			jakso_lapaisty = true;
			jaksot[jakso_indeksi_nyt].lapaisty = true;
			jakso++;
		}

		if (PisteInput_Keydown(DIK_E))
			spritet[pelaaja_index].energia = 3;

		if (PisteInput_Keydown(DIK_LSHIFT))
		{
			for (int r=1;r<6;r++)
				PK_Partikkeli_Uusi(PARTIKKELI_KIPINA,spritet[pelaaja_index].x+rand()%10-rand()%10, spritet[pelaaja_index].y+rand()%10-rand()%10,0,0,rand()%100,0.1,32);
			spritet[pelaaja_index] = PK2Sprite(&protot[PROTOTYYPPI_KANA],1,false,spritet[pelaaja_index].x, spritet[pelaaja_index].y);
		}
	}

	return 0;
}

int PK_Main_Loppu(void)
{

	hiiri_x += PisteInput_Hiiri_X(0);
	hiiri_y += PisteInput_Hiiri_Y(0);

	hiiri_x += PisteInput_Ohjain_X(PI_PELIOHJAIN_1)/30;
	hiiri_y += PisteInput_Ohjain_Y(PI_PELIOHJAIN_1)/30;

	if (PisteInput_Keydown(DIK_LEFT))
		hiiri_x -= 3;

	if (PisteInput_Keydown(DIK_RIGHT))
		hiiri_x += 3;

	if (PisteInput_Keydown(DIK_UP))
		hiiri_y -= 3;

	if (PisteInput_Keydown(DIK_DOWN))
		hiiri_y += 3;

	if (hiiri_x < 0)
		hiiri_x = 0;

	if (hiiri_y < 0)
		hiiri_y = 0;

	if (hiiri_x > 640-19)
		hiiri_x = 640-19;

	if (hiiri_y > 480-19)
		hiiri_y = 480-19;
	
	PK_Piirra_Loppu();

	degree = 1 + degree % 360;

	loppulaskuri++;
	introlaskuri = loppulaskuri; // introtekstejä varten

	if (siirry_lopusta_menuun && PisteDraw_Fade_Paletti_Valmis())
	{
		pelin_seuraava_tila = TILA_MENUT;
		menu_nyt = MENU_PAAVALIKKO;
		peli_kesken = false;
	}

	if (key_delay > 0)
		key_delay--;

	if (key_delay == 0)
	{
		if (PisteInput_Keydown(DIK_RETURN) || PisteInput_Keydown(DIK_SPACE))
		{
			siirry_lopusta_menuun = true;
			musiikin_voimakkuus = 0;
			PisteDraw_Fade_Paletti_Out(PD_FADE_HIDAS);
		}
	}

	return 0;
}

int PK_Main(void)
{
	if (window_closed)
		return(0);

	/* HAETAAN NÄPPÄIMISTÖN, HIIREN JA PELIOHJAINTEN TÄMÄNHETKISET TILAT */

	// Näppäimistö 
	if (!PisteInput_Hae_Nappaimet())		//Haetaan näppäinten tilat
		PK2_virhe = true;
	
	// Hiirulainen
	if (!PisteInput_Hae_Hiiri())			//Haetaan hiiren tila
		PK2_virhe = true;	

	if (!PisteInput_Hae_Ohjaimet())			//Haetaan peliohjainten tilat
	{
		//PK2_virhe = true;
	}

	switch (pelin_tila)
	{
	case TILA_PELI		: PK_Main_Peli();break;
	case TILA_MENUT		: PK_Main_Menut();break;
	case TILA_KARTTA	: PK_Main_Kartta();break;
	case TILA_PISTELASKU: PK_Main_Pistelasku();break;
	case TILA_INTRO		: PK_Main_Intro();break;
	case TILA_LOPPU		: PK_Main_Loppu();break;
	default				: lopeta_peli = true;break;
	}

	// MUSIIKIN ÄÄNENSÄÄTÖ

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
		MIDASsetMusicVolume(playHandle,musiikin_voimakkuus_nyt);


	if (PisteInput_Keydown(DIK_ESCAPE) && key_delay == 0) 
	{
		if (menu_nyt != MENU_PAAVALIKKO || pelin_tila != TILA_MENUT)
		{
			pelin_seuraava_tila = TILA_MENUT;
			menu_nyt = MENU_PAAVALIKKO;
			key_delay = 70;
			degree_temp = degree; 
		}
		else if (pelin_tila == TILA_MENUT)
		{
			lopeta_peli = true;
			musiikin_voimakkuus = 0;
			PisteDraw_Fade_Paletti_Out(PD_FADE_NOPEA);
		}
	}

	if (PisteInput_Keydown(DIK_LALT) && PisteInput_Keydown(DIK_TAB)) 
	{
		PisteLog_Kirjoita("- ALT+TAB pressed. \n");
		
		if (ikkuna_aktiivinen) 
			SendMessage(ikkunan_kahva, WM_ACTIVATE,HIWORD(TRUE),0);

		//SendMessage(ikkunan_kahva, WM_CLOSE,0,0);
		//window_closed = true;
	}
/*
	if (PisteInput_Keydown(DIK_LALT) && PisteInput_Keydown(DIK_Q)) 
	{
		if (ikkuna_aktiivinen) 
			SendMessage(ikkunan_kahva, WM_ACTIVATE,HIWORD(TRUE),0);

		PisteLog_Kirjoita("- ALT+Q pressed. \n");

		//SendMessage(ikkunan_kahva, WM_CLOSE,0,0);
		//window_closed = true;
	}
*/
	if (lopeta_peli && PisteDraw_Fade_Paletti_Valmis()) 
	{
		SendMessage(ikkunan_kahva, WM_CLOSE,0,0);
		window_closed = true;
	}
	/*
	if (PisteInput_Keydown(DIK_X)) 
	{
		SendMessage(ikkunan_kahva, WM_CLOSE,0,0);
		window_closed = true;
	}*/

	return 0;
}

int PK_Reload(void) {
	
	if (unload) {

		PisteLog_Kirjoita("  - Reloading all devices. \n");
/*
		PisteLog_Kirjoita("    - Reloading Midas config. \n");
		if ( !MIDASconfig() )
		{
			MIDASvirhe = true;
			asetukset.musiikki = false;
		}
*/
		pelin_seuraava_tila = TILA_PERUSALUSTUS;
		pelin_tila = TILA_EI_ALUSTETTU;
	
		if (PK_Alusta_Tilat() != 0)
			return 1;

		pelin_seuraava_tila = TILA_MENUT;

		//PK_Alusta_Tilat();

		unload = false;

		PisteLog_Kirjoita("  - Reloading all DirectX devices done. \n");
	}
	return 0;
}


int PK_Unload(void) {

	if (!unload) {

		PisteLog_Kirjoita("  - Unloading all DirectX devices. \n");
		
		PisteLog_Kirjoita("    - Unloading PisteDraw \n");
		PisteDraw_Lopeta();

		PisteLog_Kirjoita("    - Unloading PisteInput. \n");
		PisteInput_Lopeta();

		PisteLog_Kirjoita("    - Stopping music. \n");

		if (modi_ladattu && asetukset.musiikki)
		{
			PK_Midas_Pysayta();
			MIDASfreeModule(module);
			modi_ladattu = false;
		}

		PisteLog_Kirjoita("    - Deleting map. \n");
		delete kartta;

		PisteLog_Kirjoita("    - Unloading Midas. \n");
		PK_Midas_Lopeta();

		PisteLog_Kirjoita("    - Unloading Direct Sound. \n");
		PisteSound_Lopeta();

		unload = true;
	}
	
	return 0;
}


int PK_Quit(void) {

	PK_Unload();

	PisteLog_Kirjoita("    - Deleting game texts. \n");
	delete tekstit;

	// Jos on ilmennyt virhe, niin näytetään tekstiboksi.

	if (PK2_virhe)
	{
		char virheilmoitus[255];
		strcpy(virheilmoitus,PisteDraw_Virheilmoitus());
		strcat(virheilmoitus,"\n");
		strcat(virheilmoitus,PisteSound_Virheilmoitus());
		
		MessageBox(ikkunan_kahva, virheilmoitus,"Pekka Kana 2",
				   MB_OK | MB_ICONEXCLAMATION);
	}
	
	return 0;
}


/* IKKUNAT -------------------------------------------------------------------------------------*/

int PK_Kielitiedostot_Hae()
{
	char hakemisto[_MAX_PATH];
	strcpy(hakemisto,"language\\");
	strcat(hakemisto,"*.txt");

    struct _finddata_t map_file;
    long hFile;
	int i=0;


	if((hFile = _findfirst(hakemisto, &map_file )) == -1L )
       return 1;
	else
	{
		SendMessage(hCMB_Languages,CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)map_file.name);
		SendMessage(hCMB_Languages,CB_SETCURSEL, 0, 0);
		i++;
	}

	while (_findnext( hFile, &map_file ) == 0)
	{
		SendMessage(hCMB_Languages,CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)map_file.name);
		/*
		if (strcmp(asetukset.kieli,map_file.name)==0)
			SendMessage(hCMB_Languages,CB_SETCURSEL, i-1, 0);
		*/
		i++;
	}

	SendMessage(hCMB_Languages,CB_SETCURSEL, 0, 0);

	int lkm = i;
	char kielifile[_MAX_PATH];

	for (i=0;i<lkm;i++)
	{
		SendMessage(hCMB_Languages, CB_GETLBTEXT, i, (LPARAM)(LPCSTR)kielifile);
		
		if (strcmp(kielifile, asetukset.kieli) == 0)
			SendMessage(hCMB_Languages,CB_SETCURSEL, i, 0);
	}


	_findclose( hFile );

	return 0;
}

// Seuraa ikkuna-koodia. Allaoleva on ikkunan "tapahtumakuuntelija".

LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	PAINTSTRUCT		ps;		
	HDC				hdc;	

	switch(msg)
	{	
		case WM_ACTIVATE: 
	    {

			//PisteLog_Kirjoita("- Main window's active state has changed.\n");

			if ((BOOL) HIWORD(wparam) != 0) {
				PisteLog_Kirjoita("- Main window has been minimized.\n");
				ikkuna_aktiivinen = false;
				PostQuitMessage(0);
				window_closed = true;

/*
				if (PK_Unload() != 0) {
					PisteLog_Kirjoita("- Unload failed.\n");
					PostQuitMessage(0);
					window_closed = true;
				}
				else {
					ShowCursor(TRUE);
				}*/

			}

			if (LOWORD(wparam) == WA_INACTIVE) {
				PisteLog_Kirjoita("- Main window is set inactive.\n");
				ikkuna_aktiivinen = false;
				PostQuitMessage(0);
				window_closed = true;
			}
			
			if (LOWORD(wparam) == WA_ACTIVE || LOWORD(wparam) == WA_CLICKACTIVE) {
/*
				PisteLog_Kirjoita("- Activating game's main window.\n");

				if (!ikkuna_aktiivinen) {
					PisteLog_Kirjoita("  - Trying to recover.\n");
					
					if (PK_Reload() != 0) {
						PisteLog_Kirjoita("  - Recovery failed! \n");
						PostQuitMessage(0);
						window_closed = true;
					}
					else {
						ikkuna_aktiivinen = true;
						ShowCursor(FALSE);
					}
					/*
					if (PisteDraw_Palauta_Pinnat() == PD_VIRHE) {
						PisteLog_Kirjoita("  - Reallocation of surfaces failed.\n");
						PostQuitMessage(0);
						//PK2_virhe = true;
					}*/
			/*	} */
			}
		
			/*
			if (HIWORD(wparam) == 0) {
				
				PisteLog_Kirjoita("- Activating game's main window.\n");

				if (!ikkuna_aktiivinen) {
					PisteLog_Kirjoita("  - Trying to revieve surfaces.\n");
					if (PisteDraw_Palauta_Pinnat() == PD_VIRHE) {
						PisteLog_Kirjoita("  - Reallocation of surfaces failed.\n");
						PK2_virhe = true;
					}
				}
				
				ikkuna_aktiivinen = true;
			}
			else {
				PisteLog_Kirjoita("- ALT+TAB pressed!\n");
				ikkuna_aktiivinen = false;
				PostQuitMessage(0);
			}*/

					
			return(0);
		}	break;
	
		case WM_CREATE: 
	    {
		
			return(0);
		}	break;
   
		case WM_PAINT: 
		{

			hdc = BeginPaint(hwnd,&ps);	 

			EndPaint(hwnd,&ps);

			return(0);
		}	break;

		case WM_DESTROY: 
		{
			PisteLog_Kirjoita("- Main window receives exit message.\n");
			PostQuitMessage(0);
			return(0);
		}	break;

		default:break;

    } 

	return (DefWindowProc(hwnd, msg, wparam, lparam));

} 

LRESULT CALLBACK WindowProcSetup(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	PAINTSTRUCT		ps;		
	HDC				hdc;	

	switch(msg)
	{	
		case WM_CREATE: 
	    {
			return(0);
		}	break;
		case WM_COMMAND:
		{
			switch(LOWORD(wparam))
			{
				case ID_BT_Exit :
				{
					SendMessage(hSetupIkkuna, WM_CLOSE,0,0);
					SendMessage(hwnd, WM_CLOSE,0,0);
				}	break;

				case ID_BT_Music :
				{
					PisteLog_Kirjoita("    - Setting Midas config. \n");
					if ( !MIDASconfig() )
					{
						MIDASvirhe = true;
						asetukset.musiikki = false;
					}
					else
					{
						MIDASsaveConfig("data\\msetup.ini");
						asetukset.musiikki = true;
					}

				}   break;

				case ID_BT_Play :
				{
					alkusetup = false;
					
					if (SendMessage(hRB_1024x768, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
						RUUDUN_LEVEYS  = 1024;
						RUUDUN_KORKEUS = 768;
					}
					if (SendMessage(hRB_640x480, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
						RUUDUN_LEVEYS  = 640;
						RUUDUN_KORKEUS = 480;
					}
					if (SendMessage(hRB_800x600, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
						RUUDUN_LEVEYS  = 800;
						RUUDUN_KORKEUS = 600;
					}

					if (SendMessage(hCB_Music, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
						asetukset.musiikki = true;
					}
					else
						asetukset.musiikki = false;

					if (SendMessage(hCB_SFX, BM_GETSTATE, 0, 0) == BST_CHECKED)
					{
						asetukset.aanet = true;
					}
					else
						asetukset.aanet = false;
					
					// Haetaan valittu kieli
					char kielifile[_MAX_PATH];
					char hakemisto[_MAX_PATH];
					long valittu = SendMessage(hCMB_Languages, CB_GETCURSEL, 0, 0);
					SendMessage(hCMB_Languages, CB_GETLBTEXT, valittu, (LPARAM)(LPCSTR)kielifile);
					//tekstit->Lue_Tiedosto(kielifile);
					
					if (strcmp(kielifile,asetukset.kieli) != 0)	{
						strcpy(asetukset.kieli,kielifile);
						strcpy(hakemisto,"language\\");
						strcat(hakemisto,kielifile);
						PK_Lataa_Kieli(hakemisto);
					}

					//DestroyWindow(hSetupIkkuna);
					//SendMessage(hSetupIkkuna, WM_CLOSE,0,0);
				}	break;

				default : break;
			}
			
			return 0;
		}   
		case WM_PAINT: 
		{

			hdc = BeginPaint(hwnd,&ps);	 

			EndPaint(hwnd,&ps);

			return(0);
		}	break;

		case WM_DESTROY: 
		{

			PostQuitMessage(0);
			return(0);
		}	break;

/*
		case WM_CTLCOLORSTATIC: 
		{
			
			HDC  hdcButton  = (HDC) wparam;
			HWND hwndButton = (HWND) lparam;

			return((LRESULT)(HBRUSH)GetStockObject(LTGRAY_BRUSH));
			break;

		}

		case WM_CTLCOLORBTN: 
		{

			HDC  hdcButton  = (HDC) wparam;
			HWND hwndButton = (HWND) lparam;

			return((LRESULT)(HBRUSH)GetStockObject(LTGRAY_BRUSH));
			break;
		}
*/
		default:break;
    } 

	return (DefWindowProc(hwnd, msg, wparam, lparam));

} 

int PK_SetupIkkuna(HWND &hwnd, HINSTANCE &hinstance, int &ncmdshow)
{
	WNDCLASSEX wc;								// uusi ikkuna-luokka (struct)

	wc.cbSize		= sizeof(WNDCLASSEX);
	wc.style		= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	= WindowProcSetup;			
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hInstance	= hinstance;								
	//wc.hIcon		= LoadIcon(NULL, IDI_APPLICATION);				
	wc.hIcon		= LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	//wc.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm		= LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground= (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	//wc.hbrBackground= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;											
	wc.lpszClassName= "SetupWindow";								

	if (!RegisterClassEx(&wc))		// rekisteröidään ikkuna
		return 1;

	if (!(hSetupIkkuna = CreateWindowEx(WS_EX_LEFT,
		"SetupWindow", 
		_T("Pekka Kana 2 v1.21 Setup"),	
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		560,					// leveys ja
		440,					// korkeus 
		NULL,					// emo-ikkuna jos lapsi-ikkuna
		NULL,					// menu
		hinstance,				// ohjelman tunnus
		NULL					// ikkunan luomiseen dataa(?)
		)))
		return 1;

	/* STATIC TEKSTIT */

	CreateWindowEx(	NULL,
					"static",
					" ",
					WS_CHILD | WS_VISIBLE | SS_LEFT,
					//320,360,
					0,0,
					560,440,
					hSetupIkkuna,
					(HMENU)(10),
					hinstance,
					NULL);	

	CreateWindowEx(	NULL,
					"static",
					//"Setup options",
					tekstit->Hae_Teksti(txt_setup_options),
					WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOTIFY,
					10,12,
					100,24,
					hSetupIkkuna,
					(HMENU)(10),
					hinstance,
					NULL);

	CreateWindowEx(	NULL,
					"static",
					"www.pistegamez.net",
					WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOTIFY,
					10,390,
					200,24,
					hSetupIkkuna,
					(HMENU)(10),
					hinstance,
					NULL);

/*
	CreateWindowEx(	NULL,
					"static",
					"Pekka Kana 2 (C) 2002-2003 Piste Gamez \nhttp://pistegamez.cjb.net",
					WS_CHILD | WS_VISIBLE | SS_LEFT,
					300,275,
					300,60,
					hSetupIkkuna,
					(HMENU)(10),
					hinstance,
					NULL);
*/
	/* KUVA */

	CreateWindowEx(	NULL,
					"STATIC",
					"SETUPBMP",
					WS_CHILD | WS_VISIBLE | SS_BITMAP,
					295,0,
					266,400,
					hSetupIkkuna,
					(HMENU)(11),
					hinstance,
					NULL);

	CreateWindowEx(	NULL,
					"static",
					tekstit->Hae_Teksti(txt_setup_language),
					WS_CHILD | WS_VISIBLE | SS_LEFT,
					//320,360,
					295,345,
					266,90,
					hSetupIkkuna,
					(HMENU)(10),
					hinstance,
					NULL);


	/* GROUPBOXIT */

	CreateWindowEx(	NULL,
					"button",
					//"Video modes:",
					tekstit->Hae_Teksti(txt_setup_videomodes),
					WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
					10,40,
					250,150,
					hSetupIkkuna,
					(HMENU)(20),
					hinstance,
					NULL);

	CreateWindowEx(	NULL,
					"button",
					//"Music and sound effects:",
					tekstit->Hae_Teksti(txt_setup_music_and_sounds),
					WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
					10,200,
					250,100,
					hSetupIkkuna,
					(HMENU)(30),
					hinstance,
					NULL);

	/* RADIOBUTTONIT */
	
	hRB_640x480 = CreateWindowEx(NULL,
					"button",
					"640x480",
					WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
					20,65,
					100,24,
					hSetupIkkuna,
					(HMENU)(ID_RB),
					hinstance,
					NULL);

	hRB_800x600 = CreateWindowEx(NULL,
					"button",
					"800x600",
					WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
					20,90,
					100,24,
					hSetupIkkuna,
					(HMENU)(ID_RB),
					hinstance,
					NULL);

	hRB_1024x768 = CreateWindowEx(NULL,
					"button",
					"1024x768",
					WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
					20,115,
					100,24,
					hSetupIkkuna,
					(HMENU)(ID_RB),
					hinstance,
					NULL);

	/* CHECK BOXIT */

	hCB_Music = CreateWindowEx(NULL,
					"button",
					//"Music",
					tekstit->Hae_Teksti(txt_setup_music),
					WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
					20,225,
					100,24,
					hSetupIkkuna,
					(HMENU)(ID_CB_Music),
					hinstance,
					NULL);

	hCB_SFX = CreateWindowEx(NULL,
					"button",
					//"Sound Effects",
					tekstit->Hae_Teksti(txt_setup_sounds),
					WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
					20,255,
					120,24,
					hSetupIkkuna,
					(HMENU)(ID_CB_SFX),
					hinstance,
					NULL);

	/* COMBOBOXIT */

	hCMB_Languages = CreateWindowEx(NULL,
					"combobox",
					"Kielet",
					//tekstit->Hae_Teksti(txt_setup_languages),
					WS_CHILD | WS_VSCROLL | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT,
					300,370,
					250,224,
					hSetupIkkuna,
					(HMENU)(ID_CMB_Languages),
					hinstance,
					NULL);

	/* PAINONAPIT */

	hBT_Play = CreateWindowEx(	NULL,
					"button",
					"Music Setup...",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					130,225,
					120,24,
					hSetupIkkuna,
					(HMENU)(ID_BT_Music),
					hinstance,
					NULL);	

	hBT_Play = CreateWindowEx(	NULL,
					"button",
					//"Play",
					tekstit->Hae_Teksti(txt_setup_play),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					10,320,
					250,24,
					hSetupIkkuna,
					(HMENU)(ID_BT_Play),
					hinstance,
					NULL);	
	
	hBT_Exit = CreateWindowEx(	NULL,
					"button",
					//"Exit",
					tekstit->Hae_Teksti(txt_setup_exit),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					10,355,
					250,24,
					hSetupIkkuna,
					(HMENU)(ID_BT_Exit),
					hinstance,
					NULL);	

	switch(asetukset.ruudun_leveys)
	{
	case 800:	SendMessage(hRB_800x600,BM_SETCHECK, TRUE, 0);break;
	case 640:	SendMessage(hRB_640x480,BM_SETCHECK, TRUE, 0);break;
	case 1024:	SendMessage(hRB_1024x768,BM_SETCHECK, TRUE, 0);break;
	default:	SendMessage(hRB_800x600,BM_SETCHECK, TRUE, 0);break;
	}

	if (asetukset.musiikki)
		SendMessage(hCB_Music,BM_SETCHECK, TRUE, 0);
	else
		SendMessage(hCB_Music,BM_SETCHECK, FALSE, 0);

	if (asetukset.aanet)
		SendMessage(hCB_SFX,BM_SETCHECK, TRUE, 0);
	else
		SendMessage(hCB_SFX,BM_SETCHECK, FALSE, 0);

	PK_Kielitiedostot_Hae();
	/*
	SendMessage(hCMB_Languages,CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"suomi.txt");
	SendMessage(hCMB_Languages,CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"english.txt");
	*/

	if (!ShowWindow(hSetupIkkuna,ncmdshow))
		return 1;

	if (!UpdateWindow(hSetupIkkuna))
		return 1;
	
	return 0;
}


// Kaiken alku ja juuri: WinMain. Tästä se kaikki alkaa ja tämän sisällä peli pyörii.

int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{
	WNDCLASSEX winclass; 
	HWND	   hwnd;	 
	MSG		   msg;

	char *komennot = lpcmdline;

	if (strcmp(komennot, "log" ) == 0) {
		PisteLog_Salli_Kirjoitus();
	}

	PisteLog_Kirjoita("PK2 STARTUP\n");
	PisteLog_Kirjoita("========================\n");
	
	PisteLog_Kirjoita("- Command string: ");
	PisteLog_Kirjoita(komennot);
	PisteLog_Kirjoita("\n");
	
	if (strcmp(komennot, "developer" ) == 0) {
		PisteLog_Kirjoita("- Developer mode set. Cheats are on.\n");
		huijaukset = true;
	}

	if (strcmp(komennot, "nolimits" ) == 0)
	{
		PisteLog_Kirjoita("- No limits mode set. Game area is drawn full screen.\n");
		RAJAA_KARTANPIIRTOALUE = false;
	}

	/* ALKU SETUP */

	PK_Asetukset_Lataa("data\\settings.ini");

	tekstit = new PisteLanguage();

	char hakemisto[_MAX_PATH];

	strcpy(hakemisto,"language\\");
	strcat(hakemisto,asetukset.kieli);
	if (!PK_Lataa_Kieli(hakemisto))
	{
		MessageBox(ikkunan_kahva, "Could not find the default language file!","Pekka Kana 2",
				   MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	if (!PK_SetupIkkuna(hwnd, hinstance, ncmdshow))
		return 0;

	while (alkusetup)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
	   
			if (msg.message == WM_QUIT)
				break;
	   
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		} 
	}
	
	if (alkusetup)
		return(msg.wParam);

	/* PELIN IKKUNA */
	
	winclass.cbSize         = sizeof(WNDCLASSEX);
	winclass.style			= CS_DBLCLKS | CS_OWNDC | 
							  CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	winclass.hCursor		= LoadCursor(hinstance, MAKEINTRESOURCE(IDC_CURSOR1)); 
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= WINDOW_CLASS_NAME;
	winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);//IDI_APPLICATION


	hinstance_app = hinstance;

	if (!RegisterClassEx(&winclass))
		return(0);

	if (!(hwnd = CreateWindowEx(NULL,                  
		                        WINDOW_CLASS_NAME,     
							    GAME_NAME, 
							    WS_POPUP | WS_VISIBLE,
						 	    0,0,	  
							    RUUDUN_LEVEYS, RUUDUN_KORKEUS,  
							    NULL,	   
							    NULL,	  
							    hinstance,
							    NULL)))	
		return(0);

	ikkunan_kahva = hwnd;

	PK_Alusta_Tilat();
	//PK_Init();

	ShowCursor(FALSE);

	if (huijaukset)
		pelin_seuraava_tila = TILA_MENUT;
	else
		pelin_seuraava_tila = TILA_INTRO;

	while(!PK2_virhe)
	{
    
		PK_Alusta_Tilat();

		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
	   
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);

			DispatchMessage(&msg);
		} 
    
		if (ikkuna_aktiivinen)
			PK_Main();

	} 

	if (PK2_virhe)
		PisteLog_Kirjoita("- PK2 Error! Stopping.\n");

	PisteLog_Kirjoita("- Saving settings.\n");
	PK_Asetukset_Tallenna("data\\settings.ini");

	PK_Quit();

	PisteLog_Kirjoita("========================\n");
	PisteLog_Kirjoita("PK2 EXITS\n");
	PisteLog_Kirjoita("========================\n");

	ShowCursor(TRUE);

	return(msg.wParam);
} 















