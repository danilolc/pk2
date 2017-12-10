//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################

#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <inttypes.h>
#include <fstream>
#include <cstdlib>
#include <cstdio>

#include "map.h"
#include "PisteDraw.h"
#include "PisteUtils.h"

using namespace std;

double *kartta_cos_table;
double *kartta_sin_table;

int aste,
	vesiaste = 0,
	animaatio,
	ajastin1,
	ajastin2,
	ajastin3,
	avaimet;

int ruudun_leveys_palikoina  = 21,
	ruudun_korkeus_palikoina = 16;

struct PK2KARTTA{ // Vanha versio 0.1
	char		versio[8];
	char		nimi[40];
	BYTE		taustakuva;
	BYTE		musiikki;
	BYTE		kartta [640*224];
	BYTE		palikat[320*256];
	BYTE		extrat [640*480];
};

bool PK2Kartta_Onko_File(char *filename){
	if(PisteUtils_FindImage(filename) == NULL){
		printf("PK2MAP - asked about non-existing file: %s\n", filename);
		PisteUtils_Lower(filename);
		return PisteUtils_FindImage(filename) != NULL;
	}
	return true;
}

void PK2Kartta_Cos_Sin(double *cost, double *sint){
	kartta_cos_table = cost;
	kartta_sin_table = sint;
}

void PK2Kartta_Animoi(int degree, int anim, int aika1, int aika2, int aika3, bool keys){
	aste = degree;
	animaatio = anim;
	ajastin1 = aika1;
	ajastin2 = aika2;
	ajastin3 = aika3;
	avaimet  = keys;
}

void PK2Kartta_Aseta_Ruudun_Mitat(int leveys, int korkeus){
	ruudun_leveys_palikoina  = leveys/32+1;
	ruudun_korkeus_palikoina = korkeus/32+1;
}

char PK2Kartta::pk2_hakemisto[256] = "";

PK2Kartta::PK2Kartta(){

	this->palikat_buffer = -1;
	this->taustakuva_buffer = -1;
	this->palikat_vesi_buffer = -1;

	strcpy(this->versio, PK2KARTTA_VIIMEISIN_VERSIO);
	strcpy(this->palikka_bmp,"blox.bmp");
	strcpy(this->taustakuva, "default.bmp");
	strcpy(this->musiikki,   "default.xm");

	strcpy(this->nimi,  "untitled");
	strcpy(this->tekija,"unknown");

	this->jakso		= 0;
	this->ilma		= ILMA_NORMAALI;
	this->kytkin1_aika = KYTKIN_ALOITUSARVO;
	this->kytkin2_aika = KYTKIN_ALOITUSARVO;
	this->kytkin3_aika = KYTKIN_ALOITUSARVO;
	this->pelaaja_sprite = 0;
	this->aika		= 0;
	this->extra		= PK2KARTTA_EXTRA_EI;
	this->tausta	= TAUSTA_STAATTINEN;

	this->x = 0;
	this->y = 0;
	this->ikoni = 0;

	memset(this->taustat, 255, sizeof(taustat));
	memset(this->seinat , 255, sizeof(seinat));
	memset(this->spritet, 255, sizeof(spritet));
	memset(this->reunat,  0,   sizeof(reunat));

	for (int i=0;i<PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA;i++)
		strcpy(this->protot[i],"");

	this->taustakuva_buffer = PisteDraw2_Image_New(640,480);
	this->palikat_buffer = PisteDraw2_Image_New(PK2KARTTA_BLOCK_PALETTI_LEVEYS,PK2KARTTA_BLOCK_PALETTI_KORKEUS);
	this->palikat_vesi_buffer = PisteDraw2_Image_New(PK2KARTTA_BLOCK_PALETTI_LEVEYS,32); //water

	PisteDraw2_ImageFill(this->taustakuva_buffer,255);
	PisteDraw2_ImageFill(this->palikat_buffer,255);
	PisteDraw2_ImageFill(this->palikat_buffer,255);
}

