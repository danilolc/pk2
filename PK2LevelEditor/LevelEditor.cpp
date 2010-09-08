/* PRE DEFINITIONS ----------------------------------------------------------------------------*/

#define WIN32_LEAN_AND_MEAN  
#define INITGUID 

/* INCLUDES -----------------------------------------------------------------------------------*/

#include <windows.h>   
#include <windowsx.h> 
#include <mmsystem.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <io.h>
#include <fstream.h>
#include <iostream.h>
#include <string.h>
#include "G:\Microsoft Visual Studio\MyProjects\PisteEngine\PisteInput.h"
#include "G:\Microsoft Visual Studio\MyProjects\PisteEngine\PisteDraw.h"
#include "G:\Microsoft Visual Studio\MyProjects\PisteEngine\PisteSound.h"
#include "G:\Microsoft Visual Studio\MyProjects\PisteEngine\PisteWait.h"
#include "G:\Microsoft Visual Studio\MyProjects\Pekka Kana 2\PK2Map.h"
#include "G:\Microsoft Visual Studio\MyProjects\Pekka Kana 2\PK2Sprite.h"
#include "Resource.h"

/* TYYPPIMÄÄRITTELYT ---------------------------------------------------------------------------*/

typedef unsigned short USHORT;
typedef unsigned short WORD; 
typedef unsigned long  DWORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;


struct TOOL_LIST
{
	bool		nakyva;
	int			x,y,
				valinta;
};

struct MENU
{
	char		otsikko[30];
	int			x;
	int			y;
	int			leveys;
	int			korkeus;
	bool		piilota;
	int			pika;
	TOOL_LIST	lista;
};

struct TIEDOSTO
{
	char		nimi[_MAX_PATH];
	UCHAR		tyyppi;
};

struct LEIKEPOYTA
{
	UCHAR	taustat[PK2KARTTA_KARTTA_KOKO];
	UCHAR	seinat[PK2KARTTA_KARTTA_KOKO];
	UCHAR	spritet[PK2KARTTA_KARTTA_KOKO];
	RECT	alue;
};

struct LOKIMERKINTA
{
	char	teksti[200];
	UCHAR	tyyppi;
};

/* MÄÄRITTELYT --------------------------------------------------------------------------------*/

#define WINDOW_CLASS_NAME "WINCLASS1"
#define	GAME_NAME		  "PK2 LEVEL EDITOR"

#define TAUSTAKUVA_EI		0
#define TAUSTAKUVA_METSA	1

#define MUSIIKKI_EI			0

const int RUUDUN_LEVEYS			= 1024;//1024;	
const int RUUDUN_KORKEUS		= 768;//768;	
const int SCREEN_BPP			= 8;	
const int MAX_COLORS_PALETTE	= 256;	

const UCHAR MENU_EI				= 100;
const UCHAR	MENU_KARTTA			= 0;
const UCHAR	MENU_PALIKAT		= 1;
const UCHAR	MENU_INFO			= 2;
const UCHAR	MENU_SPRITET		= 3;
const UCHAR	MENU_TIEDOSTOT		= 4;
const UCHAR	MENU_SAVE			= 5;
const UCHAR	MENU_HELP			= 6;
const UCHAR	MENU_EXIT			= 7;
const UCHAR	MENU_TOOLS			= 8;
const UCHAR	MENU_LOKI			= 9;

const UCHAR MAX_MENUJA			= 10;

const UCHAR EDIT_KARTTA			= 0;
const UCHAR EDIT_MENU			= 1;
const UCHAR EDIT_SPRITE			= 2;

const int MAX_DIR_TIEDOSTOJA	= 1000;

const int TIEDOSTO_DIR			= 1;
const int TIEDOSTO_MAP			= 2;
const int TIEDOSTO_BMP			= 3;
const int TIEDOSTO_SPR			= 4;
const int TIEDOSTO_MUS			= 5;

const int EDIT_KERROS_TAUSTA	= 0;
const int EDIT_KERROS_SEINAT	= 1;
const int EDIT_KERROS_VAIN_SEINAT = 2;

const int MAX_SPRITEJA			= 600;

const int KENTTA_FILE		= 1;
const int KENTTA_NIMI		= 2;
const int KENTTA_TEKIJA		= 3;
const int KENTTA_MUSIIKKI	= 4;

const int MAX_LOKIMERKINTOJA	= 55;
const UCHAR LOKI_INFO			= 1;
const UCHAR LOKI_VIRHE			= 2;

struct ASETUKSET
{
	char		tyohakemisto[_MAX_PATH];
	char		epd_hakemisto[_MAX_PATH];
	char		editor_hakemisto[_MAX_PATH];
	char		bg_hakemisto[_MAX_PATH];
	char		tile_hakemisto[_MAX_PATH];
	char		karttatiedosto[_MAX_PATH];
	MENU		menut[MAX_MENUJA];
	int			kartta_x;
	int			kartta_y;
};

/* GLOBAALIT MUUTTUJAT ---------------------------------------------------------------------------*/

HWND      ikkunan_kahva			= NULL; // pääikkunan kahva
HINSTANCE hinstance_app			= NULL; // hinstance?
HDC       global_dc				= NULL; // global dc?

bool DirectX_virhe				= false;// jos tämä muuttuu todeksi niin ohjelma lopetetaan
char virheviesti[60];		

bool window_closed				= false;// onko ikkuna kiinni

double cos_table[360];
double sin_table[360];
int degree = 0;

UCHAR edit_tila = EDIT_KARTTA;
UCHAR aktiivinen_menu = MENU_EI;

bool liikuta_ikkunaa = false;

bool editoi_tekstia = false;

char viesti[60];
char tyohakemisto[_MAX_PATH];
char pk2_hakemisto[_MAX_PATH] = " ";
char epd_hakemisto[_MAX_PATH] = " ";
char editor_hakemisto[_MAX_PATH] = "";
char bg_hakemisto[_MAX_PATH] = " ";
char tile_hakemisto[_MAX_PATH] = " ";
TIEDOSTO tiedostot[MAX_DIR_TIEDOSTOJA];

char karttatiedosto[_MAX_PATH];

DWORD tiedostoja = 0;

bool nayta_hakemistot = true;

PK2Kartta *kartta, *undo;
bool varmistettu = false;

RECT kartta_kohde = {0,0,0,0};
int	 kartta_kohde_status = 0;
RECT kartta_lahde = {0,0,0,0};

// KUVABUFFERIT
int  kuva_editori;
int  kuva_kartta;

// FONTIT
int fontti1;
int fontti2;

// KONTROLLIT
int hiiri_x = 10;
int hiiri_y = 10;
int key_delay = 0;

// MENUT
MENU menut[MAX_MENUJA];
int menu_palikat_nayta_y = 0;
int menu_tiedostot_nayta = 1;
int menu_tiedostot_eka = 0;
int menu_spritet_eka = 0;

// KARTTA
int kartta_x = 0;
int kartta_y = 0;
int edit_palikka = 0;
int edit_kerros = EDIT_KERROS_SEINAT;

int focustile_etu = 255;
int focustile_taka = 255;
int focussprite = 255;

bool kartta_ladattu = false;

int palikka_animaatio = 0;

bool animaatio_paalla = true;

LEIKEPOYTA leikepoyta;
bool aseta_leikepoyta_alue = false;
RECT leikepoyta_alue = {0,0,0,0};
TOOL_LIST leikepoyta_lista;

PK2Sprite_Prototyyppi protot[MAX_PROTOTYYPPEJA];
int seuraava_vapaa_proto = 0;
//int vakio_spriteja = 0;
//PK2Sprite spritet[MAX_SPRITEJA];
int spriteja = 0;

int proto_valittu = MAX_PROTOTYYPPEJA;

// TEKSTINKÄSITTELY
int editKenttaId = 0;
char editTeksti[500];
int editKursori = 0;
char editMerkki = ' ';


// LOKI

LOKIMERKINTA loki[MAX_LOKIMERKINTOJA];

bool kirjoita_loki = false;

UCHAR karttavarit[150];

// ASETUKSET

ASETUKSET asetukset;

/* PROTOT ---------------------------------------------------------------------------------------*/

int Level_Editor_Loki_Tallenna(char *viesti);
int Level_Editor_Piirra_Nelio(int vasen, int yla, int oikea, int ala, UCHAR color);
int Level_Editor_Kartta_Paivita(void);
void Level_Editor_Laske_TileVarit();
void Level_Editor_Aseta_Palikat(char *filename);
int Level_Editor_Kartta_Oletusasetukset();
int Level_Editor_Kartta_Lataa(char *filename);


/* EDITORI ---------------------------------------------------------------------------------------*/

bool exit_editor = false;
bool piirto_aloitettu = false;

/* ALUSTUKSET ---------------------------------------------------------------*/


void Level_Editor_Loki_Alusta()
{
	for (int i=0;i<MAX_LOKIMERKINTOJA;i++)
	{
		loki[i].tyyppi = 0;
		strcpy(loki[i].teksti," ");
	}
}

void Level_Editor_Loki_Kirjaa(char *teksti, UCHAR tyyppi)
{
	strcpy(viesti,teksti);
	
	for (int i=0;i<MAX_LOKIMERKINTOJA-1;i++)
	{
		strcpy(loki[i].teksti,loki[i+1].teksti);
		loki[i].tyyppi = loki[i+1].tyyppi;

	}
	loki[MAX_LOKIMERKINTOJA-1].tyyppi = tyyppi;
	strcpy(loki[MAX_LOKIMERKINTOJA-1].teksti,teksti);
}

int Level_Editor_Asetukset_Lataa(void)
{
	int i;

	char	tiedostonimi[_MAX_PATH];

	strcpy(tiedostonimi,editor_hakemisto);
	strcat(tiedostonimi,"//settings.dat");

	Level_Editor_Loki_Kirjaa("loading settings...", LOKI_INFO);

	ifstream *tiedosto = new ifstream(tiedostonimi, ios::in | ios::nocreate);

	if (tiedosto->fail())
	{
		delete (tiedosto);
		Level_Editor_Loki_Kirjaa("no settings saved.", LOKI_INFO);
		return 1;
	}
	
	tiedosto->read((char *)&asetukset,sizeof(asetukset));
		
	if (tiedosto->fail())
	{
		delete (tiedosto);
		strcpy(viesti,"could not read settings.dat");
		Level_Editor_Loki_Kirjaa("loading settings failed.", LOKI_VIRHE);
		return 1;
	}

	delete (tiedosto);

	strcpy(bg_hakemisto,asetukset.bg_hakemisto);
	strcpy(epd_hakemisto,asetukset.epd_hakemisto);
	strcpy(karttatiedosto,asetukset.karttatiedosto);
	strcpy(tile_hakemisto,asetukset.tile_hakemisto);
	strcpy(tyohakemisto,asetukset.tyohakemisto);

	for (i=0;i<MAX_MENUJA;i++) {
		menut[i].x = asetukset.menut[i].x;
		menut[i].y = asetukset.menut[i].y;
		menut[i].piilota = asetukset.menut[i].piilota;
	}	

	kartta_x = asetukset.kartta_x;
	kartta_y = asetukset.kartta_y;

	if (_chdir(tyohakemisto) == 0)
		_getcwd(tyohakemisto, _MAX_PATH );	

	Level_Editor_Kartta_Lataa(karttatiedosto);

	hiiri_x = 250;
	hiiri_y = 250;

	return 0;
}

void Level_Editor_Asetukset_Tallenna(void)
{
	int i;
	
	Level_Editor_Loki_Kirjaa("saving settings", LOKI_INFO);

	strcpy(asetukset.bg_hakemisto,bg_hakemisto);
	strcpy(asetukset.editor_hakemisto,bg_hakemisto);
	strcpy(asetukset.epd_hakemisto,epd_hakemisto);
	strcpy(asetukset.karttatiedosto,karttatiedosto);
	strcpy(asetukset.tile_hakemisto,tile_hakemisto);
	strcpy(asetukset.tyohakemisto,tyohakemisto);

	for (i=0;i<MAX_MENUJA;i++) {
		asetukset.menut[i].x = menut[i].x;
		asetukset.menut[i].y = menut[i].y;
		asetukset.menut[i].piilota = menut[i].piilota;
	}

	asetukset.menut[MENU_EXIT].piilota = true;

	asetukset.kartta_x = kartta_x;
	asetukset.kartta_y = kartta_y;

	char	tiedostonimi[_MAX_PATH];

	strcpy(tiedostonimi,editor_hakemisto);
	strcat(tiedostonimi,"//settings.dat");

	ofstream *tiedosto = new ofstream(tiedostonimi, ios::binary);
	tiedosto->write ((char *)&asetukset, sizeof (asetukset));
	
	delete (tiedosto);
}

// LEIKEPÖYTÄ ///////////////////////////////////////////////

void Level_Editor_Leikepoyta_Alusta(void)
{
	memset(leikepoyta.seinat, 0,sizeof(leikepoyta.seinat));
	memset(leikepoyta.taustat,0,sizeof(leikepoyta.taustat));
	memset(leikepoyta.spritet,0,sizeof(leikepoyta.spritet));

	leikepoyta.alue.left	= 0;
	leikepoyta.alue.right	= 0;
	leikepoyta.alue.top		= 0;
	leikepoyta.alue.bottom	= 0;
}

void Level_Editor_Leikepoyta_Kopioi(RECT alue)
{
	Level_Editor_Leikepoyta_Alusta();

	if (alue.right > PK2KARTTA_KARTTA_LEVEYS)
		alue.right = PK2KARTTA_KARTTA_LEVEYS;
	
	if (alue.left < 0)
		alue.left = 0;

	if (alue.bottom > PK2KARTTA_KARTTA_KORKEUS)
		alue.bottom = PK2KARTTA_KARTTA_KORKEUS;

	if (alue.top < 0)
		alue.top = 0;	

	leikepoyta.alue.left = 0;
	leikepoyta.alue.top = 0;
	leikepoyta.alue.right = alue.right - alue.left;
	leikepoyta.alue.bottom = alue.bottom - alue.top;

	for (int x=alue.left;x<alue.right;x++)
		for (int y=alue.top;y<alue.bottom;y++)
		{
			leikepoyta.taustat[x-alue.left+(y-alue.top)*PK2KARTTA_KARTTA_LEVEYS] = kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS];
			leikepoyta.seinat[ x-alue.left+(y-alue.top)*PK2KARTTA_KARTTA_LEVEYS] = kartta->seinat[ x+y*PK2KARTTA_KARTTA_LEVEYS];
			leikepoyta.spritet[x-alue.left+(y-alue.top)*PK2KARTTA_KARTTA_LEVEYS] = kartta->spritet[x+y*PK2KARTTA_KARTTA_LEVEYS];
		}
	
	Level_Editor_Kartta_Paivita();
}

void Level_Editor_Leikepoyta_Liita(RECT alue)
{
	//alue.left = x;
	//alue.top  = y;
	//alue.right = x + leikepoyta.alue.right;
	//alue.bottom = y + leikepoyta.alue.bottom;
	//Level_Editor_Leikepoyta_Alusta();

	if (alue.right > PK2KARTTA_KARTTA_LEVEYS)
		alue.right = PK2KARTTA_KARTTA_LEVEYS;
	
	if (alue.left < 0)
		alue.left = 0;

	if (alue.bottom > PK2KARTTA_KARTTA_KORKEUS)
		alue.bottom = PK2KARTTA_KARTTA_KORKEUS;

	if (alue.top < 0)
		alue.top = 0;

	int lp_leveys  = leikepoyta.alue.right - leikepoyta.alue.left,
		lp_korkeus = leikepoyta.alue.bottom - leikepoyta.alue.top;

	if (lp_leveys > 0 && lp_korkeus > 0)
	{
		for (int x=alue.left;x<alue.right;x++)
			for (int y=alue.top;y<alue.bottom;y++)
			{
				//kartta->spritet[x+y*PK2KARTTA_KARTTA_LEVEYS] = leikepoyta.spritet[(x-alue.left)%lp_leveys +((y-alue.top)%lp_korkeus)*PK2KARTTA_KARTTA_LEVEYS];
				if (edit_kerros == EDIT_KERROS_TAUSTA || edit_kerros == EDIT_KERROS_SEINAT)
					kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS] = leikepoyta.taustat[(x-alue.left)%lp_leveys +((y-alue.top)%lp_korkeus)*PK2KARTTA_KARTTA_LEVEYS];
				if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT)
					kartta->seinat[ x+y*PK2KARTTA_KARTTA_LEVEYS] = leikepoyta.seinat[ (x-alue.left)%lp_leveys +((y-alue.top)%lp_korkeus)*PK2KARTTA_KARTTA_LEVEYS];
			}	
	}

	Level_Editor_Kartta_Paivita();
}

void Level_Editor_Leikepoyta_Liita_Koko(RECT alue)
{

	alue.right  = alue.left + leikepoyta.alue.right  - leikepoyta.alue.left;
	alue.bottom = alue.top  + leikepoyta.alue.bottom - leikepoyta.alue.top;

	if (alue.right > PK2KARTTA_KARTTA_LEVEYS)
		alue.right = PK2KARTTA_KARTTA_LEVEYS;
	
	if (alue.left < 0)
		alue.left = 0;

	if (alue.bottom > PK2KARTTA_KARTTA_KORKEUS)
		alue.bottom = PK2KARTTA_KARTTA_KORKEUS;

	if (alue.top < 0)
		alue.top = 0;

	int lp_leveys  = leikepoyta.alue.right - leikepoyta.alue.left,
		lp_korkeus = leikepoyta.alue.bottom - leikepoyta.alue.top;

	if (lp_leveys > 0 && lp_korkeus > 0)
	{
		for (int x=alue.left;x<alue.right;x++)
			for (int y=alue.top;y<alue.bottom;y++)
			{
				//kartta->spritet[x+y*PK2KARTTA_KARTTA_LEVEYS]     = leikepoyta.spritet[(x-alue.left) +(y-alue.top)*PK2KARTTA_KARTTA_LEVEYS];
				if (edit_kerros == EDIT_KERROS_TAUSTA || edit_kerros == EDIT_KERROS_SEINAT)
					kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS] = leikepoyta.taustat[(x-alue.left) +(y-alue.top)*PK2KARTTA_KARTTA_LEVEYS];
				if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT)
					kartta->seinat[ x+y*PK2KARTTA_KARTTA_LEVEYS] = leikepoyta.seinat[ (x-alue.left) +(y-alue.top)*PK2KARTTA_KARTTA_LEVEYS];
			}	
	}

	Level_Editor_Kartta_Paivita();
}

