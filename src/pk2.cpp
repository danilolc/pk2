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
//#########################
//TODO
//-Music loop
//-Set panoramic audio

#include <sys/stat.h>
#include <fstream>
#include <cmath> //sin cos
#include <cstdio>
#include <ctime>
#include <cstdlib> //rand
#ifdef _WIN32
	#include <direct.h>
#else
	#include <unistd.h> //getcwd chdir
#endif

#include "PisteEngine.h"
#include "map.h"
#include "sprite.h"

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

/*
#ifndef _WIN32
void itoa(int n, char s[], int radix){
	sprintf(s, "%i", n);
}
void ltoa(long n, char s[], int radix){
	sprintf(s, "%i", (int)n);
}
#endif
*/

//#### Constants

const int MAX_ESINEITA = 4;
const int MAX_TALLENNUKSIA = 10;

enum { //Blocks
	BLOCK_TAUSTA,
	BLOCK_SEINA,
	BLOCK_MAKI_OIKEA_YLOS,
	BLOCK_MAKI_VASEN_YLOS,
	BLOCK_MAKI_OIKEA_ALAS,
	BLOCK_MAKI_VASEN_ALAS,
	BLOCK_MAKI_YLOS,
	BLOCK_MAKI_ALAS
};

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
	short int	ylos[32]; //WORD
	short int	alas[32];
	short int	vasemmalle[32];
	short int	oikealle[32];
};

const int MAX_ILMOITUKSENNAYTTOAIKA = 700;

//Sound
const int AANET_SAMPLERATE = 22050;

//#### Global Variables

int RUUDUN_LEVEYS  = SCREEN_WIDTH;
int RUUDUN_KORKEUS = SCREEN_HEIGHT;

int KARTANPIIRTO_LEVEYS     = 800;
int KARTANPIIRTO_KORKEUS    = 480;
bool RAJAA_KARTANPIIRTOALUE = true;

bool test_level = false;
bool dev_mode = false;

bool PK2_virhe = false;


bool lopeta_peli = false;

bool running = true;
bool unload = false;
bool taulut_laskettu = false;

//ASETUKSET
PK2SETTINGS settings;

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

//PELAAJA
int pelaaja_index = 0;
DWORD pisteet = 0;
DWORD piste_lisays = 0;
char pelaajan_nimi[20] = " ";

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

//TALLENNUKSET
PK2TALLENNUS tallennukset[MAX_TALLENNUKSIA];
int lataa_peli = -1;

int avaimia = 0;

DWORD aika = 0;
int sekunti = 0;
bool aikaraja = false;

int jaristys = 0;
int kytkin_tarina = 0;

int item_paneeli_x = 10;

int ilmoitus_ajastin = 0;
char ilmoitus[80] = " ";

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

bool peli_kesken = false;
bool siirry_kartasta_peliin = false;
bool siirry_pistelaskusta_karttaan = false;

int nakymattomyys = 0;

//#### Functions

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