PK2Kartta::PK2Kartta(const PK2Kartta &kartta){

	this->palikat_buffer = -1;
	this->taustakuva_buffer = -1;
	this->palikat_vesi_buffer = -1;

	strcpy(this->versio,		kartta.versio);
	strcpy(this->palikka_bmp,	kartta.palikka_bmp);
	strcpy(this->taustakuva,	kartta.taustakuva);
	strcpy(this->musiikki,		kartta.musiikki);

	strcpy(this->nimi,			kartta.nimi);
	strcpy(this->tekija,		kartta.tekija);

	this->jakso			= kartta.jakso;
	this->ilma			= kartta.ilma;
	this->kytkin1_aika	= kartta.kytkin1_aika;
	this->kytkin2_aika	= kartta.kytkin2_aika;
	this->kytkin3_aika	= kartta.kytkin3_aika;
	this->pelaaja_sprite = kartta.pelaaja_sprite;
	this->aika			= kartta.aika;
	this->extra			= kartta.extra;
	this->tausta		= kartta.tausta;

	this->x				= kartta.x;
	this->y				= kartta.y;
	this->ikoni			= kartta.ikoni;

	int i;

	for (i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		this->taustat[i] = kartta.taustat[i];

	for (i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		this->seinat[i] = kartta.seinat[i];

	for (i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		this->spritet[i] = kartta.spritet[i];

	for (i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		this->reunat[i] = kartta.reunat[i];

	for (i=0;i<PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA;i++)
		strcpy(this->protot[i],kartta.protot[i]);


	PisteDraw2_Image_Copy(kartta.taustakuva_buffer,this->taustakuva_buffer);
	PisteDraw2_Image_Copy(kartta.palikat_buffer,this->palikat_buffer);
	PisteDraw2_Image_Copy(kartta.palikat_vesi_buffer,this->palikat_vesi_buffer);
}

PK2Kartta::~PK2Kartta(){
	PisteDraw2_Image_Delete(this->palikat_buffer);
	PisteDraw2_Image_Delete(this->taustakuva_buffer);
	PisteDraw2_Image_Delete(this->palikat_vesi_buffer);
}

RECT PK2Kartta::LaskeTallennusAlue(BYTE *lahde, BYTE *&kohde){
	int x,y;
	int kartan_vasen = PK2KARTTA_KARTTA_LEVEYS,//PK2KARTTA_KARTTA_LEVEYS/2,
		kartan_oikea = 0,
		kartan_yla	 = PK2KARTTA_KARTTA_KORKEUS,//PK2KARTTA_KARTTA_KORKEUS/2,
		kartan_ala	 = 0,
		kartan_korkeus = 0,
		kartan_leveys = 0;

	RECT rajat = {0,0,0,0};

	// tutkitaan kartan reunimmaiset tilet ja asetetaan reunat niiden mukaan
	for (y=0;y<PK2KARTTA_KARTTA_KORKEUS;y++) {
		for (x=0;x<PK2KARTTA_KARTTA_LEVEYS;x++)	{
			if (lahde[x+y*PK2KARTTA_KARTTA_LEVEYS] != 255) {
				if (x < kartan_vasen)
					kartan_vasen = x;
				if (y < kartan_yla)
					kartan_yla = y;
				if (x > kartan_oikea)
					kartan_oikea = x;
				if (y > kartan_ala)
					kartan_ala = y;
			}
		}
	}

	kartan_leveys = kartan_oikea - kartan_vasen;
	kartan_korkeus = kartan_ala - kartan_yla;

	// onko kartta tyhja?
	if (kartan_leveys < 0 || kartan_korkeus < 0) {
		kartan_vasen = kartan_yla = 0;
		kartan_ala = kartan_oikea = 1;
		kartan_leveys = kartan_oikea - kartan_vasen;
		kartan_korkeus = kartan_ala - kartan_yla;
	}

	kohde = new BYTE[kartan_leveys*kartan_korkeus];
	BYTE tile;

	for (y=0;y<kartan_korkeus;y++){
		for (x=0;x<kartan_leveys;x++){
			tile = lahde[(x+kartan_vasen)+(y+kartan_yla)*PK2KARTTA_KARTTA_LEVEYS];
			kohde[x+y*kartan_leveys] = tile;
			if (x==0 || y==0 || x==kartan_leveys-1 || y==kartan_korkeus-1)
				lahde[(x+kartan_vasen)+(y+kartan_yla)*PK2KARTTA_KARTTA_LEVEYS] = 104;
		}
	}

	rajat.left = kartan_vasen;
	rajat.top  = kartan_yla;
	rajat.right = kartan_oikea;
	rajat.bottom= kartan_ala;

	return rajat;
}

RECT PK2Kartta::LaskeTallennusAlue(BYTE *alue){
	DWORD x,y;
	DWORD kartan_vasen		= PK2KARTTA_KARTTA_LEVEYS,
		  kartan_oikea		= 0,
		  kartan_yla		= PK2KARTTA_KARTTA_KORKEUS,
		  kartan_ala		= 0;

	RECT rajat = {0,0,0,0};

	// tutkitaan kartan reunimmaiset tilet ja asetetaan reunat niiden mukaan
	for (y=0;y<PK2KARTTA_KARTTA_KORKEUS;y++) {
		for (x=0;x<PK2KARTTA_KARTTA_LEVEYS;x++)	{
			if (alue[x+y*PK2KARTTA_KARTTA_LEVEYS] != 255) {

				if (x < kartan_vasen) {
					kartan_vasen = x;
					//alue[x+y*PK2KARTTA_KARTTA_LEVEYS] = 101;
				}
				if (y < kartan_yla) {
					kartan_yla = y;
					//alue[x+y*PK2KARTTA_KARTTA_LEVEYS] = 102;
				}
				if (x > kartan_oikea) {
					kartan_oikea = x;
					//alue[x+y*PK2KARTTA_KARTTA_LEVEYS] = 103;
				}
				if (y > kartan_ala) {
					kartan_ala = y;
					//alue[x+y*PK2KARTTA_KARTTA_LEVEYS] = 104;
				}
			}
		}
	}

	// onko kartta tyhja?
	if (kartan_oikea < kartan_vasen || kartan_ala < kartan_yla) {
		kartan_vasen = 0;
		kartan_yla	 = 0;
		kartan_ala   = 1;
		kartan_oikea = 1;
	}

	rajat.left = kartan_vasen;
	rajat.top  = kartan_yla;
	rajat.right = kartan_oikea;
	rajat.bottom= kartan_ala;
	return rajat;
}

void PK2Kartta::LueTallennusAlue(BYTE *lahde, RECT alue, int kohde){
	int x,y;
	int kartan_vasen = alue.left,
		kartan_oikea = alue.right,
		kartan_yla	 = alue.top,
		kartan_ala	 = alue.bottom,
		kartan_korkeus = kartan_oikea - kartan_vasen,
		kartan_leveys = kartan_ala - kartan_yla;

	BYTE tile;
	if (&lahde != NULL && kohde != 0)	{
		for (y=0;y<kartan_korkeus;y++) {
			for (x=0;x<kartan_leveys;x++) {
				tile = lahde[x+y*kartan_leveys];
				if (kohde == 1)
					taustat[(x+kartan_vasen)+(y+kartan_yla)*PK2KARTTA_KARTTA_LEVEYS] = tile;
				if (kohde == 2)
					seinat[(x+kartan_vasen)+(y+kartan_yla)*PK2KARTTA_KARTTA_LEVEYS] = tile;
				if (kohde == 3)
					spritet[(x+kartan_vasen)+(y+kartan_yla)*PK2KARTTA_KARTTA_LEVEYS] = tile;
			}
		}
	}
}

int PK2Kartta::Tallenna(char *filename){
	char luku[8];
	DWORD i;

	ofstream *tiedosto = new ofstream(filename, ios::binary);

	strcpy(this->versio, PK2KARTTA_VIIMEISIN_VERSIO);

	//tiedosto->write ((char *)this, sizeof (*this));

	tiedosto->write(this->versio,		sizeof(versio));
	tiedosto->write(this->palikka_bmp,	sizeof(palikka_bmp));
	tiedosto->write(this->taustakuva,	sizeof(taustakuva));
	tiedosto->write(this->musiikki,		sizeof(musiikki));
	tiedosto->write(this->nimi,			sizeof(nimi));
	tiedosto->write(this->tekija,		sizeof(tekija));

	//itoa(this->jakso,luku,10);
	sprintf(luku, "%i", this->jakso);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	//itoa(this->ilma,luku,10);
	sprintf(luku, "%i", this->ilma);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	//itoa(this->kytkin1_aika,luku,10);
	sprintf(luku, "%" PRIu32, this->kytkin1_aika);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	//itoa(this->kytkin2_aika,luku,10);
	sprintf(luku, "%" PRIu32, this->kytkin2_aika);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	//itoa(this->kytkin3_aika,luku,10);
	sprintf(luku, "%" PRIu32, this->kytkin3_aika);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	//itoa(this->aika,luku,10);
	sprintf(luku, "%i", this->aika);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	//itoa(this->extra,luku,10);
	sprintf(luku, "%i", this->extra);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	//itoa(this->tausta,luku,10);
	sprintf(luku, "%i", this->tausta);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	//itoa(this->pelaaja_sprite,luku,10);
	sprintf(luku, "%i", this->pelaaja_sprite);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	//itoa(this->x,luku,10);
	sprintf(luku, "%i", this->x);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	//itoa(this->y,luku,10);
	sprintf(luku, "%i", this->y);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	//itoa(this->ikoni,luku,10);
	sprintf(luku, "%i", this->ikoni);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	int protoja = 0;

	for (i=0;i<PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA;i++)
		if (strlen(this->protot[i]) > 0)
			protoja++;

	//itoa(protoja,luku,10);
	sprintf(luku, "%i", protoja);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	for (i=0;i<PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA;i++)
		if (strlen(this->protot[i]) > 0)
			tiedosto->write(protot[i],sizeof(protot[i]));

	// laske alue

	//BYTE *alue_taustat = NULL, *alue_seinat = NULL, *alue_spritet = NULL;
	RECT alue = {0,0,0,0};
	DWORD /*koko, aloituskohta,*/ leveys, korkeus, x, y;
	DWORD aloitus_x,aloitus_y;
	char tile[1];

	// taustat
	alue = LaskeTallennusAlue(this->taustat);
	leveys = alue.right - alue.left;
	korkeus = alue.bottom - alue.top;
	aloitus_x = alue.left;
	aloitus_y = alue.top;

	sprintf(luku, "%" PRIu32, aloitus_x);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	sprintf(luku, "%" PRIu32, aloitus_y);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	sprintf(luku, "%" PRIu32, leveys);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	sprintf(luku, "%" PRIu32, korkeus);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	for (y=aloitus_y;y<=aloitus_y+korkeus;y++) {	// Kirjoitetaan alue tiedostoon tile by tile
		for (x=aloitus_x;x<=aloitus_x+leveys;x++) {
			tile[0] = this->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS];
			tiedosto->write(tile, sizeof(tile));
		}
	}

	// seinat
	alue = LaskeTallennusAlue(this->seinat);
	leveys = alue.right - alue.left;
	korkeus = alue.bottom - alue.top;
	aloitus_x = alue.left;
	aloitus_y = alue.top;
	//ltoa(aloitus_x,luku,10);
	sprintf(luku, "%" PRIu32, aloitus_x);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	sprintf(luku, "%" PRIu32, aloitus_y);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	sprintf(luku, "%" PRIu32, leveys);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));

	sprintf(luku, "%" PRIu32, korkeus);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku)); //TODO - MAKE A FUNCTION TO DO THIS
	for (y=aloitus_y;y<=aloitus_y+korkeus;y++) {	// Kirjoitetaan alue tiedostoon tile by tile
		for (x=aloitus_x;x<=aloitus_x+leveys;x++) {
			tile[0] = this->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS];
			tiedosto->write(tile, sizeof(tile));
		}
	}

	// spritet
	alue = LaskeTallennusAlue(this->spritet);
	leveys = alue.right - alue.left;
	korkeus = alue.bottom - alue.top;
	aloitus_x = alue.left;
	aloitus_y = alue.top;
	//ltoa(aloitus_x,luku,10);
	sprintf(luku, "%" PRIu32, aloitus_x);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));
	//ltoa(aloitus_y,luku,10);
	sprintf(luku, "%" PRIu32, aloitus_y);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));
	//ltoa(leveys,luku,10);
	sprintf(luku, "%" PRIu32, leveys);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));
	//ltoa(korkeus,luku,10);
	sprintf(luku, "%" PRIu32, korkeus);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));
	for (y=aloitus_y;y<=aloitus_y+korkeus;y++) {	// Kirjoitetaan alue tiedostoon tile by tile
		for (x=aloitus_x;x<=aloitus_x+leveys;x++) {
			tile[0] = this->spritet[x+y*PK2KARTTA_KARTTA_LEVEYS];
			tiedosto->write(tile, sizeof(tile));
		}
	}
	/*

	alue = LaskeTallennusAlue(this->seinat);
	koko = (alue.right - alue.left) * (alue.bottom - alue.top);
	aloituskohta = alue.left+alue.top*PK2KARTTA_KARTTA_LEVEYS;
	ltoa(aloituskohta,luku,10);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));
	ltoa(koko,luku,10);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));
	for (i=0;i<koko;i++){
		tile[0] = this->seinat[aloituskohta+i];
		tiedosto->write(tile, sizeof(tile));
	}

	alue = LaskeTallennusAlue(this->spritet);
	koko = (alue.right - alue.left) * (alue.bottom - alue.top);
	aloituskohta = alue.left+alue.top*PK2KARTTA_KARTTA_LEVEYS;
	ltoa(aloituskohta,luku,10);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));
	ltoa(koko,luku,10);
	tiedosto->write(luku, sizeof(luku));
	memset(luku, 0, sizeof(luku));
	for (i=0;i<koko;i++){
		tile[0] = this->spritet[aloituskohta+i];
		tiedosto->write(tile, sizeof(tile));
	}
	/*
	alue = LaskeTallennusAlue(this->seinat);
	koko = (alue.right - alue.left) * (alue.bottom - alue.top);
	ltoa(alue.left,luku,10); tiedosto->write(luku, sizeof(luku));
	ltoa(alue.right,luku,10); tiedosto->write(luku, sizeof(luku));
	ltoa(koko,luku,10); tiedosto->write(luku, sizeof(luku));
	for (i=0;i<koko;i++){
		tile = (char *)this->seinat[(alue.left+alue.top*PK2KARTTA_KARTTA_LEVEYS)+i];
		tiedosto->write(tile, sizeof(tile));
	}

	alue = LaskeTallennusAlue(this->spritet);
	koko = (alue.right - alue.left) * (alue.bottom - alue.top);
	ltoa(alue.left,luku,10); tiedosto->write(luku, sizeof(luku));
	ltoa(alue.right,luku,10); tiedosto->write(luku, sizeof(luku));
	ltoa(koko,luku,10); tiedosto->write(luku, sizeof(luku));
	for (i=0;i<koko;i++){
		tile = (char *)this->spritet[(alue.left+alue.top*PK2KARTTA_KARTTA_LEVEYS)+i];
		tiedosto->write(tile, sizeof(tile));
	}
	/*
	alue = LaskeTallennusAlue(this->seinat, *&alue_seinat);
	if (alue_seinat != NULL) {
		ltoa(alue.left,luku,10); tiedosto->write(luku, sizeof(luku));
		ltoa(alue.right,luku,10); tiedosto->write(luku, sizeof(luku));
		ltoa(alue.bottom,luku,10); tiedosto->write(luku, sizeof(luku));
		ltoa(alue.top,luku,10); tiedosto->write(luku, sizeof(luku));
		tiedosto->write(alue_seinat, sizeof(alue_seinat));
	}

	alue = LaskeTallennusAlue(this->taustat, *&alue_taustat);
	if (alue_taustat != NULL) {
		ltoa(alue.left,luku,10); tiedosto->write(luku, sizeof(luku));
		ltoa(alue.right,luku,10); tiedosto->write(luku, sizeof(luku));
		ltoa(alue.bottom,luku,10); tiedosto->write(luku, sizeof(luku));
		ltoa(alue.top,luku,10); tiedosto->write(luku, sizeof(luku));
		tiedosto->write(alue_taustat, sizeof(alue_taustat));
	}

	alue = LaskeTallennusAlue(this->spritet, *&alue_spritet);
	if (alue_spritet != NULL) {
		ltoa(alue.left,luku,10); tiedosto->write(luku, sizeof(luku));
		ltoa(alue.right,luku,10); tiedosto->write(luku, sizeof(luku));
		ltoa(alue.bottom,luku,10); tiedosto->write(luku, sizeof(luku));
		ltoa(alue.top,luku,10); tiedosto->write(luku, sizeof(luku));
		tiedosto->write(alue_spritet, sizeof(alue_spritet));
	}
	/*

	tiedosto->write(this->taustat,		sizeof(taustat));
	tiedosto->write(this->seinat,		sizeof(seinat));
	tiedosto->write(this->spritet,		sizeof(spritet));
	*/

	if (tiedosto->fail()){
		delete (tiedosto);
		return 1;
	}

	delete (tiedosto);

	return 0;
}