void Level_Editor_Leikepoyta_Leikkaa(RECT alue)
{
	Level_Editor_Leikepoyta_Alusta();

	if (alue.right > PK2KARTTA_KARTTA_LEVEYS)
		alue.right = PK2KARTTA_KARTTA_LEVEYS;
	
	if (alue.left < 0)
		alue.left = 0;

	if (alue.bottom > PK2KARTTA_KARTTA_KORKEUS)
		alue.bottom = PK2KARTTA_KARTTA_KORKEUS;

	if (alue.top < 0)
		alue.top = 0;	

	leikepoyta.alue.left = 0;
	leikepoyta.alue.top = 0;
	leikepoyta.alue.right = alue.right - alue.left;
	leikepoyta.alue.bottom = alue.bottom - alue.top;

	for (int x=alue.left;x<alue.right;x++)
		for (int y=alue.top;y<alue.bottom;y++)
		{
			if (edit_kerros == EDIT_KERROS_TAUSTA || edit_kerros == EDIT_KERROS_SEINAT)
			{
				leikepoyta.taustat[x-alue.left+(y-alue.top)*PK2KARTTA_KARTTA_LEVEYS] = kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS];
				kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS] = 255;
			}
			if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT)
			{
				leikepoyta.seinat[ x-alue.left+(y-alue.top)*PK2KARTTA_KARTTA_LEVEYS] = kartta->seinat[ x+y*PK2KARTTA_KARTTA_LEVEYS];
				kartta->seinat[ x+y*PK2KARTTA_KARTTA_LEVEYS] = 255;
			}
			
			//leikepoyta.spritet[x-alue.left+(y-alue.top)*PK2KARTTA_KARTTA_LEVEYS] = kartta->spritet[x+y*PK2KARTTA_KARTTA_LEVEYS];
			//kartta->spritet[x+y*PK2KARTTA_KARTTA_LEVEYS] = 255;
		}

	Level_Editor_Kartta_Paivita();
}

void Level_Editor_Leikepoyta_Piirra()
{
	//Level_Editor_Piirra_Nelio(leikepoyta.alue.left *32-kartta_x,leikepoyta.alue.top   *32-kartta_y,
	//						  leikepoyta.alue.right*32-kartta_x,leikepoyta.alue.bottom*32-kartta_y,28);
	
//	Level_Editor_Piirra_Nelio((leikepoyta_alue.left-kartta_x) *32,(leikepoyta_alue.top-kartta_y)  *32,
//							  (leikepoyta_alue.right-kartta_x)*32,(leikepoyta_alue.bottom-kartta_y)*32,224);

	Level_Editor_Piirra_Nelio((leikepoyta_alue.left-kartta_x) *32-1,(leikepoyta_alue.top-kartta_y)  *32-1,
							  (leikepoyta_alue.right-kartta_x)*32+1,(leikepoyta_alue.bottom-kartta_y)*32+1,0);

	Level_Editor_Piirra_Nelio((leikepoyta_alue.left-kartta_x) *32,(leikepoyta_alue.top-kartta_y)  *32,
							  (leikepoyta_alue.right-kartta_x)*32,(leikepoyta_alue.bottom-kartta_y)*32,31);

	Level_Editor_Piirra_Nelio((leikepoyta_alue.left-kartta_x) *32+1,(leikepoyta_alue.top-kartta_y)  *32+1,
							  (leikepoyta_alue.right-kartta_x)*32-1,(leikepoyta_alue.bottom-kartta_y)*32-1,0);
}


/*
void Level_Editor_Prototyyppi_Poista(int proto)
{
	protot[proto].Uusi();
	strcpy(kartta->protot[proto],"");

	for (DWORD x=0; x<PK2KARTTA_KARTTA_KOKO; x++)
	{
		if (kartta->spritet[x] == proto)
			kartta->spritet[x] = 255;
	}
}
*/

int Level_Editor_Aseta_Episodihakemisto()
{
	_getcwd(epd_hakemisto, _MAX_PATH );
	//strcpy(epd_hakemisto, "1234");
	//strcpy(viesti, "1234");

	return 0;
}

int Level_Editor_Lataa_PK2_Hakemisto(void)
{
	Level_Editor_Loki_Kirjaa("loading pk2le_dir.ini", LOKI_INFO);
	
	char hakemisto[_MAX_PATH];
	ifstream *tiedosto = new ifstream("pk2le_dir.ini", ios::in | ios::nocreate);
	
	if (tiedosto->fail())
	{
		delete (tiedosto);
		Level_Editor_Loki_Kirjaa("no pk2le_dir.ini found.", LOKI_VIRHE);
		return 1;
	}
	
	tiedosto->read(hakemisto, 255);
		
	if (tiedosto->fail())
	{
		delete (tiedosto);
		strcpy(viesti,"could not read pk2_dir.ini");
		Level_Editor_Loki_Kirjaa("loading pk2le_dir.ini failed.", LOKI_VIRHE);
		return 1;
	}

	delete (tiedosto);
	
	strcpy(pk2_hakemisto, hakemisto);

	strcpy(PK2Kartta::pk2_hakemisto, hakemisto);
	strcat(PK2Kartta::pk2_hakemisto, "\\");

	strcpy(bg_hakemisto, PK2Kartta::pk2_hakemisto);
	strcat(bg_hakemisto, "\\gfx\\scenery\\");

	strcpy(tile_hakemisto, PK2Kartta::pk2_hakemisto);
	strcat(tile_hakemisto, "\\gfx\\tiles\\");

	if (_chdir(pk2_hakemisto) == 0)
		_getcwd(tyohakemisto, _MAX_PATH );

	return 0;
}

int Level_Editor_Tallenna_PK2_Hakemisto(char *hakemisto)
{
	char tal[_MAX_DIR] = "";
	strcpy(tal,editor_hakemisto);
	strcat(tal,"//pk2le_dir.ini");
	Level_Editor_Loki_Kirjaa("saving pk2le_dir.ini.", LOKI_INFO);

	if (strcmp(pk2_hakemisto," ")==0)
	{
		ofstream *tiedosto = new ofstream(tal, ios::out);
		tiedosto->write(hakemisto, 255);
		
		tiedosto->close();

		if (tiedosto->fail())
		{
			delete (tiedosto);
			Level_Editor_Loki_Kirjaa("failed to save pk2_dir.ini.", LOKI_VIRHE);
			return 1;
		}

		delete (tiedosto);		
		strcpy(pk2_hakemisto, hakemisto);

		strcpy(PK2Kartta::pk2_hakemisto, hakemisto);
		strcat(PK2Kartta::pk2_hakemisto, "\\");

		strcpy(bg_hakemisto, PK2Kartta::pk2_hakemisto);
		strcat(bg_hakemisto, "\\gfx\\scenery\\");

		strcpy(tile_hakemisto, PK2Kartta::pk2_hakemisto);
		strcat(tile_hakemisto, "\\gfx\\tiles\\");

		if (_chdir(pk2_hakemisto) == 0)
			_getcwd(tyohakemisto, _MAX_PATH );		
		
		//Level_Editor_Lataa_PK2_Hakemisto();
		strcpy(viesti,"pk2 directory found");

		Level_Editor_Kartta_Oletusasetukset();
		menut[MENU_PALIKAT].piilota = false;
		menut[MENU_SPRITET].piilota = false;
	}

	return 0;
}

void Level_Editor_Laske_Spritet(void)
{
	int proto;

	spriteja = 0;

	for (int x=0;x<PK2KARTTA_KARTTA_KOKO;x++)
	{
		proto = kartta->spritet[x];
		if (proto != 255)
			spriteja++;
	}
	
	Level_Editor_Loki_Tallenna("Spritejen maara laskettu. \n");
}

void Level_Editor_Prototyyppi_Poista(int proto)
{

	if (strcmp(protot[proto].nimi,"")!=0)
	{
		Level_Editor_Loki_Tallenna("Poistetaan proto ");
		Level_Editor_Loki_Tallenna(protot[proto].nimi);
		Level_Editor_Loki_Tallenna("\n");
	}
	
	protot[proto].Uusi();
	strcpy(kartta->protot[proto],"");

	if (seuraava_vapaa_proto > 0)
		seuraava_vapaa_proto--;

	if (kartta->pelaaja_sprite > proto)
		kartta->pelaaja_sprite--;

	if (proto < MAX_PROTOTYYPPEJA-1)
	{
		for (int i=proto;i<MAX_PROTOTYYPPEJA;i++)
		{
			strcpy(kartta->protot[i],kartta->protot[i+1]);
			protot[i].Kopioi(protot[i+1]);
		}

		strcpy(kartta->protot[MAX_PROTOTYYPPEJA-1],"");
		protot[MAX_PROTOTYYPPEJA-1].Uusi();

		for (DWORD x=0; x<PK2KARTTA_KARTTA_KOKO; x++)
		{
			if (kartta->spritet[x] == proto)
				kartta->spritet[x] = 255;

			if (kartta->spritet[x] > proto && kartta->spritet[x] != 255)
				kartta->spritet[x]--;
		}
	}

	int i = MAX_PROTOTYYPPEJA-1;
	while (strcmp(protot[i].nimi,"")==0 && i >= 0)
		i--;

	if (i!=-1)
		seuraava_vapaa_proto = i+1;
}

void Level_Editor_Prototyyppi_Poista_Tyhjat()
{
	Level_Editor_Loki_Kirjaa("deleting empty sprites.", LOKI_INFO);
	for (int i=0; i<MAX_PROTOTYYPPEJA; i++)
	{
		if (strcmp(protot[i].nimi,"")==0)
			Level_Editor_Prototyyppi_Poista(i);
	}
}

void Level_Editor_Prototyyppi_Tyhjenna()
{
	Level_Editor_Loki_Kirjaa("deleting all sprites.", LOKI_INFO);
	//Level_Editor_Loki_Tallenna("Tyhjennetaan kaikki prototyypit.\n");

	for (int i=0; i<MAX_PROTOTYYPPEJA; i++)
	{
		Level_Editor_Loki_Tallenna("Poistetaan proto ");
		Level_Editor_Loki_Tallenna(protot[i].nimi);
		Level_Editor_Loki_Tallenna("\n");
		//Level_Editor_Prototyyppi_Poista(i);
		protot[i].Uusi();
		strcpy(kartta->protot[i],"");
	}

	seuraava_vapaa_proto = 0;
}

int Level_Editor_Prototyyppi_Laske_Lkm(int proto) 
{
	int lkm = 0;

	for (DWORD x=0; x<PK2KARTTA_KARTTA_KOKO; x++) {
		
		if (kartta->spritet[x] == proto)
			lkm++;
	}

	return lkm;
}

void Level_Editor_Prototyyppi_Poista_Turhat()
{
	int i = 0;

	Level_Editor_Loki_Kirjaa("deleting unused sprites:", LOKI_INFO);
	//Level_Editor_Loki_Tallenna("Tyhjennetaan kaikki prototyypit.\n");

	//for (int i=0; i<MAX_PROTOTYYPPEJA; i++)	{
	while(i < MAX_PROTOTYYPPEJA) {
		if (strcmp(protot[i].nimi,"")!=0) {
			if (Level_Editor_Prototyyppi_Laske_Lkm(i) == 0 && i != kartta->pelaaja_sprite) {
				Level_Editor_Loki_Tallenna("Poistetaan proto ");
				Level_Editor_Loki_Tallenna(protot[i].nimi);
				Level_Editor_Loki_Tallenna("\n");
				Level_Editor_Loki_Kirjaa(protot[i].nimi, LOKI_INFO);
				Level_Editor_Prototyyppi_Poista(i);
			}
			else
				i++;
		}
		else
			i++;
	}

	seuraava_vapaa_proto = 0;
}
/*
int PK_Prototyyppi_Lataa_VakioSprite(char *polku, char *tiedosto)
{
	if (seuraava_vapaa_proto < MAX_PROTOTYYPPEJA)
	{
		if (protot[seuraava_vapaa_proto].Lataa(polku, tiedosto) == 1)
			return 1;
	}
	else
		return 1;

	seuraava_vapaa_proto++;
	vakio_spriteja++;

	return 0;
}
*/
// Ladataan kartan mukana tulevat spritet
int Level_Editor_Prototyyppi_Lataa_Vanha(char *polku, char *tiedosto)
{
	char msg[200];
	strcpy(msg,"loading sprite: ");
	strcat(msg,tiedosto);
	Level_Editor_Loki_Kirjaa(msg, LOKI_INFO);
		
	//Level_Editor_Loki_Kirjaa("loading sprite.", LOKI_INFO);
	Level_Editor_Loki_Tallenna("Ladataan vanha prototyyppi ");
	Level_Editor_Loki_Tallenna(tiedosto);	
	Level_Editor_Loki_Tallenna("\n");	
	
	char tiedostopolku[255];

	strcpy(tiedostopolku,polku);
	strcat(tiedostopolku,"\\");	

	
	if (seuraava_vapaa_proto < MAX_PROTOTYYPPEJA)
	{
		if (protot[seuraava_vapaa_proto].Lataa(tiedostopolku, tiedosto) == 1)
		{
			strcpy(viesti,"could not load sprite ");
			strcat(viesti,tiedosto);
			Level_Editor_Loki_Kirjaa(viesti, LOKI_VIRHE);

			protot[seuraava_vapaa_proto].Uusi();
			strcpy(protot[seuraava_vapaa_proto].nimi,"loading error!");
			strcpy(protot[seuraava_vapaa_proto].tiedosto,tiedosto);
			protot[seuraava_vapaa_proto].korkeus = 10;
			protot[seuraava_vapaa_proto].leveys  = 10;
			protot[seuraava_vapaa_proto].kuva_frame_leveys = 10;
			protot[seuraava_vapaa_proto].kuva_frame_korkeus = 10;

			seuraava_vapaa_proto++;

			return 1;
		}
	}
	else
		return 1;

	seuraava_vapaa_proto++;

	return 0;
}
/*
// Tarvitaan jotta myös yhteysspritet tulisi lisättyä kertan tietoihin
int Level_Editor_Prototyyppi_Lataa_Yhteyssprite(char *polku, char *tiedosto)
{
	
	char tiedostopolku[255];

	strcpy(tiedostopolku,polku);
	strcat(tiedostopolku,"\\");
	
	if (seuraava_vapaa_proto < MAX_PROTOTYYPPEJA)
	{
		if (protot[seuraava_vapaa_proto].Lataa(tiedostopolku, tiedosto) == 1)
		{
			strcpy(viesti,"could not load ");
			strcat(viesti,tiedostopolku);
			strcat(viesti,tiedosto);
			return 1;
		}
		
		strcpy(kartta->protot[seuraava_vapaa_proto],tiedosto);
	}
	else
		return 2;

	seuraava_vapaa_proto++;

	return 0;
}

void Level_Editor_Prototyyppi_Aseta_Muutos_Sprite(int i, char *tiedostopolku)
{
	int j = 0;
	bool loytyi = false;
	
	if (strcmp(protot[i].muutos_sprite,"")!=0)
	{
		while (j<MAX_PROTOTYYPPEJA && !loytyi)
		{
			if (j!=i)
			{
				if (strcmp(protot[i].muutos_sprite,protot[j].tiedosto)==0)
				{
					protot[i].muutos = j;
					loytyi = true;
				}
			}

			j++;
		}
		
		if (!loytyi)
		{
			char polku[255];
			strcpy(polku,tiedostopolku);// sprites//

			if (Level_Editor_Prototyyppi_Lataa_Yhteyssprite(polku, protot[i].muutos_sprite)==0)
				protot[i].muutos = seuraava_vapaa_proto-1;
		}
	}
}

void Level_Editor_Prototyyppi_Aseta_Bonus_Sprite(int i, char *tiedostopolku)
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
			char polku[255];
			strcpy(polku,tiedostopolku);

			if (Level_Editor_Prototyyppi_Lataa_Yhteyssprite(polku, protot[i].bonus_sprite)==0)
				protot[i].bonus = seuraava_vapaa_proto-1;
		}
	}
}

void Level_Editor_Prototyyppi_Aseta_Ammus1_Sprite(int i, char *tiedostopolku)
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
			char polku[255];
			strcpy(polku,tiedostopolku);

			if (Level_Editor_Prototyyppi_Lataa_Yhteyssprite(polku, protot[i].ammus1_sprite)==0)
				protot[i].ammus1 = seuraava_vapaa_proto-1;
		}
	}
}

void Level_Editor_Prototyyppi_Aseta_Ammus2_Sprite(int i, char *tiedostopolku)
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
			char polku[255];
			strcpy(polku,tiedostopolku);

			if (Level_Editor_Prototyyppi_Lataa_Yhteyssprite(polku, protot[i].ammus2_sprite)==0)
				protot[i].ammus2 = seuraava_vapaa_proto-1;
		}
	}
}
*/
/*
int Level_Editor_Prototyyppi_Lataa_Uusi(char *polku, char *tiedosto)
{
	
	char tiedostopolku[255];

	strcpy(tiedostopolku,polku);
	strcat(tiedostopolku,"\\");

	
	if (seuraava_vapaa_proto < MAX_PROTOTYYPPEJA)
	{
		if (protot[seuraava_vapaa_proto].Lataa(tiedostopolku, tiedosto) == 1)
		{
			strcpy(viesti,"could not load ");
			strcat(viesti,tiedostopolku);
			strcat(viesti,tiedosto);
			return 1;
		}
		
		strcpy(kartta->protot[seuraava_vapaa_proto],tiedosto);
		strcpy(viesti,"loaded sprite ");
		strcat(viesti,kartta->protot[seuraava_vapaa_proto]);
	}
	else
	{
		strcpy(viesti,"could not load any more sprites");
		return 2;
	}

	strcpy(tiedostopolku,polku);
	strcat(tiedostopolku,"\\");

	seuraava_vapaa_proto++;
	
	return 0;
}*/

