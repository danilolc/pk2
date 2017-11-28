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
//-_MAX_PATH is now PATH_SIZE

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

//KARTTA
PK2Kartta *kartta;
char seuraava_kartta[_MAX_PATH];

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