int PK2Kartta::Lataa(char *polku, char *nimi){
	char file[_MAX_PATH];
	strcpy(file,polku);
	strcat(file,nimi);

	ifstream *tiedosto = new ifstream(file, ios::binary);
	char versio[8] = "\0";

	if (tiedosto->fail()){
		delete (tiedosto);
		return 1;
	}

	tiedosto->read ((char *)versio, sizeof (versio));

	if (tiedosto->fail()){
		delete (tiedosto);
		return 1;
	}

	delete (tiedosto);

	int ok = 2;

	if (strcmp(versio,"1.3")==0){
		this->LataaVersio13(file);
		ok = 0;
	}
	if (strcmp(versio,"1.2")==0){
		this->LataaVersio12(file);
		ok = 0;
	}
	if (strcmp(versio,"1.1")==0){
		this->LataaVersio11(file);
		ok = 0;
	}
	if (strcmp(versio,"1.0")==0){
		this->LataaVersio10(file);
		ok = 0;
	}
	if (strcmp(versio,"0.1")==0){
		this->LataaVersio01(file);
		ok = 0;
	}

	Lataa_PalikkaPaletti(polku, this->palikka_bmp);
	Lataa_Taustakuva(polku,this->taustakuva);

	return(ok);
}

int PK2Kartta::Lataa_Pelkat_Tiedot(char *polku, char *nimi){
	char file[_MAX_PATH];
	strcpy(file,polku);
	strcat(file,nimi);

	ifstream *tiedosto = new ifstream(file, ios::binary);
	char versio[8] = "\0";

	if (tiedosto->fail())
	{
		delete (tiedosto);
		return 1;
	}

	tiedosto->read ((char *)versio, sizeof (versio));

	if (tiedosto->fail())
	{
		delete (tiedosto);
		return 1;
	}

	delete (tiedosto);

	if (strcmp(versio,"1.3")==0)
		this->LataaVersio13(file);

	if (strcmp(versio,"1.2")==0)
		this->LataaVersio12(file);

	if (strcmp(versio,"1.1")==0)
		this->LataaVersio11(file);

	if (strcmp(versio,"1.0")==0)
		this->LataaVersio10(file);

	if (strcmp(versio,"0.1")==0)
		this->LataaVersio01(file);

	return(0);
}