int Level_Editor_Prototyyppi_Lataa_Uusi(char *polku, char *tiedosto)
{
	char msg[200];
	strcpy(msg,"loading new sprite: ");
	//strcat(msg,polku);
	//strcat(msg,"\\");
	strcat(msg,tiedosto);
	Level_Editor_Loki_Kirjaa(msg, LOKI_INFO);
	
	//Level_Editor_Loki_Kirjaa("loading new sprite.", LOKI_INFO);
	Level_Editor_Loki_Tallenna("Ladataan uusi prototyyppi ");
	Level_Editor_Loki_Tallenna(tiedosto);	
	Level_Editor_Loki_Tallenna("\n");

	char tiedostopolku[_MAX_PATH];

	strcpy(tiedostopolku,polku);
	strcat(tiedostopolku,"\\");

	
	if (seuraava_vapaa_proto < MAX_PROTOTYYPPEJA)
	{
		if (protot[seuraava_vapaa_proto].Lataa(tiedostopolku, tiedosto) == 1)
		{
			Level_Editor_Loki_Kirjaa("loading sprite failed.", LOKI_VIRHE);
			//strcpy(viesti,"could not load ");
			//strcat(viesti,tiedostopolku);
			//strcat(viesti,tiedosto);
			return 1;
		}
		
		strcpy(kartta->protot[seuraava_vapaa_proto],tiedosto);
		strcpy(viesti,"loaded sprite ");
		strcat(viesti,kartta->protot[seuraava_vapaa_proto]);
	}
	else
	{
		Level_Editor_Loki_Kirjaa("too many sprites to load.", LOKI_VIRHE);
		strcpy(viesti,"could not load any more sprites");
		return 2;
	}

	strcpy(tiedostopolku,polku);
	strcat(tiedostopolku,"\\");

	seuraava_vapaa_proto++;
	
	return 0;
}

int Level_Editor_Prototyyppi_Lataa_Kaikki()
{
	char tiedosto[_MAX_PATH];
	int spriteja = 0;
	int viimeinen_proto = 0;

	Level_Editor_Loki_Kirjaa("loading all sprites.", LOKI_INFO);
	//strcpy(viesti,"map and all sprites loaded");

	for (int i=0;i < MAX_PROTOTYYPPEJA;i++) {

		if (strcmp(kartta->protot[i],"") != 0) {
			viimeinen_proto = i;
			strcpy(tiedosto,pk2_hakemisto);
			strcat(tiedosto,"\\sprites\\");
			//strcat(tiedosto,kartta->protot[i]);
			spriteja++;

			if (Level_Editor_Prototyyppi_Lataa_Vanha(tiedosto,kartta->protot[i])==1) {
				Level_Editor_Loki_Tallenna("lataus epäonnistui.\n");
				
				//seuraava_vapaa_proto++;

				//Level_Editor_Prototyyppi_Poista(i);
				//strcpy(kartta->protot[i],"");
				//seuraava_vapaa_proto++;
			}
			else
				Level_Editor_Loki_Tallenna("lataus onnistui.\n");
		}
		else
			seuraava_vapaa_proto++;

	}

	seuraava_vapaa_proto = viimeinen_proto+1;

	strcpy(tiedosto,"sprites\\");
/*
	for (i=0;i<MAX_PROTOTYYPPEJA;i++)
	{
		Level_Editor_Prototyyppi_Aseta_Muutos_Sprite(i, tiedosto);
		Level_Editor_Prototyyppi_Aseta_Bonus_Sprite( i, tiedosto);
		Level_Editor_Prototyyppi_Aseta_Ammus1_Sprite(i, tiedosto);
		Level_Editor_Prototyyppi_Aseta_Ammus2_Sprite(i, tiedosto);
	}*/

	if (spriteja == 0)
		strcpy(viesti,"map loaded without sprites");

	Level_Editor_Prototyyppi_Poista_Tyhjat();

	Level_Editor_Loki_Tallenna("Tyhjat prototyypit on poistettu");

	return 0;
}

void Level_Editor_Kartta_Varmista(void)
{
	//Level_Editor_Loki_Kirjaa("making backup of map (undo)", LOKI_INFO);
	undo->Kopioi(*kartta);
	varmistettu = true;
}

void Level_Editor_Kartta_Undo(void)
{
	if (varmistettu)
	{
		Level_Editor_Loki_Kirjaa("used undo", LOKI_INFO);
		strcpy(viesti,"used undo");
		kartta->Kopioi(*undo);
	}
	else
		strcpy(viesti,"nothing to undo");
}

int Level_Editor_Kartta_Paivita(void)
{
	UCHAR *buffer = NULL;
	DWORD tod_leveys;
	UCHAR vari_taka, vari_etu, vari, vari_sprite;

	PisteDraw_Piirto_Aloita(kuva_kartta, *&buffer, (DWORD &)tod_leveys);

	for (int x=0; x<PK2KARTTA_KARTTA_LEVEYS; x++)
	{
		for (int y=0; y<PK2KARTTA_KARTTA_KORKEUS; y++)
		{
			vari_taka = kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS];
			vari_etu  = kartta->seinat [x+y*PK2KARTTA_KARTTA_LEVEYS];
			vari_sprite = kartta->spritet[x+y*PK2KARTTA_KARTTA_LEVEYS];
			
			vari = 0;
/*
			if (vari_taka != 255)
				vari = 12+128;//96;

			if (vari_taka >= 130 && vari_taka <= 139)
				vari = 12+32;			

			if (vari_etu != 255)
				vari = 24+128;//96;

			if (vari_etu != 255)
				vari = 24+96;			

			if (vari_etu >= 130 && vari_etu <= 139)
				vari = 20+32;*/
			
			if (vari_taka < 150)
				vari = karttavarit[vari_taka];

			if (vari_etu < 150 && vari_etu != 255 && vari_etu != PALIKKA_ESTO_ALAS)
				vari = karttavarit[vari_etu];
			
			if (vari_sprite != 255)
				vari = 231;
			
			buffer[x+y*tod_leveys] = vari;
		}
	}

	PisteDraw_Piirto_Lopeta(kuva_kartta);

	Level_Editor_Laske_Spritet();

	return 0;
}

int Level_Editor_Kartta_Oletusasetukset()
{
	char tiedosto[_MAX_PATH];

	Level_Editor_Loki_Kirjaa("loading map default settings.", LOKI_INFO);

	if (strcmp(tile_hakemisto," ")!=0){
		//memset(tiedosto,'\0',sizeof(tiedosto));
		//strcpy(tiedosto,tile_hakemisto);
		//strcat(tiedosto,"tiles01.bmp");
		strcpy(tiedosto,"tiles01.bmp");
		Level_Editor_Aseta_Palikat(tiedosto);
		
		//PisteDraw_Lataa_Kuva(kartta->palikat_buffer,tiedosto,false);
	}

	if (strcmp(pk2_hakemisto," ")!=0){
		//memset(tiedosto,'\0',sizeof(tiedosto));
		strcpy(tiedosto,pk2_hakemisto);
		strcat(tiedosto,"\\sprites");
		Level_Editor_Prototyyppi_Lataa_Uusi(tiedosto, "rooster.spr");
		
	}

	strcpy(kartta->musiikki,"song01.xm");
	strcpy(karttatiedosto,"untitled.map");

	return 0;
}

int Level_Editor_Kartta_Alusta(void)
{
	Level_Editor_Loki_Kirjaa("clearing map.", LOKI_INFO);
	
	kartta->Tyhjenna();
	
	PisteDraw_Buffer_Tayta(kartta->taustakuva_buffer,37);

	Level_Editor_Prototyyppi_Tyhjenna();
	
	Level_Editor_Kartta_Oletusasetukset();

	Level_Editor_Kartta_Paivita();
	
	Level_Editor_Loki_Kirjaa("map cleared.", LOKI_INFO);

	return 0;
}

int Level_Editor_Loki_Tallenna(char *viesti)
{
	if (kirjoita_loki)
	{
		int virhe = 0;

		char *filename = "loki.txt";
			
		FILE *tiedosto;

		if ((tiedosto = fopen(filename, "a")) == NULL)
		{
			return(1);
		}

		char mjono[255];

		//memset(mjono,' ',sizeof(mjono));
		//mjono[60] = '\n';

		strcpy(mjono,viesti);

		fwrite(mjono,		sizeof(CHAR),	strlen(mjono),  tiedosto);

		fclose(tiedosto);
	}

	return(0);
}

int Level_Editor_Kartta_Tallenna(void)
{
	Level_Editor_Loki_Kirjaa("trying to save map...", LOKI_INFO);

	int virhe;

	char filename[_MAX_PATH]; 
	
	//strcpy(filename,kartta->nimi);
	strcpy(filename, karttatiedosto);

	if (strstr(filename,".map") == NULL)
		strcat(filename,".map");

	if ((virhe = kartta->Tallenna(filename)) != 0)
	{
		if (virhe == 1)
		{
			Level_Editor_Loki_Kirjaa("could not save map!", LOKI_VIRHE);
			//strcpy(viesti,"could not save map!");
			return(1);
		}
		if (virhe == 2)
		{
			strcpy(viesti,"error saving map!");
			Level_Editor_Loki_Kirjaa("error occured while saving map!", LOKI_VIRHE);
			return(1);
		}
	}
	
	Level_Editor_Loki_Kirjaa("map saved succesfully!", LOKI_INFO);
	strcpy(viesti,"map saved.");
	
	return(0);
}

int Level_Editor_Kartta_Lataa(char *filename)
{
	int virhe;

	char msg[200];
	strcpy(msg,"loading map: ");
	strcat(msg,filename);
	Level_Editor_Loki_Kirjaa("----------------------", LOKI_INFO);
	Level_Editor_Loki_Kirjaa(msg, LOKI_INFO);
	Level_Editor_Loki_Kirjaa("----------------------", LOKI_INFO);

	//Level_Editor_Loki_Kirjaa("trying to load map.", LOKI_INFO);

	DWORD loki_muistia = PisteDraw_Videomuistia();
	char muistia[255];
	itoa(loki_muistia,muistia,10);
	
	Level_Editor_Loki_Tallenna("Videomuistia: ");
	Level_Editor_Loki_Tallenna(muistia);
	Level_Editor_Loki_Tallenna("\n");

	Level_Editor_Loki_Tallenna("Aloitetaan kartan ");
	Level_Editor_Loki_Tallenna(filename);
	Level_Editor_Loki_Tallenna(" lataus.\n");

	Level_Editor_Kartta_Varmista();

	//Level_Editor_Loki_Tallenna("Kartta varmistettu (undo).\n");

	Level_Editor_Prototyyppi_Tyhjenna();

	//Level_Editor_Loki_Tallenna("Prototyypit tyhjennetty.\n");

	strcpy(karttatiedosto, filename);

	if ((virhe = kartta->Lataa("",filename)) != 0)
	{
		if (virhe == 1)
		{
			Level_Editor_Loki_Kirjaa("could not load map!", LOKI_VIRHE);
			//Level_Editor_Loki_Tallenna("Kartan lataus epäonnistui.\n");
			strcpy(karttatiedosto, " ");
			return(1);
		}
		
		if (virhe == 2)
		{
			Level_Editor_Loki_Kirjaa("this editor version cant read this map!", LOKI_VIRHE);
			return(1);
		}
	}

	Level_Editor_Loki_Kirjaa("map loaded", LOKI_INFO);
	//strcpy(viesti,"map loaded: ");

	strcpy(karttatiedosto, filename);

	Level_Editor_Kartta_Paivita();

	//Level_Editor_Loki_Tallenna("Kartta päivitetty.\n");
	
	
	kartta_ladattu = true;

	//Level_Editor_Loki_Tallenna("Kartta ladattu.\n");

	if (strcmp(kartta->versio,"1.2")==0 || strcmp(kartta->versio,"1.3")==0)
		Level_Editor_Prototyyppi_Lataa_Kaikki();
	else
		Level_Editor_Prototyyppi_Tyhjenna();
/*
	char polku[_MAX_DIR];
	strcpy(polku,pk2_hakemisto);
	strcat(polku,"\\");

	if (kartta->Lataa_PalikkaPaletti("",kartta->palikka_bmp)!=0)
	{
		if (kartta->Lataa_PalikkaPaletti(polku,kartta->palikka_bmp)!=0)
		{
			strcpy(viesti,"could not find tiles ");
			strcat(viesti,kartta->palikka_bmp);
		}
	}

	if (kartta->Lataa_Taustakuva("",kartta->taustakuva)!=0)
	{
		if (kartta->Lataa_Taustakuva(polku,kartta->taustakuva)!=0)
		{
			strcpy(viesti,"could not find background ");
			strcat(viesti,kartta->taustakuva);
		}
	}
*/
	Level_Editor_Loki_Tallenna("Sprite-prototyypit on ladattu.\n");

	menu_spritet_eka = 0;

	Level_Editor_Laske_Spritet();

	Level_Editor_Laske_TileVarit();

	Level_Editor_Kartta_Paivita();

	Level_Editor_Loki_Kirjaa("loading map completed.", LOKI_INFO);

	return(0);
}

void Level_Editor_Laske_TileVarit()
{
	
	UCHAR *buffer = NULL;
	DWORD leveys;
	int i;
	DWORD x,y;
	DWORD paavari;
	DWORD keskiarvoVari;
	int lapinakyvia;
	UCHAR vari;
	DWORD lkm;
	DWORD paavarit[9];
	
	Level_Editor_Loki_Kirjaa("calculating tile colors.", LOKI_INFO);

	PisteDraw_Piirto_Aloita(kartta->palikat_buffer,*&buffer,(DWORD &)leveys);

	for (int tile = 0; tile < 150; tile++){
		
		paavari = 0;
		keskiarvoVari = 0;
		lapinakyvia = 0;
		lkm = 0;
		
		for (i=0;i<8;i++)
			paavarit[i] = 0;

		for (x=0;x<32;x++){
			for (y=0;y<32;y++){
				
				vari = buffer[x + ((tile%10)*32) + (y+(tile/10)*32) * leveys];

				
				if (vari < 224) {
					
					paavari = (vari/32);
					paavarit[paavari] = paavarit[paavari] + 1;
					keskiarvoVari = keskiarvoVari+(vari%32);
					lkm++;
				}
			}
		}

		paavari = 0;
		DWORD paavarilkm = 0;

		for (i=0;i<8;i++)
			if (paavarit[i] > paavarilkm){
				paavari = i;
				paavarilkm = paavarit[i];
			}

		if (lkm > 0)
			keskiarvoVari     = keskiarvoVari / lkm;
		else
			keskiarvoVari	  = 0;

		if (tile < 90)
			keskiarvoVari += 3;

		if (tile == PALIKKA_KYTKIN1 || tile == PALIKKA_KYTKIN2 || tile == PALIKKA_KYTKIN3 ||
			tile == PALIKKA_ALOITUS || tile == PALIKKA_LOPETUS)
			keskiarvoVari += 12;

		if (keskiarvoVari > 31)
			keskiarvoVari = 31;


		karttavarit[tile] = (UCHAR)(keskiarvoVari + paavari*32);
		//karttavarit[tile] = UCHAR(paavari*32);*/
	}


	PisteDraw_Piirto_Lopeta(kartta->palikat_buffer);

}


int Level_Editor_Menu_Alusta(int index, char *otsikko, int leveys, int korkeus, int x, int y, int pika)
{
	menut[index].korkeus = korkeus;
	menut[index].leveys = leveys;
	menut[index].x = x;
	menut[index].y = y;
	strcpy(menut[index].otsikko,otsikko);
	menut[index].piilota = true;
	menut[index].pika = pika;
	return 0;
}


int Level_Editor_Menut_Alusta(void)
{
	Level_Editor_Menu_Alusta(MENU_HELP,		"help    (f1)", 256, 240, 192, 120, DIK_F1);
	Level_Editor_Menu_Alusta(MENU_KARTTA,	"map     (f2)", PK2KARTTA_KARTTA_LEVEYS, PK2KARTTA_KARTTA_KORKEUS, 492, 130, DIK_F2);
	Level_Editor_Menu_Alusta(MENU_PALIKAT,	"tiles   (f3)", 320, 480, 592, 145, DIK_F3);
	Level_Editor_Menu_Alusta(MENU_SPRITET,	"sprites (f4)", 320, 480, 262, 45,  DIK_F4);
	Level_Editor_Menu_Alusta(MENU_TIEDOSTOT,"files   (f5)", 320, 300, 192, 40,  DIK_F5);
	Level_Editor_Menu_Alusta(MENU_SAVE,		"save    (f6)", 320, 50,  192, 355, DIK_F6);
	Level_Editor_Menu_Alusta(MENU_INFO,		"map information (f7)", 320, 240,  150, 40, DIK_F7);
	Level_Editor_Menu_Alusta(MENU_EXIT,		"exit editor?", 256, 35,  RUUDUN_LEVEYS/2-128, RUUDUN_KORKEUS/2-15, DIK_ESCAPE);
	Level_Editor_Menu_Alusta(MENU_TOOLS,	"tools   (f8)", 320, 200, 422, 210, DIK_F8);
	Level_Editor_Menu_Alusta(MENU_LOKI,		"log     (f9)", 320, 500, 222, 110, DIK_F9);

	menut[MENU_HELP].piilota = false;
	
	return 0;
}