//PK_Play_Sound
void PK_Soita_Aani(int aani, int voimakkuus, int x, int y, int freq, bool random_freq){
	if (aani > -1 && aanenvoimakkuus > 0 && voimakkuus > 0){
		if (x < kamera_x+RUUDUN_LEVEYS && x > kamera_x && y < kamera_y+RUUDUN_KORKEUS && y > kamera_y){
			voimakkuus = voimakkuus / (100 / aanenvoimakkuus);

			if (voimakkuus > 100)
				voimakkuus = 100;

			if (voimakkuus < 0)
				voimakkuus = 0;

			int pan = kamera_x + (RUUDUN_LEVEYS / 2) - x;
			pan *= -2;

			if (random_freq)
				freq = freq + rand()%4000 - rand()%2000;

			PisteSound_PlaySFX(aani,aanenvoimakkuus, pan, freq);
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

		PisteSound_PlaySFX(aani, aanenvoimakkuus, 0, freq);
	}
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

			if (settings.lapinakyvat_menutekstit)
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
						 DWORD kohde_x, DWORD kohde_y, int pros, BYTE vari){ //Substitute to PisteDraw2
	PD_RECT src = {lahde_x, lahde_y, lahde_leveys, lahde_korkeus};
	PD_RECT dst = {kohde_x, kohde_y, lahde_leveys, lahde_korkeus};
	PisteDraw2_Image_CutClipTransparent(lahde_buffer, src, dst, pros, vari);
	return 0;
}

void PK_Uusi_Ilmoitus(char *teksti){
	if (strcmp(teksti,ilmoitus) != 0 || ilmoitus_ajastin == 0){
		strcpy(ilmoitus,teksti);
		ilmoitus_ajastin = MAX_ILMOITUKSENNAYTTOAIKA;
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

			if (PisteUtils_CheckFile(testipolku))
				protot[seuraava_vapaa_proto].aanet[i] = PK_Prototyyppi_Lataa_Aani(aanipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);
			else{
			//if (protot[seuraava_vapaa_proto].aanet[i] == -1) {
				//strcpy(aanipolku,"sprites/");
				getcwd(aanipolku, _MAX_PATH);
				strcat(aanipolku,"/sprites/");

				strcpy(testipolku,aanipolku);
				strcat(testipolku,"/");
				strcat(testipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);

				if (PisteUtils_CheckFile(testipolku))
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

	if (kartta->Lataa(polku, nimi) == 1){
		printf("PK2    - Error loading map '%s' at '%s'\n", seuraava_kartta, polku);
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
//Clean items
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
				musiikin_voimakkuus = musiikin_max_voimakkuus;
				musiikin_voimakkuus_nyt = musiikin_max_voimakkuus-1;
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

//PK_Move_Sprite
int PK_Sprite_Liikuta(int i){

	PK2Sprite &sprite = spritet[i]; //address of sprite = address of spritet[i] (if change sprite, change spritet[i])

	if (i >= MAX_SPRITEJA || i < 0)
		return -1;

	if (!sprite.tyyppi)
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
		if (PisteInput_Keydown(kontrolli_juoksu))
			lisavauhti = false;

		/* ATTACK 1 */
		if (PisteInput_Keydown(kontrolli_hyokkays1) && sprite.lataus == 0 && sprite.ammus1 != -1) {
			sprite.hyokkays1 = sprite.tyyppi->hyokkays1_aika;
		}
		/* ATTACK 2 */
		else {
			if (PisteInput_Keydown(kontrolli_hyokkays2) && sprite.lataus == 0 && sprite.ammus2 != -1)
				sprite.hyokkays2 = sprite.tyyppi->hyokkays2_aika;
		}

		/* CROUCH */
		sprite.kyykky = false;

		if (PisteInput_Keydown(kontrolli_alas) && !sprite.alas)
		{
			sprite.kyykky = true;
			sprite_yla += sprite_korkeus/1.5;
		}

		double a_lisays = 0;

		/* NAVIGATING TO RIGHT */
		if (PisteInput_Keydown(kontrolli_oikealle))
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
		if (PisteInput_Keydown(kontrolli_vasemmalle))
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
			if (PisteInput_Keydown(kontrolli_hyppy))
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
			if (PisteInput_Keydown(kontrolli_hyppy) && sprite.hyppy_ajastin >= 150/*90+20*/ &&
				sprite.tyyppi->liitokyky)
				hidastus = true;
		}
		/* MOVING UP AND DOWN */
		else{ // if the player sprite-weight is 0 - like birds

			if (PisteInput_Keydown(kontrolli_hyppy))
				sprite_b -= 0.15;

			if (PisteInput_Keydown(kontrolli_alas))
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

			if (sprite2->tyyppi->este && sprite.tyyppi->tiletarkistus) //If there is a block sprite active
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

					PK_Palikka_Este(spritepalikka);

					if (!sprite.tyyppi->este){
						if (!sprite2->tyyppi->este_alas)
							spritepalikka.alas = BLOCK_TAUSTA;
						if (!sprite2->tyyppi->este_ylos)
							spritepalikka.ylos = BLOCK_TAUSTA;
						if (!sprite2->tyyppi->este_oikealle)
							spritepalikka.oikealle = BLOCK_TAUSTA;
						if (!sprite2->tyyppi->este_vasemmalle)
							spritepalikka.vasemmalle = BLOCK_TAUSTA;
					}

					if (sprite2->a > 0)
						spritepalikka.koodi = BLOCK_HISSI_HORI;

					if (sprite2->b > 0)
						spritepalikka.koodi = BLOCK_HISSI_VERT;

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

	if (PisteInput_Keydown(PI_B) && dev_mode)
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
		if (settings.tausta_spritet)
			PK_Spritet_Piirra_Taustat();

	if (settings.saa_efektit)
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

	if (settings.nayta_tavarat)
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
	if (fps_nayta){
		vali = PisteDraw2_Font_Write(fontti1,"fps: ",570,48);
		fps = Piste_GetFPS();
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


	if (skip_frame) Piste_IgnoreFrame();

	if (doublespeed) skip_frame = !skip_frame;
	else skip_frame = false;

	palikka_animaatio = 1 + palikka_animaatio % 34;

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

		if (PisteInput_Keydown(kontrolli_hyokkays1) || PisteInput_Keydown(kontrolli_hyokkays2) ||
			PisteInput_Keydown(kontrolli_hyppy) || PisteInput_Keydown(PI_RETURN))
			if (lopetusajastin > 2 && lopetusajastin < 600/*1900*/ && key_delay == 0)
				lopetusajastin = 2;

		if (lopetusajastin == 2)
		{
			PisteDraw2_FadeOut(PD_FADE_NORMAL);
			//musiikin_voimakkuus = 0;
		}
	}
	if (lopetusajastin == 1 && !PisteDraw2_IsFading()){
		if(test_level){
			lopeta_peli = true;
			PK_Quit();
		}
		else{
			if (jakso_lapaisty) pelin_seuraava_tila = TILA_PISTELASKU;
			else pelin_seuraava_tila = TILA_KARTTA;
		}
	}

	if (key_delay == 0){
		if (PisteInput_Keydown(kontrolli_kayta_esine) && spritet[pelaaja_index].energia > 0){
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

	if (key_delay == 0 && dev_mode){ //Debug
		if (PisteInput_Keydown(PI_Z)){
			kytkin1 = KYTKIN_ALOITUSARVO;
			kytkin2 = KYTKIN_ALOITUSARVO;
			kytkin3 = KYTKIN_ALOITUSARVO;
			key_delay = 10;
		}
		if (PisteInput_Keydown(PI_X)) {
			kytkin1 = 64;
			kytkin2 = 64;
			kytkin3 = 64;
			key_delay = 10;
		}
		if (PisteInput_Keydown(PI_T)) {
			doublespeed = !doublespeed;
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_G)){
			settings.lapinakyvat_objektit = !settings.lapinakyvat_objektit;
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
			settings.isFullScreen = !settings.isFullScreen;
			PisteDraw2_FullScreen(settings.isFullScreen);
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
		if (PisteInput_Keydown(PI_U)){
			spritet[pelaaja_index].b = -10;
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

void PK_Piirra_Kursori(int x,int y){
	PisteDraw2_Image_CutClip(kuva_peli,x,y,621,461,640,480);
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

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_mainmenu_return),100,430)){
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

			//PisteDraw2_Image_Clip(kuva_peli,jaksot[i].x-4,jaksot[i].y-4-30,1+(ikoni*27),452,27+(ikoni*27),478);
			PisteDraw2_Image_CutClip(kuva_peli,jaksot[i].x-9,jaksot[i].y-14,1+(ikoni*28),452,28+(ikoni*28),479);

			if (tyyppi==1) {
				//PisteDraw2_Image_Clip(kuva_peli,jaksot[i].x-30,jaksot[i].y-4,157,46,181,79);
				sinx = (int)(sin_table[degree%360]/2);
				cosy = (int)(cos_table[degree%360]/2);
				pekkaframe = 28*((degree%360)/120);
				PisteDraw2_Image_CutClip(kuva_peli,jaksot[i].x+sinx-12,jaksot[i].y-17+cosy,157+pekkaframe,46,181+pekkaframe,79);
			}

			paluu = PK_Piirra_Nuppi(jaksot[i].x-5, jaksot[i].y-10, tyyppi);

			// jos klikattu
			if (paluu == 2) {
				if (tyyppi < 2 || (dev_mode && tyyppi > 0)) {
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

	return 0;
}
int PK_Main_Kartta(){
	PK_Piirra_Kartta();

	degree = 1 + degree % 360;

	if (siirry_kartasta_peliin && !PisteDraw2_IsFading()) {
		pelin_seuraava_tila = TILA_PELI;
		//strcpy(seuraava_kartta,jaksot[i].tiedosto);
		peli_kesken = false;//true;
		//PisteDraw2_FadeIn(PD_FADE_NORMAL);
	}

	if (key_delay > 0)
		key_delay--;

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

	if (PK_DrawMenuOption(tekstit->Hae_Teksti(txt_score_screen_continue),100,430)){
		musiikin_voimakkuus = 0;
		siirry_pistelaskusta_karttaan = true;
		PisteDraw2_FadeOut(PD_FADE_SLOW);
		//pelin_seuraava_tila = TILA_KARTTA;
	}

	PK_Piirra_Kursori(hiiri_x,hiiri_y);

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

	if (siirry_pistelaskusta_karttaan && !PisteDraw2_IsFading()){
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


void PK_Start_Test(const char* arg){
	if (arg == NULL) return;

	char buffer[_MAX_PATH];
	int sepindex;

	strcpy(buffer, arg);
	for (sepindex = 0; sepindex < _MAX_PATH; sepindex++)
		if(buffer[sepindex]=='/') break;

	strcpy(episodi, buffer); episodi[sepindex] = '\0';
	strcpy(seuraava_kartta, buffer + sepindex + 1);

	printf("PK2    - testing episode '%s' level '%s'\n", episodi, seuraava_kartta);

	PK_Lataa_Lisainfot();
	peli_kesken = false;
	PK_Uusi_Peli();

	siirry_kartasta_peliin = true;
	musiikin_voimakkuus = 0;
	peli_kesken = false;
}