int PK2Kartta::LataaVersio01(char *filename){

	FILE *tiedosto;

	PK2KARTTA kartta;

	if ((tiedosto = fopen(filename, "r")) == NULL){
		return(1);
	}

	fread(&kartta, sizeof(PK2KARTTA), 1, tiedosto);

	fclose(tiedosto);

	strcpy(this->versio, PK2KARTTA_VIIMEISIN_VERSIO);
	strcpy(this->palikka_bmp,"blox.bmp");
	strcpy(this->taustakuva, "default.bmp");
	strcpy(this->musiikki,   "default.xm");

	strcpy(this->nimi,  "v01");
	strcpy(this->tekija,"unknown");

	this->aika		= 0;
	this->extra		= 0;
	this->tausta	= kartta.taustakuva;

	for (int i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		this->seinat[i] = kartta.kartta[i%PK2KARTTA_KARTTA_LEVEYS + (i/PK2KARTTA_KARTTA_LEVEYS) * 640];

	memset(this->taustat,255, sizeof(taustat));

	memset(this->spritet,255, sizeof(spritet));

	return(0);
}
int PK2Kartta::LataaVersio10(char *filename){
	FILE *tiedosto;

	PK2Kartta *kartta = new PK2Kartta();

	if ((tiedosto = fopen(filename, "r")) == NULL)
	{
		return(1);
	}

	fread(kartta, sizeof(PK2Kartta), 1, tiedosto);

	fclose(tiedosto);

	strcpy(this->versio,		kartta->versio);
	strcpy(this->palikka_bmp,	kartta->palikka_bmp);
	strcpy(this->taustakuva,	kartta->taustakuva);
	strcpy(this->musiikki,		kartta->musiikki);

	strcpy(this->nimi,			kartta->nimi);
	strcpy(this->tekija,		kartta->tekija);

	this->aika			= kartta->aika;
	this->extra			= kartta->extra;
	this->tausta		= kartta->tausta;

	for (int i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		this->taustat[i] = kartta->taustat[i];

	for (int i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		this->seinat[i] = kartta->seinat[i];

	for (int i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		this->spritet[i] = kartta->spritet[i];


	//Lataa_PalikkaPaletti(kartta->palikka_bmp);
	//Lataa_Taustakuva(kartta->taustakuva);

	//delete kartta;

	return(0);
}
int PK2Kartta::LataaVersio11(char *filename){
	FILE *tiedosto;
	int virhe = 0;


	if ((tiedosto = fopen(filename, "r")) == NULL)
	{
		return(1);
	}

	memset(this->taustat, 255, sizeof(this->taustat));
	memset(this->seinat , 255, sizeof(this->seinat));
	memset(this->spritet, 255, sizeof(this->spritet));

	fread(this->versio,		sizeof(char),	5, tiedosto);
	fread(this->palikka_bmp,sizeof(char),	13, tiedosto);
	fread(this->taustakuva,	sizeof(char),	13, tiedosto);
	fread(this->musiikki,	sizeof(char),	13, tiedosto);
	fread(this->nimi,		sizeof(char),	40, tiedosto);
	fread(this->tekija,		sizeof(char),	40, tiedosto);
	fread(&this->aika,		sizeof(int),	1, tiedosto);
	fread(&this->extra,		sizeof(BYTE),	1, tiedosto);
	fread(&this->tausta,	sizeof(BYTE),	1, tiedosto);
	fread(this->taustat,	sizeof(taustat),1, tiedosto);
	if (fread(this->seinat,	sizeof(seinat),	1, tiedosto) != PK2KARTTA_KARTTA_KOKO)
		virhe = 2;
	fread(this->spritet,	sizeof(spritet),1, tiedosto);

	fclose(tiedosto);

	int i;

	for (i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		if (seinat[i] != 255)
			seinat[i] -= 50;

	for (i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		if (taustat[i] != 255)
			taustat[i] -= 50;

	for (i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		if (spritet[i] != 255)
			spritet[i] -= 50;

	//Lataa_PalikkaPaletti(this->palikka_bmp);
	//Lataa_Taustakuva(this->taustakuva);

	return (virhe);
}
int PK2Kartta::LataaVersio12(char *filename){

	ifstream *tiedosto = new ifstream(filename, ios::binary);
	char luku[8];

	if (tiedosto->fail())
	{
		delete (tiedosto);
		return 1;
	}

	memset(this->taustat, 255, sizeof(this->taustat));
	memset(this->seinat , 255, sizeof(this->seinat));
	memset(this->spritet, 255, sizeof(this->spritet));

	for (int i=0;i<PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA;i++)
		strcpy(this->protot[i],"");

	//tiedosto->read ((char *)this, sizeof (*this));
	tiedosto->read(this->versio,		sizeof(versio));
	tiedosto->read(this->palikka_bmp,	sizeof(palikka_bmp));
	tiedosto->read(this->taustakuva,	sizeof(taustakuva));
	tiedosto->read(this->musiikki,		sizeof(musiikki));
	tiedosto->read(this->nimi,			sizeof(nimi));
	tiedosto->read(this->tekija,		sizeof(tekija));

	tiedosto->read(luku, sizeof(luku));
	this->jakso = atoi(luku);

	tiedosto->read(luku, sizeof(luku));
	this->ilma = atoi(luku);

	tiedosto->read(luku, sizeof(luku));
	this->kytkin1_aika = atoi(luku);

	tiedosto->read(luku, sizeof(luku));
	this->kytkin2_aika = atoi(luku);

	tiedosto->read(luku, sizeof(luku));
	this->kytkin3_aika = atoi(luku);

	tiedosto->read(luku, sizeof(luku));
	this->aika = atoi(luku);

	tiedosto->read(luku, sizeof(luku));
	this->extra = atoi(luku);

	tiedosto->read(luku, sizeof(luku));
	this->tausta = atoi(luku);

	tiedosto->read(luku, sizeof(luku));
	this->pelaaja_sprite = atoi(luku);

	tiedosto->read((char*)this->taustat,		sizeof(taustat));
	tiedosto->read((char*)this->seinat,		sizeof(seinat));
	tiedosto->read((char*)this->spritet,		sizeof(spritet));

	for (int i=0;i<PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA;i++)
		tiedosto->read(this->protot[i],sizeof(protot[i]));

	if (tiedosto->fail())
	{
		delete (tiedosto);
		return 1;
	}

	delete (tiedosto);

	//Lataa_PalikkaPaletti(this->palikka_bmp);
	//Lataa_Taustakuva(this->taustakuva);

	return 0;
}
int PK2Kartta::LataaVersio13(char *filename){

	ifstream *tiedosto = new ifstream(filename, ios::binary);
	char luku[8];
	DWORD i;

	if (tiedosto->fail())
	{
		delete (tiedosto);
		return 1;
	}

	memset(this->taustat, 255, sizeof(this->taustat));
	memset(this->seinat , 255, sizeof(this->seinat));
	memset(this->spritet, 255, sizeof(this->spritet));

	for (i=0;i<PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA;i++)
		strcpy(this->protot[i],"");

	//tiedosto->read ((char *)this, sizeof (*this));
	tiedosto->read(this->versio,		sizeof(versio));
	tiedosto->read(this->palikka_bmp,	sizeof(palikka_bmp));
	tiedosto->read(this->taustakuva,	sizeof(taustakuva));
	tiedosto->read(this->musiikki,		sizeof(musiikki));
	tiedosto->read(this->nimi,			sizeof(nimi));
	tiedosto->read(this->tekija,		sizeof(tekija));

	tiedosto->read(luku, sizeof(luku));
	this->jakso = atoi(luku);
	memset(luku, 0, sizeof(luku));

	tiedosto->read(luku, sizeof(luku));
	this->ilma = atoi(luku);
	memset(luku, 0, sizeof(luku));

	tiedosto->read(luku, sizeof(luku));
	this->kytkin1_aika = atoi(luku);
	memset(luku, 0, sizeof(luku));

	tiedosto->read(luku, sizeof(luku));
	this->kytkin2_aika = atoi(luku);
	memset(luku, 0, sizeof(luku));

	tiedosto->read(luku, sizeof(luku));
	this->kytkin3_aika = atoi(luku);
	memset(luku, 0, sizeof(luku));

	tiedosto->read(luku, sizeof(luku));
	this->aika = atoi(luku);
	memset(luku, 0, sizeof(luku));

	tiedosto->read(luku, sizeof(luku));
	this->extra = atoi(luku);
	memset(luku, 0, sizeof(luku));

	tiedosto->read(luku, sizeof(luku));
	this->tausta = atoi(luku);
	memset(luku, 0, sizeof(luku));

	tiedosto->read(luku, sizeof(luku));
	this->pelaaja_sprite = atoi(luku);
	memset(luku, 0, sizeof(luku));

	tiedosto->read(luku, sizeof(luku));
	this->x = atoi(luku);
	memset(luku, 0, sizeof(luku));

	tiedosto->read(luku, sizeof(luku));
	this->y = atoi(luku);
	memset(luku, 0, sizeof(luku));

	tiedosto->read(luku, sizeof(luku));
	this->ikoni = atoi(luku);
	memset(luku, 0, sizeof(luku));

	DWORD lkm;
	tiedosto->read(luku, sizeof(luku));
	lkm = (int)atoi(luku);

	//for (i=0;i<PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA;i++)
	//	itoa(lkm,protot[i],10);//strcpy(protot[i],"");

	for (i=0;i<lkm/*PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA*/;i++)
		tiedosto->read(protot[i],sizeof(protot[i]));

	DWORD leveys, korkeus,
		  aloitus_x,aloitus_y,
		  x,y;
	char tile[1];

	// taustat
	tiedosto->read(luku, sizeof(luku)); aloitus_x = atol(luku); memset(luku, 0, sizeof(luku));
	tiedosto->read(luku, sizeof(luku)); aloitus_y = atol(luku); memset(luku, 0, sizeof(luku));
	tiedosto->read(luku, sizeof(luku)); leveys    = atol(luku); memset(luku, 0, sizeof(luku));
	tiedosto->read(luku, sizeof(luku)); korkeus   = atol(luku); memset(luku, 0, sizeof(luku));
	for (y=aloitus_y;y<=aloitus_y+korkeus;y++) {	// Luetaan alue tile by tile
		for (x=aloitus_x;x<=aloitus_x+leveys;x++) {
			tiedosto->read(tile, sizeof(tile));
			this->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS] = tile[0];
		}
	}

	// seinat
	tiedosto->read(luku, sizeof(luku)); aloitus_x = atol(luku); memset(luku, 0, sizeof(luku));
	tiedosto->read(luku, sizeof(luku)); aloitus_y = atol(luku); memset(luku, 0, sizeof(luku));
	tiedosto->read(luku, sizeof(luku)); leveys    = atol(luku); memset(luku, 0, sizeof(luku));
	tiedosto->read(luku, sizeof(luku)); korkeus   = atol(luku); memset(luku, 0, sizeof(luku));
	for (y=aloitus_y;y<=aloitus_y+korkeus;y++) {	// Luetaan alue tile by tile
		for (x=aloitus_x;x<=aloitus_x+leveys;x++) {
			tiedosto->read(tile, sizeof(tile));
			this->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS] = tile[0];
		}
	}

	//spritet
	tiedosto->read(luku, sizeof(luku)); aloitus_x = atol(luku); memset(luku, 0, sizeof(luku));
	tiedosto->read(luku, sizeof(luku)); aloitus_y = atol(luku); memset(luku, 0, sizeof(luku));
	tiedosto->read(luku, sizeof(luku)); leveys    = atol(luku); memset(luku, 0, sizeof(luku));
	tiedosto->read(luku, sizeof(luku)); korkeus   = atol(luku); memset(luku, 0, sizeof(luku));
	for (y=aloitus_y;y<=aloitus_y+korkeus;y++) {	// Luetaan alue tile by tile
		for (x=aloitus_x;x<=aloitus_x+leveys;x++) {
			tiedosto->read(tile, sizeof(tile));
			this->spritet[x+y*PK2KARTTA_KARTTA_LEVEYS] = tile[0];
		}
	}

	if (tiedosto->fail())
	{
		delete (tiedosto);
		return 1;
	}

	delete (tiedosto);

	//Lataa_PalikkaPaletti(this->palikka_bmp);
	//Lataa_Taustakuva(this->taustakuva);

	return 0;
}

void PK2Kartta::Tyhjenna(){
	strcpy(this->versio, PK2KARTTA_VIIMEISIN_VERSIO);
	strcpy(this->palikka_bmp,"blox.bmp");
	strcpy(this->taustakuva, "default.bmp");
	strcpy(this->musiikki,   "default.xm");

	strcpy(this->nimi,  "untitled");
	strcpy(this->tekija,"unknown");

	this->jakso			= 0;
	this->ilma			= ILMA_NORMAALI;
	this->kytkin1_aika	= KYTKIN_ALOITUSARVO;
	this->kytkin2_aika	= KYTKIN_ALOITUSARVO;
	this->kytkin3_aika	= KYTKIN_ALOITUSARVO;
	this->pelaaja_sprite = 0;
	this->aika		= 0;
	this->extra		= PK2KARTTA_EXTRA_EI;
	this->tausta	= PK2KARTTA_TAUSTAKUVA_EI;

	this->x = 0;
	this->y = 0;
	this->ikoni = 0;

	memset(this->taustat, 255, sizeof(taustat));
	memset(this->seinat,  255, sizeof(seinat));
	memset(this->spritet, 255, sizeof(spritet));

	for (int i=0;i<PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA;i++)
		strcpy(this->protot[i],"");

	//PisteDraw2_ImageFill(this->palikat_buffer,255);
	//PisteDraw2_ImageFill(this->taustakuva_buffer,255);
}

PK2Kartta &PK2Kartta::operator = (const PK2Kartta &kartta){
	if (this == &kartta) return *this;

	strcpy(this->versio,		kartta.versio);
	strcpy(this->palikka_bmp,	kartta.palikka_bmp);
	strcpy(this->taustakuva,	kartta.taustakuva);
	strcpy(this->musiikki,		kartta.musiikki);

	strcpy(this->nimi,			kartta.nimi);
	strcpy(this->tekija,		kartta.tekija);

	this->aika		= kartta.aika;
	this->extra		= kartta.extra;
	this->tausta	= kartta.tausta;

	int i;

	for (i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		this->seinat[i] = kartta.seinat[i];

	for (i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		this->taustat[i] = kartta.taustat[i];

	for (i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
		this->spritet[i] = kartta.spritet[i];

	PisteDraw2_Image_Copy(kartta.taustakuva_buffer,this->taustakuva_buffer);
	PisteDraw2_Image_Copy(kartta.palikat_buffer,this->palikat_buffer);

	return *this;
}

int PK2Kartta::Lataa_Taustakuva(char *polku, char *filename){
	int i;
	char file[_MAX_PATH];
	strcpy(file,"");
	strcpy(file,polku);

	for (i=0 ; filename[i]!='\0' ; i++)
		filename[i]=tolower(filename[i]);

	strcat(file,filename);

	if (!PK2Kartta_Onko_File(file)){
		strcpy(file,PK2Kartta::pk2_hakemisto);
		strcat(file,"gfx/scenery/");
		strcat(file,filename);
		if (!PK2Kartta_Onko_File(file)) return 1;
	}

	i = PisteDraw2_Image_Load(file,true);
	if(i == -1) return 2;
	PisteDraw2_Image_Copy(i,this->taustakuva_buffer);
	PisteDraw2_Image_Delete(i);

	strcpy(this->taustakuva,filename);

	BYTE *buffer = NULL;
	DWORD leveys;
	int x,y;
	int color;

	PisteDraw2_DrawImage_Start(taustakuva_buffer,*&buffer,(DWORD &)leveys);

	for (x=0;x<640;x++)
		for (y=0;y<480;y++)
		{
			color = buffer[x+y*leveys];

			if (color == 255)
				color--;

			buffer[x+y*leveys] = color;
		}

	PisteDraw2_DrawImage_End(taustakuva_buffer);

	return 0;
}

int PK2Kartta::Lataa_PalikkaPaletti(char *polku, char *filename){
	int i;
	int img;
	char file[_MAX_PATH];
	strcpy(file,"");
	strcpy(file,polku);

	for (i=0 ; filename[i]!='\0' ; i++)
		filename[i]=tolower(filename[i]); //TODO PisteUtils_Lower

	strcat(file,filename);

	if (!PK2Kartta_Onko_File(file)){
		strcpy(file,PK2Kartta::pk2_hakemisto);
		strcat(file,"gfx/tiles/");
		strcat(file,filename);
		if (!PK2Kartta_Onko_File(file))
			return 1;
	}

	img = PisteDraw2_Image_Load(file,false);
	if(img == -1) return 2;
	PisteDraw2_Image_Copy(img,this->palikat_buffer);
	PisteDraw2_Image_Delete(img);

	PisteDraw2_Image_Delete(this->palikat_vesi_buffer); //Delete last water buffer
	this->palikat_vesi_buffer = PisteDraw2_Image_Cut(this->palikat_buffer,0,416,320,32);

	strcpy(this->palikka_bmp,filename);
	return 0;
}

int PK2Kartta::Lataa_TaustaMusiikki(char *filename){

	return 0;
}

void PK2Kartta::Kopioi(PK2Kartta &kartta){
	if (this != &kartta){
		strcpy(this->versio,		kartta.versio);
		strcpy(this->palikka_bmp,	kartta.palikka_bmp);
		strcpy(this->taustakuva,	kartta.taustakuva);
		strcpy(this->musiikki,		kartta.musiikki);

		strcpy(this->nimi,			kartta.nimi);
		strcpy(this->tekija,		kartta.tekija);

		this->jakso			= kartta.jakso;
		this->ilma			= kartta.ilma;
		this->kytkin1_aika	= kartta.kytkin1_aika;
		this->kytkin2_aika	= kartta.kytkin2_aika;
		this->kytkin3_aika	= kartta.kytkin3_aika;
		this->pelaaja_sprite = kartta.pelaaja_sprite;
		this->aika		= kartta.aika;
		this->extra		= kartta.extra;
		this->tausta	= kartta.tausta;

		int i;

		for (i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
			this->seinat[i] = kartta.seinat[i];

		for (i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
			this->taustat[i] = kartta.taustat[i];

		for (i=0;i<PK2KARTTA_KARTTA_KOKO;i++)
			this->spritet[i] = kartta.spritet[i];

		for (i=0;i<PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA;i++)
			strcpy(this->protot[i],kartta.protot[i]);

		PisteDraw2_ImageFill(palikat_buffer,255);
		PisteDraw2_ImageFill(taustakuva_buffer,0);

		PisteDraw2_Image_Copy(kartta.taustakuva_buffer,this->taustakuva_buffer);
		PisteDraw2_Image_Copy(kartta.palikat_buffer,this->palikat_buffer);
		PisteDraw2_Image_Copy(kartta.palikat_vesi_buffer,this->palikat_vesi_buffer);
	}
}



/* Kartanpiirtorutiineja ----------------------------------------------------------------*/
//Anim Fire
void PK2Kartta::Animoi_Tuli(void){
	BYTE *buffer = NULL;
	DWORD leveys;
	int x,y;
	int color;

	PisteDraw2_DrawImage_Start(palikat_buffer,*&buffer,(DWORD &)leveys);

	for (x=128;x<160;x++)
		for (y=448;y<479;y++)
		{
			color = buffer[x+(y+1)*leveys];

			if (color != 255)
			{
				color %= 32;
				color = color - rand()%4;

				if (color < 0)
					color = 255;
				else
				{
					if (color > 21)
						color += 128;
					else
						color += 64;
				}
			}

			buffer[x+y*leveys] = color;
		}

	if (ajastin1 < 20)
	{
		for (x=128;x<160;x++)
			buffer[x+479*leveys] = rand()%15+144;
	}
	else
		for (x=128;x<160;x++)
			buffer[x+479*leveys] = 255;

	PisteDraw2_DrawImage_End(palikat_buffer);
}
//Anim
void PK2Kartta::Animoi_Vesiputous(void){
	BYTE *buffer = NULL;
	DWORD leveys;
	int x,y,plus;
	int color,color2;

	BYTE temp[32*32];

	PisteDraw2_DrawImage_Start(palikat_buffer,*&buffer,(DWORD &)leveys);

	for (x=32;x<64;x++)
		for (y=416;y<448;y++)
			temp[x-32+(y-416)*32] = buffer[x+y*leveys];

	color2 = (temp[0]/32)*32;	// mahdollistaa erivriset vesiputoukset

	for (x=32;x<64;x++)
	{
		plus = rand()%2+2;//...+1
		for (y=416;y<448;y++)
		{
			color = temp[x-32+(y-416)*32];

			if (color != 255)	// mahdollistaa eri leveyksiset vesiputoukset
			{
				color %= 32;
				if (color > 10)//20
					color--;
				if (rand()%40 == 1)
					color = 11+rand()%11;//15+rand()%8;//9+rand()%5;
				if (rand()%160 == 1)
					color = 30;
				buffer[x + (416+(y+plus)%32)*leveys] = color+color2;
			}
			else
				buffer[x + (416+(y+plus)%32)*leveys] = color;
		}
	}

	PisteDraw2_DrawImage_End(palikat_buffer);
}
//Anim
void PK2Kartta::Animoi_Vedenpinta(void){
	BYTE *buffer = NULL;
	DWORD leveys;
	int x,y;

	BYTE temp[32];

	PisteDraw2_DrawImage_Start(palikat_buffer,*&buffer,(DWORD &)leveys);

	for (y=416;y<448;y++)
		temp[y-416] = buffer[y*leveys];

	for (y=416;y<448;y++)
	{
		for (x=0;x<31;x++)
		{
			buffer[x+y*leveys] = buffer[x+1+y*leveys];
		}
	}

	for (y=416;y<448;y++)
		buffer[31+y*leveys] = temp[y-416];

	PisteDraw2_DrawImage_End(palikat_buffer);
}
//Anim water
void PK2Kartta::Animoi_Vesi(void){
	BYTE *buffer_lahde = NULL, *buffer_kohde = NULL;
	DWORD leveys_lahde, leveys_kohde;
	int x, y, color1, color2,
		d1 = vesiaste / 2, d2;
	int sini, cosi;
	int vx,vy;
	int i;


	PisteDraw2_DrawImage_Start(palikat_buffer,		*&buffer_kohde,(DWORD &)leveys_kohde);
	PisteDraw2_DrawImage_Start(palikat_vesi_buffer,*&buffer_lahde,(DWORD &)leveys_lahde);

	for (y=0;y<32;y++){
		d2 = d1;

		for (x=0;x<32;x++){
			sini = int((y+d2/2)*11.25)%360;
			cosi = int((x+d1/2)*11.25)%360;
			sini = (int)kartta_sin_table[sini];
			cosi = (int)kartta_cos_table[cosi];

			vy = (y+sini/11)%32;
			vx = (x+cosi/11)%32;

			if (vy < 0){
				vy = -vy;
				vy = 31-(vy%32);
			}

			if (vx < 0){
				vx= -vx;
				vx = 31-(vx%32);
			}

			color1 = buffer_lahde[64+vx+vy*leveys_lahde];
			buffer_lahde[32+x+y*leveys_lahde] = color1;
			d2 = 1 + d2 % 360;
		}

		d1 = 1 + d1 % 360;
	}

	int vy2;

	for (int p=2;p<5;p++){
		i = p*32;

		for (y=0;y<32;y++){
			//d2 = d1;
			vy = y*leveys_lahde;
			vy2 = (y+416)*leveys_kohde;

			for (x=0;x<32;x++){
				vx = x+vy;
				color1 = buffer_lahde[32+vx];
				color2 = buffer_lahde[ i+vx];
				buffer_kohde[i+x+vy2] = (color1 + color2*2) / 3;
			}
		}
	}
	PisteDraw2_DrawImage_End(palikat_buffer);
	PisteDraw2_DrawImage_End(palikat_vesi_buffer);
}

void PK2Kartta::Animoi_Virta_Ylos(void){
	BYTE *buffer = NULL;
	DWORD leveys;
	int x,y;

	BYTE temp[32];

	PisteDraw2_DrawImage_Start(palikat_buffer,*&buffer,(DWORD &)leveys);

	for (x=64;x<96;x++)
		temp[x-64] = buffer[x+448*leveys];

	for (x=64;x<96;x++)
	{
		for (y=448;y<479;y++)
		{
			buffer[x+y*leveys] = buffer[x+(y+1)*leveys];
		}
	}

	for (x=64;x<96;x++)
		buffer[x+479*leveys] = temp[x-64];

	PisteDraw2_DrawImage_End(palikat_buffer);
}

int PK2Kartta::Piirra_Taustat(int kamera_x, int kamera_y, bool editor){
	int palikka;
	int px = 0,
		py = 0,
		kartta_x = kamera_x/32,
		kartta_y = kamera_y/32;

	for (int x=-1;x<ruudun_leveys_palikoina;x++){
		for (int y=0;y<ruudun_korkeus_palikoina;y++){
			if (x + kartta_x < 0 || x + kartta_x > PK2KARTTA_KARTTA_LEVEYS) continue;
			if (y + kartta_y < 0 || y + kartta_y > PK2KARTTA_KARTTA_KORKEUS) continue;

			int i = x + kartta_x + (y + kartta_y)*PK2KARTTA_KARTTA_LEVEYS;
			if(i<0 || i >= sizeof(taustat)) continue; //Dont access a not allowed address

			palikka = taustat[i];

			if (palikka != 255){
				px = ((palikka%10)*32);
				py = ((palikka/10)*32);

				if (palikka == BLOCK_ANIM1 || palikka == BLOCK_ANIM2 || palikka == BLOCK_ANIM3 || palikka == BLOCK_ANIM4)
					px += animaatio * 32;

				PisteDraw2_Image_CutClip(palikat_buffer, x*32-(kamera_x%32), y*32-(kamera_y%32), px, py, px+32, py+32);
			}
		}
	}

	return 0;
}

int PK2Kartta::Piirra_Seinat(int kamera_x, int kamera_y, bool editor){
	int palikka;
	int px = 0,
		py = 0,
		ay = 0,
		ax = 0,
		by = 0, bx = 0,
		kartta_x = kamera_x/32,
		kartta_y = kamera_y/32;

	int ajastin1_y = 0,
		ajastin2_y = 0,
		ajastin3_x = 0;

	if (ajastin1 > 0){
		ajastin1_y = 64;

		if (ajastin1 < 64)
			ajastin1_y = ajastin1;

		if (ajastin1 > KYTKIN_ALOITUSARVO-64)
			ajastin1_y = KYTKIN_ALOITUSARVO - ajastin1;
	}

	if (ajastin2 > 0){
		ajastin2_y = 64;

		if (ajastin2 < 64)
			ajastin2_y = ajastin2;

		if (ajastin2 > KYTKIN_ALOITUSARVO-64)
			ajastin2_y = KYTKIN_ALOITUSARVO - ajastin2;
	}

	if (ajastin3 > 0){
		ajastin3_x = 64;

		if (ajastin3 < 64)
			ajastin3_x = ajastin3;

		if (ajastin3 > KYTKIN_ALOITUSARVO-64)
			ajastin3_x = KYTKIN_ALOITUSARVO - ajastin3;
	}


	for (int x=-1; x < ruudun_leveys_palikoina + 1; x++){
		for (int y=-1; y < ruudun_korkeus_palikoina + 1; y++){
			if (x + kartta_x < 0 || x + kartta_x > PK2KARTTA_KARTTA_LEVEYS) continue;
			if (y + kartta_y < 0 || y + kartta_y > PK2KARTTA_KARTTA_KORKEUS) continue;

			int i = x + kartta_x + (y + kartta_y)*PK2KARTTA_KARTTA_LEVEYS;
			if(i<0 || i >= sizeof(seinat)) continue; //Dont access a not allowed address

			palikka = seinat[i];

			if (palikka != 255 && !(!editor && palikka == BLOCK_ESTO_ALAS)){
				px = ((palikka%10)*32);
				py = ((palikka/10)*32);
				ay = 0;
				ax = 0;

				if (!editor){
					if (palikka == BLOCK_HISSI_VERT)
						ay = (int)kartta_sin_table[aste%360];

					if (palikka == BLOCK_HISSI_HORI)
						ax = (int)kartta_cos_table[aste%360];

					if (palikka == BLOCK_KYTKIN1)
						ay = ajastin1_y/2;

					if (palikka == BLOCK_KYTKIN2_YLOS)
						ay = -ajastin2_y/2;

					if (palikka == BLOCK_KYTKIN2_ALAS)
						ay = ajastin2_y/2;

					if (palikka == BLOCK_KYTKIN2)
						ay = ajastin2_y/2;

					if (palikka == BLOCK_KYTKIN3_OIKEALLE)
						ax = ajastin3_x/2;

					if (palikka == BLOCK_KYTKIN3_VASEMMALLE)
						ax = -ajastin3_x/2;

					if (palikka == BLOCK_KYTKIN3)
						ay = ajastin3_x/2;
				}

				if (palikka == BLOCK_ANIM1 || palikka == BLOCK_ANIM2 || palikka == BLOCK_ANIM3 || palikka == BLOCK_ANIM4)
					px += animaatio * 32;

				PisteDraw2_Image_CutClip(palikat_buffer, x*32-(kamera_x%32)+ax, y*32-(kamera_y%32)+ay, px, py, px+32, py+32);
			}
		}
	}

	if (vesiaste%2 == 0)
	{
		Animoi_Tuli();
		Animoi_Vesiputous();
		Animoi_Virta_Ylos();
		Animoi_Vedenpinta();
	}

	if (vesiaste%4 == 0)
	{
		Animoi_Vesi();
		PisteDraw2_RotatePalette(224,239);
	}

	vesiaste = 1 + vesiaste % 320;

	return 0;
}