int Level_Editor_Tiedostot_Alusta(void)
{
	for (int i=0;i<MAX_DIR_TIEDOSTOJA;i++)
	{
		strcpy(tiedostot[i].nimi,"\0");
		tiedostot[i].tyyppi = 0;
	}

	return 0;
}

int Level_Editor_Tiedostot_Vertaa(char *a, char *b)
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

int Level_Editor_Tiedostot_Aakkosta(void)
{
	DWORD i,t;
	TIEDOSTO temp;
	bool tehty;

	if (tiedostoja > 1)
	{
		for (i=tiedostoja-1;i>=0;i--)
		{
			tehty = true;

			for (t=0;t<i;t++)
			{
				if (Level_Editor_Tiedostot_Vertaa(tiedostot[t].nimi,tiedostot[t+1].nimi) == 1)
				{
					temp = tiedostot[t];
					tiedostot[t] = tiedostot[t+1];
					tiedostot[t+1] = temp;
					tehty = false;
				}
			}
			
			if (tehty)
				return 0;
		}
	}
	
	return 0;
}

int Level_Editor_Tiedostot_Hae_Tyyppi2(struct _finddata_t &map_file, long &hFile, int &i, 
									  char *tiedosto, int tyyppi)
{
	//char tiedosto[6];
	bool loytyi = false;
	
	//strcpy(tiedosto,"*.");
	//strcat(tiedosto,tarkenne);
	
	if (i >= MAX_DIR_TIEDOSTOJA)
		return 2;
	
	if((hFile = _findfirst(tiedosto, &map_file )) == -1L ){
       //strcpy(viesti,"no files found.");
		_findclose( hFile );
	}
	else
	{
		strcpy(tiedostot[i].nimi, map_file.name);
		tiedostot[i].tyyppi = tyyppi;
		i++;
		
		if (tyyppi == TIEDOSTO_MAP)
			Level_Editor_Aseta_Episodihakemisto();

		loytyi = true;
	}

	while (i<MAX_DIR_TIEDOSTOJA && _findnext( hFile, &map_file ) == 0)
	{
		strcpy(tiedostot[i].nimi, map_file.name);
		tiedostot[i].tyyppi = tyyppi;		
		i++;
		loytyi = true;
	}	

	_findclose( hFile );

	if (!loytyi)
		return 1;

	return 0;
}

int Level_Editor_Tiedostot_Hae_Tyyppi(int &i, char *tiedosto, int tyyppi)
{
    struct _finddata_t map_file;
    long hFile;
	bool loytyi = false;
	
	if (i >= MAX_DIR_TIEDOSTOJA)
		return 2;
	
	if((hFile = _findfirst(tiedosto, &map_file )) == -1L ){
       //strcpy(viesti,"no files found.");
		_findclose( hFile );
	}
	else
	{
		strcpy(tiedostot[i].nimi, map_file.name);
		tiedostot[i].tyyppi = tyyppi;
		i++;
		
		if (tyyppi == TIEDOSTO_MAP)
			Level_Editor_Aseta_Episodihakemisto();

		loytyi = true;
	}

	while (i<MAX_DIR_TIEDOSTOJA && _findnext( hFile, &map_file ) == 0)
	{
		strcpy(tiedostot[i].nimi, map_file.name);
		tiedostot[i].tyyppi = tyyppi;		
		i++;
		loytyi = true;
	}	

	_findclose( hFile );

	if (!loytyi)
		return 1;

	return 0;
}

int Level_Editor_Tiedostot_Hae(void)
{
/*    struct _finddata_t map_file;
    long hFile;*/
	int i=0;
/*
	if((hFile = _findfirst( "*.", &map_file )) == -1L )
       strcpy(viesti,"no directories found");
	else
	{
		strcpy(tiedostot[i].nimi, map_file.name);
		tiedostot[i].tyyppi = TIEDOSTO_DIR;
		i++;
	}

	while (i<MAX_DIR_TIEDOSTOJA && _findnext( hFile, &map_file ) == 0)
	{
		strcpy(tiedostot[i].nimi, map_file.name);
		tiedostot[i].tyyppi = TIEDOSTO_DIR;		
		i++;
	}

	_findclose( hFile );	
*/
	if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.", TIEDOSTO_DIR) == 2)
		return 0;

	if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.map", TIEDOSTO_MAP) == 2)
		return 0;		

	if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.bmp", TIEDOSTO_BMP) == 2)
		return 0;

	if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.pcx", TIEDOSTO_BMP) == 2)
		return 0;

	if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.spr", TIEDOSTO_SPR) == 2)
		return 0;	

	if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.xm", TIEDOSTO_MUS) == 2)
		return 0;

	if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.mod", TIEDOSTO_MUS) == 2)
		return 0;

	if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.s3m", TIEDOSTO_MUS) == 2)
		return 0;

	if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.it", TIEDOSTO_MUS) == 2)
		return 0;

	if (strcmp(pk2_hakemisto," ")==0) // jos hakemistoa, jossa pk2.exe sijaitsee ei ole vielä määritelty...
		if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "pk2.exe", 0) == 0)
			Level_Editor_Tallenna_PK2_Hakemisto(tyohakemisto);
					
		/*
	if (i >= MAX_DIR_TIEDOSTOJA)
		return 0;

	if((hFile = _findfirst( "*.map", &map_file )) == -1L )
       strcpy(viesti,"no map files found");
	else
	{
		strcpy(tiedostot[i].nimi, map_file.name);
		tiedostot[i].tyyppi = TIEDOSTO_MAP;
		i++;
		Level_Editor_Aseta_Episodihakemisto();
	}

	while (i<MAX_DIR_TIEDOSTOJA && _findnext( hFile, &map_file ) == 0)
	{
		strcpy(tiedostot[i].nimi, map_file.name);
		tiedostot[i].tyyppi = TIEDOSTO_MAP;		
		i++;
	}

	_findclose( hFile );

	if (i >= MAX_DIR_TIEDOSTOJA)
		return 0;

	if((hFile = _findfirst( "*.bmp", &map_file )) != -1L )
	{
		strcpy(tiedostot[i].nimi, map_file.name);
		tiedostot[i].tyyppi = TIEDOSTO_BMP;
		i++;
	}

	while (i<MAX_DIR_TIEDOSTOJA && _findnext( hFile, &map_file ) == 0)
	{
		strcpy(tiedostot[i].nimi, map_file.name);
		tiedostot[i].tyyppi = TIEDOSTO_BMP;		
		i++;
	}

	_findclose( hFile );


	if (i >= MAX_DIR_TIEDOSTOJA)
		return 0;

	if((hFile = _findfirst( "*.spr", &map_file )) != -1L )
	{
		strcpy(tiedostot[i].nimi, map_file.name);
		tiedostot[i].tyyppi = TIEDOSTO_SPR;
		i++;
	}

	while (i<MAX_DIR_TIEDOSTOJA && _findnext( hFile, &map_file ) == 0)
	{
		strcpy(tiedostot[i].nimi, map_file.name);
		tiedostot[i].tyyppi = TIEDOSTO_SPR;		
		i++;
	}

	_findclose( hFile );

	if (strcmp(pk2_hakemisto," ")==0) // jos hakemistoa, jossa pk2.exe sijaitsee ei ole vielä määritelty...
	{

		if((hFile = _findfirst( "*.exe", &map_file )) != -1L )
		{
			if (strcmp("PK2.exe", map_file.name)==0)
				Level_Editor_Tallenna_PK2_Hakemisto(tyohakemisto);
		}

		while (_findnext( hFile, &map_file ) == 0)
		{
			if (strcmp("PK2.exe", map_file.name)==0)
				Level_Editor_Tallenna_PK2_Hakemisto(tyohakemisto);
		}

		_findclose( hFile );

	}
*/
	tiedostoja = i;

	return 0;
}

void Level_Editor_Aseta_Palikat(char *filename)
{
	Level_Editor_Loki_Tallenna("Ladataan palikkapaletti.\n");
	Level_Editor_Loki_Kirjaa("loading tile palette.", LOKI_INFO);
	
	if (kartta->Lataa_PalikkaPaletti("",filename) == 1)
		DirectX_virhe = true;

	Level_Editor_Laske_TileVarit();
	
}

void Level_Editor_Aseta_Taustakuva(char *filename)
{
	Level_Editor_Loki_Tallenna("Ladataan taustakuva.\n");
	Level_Editor_Loki_Kirjaa("loading background image.", LOKI_INFO);
	
	if (kartta->Lataa_Taustakuva("",filename) == 1)
		DirectX_virhe = true;
}

int Level_Editor_Init(void)
{
	Level_Editor_Loki_Alusta();
	Level_Editor_Loki_Kirjaa("--------------------", LOKI_INFO);
	Level_Editor_Loki_Kirjaa("starting new session", LOKI_INFO);
	Level_Editor_Loki_Kirjaa("--------------------", LOKI_INFO);
	Level_Editor_Loki_Tallenna("----------------------------------\n");
	Level_Editor_Loki_Tallenna("Uusi istunto.\n");
	Level_Editor_Loki_Tallenna("----------------------------------\n");

	for (int ci=0; ci<360; ci++) 
		cos_table[ci] = cos(3.1415*2* (ci%360)/180)*33;
    
	for (int si=0; si<360; si++) 
		sin_table[si] = sin(3.1415*2* (si%360)/180)*33;

	PK2Kartta_Cos_Sin(cos_table, sin_table);
	PK2Kartta_Aseta_Ruudun_Mitat(RUUDUN_LEVEYS,RUUDUN_KORKEUS);

	_getcwd(tyohakemisto, _MAX_PATH);

	strcpy(editor_hakemisto,tyohakemisto);

	Level_Editor_Loki_Kirjaa("looking for pk2 directory.", LOKI_INFO);
	Level_Editor_Lataa_PK2_Hakemisto();
	
	Level_Editor_Tiedostot_Alusta();
	Level_Editor_Tiedostot_Hae();
	Level_Editor_Tiedostot_Aakkosta();

	if (strcmp(pk2_hakemisto," ")==0)
	{
		Level_Editor_Loki_Kirjaa("pk2 directory not found.", LOKI_INFO);
		MessageBox(ikkunan_kahva, "Welcome to Pekka Kana 2 Level Editor :)\nUse the file window (f5) to find the directory\nwhere you have installed Pekka Kana 2.", "PK2 Level Editor 1.0", MB_OK);
	}
	else
		MessageBox(ikkunan_kahva, "Welcome to Pekka Kana 2 Level Editor:)","PK2 Level Editor 0.91", MB_OK);


	if ((PisteInput_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &)hinstance_app)) == PI_VIRHE)
		DirectX_virhe = true;

	if ((PisteDraw_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &) hinstance_app, 
								  RUUDUN_LEVEYS, RUUDUN_KORKEUS, SCREEN_BPP, MAX_COLORS_PALETTE)) == PD_VIRHE)
		DirectX_virhe = true;


	if ((kuva_editori = PisteDraw_Buffer_Uusi(RUUDUN_LEVEYS,RUUDUN_KORKEUS, true, 255)) == PD_VIRHE)
		DirectX_virhe = true;

	if ((kuva_kartta  = PisteDraw_Buffer_Uusi(PK2KARTTA_KARTTA_LEVEYS,PK2KARTTA_KARTTA_KORKEUS, true, 255)) == PD_VIRHE)
		DirectX_virhe = true;	

	if (PisteDraw_Lataa_Kuva(kuva_editori,"pk2edit.bmp", true) == PD_VIRHE)
	{
		Level_Editor_Loki_Kirjaa("could not load pk2edit.bmp.", LOKI_INFO);
		DirectX_virhe = true;
	}

	kartta  = new PK2Kartta();
	undo	= new PK2Kartta();

	if ((fontti1 = PisteDraw_Font_Uusi(kuva_editori,1,456,8,8,52)) == PD_VIRHE)
		DirectX_virhe = true;
	/*
	protot[PROTOTYYPPI_KANA].Kana("pk2spr01.bmp");
	protot[PROTOTYYPPI_MUNA].Muna("pk2spr01.bmp");
	protot[PROTOTYYPPI_PIKKUKANA].Pikkukana("pk2spr01.bmp");
	protot[PROTOTYYPPI_OMENA].Omena("pk2spr01.bmp");
*/
	Level_Editor_Kartta_Alusta();

	//Level_Editor_Kartta_Oletusasetukset();

	Level_Editor_Menut_Alusta();
	
	Level_Editor_Kartta_Varmista();

	PisteDraw_Fade_Paletti_In(PD_FADE_NOPEA);

	PisteWait_Start();

	Level_Editor_Loki_Kirjaa("startup completed.", LOKI_INFO);

	Level_Editor_Asetukset_Lataa();

	return 0;
}

/* PIIRTORUTIINIT ---------------------------------------------------------------*/

int Level_Editor_Piirra_Nuolet(int x, int y, int menuId)
{
	int leveys = 13,
		korkeus = 27;

	PisteDraw_Buffer_Flip_Nopea(kuva_editori,PD_TAUSTABUFFER,x,y,324,1,337,28);

	if (PisteInput_Hiiri_Vasen() && key_delay == 0 && menuId==aktiivinen_menu)
	{
		if (hiiri_x > x && hiiri_x < x+leveys && hiiri_y > y && hiiri_y < y+korkeus/2)
		{
			key_delay = 8;
			return 1;
		}
		
		if (hiiri_x > x && hiiri_x < x+leveys && hiiri_y > y+korkeus/2 && hiiri_y < y+korkeus)
		{
			key_delay = 8;
			return 2;
		}
	}

	return 0;
}

int Level_Editor_Piirra_Nuolet2(int x, int y, int menuId)
{
	int leveys = 13,
		korkeus = 13;

	PisteDraw_Buffer_Flip_Nopea(kuva_editori,PD_TAUSTABUFFER,x,y,   324, 1,337,15);
	PisteDraw_Buffer_Flip_Nopea(kuva_editori,PD_TAUSTABUFFER,x+14,y,324,15,337,28);

	if (PisteInput_Hiiri_Vasen() && key_delay == 0 && menuId==aktiivinen_menu)
	{
		if (hiiri_x > x && hiiri_x < x+13 && hiiri_y > y && hiiri_y < y+13)
		{
			key_delay = 8;
			return 1;
		}
		
		if (hiiri_x > x+14 && hiiri_x < x+27 && hiiri_y > y && hiiri_y < y+13)
		{
			key_delay = 8;
			return 2;
		}
	}

	return 0;
}

int Level_Editor_Piirra_Nelio(int vasen, int yla, int oikea, int ala, UCHAR color)
{
/*
	UCHAR *buffer = NULL;
	DWORD tod_leveys;

	PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer, (DWORD &)tod_leveys);

	for (int x = vasen; x <= oikea; x++)
	{
		for (int y = yla; y <= ala; y++)
		{
			if (y == yla || y == ala || x == vasen || x == oikea)
			{
				buffer[x+y*tod_leveys] = color;
			}
		}		
	}

	PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER);
*/
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen,yla,oikea,yla+1,color);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen,ala-1,oikea,ala,color);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen,yla,vasen+1,ala,color);
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,oikea-1,yla,oikea,ala,color);

	return 0;
}

void Level_Editor_Viiva_Hori(int x, int y, int pituus, UCHAR vari)
{
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,x,y,x+pituus,y+1,vari);
}

void Level_Editor_Viiva_Vert(int x, int y, int pituus, UCHAR vari)
{
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,x,y,x+1,y+pituus,vari);
}

bool Level_Editor_Nappi(char *otsikko, int vasen, int yla, int menuId)
{
	bool paalla = false;

	int vali = PisteDraw_Font_Kirjoita(fontti1,otsikko,PD_TAUSTABUFFER,vasen+2,yla+2);
	
	//UCHAR *buffer = NULL;
	//DWORD tod_leveys;
	UCHAR color = 18;

	int oikea = vasen+vali+2,
		ala   = yla+12;

	if (hiiri_x < oikea && hiiri_x > vasen && hiiri_y > yla && hiiri_y < ala && 
		(menuId == aktiivinen_menu || menuId == -1))
	{
		paalla = true;
		color = 29;
	}

	Level_Editor_Piirra_Nelio(vasen+1,yla+1,oikea+1,ala+1,0);
	Level_Editor_Piirra_Nelio(vasen,yla,oikea,ala,color);

	if (paalla && PisteInput_Hiiri_Vasen() && key_delay == 0)
	{
		key_delay = 20;
		return true;
	}
	
	return false;
}

void Level_Editor_Lista(TOOL_LIST &lista)
{
	if (lista.nakyva)
	{
		int lista_y = 3;
		
		PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,lista.x, lista.y, lista.x + 58, lista.y + 75, 12);

		if (hiiri_x > lista.x && hiiri_x < lista.x+58 && hiiri_y > lista.y && hiiri_y < lista.y+75)
			edit_tila = EDIT_MENU;

		Level_Editor_Piirra_Nelio(lista.x,lista.y,lista.x+58,lista.y+75,1);

		if (Level_Editor_Nappi("copy  ",lista.x+3,lista.y+lista_y,-1))	lista.valinta = 1; lista_y += 14;
		if (Level_Editor_Nappi("paste ",lista.x+3,lista.y+lista_y,-1))	lista.valinta = 2; lista_y += 14;
		if (Level_Editor_Nappi("cut   ",lista.x+3,lista.y+lista_y,-1))	lista.valinta = 3; lista_y += 14;
		if (Level_Editor_Nappi("fill  ",lista.x+3,lista.y+lista_y,-1))	lista.valinta = 4; lista_y += 14;
		if (Level_Editor_Nappi("cancel",lista.x+3,lista.y+lista_y,-1))   lista.valinta = 5; lista_y += 14;

		if (lista.valinta != 0) 
			lista.nakyva = false;
	}
	//else
	//	lista.valinta = 0;
}

