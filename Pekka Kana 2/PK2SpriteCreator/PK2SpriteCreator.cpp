#define WIN32_LEAN_AND_MEAN  
#define INITGUID

#include "D:\DXSDK\Include\ddraw.h" 
#include "D:\Visual Studio\MyProjects\Pekka Kana 2\PK2Sprite.h"
#include <string.h>
#include <istream.h>
#include <stdlib.h>
#include <stdio.h>
#include <mmsystem.h>

typedef unsigned char  UCHAR;

void Animaatio_Uusi(PK2Sprite_Prototyyppi &proto, int anim_i, UCHAR *sekvenssi, bool looppi)
{
	UCHAR frame_i = 0;
	
	if (anim_i < SPRITE_MAX_ANIMAATIOITA)
	{
		proto.animaatiot[anim_i].frameja = 0;

		while (frame_i < ANIMAATIO_MAX_SEKVENSSEJA && sekvenssi[frame_i] != 0)
		{
			proto.animaatiot[anim_i].sekvenssi[frame_i] = sekvenssi[frame_i];
			proto.animaatiot[anim_i].frameja++;
			frame_i++;
		}
	
		proto.animaatiot[anim_i].looppi = looppi;
	}
}
/*
void Animaatio_Uusi(PK2Sprite_Prototyyppi &proto, int anim_i, char *sekvenssi, bool looppi)
{
	UCHAR frame_i = 0;
	int frame = 1;
	
	if (anim_i < SPRITE_MAX_ANIMAATIOITA)
	{
		proto.animaatiot[anim_i].frameja = 0;

		while (frame_i < ANIMAATIO_MAX_SEKVENSSEJA && frame != 0)
		{
			switch (sekvenssi[frame_i])
			{
			case '1' : frame = 1;break;
			case '2' : frame = 2;break;
			case '3' : frame = 3;break;
			case '4' : frame = 4;break;
			case '5' : frame = 5;break;
			case '6' : frame = 6;break;
			case '7' : frame = 7;break;
			case '8' : frame = 8;break;
			case '9' : frame = 9;break;
			case '0' : frame = 0;break;
			case ',' : frame = 10;break;
			default  : frame = 0;break;
			}

			proto.animaatiot[anim_i].sekvenssi[frame_i] = frame;
			proto.animaatiot[anim_i].frameja++;
			frame_i++;
		}
	
		proto.animaatiot[anim_i].looppi = looppi;
	}
}
*/
void luo_prototyyppi_kukko(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "rooster");
	strcpy(proto.muutos_sprite,"r_naked.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");//egg.spr
	strcpy(proto.ammus2_sprite,"");//doodle.spr
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"p_damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"p_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"p_layegg.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"p_doodle.wav");

	proto.AI[0]			= AI_KANA;
	proto.AI[1]			= AI_MUUTOS_JOS_ENERGIAA_ALLE_2;
	
	for (int ai=2;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.aani_frq		= 22050;
	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 3;
	proto.este			= false;
	proto.frameja		= 21;
	proto.frame_rate	= 4;
	proto.hyokkays1_aika= 30;
	proto.hyokkays2_aika= 60;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 59;
	proto.korkeus		= 59;//58
	proto.latausaika	= 0;//60
	proto.leveys		= 30;
	proto.max_hyppy		= 26;//24
	proto.max_nopeus	= 3;
	proto.paino			= 1;
	proto.random_frq	= true;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO + TUHOUTUMINEN_HOYHENET;
	proto.tarisee       = false;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.lapinakyvyys	= false;
	proto.hehkuu		= false;
	proto.tulitauko		= 0;
	proto.liitokyky		= true;
	proto.boss			= false;
	proto.bonus_aina	= false;

	UCHAR seq1[8] = {1,1,1,1,1,2,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[5] = {3,4,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[5] = {8,8,9,9,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[5] = {1,10,11,12,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {13,14,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[8] = {15,15,16,16,17,17,18,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {19,19,19,19,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);

	UCHAR seq9[5] = {20,20,21,21,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq9, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kukko_kynitty(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "naked rooster");
	strcpy(proto.muutos_sprite,"rooster.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	/*
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}*/

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "p_damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "p_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "p_layegg.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "p_doodle.wav");

	proto.AI[0]			= AI_KANA;
	proto.AI[1]			= AI_MUUTOS_JOS_ENERGIAA_YLI_1;
	proto.AI[2]			= AI_PIILOUTUU;

	for (int ai=2;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 22;
	proto.frame_rate	= 4;
	proto.hyokkays1_aika= 30;
	proto.hyokkays2_aika= 60;
	proto.kuva_x		= 1;
	proto.kuva_y		= 125;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 59;
	proto.korkeus		= 58;
	proto.latausaika	= 0;//60
	proto.leveys		= 30;
	proto.max_hyppy		= 26;
	proto.max_nopeus	= 3;
	proto.paino			= 1;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO + TUHOUTUMINEN_HOYHENET;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.liitokyky		= true;

	UCHAR seq1[9] = {1,1,21,21,22,22,2,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[5] = {3,4,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[5] = {8,8,9,9,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[5] = {1,10,11,12,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {13,14,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[8] = {15,15,16,16,17,17,18,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {19,19,19,19,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);

	UCHAR seq9[5] = {20,20,20,20,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq9, false);

	proto.Tallenna(tiedosto);
}



void luo_prototyyppi_pikkukana(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "baby chicken");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "chick.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PIKKUKANA;
	proto.AI[1]			= AI_SEURAA_PELAAJAA;

	proto.animaatioita	= 7;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 8;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 249;
	proto.kuva_frame_leveys  = 26;
	proto.kuva_frame_korkeus = 29;
	proto.korkeus		= 29;
	proto.latausaika	= 5;
	proto.leveys		= 26;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 3;
	proto.paino			= 0.8;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.liitokyky     = false;

	UCHAR seq1[9] = {1,1,1,1,1,1,2,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	UCHAR seq2[3] = {1,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,	 seq2, true);

	UCHAR seq3[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS, seq3, true);

	UCHAR seq4[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS, seq4, true);

	UCHAR seq5[2] = {8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY, seq5, false);

	UCHAR seq6[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO, seq6, true);

	UCHAR seq7[7] = {7,4,4,5,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA, seq7, false);

	UCHAR seq8[7] = {1,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kana(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "a hen");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"feather.spr");
	strcpy(proto.ammus1_sprite,"egg.spr");//egg.spr
	strcpy(proto.ammus2_sprite,"");//doodle.spr
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"p_damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"p_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"p_layegg.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"p_layegg.wav");
	strcpy(proto.aanitiedostot[AANI_RANDOM],		"hen.wav");


	proto.AI[0]			= AI_KANA;
	
	proto.aani_frq		= 26050;
	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 10;
	proto.frame_rate	= 7;
	proto.hyokkays1_aika= 30;
	proto.hyokkays2_aika= 60;
	proto.kuva_x		= 106;
	proto.kuva_y		= 63;
	proto.kuva_frame_leveys  = 34;
	proto.kuva_frame_korkeus = 37;
	proto.korkeus		= 37;//58
	proto.latausaika	= 60;//40
	proto.leveys		= 30;
	proto.max_hyppy		= 20;
	proto.max_nopeus	= 3;
	proto.paino			= 1;
	proto.random_frq	= true;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO + TUHOUTUMINEN_HOYHENET;
	proto.tarisee       = false;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.lapinakyvyys	= false;
	proto.hehkuu		= false;
	proto.tulitauko		= 0;
	proto.liitokyky		= true;
	proto.boss			= false;
	proto.bonus_aina	= false;

	UCHAR seq1[8] = {1,1,1,1,1,2,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[5] = {3,3,4,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[5] = {5,5,6,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {8,9,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[6] = {8,8,8,7,10,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {5,5,5,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);

	UCHAR seq9[5] = {9,9,9,9,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq9, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_karkki(char *tiedosto, char *nimi, UCHAR vari, int pisteet)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bon2.bmp");
	strcpy(proto.nimi, nimi);
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus8.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 10;//1
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;//1
	proto.kuva_y		= 59;
	proto.kuva_frame_leveys  = 27;
	proto.kuva_frame_korkeus = 27;
	proto.korkeus		= 27;
	proto.latausaika	= 0;
	proto.leveys		= 27;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= pisteet;
	proto.random_frq	= true;//false
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_HARMAA  + vari/32;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= vari;
	proto.vihollinen	= false;

	//UCHAR seq1[2] = {1,0}; 
	UCHAR seq1[11] = {1,2,3,4,5,6,7,8,9,10,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_jalokivi(char *tiedosto, char *nimi, UCHAR vari, int pisteet)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bon2.bmp");
	strcpy(proto.nimi, nimi);
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus1.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 10;//1
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;//1
	proto.kuva_y		= 89;
	proto.kuva_frame_leveys  = 22;
	proto.kuva_frame_korkeus = 24;
	proto.korkeus		= 24;
	proto.latausaika	= 0;
	proto.leveys		= 22;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 3;
	proto.paino			= 0.5;
	proto.pisteet		= pisteet;
	proto.random_frq	= true;//false
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_HARMAA + vari/32;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= vari;
	proto.vihollinen	= false;

	//UCHAR seq1[2] = {1,0}; 
	UCHAR seq1[11] = {1,2,3,4,5,6,7,8,9,10,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_omena1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "apple");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus9.wav");//bonus9.wav/apple.wav

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 8;//1
	proto.frame_rate	= 4;
	proto.kuva_x		= 380;//1
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 19;
	proto.kuva_frame_korkeus = 23;
	proto.korkeus		= 23;
	proto.latausaika	= 0;
	proto.leveys		= 19;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 10;
	proto.random_frq	= true;//false
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	//UCHAR seq1[2] = {1,0}; 
	UCHAR seq1[9] = {1,2,3,4,5,6,7,8,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_omena2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "falling apple");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus12.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 8;//1
	proto.frame_rate	= 4;
	proto.kuva_x		= 380;//1
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 19;
	proto.kuva_frame_korkeus = 23;
	proto.korkeus		= 23;
	proto.latausaika	= 0;
	proto.leveys		= 19;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 3;
	proto.paino			= 0.5;
	proto.pisteet		= 10;
	proto.random_frq	= true;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[9] = {1,2,3,4,5,6,7,8,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_omena3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "changing apple");
	strcpy(proto.muutos_sprite,"apple4.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus13.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]			= AI_MUUTOS_AJASTIN;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 8;
	proto.frame_rate	= 4;
	proto.kuva_x		= 380;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 19;
	proto.kuva_frame_korkeus = 23;
	proto.korkeus		= 23;
	proto.latausaika	= 400;
	proto.leveys		= 19;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 3;
	proto.paino			= 0.5;
	proto.pisteet		= 12;
	proto.random_frq	= true;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_ORANSSI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_ORANSSI;
	proto.vihollinen	= false;

	UCHAR seq1[9] = {1,2,3,4,5,6,7,8,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_omena4(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "rotten apple");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 8;
	proto.frame_rate	= 4;
	proto.kuva_x		= 380;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 19;
	proto.kuva_frame_korkeus = 23;
	proto.korkeus		= 23;
	proto.latausaika	= 500;
	proto.leveys		= 19;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 3;
	proto.paino			= 0.5;
	proto.pisteet		= 0;
	proto.random_frq	= true;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= false;

	UCHAR seq1[9] = {1,2,3,4,5,6,7,8,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_omena5(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "newton apple");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus9.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]			= AI_TIPPUU_TARINASTA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 8;
	proto.frame_rate	= 4;
	proto.kuva_x		= 380;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 19;
	proto.kuva_frame_korkeus = 23;
	proto.korkeus		= 23;
	proto.latausaika	= 500;
	proto.leveys		= 19;
	proto.max_hyppy		= 20;
	proto.max_nopeus	= 4;
	proto.paino			= 0;
	proto.pisteet		= 14;
	proto.random_frq	= true;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_VIOLETTI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_VIOLETTI;
	proto.vihollinen	= false;

	UCHAR seq1[9] = {1,2,3,4,5,6,7,8,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_iso_omena(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "big apple");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "app_bite.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 134;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 26;
	proto.kuva_frame_korkeus = 27;
	proto.korkeus		= 27;
	proto.latausaika	= 0;
	proto.leveys		= 26;
	proto.max_hyppy		= 20;
	proto.max_nopeus	= 2;
	proto.paino			= 1;
	proto.pisteet		= 100;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_iso_omena2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "big apple 2");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "app_bite.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 134;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 26;
	proto.kuva_frame_korkeus = 27;
	proto.korkeus		= 27;
	proto.latausaika	= 0;
	proto.leveys		= 26;
	proto.max_hyppy		= 20;
	proto.max_nopeus	= 2;
	proto.paino			= 1;
	proto.pisteet		= 200;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_ORANSSI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_ORANSSI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_porkkana(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "carrot");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus8.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 97;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 10;
	proto.kuva_frame_korkeus = 21;
	proto.korkeus		= 21;
	proto.latausaika	= 0;
	proto.leveys		= 10;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_ORANSSI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_juusto(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "carrot");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus11.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 14;
	proto.kuva_x		= 336;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 24;
	proto.kuva_frame_korkeus = 21;
	proto.korkeus		= 21;
	proto.latausaika	= 0;
	proto.leveys		= 24;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 30;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_ORANSSI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_pippuri(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "hot pepper");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"doodlef2.spr");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus6.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 186;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 15;
	proto.kuva_frame_korkeus = 20;
	proto.korkeus		= 20;
	proto.latausaika	= 0;
	proto.leveys		= 15;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 40;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_hoyhen(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "feather");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus4.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 77;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 17;
	proto.kuva_frame_korkeus = 17;
	proto.korkeus		= 17;
	proto.latausaika	= 0;
	proto.leveys		= 17;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_HOYHENET;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= -1;
	proto.vahinko_tyyppi= VAHINKO_BONUS;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_hoyhen2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "super feather");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus4.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 77;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 17;
	proto.kuva_frame_korkeus = 17;
	proto.korkeus		= 17;
	proto.latausaika	= 0;
	proto.leveys		= 17;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_HOYHENET;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= -3;
	proto.vahinko_tyyppi= VAHINKO_BONUS;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}
/*
void luo_prototyyppi_appelsiini(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "orange");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus1.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 23;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 19;
	proto.kuva_frame_korkeus = 19;
	proto.korkeus		= 19;
	proto.latausaika	= 0;
	proto.leveys		= 19;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 3;
	proto.paino			= 0.4;
	proto.pisteet		= 30;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_ORANSSI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}
*/
void luo_prototyyppi_appelsiini(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "orange");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus12.wav");

	proto.AI[0]			= AI_PERUS;//AI_BONUS;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 8;
	proto.frame_rate	= 4;
	proto.kuva_x		= 204;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 19;
	proto.kuva_frame_korkeus = 19;
	proto.korkeus		= 19;
	proto.latausaika	= 0;
	proto.leveys		= 19;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 3;
	proto.paino			= 0.5;
	proto.pisteet		= 30;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_ORANSSI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);
	
	UCHAR seq2[9] = {1,2,3,4,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq2, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_greippi(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "orange");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus12.wav");

	proto.AI[0]			= AI_PERUS;//AI_BONUS;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 8;
	proto.frame_rate	= 4;
	proto.kuva_x		= 204;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 19;
	proto.kuva_frame_korkeus = 19;
	proto.korkeus		= 19;
	proto.latausaika	= 0;
	proto.leveys		= 19;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 3;
	proto.paino			= 0.5;
	proto.pisteet		= 45;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_VIHREA;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);
	
	UCHAR seq2[9] = {1,2,3,4,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq2, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_viinirypale(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "grape");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus12.wav");

	proto.AI[0]			= AI_PERUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 585;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 13;
	proto.kuva_frame_korkeus = 11;
	proto.korkeus		= 11;
	proto.latausaika	= 0;
	proto.leveys		= 13;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 4;
	proto.paino			= 0.5;
	proto.pisteet		= 5;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_VIHREA;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_ananas1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bon2.bmp");
	strcpy(proto.nimi, "pineapple");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus9.wav");//bonus9.wav/apple.wav

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 6;//1
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;//1
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 21;
	proto.kuva_frame_korkeus = 28;
	proto.korkeus		= 28;
	proto.latausaika	= 0;
	proto.leveys		= 21;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 18;
	proto.random_frq	= true;//false
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_ORANSSI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	//UCHAR seq1[2] = {1,0}; 
	UCHAR seq1[7] = {1,2,3,4,5,6,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kummitusjauhe1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bon2.bmp");
	strcpy(proto.nimi, "ghost powder");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus12.wav");

	proto.AI[0]			= AI_PERUS;//AI_BONUS;
	proto.AI[1]			= AI_BONUS_NAKYMATTOMYYS;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 32;
	proto.kuva_frame_leveys  = 20;
	proto.kuva_frame_korkeus = 24;
	proto.korkeus		= 24;
	proto.latausaika	= 1500;
	proto.leveys		= 20;
	proto.max_hyppy		= 10;
	proto.max_nopeus	= 3;
	proto.paino			= 0.5;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_SININEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_SININEN;
	proto.vihollinen	= false;

	UCHAR seq2[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq2, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kummitusjauhe2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bon2.bmp");
	strcpy(proto.nimi, "ghost powder renew");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus12.wav");

	proto.AI[0]			= AI_PERUS;//AI_BONUS;
	proto.AI[1]			= AI_BONUS_NAKYMATTOMYYS;
	proto.AI[2]			= AI_UUSI_JOS_TUHOUTUU;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 32;
	proto.kuva_frame_leveys  = 20;
	proto.kuva_frame_korkeus = 24;
	proto.korkeus		= 24;
	proto.latausaika	= 1500;
	proto.leveys		= 20;
	proto.max_hyppy		= 10;
	proto.max_nopeus	= 3;
	proto.paino			= 0.5;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_TURKOOSI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_TURKOOSI;
	proto.vihollinen	= false;

	UCHAR seq2[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq2, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq2, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_paasiaismuna(char *tiedosto, char *nimi, char *sprite, 
								  int pisteet, UCHAR vari, UCHAR tuhoutuminen)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, nimi);
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,sprite);
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus7.wav");//bonus9.wav/apple.wav

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 6;
	proto.frame_rate	= 4;
	proto.kuva_x		= 380;
	proto.kuva_y		= 27;
	proto.kuva_frame_leveys  = 20;
	proto.kuva_frame_korkeus = 21;
	proto.korkeus		= 20;
	proto.latausaika	= 0;
	proto.leveys		= 21;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= pisteet;
	proto.random_frq	= true;//false
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= tuhoutuminen;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= vari;
	proto.vihollinen	= false;

	//UCHAR seq1[2] = {1,0}; 
	UCHAR seq1[9] = {1,2,3,4,5,6,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_avain1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "key type 1");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus3.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= true;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 14;
	proto.kuva_x		= 45;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 13;
	proto.kuva_frame_korkeus = 20;
	proto.korkeus		= 20;
	proto.latausaika	= 0;
	proto.leveys		= 13;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 3;
	proto.paino			= 0.5;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_ORANSSI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_avain2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "key type 2");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= true;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 45;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 13;
	proto.kuva_frame_korkeus = 20;
	proto.korkeus		= 20;
	proto.latausaika	= 0;
	proto.leveys		= 13;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 3;
	proto.paino			= 0.5;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_SININEN;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_avain3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "spoo-key 1");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus3.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]			= AI_TIPPUU_TARINASTA;

	proto.animaatioita	= 1;
	proto.avain			= true;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 14;
	proto.kuva_x		= 45;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 13;
	proto.kuva_frame_korkeus = 20;
	proto.korkeus		= 20;
	proto.latausaika	= 0;
	proto.leveys		= 13;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_TURKOOSI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_TURKOOSI;
	proto.vihollinen	= false;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_avain4(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "jumping key");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus3.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]			= AI_HYPPIJA;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_NONSTOP;

	proto.animaatioita	= 1;
	proto.avain			= true;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 8;
	proto.kuva_x		= 45;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 13;
	proto.kuva_frame_korkeus = 20;
	proto.korkeus		= 20;
	proto.latausaika	= 0;
	proto.leveys		= 13;
	proto.max_hyppy		= 10;
	proto.max_nopeus	= 2;
	proto.paino			= 0.3;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kello1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "clock 20 sec");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus1.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]			= AI_BONUS_AIKA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 79;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 28;
	proto.kuva_frame_korkeus = 28;
	proto.korkeus		= 28;
	proto.latausaika	= 20;//sekunteja
	proto.leveys		= 28;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 10;
	proto.random_frq	= true;//false
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_ORANSSI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kello2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "clock 10 sec");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus1.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]			= AI_BONUS_AIKA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 79;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 28;
	proto.kuva_frame_korkeus = 28;
	proto.korkeus		= 28;
	proto.latausaika	= 10;//sekunteja
	proto.leveys		= 28;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 20;
	proto.random_frq	= true;//false
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kello3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "clock 30 sec");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus1.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]			= AI_BONUS_AIKA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 79;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 28;
	proto.kuva_frame_korkeus = 28;
	proto.korkeus		= 28;
	proto.latausaika	= 30;//sekunteja
	proto.leveys		= 28;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 20;
	proto.random_frq	= true;//false
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_VIHREA;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_muna(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "egg");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"eggcrack.wav");

	proto.AI[0]			= AI_MUNA;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 5;
	proto.frame_rate	= 1;
	proto.kuva_x		= 1;
	proto.kuva_y		= 281;
	proto.kuva_frame_leveys  = 11;
	proto.kuva_frame_korkeus = 13;
	proto.korkeus		= 13;
	proto.latausaika	= 10;
	proto.leveys		= 11;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0.3;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_AMMUS;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_PUDOTUS;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.tulitauko		= 50;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	UCHAR seq2[5] = {2,3,4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	 seq2, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_muna_haju(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "egg");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"eggcrack.wav");

	proto.AI[0]			= AI_MUNA;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 5;
	proto.frame_rate	= 1;
	proto.kuva_x		= 1;
	proto.kuva_y		= 281;
	proto.kuva_frame_leveys  = 11;
	proto.kuva_frame_korkeus = 13;
	proto.korkeus		= 13;
	proto.latausaika	= 10;
	proto.leveys		= 11;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0.3;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_AMMUS;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_PUDOTUS;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= false;
	proto.tulitauko		= 10;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	UCHAR seq2[5] = {2,3,4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	 seq2, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_muna_tuli(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "fire egg");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"flame.spr");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"eggcrack.wav");

	proto.AI[0]			= AI_AMMUS;
	proto.AI[1]			= AI_HYOKKAYS_1_JOS_OSUTTU;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 5;
	proto.frame_rate	= 1;
	proto.kuva_x		= 1;
	proto.kuva_y		= 281;
	proto.kuva_frame_leveys  = 11;
	proto.kuva_frame_korkeus = 13;
	proto.korkeus		= 13;
	proto.latausaika	= 40;
	proto.leveys		= 11;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0.3;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_PUNAINEN;
	proto.tyyppi		= TYYPPI_AMMUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= false;
	proto.tulitauko		= 50;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	UCHAR seq2[5] = {2,3,4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	 seq2, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_muna_jaa(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "ice egg");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"icecube.spr");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"eggcrack.wav");

	proto.AI[0]			= AI_AMMUS;
	proto.AI[1]			= AI_HYOKKAYS_1_JOS_OSUTTU;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 5;
	proto.frame_rate	= 1;
	proto.kuva_x		= 1;
	proto.kuva_y		= 281;
	proto.kuva_frame_leveys  = 11;
	proto.kuva_frame_korkeus = 13;
	proto.korkeus		= 13;
	proto.latausaika	= 40;
	proto.leveys		= 11;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0.3;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_SININEN;
	proto.tyyppi		= TYYPPI_AMMUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_SININEN;
	proto.vihollinen	= false;
	proto.tulitauko		= 50;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	UCHAR seq2[5] = {2,3,4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	 seq2, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_pommi(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_batb.bmp");
	strcpy(proto.nimi, "bomb");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"flame.spr");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"explot.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]			= AI_POMMI;
	proto.AI[2]			= AI_HYOKKAYS_1_JOS_OSUTTU;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 3;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 81;
	proto.kuva_frame_leveys  = 23;
	proto.kuva_frame_korkeus = 28;
	proto.korkeus		= 28;
	proto.latausaika	= 60;
	proto.leveys		= 23;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 0.4;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_ORANSSI;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.tulitauko		= 0;

	UCHAR seq1[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kieku(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "cockadoodledoo");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");

	proto.AI[0]			= AI_AMMUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;

	for (int ai=2;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 1;
	proto.kuva_x		= 1;
	proto.kuva_y		= 297;
	proto.kuva_frame_leveys  = 25;
	proto.kuva_frame_korkeus = 19;
	proto.korkeus		= 19;
	proto.latausaika	= 50;
	proto.leveys		= 25;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_AMMUS;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_MELU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.tulitauko		= 90;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kieku_tuli(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "fire doodle flame");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"flame.spr");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");

	proto.AI[0]			= AI_AMMUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_HYOKKAYS_1_JOS_OSUTTU;
	proto.AI[3]			= AI_VAHINGOITTUU_VEDESTA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 319;
	proto.kuva_frame_leveys  = 25;
	proto.kuva_frame_korkeus = 19;
	proto.korkeus		= 19;
	proto.latausaika	= 50;
	proto.leveys		= 25;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVUPILVET;
	proto.tyyppi		= TYYPPI_AMMUS;
	proto.vahinko		= 1;//2
	proto.vahinko_tyyppi= VAHINKO_TULI;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= false;
	proto.tulitauko		= 60;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kieku_tuli_2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "fire doodle");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");

	proto.AI[0]			= AI_AMMUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_VAHINGOITTUU_VEDESTA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 319;
	proto.kuva_frame_leveys  = 25;
	proto.kuva_frame_korkeus = 19;
	proto.korkeus		= 19;
	proto.latausaika	= 50;
	proto.leveys		= 25;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_TULI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVUPILVET;
	proto.tyyppi		= TYYPPI_AMMUS;
	proto.vahinko		= 1;//2
	proto.vahinko_tyyppi= VAHINKO_TULI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.tulitauko		= 50;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kieku_lumi(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "ice doodle");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"icecube.spr");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");

	proto.AI[0]			= AI_AMMUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_HYOKKAYS_1_JOS_OSUTTU;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 1;
	proto.kuva_x		= 1;
	proto.kuva_y		= 341;
	proto.kuva_frame_leveys  = 25;
	proto.kuva_frame_korkeus = 19;
	proto.korkeus		= 19;
	proto.latausaika	= 50;
	proto.leveys		= 25;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_LUMI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_SININEN;
	proto.tyyppi		= TYYPPI_AMMUS;
	proto.vahinko		= 1;//2
	proto.vahinko_tyyppi= VAHINKO_LUMI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.tulitauko		= 60;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kieku_valkosipuli(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "bad breath");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");

	proto.AI[0]			= AI_AMMUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 363;
	proto.kuva_frame_leveys  = 27;
	proto.kuva_frame_korkeus = 21;
	proto.korkeus		= 21;
	proto.latausaika	= 30;
	proto.leveys		= 27;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVUPILVET;//TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_AMMUS;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_HAJU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.tulitauko		= 10;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kieku_sonic(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "sonic doodle");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");

	proto.AI[0]			= AI_AMMUS;
	proto.AI[1]			= AI_NONSTOP;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 387;
	proto.kuva_frame_leveys  = 27;
	proto.kuva_frame_korkeus = 27;
	proto.korkeus		= 27;
	proto.latausaika	= 50;
	proto.leveys		= 27;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 4;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_AMMUS;
	proto.tiletarkistus = false;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_MELU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.tulitauko		= 20;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_ammus1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_tank.bmp");
	strcpy(proto.nimi, "ammo type 1");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");

	proto.AI[0]			= AI_AMMUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 1;
	proto.kuva_x		= 1;
	proto.kuva_y		= 44;
	proto.kuva_frame_leveys  = 7;
	proto.kuva_frame_korkeus = 7;
	proto.korkeus		= 7;
	proto.latausaika	= 40;
	proto.leveys		= 7;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 4;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_AMMUS;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_SAHKO;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.tulitauko		= 40;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_ammus2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_tank.bmp");
	strcpy(proto.nimi, "ammo type 1");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");

	proto.AI[0]			= AI_AMMUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 1;
	proto.kuva_x		= 1;
	proto.kuva_y		= 44;
	proto.kuva_frame_leveys  = 7;
	proto.kuva_frame_korkeus = 7;
	proto.korkeus		= 7;
	proto.latausaika	= 140;
	proto.leveys		= 7;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 4;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_AMMUS;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_ORANSSI;
	proto.vihollinen	= true;
	proto.tulitauko		= 40;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_jaapala(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "ice cube");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "");
	
	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_POMMI;
	proto.AI[2]			= AI_KITKA_VAIKUTTAA;
	//proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= true;
	proto.frameja		= 1;
	proto.frame_rate	= 8;
	proto.kuva_x		= 29;
	proto.kuva_y		= 341;
	proto.kuva_frame_leveys  = 19;
	proto.kuva_frame_korkeus = 19;
	proto.korkeus		= 19;
	proto.latausaika	= 300;
	proto.leveys		= 19;
	proto.max_hyppy		= 5;
	proto.max_nopeus	= 1;
	proto.paino			= 0.5;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_LUMI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_SININEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_LUMI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kivipala(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hens.bmp");
	strcpy(proto.nimi, "stone cube");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "");
	
	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= true;
	proto.frameja		= 1;
	proto.frame_rate	= 8;
	proto.kuva_x		= 29;
	proto.kuva_y		= 341;
	proto.kuva_frame_leveys  = 19;
	proto.kuva_frame_korkeus = 19;
	proto.korkeus		= 19;
	proto.latausaika	= 300;
	proto.leveys		= 19;
	proto.max_hyppy		= 5;
	proto.max_nopeus	= 1;
	proto.paino			= 0.5;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_HARMAA;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_HARMAA;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kivipala2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "stone cube item");
	strcpy(proto.muutos_sprite,"stncube3.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "");
	
	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KITKA_VAIKUTTAA;
	proto.AI[2]			= AI_MUUTOS_AJASTIN;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 8;
	proto.kuva_x		= 110;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 25;
	proto.kuva_frame_korkeus = 17;
	proto.korkeus		= 17;
	proto.latausaika	= 150;
	proto.leveys		= 25;
	proto.max_hyppy		= 5;
	proto.max_nopeus	= 1;
	proto.paino			= 0.5;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kivipala3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "stone cube bonus");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"stncube2.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "");
	
	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 8;
	proto.kuva_x		= 110;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 25;
	proto.kuva_frame_korkeus = 17;
	proto.korkeus		= 17;
	proto.latausaika	= 150;
	proto.leveys		= 25;
	proto.max_hyppy		= 5;
	proto.max_nopeus	= 1;
	proto.paino			= 1;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_HARMAA;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tennispallo(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_mous.bmp");
	strcpy(proto.nimi, "tennis ball");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "");
	
	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_HEITTOASE;
	proto.AI[2]			= AI_POMMI;
	proto.AI[3]			= AI_HYPPIJA;
	proto.AI[4]			= AI_KAANTYY_ESTEESTA_HORI;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 8;
	proto.kuva_x		= 1;
	proto.kuva_y		= 71;
	proto.kuva_frame_leveys  = 15;
	proto.kuva_frame_korkeus = 15;
	proto.korkeus		= 15;
	proto.latausaika	= 300;
	proto.leveys		= 15;
	proto.max_hyppy		= 25;
	proto.max_nopeus	= 4;
	proto.paino			= 0.5;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_ORANSSI;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[5] = {1,2,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq1, true);

	proto.Tallenna(tiedosto);
}
/*
void luo_prototyyppi_hyppija(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"pk2spr01.bmp");
	strcpy(proto.nimi, "red jumper");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"apple.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_HYPPIJA;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_NONSTOP;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;
	for (int ai=4;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 3;
	proto.avain			= false;
	proto.energia		= 3;
	proto.este			= false;
	proto.frameja		= 12;
	proto.frame_rate	= 6;
	proto.kuva_x		= 1;
	proto.kuva_y		= 120;
	proto.kuva_frame_leveys  = 40;
	proto.kuva_frame_korkeus = 44;
	proto.korkeus		= 44;
	proto.latausaika	= 15;
	proto.leveys		= 40;
	proto.max_hyppy		= 20;//20
	proto.max_nopeus	= 2;
	proto.paino			= 1;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_PUNAINEN;//TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[10] = {6,6,7,7,8,9,10,11,12,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_hyppija2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"pk2spr01.bmp");
	strcpy(proto.nimi, "green jumper");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"grape.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_HYPPIJA;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_SEURAA_PELAAJAA;
	for (int ai=3;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 3;
	proto.avain			= false;
	proto.energia		= 3;
	proto.este			= false;
	proto.frameja		= 12;
	proto.frame_rate	= 6;
	proto.kuva_x		= 1;
	proto.kuva_y		= 120;
	proto.kuva_frame_leveys  = 40;
	proto.kuva_frame_korkeus = 44;
	proto.korkeus		= 44;
	proto.latausaika	= 15;
	proto.leveys		= 40;
	proto.max_hyppy		= 20;//20
	proto.max_nopeus	= 0;
	proto.paino			= 1;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_VIHREA;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 2;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[10] = {6,6,7,7,8,9,10,11,12,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);

	proto.Tallenna(tiedosto);
}*/
/*
void luo_prototyyppi_kani(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"pk2spr01.bmp");
	strcpy(proto.nimi, "rabbit");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"carrot.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");


	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_RANDOM_ALOITUSSUUNTA_HORI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;

	for (int ai=4;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 6;
	proto.avain			= false;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 19;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 167;
	proto.kuva_frame_leveys  = 38;
	proto.kuva_frame_korkeus = 50;
	proto.korkeus		= 50;
	proto.latausaika	= 15;
	proto.leveys		= 31;
	proto.max_hyppy		= 35;
	proto.paino			= 1;
	proto.pisteet		= 20;
	proto.suojaus		= SUOJA_EI_MITAAN;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[10] = {1,1,2,2,1,1,2,2,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[7] = {7,8,9,10,11,12,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[4] = {1,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[5] = {13,13,14,14,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[10] = {15,15,16,16,17,17,18,18,19,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	proto.Tallenna(tiedosto);
}
*/
void luo_prototyyppi_susi1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_wolf.bmp");
	strcpy(proto.nimi, "wolf");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "wolf_ko.wav");

	proto.AI[0]			= AI_PERUS;
	//proto.AI[0]			= AI_PIILOUTUU;
	proto.AI[1]			= AI_RANDOM_KAANTYMINEN;
	proto.AI[2]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;
	proto.AI[3]			= AI_VAROO_KUOPPAA;
	proto.AI[4]			= AI_KAANTYY_ESTEESTA_HORI;
	//proto.AI[4]			= AI_PALAA_ALKUUN_X;
	/*
	proto.AI[0]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;
	proto.AI[3]			= AI_VAROO_KUOPPAA;
	proto.AI[4]			= AI_PALAA_ALKUUN_X;//AI_NONSTOP;
	*/

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.energia		= 5;
	proto.este			= false;
	proto.frameja		= 16;
	proto.frame_rate	= 8;
	proto.kuva_x		= 2;
	proto.kuva_y		= 2;
	proto.kuva_frame_leveys  = 47;
	proto.kuva_frame_korkeus = 52;
	proto.korkeus		= 52;
	proto.latausaika	= 15;
	proto.leveys		= 31;
	proto.max_hyppy		= 25;
	proto.max_nopeus	= 3;
	proto.paino			= 1;
	proto.pisteet		= 60;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 2;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[8] = {5,5,5,5,5,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[5] = {1,2,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[4] = {9,9,10,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {13,14,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[10] = {13,13,13,14,14,14,14,15,16,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {11,11,12,12,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);


	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_susi2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_wolf.bmp");
	strcpy(proto.nimi, "evil wolf");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "wolf_ko.wav");

	proto.AI[0]			= AI_PIKKUKANA;
	proto.AI[1]			= AI_SEURAA_PELAAJAA;
	proto.AI[2]			= AI_PIILOUTUU;
	/*
	proto.AI[0]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;
	proto.AI[3]			= AI_VAROO_KUOPPAA;
	proto.AI[4]			= AI_PALAA_ALKUUN_X;//AI_NONSTOP;
	*/

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.energia		= 5;
	proto.este			= false;
	proto.frameja		= 16;
	proto.frame_rate	= 8;
	proto.kuva_x		= 2;
	proto.kuva_y		= 2;
	proto.kuva_frame_leveys  = 47;
	proto.kuva_frame_korkeus = 52;
	proto.korkeus		= 52;
	proto.latausaika	= 15;
	proto.leveys		= 31;
	proto.max_hyppy		= 30;
	proto.max_nopeus	= 3;
	proto.paino			= 1;
	proto.pisteet		= 60;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 2;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_VIOLETTI;
	proto.vihollinen	= true;

	UCHAR seq1[8] = {5,5,5,5,5,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[5] = {1,2,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[4] = {9,9,10,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {13,14,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[10] = {13,13,13,14,14,14,14,15,16,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {11,11,12,12,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);


	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_susi3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_wolf.bmp");
	strcpy(proto.nimi, "smart wolf");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "wolf_ko.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_PIILOUTUU;
	proto.AI[2]         = AI_PIKKUKANA;
	proto.AI[3]			= AI_SEURAA_PELAAJAA;
	proto.AI[4]			= AI_HYPPY_JOS_PELAAJA_YLAPUOLELLA;
	/*
	proto.AI[0]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;
	proto.AI[3]			= AI_VAROO_KUOPPAA;
	proto.AI[4]			= AI_PALAA_ALKUUN_X;//AI_NONSTOP;
	*/

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.energia		= 5;
	proto.este			= false;
	proto.frameja		= 16;
	proto.frame_rate	= 8;
	proto.kuva_x		= 2;
	proto.kuva_y		= 2;
	proto.kuva_frame_leveys  = 47;
	proto.kuva_frame_korkeus = 52;
	proto.korkeus		= 52;
	proto.latausaika	= 15;
	proto.leveys		= 31;
	proto.max_hyppy		= 25;
	proto.max_nopeus	= 3;
	proto.paino			= 1;
	proto.pisteet		= 60;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 2;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= true;

	UCHAR seq1[8] = {5,5,5,5,5,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[5] = {1,2,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[4] = {9,9,10,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {13,14,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[10] = {13,13,13,14,14,14,14,15,16,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {11,11,12,12,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);


	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_sammakko1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_frog.bmp");
	strcpy(proto.nimi, "frog");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"grape.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "frog_dmg.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "frog_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "frog_att.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "frog_att.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_SAMMAKKO1;
	proto.AI[3]			= AI_SEURAA_PELAAJAA;
	proto.AI[4]			= AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 8;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 30;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 30;
	proto.latausaika	= 15;
	proto.leveys		= 30;
	proto.max_hyppy		= 26;
	proto.max_nopeus	= 0;
	proto.paino			= 1;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_VIHREA;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[5] = {1,1,2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[5] = {1,4,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[6] = {4,4,4,4,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, false);

	UCHAR seq4[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[2] = {8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_sammakko2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_frog.bmp");
	strcpy(proto.nimi, "frog");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"grape.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "frog_dmg.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "frog_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "frog_att.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "frog_att.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_SAMMAKKO2;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[4]			= AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 8;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 30;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 30;
	proto.latausaika	= 15;
	proto.leveys		= 30;
	proto.max_hyppy		= 26;
	proto.max_nopeus	= 4;
	proto.paino			= 1;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_VIHREA;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= true;

	UCHAR seq1[5] = {1,1,2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[5] = {5,5,5,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, false);

	UCHAR seq3[6] = {4,4,4,4,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, false);

	UCHAR seq4[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[2] = {8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_sammakko3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_frog.bmp");
	strcpy(proto.nimi, "frog");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"grape.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "frog_dmg.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "frog_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "frog_att.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "frog_att.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_SAMMAKKO1;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[4]			= AI_NONSTOP;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 8;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 30;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 30;
	proto.latausaika	= 15;
	proto.leveys		= 30;
	proto.max_hyppy		= 26;
	proto.max_nopeus	= 2;
	proto.paino			= 1;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_VIHREA;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_SININEN;
	proto.vihollinen	= true;

	UCHAR seq1[5] = {1,1,2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[5] = {1,4,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[6] = {4,4,4,4,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, false);

	UCHAR seq4[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[2] = {8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_sammakko4(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_frog.bmp");
	strcpy(proto.nimi, "evil frog");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"grape.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "frog_dmg.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "frog_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "frog_att.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "frog_att.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SEURAA_PELAAJAA;
	proto.AI[2]			= AI_SAMMAKKO2;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[4]			= AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 8;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 30;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 30;
	proto.latausaika	= 15;
	proto.leveys		= 30;
	proto.max_hyppy		= 26;
	proto.max_nopeus	= 2;
	proto.paino			= 1;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_VIOLETTI;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_VIOLETTI;
	proto.vihollinen	= true;

	UCHAR seq1[5] = {1,1,2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[5] = {1,4,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[6] = {4,4,4,4,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, false);

	UCHAR seq4[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[2] = {8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_sammakko5(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_frog.bmp");
	strcpy(proto.nimi, "player frog");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "frog_dmg.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "frog_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "frog_att.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "frog_att.wav");

	proto.AI[0]			= AI_PERUS;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 8;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 30;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 30;
	proto.latausaika	= 15;
	proto.leveys		= 30;
	proto.max_hyppy		= 26;
	proto.max_nopeus	= 2;
	proto.paino			= 1;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_VIHREA;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.liitokyky     = false;

	UCHAR seq1[5] = {1,1,2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[5] = {1,4,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[6] = {4,4,4,4,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, false);

	UCHAR seq4[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[2] = {8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_hyppija(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bun.bmp");
	strcpy(proto.nimi, "jumping bunny");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"carrot.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

//	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
//	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "bunny_dm.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bunny_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "bunny_at.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "bunny_at.wav");

	proto.AI[0]			= AI_HYPPIJA;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_NONSTOP;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;
	for (int ai=4;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 19;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 34;
	proto.kuva_frame_korkeus = 47;
	proto.korkeus		= 47;
	proto.latausaika	= 15;
	proto.leveys		= 31;
	proto.max_hyppy		= 5;
	proto.max_nopeus	= 2;
	proto.paino			= 1;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= true;

	UCHAR seq1[10] = {1,1,2,2,1,1,2,2,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[7] = {9,10,11,12,13,14,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {19,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[5] = {15,15,16,16,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[8] = {5,5,6,6,7,7,8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {17,17,18,18,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_hyppija2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bun.bmp");
	strcpy(proto.nimi, "juping bunny 2");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"carrot.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "bunny_dm.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bunny_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "bunny_at.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "bunny_at.wav");


	proto.AI[0]			= AI_HYPPIJA;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_SEURAA_PELAAJAA;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 19;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 34;
	proto.kuva_frame_korkeus = 47;
	proto.korkeus		= 47;
	proto.latausaika	= 15;
	proto.leveys		= 31;
	proto.max_hyppy		= 24;//20
	proto.max_nopeus	= 0;
	proto.paino			= 1;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= true;

	UCHAR seq1[10] = {1,1,2,2,1,1,2,2,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[7] = {9,10,11,12,13,14,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {19,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[5] = {15,15,16,16,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[8] = {5,5,6,6,7,7,8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {17,17,18,18,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kani(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bun.bmp");
	strcpy(proto.nimi, "boxing bunny");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"carrot.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "bunny_dm.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bunny_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "bunny_at.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "bunny_at.wav");

	proto.AI[0]			= AI_PERUS;
	//proto.AI[1]			= AI_PIILOUTUU;
	proto.AI[1]			= AI_RANDOM_KAANTYMINEN;
	proto.AI[2]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[4]			= AI_VAROO_KUOPPAA;
	proto.AI[5]			= AI_KAANTYY_JOS_OSUTTU;
	//proto.AI[4]			= AI_PALAA_ALKUUN_X;
	/*
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;
	proto.AI[4]			= AI_VAROO_KUOPPAA;
	proto.AI[5]			= AI_PALAA_ALKUUN_X;//AI_NONSTOP;
	*/

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 19;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 34;
	proto.kuva_frame_korkeus = 47;
	proto.korkeus		= 47;
	proto.latausaika	= 15;
	proto.leveys		= 31;
	proto.max_hyppy		= 35;
	proto.max_nopeus	= 3;
	proto.paino			= 1;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[10] = {1,1,2,2,1,1,2,2,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[7] = {9,10,11,12,13,14,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {19,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[5] = {15,15,16,16,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[8] = {5,5,6,6,7,7,8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {17,17,18,18,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kani2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bun.bmp");
	strcpy(proto.nimi, "jumping bunny");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"carrot.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "bunny_dm.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bunny_k2.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "bunny_at.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "bunny_at.wav");


	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	//proto.AI[3]			= AI_RANDOM_SUUNNANVAIHTO_HORI;
	proto.AI[3]			= AI_HYPPY_JOS_PELAAJA_YLAPUOLELLA;
	proto.AI[4]			= AI_RANDOM_HYPPY;
	proto.AI[5]			= AI_KAANTYY_JOS_OSUTTU;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 3;
	proto.este			= false;
	proto.frameja		= 19;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 34;
	proto.kuva_frame_korkeus = 47;
	proto.korkeus		= 47;
	proto.latausaika	= 15;
	proto.leveys		= 31;
	proto.max_hyppy		= 35;
	proto.max_nopeus	= 2;
	proto.paino			= 1;
	proto.pisteet		= 30;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_ORANSSI;
	proto.vihollinen	= true;

	UCHAR seq1[10] = {1,1,2,2,1,1,2,2,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[7] = {9,10,11,12,13,14,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {19,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[5] = {15,15,16,16,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[8] = {5,5,6,6,7,7,8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {17,17,18,18,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kilpikonna_vaihe1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_turt.bmp");
	strcpy(proto.nimi, "turtle (walking)");
	strcpy(proto.muutos_sprite,"turtle2.spr");
	strcpy(proto.bonus_sprite,"jewel3.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_MUUTOS_AJASTIN;
	proto.AI[2]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[4]			= AI_VAROO_KUOPPAA;
	proto.AI[5]			= AI_NONSTOP;//AI_MUUTOS_JOS_ENERGIAA_ALLE_2;

	proto.este			= true;
	proto.este_ylos		= false;
	proto.este_alas		= true;
	proto.este_oikealle = false;
	proto.este_vasemmalle = false;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 3;
	proto.frameja		= 13;
	proto.frame_rate	= 6;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 44;
	proto.kuva_frame_korkeus = 27;
	proto.korkeus		= 27;
	proto.latausaika	= 1000;
	proto.leveys		= 30;
	proto.max_hyppy		= 15;
	proto.max_nopeus	= 1;
	proto.paino			= 1;
	proto.pisteet		= 80;
	proto.suojaus		= VAHINKO_PUDOTUS;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[5] = {2,3,4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {11,12,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[7] = {12,12,12,11,1,13,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {2,3,4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kilpikonna_vaihe2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_turt.bmp");
	strcpy(proto.nimi, "turtle (shell)");
	strcpy(proto.muutos_sprite,"turtle3.spr");
	strcpy(proto.bonus_sprite,"jewel4.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_MUUTOS_AJASTIN;
	proto.AI[2]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[4]			= AI_VAROO_KUOPPAA;
	proto.AI[5]			= AI_KITKA_VAIKUTTAA;
	
	proto.este			= true;
	proto.este_ylos		= false;
	proto.este_alas		= true;
	proto.este_oikealle = true;
	proto.este_vasemmalle = true;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.frameja		= 8;
	proto.frame_rate	= 6;
	proto.kuva_x		= 236;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 44;
	proto.kuva_frame_korkeus = 27;
	proto.korkeus		= 27;
	proto.latausaika	= 800;
	proto.leveys		= 30;
	proto.max_hyppy		= 15;
	proto.max_nopeus	= 1;
	proto.paino			= 1;
	proto.pisteet		= 80;
	proto.suojaus		= VAHINKO_PUDOTUS;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[10] = {1,1,2,2,3,3,4,4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[5] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, false);

	UCHAR seq3[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, false);

	UCHAR seq4[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, false);

	UCHAR seq5[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[6] = {6,7,6,7,8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kilpikonna_vaihe3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_turt.bmp");
	strcpy(proto.nimi, "turtle (waking)");
	strcpy(proto.muutos_sprite,"turtle1.spr");
	strcpy(proto.bonus_sprite,"jewel4.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_MUUTOS_AJASTIN;
	proto.AI[2]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[4]			= AI_VAROO_KUOPPAA;
	proto.AI[5]			= AI_KITKA_VAIKUTTAA;
	
	proto.este			= true;
	proto.este_ylos		= false;
	proto.este_alas		= true;
	proto.este_oikealle = false;
	proto.este_vasemmalle = false;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.frameja		= 8;
	proto.frame_rate	= 6;
	proto.kuva_x		= 236;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 44;
	proto.kuva_frame_korkeus = 27;
	proto.korkeus		= 27;
	proto.latausaika	= 200;
	proto.leveys		= 30;
	proto.max_hyppy		= 15;
	proto.max_nopeus	= 1;
	proto.paino			= 1;
	proto.pisteet		= 80;
	proto.suojaus		= VAHINKO_PUDOTUS;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[10] = {5,5,4,4,3,3,2,2,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[5] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, false);

	UCHAR seq3[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, false);

	UCHAR seq4[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, false);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[6] = {6,7,6,7,8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tulikilpikonna(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_turt.bmp");
	strcpy(proto.nimi, "red turtle");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"jewel2.spr");
	strcpy(proto.ammus1_sprite,"");
	//strcpy(proto.ammus2_sprite,"doodlef2.spr");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	//proto.AI[1]			= AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	//proto.AI[3]			= AI_VAROO_KUOPPAA;
	proto.AI[3]			= AI_NONSTOP;//AI_MUUTOS_JOS_ENERGIAA_ALLE_2;

	proto.este			= true;
	proto.este_ylos		= false;
	proto.este_alas		= true;
	proto.este_oikealle = false;
	proto.este_vasemmalle = false;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 5;
	proto.frameja		= 13;
	proto.frame_rate	= 6;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 44;
	proto.kuva_frame_korkeus = 27;
	proto.korkeus		= 27;
	proto.latausaika	= 300;
	proto.leveys		= 30;
	proto.max_hyppy		= 15;
	proto.max_nopeus	= 1;
	proto.paino			= 1;
	proto.pisteet		= 40;
	proto.suojaus		= VAHINKO_TULI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_TULI;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= true;
	proto.bonus_aina    = true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[5] = {2,3,4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {11,12,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[7] = {12,12,12,11,1,13,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {2,3,4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_siili(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hedg.bmp");
	strcpy(proto.nimi, "stupid hedgehog");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"orange.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	/*
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}*/

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_VAROO_KUOPPAA;

	for (int ai=4;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 6;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 30;
	proto.latausaika	= 12;
	proto.leveys		= 30;
	proto.max_hyppy		= 15;
	proto.max_nopeus	= 2;
	proto.paino			= 1;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[3] = {3,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_siili2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hedg.bmp");
	strcpy(proto.nimi, "evil hedgehog");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"orange.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	/*
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}*/

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_RANDOM_ALOITUSSUUNTA_HORI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_VAROO_KUOPPAA;
	proto.AI[4]			= AI_SEURAA_PELAAJAA;
	proto.AI[5]			= AI_EI;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 6;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 30;
	proto.latausaika	= 12;
	proto.leveys		= 30;
	proto.max_hyppy		= 15;
	proto.max_nopeus	= 4;
	proto.paino			= 1;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_VIOLETTI;
	proto.vihollinen	= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[3] = {3,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_siili3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hedg.bmp");
	strcpy(proto.nimi, "deaf hedgehog");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"orange.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	/*
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}*/

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_VAROO_KUOPPAA;

	for (int ai=4;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 6;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 36;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 30;
	proto.latausaika	= 12;
	proto.leveys		= 30;
	proto.max_hyppy		= 15;
	proto.max_nopeus	= 2;
	proto.paino			= 1;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_MELU;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[3] = {3,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_siili4(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_hedg.bmp");
	strcpy(proto.nimi, "stoned hedgehog");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"orange.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_VAROO_KUOPPAA;

	for (int ai=4;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 6;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 36;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 30;
	proto.latausaika	= 12;
	proto.leveys		= 30;
	proto.max_hyppy		= 15;
	proto.max_nopeus	= 0;
	proto.paino			= 1;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_MELU;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_HARMAA;
	proto.vihollinen	= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[3] = {3,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}
void luo_prototyyppi_lepakko(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_batb.bmp");
	strcpy(proto.nimi, "bat");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"grape.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_NONSTOP;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;
	for (int ai=4;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 6;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 41;
	proto.kuva_frame_korkeus = 30;
	proto.korkeus		= 30;
	proto.latausaika	= 15;
	proto.leveys		= 41;
	proto.max_hyppy		= 20;//20
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_HARMAA;//TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[3] = {4,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lepakko2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_batb.bmp");
	strcpy(proto.nimi, "bomber bat");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"bomb.spr");
	strcpy(proto.ammus1_sprite,"bomb.spr");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;
	proto.AI[4]			= AI_HYOKKAYS_1_JOS_PELAAJA_ALAPUOLELLA;
	proto.AI[5]			= AI_EI;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 6;
	proto.kuva_x		= 1;
	proto.kuva_y		= 34;
	proto.kuva_frame_leveys  = 41;
	proto.kuva_frame_korkeus = 44;
	proto.korkeus		= 44;
	proto.latausaika	= 40;
	proto.leveys		= 41;
	proto.max_hyppy		= 20;//20
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 30;
	proto.suojaus		= VAHINKO_TULI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_VIOLETTI;//TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[3] = {4,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lepakko3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_batb.bmp");
	strcpy(proto.nimi, "evil bomber bat");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"bomb.spr");
	strcpy(proto.ammus1_sprite,"bomb.spr");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_SEURAA_PELAAJAA;
	proto.AI[4]			= AI_HYOKKAYS_1_JOS_PELAAJA_ALAPUOLELLA;
	proto.AI[5]			= AI_EI;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 6;
	proto.kuva_x		= 1;
	proto.kuva_y		= 34;
	proto.kuva_frame_leveys  = 41;
	proto.kuva_frame_korkeus = 44;
	proto.korkeus		= 44;
	proto.latausaika	= 40;
	proto.leveys		= 41;
	proto.max_hyppy		= 20;//20
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 30;
	proto.suojaus		= VAHINKO_TULI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_VIOLETTI;//TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_VIOLETTI;
	proto.vihollinen	= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[3] = {4,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lintu(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bird.bmp");
	strcpy(proto.nimi, "bad birdy");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"egg.spr");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "chick.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_NONSTOP;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[4]			= AI_HYOKKAYS_1_JOS_PELAAJA_ALAPUOLELLA;

	proto.animaatioita	= 4;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 9;
	proto.frame_rate	= 6;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 29;
	proto.kuva_frame_korkeus = 26;
	proto.korkeus		= 26;
	proto.latausaika	= 50;
	proto.leveys		= 29;
	proto.max_hyppy		= 20;//20
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO + TUHOUTUMINEN_HOYHENET;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	
	UCHAR seq6[3] = {7,8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[4] = {7,8,9,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {4,5,6,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lintu2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bird.bmp");
	strcpy(proto.nimi, "evil birdy");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"grape.spr");
	strcpy(proto.ammus1_sprite,"egg.spr");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "chick.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	//proto.AI[2]			= AI_NONSTOP;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[4]			= AI_HYOKKAYS_1_JOS_PELAAJA_ALAPUOLELLA;
	proto.AI[2]			= AI_SEURAA_PELAAJAA;

	proto.animaatioita	= 4;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 9;
	proto.frame_rate	= 6;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 29;
	proto.kuva_frame_korkeus = 26;
	proto.korkeus		= 26;
	proto.latausaika	= 50;
	proto.leveys		= 29;
	proto.max_hyppy		= 20;//20
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 30;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO + TUHOUTUMINEN_HOYHENET;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_VIOLETTI;
	proto.vihollinen	= true;

	UCHAR seq1[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	
	UCHAR seq6[3] = {7,8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[4] = {7,8,9,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {4,5,6,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lintu3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bird.bmp");
	strcpy(proto.nimi, "still birdy");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"grape.spr");
	strcpy(proto.ammus1_sprite,"egg.spr");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "chick.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_HYOKKAYS_1_JOS_PELAAJA_ALAPUOLELLA;
	proto.AI[2]			= AI_SEURAA_PELAAJAA;

	proto.animaatioita	= 4;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 9;
	proto.frame_rate	= 6;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 29;
	proto.kuva_frame_korkeus = 26;
	proto.korkeus		= 26;
	proto.latausaika	= 50;
	proto.leveys		= 29;
	proto.max_hyppy		= 20;//20
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO + TUHOUTUMINEN_HOYHENET;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= true;

	UCHAR seq1[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	
	UCHAR seq6[3] = {7,8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[4] = {7,8,9,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {4,5,6,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lintu4(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bird.bmp");
	strcpy(proto.nimi, "friendly birdy");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"egg.spr");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "chick.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "chick.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_HYOKKAYS_1_JOS_PELAAJA_ALAPUOLELLA;
	proto.AI[2]			= AI_SEURAA_PELAAJAA;

	proto.animaatioita	= 4;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 9;
	proto.frame_rate	= 6;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 29;
	proto.kuva_frame_korkeus = 26;
	proto.korkeus		= 26;
	proto.latausaika	= 50;
	proto.leveys		= 29;
	proto.max_hyppy		= 20;//20
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO + TUHOUTUMINEN_HOYHENET;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_TURKOOSI;
	proto.vihollinen	= false;

	UCHAR seq1[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);

	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	
	UCHAR seq6[3] = {7,8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[4] = {7,8,9,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[5] = {4,5,6,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kala1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_fisb.bmp");
	strcpy(proto.nimi, "fish");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_NONSTOP;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 7;
	proto.frame_rate	= 8;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 36;
	proto.kuva_frame_korkeus = 28;
	proto.korkeus		= 28;
	proto.latausaika	= 15;
	proto.leveys		= 36;
	proto.max_hyppy		= 20;//20
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 5;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;//TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[7] = {1,1,1,2,2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[3] = {4,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[3] = {5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[3] = {3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kala2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_fisb.bmp");
	strcpy(proto.nimi, "evil fish");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SEURAA_PELAAJAA;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 7;
	proto.frame_rate	= 8;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 36;
	proto.kuva_frame_korkeus = 28;
	proto.korkeus		= 28;
	proto.latausaika	= 15;
	proto.leveys		= 36;
	proto.max_hyppy		= 20;//20
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 9;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;//TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_VIOLETTI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[7] = {1,1,1,2,2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[3] = {4,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[3] = {5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[3] = {3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kala3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_fisb.bmp");
	strcpy(proto.nimi, "guard fish");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;
	proto.AI[2]			= AI_PALAA_ALKUUN_X;
	proto.AI[3]			= AI_RANDOM_KAANTYMINEN;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 7;
	proto.frame_rate	= 8;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 36;
	proto.kuva_frame_korkeus = 28;
	proto.korkeus		= 28;
	proto.latausaika	= 15;
	proto.leveys		= 36;
	proto.max_hyppy		= 20;//20
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 7;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;//TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[7] = {1,1,1,2,2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[3] = {4,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[3] = {5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[3] = {3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_mehilainen(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_fisb.bmp");
	strcpy(proto.nimi, "bee");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"bee_ko.wav");
	strcpy(proto.aanitiedostot[AANI_RANDOM],	  "bee.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SEURAA_PELAAJAA_JOS_NAKEE_VERT_HORI;
	proto.AI[2]			= AI_PALAA_ALKUUN_X;
	proto.AI[3]			= AI_PALAA_ALKUUN_Y;
	proto.AI[4]			= AI_RANDOM_LIIKAHDUS_VERT_HORI;
	proto.AI[5]			= AI_VAHINGOITTUU_VEDESTA;
	/*
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;
	proto.AI[4]			= AI_VAROO_KUOPPAA;
	proto.AI[5]			= AI_PALAA_ALKUUN_X;//AI_NONSTOP;
	*/

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 32;
	proto.kuva_frame_leveys  = 10;
	proto.kuva_frame_korkeus = 10;
	proto.korkeus		= 10;
	proto.latausaika	= 15;
	proto.leveys		= 10;
	proto.max_hyppy		= 10;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 1;
	proto.suojaus		= VAHINKO_PISTO;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_ORANSSI;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_PISTO;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);

	UCHAR seq2[3] = {3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq2, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_mehilainen2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_fisb.bmp");
	strcpy(proto.nimi, "angry bee");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"bee_ko.wav");
	strcpy(proto.aanitiedostot[AANI_RANDOM],	  "bee.wav");


	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SEURAA_PELAAJAA_VERT_HORI;
	proto.AI[2]			= AI_VAHINGOITTUU_VEDESTA;
	proto.AI[3]			= AI_TAPA_KAIKKI;
	/*
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;
	proto.AI[4]			= AI_VAROO_KUOPPAA;
	proto.AI[5]			= AI_PALAA_ALKUUN_X;//AI_NONSTOP;
	*/

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 32;
	proto.kuva_frame_leveys  = 10;
	proto.kuva_frame_korkeus = 10;
	proto.korkeus		= 10;
	proto.latausaika	= 15;
	proto.leveys		= 10;
	proto.max_hyppy		= 10;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 1;
	proto.suojaus		= VAHINKO_PISTO;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_PISTO;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);

	UCHAR seq2[3] = {3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq2, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_mehilainen3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_fisb.bmp");
	strcpy(proto.nimi, "friendly bee");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"bee_ko.wav");
	strcpy(proto.aanitiedostot[AANI_RANDOM],	  "bee.wav");


	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SEURAA_PELAAJAA_VERT_HORI;
	//proto.AI[2]			= AI_VAHINGOITTUU_VEDESTA;
	/*
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;
	proto.AI[4]			= AI_VAROO_KUOPPAA;
	proto.AI[5]			= AI_PALAA_ALKUUN_X;//AI_NONSTOP;
	*/

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1000;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 32;
	proto.kuva_frame_leveys  = 10;
	proto.kuva_frame_korkeus = 10;
	proto.korkeus		= 10;
	proto.latausaika	= 15;
	proto.leveys		= 10;
	proto.max_hyppy		= 10;
	proto.max_nopeus	= 3;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_PISTO;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 3;
	proto.vahinko_tyyppi= VAHINKO_PISTO;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= false;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);

	UCHAR seq2[3] = {3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq2, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_mehilaispesa(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_fisb.bmp");
	strcpy(proto.nimi, "beehive");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"bee2.spr");
	strcpy(proto.ammus1_sprite,"bee2.spr");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"beehive.wav");
	strcpy(proto.aanitiedostot[AANI_RANDOM],	  "beehive.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]			= AI_HYOKKAYS_1_JOS_OSUTTU;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 5;
	proto.energia		= 4;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 45;
	proto.kuva_frame_leveys  = 27;
	proto.kuva_frame_korkeus = 30;
	proto.korkeus		= 30;
	proto.latausaika	= 30;
	proto.leveys		= 27;
	proto.max_hyppy		= 17;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 100;
	proto.suojaus		= VAHINKO_PISTO;
	proto.tiletarkistus = false;
	proto.tarisee       = true;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_mehilaispesa2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_fisb.bmp");
	strcpy(proto.nimi, "beehive 2");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"bee2.spr");
	strcpy(proto.ammus1_sprite,"bee2.spr");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"beehive.wav");
	strcpy(proto.aanitiedostot[AANI_RANDOM],	  "beehive.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 5;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 45;
	proto.kuva_frame_leveys  = 27;
	proto.kuva_frame_korkeus = 30;
	proto.korkeus		= 30;
	proto.latausaika	= 30;
	proto.leveys		= 27;
	proto.max_hyppy		= 17;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_PISTO;
	proto.tiletarkistus = false;
	proto.tarisee       = true;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}
/*
void luo_prototyyppi_jattimehilainen(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_fisb.bmp");
	strcpy(proto.nimi, "giant bee");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO],	  "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"bee_ko.wav");
	strcpy(proto.aanitiedostot[AANI_RANDOM],	  "bee.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SEURAA_PELAAJAA_JOS_NAKEE_VERT_HORI;
	proto.AI[2]			= AI_PALAA_ALKUUN_X;
	proto.AI[3]			= AI_PALAA_ALKUUN_Y;
	proto.AI[4]			= AI_RANDOM_LIIKAHDUS_VERT_HORI;
	proto.AI[5]			= AI_VAHINGOITTUU_VEDESTA;

	proto.aani_frq		= 15000;
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 3;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 2;
	proto.kuva_x		= 53;
	proto.kuva_y		= 32;
	proto.kuva_frame_leveys  = 42;
	proto.kuva_frame_korkeus = 27;
	proto.korkeus		= 27;
	proto.latausaika	= 15;
	proto.leveys		= 42;
	proto.max_hyppy		= 10;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 40;
	proto.suojaus		= VAHINKO_PISTO;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_ORANSSI;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_PISTO;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq1, true);

	proto.Tallenna(tiedosto);
}
*/
void luo_prototyyppi_jattimehilainen(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_fisb.bmp");
	strcpy(proto.nimi, "giant bee");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO],	  "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"bee_ko.wav");
	strcpy(proto.aanitiedostot[AANI_RANDOM],	  "bee.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SEURAA_PELAAJAA_JOS_NAKEE_VERT_HORI;
	proto.AI[2]			= AI_PALAA_ALKUUN_X;
	proto.AI[3]			= AI_PALAA_ALKUUN_Y;
	proto.AI[4]			= AI_RANDOM_LIIKAHDUS_VERT_HORI;
	proto.AI[5]			= AI_VAHINGOITTUU_VEDESTA;
	/*
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;
	proto.AI[4]			= AI_VAROO_KUOPPAA;
	proto.AI[5]			= AI_PALAA_ALKUUN_X;//AI_NONSTOP;
	*/
	proto.aani_frq		= 15000;
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 10;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 2;
	proto.kuva_x		= 274;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 87;
	proto.kuva_frame_korkeus = 70;
	proto.korkeus		= 80;
	proto.latausaika	= 15;
	proto.leveys		= 60;
	proto.max_hyppy		= 10;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 100;
	proto.suojaus		= VAHINKO_PISTO;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_ORANSSI;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 2;
	proto.vahinko_tyyppi= VAHINKO_PISTO;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[3] = {1,2,0}; 
	UCHAR seq2[5] = {1,1,2,2,0}; 
	UCHAR seq3[2] = {3,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq3, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq2, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_jattihiiri(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_mous.bmp");
	strcpy(proto.nimi, "giant mouse");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");//tennisb.spr
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"tennisb.spr");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],  "ko1.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"tennis.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"tennis.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SEURAA_PELAAJAA;
	proto.AI[2]			= AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA;
	proto.AI[3]			= AI_VAIHDA_KALLOT_JOS_TYRMATTY;
	proto.AI[4]			= AI_RANDOM_HYPPY;
	
	proto.aani_frq		= 11050;
	proto.animaatioita	= 5;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 20;
	proto.este			= false;
	proto.frameja		= 7;
	proto.frame_rate	= 7;
	proto.hyokkays1_aika= 9;
	proto.hyokkays2_aika= 9;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 71;
	proto.kuva_frame_korkeus = 67;
	proto.korkeus		= 67;//58
	proto.latausaika	= 80;//40
	proto.leveys		= 71;
	proto.max_hyppy		= 30;
	proto.max_nopeus	= 0;
	proto.paino			= 3;
	proto.random_frq	= true;
	proto.pisteet		= 100;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tarisee       = false;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 3;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[8] = {1,1,1,1,2,2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[5] = {6,6,7,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, true);

	UCHAR seq8[9] = {3,3,4,4,4,4,4,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_jattihiiri2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_mous.bmp");
	strcpy(proto.nimi, "giant mouse 2");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");//tennisb.spr
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"tennisb.spr");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],  "ko1.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SAMMAKKO2;
	proto.AI[2]			= AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA;
	proto.AI[3]			= AI_VAIHDA_KALLOT_JOS_TYRMATTY;
	proto.AI[4]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[5]			= AI_KAANTYY_JOS_OSUTTU;
	proto.AI[6]			= AI_KITKA_VAIKUTTAA;
	
	proto.aani_frq		= 11050;
	proto.animaatioita	= 5;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 20;
	proto.este			= false;
	proto.frameja		= 7;
	proto.frame_rate	= 7;
	proto.hyokkays1_aika= 9;
	proto.hyokkays2_aika= 9;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 71;
	proto.kuva_frame_korkeus = 67;
	proto.korkeus		= 67;//58
	proto.latausaika	= 120;//40
	proto.leveys		= 71;
	proto.max_hyppy		= 43;
	proto.max_nopeus	= 2;
	proto.paino			= 1;
	proto.random_frq	= true;
	proto.pisteet		= 100;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tarisee       = false;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 3;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= true;

	UCHAR seq1[8] = {1,1,1,1,2,2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[5] = {6,6,7,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, true);

	UCHAR seq8[9] = {3,3,4,4,4,4,4,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_jattihiiri3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_mous.bmp");
	strcpy(proto.nimi, "giant ice mouse");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");//tennisb.spr
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"tennisb.spr");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],  "ko1.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SEURAA_PELAAJAA;
	proto.AI[2]			= AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA;
	proto.AI[3]			= AI_VAIHDA_KALLOT_JOS_TYRMATTY;
	proto.AI[4]			= AI_RANDOM_HYPPY;
	
	proto.aani_frq		= 11050;
	proto.animaatioita	= 5;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 20;
	proto.este			= false;
	proto.frameja		= 7;
	proto.frame_rate	= 7;
	proto.hyokkays1_aika= 9;
	proto.hyokkays2_aika= 9;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 71;
	proto.kuva_frame_korkeus = 67;
	proto.korkeus		= 67;//58
	proto.latausaika	= 80;//40
	proto.leveys		= 71;
	proto.max_hyppy		= 30;
	proto.max_nopeus	= 0;
	proto.paino			= 3;
	proto.random_frq	= true;
	proto.pisteet		= 100;
	proto.suojaus		= VAHINKO_LUMI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tarisee       = false;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 3;
	proto.vahinko_tyyppi= VAHINKO_LUMI;
	proto.vari			= VARI_SININEN;
	proto.vihollinen	= true;

	UCHAR seq1[8] = {1,1,1,1,2,2,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[5] = {6,6,7,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, true);

	UCHAR seq8[9] = {3,3,4,4,4,4,4,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_jattirobokana_muoto1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_mach.bmp");
	strcpy(proto.nimi, "walking metal hen");
	strcpy(proto.muutos_sprite,"roboss2.spr");
	strcpy(proto.bonus_sprite,"");//tennisb.spr
	strcpy(proto.ammus1_sprite,"robohead.spr");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"boss2_dm.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],  "boss2_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_MUUTOS_AJASTIN;
	proto.AI[2]			= AI_VAIHDA_KALLOT_JOS_TYRMATTY;
	proto.AI[3]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[4]			= AI_NONSTOP;
	proto.AI[5]			= AI_SAMMAKKO1;
	//proto.AI[4]			= AI_HYOKKAYS_1_NONSTOP;
	
	/*
	proto.AI[1]			= AI_SAMMAKKO2;
	proto.AI[2]			= AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA;
	proto.AI[3]			= AI_VAIHDA_KALLOT_JOS_TYRMATTY;
	proto.AI[4]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[5]			= AI_KAANTYY_JOS_OSUTTU;
	proto.AI[6]			= AI_KITKA_VAIKUTTAA;*/
	
	proto.aani_frq		= 11050;
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 40;
	proto.este			= false;
	proto.frameja		= 6;
	proto.frame_rate	= 5;
	proto.hyokkays1_aika= 100;
	proto.hyokkays2_aika= 100;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 78;
	proto.kuva_frame_korkeus = 78;
	proto.korkeus		= 78;
	proto.latausaika	= 400;
	proto.leveys		= 78;
	proto.max_hyppy		= 43;
	proto.max_nopeus	= 4;
	proto.paino			= 2;
	proto.random_frq	= true;
	proto.pisteet		= 500;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_ORANSSI;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 3;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {2,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, true);

	UCHAR seq8[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_jattirobokana_muoto2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_mach.bmp");
	strcpy(proto.nimi, "flying metal hen");
	strcpy(proto.muutos_sprite,"roboss3.spr");
	strcpy(proto.bonus_sprite,"");//tennisb.spr
	strcpy(proto.ammus1_sprite,"robohead.spr");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"boss2_dm.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],  "boss2_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_MUUTOS_AJASTIN;
	proto.AI[2]			= AI_VAIHDA_KALLOT_JOS_TYRMATTY;
	proto.AI[3]			= AI_SEURAA_PELAAJAA;
	
	//proto.AI[3]			= AI_HYOKKAYS_1_JOS_PELAAJA_EDESSA;
	/*
	proto.AI[4]			= AI_MUUTOS_AJASTIN;
	/*
	proto.AI[1]			= AI_SAMMAKKO2;
	proto.AI[2]			= AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA;
	proto.AI[3]			= AI_VAIHDA_KALLOT_JOS_TYRMATTY;
	proto.AI[4]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[5]			= AI_KAANTYY_JOS_OSUTTU;
	proto.AI[6]			= AI_KITKA_VAIKUTTAA;*/
	
	proto.aani_frq		= 11050;
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 40;
	proto.este			= false;
	proto.frameja		= 6;
	proto.frame_rate	= 5;
	proto.hyokkays1_aika= 20;
	proto.hyokkays2_aika= 20;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 78;
	proto.kuva_frame_korkeus = 78;
	proto.korkeus		= 78;
	proto.latausaika	= 300;
	proto.leveys		= 78;
	proto.max_hyppy		= 20;
	proto.max_nopeus	= 3;
	proto.paino			= -0.1;
	proto.random_frq	= true;
	proto.pisteet		= 500;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_ORANSSI;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 3;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {2,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, true);

	UCHAR seq8[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_jattirobokana_muoto3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_mach.bmp");
	strcpy(proto.nimi, "static metal hen");
	strcpy(proto.muutos_sprite,"roboss1.spr");
	strcpy(proto.bonus_sprite,"");//tennisb.spr
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"boss2_dm.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],  "boss2_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_MUUTOS_AJASTIN;
	proto.AI[2]			= AI_VAIHDA_KALLOT_JOS_TYRMATTY;
	
	proto.aani_frq		= 11050;
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 40;
	proto.este			= false;
	proto.frameja		= 6;
	proto.frame_rate	= 5;
	proto.hyokkays1_aika= 20;
	proto.hyokkays2_aika= 20;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 78;
	proto.kuva_frame_korkeus = 78;
	proto.korkeus		= 78;
	proto.latausaika	= 130;
	proto.leveys		= 78;
	proto.max_hyppy		= 20;
	proto.max_nopeus	= 0;
	proto.paino			= 2;
	proto.random_frq	= true;
	proto.pisteet		= 500;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_ORANSSI;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 3;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, true);

	UCHAR seq8[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_jattirobokana_muoto4(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_mach.bmp");
	strcpy(proto.nimi, "metal hen 2");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");//tennisb.spr
	strcpy(proto.ammus1_sprite,"robohen.spr");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"boss2_dm.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],  "boss2_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_HYOKKAYS_1_NONSTOP;
	
	proto.aani_frq		= 11050;
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 6;
	proto.frame_rate	= 5;
	proto.hyokkays1_aika= 600;
	proto.hyokkays2_aika= 20;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 78;
	proto.kuva_frame_korkeus = 78;
	proto.korkeus		= 78;
	proto.latausaika	= 600;
	proto.leveys		= 78;
	proto.max_hyppy		= 20;
	proto.max_nopeus	= 0;
	proto.paino			= 2;
	proto.random_frq	= true;
	proto.pisteet		= 500;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_ORANSSI;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, true);

	UCHAR seq8[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_jattirobokana_muoto5(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_mach.bmp");
	strcpy(proto.nimi, "metal hen 3");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");//tennisb.spr
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"boss2_dm.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],  "boss2_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	
	proto.aani_frq		= 11050;
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 6;
	proto.frame_rate	= 5;
	proto.hyokkays1_aika= 20;
	proto.hyokkays2_aika= 20;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 78;
	proto.kuva_frame_korkeus = 78;
	proto.korkeus		= 78;
	proto.latausaika	= 130;
	proto.leveys		= 78;
	proto.max_hyppy		= 20;
	proto.max_nopeus	= 0;
	proto.paino			= 2;
	proto.random_frq	= true;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_ORANSSI;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, true);

	UCHAR seq8[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_liekki(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_batb.bmp");
	strcpy(proto.nimi, "flame");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_POMMI;
	proto.AI[2]			= AI_VAHINGOITTUU_VEDESTA;
	proto.AI[3]			= AI_TAPA_KAIKKI;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 3;
	proto.frame_rate	= 8;
	proto.kuva_x		= 1;
	proto.kuva_y		= 112;
	proto.kuva_frame_leveys  = 28;
	proto.kuva_frame_korkeus = 30;
	proto.korkeus		= 30;
	proto.latausaika	= 200;
	proto.leveys		= 28;
	proto.max_hyppy		= 5;
	proto.max_nopeus	= 0;
	proto.paino			= 0.5;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_TULI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_TULI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[4] = {1,2,3,0};  
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[4] = {1,2,3,0};  
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[4] = {1,2,3,0};  
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, true);

	UCHAR seq8[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_liekki2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_batb.bmp");
	strcpy(proto.nimi, "flame 2");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_VAHINGOITTUU_VEDESTA;
	proto.AI[2]			= AI_TAPA_KAIKKI;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 3;
	proto.frame_rate	= 8;
	proto.kuva_x		= 1;
	proto.kuva_y		= 112;
	proto.kuva_frame_leveys  = 28;
	proto.kuva_frame_korkeus = 30;
	proto.korkeus		= 30;
	proto.latausaika	= 200;
	proto.leveys		= 28;
	proto.max_hyppy		= 5;
	proto.max_nopeus	= 0;
	proto.paino			= 0.5;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_TULI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_TULI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[4] = {1,2,3,0};  
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[4] = {1,2,3,0};  
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[4] = {1,2,3,0};  
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, true);

	UCHAR seq8[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_liekki3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_batb.bmp");
	strcpy(proto.nimi, "jumping flame");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SAMMAKKO2;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_KITKA_VAIKUTTAA;
	proto.AI[4]			= AI_VAHINGOITTUU_VEDESTA;
	proto.AI[5]			= AI_TAPA_KAIKKI;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 3;
	proto.frame_rate	= 8;
	proto.kuva_x		= 1;
	proto.kuva_y		= 112;
	proto.kuva_frame_leveys  = 28;
	proto.kuva_frame_korkeus = 30;
	proto.korkeus		= 30;
	proto.latausaika	= 200;
	proto.leveys		= 28;
	proto.max_hyppy		= 5;
	proto.max_nopeus	= 2;
	proto.paino			= 0.5;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_TULI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_TULI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[4] = {1,2,3,0};  
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[4] = {1,2,3,0};  
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[4] = {1,2,3,0};  
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, true);

	UCHAR seq8[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_possu(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_pigs.bmp");
	strcpy(proto.nimi, "chilli pig");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"apple.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"doodlef2.spr");

	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "pig2.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "pig2.wav");
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "pig.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "pig_ko.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_NONSTOP;
	proto.AI[4]			= AI_VAROO_KUOPPAA;
//	proto.AI[5]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.energia		= 3;
	proto.este			= false;
	proto.frameja		= 18;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 34;
	proto.kuva_frame_korkeus = 40;
	proto.korkeus		= 40;
	proto.latausaika	= 30;
	proto.leveys		= 31;
	proto.max_hyppy		= 28;
	proto.max_nopeus	= 2;
	proto.paino			= 1.5;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.liitokyky     = false;

	UCHAR seq1[9] = {1,1,1,1,2,2,2,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[5] = {3,4,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {9,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {13,14,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[8] = {15,15,16,16,17,17,18,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[10] = {10,11,12,12,12,12,11,10,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_possu2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_pigs.bmp");
	strcpy(proto.nimi, "hot chilli pig");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"apple2.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"doodle_f.spr");

	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "pig2.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "pig2.wav");
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "pig.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "pig_ko.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_NONSTOP;
	proto.AI[4]			= AI_VAROO_KUOPPAA;
//	proto.AI[5]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.energia		= 5;
	proto.este			= false;
	proto.frameja		= 18;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 34;
	proto.kuva_frame_korkeus = 40;
	proto.korkeus		= 40;
	proto.latausaika	= 30;
	proto.leveys		= 31;
	proto.max_hyppy		= 28;
	proto.max_nopeus	= 2;
	proto.paino			= 1.5;
	proto.pisteet		= 50;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_ORANSSI;
	proto.vihollinen	= true;
	proto.liitokyky     = false;

	UCHAR seq1[9] = {1,1,1,1,2,2,2,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[5] = {3,4,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {8,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {9,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {13,14,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[8] = {15,15,16,16,17,17,18,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[10] = {10,11,12,12,12,12,11,10,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lentava_possu(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_pigs.bmp");
	strcpy(proto.nimi, "flying pig");
	strcpy(proto.muutos_sprite,"pig.spr");
	strcpy(proto.bonus_sprite,"orange.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"doodlef2.spr");

	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "pig2.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "pig2.wav");
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "pig.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_MUUTOS_JOS_ENERGIAA_ALLE_2;	
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA;
	proto.AI[4]			= AI_NONSTOP;
//	proto.AI[5]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.energia		= 3;
	proto.este			= false;
	proto.frameja		= 7;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 87;
	proto.kuva_frame_leveys  = 34;
	proto.kuva_frame_korkeus = 40;
	proto.korkeus		= 40;
	proto.latausaika	= 30;
	proto.leveys		= 34;
	proto.max_hyppy		= 15;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);

	UCHAR seq6[3] = {3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[3] = {3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[3] = {5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tankki1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_tank.bmp");
	strcpy(proto.nimi, "tank squirel");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"cheese.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"ammo1.spr");

	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "tankshot.wav");
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	//proto.AI[1]			= AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA;
	proto.AI[1]			= AI_HYOKKAYS_2_NONSTOP;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_NONSTOP;
	proto.AI[4]			= AI_VAROO_KUOPPAA;
	proto.AI[5]			= AI_VAHINGOITTUU_VEDESTA;
	proto.AI[6]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.energia		= 3;
	proto.este			= false;
	proto.frameja		= 18;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 43;
	proto.kuva_frame_korkeus = 40;
	proto.korkeus		= 40;
	proto.latausaika	= 100;
	proto.leveys		= 43;
	proto.max_hyppy		= 28;
	proto.max_nopeus	= 1;
	proto.paino			= 1.5;
	proto.pisteet		= 50;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tankki2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_tank.bmp");
	strcpy(proto.nimi, "tank squirel 2");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"cheese.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"ammo1.spr");

	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "tankshot.wav");
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_HYOKKAYS_2_NONSTOP;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_NONSTOP;
	proto.AI[4]			= AI_VAROO_KUOPPAA;
	proto.AI[5]			= AI_VAHINGOITTUU_VEDESTA;
	proto.AI[6]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.energia		= 3;
	proto.este			= false;
	proto.frameja		= 18;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 43;
	proto.kuva_frame_korkeus = 40;
	proto.korkeus		= 40;
	proto.latausaika	= 100;
	proto.leveys		= 43;
	proto.max_hyppy		= 28;
	proto.max_nopeus	= 1;
	proto.paino			= 1.5;
	proto.pisteet		= 50;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tankki3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_tank.bmp");
	strcpy(proto.nimi, "tank squirel 3");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"cheese.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"ammo2.spr");

	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "tankshot.wav");
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_HYOKKAYS_2_NONSTOP;
	proto.AI[2]			= AI_SEURAA_PELAAJAA;
	proto.AI[3]			= AI_VAHINGOITTUU_VEDESTA;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.energia		= 3;
	proto.este			= false;
	proto.frameja		= 18;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 43;
	proto.kuva_frame_korkeus = 40;
	proto.korkeus		= 40;
	proto.latausaika	= 100;
	proto.leveys		= 43;
	proto.max_hyppy		= 28;
	proto.max_nopeus	= 0;
	proto.paino			= 1.5;
	proto.pisteet		= 50;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tankki4(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_tank.bmp");
	strcpy(proto.nimi, "evil tank 3");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"cheese.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"ammo1.spr");

	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "tankshot.wav");
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA;
	proto.AI[2]			= AI_SEURAA_PELAAJAA;
	proto.AI[3]			= AI_VAHINGOITTUU_VEDESTA;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.energia		= 3;
	proto.este			= false;
	proto.frameja		= 18;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 43;
	proto.kuva_frame_korkeus = 40;
	proto.korkeus		= 40;
	proto.latausaika	= 100;
	proto.leveys		= 43;
	proto.max_hyppy		= 28;
	proto.max_nopeus	= 1;
	proto.paino			= 1.5;
	proto.pisteet		= 50;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_VIOLETTI;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[3] = {4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);

	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_aave1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_batb.bmp");
	strcpy(proto.nimi, "ghost 1");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[3]			= AI_NONSTOP;
	proto.AI[4]			= AI_LIIKKUU_Y_SIN_VAPAA;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 7;
	proto.kuva_x		= 1;
	proto.kuva_y		= 145;
	proto.kuva_frame_leveys  = 38;
	proto.kuva_frame_korkeus = 33;
	proto.korkeus		= 33;
	proto.latausaika	= 10;
	proto.leveys		= 38;
	proto.max_hyppy		= 1;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_SAHKO;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[3] = {1,2,0}; 
	UCHAR seq2[3] = {3,4,0};
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq2, true);
	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_pahakana(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_evil.bmp");
	strcpy(proto.nimi, "the evil one");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "crow.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "crow_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "crow.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "crow.wav");
	strcpy(proto.aanitiedostot[AANI_RANDOM], "crow.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_SEURAA_PELAAJAA;
	proto.AI[3]			= AI_RANDOM_HYPPY;
	proto.AI[4]			= AI_EVIL_ONE;
	//proto.AI[3]			= AI_VAIHDA_KALLOT_JOS_TYRMATTY;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 7;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 38;
	proto.kuva_frame_korkeus = 46;
	proto.korkeus		= 46;
	proto.latausaika	= 15;
	proto.leveys		= 31;
	proto.max_hyppy		= 16;
	proto.max_nopeus	= 0;
	proto.paino			= 0.5;
	proto.pisteet		= 1000;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[9] = {1,1,1,2,1,2,1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[5] = {4,4,3,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);
	UCHAR seq3[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq1, true);
	UCHAR seq4[3] = {6,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq4, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_pahakana2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_evil.bmp");
	strcpy(proto.nimi, "the evil one 2");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"robohen.spr");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "crow.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "crow_ko.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1], "crow.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2], "crow.wav");
	strcpy(proto.aanitiedostot[AANI_RANDOM], "crow.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_SEURAA_PELAAJAA;
	proto.AI[3]			= AI_RANDOM_HYPPY;
	proto.AI[4]			= AI_EVIL_ONE;
	proto.AI[5]			= AI_HYOKKAYS_1_NONSTOP;
	//proto.AI[3]			= AI_VAIHDA_KALLOT_JOS_TYRMATTY;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 7;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 38;
	proto.kuva_frame_korkeus = 46;
	proto.korkeus		= 46;
	proto.hyokkays1_aika= 600;
	proto.latausaika	= 600;
	proto.leveys		= 31;
	proto.max_hyppy		= 16;
	proto.max_nopeus	= 0;
	proto.paino			= 0.5;
	proto.pisteet		= 1000;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[9] = {1,1,1,2,1,2,1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[5] = {4,4,3,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);
	UCHAR seq3[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq1, true);
	UCHAR seq4[3] = {6,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq4, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_luut1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bone.bmp");
	strcpy(proto.nimi, "chicken bones 1");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");//egg.spr
	strcpy(proto.ammus2_sprite,"");//doodle.spr
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_VAROO_KUOPPAA;
	
	proto.aani_frq		= 22050;
	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 13;
	proto.frame_rate	= 8;
	proto.hyokkays1_aika= 30;
	proto.hyokkays2_aika= 60;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 37;
	proto.kuva_frame_korkeus = 45;
	proto.korkeus		= 45;//58
	proto.latausaika	= 0;//60
	proto.leveys		= 30;
	proto.max_hyppy		= 26;//24
	proto.max_nopeus	= 1;
	proto.paino			= 1;
	proto.random_frq	= true;
	proto.pisteet		= 25;
	proto.suojaus		= VAHINKO_MELU;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO + TUHOUTUMINEN_SAVUPILVET;
	proto.tarisee       = false;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_SAHKO;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.lapinakyvyys	= false;
	proto.hehkuu		= false;
	proto.tulitauko		= 0;
	proto.liitokyky		= true;
	proto.boss			= false;
	proto.bonus_aina	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[5] = {2,3,4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[5] = {6,6,7,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[5] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[4] = {8,8,9,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {10,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[6] = {11,11,12,12,13,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[3] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);

	UCHAR seq9[3] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq9, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_luut2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bone.bmp");
	strcpy(proto.nimi, "chicken bones 2");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");//egg.spr
	strcpy(proto.ammus2_sprite,"");//doodle.spr
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI_VERT;
	proto.AI[2]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[3]			= AI_NONSTOP;
	proto.AI[4]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[5]			= AI_KAANTYY_ESTEESTA_VERT;
	
	proto.aani_frq		= 22050;
	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 13;
	proto.frame_rate	= 10;
	proto.hyokkays1_aika= 30;
	proto.hyokkays2_aika= 60;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 37;
	proto.kuva_frame_korkeus = 45;
	proto.korkeus		= 45;//58
	proto.latausaika	= 0;//60
	proto.leveys		= 30;
	proto.max_hyppy		= 26;//24
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.random_frq	= true;
	proto.pisteet		= 25;
	proto.suojaus		= VAHINKO_MELU;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO + TUHOUTUMINEN_SAVUPILVET;
	proto.tarisee       = false;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_SAHKO;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.lapinakyvyys	= false;
	proto.hehkuu		= false;
	proto.tulitauko		= 0;
	proto.liitokyky		= true;
	proto.boss			= false;
	proto.bonus_aina	= false;

	UCHAR seq1[3] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[3] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[5] = {6,6,7,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[5] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[4] = {8,8,9,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[2] = {10,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq7[7] = {10,11,11,12,12,13,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, false);

	UCHAR seq8[3] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);

	UCHAR seq9[3] = {6,7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq9, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_robokana(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_robo.bmp");
	strcpy(proto.nimi, "robot hen");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"robohead.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "robo_dmg.wav");
	strcpy(proto.aanitiedostot[AANI_RANDOM], "robo_rnd.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "explot.wav");

	proto.AI[0]			= AI_PIKKUKANA;
	proto.AI[1]			= AI_SEURAA_PELAAJAA_JOS_NAKEE;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[3]			= AI_VAROO_KUOPPAA;
	proto.AI[4]			= AI_VAHINGOITTUU_VEDESTA;

	proto.animaatioita	= 7;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 11;
	proto.frame_rate	= 5;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 39;
	proto.kuva_frame_korkeus = 36;
	proto.korkeus		= 36;
	proto.latausaika	= 5;
	proto.leveys		= 30;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 3;
	proto.paino			= 0.8;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_ORANSSI;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.liitokyky     = false;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	UCHAR seq2[5] = {2,3,4,5,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,	 seq2, true);

	UCHAR seq3[2] = {6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS, seq3, true);

	UCHAR seq4[2] = {7,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS, seq4, true);

	UCHAR seq5[4] = {8,9,10,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY, seq5, false);

	UCHAR seq6[2] = {11,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO, seq6, true);

	UCHAR seq7[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA, seq7, false);

	UCHAR seq8[3] = {3,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_robokanan_paa(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_robo.bmp");
	strcpy(proto.nimi, "head of robot hen");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_VAHINKO], "robo_dmg.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"explot.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SEURAA_PELAAJAA_VERT_HORI;
	proto.AI[2]			= AI_VAHINGOITTUU_VEDESTA;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 40;
	proto.kuva_frame_leveys  = 25;
	proto.kuva_frame_korkeus = 24;
	proto.korkeus		= 24;
	proto.latausaika	= 15;
	proto.leveys		= 25;
	proto.max_hyppy		= 10;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 5;
	proto.suojaus		= VAHINKO_ISKU;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[5] = {1,2,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);

	UCHAR seq2[3] = {3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq2, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_piikkipaa1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_shed.bmp");
	strcpy(proto.nimi, "spike head (slow circle)");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_LIIKKUU_X_COS_HIDAS;
	proto.AI[2]			= AI_LIIKKUU_Y_SIN_HIDAS;
	//proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	//proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 33;
	proto.kuva_frame_korkeus = 33;
	proto.korkeus		= 31;
	proto.latausaika	= 12;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_piikkipaa2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_shed.bmp");
	strcpy(proto.nimi, "spike head (up and down)");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_VERT;
	proto.AI[2]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI_VERT;
	//proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	//proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 33;
	proto.kuva_frame_korkeus = 33;
	proto.korkeus		= 31;
	proto.latausaika	= 12;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_piikkipaa3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_shed.bmp");
	strcpy(proto.nimi, "spike head (left and right)");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	//proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	//proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 33;
	proto.kuva_frame_korkeus = 33;
	proto.korkeus		= 31;
	proto.latausaika	= 12;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_piikkipaa4(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_shed.bmp");
	strcpy(proto.nimi, "spike head (bouncer)");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_VERT;
	proto.AI[3]			= AI_NONSTOP;
	proto.AI[4]			= AI_EI;
	proto.AI[5]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[6]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI_VERT;

	//proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	//proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 33;
	proto.kuva_frame_korkeus = 33;
	proto.korkeus		= 31;
	proto.latausaika	= 12;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_piikkipaa5(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_shed.bmp");
	strcpy(proto.nimi, "spike head (wall climber)");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KIIPEILIJA;
	//proto.AI[2]			= AI_NONSTOP;
	proto.AI[2]			= AI_EI;
	proto.AI[3]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	//proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	//proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 33;
	proto.kuva_frame_korkeus = 33;
	proto.korkeus		= 31;
	proto.latausaika	= 12;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_piikkipaa6(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_shed.bmp");
	strcpy(proto.nimi, "spike head (static)");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;

	//proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	//proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 5;
	proto.kuva_x		= 145;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 33;
	proto.kuva_frame_korkeus = 33;
	proto.korkeus		= 31;
	proto.latausaika	= 12;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[9] = {4,3,2,1,1,2,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);

	UCHAR seq2[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq2, true);
	
	UCHAR seq3[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq3, true);

	UCHAR seq8[9] = {1,1,1,1,1,2,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_piikkipaa7(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_shed.bmp");
	strcpy(proto.nimi, "spike head (pulser 1)");
	strcpy(proto.muutos_sprite,"spkhead8.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_MUUTOS_AJASTIN;

	//proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	//proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 5;
	proto.kuva_x		= 145;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 33;
	proto.kuva_frame_korkeus = 33;
	proto.korkeus		= 31;
	proto.latausaika	= 100;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[5] = {4,3,2,1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[9] = {1,1,1,1,1,2,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_piikkipaa8(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_shed.bmp");
	strcpy(proto.nimi, "spike head (pulser 2)");
	strcpy(proto.muutos_sprite,"spkhead7.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_MUUTOS_AJASTIN;

	//proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	//proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 5;
	proto.kuva_x		= 145;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 33;
	proto.kuva_frame_korkeus = 33;
	proto.korkeus		= 31;
	proto.latausaika	= 100;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[5] = {1,2,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	UCHAR seq2[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[2] = {4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_hamahakki1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_spid.bmp");
	strcpy(proto.nimi, "spider");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_VERT;
	proto.AI[2]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI_VERT;
	//proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	//proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 3;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 38;
	proto.kuva_frame_korkeus = 38;
	proto.korkeus		= 26;
	proto.latausaika	= 12;
	proto.leveys		= 26;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_ORANSSI;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_hamahakki2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_spid.bmp");
	strcpy(proto.nimi, "evil spider");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SEURAA_PELAAJAA_VERT_HORI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_VERT;
	proto.AI[3]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI_VERT;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 3;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 38;
	proto.kuva_frame_korkeus = 38;
	proto.korkeus		= 26;
	proto.latausaika	= 12;
	proto.leveys		= 26;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_VIOLETTI;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_VIOLETTI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_hamahakki3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_spid.bmp");
	strcpy(proto.nimi, "guard spider");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_SEURAA_PELAAJAA_JOS_NAKEE_VERT_HORI;
	proto.AI[2]			= AI_KAANTYY_ESTEESTA_VERT;
	proto.AI[3]			= AI_PALAA_ALKUUN_Y;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 3;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 38;
	proto.kuva_frame_korkeus = 38;
	proto.korkeus		= 26;
	proto.latausaika	= 12;
	proto.leveys		= 26;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_HARMAA;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_HARMAA;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[5] = {1,2,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_hissi1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_plat.bmp");
	strcpy(proto.nimi, "moving platform (left and right)");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[3]			= AI_NUOLET_VAIKUTTAVAT;
	//proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	//proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	
	proto.este			= true;
	proto.este_ylos		= false;
	proto.este_alas		= true;
	proto.este_oikealle = false;
	proto.este_vasemmalle = false;

	proto.frameja		= 3;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 31;
	proto.latausaika	= 12;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_hissi2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_plat.bmp");
	strcpy(proto.nimi, "moving platform (up and down)");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_VERT;
	proto.AI[2]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI_VERT;
	proto.AI[3]			= AI_NUOLET_VAIKUTTAVAT;
	//proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	//proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= true;
	proto.este_ylos		= false;
	proto.este_alas		= true;
	proto.este_oikealle = false;
	proto.este_vasemmalle = false;
	proto.frameja		= 2;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 31;
	proto.latausaika	= 12;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_hissi3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_plat.bmp");
	strcpy(proto.nimi, "moving platform (wall climber)");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KIIPEILIJA;
	//proto.AI[2]			= AI_NONSTOP;
	proto.AI[2]			= AI_EI;
	proto.AI[3]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;
	proto.AI[4]			= AI_NUOLET_VAIKUTTAVAT;

	//proto.AI[1]			= AI_ALOITUSSUUNTA_PELAAJAA_KOHTI;

	//proto.AI[2]			= AI_KAANTYY_ESTEESTA_HORI;
	
	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= true;
	proto.este_ylos		= false;
	proto.este_alas		= true;
	proto.este_oikealle = false;
	proto.este_vasemmalle = false;
	proto.frameja		= 3;
	proto.frame_rate	= 3;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 31;
	proto.latausaika	= 12;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 2;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 1;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[4] = {1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, true);
	
	UCHAR seq6[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, true);

	UCHAR seq8[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_nuoli_oikealle(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_arrw.bmp");
	strcpy(proto.nimi, "arrow right");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_NUOLI_OIKEALLE;
	
	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 19;
	proto.kuva_x		= 1;
	proto.kuva_y		= 36;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 31;
	proto.latausaika	= 0;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.osaa_uida		= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_nuoli_ylos(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_arrw.bmp");
	strcpy(proto.nimi, "arrow up");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_NUOLI_YLOS;
	
	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 19;
	proto.kuva_x		= 69;
	proto.kuva_y		= 36;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 31;
	proto.latausaika	= 0;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.osaa_uida		= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_nuoli_vasemmalle(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_arrw.bmp");
	strcpy(proto.nimi, "arrow left");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_NUOLI_VASEMMALLE;
	
	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 19;
	proto.kuva_x		= 137;
	proto.kuva_y		= 36;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 31;
	proto.latausaika	= 0;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.osaa_uida		= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_nuoli_alas(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_arrw.bmp");
	strcpy(proto.nimi, "arrow down");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_NUOLI_ALAS;
	
	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 19;
	proto.kuva_x		= 205;
	proto.kuva_y		= 36;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 31;
	proto.latausaika	= 0;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tiletarkistus = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	proto.osaa_uida		= true;

	UCHAR seq1[3] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_nuoli_oikealle2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_arrw.bmp");
	strcpy(proto.nimi, "arrow right 2");
	strcpy(proto.muutos_sprite,"arrow2_d.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_NUOLI_OIKEALLE;
	proto.AI[2]			= AI_MUUTOS_JOS_OSUTTU;
	
	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 10000;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 10;
	proto.kuva_x		= 1;
	proto.kuva_y		= 70;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 31;
	proto.latausaika	= 0;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tiletarkistus = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[7] = {4,3,2,1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_nuoli_ylos2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_arrw.bmp");
	strcpy(proto.nimi, "arrow up 2");
	strcpy(proto.muutos_sprite,"arrow2_r.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_NUOLI_YLOS;
	proto.AI[2]			= AI_MUUTOS_JOS_OSUTTU;
	
	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 10000;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 10;
	proto.kuva_x		= 1;
	proto.kuva_y		= 172;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 31;
	proto.latausaika	= 0;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tiletarkistus = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[7] = {4,3,2,1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_nuoli_vasemmalle2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_arrw.bmp");
	strcpy(proto.nimi, "arrow left 2");
	strcpy(proto.muutos_sprite,"arrow2_u.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_NUOLI_VASEMMALLE;
	proto.AI[2]			= AI_MUUTOS_JOS_OSUTTU;
	
	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 10000;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 10;
	proto.kuva_x		= 1;
	proto.kuva_y		= 138;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 31;
	proto.latausaika	= 0;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tiletarkistus = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[7] = {4,3,2,1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_nuoli_alas2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_arrw.bmp");
	strcpy(proto.nimi, "arrow down 2");
	strcpy(proto.muutos_sprite,"arrow2_l.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	//strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	//strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "ko1.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_NUOLI_ALAS;
	proto.AI[2]			= AI_MUUTOS_JOS_OSUTTU;
	
	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 10000;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 10;
	proto.kuva_x		= 1;
	proto.kuva_y		= 104;
	proto.kuva_frame_leveys  = 31;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 31;
	proto.latausaika	= 0;
	proto.leveys		= 31;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tiletarkistus = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.osaa_uida		= true;

	UCHAR seq1[7] = {4,3,2,1,2,3,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_kallokytkin1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_wall.bmp");
	strcpy(proto.nimi, "skull switch 1");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");//tennisb.spr
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],  "tennis.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_VAIHDA_KALLOT_JOS_TYRMATTY;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 11;
	proto.frame_rate	= 5;
	proto.hyokkays1_aika= 9;
	proto.hyokkays2_aika= 9;
	proto.kuva_x		= 1;
	proto.kuva_y		= 100;
	proto.kuva_frame_leveys  = 26;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 31;//58
	proto.latausaika	= 80;//40
	proto.leveys		= 26;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0.5;
	proto.random_frq	= true;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tarisee       = false;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[10] = {1,1,1,1,1,1,11,11,11,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[10] = {1,2,3,4,5,6,7,8,9,10}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, false);

	UCHAR seq7[10] = {1,2,3,4,5,6,7,8,9,10}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, true);

	UCHAR seq8[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, false);

	proto.Tallenna(tiedosto);
}


void luo_prototyyppi_kallokytkin2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_wall.bmp");
	strcpy(proto.nimi, "skull switch 2");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");//tennisb.spr
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],  "tennis.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_VAIHDA_KALLOT_JOS_OSUTTU;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 30000;
	proto.este			= false;
	proto.frameja		= 11;
	proto.frame_rate	= 5;
	proto.hyokkays1_aika= 9;
	proto.hyokkays2_aika= 9;
	proto.kuva_x		= 1;
	proto.kuva_y		= 100;
	proto.kuva_frame_leveys  = 26;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 31;//58
	proto.latausaika	= 80;//40
	proto.leveys		= 26;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0.5;
	proto.random_frq	= true;
	proto.pisteet		= 30000;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	proto.tarisee       = false;
	proto.tiletarkistus = true;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= VARI_ORANSSI;
	proto.vihollinen	= true;

	UCHAR seq1[10] = {1,1,1,1,1,1,11,11,11,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	UCHAR seq2[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq2, true);

	UCHAR seq3[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq3, true);

	UCHAR seq4[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq4, true);

	UCHAR seq5[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq5, false);
	
	UCHAR seq6[10] = {1,2,3,4,5,6,7,8,9,10}; 
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq6, false);

	UCHAR seq7[10] = {1,2,3,4,5,6,7,8,9,10}; 
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq7, true);

	UCHAR seq8[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq8, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq8, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_laatikko_appelsiini(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "box of orange");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"orange.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"boxcrack.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]         = AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 4;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 32;
	proto.latausaika	= 0;
	proto.leveys		= 32;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 2;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_ORANSSI;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_laatikko_megafooni(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "box of mega phone");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"megaphon.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"boxcrack.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]         = AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 32;
	proto.latausaika	= 0;
	proto.leveys		= 32;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 2;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_VIHREA;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_laatikko_juoma_muna(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "box of easter egg");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"eegg_egg.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"boxcrack.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]         = AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 32;
	proto.latausaika	= 0;
	proto.leveys		= 32;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 2;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_HARMAA;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_HARMAA;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_laatikko_juoma_tulimuna(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "box of fire egg");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"eegg_fir.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"boxcrack.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]         = AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 32;
	proto.latausaika	= 0;
	proto.leveys		= 32;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 2;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_laatikko_hoyhen(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "box of feather");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"feather.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"boxcrack.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]         = AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 32;
	proto.latausaika	= 0;
	proto.leveys		= 32;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 2;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_HARMAA;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_laatikko_kana(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "box of chicken");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"hen.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"boxcrack.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]			= AI_RANDOM_HYPPY;
	proto.AI[2]         = AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 2;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 32;
	proto.latausaika	= 0;
	proto.leveys		= 32;
	proto.max_hyppy		= 18;
	proto.max_nopeus	= 1;
	proto.paino			= 1;
	proto.pisteet		= 25;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;//true;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_HARMAA;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_laatikko_pikkukana(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "box of chick");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"smallhen.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"boxcrack.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]			= AI_RANDOM_HYPPY;
	proto.AI[2]         = AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 32;
	proto.latausaika	= 0;
	proto.leveys		= 32;
	proto.max_hyppy		= 13;
	proto.max_nopeus	= 1;
	proto.paino			= 1;
	proto.pisteet		= 50;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = true;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_laatikko_pikkukanoja(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "box of chicks");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"smallhen.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"boxcrack.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]			= AI_RANDOM_HYPPY;
	proto.AI[2]         = AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 3;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 32;
	proto.latausaika	= 0;
	proto.leveys		= 32;
	proto.max_hyppy		= 17;
	proto.max_nopeus	= 1;
	proto.paino			= 1;
	proto.pisteet		= 300;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = true;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_laatikko_siili(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "box of hedgehog");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"hedgehog.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"boxcrack.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[2]         = AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 32;
	proto.latausaika	= 0;
	proto.leveys		= 32;
	proto.max_hyppy		= 18;
	proto.max_nopeus	= 1;
	proto.paino			= 1;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;//true;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_HARMAA;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_laatikko_perhoset(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "box of butterflys");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"bttrfly2.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO], "damage.wav");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"boxcrack.wav");

	proto.AI[0]			= AI_BONUS;
	proto.AI[1]			= AI_RANDOM_HYPPY;
	proto.AI[2]         = AI_KITKA_VAIKUTTAA;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 9;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 32;
	proto.latausaika	= 0;
	proto.leveys		= 32;
	proto.max_hyppy		= 17;
	proto.max_nopeus	= 1;
	proto.paino			= 1;
	proto.pisteet		= 300;
	proto.suojaus		= VAHINKO_EI;
	proto.tarisee       = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_nippu_omenoita(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "5 apples");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"apple3.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"damage.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 5;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 36;
	proto.kuva_y		= 62;
	proto.kuva_frame_leveys  = 40;
	proto.kuva_frame_korkeus = 38;
	proto.korkeus		= 38;
	proto.latausaika	= 0;
	proto.leveys		= 40;
	proto.max_hyppy		= 17;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_EI;
	proto.tiletarkistus = false;
	proto.tarisee       = true;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_PUNAINEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_rypaleterttu(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "8 grapes");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"wgrape.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],"damage.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 8;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 556;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 26;
	proto.kuva_frame_korkeus = 41;
	proto.korkeus		= 41;
	proto.latausaika	= 0;
	proto.leveys		= 26;
	proto.max_hyppy		= 17;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 10;
	proto.suojaus		= VAHINKO_EI;
	proto.tiletarkistus = false;
	proto.tarisee       = true;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_VIHREA;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lahja_hoyhen(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "feather gift");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"feather.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus2.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 10;
	proto.frame_rate	= 5;
	proto.kuva_x		= 138;
	proto.kuva_y		= 90;
	proto.kuva_frame_leveys  = 21;
	proto.kuva_frame_korkeus = 21;
	proto.korkeus		= 21;
	proto.latausaika	= 0;
	proto.leveys		= 21;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 15;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[10] = {1,2,3,4,5,6,7,8,9,10}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lahja_siili(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "hedgehog gift");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"hedgehog.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus2.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 10;
	proto.frame_rate	= 5;
	proto.kuva_x		= 138;
	proto.kuva_y		= 90;
	proto.kuva_frame_leveys  = 21;
	proto.kuva_frame_korkeus = 21;
	proto.korkeus		= 21;
	proto.latausaika	= 0;
	proto.leveys		= 21;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 15;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[10] = {1,2,3,4,5,6,7,8,9,10}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lahja_pikkukana(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "gift small hen");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"smallhen.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus2.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 10;
	proto.frame_rate	= 5;
	proto.kuva_x		= 138;
	proto.kuva_y		= 90;
	proto.kuva_frame_leveys  = 21;
	proto.kuva_frame_korkeus = 21;
	proto.korkeus		= 21;
	proto.latausaika	= 0;
	proto.leveys		= 21;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 15;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[10] = {1,2,3,4,5,6,7,8,9,10}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lahja_kivikuutio(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "gift stone cube");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"stncube.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus2.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 10;
	proto.frame_rate	= 5;
	proto.kuva_x		= 138;
	proto.kuva_y		= 90;
	proto.kuva_frame_leveys  = 21;
	proto.kuva_frame_korkeus = 21;
	proto.korkeus		= 21;
	proto.latausaika	= 0;
	proto.leveys		= 21;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 15;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[10] = {1,2,3,4,5,6,7,8,9,10}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lahja(char *tiedosto, char *csprite, char *snimi)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, snimi);
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,csprite);
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus2.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 10;
	proto.frame_rate	= 5;
	proto.kuva_x		= 138;
	proto.kuva_y		= 90;
	proto.kuva_frame_leveys  = 21;
	proto.kuva_frame_korkeus = 21;
	proto.korkeus		= 21;
	proto.latausaika	= 0;
	proto.leveys		= 21;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 15;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[10] = {1,2,3,4,5,6,7,8,9,10}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lahja_doodle_tuli(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "chili-doodle gift");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"can_chil.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus2.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 10;
	proto.frame_rate	= 4;
	proto.kuva_x		= 138;
	proto.kuva_y		= 90;
	proto.kuva_frame_leveys  = 21;
	proto.kuva_frame_korkeus = 21;
	proto.korkeus		= 21;
	proto.latausaika	= 0;
	proto.leveys		= 21;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 15;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= false;

	UCHAR seq1[10] = {1,2,3,4,5,6,7,8,9,10}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_lahja_doodle_lumi(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "ice-doodle gift");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"can_icet.spr");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus2.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 10;
	proto.frame_rate	= 4;
	proto.kuva_x		= 138;// 110;
	proto.kuva_y		= 90;//1;
	proto.kuva_frame_leveys  = 21;
	proto.kuva_frame_korkeus = 21;
	proto.korkeus		= 21;
	proto.latausaika	= 0;
	proto.leveys		= 21;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 15;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_SININEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_SININEN;
	proto.vihollinen	= false;

	UCHAR seq1[10] = {1,2,3,4,5,6,7,8,9,10}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_taikajuoma_kana(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "rooster potion");
	strcpy(proto.muutos_sprite,"rooster.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus7.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
    proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 163;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 20;
	proto.kuva_frame_korkeus = 22;
	proto.korkeus		= 22;
	proto.latausaika	= 0;
	proto.leveys		= 20;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_SININEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_taikajuoma_pikkukana(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "little hen potion");
	strcpy(proto.muutos_sprite,"smallhen.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus7.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 163;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 20;
	proto.kuva_frame_korkeus = 22;
	proto.korkeus		= 22;
	proto.latausaika	= 0;
	proto.leveys		= 20;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_ORANSSI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_ORANSSI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_taikajuoma_possu(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "pig potion");
	strcpy(proto.muutos_sprite,"pig.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus7.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 163;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 20;
	proto.kuva_frame_korkeus = 22;
	proto.korkeus		= 22;
	proto.latausaika	= 0;
	proto.leveys		= 20;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_taikajuoma_lintu(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "bird potion");
	strcpy(proto.muutos_sprite,"bird4.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus7.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 163;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 20;
	proto.kuva_frame_korkeus = 22;
	proto.korkeus		= 22;
	proto.latausaika	= 0;
	proto.leveys		= 20;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 20;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_TURKOOSI;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_TURKOOSI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_taikajuoma_sammakko(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "frog potion");
	strcpy(proto.muutos_sprite,"frog5.spr");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus7.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 163;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 20;
	proto.kuva_frame_korkeus = 22;
	proto.korkeus		= 22;
	proto.latausaika	= 0;
	proto.leveys		= 20;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 60;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_VIHREA;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_juoma_muna(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "egg potion");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"egg.spr");
	strcpy(proto.ammus2_sprite,"");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus7.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
    proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 125;
	proto.kuva_frame_leveys  = 20;
	proto.kuva_frame_korkeus = 22;
	proto.korkeus		= 22;
	proto.latausaika	= 0;
	proto.leveys		= 20;
	proto.max_hyppy		= 20;
	proto.max_nopeus	= 1;
	proto.paino			= 0.5;
	proto.pisteet		= 70;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_HARMAA;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_juoma_tulimuna(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "fire egg potion");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"egg_fire.spr");
	strcpy(proto.ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], "");
		proto.aanet[aani] = -1;
	}

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus7.wav");

	proto.AI[0]			= AI_BONUS;

	for (int ai=1;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_EI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
    proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 125;
	proto.kuva_frame_leveys  = 20;
	proto.kuva_frame_korkeus = 22;
	proto.korkeus		= 22;
	proto.latausaika	= 0;
	proto.leveys		= 20;
	proto.max_hyppy		= 20;
	proto.max_nopeus	= 1;
	proto.paino			= 0.5;
	proto.pisteet		= 70;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_SAVU_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_megafooni(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "loud speaker");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"doodle.spr");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus1.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 10;
	proto.frame_rate	= 5;
	proto.kuva_x		= 138;//1;
	proto.kuva_y		= 62;//97;
	proto.kuva_frame_leveys  = 29;
	proto.kuva_frame_korkeus = 25;
	proto.korkeus		= 25;
	proto.latausaika	= 0;
	proto.leveys		= 29;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 0;
	proto.paino			= 0.5;
	proto.pisteet		= 15;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_SININEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[10] = {1,2,3,4,5,6,7,8,9,10}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_doodle_chili(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "fire doodle drink");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"doodle_f.spr");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus1.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 27;
	proto.kuva_y		= 32;
	proto.kuva_frame_leveys  = 23;
	proto.kuva_frame_korkeus = 27;
	proto.korkeus		= 27;
	proto.latausaika	= 0;
	proto.leveys		= 23;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 1;
	proto.paino			= 0.5;
	proto.pisteet		= 40;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_PUNAINEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_doodle_jaatee(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "ice doodle drink");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"egg.spr");
	strcpy(proto.ammus2_sprite,"doodle_i.spr");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus1.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 53;
	proto.kuva_y		= 32;
	proto.kuva_frame_leveys  = 23;
	proto.kuva_frame_korkeus = 27;
	proto.korkeus		= 27;
	proto.latausaika	= 0;
	proto.leveys		= 23;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 1;
	proto.paino			= 0.5;
	proto.pisteet		= 40;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_SININEN;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_doodle_valkosipuli(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "garlic doodle drink");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"bad_brth.spr");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus1.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 79;
	proto.kuva_y		= 32;
	proto.kuva_frame_leveys  = 23;
	proto.kuva_frame_korkeus = 27;
	proto.korkeus		= 27;
	proto.latausaika	= 0;
	proto.leveys		= 23;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 1;
	proto.paino			= 0.5;
	proto.pisteet		= 50;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_VIHREA;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_doodle_sonic(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, "sonic doodle drink");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"doodle_s.spr");

	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN], "bonus1.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 1;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 32;
	proto.kuva_frame_leveys  = 23;
	proto.kuva_frame_korkeus = 27;
	proto.korkeus		= 27;
	proto.latausaika	= 0;
	proto.leveys		= 23;
	proto.max_hyppy		= 22;
	proto.max_nopeus	= 1;
	proto.paino			= 0.5;
	proto.pisteet		= 50;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_RAJAHDYS_HARMAA;
	proto.tyyppi		= TYYPPI_BONUS;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	 seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_teleportti1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_tele.bmp");
	strcpy(proto.nimi, "teleporter 1");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"teleport.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"teleport.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_TELEPORTTI;
	
	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 6;
	proto.frame_rate	= 2;
	proto.hyokkays1_aika= 200;
	proto.hyokkays2_aika= 200;
	proto.kuva_x		= 1;
	proto.kuva_y		= 68;//1
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 64;
	proto.korkeus		= 62;//58
	proto.latausaika	= 200;
	proto.leveys		= 30;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TELEPORTTI;
	proto.tiletarkistus = false;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_SININEN;//VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[7] = {1,2,3,4,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	UCHAR seq2[7] = {6,5,4,3,2,1,0};
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq2, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_teleportti2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_tele.bmp");
	strcpy(proto.nimi, "teleporter 2");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"teleport.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"teleport.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_TELEPORTTI;
	
	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 6;
	proto.frame_rate	= 2;
	proto.hyokkays1_aika= 200;
	proto.hyokkays2_aika= 200;
	proto.kuva_x		= 1;
	proto.kuva_y		= 68;//1
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 64;
	proto.korkeus		= 62;//58
	proto.latausaika	= 200;
	proto.leveys		= 30;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TELEPORTTI;
	proto.tiletarkistus = false;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_PUNAINEN;
	proto.vihollinen	= true;

	UCHAR seq1[7] = {1,2,3,4,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	UCHAR seq2[7] = {6,5,4,3,2,1,0};
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq2, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_teleportti3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_tele.bmp");
	strcpy(proto.nimi, "teleporter 3");
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"teleport.wav");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"teleport.wav");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_TELEPORTTI;
	
	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 6;
	proto.frame_rate	= 2;
	proto.hyokkays1_aika= 200;
	proto.hyokkays2_aika= 200;
	proto.kuva_x		= 1;
	proto.kuva_y		= 68;//1
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 64;
	proto.korkeus		= 62;//58
	proto.latausaika	= 200;
	proto.leveys		= 30;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TELEPORTTI;
	proto.tiletarkistus = false;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_VIHREA;
	proto.vihollinen	= true;

	UCHAR seq1[7] = {1,2,3,4,5,6,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	UCHAR seq2[7] = {6,5,4,3,2,1,0};
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq2, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq2, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tausta_puu(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "bg tree");

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 246;
	proto.kuva_frame_korkeus = 244;
	proto.korkeus		= 244;
	proto.latausaika	= 0;
	proto.leveys		= 246;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pallarx_kerroin = 14; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TAUSTA;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tausta_puu2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "bg tree 2");

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 248;
	proto.kuva_frame_leveys  = 178;
	proto.kuva_frame_korkeus = 174;
	proto.korkeus		= 150;//174;
	proto.latausaika	= 0;
	proto.leveys		= 178;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pallarx_kerroin = 8; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TAUSTA;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tausta_pensas(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "bg bush");

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 211;
	proto.kuva_y		= 385;//400
	proto.kuva_frame_leveys  = 330;
	proto.kuva_frame_korkeus = 93;
	proto.korkeus		= 80;
	proto.latausaika	= 0;
	proto.leveys		= 330;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pallarx_kerroin = 19; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TAUSTA;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tausta_vesipensas(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "bg water bush");

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 425;
	proto.kuva_frame_leveys  = 207;
	proto.kuva_frame_korkeus = 53;
	proto.korkeus		= 300;//260
	proto.latausaika	= 0;
	proto.leveys		= 207;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pallarx_kerroin = 2; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TAUSTA;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tausta_pilvi(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "bg cloud");

	//proto.AI[0] = AI_TAUSTA_LIIKKUU_VASEMMALLE;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 250;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 234;
	proto.kuva_frame_korkeus = 134;
	proto.korkeus		= 134;
	proto.latausaika	= 0;
	proto.leveys		= 234;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pallarx_kerroin = 3; //2
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TAUSTA;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tausta_kuu(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "bg moon");

	proto.AI[0] = AI_TAUSTA_KUU;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 487;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 63;
	proto.kuva_frame_korkeus = 63;
	proto.korkeus		= 63;
	proto.latausaika	= 0;
	proto.leveys		= 63;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pallarx_kerroin = 1; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TAUSTA;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tausta_kukka1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "bg flower1");

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 8;
	proto.kuva_x		= 553;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 13;
	proto.kuva_frame_korkeus = 15;
	proto.korkeus		= 15;
	proto.latausaika	= 0;
	proto.leveys		= 13;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pallarx_kerroin = 0; 
	proto.pisteet		= 100;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TAUSTA;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[5] = {1,2,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tausta_kukka2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "bg flower2");

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 13;
	proto.kuva_x		= 553;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 13;
	proto.kuva_frame_korkeus = 15;
	proto.korkeus		= 15;
	proto.latausaika	= 0;
	proto.leveys		= 13;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pallarx_kerroin = 0; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TAUSTA;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_SININEN;
	proto.vihollinen	= false;

	UCHAR seq1[5] = {1,2,3,4,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tausta_ketju_vert(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "bg chain 1");

	proto.AI[0] = AI_LIIKKUU_Y_SIN;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 250;
	proto.kuva_y		= 138;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 96;
	proto.korkeus		= 96;
	proto.latausaika	= 0;
	proto.leveys		= 32;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pallarx_kerroin = 0; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tiletarkistus = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;//TYYPPI_TAUSTA;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tausta_ketju_hori(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "bg chain 2");

	proto.AI[0] = AI_LIIKKUU_X_COS;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 285;
	proto.kuva_y		= 138;
	proto.kuva_frame_leveys  = 96;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 32;
	proto.latausaika	= 0;
	proto.leveys		= 96;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pallarx_kerroin = 0; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tiletarkistus = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;//TYYPPI_TAUSTA;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;
	

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tausta_kivikasa(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "bg pile of stones");

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 384;
	proto.kuva_y		= 138;//400
	proto.kuva_frame_leveys  = 179;
	proto.kuva_frame_korkeus = 87;
	proto.korkeus		= 87;
	proto.latausaika	= 0;
	proto.leveys		= 179;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pallarx_kerroin = 16; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TAUSTA;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tausta_kanto(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "bg stump");

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 8;
	proto.kuva_x		= 487;
	proto.kuva_y		= 89;
	proto.kuva_frame_leveys  = 99;
	proto.kuva_frame_korkeus = 46;
	proto.korkeus		= 46;
	proto.latausaika	= 0;
	proto.leveys		= 99;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pallarx_kerroin = 0; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TAUSTA;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_tausta_mainos(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "bg metal hen ad");

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 1;
	proto.frame_rate	= 8;
	proto.kuva_x		= 182;
	proto.kuva_y		= 248;
	proto.kuva_frame_leveys  = 176;
	proto.kuva_frame_korkeus = 123;
	proto.korkeus		= 123;
	proto.latausaika	= 0;
	proto.leveys		= 176;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pallarx_kerroin = 60; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_TAUSTA;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_mato(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "worm");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_NONSTOP;
	proto.AI[3]			= AI_VAROO_KUOPPAA;

	proto.animaatioita	= 9;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 4;
	proto.frame_rate	= 7;
	proto.kuva_x		= 553;
	proto.kuva_y		= 31;
	proto.kuva_frame_leveys  = 10;
	proto.kuva_frame_korkeus = 4;
	proto.korkeus		= 4;
	proto.latausaika	= 10;
	proto.leveys		= 10;
	proto.max_hyppy		= 15;
	proto.max_nopeus	= 1;
	proto.paino			= 0.4;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[7] = {1,2,3,4,3,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq1, true);
	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_perhonen(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "butterfly");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_NONSTOP;
	proto.AI[3]			= AI_LIIKKUU_Y_SIN_VAPAA;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 7;
	proto.kuva_x		= 553;
	proto.kuva_y		= 19;
	proto.kuva_frame_leveys  = 11;
	proto.kuva_frame_korkeus = 9;
	proto.korkeus		= 9;
	proto.latausaika	= 10;
	proto.leveys		= 11;
	proto.max_hyppy		= 1;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 50;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[7] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq1, true);
	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_perhonen2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "butterfly 2");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_LIIKKUU_Y_SIN_HIDAS;
	proto.AI[3]			= AI_NONSTOP;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.energia		= 8;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 7;
	proto.kuva_x		= 553;
	proto.kuva_y		= 19;
	proto.kuva_frame_leveys  = 11;
	proto.kuva_frame_korkeus = 9;
	proto.korkeus		= 9;
	proto.latausaika	= 10;
	proto.leveys		= 11;
	proto.max_hyppy		= 1;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 150;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_ORANSSI;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_ORANSSI;
	proto.vihollinen	= true;

	UCHAR seq1[7] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq1, true);
	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_perhonen3(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "butterfly 3");
	strcpy(proto.bonus_sprite,"key2.spr");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_KAANTYY_ESTEESTA_HORI;
	proto.AI[2]			= AI_LIIKKUU_Y_SIN_HIDAS;
	proto.AI[3]			= AI_NONSTOP;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.bonusten_lkm	= 1;
	proto.energia		= 2;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 7;
	proto.kuva_x		= 553;
	proto.kuva_y		= 19;
	proto.kuva_frame_leveys  = 11;
	proto.kuva_frame_korkeus = 9;
	proto.korkeus		= 9;
	proto.latausaika	= 10;
	proto.leveys		= 11;
	proto.max_hyppy		= 1;
	proto.max_nopeus	= 1;
	proto.paino			= 0;
	proto.pisteet		= 250;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_TAHDET_SININEN;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_SININEN;
	proto.vihollinen	= true;
	proto.bonus_aina	= true;

	UCHAR seq1[7] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq1, true);
	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_karpanen(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bg1.bmp");
	strcpy(proto.nimi, "fly");
	
	strcpy(proto.aanitiedostot[AANI_VAHINKO],		"");
	strcpy(proto.aanitiedostot[AANI_TUHOUTUMINEN],	"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS1],		"");
	strcpy(proto.aanitiedostot[AANI_HYOKKAYS2],		"");

	proto.AI[0]			= AI_PERUS;
//	proto.AI[1]			= AI_LIIKKUU_X_COS_NOPEA;
//	proto.AI[2]			= AI_LIIKKUU_Y_SIN_NOPEA;

	proto.AI[1]			= AI_PALAA_ALKUUN_X;
	proto.AI[2]			= AI_PALAA_ALKUUN_Y;
	proto.AI[3]			= AI_RANDOM_LIIKAHDUS_VERT_HORI;

	proto.animaatioita	= 2;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;
	proto.frameja		= 2;
	proto.frame_rate	= 7;
	proto.kuva_x		= 581;
	proto.kuva_y		= 19;
	proto.kuva_frame_leveys  = 7;
	proto.kuva_frame_korkeus = 5;
	proto.korkeus		= 5;
	proto.latausaika	= 10;
	proto.leveys		= 7;
	proto.max_hyppy		= 1;
	proto.max_nopeus	= 5;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tiletarkistus = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[7] = {1,2,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq1, true);
	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_ansa(char *tiedosto, char *nimi, int AI, 
						  bool ylos, bool alas, bool oikealle, bool vasemmalle,
						  int kuva_x, double paino, int vari)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_spks.bmp");
	strcpy(proto.nimi, nimi);
	strcpy(proto.muutos_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.ammus1_sprite,"");
	strcpy(proto.ammus2_sprite,"");

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI;
	proto.AI[2]			= AI_TAPA_KAIKKI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= true;
	proto.este_ylos     = ylos;
	proto.este_alas     = alas;
	proto.este_oikealle = oikealle;
	proto.este_vasemmalle = vasemmalle;

	proto.frameja		= 1;
	proto.frame_rate	= 8;
	proto.kuva_x		= 1 + (kuva_x-1)*35;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 32;
	proto.kuva_frame_korkeus = 32;
	proto.korkeus		= 31;
	proto.latausaika	= 300;
	proto.leveys		= 31;
	proto.max_hyppy		= 5;
	proto.max_nopeus	= 3;
	proto.paino			= paino;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_ISKU;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 2;
	proto.vahinko_tyyppi= VAHINKO_ISKU;
	proto.vari			= vari;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_seina_iso(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_wall.bmp");
	strcpy(proto.nimi, "big wall");

	//proto.AI[0] = AI_LIIKKUU_Y_SIN;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 2;
	proto.este			= true;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 96;
	proto.kuva_frame_korkeus = 96;
	proto.korkeus		= 96;
	proto.latausaika	= 0;
	proto.leveys		= 96;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 3;
	proto.pallarx_kerroin = 0; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 3;
	proto.vahinko_tyyppi= VAHINKO_PUDOTUS;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_seina_iso_liikkuu_y(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_wall.bmp");
	strcpy(proto.nimi, "big wall elevator vert");

	proto.AI[0] = AI_LIIKKUU_Y_SIN;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 2;
	proto.este			= true;
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 96;
	proto.kuva_frame_korkeus = 96;
	proto.korkeus		= 96;
	proto.latausaika	= 0;
	proto.leveys		= 96;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 3;
	proto.pallarx_kerroin = 0; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tiletarkistus = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 3;
	proto.vahinko_tyyppi= VAHINKO_PUDOTUS;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_iso_ansaseina_vaihe1(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_spk2.bmp");
	strcpy(proto.nimi, "big trap (falling)");

	strcpy(proto.muutos_sprite,"bigtrap2.spr");

	//proto.AI[0] = AI_LIIKKUU_Y_SIN;

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_MUUTOS_AJASTIN;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 2;
	proto.este			= true;
	//proto.este_ylos
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 96;
	proto.kuva_frame_korkeus = 96;
	proto.korkeus		= 96;
	proto.latausaika	= 100;
	proto.leveys		= 96;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 3;
	proto.pallarx_kerroin = 0; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 3;
	proto.vahinko_tyyppi= VAHINKO_PUDOTUS;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_iso_ansaseina_vaihe2(char *tiedosto)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_spk2.bmp");
	strcpy(proto.nimi, "big trap (rising)");

	strcpy(proto.muutos_sprite,"bigtrap1.spr");

	//proto.AI[0] = AI_LIIKKUU_Y_SIN;

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI_MUUTOS_AJASTIN;

	proto.animaatioita	= 8;
	proto.avain			= false;
	proto.bonusten_lkm  = 0;
	proto.energia		= 2;
	proto.este			= true;
	//proto.este_ylos
	proto.frameja		= 1;
	proto.frame_rate	= 4;
	proto.kuva_x		= 1;
	proto.kuva_y		= 1;
	proto.kuva_frame_leveys  = 96;
	proto.kuva_frame_korkeus = 96;
	proto.korkeus		= 96;
	proto.latausaika	= 100;
	proto.leveys		= 96;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= -0.5;
	proto.pallarx_kerroin = 0; 
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tarisee       = false;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 3;
	proto.vahinko_tyyppi= VAHINKO_PUDOTUS;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= true;
	

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, false);
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq1, true);

	proto.Tallenna(tiedosto);
}

void luo_prototyyppi_info(char *tiedosto, char *nimi, int AI)
{
	PK2Sprite_Prototyyppi proto; 

	strcpy(proto.versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(proto.kuvatiedosto,"spr_bons.bmp");
	strcpy(proto.nimi, nimi);

	proto.AI[0]			= AI_PERUS;
	proto.AI[1]			= AI;

	proto.animaatioita	= 1;
	proto.avain			= false;
	proto.energia		= 1;
	proto.este			= false;

	proto.frameja		= 1;
	proto.frame_rate	= 8;
	proto.kuva_x		= 1;
	proto.kuva_y		= 150;
	proto.kuva_frame_leveys  = 23;
	proto.kuva_frame_korkeus = 31;
	proto.korkeus		= 31;
	proto.latausaika	= 300;
	proto.leveys		= 23;
	proto.max_hyppy		= 0;
	proto.max_nopeus	= 0;
	proto.paino			= 0;
	proto.pisteet		= 0;
	proto.suojaus		= VAHINKO_EI;
	proto.tiletarkistus = false;
	proto.tuhoutuminen	= TUHOUTUMINEN_EI_TUHOUDU;
	proto.tyyppi		= TYYPPI_PELIHAHMO;
	proto.vahinko		= 0;
	proto.vahinko_tyyppi= VAHINKO_EI;
	proto.vari			= VARI_NORMAALI;
	proto.vihollinen	= false;

	UCHAR seq1[2] = {1,0}; 
	Animaatio_Uusi(proto, ANIMAATIO_PAIKALLA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KAVELY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_YLOS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYPPY_ALAS,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KYYKKY,		seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_VAHINKO,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_KUOLEMA,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS1,	seq1, true);
	Animaatio_Uusi(proto, ANIMAATIO_HYOKKAYS2,	seq1, true);

	proto.Tallenna(tiedosto);
}

int main()
{
/*
	// PELIHAHMOT
	luo_prototyyppi_kukko(				"sprites\\rooster.spr");	// päivitetty - update 1!
	luo_prototyyppi_kukko_kynitty(		"sprites\\r_naked.spr");	// päivitetty - update 1!
	luo_prototyyppi_kana(				"sprites\\hen.spr");
	luo_prototyyppi_pikkukana(			"sprites\\smallhen.spr");
	luo_prototyyppi_sammakko1(			"sprites\\frog1.spr");
	luo_prototyyppi_sammakko2(			"sprites\\frog2.spr");
	luo_prototyyppi_sammakko3(			"sprites\\frog3.spr");
	luo_prototyyppi_sammakko4(			"sprites\\frog4.spr");		//uusi! - update 2!
	luo_prototyyppi_sammakko5(			"sprites\\frog5.spr");		//uusi! - update 2!
	luo_prototyyppi_hyppija(			"sprites\\jumper.spr");
	luo_prototyyppi_hyppija2(			"sprites\\jumper2.spr");
	luo_prototyyppi_kani(				"sprites\\rabbit.spr");		//äänet päivitetty - update 2!
	luo_prototyyppi_kani2(				"sprites\\rabbit2.spr");	//äänet päivitetty - update 2!
	luo_prototyyppi_siili(				"sprites\\hedgehog.spr");
	luo_prototyyppi_siili2(				"sprites\\hedgeho2.spr");
	luo_prototyyppi_siili3(				"sprites\\hedgeho3.spr");
	luo_prototyyppi_siili4(				"sprites\\hedgeho4.spr");
	luo_prototyyppi_lepakko(			"sprites\\bat.spr");
	luo_prototyyppi_lepakko2(			"sprites\\bat2.spr");
	luo_prototyyppi_lepakko3(			"sprites\\bat3.spr");
	luo_prototyyppi_liekki(				"sprites\\flame.spr");
	luo_prototyyppi_liekki2(			"sprites\\flame2.spr");*/
	luo_prototyyppi_liekki3(			"sprites\\flame3.spr");	// uusi - update 3!
/*	luo_prototyyppi_possu(				"sprites\\pig.spr");
	luo_prototyyppi_possu2(				"sprites\\pig2.spr");
	luo_prototyyppi_lentava_possu(		"sprites\\fly_pig.spr");
	luo_prototyyppi_pommi(				"sprites\\bomb.spr");	// päivitetty - update 2!
	luo_prototyyppi_lintu(				"sprites\\bird1.spr");
	luo_prototyyppi_lintu2(				"sprites\\bird2.spr");
	luo_prototyyppi_lintu3(				"sprites\\bird3.spr");
	luo_prototyyppi_lintu4(				"sprites\\bird4.spr");	// päivitetty - update 2
	luo_prototyyppi_susi1(				"sprites\\wolf1.spr");
	luo_prototyyppi_susi2(				"sprites\\wolf2.spr");
	luo_prototyyppi_susi3(				"sprites\\wolf3.spr");
	luo_prototyyppi_jattihiiri(			"sprites\\gmouse1.spr");
	luo_prototyyppi_jattihiiri2(		"sprites\\gmouse2.spr"); // päivitetty - update 1!
	luo_prototyyppi_jattihiiri3(		"sprites\\gmouse3.spr");
	luo_prototyyppi_kala1(				"sprites\\fish1.spr");
	luo_prototyyppi_kala2(				"sprites\\fish2.spr");
	luo_prototyyppi_kala3(				"sprites\\fish3.spr");
	luo_prototyyppi_mehilainen(			"sprites\\bee.spr");
	luo_prototyyppi_mehilainen2(		"sprites\\bee2.spr");
	luo_prototyyppi_mehilainen3(		"sprites\\bee3.spr");	// uusi - update 1!
	luo_prototyyppi_mehilaispesa(		"sprites\\beehive.spr");
	luo_prototyyppi_mehilaispesa2(		"sprites\\beehive2.spr");
	luo_prototyyppi_tankki1(			"sprites\\tank.spr");
	luo_prototyyppi_tankki2(			"sprites\\tank2.spr");
	luo_prototyyppi_tankki3(			"sprites\\tank3.spr");
	luo_prototyyppi_tankki4(			"sprites\\tank4.spr");
	luo_prototyyppi_aave1(				"sprites\\ghost1.spr");
	luo_prototyyppi_pahakana(			"sprites\\evilone.spr");
	luo_prototyyppi_pahakana2(			"sprites\\evilone2.spr"); // uusi - update 2!
	luo_prototyyppi_jattimehilainen(	"sprites\\gbee.spr");	// uusi - update 2!
	luo_prototyyppi_robokana(			"sprites\\robohen.spr"); // uusi - update 2!
	luo_prototyyppi_robokanan_paa(		"sprites\\robohead.spr"); // uusi - update 2!
	luo_prototyyppi_jattirobokana_muoto1("sprites\\roboss1.spr"); // uusi - update 2!
	luo_prototyyppi_jattirobokana_muoto2("sprites\\roboss2.spr"); // uusi - update 2!
	luo_prototyyppi_jattirobokana_muoto3("sprites\\roboss3.spr"); // uusi - update 2!
	luo_prototyyppi_jattirobokana_muoto4("sprites\\roboss4.spr"); // uusi - update 2!
	luo_prototyyppi_jattirobokana_muoto5("sprites\\roboss5.spr"); // uusi - update 2!
	*/
	luo_prototyyppi_luut1(				"sprites\\bones1.spr");		// uusi - update 3!!
	luo_prototyyppi_luut2(				"sprites\\bones2.spr");		// uusi - update 3!!
	luo_prototyyppi_piikkipaa1(			"sprites\\spkhead1.spr");	// uusi - update 3!!
	luo_prototyyppi_piikkipaa2(			"sprites\\spkhead2.spr");	// uusi - update 3!!
	luo_prototyyppi_piikkipaa3(			"sprites\\spkhead3.spr");	// uusi - update 3!!
	luo_prototyyppi_piikkipaa4(			"sprites\\spkhead4.spr");	// uusi - update 3!!
	luo_prototyyppi_piikkipaa5(			"sprites\\spkhead5.spr");	// uusi - update 3!!
	luo_prototyyppi_piikkipaa6(			"sprites\\spkhead6.spr");	// uusi - update 3!!
	luo_prototyyppi_piikkipaa7(			"sprites\\spkhead7.spr");	// uusi - update 3!!
	luo_prototyyppi_piikkipaa8(			"sprites\\spkhead8.spr");	// uusi - update 3!!

	luo_prototyyppi_hamahakki1(			"sprites\\spider1.spr");	// uusi - update 3!!
	//luo_prototyyppi_hamahakki2(			"sprites\\spider2.spr");	// uusi - update 3!!
	//luo_prototyyppi_hamahakki3(			"sprites\\spider3.spr");	// uusi - update 3!!

	luo_prototyyppi_kilpikonna_vaihe1(	"sprites\\turtle1.spr");	// uusi - update 3!!
	luo_prototyyppi_kilpikonna_vaihe2(	"sprites\\turtle2.spr");	// uusi - update 3!!
	luo_prototyyppi_kilpikonna_vaihe3(	"sprites\\turtle3.spr");	// uusi - update 3!!

	luo_prototyyppi_tulikilpikonna(		"sprites\\turtle4.spr");	// uusi - update 3!!

	luo_prototyyppi_hissi1(				"sprites\\platfrm1.spr");	// uusi - update 3!!
	luo_prototyyppi_hissi2(				"sprites\\platfrm2.spr");	// uusi - update 3!!
	luo_prototyyppi_hissi3(				"sprites\\platfrm3.spr");	// uusi - update 3!!

	luo_prototyyppi_nuoli_oikealle(		"sprites\\arrow_r.spr");	// uusi - update 3!!
	luo_prototyyppi_nuoli_ylos(			"sprites\\arrow_u.spr");	// uusi - update 3!!
	luo_prototyyppi_nuoli_vasemmalle(	"sprites\\arrow_l.spr");	// uusi - update 3!!
	luo_prototyyppi_nuoli_alas(			"sprites\\arrow_d.spr");	// uusi - update 3!!
	luo_prototyyppi_nuoli_oikealle2(	"sprites\\arrow2_r.spr");	// uusi - update 3!!
	luo_prototyyppi_nuoli_ylos2(		"sprites\\arrow2_u.spr");	// uusi - update 3!!
	luo_prototyyppi_nuoli_vasemmalle2(	"sprites\\arrow2_l.spr");	// uusi - update 3!!
	luo_prototyyppi_nuoli_alas2(		"sprites\\arrow2_d.spr");	// uusi - update 3!!
	//luo_prototyyppi_aave2(				"sprites\\ghost1.spr");
	/*
	
	// BONUKSET
	*/
	luo_prototyyppi_ananas1(			"sprites\\pineapp1.spr");	// uusi - update 3!!
	luo_prototyyppi_karkki(				"sprites\\candy1.spr","candy 1",VARI_SININEN,10);// uusi - update 3!!
	luo_prototyyppi_karkki(				"sprites\\candy2.spr","candy 2",VARI_PUNAINEN,20);// uusi - update 3!!
	luo_prototyyppi_karkki(				"sprites\\candy3.spr","candy 3",VARI_VIHREA,30);// uusi - update 3!!
	luo_prototyyppi_karkki(				"sprites\\candy4.spr","candy 4",VARI_ORANSSI,40);// uusi - update 3!!
	luo_prototyyppi_karkki(				"sprites\\candy5.spr","candy 5",VARI_VIOLETTI,50);// uusi - update 3!!
	luo_prototyyppi_karkki(				"sprites\\candy6.spr","candy 6",VARI_TURKOOSI,60);// uusi - update 3!!
	luo_prototyyppi_jalokivi(			"sprites\\jewel1.spr","jewel 1",VARI_SININEN,80);// uusi - update 3!!
	luo_prototyyppi_jalokivi(			"sprites\\jewel2.spr","jewel 2",VARI_PUNAINEN,90);// uusi - update 3!!
	luo_prototyyppi_jalokivi(			"sprites\\jewel3.spr","jewel 3",VARI_VIHREA,100);// uusi - update 3!!
	luo_prototyyppi_jalokivi(			"sprites\\jewel4.spr","jewel 4",VARI_ORANSSI,110);// uusi - update 3!!
	luo_prototyyppi_jalokivi(			"sprites\\jewel5.spr","jewel 5",VARI_VIOLETTI,120);// uusi - update 3!!
	luo_prototyyppi_jalokivi(			"sprites\\jewel6.spr","jewel 6",VARI_TURKOOSI,130);// uusi - update 3!!
/*	luo_prototyyppi_omena1(				"sprites\\apple.spr");
	luo_prototyyppi_omena2(				"sprites\\apple2.spr");
	luo_prototyyppi_omena3(				"sprites\\apple3.spr");
	luo_prototyyppi_omena4(				"sprites\\apple4.spr");
	luo_prototyyppi_omena5(				"sprites\\apple5.spr");
	luo_prototyyppi_iso_omena(			"sprites\\big_appl.spr");
	luo_prototyyppi_iso_omena2(			"sprites\\big_app2.spr");
	luo_prototyyppi_nippu_omenoita(		"sprites\\apples.spr");
	luo_prototyyppi_porkkana(			"sprites\\carrot.spr");
	luo_prototyyppi_juusto(				"sprites\\cheese.spr");
	luo_prototyyppi_pippuri(			"sprites\\pepper.spr");
	luo_prototyyppi_hoyhen(				"sprites\\feather.spr");
	luo_prototyyppi_hoyhen2(			"sprites\\feather2.spr");
	luo_prototyyppi_appelsiini(			"sprites\\orange.spr");
	luo_prototyyppi_greippi(			"sprites\\grape.spr");
	luo_prototyyppi_viinirypale(		"sprites\\wgrape.spr"); // uusi - update 2!
	luo_prototyyppi_rypaleterttu(		"sprites\\wgrapes.spr"); // uusi - update 2!
*/
	luo_prototyyppi_kummitusjauhe1(		"sprites\\gpowder1.spr"); // uusi - update 3!!
	luo_prototyyppi_kummitusjauhe2(		"sprites\\gpowder2.spr"); // uusi - update 3!!
/*
	luo_prototyyppi_avain1(				"sprites\\key.spr");
	luo_prototyyppi_avain2(				"sprites\\key2.spr");
	luo_prototyyppi_avain3(				"sprites\\key3.spr");
	luo_prototyyppi_avain4(				"sprites\\key4.spr");	// päivitetty - update 2

	luo_prototyyppi_kello1(				"sprites\\clock1.spr");
	luo_prototyyppi_kello2(				"sprites\\clock2.spr");
	luo_prototyyppi_kello3(				"sprites\\clock3.spr");
	
	// "AMMUKSET"
	luo_prototyyppi_muna(				"sprites\\egg.spr");//		
	luo_prototyyppi_muna_tuli(			"sprites\\egg_fire.spr");
	luo_prototyyppi_muna_haju(			"sprites\\egg_stnk.spr");
	luo_prototyyppi_muna_jaa(			"sprites\\egg_ice.spr");	// uusi! - update 2
	luo_prototyyppi_kieku(				"sprites\\doodle.spr");
	luo_prototyyppi_kieku_tuli(			"sprites\\doodle_f.spr");
	luo_prototyyppi_kieku_tuli_2(		"sprites\\doodlef2.spr");
	luo_prototyyppi_kieku_lumi(			"sprites\\doodle_i.spr");
	luo_prototyyppi_kieku_sonic(		"sprites\\doodle_s.spr");
	luo_prototyyppi_jaapala(			"sprites\\icecube.spr");
	luo_prototyyppi_kieku_valkosipuli(	"sprites\\bad_brth.spr");
	luo_prototyyppi_tennispallo(		"sprites\\tennisb.spr");
	luo_prototyyppi_ammus1(				"sprites\\ammo1.spr");
	luo_prototyyppi_ammus2(				"sprites\\ammo2.spr");
	
	
	// LAATIKOT
	luo_prototyyppi_laatikko_megafooni( "sprites\\box_mph.spr");
	luo_prototyyppi_laatikko_juoma_muna("sprites\\box_egg.spr");
	luo_prototyyppi_laatikko_juoma_tulimuna("sprites\\box_fegg.spr");
	luo_prototyyppi_laatikko_appelsiini("sprites\\box_o.spr");
	luo_prototyyppi_laatikko_hoyhen(	"sprites\\box_f.spr");
	luo_prototyyppi_laatikko_kana(		"sprites\\box_r.spr");
	luo_prototyyppi_laatikko_pikkukana(	"sprites\\box_s.spr");
	luo_prototyyppi_laatikko_pikkukanoja("sprites\\box_3s.spr");
	luo_prototyyppi_laatikko_siili(		"sprites\\box_hedg.spr");
	luo_prototyyppi_laatikko_perhoset(	"sprites\\box_bfly.spr");	// uusi! - update 2

	// LAHJAT
	luo_prototyyppi_lahja_hoyhen(		"sprites\\gift_fth.spr");
	luo_prototyyppi_lahja_doodle_tuli(	"sprites\\gift_cac.spr");
	luo_prototyyppi_lahja_doodle_lumi(	"sprites\\gift_cai.spr");
	luo_prototyyppi_lahja(				"sprites\\gift_flw.spr", "bg_flwr1.spr", "gift flower"); // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_chi.spr", "smallhen.spr", "gift chick"); // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_hed.spr", "hedgehog.spr", "gift hedgehog"); // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_stn.spr", "stncube.spr", "stone cube");  // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_swi.spr", "switch1.spr", "gift switch");  // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_bf1.spr", "bttrfly2.spr", "gift butterfly"); // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_bf2.spr", "bttrfly3.spr", "gift butterfly 2"); // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_key.spr", "key.spr", "gift key");      // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_ke2.spr", "key2.spr", "gift blue key");     // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_bee.spr", "bee3.spr", "gift bee");     // uusi - update 1!
	
	// TAIKAJUOMAT
	luo_prototyyppi_taikajuoma_kana(	"sprites\\potion_h.spr");
	luo_prototyyppi_taikajuoma_pikkukana("sprites\\potion_s.spr");
	luo_prototyyppi_taikajuoma_possu(	"sprites\\potion_p.spr");
	luo_prototyyppi_taikajuoma_lintu(	"sprites\\potion_b.spr");
	luo_prototyyppi_taikajuoma_sammakko("sprites\\potion_f.spr"); // uusi! - update 2
	luo_prototyyppi_juoma_muna(	        "sprites\\ptn_egg.spr");
	luo_prototyyppi_juoma_tulimuna(     "sprites\\ptn_fegg.spr");
	luo_prototyyppi_paasiaismuna(		"sprites\\eegg_egg.spr","easter egg","egg.spr",10,VARI_NORMAALI, TUHOUTUMINEN_SAVU_HARMAA);
	luo_prototyyppi_paasiaismuna(		"sprites\\eegg_fir.spr","fire easter egg","egg_fire.spr",20,VARI_PUNAINEN, TUHOUTUMINEN_SAVU_PUNAINEN);
	luo_prototyyppi_paasiaismuna(		"sprites\\eegg_stn.spr","smelly easter egg","egg_stnk.spr",30,VARI_VIHREA, TUHOUTUMINEN_SAVU_VIHREA);
	luo_prototyyppi_paasiaismuna(		"sprites\\eegg_ice.spr","ice egg","egg_ice.spr",30,VARI_SININEN, TUHOUTUMINEN_SAVU_SININEN);  // uusi! - update 2

	

	// DOODLET-JUOMAT JA MEGAFOONI
	luo_prototyyppi_megafooni(		    "sprites\\megaphon.spr");
	luo_prototyyppi_doodle_chili(		"sprites\\can_chil.spr");
	luo_prototyyppi_doodle_jaatee(		"sprites\\can_icet.spr");
	luo_prototyyppi_doodle_valkosipuli(	"sprites\\can_garl.spr");
	luo_prototyyppi_doodle_sonic(		"sprites\\can_snic.spr");
	

	// TELEPORTIT
	luo_prototyyppi_teleportti1(		"sprites\\tlport1.spr");
	luo_prototyyppi_teleportti2(		"sprites\\tlport2.spr");
	luo_prototyyppi_teleportti3(		"sprites\\tlport3.spr");

	// KYTKIMET
	luo_prototyyppi_kallokytkin1(		"sprites\\switch1.spr");
	luo_prototyyppi_kallokytkin2(		"sprites\\switch2.spr");

	// TAUSTASPRITET
	luo_prototyyppi_tausta_kukka1(		"sprites\\bg_flwr1.spr");	// päivitetty - update 1!
	luo_prototyyppi_tausta_kukka2(		"sprites\\bg_flwr2.spr");
	luo_prototyyppi_tausta_puu(			"sprites\\bg_tree.spr");
	luo_prototyyppi_tausta_puu2(		"sprites\\bg_tree2.spr");
	luo_prototyyppi_tausta_pensas(		"sprites\\bg_bush.spr");
	luo_prototyyppi_tausta_vesipensas(	"sprites\\bg_wbush.spr");
	luo_prototyyppi_tausta_pilvi(		"sprites\\bg_cloud.spr");
	luo_prototyyppi_tausta_kuu(		    "sprites\\bg_moon.spr");
	luo_prototyyppi_tausta_ketju_vert(  "sprites\\bg_chan1.spr");
	luo_prototyyppi_tausta_ketju_hori(  "sprites\\bg_chan2.spr");
	luo_prototyyppi_mato(				"sprites\\worm.spr");
	luo_prototyyppi_perhonen(			"sprites\\bttrfly.spr");	// päivitetty - update 1!
	luo_prototyyppi_perhonen2(			"sprites\\bttrfly2.spr");	// päivitetty - update 1!
	luo_prototyyppi_perhonen3(			"sprites\\bttrfly3.spr");	// päivitetty - update 1!
	luo_prototyyppi_karpanen(			"sprites\\fly.spr");
	luo_prototyyppi_tausta_kivikasa(	"sprites\\bg_spile.spr");	// uusi - update 2!
	luo_prototyyppi_tausta_kanto(		"sprites\\bg_stump.spr");	// uusi - update 2!
	luo_prototyyppi_tausta_mainos(		"sprites\\bg_ad.spr");		// uusi - update 2!
	
	// SEINÄT
	luo_prototyyppi_kivipala(			"sprites\\stncube.spr");
	luo_prototyyppi_kivipala2(			"sprites\\stncube2.spr");
	luo_prototyyppi_kivipala3(			"sprites\\stncube3.spr");

	luo_prototyyppi_ansa("sprites\\trap1_ff.spr", "falling spike floor",		AI_KITKA_VAIKUTTAA, true, false, true, true, 1, 1, VARI_NORMAALI);
	luo_prototyyppi_ansa("sprites\\trap2_cf.spr", "falling spike ceiling",		AI_KITKA_VAIKUTTAA, false, true, true, true, 2, 1, VARI_NORMAALI);
	luo_prototyyppi_ansa("sprites\\trap3_wf.spr", "falling spike wall left",	AI_KITKA_VAIKUTTAA, true, true, false, true, 3, 1, VARI_NORMAALI);
	luo_prototyyppi_ansa("sprites\\trap4_wf.spr", "falling spike wall right",	AI_KITKA_VAIKUTTAA, true, true, true, false, 4, 1, VARI_NORMAALI);
	luo_prototyyppi_ansa("sprites\\trap5_f2.spr", "switch 2 move up",			AI_LIIKKUU_YLOS_JOS_KYTKIN2_PAINETTU, true, false, true, true, 1, 0, VARI_SININEN);
	luo_prototyyppi_ansa("sprites\\trap6_c2.spr", "switch 2 move down",			AI_LIIKKUU_ALAS_JOS_KYTKIN2_PAINETTU, false, true, true, true, 2, 0, VARI_SININEN);
	luo_prototyyppi_ansa("sprites\\trap7_w3.spr", "switch 3 move left",			AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN3_PAINETTU, true, true, false, true, 3, 0, VARI_VIHREA);
	luo_prototyyppi_ansa("sprites\\trap8_w3.spr", "switch 3 move right",		AI_LIIKKUU_OIKEALLE_JOS_KYTKIN3_PAINETTU, true, true, true, false,   4, 0, VARI_VIHREA);

	luo_prototyyppi_seina_iso(		    "sprites\\wall1big.spr");
	luo_prototyyppi_seina_iso_liikkuu_y("sprites\\wall2big.spr");*/
	luo_prototyyppi_iso_ansaseina_vaihe1("sprites\\bigtrap1.spr");	//uusi! - update 3!
	luo_prototyyppi_iso_ansaseina_vaihe2("sprites\\bigtrap2.spr");	//uusi! - update 3!
/*
	// INFO-KYLTIT
	luo_prototyyppi_info( "sprites\\info1.spr" ,"doodle attack info" ,	AI_INFO1);
	luo_prototyyppi_info( "sprites\\info2.spr" ,"egg attack info"    ,	AI_INFO2);
	luo_prototyyppi_info( "sprites\\info3.spr" ,"exit sign info"    ,	AI_INFO3);
	luo_prototyyppi_info( "sprites\\info4.spr" ,"keys info"    ,		AI_INFO4);
	luo_prototyyppi_info( "sprites\\info5.spr" ,"switch info"    ,		AI_INFO5);
	luo_prototyyppi_info( "sprites\\info6.spr" ,"green switch info"    ,AI_INFO6);
	luo_prototyyppi_info( "sprites\\info7.spr" ,"blue switch info",		AI_INFO7);
	luo_prototyyppi_info( "sprites\\info8.spr" ,"orange switch info",	AI_INFO8);
	luo_prototyyppi_info( "sprites\\info9.spr" ,"gifts info"    ,		AI_INFO9);
	luo_prototyyppi_info( "sprites\\info10.spr" ,"energy items info",	AI_INFO10);
	luo_prototyyppi_info( "sprites\\info11.spr" ,"teleports info"    ,	AI_INFO11);
	luo_prototyyppi_info( "sprites\\info12.spr" ,"score info"    ,		AI_INFO12);
	luo_prototyyppi_info( "sprites\\info13.spr" ,"hidden bonuses info", AI_INFO13);
	luo_prototyyppi_info( "sprites\\info14.spr" ,"key-lock info"   ,	AI_INFO14);
	luo_prototyyppi_info( "sprites\\info15.spr" ,"traps info"    ,		AI_INFO15);
	luo_prototyyppi_info( "sprites\\info16.spr" ,"time info"    ,		AI_INFO16);
	luo_prototyyppi_info( "sprites\\info17.spr" ,"enemys info"    ,		AI_INFO17);
	luo_prototyyppi_info( "sprites\\info18.spr" ,"wind info"    ,		AI_INFO18);
	luo_prototyyppi_info( "sprites\\info19.spr" ,"rescue info"    ,		AI_INFO19);

	*/

	/* UPDATE 2 

	luo_prototyyppi_kukko(				"sprites\\rooster.spr");	// päivitetty - update 1!
	luo_prototyyppi_kukko_kynitty(		"sprites\\r_naked.spr");	// päivitetty - update 1!
	luo_prototyyppi_sammakko4(			"sprites\\frog4.spr");		//uusi! - update 2!
	luo_prototyyppi_sammakko5(			"sprites\\frog5.spr");		//uusi! - update 2!
	luo_prototyyppi_hyppija(			"sprites\\jumper.spr");		//äänet päivitetty - update 2!
	luo_prototyyppi_hyppija2(			"sprites\\jumper2.spr");	//äänet päivitetty - update 2!
	luo_prototyyppi_kani(				"sprites\\rabbit.spr");		//äänet päivitetty - update 2!
	luo_prototyyppi_kani2(				"sprites\\rabbit2.spr");	//äänet päivitetty - update 2!
	luo_prototyyppi_pommi(				"sprites\\bomb.spr");	// päivitetty - update 2!
	luo_prototyyppi_jattihiiri2(		"sprites\\gmouse2.spr"); // päivitetty - update 1!
	luo_prototyyppi_mehilainen3(		"sprites\\bee3.spr");	// uusi - update 1!
	luo_prototyyppi_tankki1(			"sprites\\tank.spr");	//äänet päivitetty - update 2!
	luo_prototyyppi_tankki2(			"sprites\\tank2.spr");	//äänet päivitetty - update 2!
	luo_prototyyppi_tankki3(			"sprites\\tank3.spr");	//äänet päivitetty - update 2!	
	luo_prototyyppi_tankki4(			"sprites\\tank4.spr");	//äänet päivitetty - update 2!
	luo_prototyyppi_pahakana2(			"sprites\\evilone2.spr"); // uusi - update 2!
	luo_prototyyppi_jattimehilainen(	"sprites\\gbee.spr");	// uusi - update 2!
	luo_prototyyppi_robokana(			"sprites\\robohen.spr"); // uusi - update 2!
	luo_prototyyppi_robokanan_paa(		"sprites\\robohead.spr"); // uusi - update 2!
	luo_prototyyppi_jattirobokana_muoto1("sprites\\roboss1.spr"); // uusi - update 2!
	luo_prototyyppi_jattirobokana_muoto2("sprites\\roboss2.spr"); // uusi - update 2!
	luo_prototyyppi_jattirobokana_muoto3("sprites\\roboss3.spr"); // uusi - update 2!
	luo_prototyyppi_jattirobokana_muoto4("sprites\\roboss4.spr"); // uusi - update 2!
	luo_prototyyppi_jattirobokana_muoto5("sprites\\roboss5.spr"); // uusi - update 2!
	luo_prototyyppi_lintu4(				"sprites\\bird4.spr");	// päivitetty - update 2!
	
	// BONUKSET
	luo_prototyyppi_viinirypale(		"sprites\\wgrape.spr"); // uusi - update 2!
	luo_prototyyppi_rypaleterttu(		"sprites\\wgrapes.spr"); // uusi - update 2!
	luo_prototyyppi_avain4(				"sprites\\key4.spr");	// päivitetty - update 2!
	
	// "AMMUKSET"
	luo_prototyyppi_muna_jaa(			"sprites\\egg_ice.spr");	// uusi! - update 2
	
	// LAATIKOT
	luo_prototyyppi_laatikko_perhoset(	"sprites\\box_bfly.spr");	// uusi! - update 2

	// LAHJAT
	luo_prototyyppi_lahja(				"sprites\\gift_flw.spr", "bg_flwr1.spr", "gift flower"); // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_chi.spr", "smallhen.spr", "gift chick"); // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_hed.spr", "hedgehog.spr", "gift hedgehog"); // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_stn.spr", "stncube.spr", "stone cube");  // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_swi.spr", "switch1.spr", "gift switch");  // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_bf1.spr", "bttrfly2.spr", "gift butterfly"); // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_bf2.spr", "bttrfly3.spr", "gift butterfly 2"); // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_key.spr", "key.spr", "gift key");      // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_ke2.spr", "key2.spr", "gift blue key");     // uusi - update 1!
	luo_prototyyppi_lahja(				"sprites\\gift_bee.spr", "bee3.spr", "gift bee");     // uusi - update 1!
	
	// TAIKAJUOMAT
	luo_prototyyppi_taikajuoma_sammakko("sprites\\potion_f.spr"); // uusi! - update 2
	luo_prototyyppi_paasiaismuna(		"sprites\\eegg_ice.spr","ice egg","egg_ice.spr",30,VARI_SININEN, TUHOUTUMINEN_SAVU_SININEN);  // uusi! - update 2
	

	// DOODLET-JUOMAT JA MEGAFOONI
	

	// TELEPORTIT


	// KYTKIMET

	// TAUSTASPRITET
	luo_prototyyppi_tausta_kukka1(		"sprites\\bg_flwr1.spr");	// päivitetty - update 1!
	luo_prototyyppi_perhonen(			"sprites\\bttrfly.spr");	// päivitetty - update 1!
	luo_prototyyppi_perhonen2(			"sprites\\bttrfly2.spr");	// päivitetty - update 1!
	luo_prototyyppi_perhonen3(			"sprites\\bttrfly3.spr");	// päivitetty - update 1!
	luo_prototyyppi_tausta_kivikasa(	"sprites\\bg_spile.spr");	// uusi - update 2!
	luo_prototyyppi_tausta_kanto(		"sprites\\bg_stump.spr");	// uusi - update 2!
	luo_prototyyppi_tausta_mainos(		"sprites\\bg_ad.spr");		// uusi - update 2!	
*/	
	
	
/*
*info01:			use doodle to knock out enemys.
*info02:			use eggs to knock out enemys.
*info03:			find the exit sign.
*info04:			collecting all keys opens all locks.
*info05:			you can activate switches by jumping on them.
*info06:			green switches activate green blocks.
*info07:			blue switches activate blue blocks.
*info08:			orange switches turn off fire.
*info09:			collect gifts to obtain useful items.
*info10:			some items will restore energy.
*info11:			don't get confused with teleports.
*info12:			some items can give you special attacks.
*info13:			look for hidden bonuses!
*info14:			you can stomp on enemys, but you will lose energy.
*info15:			look out for traps!
*info16:			hurry up! time is running out!
*info17:			be aware that some enemys resist certain attacks.
*info18:			a hen is mightier than the sword.
*info19:			why did the chicken cross the road?
*/
	return 0;
}