void Level_Editor_Lista_Nayta(TOOL_LIST &lista, int x, int y)
{
	lista.nakyva = true;
	lista.valinta = 0;
	lista.x = x;
	lista.y = y;
}

bool Level_Editor_Linkki(char *otsikko, int vasen, int yla, int menuId)
{
	bool paalla = false;

	int vali = PisteDraw_Font_Kirjoita(fontti1,otsikko,PD_TAUSTABUFFER,vasen,yla);
	
	int oikea = vasen+200, //vali,
		ala   = yla+8;

	if (menuId != aktiivinen_menu)
		return false;

	if (hiiri_x < oikea && hiiri_x > vasen && hiiri_y > yla && hiiri_y < ala)
	{
		paalla = true;
	}

	if (paalla)
	{
		UCHAR *buffer = NULL;
		DWORD tod_leveys;
		
		PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer, (DWORD &)tod_leveys);
	
		for (int x = vasen; x <= oikea; x++)
			for (int y = yla; y <= ala; y++)
				buffer[x+y*tod_leveys] = 41;
	
		PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER);

		PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,otsikko,PD_TAUSTABUFFER,vasen,yla,90);
	}

	if (paalla && PisteInput_Hiiri_Vasen() && key_delay == 0)
	{
		key_delay = 20;
		return true;
	}
	
	return false;
}


bool Level_Editor_Kysy2(char *teksti, int vasen, int yla, int lkm)
{
	bool paalla = false;
	
	UCHAR *buffer = NULL;
	DWORD tod_leveys;
	UCHAR color = 10;

	char vastaus[300];	
	char merkki;
	int font_index = 0;

	int oikea = vasen+lkm*8,
		ala   = yla+10;	

	int pituus = strlen(teksti);

	int i;

	strcpy(vastaus,teksti);

	font_index = (hiiri_x-vasen)/8;

	if (hiiri_x < oikea && hiiri_x > vasen && hiiri_y > yla && hiiri_y < ala)
	{
		paalla = true;
		color = 46;
		editoi_tekstia = true;
		font_index = (hiiri_x-vasen)/8;
	}

	if (font_index < lkm && paalla)
	{
		merkki = PisteInput_Lue_Nappaimisto();
		
		if (merkki != '\0' && key_delay == 0)
		{
			if (merkki == '\n')
			{
				for (i=font_index;i<lkm;i++)
					vastaus[i] = ' ';
				key_delay = 20;
				//merkki = '\0';
			}
			else
			{
				vastaus[font_index] = merkki;
			
				font_index++;
				key_delay = 20;
				hiiri_x += 8;
			}
			
			if (PisteInput_Keydown(DIK_BACK))
			{
				key_delay = 20;
				hiiri_x -= 8;
				if (font_index > 0)
					font_index--;
			}

			if (PisteInput_Keydown(DIK_DELETE))
			{
				for (i=font_index;i<lkm-1;i++)
					vastaus[i] = vastaus[i+1];				
				key_delay = 20;
			}
		}
	
		vastaus[lkm] = '\0';
	}

	PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer, (DWORD &)tod_leveys);
	
	for (int x = vasen; x <= oikea; x++)
		for (int y = yla; y <= ala; y++)
			buffer[x+y*tod_leveys] = color;

	if (paalla)
		for (x = vasen+font_index*8; x <= vasen+font_index*8+8; x++)
			for (int y = yla; y <= ala; y++)
				buffer[x+y*tod_leveys] = 20;		

	PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER);

	PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,vastaus,PD_TAUSTABUFFER,vasen,yla+2,85);

	strcpy(teksti,vastaus);
	
	return paalla;
}

bool Level_Editor_Kysy(char *teksti, int vasen, int yla, int lkm, int id, int menuId)
{
	bool editoi = false;
	
	UCHAR color = 10;

	char merkki;

	int oikea = vasen+lkm*8,
		ala   = yla+10;	

	int pituus = strlen(teksti);

	int i;

	if (menuId == aktiivinen_menu)
	{
		if (hiiri_x < oikea && hiiri_x > vasen && hiiri_y > yla && hiiri_y < ala &&
			key_delay == 0 && PisteInput_Hiiri_Vasen())
		{
			editKursori = (hiiri_x-vasen)/8;
			editKenttaId = id;
			memset(editTeksti,'\0',sizeof(editTeksti));
			strcpy(editTeksti,teksti); 
			key_delay = 20;

			if (editKursori > pituus)
				editKursori = pituus;	
		}

		if (id == editKenttaId)
		{
			editoi = true;
			editoi_tekstia = true;
			color = 46;
		}


		if (/*editKursori < lkm &&*/ editoi)
		{
			merkki = PisteInput_Lue_Nappaimisto();
			
			if (merkki != '\0' && key_delay == 0)
			{
				if (merkki == '\n')
				{
					for (i=editKursori;i<lkm;i++)
						editTeksti[i] = '\0';

					editKenttaId = 0;
					key_delay = 20;
					editoi_tekstia = false;
				}
				else
				{
					editTeksti[editKursori] = merkki;

					pituus = strlen(editTeksti);

					if (editKursori < lkm-1)
						editKursori++;
					
					if (merkki == editMerkki)
						key_delay = 10;//5;
					else
						key_delay = 20;//15;
					
					editMerkki = merkki;
				}
			}
			
			if (key_delay == 0)
			{
				if (PisteInput_Keydown(DIK_BACK))
				{
					if (editKursori > 0)
					{
						editKursori--;

						for (i=editKursori;i<lkm-1;i++)
							editTeksti[i] = editTeksti[i+1];
						
						editTeksti[lkm-1] = '\0';
						editTeksti[lkm] = '\0';
					}

					if (editMerkki == DIK_BACK)
						key_delay = 5;
					else
						key_delay = 20;
					
					editMerkki = DIK_BACK;

				}

				if (PisteInput_Keydown(DIK_DELETE))
				{
					for (i=editKursori;i<lkm-1;i++)
						editTeksti[i] = editTeksti[i+1];
					
					editTeksti[lkm-1] = '\0';
					editTeksti[lkm] = '\0';

					if (editMerkki == 'D')
						key_delay = 5;
					else
						key_delay = 20;
					
					editMerkki = 'D';
				}

				if (PisteInput_Keydown(DIK_LEFT))
				{
					if (editKursori > 0)
						editKursori--;
					
					key_delay = 5;
				}

				if (PisteInput_Keydown(DIK_RIGHT))
				{
					if (editKursori < lkm-1 && editKursori < pituus)
						editKursori++;

					key_delay = 5;
				}		
			
			}
			
			editTeksti[lkm] = '\0';
		}
	}

	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen,yla,oikea,ala,color);
	
	if (editoi)
		PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,vasen+editKursori*8,yla,vasen+editKursori*8+8,ala,20);

	PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,teksti,PD_TAUSTABUFFER,vasen,yla+2,85);

	if (editoi)
		strcpy(teksti,editTeksti);

	return editoi;
}

/*
int Level_Editor_Menu_Tyhja(int i)
{
	int x = menut[i].x,
		y = menut[i].y+16;

	jotakin...

	return 0;
}
*/

int Level_Editor_Menu_Spritet(int i)
{
	int x = menut[i].x,
		y = menut[i].y+16;
	int vali;
	
	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER, menut[i].x, menut[i].y, menut[i].x+menut[i].leveys, menut[i].y+menut[i].korkeus+16);

	int piirto_y = 5, piirto_x = 15, rv_korkeus = 0;
	bool rivinvaihto = false;
	
	int eka_proto = menu_spritet_eka;
	int vika_proto = menu_spritet_eka+8;

	if (vika_proto > MAX_PROTOTYYPPEJA)
		vika_proto = MAX_PROTOTYYPPEJA;

	if (eka_proto < 0)
		eka_proto = 0;

	char luku[10];
	int j_luku = eka_proto;

	for (int proto=eka_proto; proto<=vika_proto; proto++)
	{
		if (protot[proto].korkeus > 0)
		{
			if (protot[proto].korkeus > rv_korkeus)
				rv_korkeus = protot[proto].korkeus;

			if (hiiri_x > x+piirto_x-1 && hiiri_x < x+piirto_x+protot[proto].kuva_frame_leveys+1/*leveys+1*/ &&
				hiiri_y > y+piirto_y-1 && hiiri_y < y+piirto_y+protot[proto].kuva_frame_korkeus+1/*korkeus+1*/ &&
				hiiri_y < menut[i].y+menut[i].korkeus+16)
			{	
				Level_Editor_Piirra_Nelio(x+piirto_x-1, y+piirto_y-1, 
										  x+piirto_x+protot[proto].kuva_frame_leveys+1,/*  .frame_leveys+1*/
										  y+piirto_y+protot[proto].kuva_frame_korkeus+1, /*korkeus+1*/ 31);

				if (key_delay == 0 && PisteInput_Hiiri_Vasen())
				{
					proto_valittu = proto;
					key_delay = 10;
				}
				
				if (key_delay == 0 && PisteInput_Keydown(DIK_DELETE))
				{
					Level_Editor_Loki_Kirjaa("deleting sprite.", LOKI_INFO);
					Level_Editor_Prototyyppi_Poista(proto);
					key_delay = 30;
				}
			}
			else
			{
				Level_Editor_Piirra_Nelio(x+piirto_x-1, y+piirto_y-1, 
										  x+piirto_x+protot[proto].kuva_frame_leveys+1, 
										  y+piirto_y+protot[proto].kuva_frame_korkeus+1, 18);
			}
			
//			itoa(j_luku+1,luku,10);
//			PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,x+4,y+piirto_y);
			protot[proto].Piirra(x+piirto_x,y+piirto_y,0);
			PisteDraw_Font_Kirjoita(fontti1,protot[proto].nimi,PD_TAUSTABUFFER,x+120,y+piirto_y);
			PisteDraw_Font_Kirjoita(fontti1,kartta->protot[proto],PD_TAUSTABUFFER,x+120,y+piirto_y+10);
			
			if (protot[proto].tyyppi == TYYPPI_PELIHAHMO)
			{
				if (proto == kartta->pelaaja_sprite)
					vali = PisteDraw_Font_Kirjoita(fontti1,"player: yes",PD_TAUSTABUFFER,x+120,y+piirto_y+20);
				else
					vali = PisteDraw_Font_Kirjoita(fontti1,"player: no",PD_TAUSTABUFFER,x+120,y+piirto_y+20);

				if (Level_Editor_Nappi("set player",x+125+vali,y+piirto_y+18,i))
				{
					kartta->pelaaja_sprite = proto;
				}
			}

			itoa(j_luku+1,luku,10);
			PisteDraw_Font_Kirjoita(fontti1,luku,PD_TAUSTABUFFER,x+4,y+piirto_y);

			//vali = PisteDraw_Font_Kirjoita(fontti1,"bonus: ",PD_TAUSTABUFFER,x+240,y+piirto_y);
			//PisteDraw_Font_Kirjoita(fontti1,protot[proto].bonus_sprite,PD_TAUSTABUFFER,x+240+vali,y+piirto_y);
			

			Level_Editor_Viiva_Hori(x,y+piirto_y-3,320,13);
			
			if (protot[proto].kuva_frame_korkeus > 30)
				piirto_y += protot[proto].kuva_frame_korkeus + 6;  //rv_korkeus + 4;
			else
				piirto_y += 36;

			piirto_x = 15;
			rv_korkeus = 0;
/*
			else
			{
				piirto_x += protot[proto].leveys + 4;
			}			
*/
			j_luku++;
		}
	}

	int nuoli = Level_Editor_Piirra_Nuolet(x+menut[i].leveys-15,y+menut[i].korkeus-30,i);
	
	if (nuoli == 1 && menu_spritet_eka > 0)
	{
		menu_spritet_eka--;
	}

	if (nuoli == 2 && menu_spritet_eka < MAX_PROTOTYYPPEJA-1)
	{
		menu_spritet_eka++;
	}

	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);

	return 0;
}

int Level_Editor_Menu_Help(int i)
{
	int x = menut[i].x,
		y = menut[i].y+16,
		my = y+19;

	PisteDraw_Font_Kirjoita(fontti1,"f1 = help", PD_TAUSTABUFFER, x+3, my);			my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"f2 = map", PD_TAUSTABUFFER, x+3, my);			my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"f3 = tile palette", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"f4 = sprites", PD_TAUSTABUFFER, x+3, my);		my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"f5 = load map/files", PD_TAUSTABUFFER, x+3, my);		my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"f6 = save map", PD_TAUSTABUFFER, x+3, my);		my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"f7 = map information", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"f8 = clear map", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"s  = quick save ", PD_TAUSTABUFFER, x+3, my);	my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"u  = undo ", PD_TAUSTABUFFER, x+3, my);		my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"a  = animate moving tiles", PD_TAUSTABUFFER, x+3, my);	my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"shift = background / foreground", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"mouse left  = draw", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"mouse right = remove", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"mouse left  = paint (map menu)", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"mouse right = go to (map menu)", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"alt + mouse = select area", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"ctrl + c = copy", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"ctrl + b = paste simple", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"ctrl + v = paste pattern", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"ctrl + x = cut", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"arrows = scroll map", PD_TAUSTABUFFER, x+3, my);my+=9;
	PisteDraw_Font_Kirjoita(fontti1,"esc = exit", PD_TAUSTABUFFER, x+3, my);my+=9;

	return 0;
}

int Level_Editor_Menu_Kartta(int i)
{
	int x = menut[i].x,
		y = menut[i].y+15;

	PisteDraw_Buffer_Flip_Nopea(kuva_kartta,PD_TAUSTABUFFER, x, y, 0, 0, PK2KARTTA_KARTTA_LEVEYS, PK2KARTTA_KARTTA_KORKEUS);

	if (aktiivinen_menu == i)
	{
		if (menut[i].lista.nakyva)
			Level_Editor_Lista(menut[i].lista);
		
		RECT nelio = {0,0,0,0};
		nelio.left = hiiri_x-(RUUDUN_LEVEYS/32)/2;
		nelio.top = hiiri_y-(RUUDUN_KORKEUS/32)/2;

		if (nelio.left < x)
			nelio.left = x;

		if (nelio.left+RUUDUN_LEVEYS/32 > x + menut[i].leveys)
			nelio.left = x + menut[i].leveys - RUUDUN_LEVEYS/32;		

		if (nelio.top < y)
			nelio.top = y;

		if (nelio.top+RUUDUN_KORKEUS/32 > y + menut[i].korkeus)
			nelio.top = y + menut[i].korkeus - RUUDUN_KORKEUS/32;

		nelio.right  = nelio.left + RUUDUN_LEVEYS/32;
		nelio.bottom = nelio.top + RUUDUN_KORKEUS/32;

		Level_Editor_Piirra_Nelio(nelio.left-1, nelio.top-1, nelio.right+1, nelio.bottom+1, 0);
		Level_Editor_Piirra_Nelio(nelio.left, nelio.top, nelio.right, nelio.bottom, 31);
		Level_Editor_Piirra_Nelio(nelio.left+1, nelio.top+1, nelio.right-1, nelio.bottom-1, 0);

		if (PisteInput_Hiiri_Vasen() && hiiri_y > y && key_delay == 0)
		{
			kartta_x = nelio.left - x;
			kartta_y = nelio.top - y;
		}
		/*
		if (PisteInput_Hiiri_Oikea() && hiiri_y > y && kartta_kohde_status == 0 && key_delay == 0)
		{
			kartta_kohde_status = 1;
			kartta_kohde.left	= hiiri_x;
			kartta_kohde.top	= hiiri_y;
			kartta_kohde.right	= hiiri_x;
			kartta_kohde.bottom = hiiri_y;
			key_delay = 25;
		}

		if (kartta_kohde_status == 1)
		{
			if (kartta_kohde.left < x)
				kartta_kohde.left = x;
			
			if (kartta_kohde.top < y)
				kartta_kohde.top = y;
			
			if (hiiri_x >= kartta_kohde.left && hiiri_x < x+menut[i].leveys)
				kartta_kohde.right  = hiiri_x;
			if (hiiri_y >= kartta_kohde.top && hiiri_y < y+menut[i].korkeus)
				kartta_kohde.bottom = hiiri_y;
			
			Level_Editor_Piirra_Nelio(kartta_kohde.left, kartta_kohde.top, kartta_kohde.right, kartta_kohde.bottom, 48);
		
			if ((PisteInput_Hiiri_Oikea() && hiiri_y > y && key_delay == 0) || edit_tila == EDIT_KARTTA)
			{
				kartta_kohde_status = 2;
				Level_Editor_Lista_Nayta(menut[i].lista,x+90,y+35);
				key_delay = 25;
			}
		}

		if (kartta_kohde_status == 2)
		{
			if (kartta_kohde.left < x)
				kartta_kohde.left = x;
			
			if (kartta_kohde.top < y)
				kartta_kohde.top = y;

			if (kartta_kohde.right > x+menut[i].leveys)
				kartta_kohde.right = x+menut[i].leveys;

			if (kartta_kohde.bottom > y+menut[i].korkeus)
				kartta_kohde.bottom = y+menut[i].korkeus;			

			int k_vasen = kartta_kohde.left - x,
				k_yla	= kartta_kohde.top - y,
				k_oikea = kartta_kohde.right - x,
				k_ala	= kartta_kohde.bottom - y,
				kx = 0, ky = 0;
			
			Level_Editor_Piirra_Nelio(kartta_kohde.left, kartta_kohde.top, kartta_kohde.right, kartta_kohde.bottom, 48);

			if (menut[i].lista.valinta == 4)
			{
				Level_Editor_Kartta_Varmista();

				for (kx = k_vasen; kx < k_oikea; kx++)
					for (ky = k_yla; ky < k_ala; ky++)
					{
						if (edit_tila == EDIT_SPRITE)
						{
							kartta->spritet[kx+ky*PK2KARTTA_KARTTA_LEVEYS] = proto_valittu;
						}
						else
						{
							if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT)
								kartta->seinat[kx+ky*PK2KARTTA_KARTTA_LEVEYS] = edit_palikka;
							if (edit_kerros == EDIT_KERROS_TAUSTA)
								kartta->taustat[kx+ky*PK2KARTTA_KARTTA_LEVEYS] = edit_palikka;
						}
					}
				
				Level_Editor_Kartta_Paivita();

				strcpy(viesti,"u = undo paste");

				kartta_kohde_status = 0;
				key_delay = 25;
			}			

			if (menut[i].lista.valinta == 3)
			{
				Level_Editor_Kartta_Varmista();
				
				for (kx = k_vasen; kx < k_oikea; kx++)
					for (ky = k_yla; ky < k_ala; ky++)
					{
						if (edit_tila == EDIT_SPRITE)
						{
							kartta->spritet[kx+ky*PK2KARTTA_KARTTA_LEVEYS] = 255;
						}
						else
						{
							if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT)
								kartta->seinat[kx+ky*PK2KARTTA_KARTTA_LEVEYS] = 255;
							if (edit_kerros == EDIT_KERROS_TAUSTA)
								kartta->taustat[kx+ky*PK2KARTTA_KARTTA_LEVEYS] = 255;
						}
					}
				
				Level_Editor_Kartta_Paivita();

				strcpy(viesti,"u = undo delete");

				kartta_kohde_status = 0;
				key_delay = 25;
			}

			if (!menut[i].lista.nakyva)
				kartta_kohde_status = 0;

			
			//PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"mouse right = cancel, space = fill", PD_TAUSTABUFFER, x+2, menut[i].korkeus+menut[i].y-17, 70);
			//PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"delete = delete", PD_TAUSTABUFFER, x+2, menut[i].korkeus+menut[i].y-9, 70);
			
		}*/
	}

	return 0;
}

int Level_Editor_Menu_Palikat(int i)
{
	int x = menut[i].x,
		y = menut[i].y+15,
		y_plus = menu_palikat_nayta_y * 32;

	char luku[10];

	PisteDraw_Buffer_Flip_Nopea(kartta->palikat_buffer,PD_TAUSTABUFFER, x, y, 0, 0+y_plus, 320, 480+y_plus);

	if (aktiivinen_menu == i)
	{
		RECT nelio = {0,0,0,0};
		
		nelio.left = (((hiiri_x-x)/32)*32)+x;
		nelio.top =  (((hiiri_y-y)/32)*32)+y;

		if (nelio.left < x)
			nelio.left = x;

		if (nelio.left+32 > x + menut[i].leveys)
			nelio.left = x + menut[i].leveys - 32;		

		if (nelio.top < y)
			nelio.top = y;

		if (nelio.top+32 > y + menut[i].korkeus)
			nelio.top = y + menut[i].korkeus - 32;

		nelio.right  = nelio.left + 32;
		nelio.bottom = nelio.top + 32;

		Level_Editor_Piirra_Nelio(nelio.left+1, nelio.top+1, nelio.right+1, nelio.bottom+1, 0);
		Level_Editor_Piirra_Nelio(nelio.left, nelio.top, nelio.right, nelio.bottom, 120);

		int px = (hiiri_x-x)/32;
		int py = ((hiiri_y-y)/32)*10;

		itoa(px+py+menu_palikat_nayta_y*10+1,luku,10);
		PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,luku,PD_TAUSTABUFFER,nelio.left+2,nelio.top+2,65);

		if (PisteInput_Hiiri_Vasen() && hiiri_y > y)
		{
			edit_palikka = px + py + menu_palikat_nayta_y*10;
			proto_valittu = MAX_PROTOTYYPPEJA;
			key_delay = 15;
		}

		/*if (PisteInput_Hiiri_Oikea() && hiiri_y > y && key_delay == 0)
		{
			menu_palikat_nayta_y += 2;
			if (menu_palikat_nayta_y > 2)
				menu_palikat_nayta_y = 0;
			key_delay = 15;
		}*/
	}

	return 0;
}

int Level_Editor_Menu_Tiedostot(int i)
{
	int x = menut[i].x,
		y = menut[i].y+16,
		linkki_y = 0;

	bool paivita = false;
	char otsikko[_MAX_DIR];

	if (menu_tiedostot_eka < 0)
		menu_tiedostot_eka = 0;

	if (menu_tiedostot_eka > MAX_DIR_TIEDOSTOJA-2)
		menu_tiedostot_eka = MAX_DIR_TIEDOSTOJA-2;

	int eka_tiedosto  = menu_tiedostot_eka;
	int vika_tiedosto = menu_tiedostot_eka+35;

	if (vika_tiedosto > MAX_DIR_TIEDOSTOJA)
		vika_tiedosto = MAX_DIR_TIEDOSTOJA;

	if (eka_tiedosto < 0)
		eka_tiedosto = 0;

	int ti = menu_tiedostot_eka;
	int tilask = 0;

	//for (int ti=eka_tiedosto;ti<vika_tiedosto;ti++)
	while (linkki_y<33*9 && ti < MAX_DIR_TIEDOSTOJA)
	{
		if (tiedostot[ti].tyyppi != 0)
		{
			if (tiedostot[ti].tyyppi == TIEDOSTO_DIR && nayta_hakemistot)
			{
				strcpy(otsikko,tiedostot[ti].nimi);
				PisteDraw_Buffer_Flip_Nopea(kuva_editori,PD_TAUSTABUFFER,x+5,y+linkki_y+3,338,1,346,8);
				//strcat(otsikko,"...");
				if (Level_Editor_Linkki(otsikko,x+5+10,y+linkki_y+3,i))
				{
					if (_chdir(tiedostot[ti].nimi) == 0)
					{
						_getcwd(tyohakemisto, _MAX_PATH );
						paivita = true;
						strcpy(viesti,"changed directory");
						menu_tiedostot_eka = 0;
					}
					else
						strcpy(viesti,"could not open directory");
				}
				linkki_y += 9;
			}			
			
			switch (menu_tiedostot_nayta)
			{
			case 1:	if (tiedostot[ti].tyyppi == TIEDOSTO_MAP)
					{
						if (Level_Editor_Linkki(tiedostot[ti].nimi,x+5,y+linkki_y+3,i))
							Level_Editor_Kartta_Lataa(tiedostot[ti].nimi);
						linkki_y += 9;
					}
					break;
			case 2:	if (tiedostot[ti].tyyppi == TIEDOSTO_BMP)
					{
						if (Level_Editor_Linkki(tiedostot[ti].nimi,x+5,y+linkki_y+3,i))
						{
							Level_Editor_Aseta_Palikat(tiedostot[ti].nimi);
							strcpy(viesti,"tile palette loaded from picture");
						}
						linkki_y += 9;
					}
					break;
			case 3:	if (tiedostot[ti].tyyppi == TIEDOSTO_SPR)
					{
						if (Level_Editor_Linkki(tiedostot[ti].nimi,x+5,y+linkki_y+3,i))
							if (Level_Editor_Prototyyppi_Lataa_Uusi(tyohakemisto,tiedostot[ti].nimi) != 0)//..Uusi
							{
								//strcpy(viesti,"could not load sprite ");
								//strcat(viesti,tiedostot[ti].nimi);
							}
						linkki_y += 9;
					}
					break;
			case 4:	if (tiedostot[ti].tyyppi == TIEDOSTO_BMP)
					{
						if (Level_Editor_Linkki(tiedostot[ti].nimi,x+5,y+linkki_y+3,i))
						{
							Level_Editor_Aseta_Taustakuva(tiedostot[ti].nimi);
							strcpy(viesti,"background loaded from picture");
						}
						linkki_y += 9;
					}
					break;
			case 5:	if (tiedostot[ti].tyyppi == TIEDOSTO_MUS)
					{
						if (Level_Editor_Linkki(tiedostot[ti].nimi,x+5,y+linkki_y+3,i))
						{
							//Level_Editor_Aseta_Taustakuva(tiedostot[ti].nimi);
							strcpy(kartta->musiikki, tiedostot[ti].nimi);
							strcpy(viesti,"loaded music.");
						}
						linkki_y += 9;
					}
					break;
			default:break;//linkki_y += 9; break;
			}
			
					/*		
			if (Level_Editor_Linkki(tiedostot[ti].nimi,x+5,y+linkki_y+3))
			{
				if (tiedostot[ti].tyyppi == TIEDOSTO_DIR)
				{
					if (_chdir(tiedostot[ti].nimi) == 0)
					{
						_getcwd(tyohakemisto, _MAX_PATH );
						paivita = true;
						strcpy(viesti,"changed directory");

					}
					else
						strcpy(viesti,"could not open directory");
				}
				
				if (tiedostot[ti].tyyppi == TIEDOSTO_MAP)
				{
					Level_Editor_Kartta_Lataa(tiedostot[ti].nimi);
				}
				
				if (tiedostot[ti].tyyppi == TIEDOSTO_BMP)
				{
					Level_Editor_Aseta_Palikat(tiedostot[ti].nimi);
					strcpy(viesti,"block palette loaded from picture");
				}
				
				if (tiedostot[ti].tyyppi == TIEDOSTO_SPR)
				{
					if (Level_Editor_Prototyyppi_Lataa_Uusi(tiedostot[ti].nimi) != 0)
					{
						strcpy(viesti,"could not load sprite ");
						strcat(viesti,tiedostot[ti].nimi);
					}
				}
			}*/
		}
		ti++;
	}

	Level_Editor_Viiva_Vert(x + menut[i].leveys - 102, y, 300, 41);

	if (Level_Editor_Nappi("save   ",x + menut[i].leveys - 96,y+2,i ))
	{
		menut[MENU_SAVE].piilota = false;
		menu_tiedostot_eka = 0;
		
		if (strcmp(epd_hakemisto," ")!=0)
		{
			if (_chdir(epd_hakemisto) == 0)
			{
				_getcwd(tyohakemisto, _MAX_PATH );
				strcpy(viesti,"moved to last map folder");
				paivita = true;
			}
		}
	}
	
	Level_Editor_Viiva_Hori(x + menut[i].leveys - 102, y+20, 100, 41);
	
	if (Level_Editor_Nappi("maps   ",x + menut[i].leveys - 96,y+24,i))
	{
		menu_tiedostot_nayta = 1;
		menu_tiedostot_eka = 0;
		
		if (strcmp(epd_hakemisto," ")!=0)
		{
			if (_chdir(epd_hakemisto) == 0)
			{
				_getcwd(tyohakemisto, _MAX_PATH );
				strcpy(viesti,"moved to last map folder");
				paivita = true;
			}
		}
	}
	
	if (Level_Editor_Nappi("tiles  ",x + menut[i].leveys - 96,y+38,i))
	{
		menu_tiedostot_nayta = 2;
		menu_tiedostot_eka = 0;

		if (strcmp(tile_hakemisto," ")==0)
			strcpy(tile_hakemisto,epd_hakemisto);
		
		if (strcmp(tile_hakemisto," ")!=0)
		{
			if (_chdir(tile_hakemisto) == 0)
			{
				_getcwd(tyohakemisto, _MAX_PATH );
				strcpy(viesti,"moved to last map folder");
				paivita = true;
			}
		}
	}
	
	if (Level_Editor_Nappi("sprites",x + menut[i].leveys - 96,y+52,i))
	{
		menu_tiedostot_nayta = 3;
		menu_tiedostot_eka = 0;

		if (strcmp(pk2_hakemisto," ")!=0)
		{
			char temp[_MAX_DIR];
			strcpy(temp, pk2_hakemisto);
			strcat(temp,"\\sprites");
			if (_chdir(temp) == 0)
			{
				_getcwd(tyohakemisto, _MAX_PATH );
				strcpy(viesti,"moved to sprites folder");
				paivita = true;
			}
		}
	}

	if (Level_Editor_Nappi("scenery",x + menut[i].leveys - 96,y+66,i))
	{
		menu_tiedostot_nayta = 4;
		menu_tiedostot_eka = 0;

		if (strcmp(bg_hakemisto," ")==0)
			strcpy(bg_hakemisto,epd_hakemisto);		

		if (strcmp(bg_hakemisto," ")!=0)
		{
			if (_chdir(bg_hakemisto) == 0)
			{
				_getcwd(tyohakemisto, _MAX_PATH );
				strcpy(viesti,"moved to last map folder");
				paivita = true;
			}
		}
	}

	if (Level_Editor_Nappi("music  ",x + menut[i].leveys - 96,y+80,i))
	{
		menu_tiedostot_nayta = 5;
		menu_tiedostot_eka = 0;

		if (strcmp(pk2_hakemisto," ")!=0)
		{
			char temp[_MAX_DIR];
			strcpy(temp, pk2_hakemisto);
			strcat(temp,"\\music");
			if (_chdir(temp) == 0)
			{
				_getcwd(tyohakemisto, _MAX_PATH );
				strcpy(viesti,"moved to music folder");
				paivita = true;
			}
		}
	}

	if (nayta_hakemistot) {
		if (Level_Editor_Nappi("hide dirs",x + menut[i].leveys - 96,y+110,i)) nayta_hakemistot = !nayta_hakemistot;
	}
	else {
		if (Level_Editor_Nappi("show dirs",x + menut[i].leveys - 96,y+110,i)) nayta_hakemistot = !nayta_hakemistot;
	}

	if (paivita)
	{
		Level_Editor_Tiedostot_Alusta();
		Level_Editor_Tiedostot_Hae();
		Level_Editor_Tiedostot_Aakkosta();
	}

	int nuoli = Level_Editor_Piirra_Nuolet(x+menut[i].leveys-15,y+menut[i].korkeus-30,i);
	
	if (nuoli == 1 && menu_tiedostot_eka > 0)
	{
		menu_tiedostot_eka -= 15;
	}

	if (nuoli == 2 && menu_tiedostot_eka < MAX_DIR_TIEDOSTOJA-1)
	{
		menu_tiedostot_eka += 15;
	}

	return 0;
}

int Level_Editor_Menu_Tallenna(int i)
{
	int x = menut[i].x,
		y = menut[i].y+16;

	PisteDraw_Font_Kirjoita(fontti1,"type the name of the level:", PD_TAUSTABUFFER, x+3, y+3);

	Level_Editor_Kysy(karttatiedosto,x+3,y+15,12, KENTTA_FILE,i);

	if (Level_Editor_Nappi("save",x+3,y+30,i))
	{
		Level_Editor_Kartta_Tallenna();	
		Level_Editor_Tiedostot_Alusta();
		Level_Editor_Tiedostot_Hae();
		Level_Editor_Tiedostot_Aakkosta();
	}

	return 0;
}

int Level_Editor_Menu_Lopetus(int i)
{
	int x = menut[i].x,
		y = menut[i].y+16;

	PisteDraw_Font_Kirjoita(fontti1,"do you really want to quit?", PD_TAUSTABUFFER, x+3, y+3);

	if (Level_Editor_Nappi("quit",x+3,y+20,i))
	{
		PisteDraw_Fade_Paletti_Out(PD_FADE_NOPEA);
		exit_editor = true;
		key_delay = 15;	
	}

	if (Level_Editor_Nappi("cancel",x+45,y+20,i))
	{
		menut[i].piilota = true;
		key_delay = 15;		
	}

	if (Level_Editor_Nappi("save and quit",x+100,y+20,i))
	{
		if (Level_Editor_Kartta_Tallenna()==0)
		{
			PisteDraw_Fade_Paletti_Out(PD_FADE_NOPEA);
			exit_editor = true;
		}

		key_delay = 15;	
	}

	return 0;
}

int Level_Editor_Menu_Tiedot(int i)
{
	int x = menut[i].x,
		y = menut[i].y+16,
		my = y+19,
		vali = 0;

	char luku[20];
	
	vali = PisteDraw_Font_Kirjoita(fontti1,"version", PD_TAUSTABUFFER, x+3, my);
	PisteDraw_Font_Kirjoita(fontti1,kartta->versio, PD_TAUSTABUFFER, x+3+vali+16, my);my+=12;

	vali = PisteDraw_Font_Kirjoita(fontti1,"name:", PD_TAUSTABUFFER, x+3, my);
	Level_Editor_Kysy(kartta->nimi,x+83,my,16,KENTTA_NIMI,i);my+=12;

	vali = PisteDraw_Font_Kirjoita(fontti1,"file name:", PD_TAUSTABUFFER, x+3, my);
	Level_Editor_Kysy(karttatiedosto,x+83,my,12, KENTTA_FILE,i);my+=12;	

	vali = PisteDraw_Font_Kirjoita(fontti1,"creator:", PD_TAUSTABUFFER, x+3, my);
	Level_Editor_Kysy(kartta->tekija,x+83,my,29, KENTTA_TEKIJA,i);my+=12;

	vali = PisteDraw_Font_Kirjoita(fontti1,"tile picture:", PD_TAUSTABUFFER, x+3, my);
	PisteDraw_Font_Kirjoita(fontti1,kartta->palikka_bmp, PD_TAUSTABUFFER, x+3+vali+16, my);my+=12;

	vali = PisteDraw_Font_Kirjoita(fontti1,"background picture:", PD_TAUSTABUFFER, x+3, my);
	PisteDraw_Font_Kirjoita(fontti1,kartta->taustakuva, PD_TAUSTABUFFER, x+3+vali+16, my);my+=12;

	vali = PisteDraw_Font_Kirjoita(fontti1,"background music:", PD_TAUSTABUFFER, x+3, my);
	//PisteDraw_Font_Kirjoita(fontti1,kartta->musiikki, PD_TAUSTABUFFER, x+3+vali+16, my);my+=12;
	Level_Editor_Kysy(kartta->musiikki,x+3+vali+16,my,12, KENTTA_MUSIIKKI,i);my+=12;

	int nuoli;
	nuoli = Level_Editor_Piirra_Nuolet2(x+108,my,i);
	if (nuoli == 2 && kartta->jakso > 0)
		kartta->jakso--;
	if (nuoli == 1)
		kartta->jakso++;

	if (kartta->jakso < 0)
		kartta->jakso = 0;

	itoa(kartta->jakso,luku,10);
	vali = PisteDraw_Font_Kirjoita(fontti1,"level:", PD_TAUSTABUFFER, x+3, my);
	PisteDraw_Font_Kirjoita(fontti1,luku, PD_TAUSTABUFFER, x+3+vali+16, my);my+=12;
	my+=10;

	nuoli = Level_Editor_Piirra_Nuolet2(x+108,my,i);
	if (nuoli == 2 && kartta->aika > 0)
		kartta->aika -= 10;
	if (nuoli == 1)
		kartta->aika += 10;
	
	if (kartta->aika < 0)
		kartta->aika = 0;

	int min = kartta->aika / 60,
		sek = kartta->aika % 60;

	vali = PisteDraw_Font_Kirjoita(fontti1,"time:", PD_TAUSTABUFFER, x+3, my);
	if (min<10)
		vali += PisteDraw_Font_Kirjoita(fontti1,"0", PD_TAUSTABUFFER, x+3+vali+16, my);
	itoa(min,luku,10);	
	vali += PisteDraw_Font_Kirjoita(fontti1,luku, PD_TAUSTABUFFER, x+3+vali+16, my);

	if (sek<10)
		vali += PisteDraw_Font_Kirjoita(fontti1,":0", PD_TAUSTABUFFER, x+3+vali+16, my);	
	else
		vali += PisteDraw_Font_Kirjoita(fontti1,":", PD_TAUSTABUFFER, x+3+vali+16, my);
	
	itoa(sek,luku,10);	
	vali += PisteDraw_Font_Kirjoita(fontti1,luku, PD_TAUSTABUFFER, x+3+vali+16, my);	
	
	my+=16;
	
	
	char tausta[100];

	switch(kartta->tausta)
	{
	case TAUSTA_STAATTINEN				: strcpy(tausta,"no scrolling  ");break;
	case TAUSTA_PALLARX_HORI			: strcpy(tausta,"left and right");break;
	case TAUSTA_PALLARX_VERT			: strcpy(tausta,"up and down   ");break;
	case TAUSTA_PALLARX_VERT_JA_HORI	: strcpy(tausta,"free scrolling");break;
	default								: break;
	}
	
	vali = PisteDraw_Font_Kirjoita(fontti1,"background scrolling:", PD_TAUSTABUFFER, x+3, my);
	if (Level_Editor_Nappi(tausta,x+3+vali+16,my,i))
	{
		kartta->tausta++;
		kartta->tausta %= 4;
	}
	my+=16;

	switch(kartta->ilma)
	{
	case ILMA_NORMAALI					: strcpy(tausta,"normal       ");break;
	case ILMA_SADE						: strcpy(tausta,"rain         ");break;
	case ILMA_METSA						: strcpy(tausta,"leaves       ");break;
	case ILMA_SADEMETSA					: strcpy(tausta,"rain + leaves");break;
	case ILMA_LUMISADE					: strcpy(tausta,"snow         ");break;
	default								: break;
	}

	vali = PisteDraw_Font_Kirjoita(fontti1,"special:", PD_TAUSTABUFFER, x+3, my);
	if (Level_Editor_Nappi(tausta,x+3+vali+16,my,i))
	{
		kartta->ilma++;
		kartta->ilma %= 5;
	}
	
	my+=19;	

	/* Kartan ikoni */
	vali = PisteDraw_Font_Kirjoita(fontti1,"map icon:", PD_TAUSTABUFFER, x+3, my);
	itoa(kartta->ikoni+1,luku,10);	
	PisteDraw_Font_Kirjoita(fontti1, luku, PD_TAUSTABUFFER, x+3+vali+16, my);	
	
	nuoli = Level_Editor_Piirra_Nuolet2(x+118,my,i);
	if (nuoli == 1)
		kartta->ikoni++;
	if (nuoli == 2)
		kartta->ikoni--;
	
	if (kartta->ikoni < 0)
		kartta->ikoni = 0;

	my+=16;

	/* Kartan x-kordinaatti */
	vali = PisteDraw_Font_Kirjoita(fontti1,"map x:", PD_TAUSTABUFFER, x+3, my);
	itoa(kartta->x,luku,10);	
	PisteDraw_Font_Kirjoita(fontti1, luku, PD_TAUSTABUFFER, x+vali+9, my);	
	nuoli = Level_Editor_Piirra_Nuolet2(x+100,my,i);
	if (nuoli == 1)	kartta->x+= 15;
	if (nuoli == 2)	kartta->x-= 15;
	if (kartta->x < 0) kartta->x = 0;
	if (kartta->x > 620) kartta->x = 620;

	vali = PisteDraw_Font_Kirjoita(fontti1,"map y:", PD_TAUSTABUFFER, x+3+150, my);
	itoa(kartta->y,luku,10);	
	PisteDraw_Font_Kirjoita(fontti1, luku, PD_TAUSTABUFFER, x+vali+9+150, my);	
	nuoli = Level_Editor_Piirra_Nuolet2(x+100+150,my,i);
	if (nuoli == 1)	kartta->y+=15;
	if (nuoli == 2)	kartta->y-=15;
	if (kartta->y < 0) kartta->y = 0;
	if (kartta->y > 620) kartta->y = 620;

	my+=9;	
	
	/*
	vali = PisteDraw_Font_Kirjoita(fontti1,"block palette:", PD_TAUSTABUFFER, x+3, my);
	PisteDraw_Font_Kirjoita(fontti1,kartta->nimi, PD_TAUSTABUFFER, x+3+vali+16, my);my+=9;
	*/
	return 0;
}

int Level_Editor_Menu_Tools(int i)
{
	int x = menut[i].x,
		y = menut[i].y+16;

	//PisteDraw_Font_Kirjoita(fontti1,"", PD_TAUSTABUFFER, x+3, y+3);

	//Level_Editor_Kysy(karttatiedosto/*kartta->nimi*/,x+3,y+15,12, KENTTA_FILE);

	if (Level_Editor_Nappi("clear sprites",x+10,y+10,i)){
		if (i==aktiivinen_menu){
			Level_Editor_Kartta_Varmista();
			Level_Editor_Prototyyppi_Tyhjenna();
		}
	}

	y += 25;

	if (Level_Editor_Nappi("clear unused sprites",x+10,y+10,i)){
		if (i==aktiivinen_menu){
			Level_Editor_Kartta_Varmista();
			Level_Editor_Prototyyppi_Poista_Turhat();
			menu_spritet_eka = 0;
			//Level_Editor_Prototyyppi_Tyhjenna();
		}
	}

	y += 25;

	if (Level_Editor_Nappi("clear all",x+10,y+10,i)){
		if (i==aktiivinen_menu){
			Level_Editor_Kartta_Varmista();
			Level_Editor_Kartta_Alusta();
		}
	}

	y += 25;

	return 0;
}

int Level_Editor_Menu_Loki(int i)
{
	int x = menut[i].x,
		y = menut[i].y+16;

	int yl = 0;

	for (int l=0;l<MAX_LOKIMERKINTOJA;l++){

		if (loki[l].tyyppi == LOKI_VIRHE){
			PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER, x+3, y+3+l*9+3, x+7, y+7+l*9+3, 20+64);
		}
		
		if (loki[l].tyyppi == LOKI_INFO){
			PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER, x+3, y+3+l*9+3, x+7, y+7+l*9+3, 20+96);
		}
		
		PisteDraw_Font_Kirjoita(fontti1,loki[l].teksti, PD_TAUSTABUFFER, x+3+8, y+3+l*9);
		
	}


	return 0;
}

int Level_Editor_Menu_Piirra(int index, bool tayta, UCHAR vari)
{
	int leveys = menut[index].leveys;
	int korkeus = menut[index].korkeus;

	leveys = leveys - 14;

	//PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER, menut[index].x-1, menut[index].y-1, 
	//					   menut[index].x+menut[index].leveys+1, menut[index].y+16+korkeus, 0);

	Level_Editor_Piirra_Nelio(menut[index].x-1, menut[index].y-1, 
							  menut[index].x+menut[index].leveys+1, menut[index].y+16+korkeus, 0);

	if (tayta)
	{
		double kork = menut[index].korkeus/6.0;
		double ky = 0;
		for (int c=0;c<6;c++)	
		{
			PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER, menut[index].x, menut[index].y+15+int(ky), 
							   menut[index].x+menut[index].leveys,  menut[index].y+15+int(ky+kork), 6-c+vari);
			ky += kork;
		}
	}

	PisteDraw_Buffer_Flip_Nopea(kuva_editori,PD_TAUSTABUFFER, menut[index].x, menut[index].y, 
								1, 1, 5, 16);

	PisteDraw_Buffer_Flip_Nopea(kuva_editori,PD_TAUSTABUFFER, menut[index].x+4, menut[index].y, 
								6, 1, 6+leveys, 16);

	PisteDraw_Buffer_Flip_Nopea(kuva_editori,PD_TAUSTABUFFER, menut[index].x+2+leveys, menut[index].y, 
								311, 1, 323, 16);

	if (index == aktiivinen_menu)
		PisteDraw_Font_Kirjoita(fontti1,menut[index].otsikko,PD_TAUSTABUFFER,menut[index].x+5,menut[index].y+3);
	else
		PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,menut[index].otsikko,PD_TAUSTABUFFER,menut[index].x+5,menut[index].y+3,50);
	
/*
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER, menut[index].x, menut[index].y+15, 
						   menut[index].x+menut[index].leveys, menut[index].y+16+korkeus, 0);
*/

	switch(index)
	{
		case MENU_HELP		: Level_Editor_Menu_Help(index);break;
		case MENU_KARTTA	: Level_Editor_Menu_Kartta(index);break;
		case MENU_PALIKAT	: Level_Editor_Menu_Palikat(index);break;
		case MENU_SPRITET	: Level_Editor_Menu_Spritet(index);break;
		case MENU_TIEDOSTOT	: Level_Editor_Menu_Tiedostot(index);break;
		case MENU_SAVE		: Level_Editor_Menu_Tallenna(index);break;
		case MENU_INFO		: Level_Editor_Menu_Tiedot(index);break;
		case MENU_TOOLS		: Level_Editor_Menu_Tools(index);break;
		case MENU_LOKI		: Level_Editor_Menu_Loki(index);break;
		case MENU_EXIT		: Level_Editor_Menu_Lopetus(index);break;
		//case MENU_TYOKALUT	: Level_Editor_Menu_Tyokalut(index);break;
		default				: break;
	}

	return 0;
}

int Level_Editor_Menut_Piirra(void)
{
	for (int i=0;i<MAX_MENUJA;i++)
	{
		if (!menut[i].piilota && i != aktiivinen_menu)
			Level_Editor_Menu_Piirra(i, true, VARI_SININEN);
	}

	if (aktiivinen_menu != MENU_EI && aktiivinen_menu < MAX_MENUJA)
		Level_Editor_Menu_Piirra(aktiivinen_menu, true, VARI_SININEN);	

	return 0;
}

int Level_Editor_Piirra_Spritet(void)
{
	int proto;

	for (int x=0;x<RUUDUN_LEVEYS/32;x++)
	{
		for (int y=0;y<RUUDUN_KORKEUS/32;y++)
		{
			proto = kartta->spritet[x+kartta_x+(y+kartta_y)*PK2KARTTA_KARTTA_LEVEYS];
			
			if (proto != 255 && protot[proto].tyyppi != TYYPPI_EI_MIKAAN)
			{
				protot[proto].Piirra(x*32+16-protot[proto].leveys/2,y*32-protot[proto].korkeus+32,0);
			}
		}
	}	
	return 0;
}

int Level_Editor_Piirra_Kartta(void)
{
	/*
	int palikka;
	int px = 0,
		py = 0;*/

	//PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,640,480);

	if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_TAUSTA)
		kartta->Piirra_Taustat(kartta_x*32, kartta_y*32, animaatio_paalla);

	Level_Editor_Piirra_Spritet();

	if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT)
		kartta->Piirra_Seinat(kartta_x*32, kartta_y*32, animaatio_paalla);

	//PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,RUUDUN_LEVEYS,RUUDUN_KORKEUS);

	return 0;
}

/* Tässä editoidaan kartan tausta ja seina taulukoita */

int Level_Editor_Edit_Kartta(void)
{
	int x = kartta_x + hiiri_x / 32;
	int y = kartta_y + hiiri_y / 32;

	if (y > 223)
		y = 223;

	if (edit_tila == EDIT_SPRITE || edit_tila == EDIT_KARTTA)
	{
		if (!PisteInput_Hiiri_Vasen() && !PisteInput_Hiiri_Oikea())
			piirto_aloitettu = false;

		if ((PisteInput_Hiiri_Vasen() || PisteInput_Hiiri_Oikea()) && piirto_aloitettu == false)
		{
			piirto_aloitettu = true;
			Level_Editor_Kartta_Varmista();
		}
	}
	else
		piirto_aloitettu = false;


	if (edit_tila == EDIT_KARTTA)
	{
		focustile_etu  = kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS];
		focustile_taka = kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS];

		if (PisteInput_Hiiri_Vasen() && key_delay == 0)
		{
			
			if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT)
			{
				kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS] = edit_palikka;
			}

			if (edit_kerros == EDIT_KERROS_TAUSTA)
			{
				kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS] = edit_palikka;
			}

			Level_Editor_Kartta_Paivita();
			strcpy(viesti,"map was updated.");
			key_delay = 5;
		}
		
		if (PisteInput_Hiiri_Oikea() && key_delay == 0)
		{
		
			
			if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT)
			{
				kartta->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS] = 255;
			}

			if (edit_kerros == EDIT_KERROS_TAUSTA)
			{
				kartta->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS] = 255;
			}

			//Level_Editor_Lista_Nayta(leikepoyta_lista, hiiri_x, hiiri_y);

			Level_Editor_Kartta_Paivita();
			strcpy(viesti,"map was updated.");
			key_delay = 5;
		}
		
	}

	if (edit_tila == EDIT_SPRITE)
	{
		focussprite    = kartta->spritet[x+y*PK2KARTTA_KARTTA_LEVEYS];

		if (PisteInput_Hiiri_Vasen() && key_delay == 0)
		{
			kartta->spritet[x+y*PK2KARTTA_KARTTA_LEVEYS] = proto_valittu;
			Level_Editor_Kartta_Paivita();
			strcpy(viesti,"map was updated.");
			key_delay = 5;
		}
		
		if (PisteInput_Hiiri_Oikea() && key_delay == 0)
		{
			kartta->spritet[x+y*PK2KARTTA_KARTTA_LEVEYS] = 255;
			Level_Editor_Kartta_Paivita();
			strcpy(viesti,"map was updated.");
			key_delay = 5;
		}
	}

	return 0;
}

int Level_Editor_Piirra_Infoa(void)
{
	int vali = 0;
	char luku[40];

	if (edit_tila == EDIT_KARTTA)
	{
		vali += PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"x: ",PD_TAUSTABUFFER,2+vali,2,65);
	
		itoa(kartta_x+hiiri_x/32,luku,10);
	
		vali += 8+PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,luku,PD_TAUSTABUFFER,2+vali,2,65);
	
		vali += PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"y: ",PD_TAUSTABUFFER,2+vali,2,65);
	
		itoa(kartta_y+hiiri_y/32,luku,10);	
	
		vali += 10 + PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,luku,PD_TAUSTABUFFER,2+vali,2,65);

		
		if (focustile_etu != 255)
			itoa(focustile_etu+1,luku,10);
		else 
			strcpy(luku,"n/a");
		PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"foreground tile: ",PD_TAUSTABUFFER,2,14,65);
		PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,luku,PD_TAUSTABUFFER,150,14,65);

		if (focustile_taka != 255)
			itoa(focustile_taka+1,luku,10);
		else 
			strcpy(luku,"n/a");
		PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"background tile: ",PD_TAUSTABUFFER,2,24,65);
		PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,luku,PD_TAUSTABUFFER,150,24,65);

	}

	if (edit_tila == EDIT_SPRITE)
	{	
		PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"sprite: ",PD_TAUSTABUFFER,2,14,65);
		if (focussprite != 255)
			PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,protot[focussprite].nimi,PD_TAUSTABUFFER,90,14,65);		
		else
			PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"n/a",PD_TAUSTABUFFER,90,14,65);	
	}

	vali += PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,kartta->nimi,PD_TAUSTABUFFER,2+vali,2,65);

	PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,viesti,PD_TAUSTABUFFER,320,2,65);

	if (edit_kerros == EDIT_KERROS_SEINAT)
		PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"layer: both",PD_TAUSTABUFFER,320,12,65);
	if (edit_kerros == EDIT_KERROS_TAUSTA)
		PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"layer: only background",PD_TAUSTABUFFER,320,12,65);
	if (edit_kerros == EDIT_KERROS_VAIN_SEINAT)
		PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"layer: only foreground",PD_TAUSTABUFFER,320,12,65);
	
	PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"f1=help, f2=map, f3=tiles, f4=sprites, f5=files, f6=save map, f7=map info, f8=tools, f9=log, esc=exit",PD_TAUSTABUFFER,2,RUUDUN_KORKEUS-10,75);

	//PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,tyohakemisto,PD_TAUSTABUFFER,2,RUUDUN_KORKEUS-25,65);
	PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,PK2Kartta::pk2_hakemisto,PD_TAUSTABUFFER,2,RUUDUN_KORKEUS-25,65);
	
	vali = PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"sprites: ",PD_TAUSTABUFFER,640,2,65);
	itoa(spriteja,luku,10);
	vali += PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,luku,PD_TAUSTABUFFER,640+vali,2,65);
	vali += PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,"/",PD_TAUSTABUFFER,640+vali,2,65);
	itoa(MAX_SPRITEJA,luku,10);
	vali += PisteDraw_Font_Kirjoita_Lapinakyva(fontti1,luku,PD_TAUSTABUFFER,640+vali,2,65);
	/*
	if (Level_Editor_Nappi("help (f1)",20,RUUDUN_KORKEUS-20))
		menut[MENU_HELP].piilota = !menut[MENU_HELP].piilota;

	if (Level_Editor_Nappi("map (f2)",110,RUUDUN_KORKEUS-20))
		menut[MENU_KARTTA].piilota = !menut[MENU_KARTTA].piilota;
*/
	return 0;
}

int Level_Editor_Piirra_Kursori(void)
{
	int x,y;
	x = (hiiri_x/32)*32;
	y = (hiiri_y/32)*32;

	if (x>RUUDUN_LEVEYS-32)
		x=RUUDUN_LEVEYS-32;
	if (y>RUUDUN_KORKEUS-32)
		y=RUUDUN_KORKEUS-32;
		
	int px = ((edit_palikka%10)*32);
	int py = ((edit_palikka/10)*32);
		
	if (edit_tila == EDIT_KARTTA)
	{
		Level_Editor_Piirra_Nelio(x+1, y+1, x+33, y+33, 0);
		PisteDraw_Buffer_Flip_Nopea(kartta->palikat_buffer,PD_TAUSTABUFFER, x, y, px, py, px+32, py+32);
		Level_Editor_Piirra_Nelio(x, y, x+32, y+32, 57);
	}

	if (edit_tila == EDIT_SPRITE)
	{
		protot[proto_valittu].Piirra(x+16-protot[proto_valittu].leveys/2,y-protot[proto_valittu].korkeus+32,0);
		//protot[proto_valittu].Piirra(x,y-protot[proto_valittu].korkeus+32,0);
	}
	
	PisteDraw_Buffer_Flip_Nopea(kuva_editori,PD_TAUSTABUFFER,hiiri_x, hiiri_y, 1, 33, 19, 51);

	return 0;
}

int Level_Editor_Piirra(void)
{
	int kytkin_anim;
	
	if (!animaatio_paalla)
	{
		if (palikka_animaatio < 64)
			kytkin_anim = palikka_animaatio;
		else
			kytkin_anim = KYTKIN_ALOITUSARVO-64;

		PK2Kartta_Animoi(degree,palikka_animaatio/20,KYTKIN_ALOITUSARVO-kytkin_anim,KYTKIN_ALOITUSARVO-kytkin_anim,KYTKIN_ALOITUSARVO-kytkin_anim,true);
	}
	else
		PK2Kartta_Animoi(degree,palikka_animaatio/20,0,0,0,false);

	palikka_animaatio = 1 + palikka_animaatio%99;//100

	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,37);

	//if (kartta_ladattu)
   //PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,0,0);

	switch(kartta->tausta)
	{
	case TAUSTA_STAATTINEN				: PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,0,0);
										  break;
	case TAUSTA_PALLARX_HORI			: PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,0,0);
										  PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,640,0);
										  break;
	case TAUSTA_PALLARX_VERT			: PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,0,0);
										  PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,0,480);
										  break;
	case TAUSTA_PALLARX_VERT_JA_HORI	: PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,0,0);
										  PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,640,0);
										  PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,0,480);
										  PisteDraw_Buffer_Flip_Nopea(kartta->taustakuva_buffer,PD_TAUSTABUFFER,640,480);
										  break;
	default								: break;
	}

	Level_Editor_Piirra_Kartta();

	Level_Editor_Menut_Piirra();

	Level_Editor_Piirra_Infoa();

	Level_Editor_Leikepoyta_Piirra();

	if (leikepoyta_lista.nakyva)
		Level_Editor_Lista(leikepoyta_lista);

	Level_Editor_Piirra_Kursori();

	if (aktiivinen_menu == MENU_EI)
		Level_Editor_Edit_Kartta();
	
	PisteWait_Wait(10);

	PisteDraw_Paivita_Naytto();

	PisteWait_Start();

	//Level_Editor_Loki_Tallenna("Piirtorutiinit on suoritettu.");

	return 0;
}

/* MUUT RUTIINIT -------------------------------------------------------*/


int Level_Editor_Menut(void)
{
	
//	aktiivinen_menu = MENU_EI;
	bool aktiivisia = false;

	//if (PisteInput_Lue_Nappaimisto())
	//	aktiivinen_menu 

	for (int i=0; i < MAX_MENUJA; i++)
	{
		if (PisteInput_Keydown(menut[i].pika) && key_delay == 0)
		{
			if (menut[i].piilota)
				menut[i].piilota = false;
			else
				menut[i].piilota = true;

			key_delay = 15;
		}
		
		if (menut[i].x < 0)
			menut[i].x = 0;

		if (menut[i].x + menut[i].leveys > RUUDUN_LEVEYS)
			menut[i].x -= (menut[i].x + menut[i].leveys) - RUUDUN_LEVEYS;

		if (menut[i].y < 0)
			menut[i].y = 0;

		if (menut[i].y + menut[i].korkeus+16 > RUUDUN_KORKEUS)
			menut[i].y -= (menut[i].y + 16 + menut[i].korkeus) - RUUDUN_KORKEUS;		

		if (aktiivinen_menu == i)
		{
			/*
			if (PisteInput_Hiiri_Vasen())
				liikuta_ikkunaa = true;
			else
				liikuta_ikkunaa = false;*/

			if ((hiiri_x > menut[i].x) && (hiiri_x < menut[i].x + menut[i].leveys) &&
				(hiiri_y > menut[i].y) && (hiiri_y < menut[i].y + 16) && PisteInput_Hiiri_Vasen())
			{
				//menut[i].x += PisteInput_Hiiri_X(0);
				//menut[i].y += PisteInput_Hiiri_Y(0);
				liikuta_ikkunaa = true;
				//menut[i].x 
			}

			if (!PisteInput_Hiiri_Vasen())
				liikuta_ikkunaa = false;

			if (liikuta_ikkunaa)
			{
				menut[i].x += PisteInput_Hiiri_X(0);
				menut[i].y += PisteInput_Hiiri_Y(0);
				aktiivisia = true;
				//liikuta_ikkunaa = true;
				//menut[i].x 
			}			

			if ((hiiri_x > menut[i].x+menut[i].leveys-10) && (hiiri_x < menut[i].x + menut[i].leveys) &&
				(hiiri_y > menut[i].y) && (hiiri_y < menut[i].y + 16) && PisteInput_Hiiri_Vasen())
			{
				menut[i].piilota = true;
				key_delay = 15;
			}
		
		}

		if ((hiiri_x > menut[i].x) && (hiiri_x < menut[i].x + menut[i].leveys) &&
			(hiiri_y > menut[i].y) && (hiiri_y < menut[i].y + menut[i].korkeus+16) && !menut[i].piilota
			&& (aktiivinen_menu == MENU_EI || i == aktiivinen_menu))
		{
			aktiivinen_menu = i;
			aktiivisia = true;
		}

	}

	if (!aktiivisia)
	{
		aktiivinen_menu = MENU_EI;
		liikuta_ikkunaa = false;
	}

	if (aktiivinen_menu != MENU_EI)
	{
		edit_tila = EDIT_MENU;
	}
	else
	{
		if (proto_valittu == MAX_PROTOTYYPPEJA)
			edit_tila = EDIT_KARTTA;
		else
			edit_tila = EDIT_SPRITE;

	}

	return 0;
}

int Level_Editor_Kursori(void)
{
	hiiri_x = PisteInput_Hiiri_X(hiiri_x);
	hiiri_y = PisteInput_Hiiri_Y(hiiri_y);
	
	if (hiiri_x > RUUDUN_LEVEYS)
		hiiri_x = RUUDUN_LEVEYS;

	if (hiiri_x < 0)
		hiiri_x = 0;

	if (hiiri_y > RUUDUN_KORKEUS)
		hiiri_y = RUUDUN_KORKEUS;

	if (hiiri_y < 0)
		hiiri_y = 0;

	return 0;
}

int Level_Editor_Reunat(void)
{

	if (hiiri_x < 2)
		kartta_x -= 1;

	if (hiiri_x > RUUDUN_LEVEYS-2)
		kartta_x += 1;	

	if (hiiri_y < 2)
		kartta_y -= 1;

	if (hiiri_y > RUUDUN_KORKEUS-2)
		kartta_y += 1;	

	if (!editoi_tekstia)
	{
		if (PisteInput_Keydown(DIK_LEFT) && key_delay == 0)
		{
			kartta_x--;
			key_delay = 5;
		}

		if (PisteInput_Keydown(DIK_RIGHT) && key_delay == 0)
		{
			kartta_x++;
			key_delay = 5;
		}
	}

	if (PisteInput_Keydown(DIK_UP) && key_delay == 0)
	{
		kartta_y--;
		key_delay = 5;
	}

	if (PisteInput_Keydown(DIK_DOWN) && key_delay == 0)
	{
		kartta_y++;
		key_delay = 5;
	}

	if (kartta_x < 0)
		kartta_x = 0;

	if (kartta_x > PK2KARTTA_KARTTA_LEVEYS  - RUUDUN_LEVEYS/32)
		kartta_x = PK2KARTTA_KARTTA_LEVEYS  - RUUDUN_LEVEYS/32;

	if (kartta_y < 0)
		kartta_y = 0;

	if (kartta_y > PK2KARTTA_KARTTA_KORKEUS - RUUDUN_KORKEUS/32)
		kartta_y = PK2KARTTA_KARTTA_KORKEUS - RUUDUN_KORKEUS/32;

	return 0;
}

int Level_Editor_Main(void)
{
	if (window_closed)
		return(0);

	/* HAETAAN NÄPPÄIMISTÖN, HIIREN JA PELIOHJAINTEN TÄMÄNHETKISET TILAT */

	// Näppäimistö 
	if (!PisteInput_Hae_Nappaimet())		//Haetaan näppäinten tilat
		DirectX_virhe = true;
	
	// Hiirulainen
	if (!PisteInput_Hae_Hiiri())			//Haetaan hiiren tila
		DirectX_virhe = true;	

	//editoi_tekstia = false;

	Level_Editor_Kursori();

	Level_Editor_Reunat();

	editoi_tekstia = false;

	Level_Editor_Piirra();

	Level_Editor_Menut();

	if (key_delay > 0)
		key_delay--;

	degree = 1 + degree%359;

	if (PisteInput_Keydown(DIK_RALT))
	{
		if (!aseta_leikepoyta_alue)
		{
			leikepoyta_alue.left = (hiiri_x/32)+kartta_x;
			leikepoyta_alue.top  = (hiiri_y/32)+kartta_y;
			aseta_leikepoyta_alue = true;
		}

		leikepoyta_alue.right  = (hiiri_x/32)+kartta_x;
		leikepoyta_alue.bottom = (hiiri_y/32)+kartta_y;

		long vaihto;

		if (leikepoyta_alue.right < leikepoyta_alue.left)
		{
			vaihto = leikepoyta_alue.right;
			leikepoyta_alue.right = leikepoyta_alue.left;
			leikepoyta_alue.left = vaihto;
		}

		if (leikepoyta_alue.bottom < leikepoyta_alue.top)
		{
			vaihto = leikepoyta_alue.top;
			leikepoyta_alue.top = leikepoyta_alue.bottom;
			leikepoyta_alue.bottom = vaihto;
		}

	}
	else
	{
		if (aseta_leikepoyta_alue)
		{
			aseta_leikepoyta_alue = false;
			//Level_Editor_Lista_Nayta(leikepoyta_lista,hiiri_x,hiiri_y);
		}
	}


	if (key_delay == 0 && !editoi_tekstia)
	{
		if (PisteInput_Keydown(DIK_LCONTROL))
		{
			//Level_Editor_Kartta_Varmista();
			
			if (PisteInput_Keydown(DIK_C))
			{			
			
				Level_Editor_Leikepoyta_Kopioi(leikepoyta_alue);
				key_delay = 20;
			}
			if (PisteInput_Keydown(DIK_V))
			{			
			
				Level_Editor_Leikepoyta_Liita(leikepoyta_alue);
				key_delay = 20;
			}
			if (PisteInput_Keydown(DIK_X))
			{			
			
				Level_Editor_Leikepoyta_Leikkaa(leikepoyta_alue);
				key_delay = 20;
			}
			if (PisteInput_Keydown(DIK_B))
			{			
				RECT temp = {hiiri_x/32+kartta_x,hiiri_y/32+kartta_y,hiiri_x/32+kartta_x,hiiri_y/32+kartta_y};
				Level_Editor_Leikepoyta_Liita_Koko(temp);
				key_delay = 20;
			}
		}
/*		
		if (PisteInput_Keydown(DIK_F9))
		{
			Level_Editor_Kartta_Varmista();
			Level_Editor_Prototyyppi_Tyhjenna();
			key_delay = 20;
		}
/*
		if (PisteInput_Keydown(DIK_F8))
		{
			Level_Editor_Kartta_Varmista();
			Level_Editor_Kartta_Alusta();
			key_delay = 20;
		}
*/

		if (PisteInput_Keydown(DIK_S))
		{
			Level_Editor_Kartta_Tallenna();
			key_delay = 20;
		}

		if (PisteInput_Keydown(DIK_A))
		{
			animaatio_paalla = !animaatio_paalla;
			key_delay = 20;
		}

		if (PisteInput_Keydown(DIK_U)) 
		{
			Level_Editor_Kartta_Undo();
			Level_Editor_Kartta_Paivita();
			key_delay = 20;
		}

		if (PisteInput_Keydown(DIK_RSHIFT))
		{
			switch(edit_kerros)
			{
			case EDIT_KERROS_SEINAT:		edit_kerros = EDIT_KERROS_VAIN_SEINAT;break;
			case EDIT_KERROS_VAIN_SEINAT:	edit_kerros = EDIT_KERROS_TAUSTA;break;
			case EDIT_KERROS_TAUSTA:		edit_kerros = EDIT_KERROS_SEINAT;break;
			default:						break;
			}

			key_delay = 20;
		}

	}
	
	if (exit_editor) 
	{
		exit_editor = true;
	}

	if (exit_editor && PisteDraw_Fade_Paletti_Valmis())
	{
		SendMessage(ikkunan_kahva, WM_CLOSE,0,0);
		window_closed = true;
	}

	return 0;
}


int Level_Editor_Quit(void)
{

	//delete kartta;
	//delete undo;
	
	PisteDraw_Lopeta();
	PisteInput_Lopeta();

	if (DirectX_virhe)
	{
		strcpy(virheviesti,PisteDraw_Virheilmoitus());
		MessageBox(ikkunan_kahva, virheviesti,"PK2 LevelEditor", MB_OK | MB_ICONEXCLAMATION);
	}

	Level_Editor_Asetukset_Tallenna();

	return 0;
}


/* IKKUNA -------------------------------------------------------------------------------------*/

// Seuraa ikkuna-koodia. Allaoleva on ikkunan "tapahtumakuuntelija".


LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	PAINTSTRUCT		ps;		
	HDC				hdc;	
	char			backupfile[_MAX_PATH];


	switch(msg)
	{	
		case WM_ACTIVATE: 
	    {
			if (((BOOL) HIWORD(wparam) != 0 ||
				 LOWORD(wparam) == WA_INACTIVE) && !exit_editor) {
				
				if (kartta != NULL) {
					strcpy(karttatiedosto,"backup.map");
					/*
					strcpy(backupfile,"");
					strcpy(backupfile,editor_hakemisto);
					strcat(backupfile,"\\backup2.mop");
					kartta->Tallenna(backupfile);
					*/
					Level_Editor_Kartta_Tallenna();
					PostQuitMessage(0);
					window_closed = true;
				}
			}

			if (((BOOL) HIWORD(wparam) != 0 ||
				 LOWORD(wparam) == WA_INACTIVE) && exit_editor) {
				
				if (kartta != NULL) {
					strcpy(backupfile,"");
					strcpy(backupfile,editor_hakemisto);
					strcat(backupfile,"\\backup.mop");
					kartta->Tallenna(backupfile);
					PostQuitMessage(0);
					window_closed = true;
				}
			}
		}

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

			PostQuitMessage(0);
			return(0);
		}	break;

		default:break;

    } 

	return (DefWindowProc(hwnd, msg, wparam, lparam));

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
	winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);


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

	Level_Editor_Init();

	ShowCursor(FALSE);

	while(!DirectX_virhe)
	{
    
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
	   
			if (msg.message == WM_QUIT)
				break;
	   
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		} 
    
		Level_Editor_Main();
	} 

	Level_Editor_Quit();

	ShowCursor(TRUE);

	return(msg.wParam);
} 

