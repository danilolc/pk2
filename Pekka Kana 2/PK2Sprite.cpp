#include <windows.h>   
#include <windowsx.h> 
#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream.h>
#include "PK2Sprite.h"
#include "D:\Visual Studio\MyProjects\PisteEngine\PisteDraw.h"
#include "D:\Visual Studio\MyProjects\PisteEngine\PisteSound.h"


/* -------- PK2Sprite Prototyyppi ------------------------------------------------------------------ */

PK2Sprite_Prototyyppi::PK2Sprite_Prototyyppi()
{

	strcpy(versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(tiedosto,"");
	strcpy(kuvatiedosto,"");
	strcpy(nimi, "");
	strcpy(this->muutos_sprite,"");
	strcpy(this->bonus_sprite,"");
	strcpy(this->ammus1_sprite,"");
	strcpy(this->ammus2_sprite,"");

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(aanitiedostot[aani], "");
		aanet[aani] = -1;
	}

	aani_frq		= 22050;
	ammus1			= -1;
	ammus2			= -1;
	animaatioita	= 0;
	avain			= false;
	bonus			= -1;
	bonusten_lkm    = 1;
	energia			= 0;
	este			= false;
	este_ylos		= true;
	este_alas		= true;
	este_oikealle	= true;
	este_vasemmalle	= true;
	frameja			= 0;
	frame_rate		= 0;
	hyokkays1_aika  = 60;
	hyokkays2_aika  = 60;
	indeksi			= 0;
	kuva_x			= 0;
	kuva_y			= 0;
	kuva_frame_leveys  = 0;
	kuva_frame_korkeus = 0;
	korkeus			= 0;
	latausaika		= 0;
	leveys			= 0;
	max_hyppy		= 0;
	max_nopeus		= 3;
	muutos			= -1;
	paino			= 0;
	pallarx_kerroin = 0;
	pisteet			= 0;
	random_frq		= true;
	suojaus			= VAHINKO_EI;
	tarisee         = false;
	tiletarkistus	= true;
	tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	tyyppi			= TYYPPI_EI_MIKAAN;
	vahinko			= 0;
	vahinko_tyyppi  = VAHINKO_ISKU;
	vari			= VARI_NORMAALI;
	vihollinen		= false;

	lapinakyvyys	= false;
	hehkuu			= false;
	tulitauko		= 0;
	liitokyky		= false;
	boss			= false;
	bonus_aina		= false;
	osaa_uida		= false;

	for (int i=0;i<SPRITE_MAX_AI;i++)
	{
		AI[i] = AI_EI;
	}

	for (i=0;i<SPRITE_MAX_FRAMEJA;i++)
		framet[i] = 0;

	for (i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			animaatiot[i].sekvenssi[j] = 0;
		
		animaatiot[i].looppi  = false;
		animaatiot[i].frameja = 0;
	}
}

PK2Sprite_Prototyyppi::~PK2Sprite_Prototyyppi()
{
	for (int i=0;i<SPRITE_MAX_FRAMEJA;i++)
	{
		if (framet[i] > 0)
			PisteDraw_Buffer_Tuhoa(this->framet[i]);
	}
}

void PK2Sprite_Prototyyppi::Kopioi(const PK2Sprite_Prototyyppi &proto)
{
	strcpy(versio,proto.versio);
	strcpy(tiedosto,proto.tiedosto);
	strcpy(kuvatiedosto,proto.kuvatiedosto);
	strcpy(nimi, proto.nimi);
	strcpy(this->muutos_sprite,proto.muutos_sprite);
	strcpy(this->bonus_sprite,proto.bonus_sprite);
	strcpy(this->ammus1_sprite,proto.ammus1_sprite);
	strcpy(this->ammus2_sprite,proto.ammus2_sprite);

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(aanitiedostot[aani], proto.aanitiedostot[aani]);
		aanet[aani] = proto.aanet[aani];
	}

	aani_frq        = proto.aani_frq;
	ammus1			= proto.ammus1;
	ammus2			= proto.ammus2;
	animaatioita	= proto.animaatioita;
	avain			= proto.avain;
	bonus			= proto.bonus;
	bonusten_lkm    = proto.bonusten_lkm;
	energia			= proto.energia;
	este			= proto.este;
	este_ylos		= proto.este_ylos;
	este_alas		= proto.este_alas;
	este_oikealle	= proto.este_oikealle;
	este_vasemmalle	= proto.este_vasemmalle;
	frameja			= proto.frameja;
	frame_rate		= proto.frame_rate;
	hyokkays1_aika  = proto.hyokkays1_aika;
	hyokkays2_aika  = proto.hyokkays2_aika;
	indeksi			= proto.indeksi;
	kuva_x			= proto.kuva_x;
	kuva_y			= proto.kuva_y;
	kuva_frame_leveys  = proto.kuva_frame_leveys;
	kuva_frame_korkeus = proto.kuva_frame_korkeus;
	korkeus			= proto.korkeus;
	latausaika		= proto.latausaika;
	leveys			= proto.leveys;
	max_hyppy		= proto.max_hyppy;
	max_nopeus		= proto.max_nopeus;
	muutos			= proto.muutos;
	paino			= proto.paino;
	pallarx_kerroin = proto.pallarx_kerroin;
	pisteet			= proto.pisteet;
	random_frq		= proto.random_frq;
	suojaus			= proto.suojaus;
	tarisee         = proto.tarisee;
	tiletarkistus	= proto.tiletarkistus;
	tuhoutuminen	= proto.tuhoutuminen;
	tyyppi			= proto.tyyppi;
	vahinko			= proto.vahinko;
	vahinko_tyyppi	= proto.vahinko_tyyppi;
	vari			= proto.vari;
	vihollinen		= proto.vihollinen;

	lapinakyvyys	= proto.lapinakyvyys;
	hehkuu			= proto.hehkuu;
	tulitauko		= proto.tulitauko;
	liitokyky		= proto.liitokyky;
	boss			= proto.boss;
	bonus_aina		= proto.bonus_aina;
	osaa_uida		= proto.osaa_uida;

	for (int i=0;i<SPRITE_MAX_AI;i++)
	{
		AI[i] = proto.AI[i];
	}

	for (i=0;i<SPRITE_MAX_FRAMEJA;i++)
		framet[i] = proto.framet[i];

	for (i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];
		
		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frameja = proto.animaatiot[i].frameja;
	}	
}

void PK2Sprite_Prototyyppi::Uusi()
{
	strcpy(versio,PK2SPRITE_VIIMEISIN_VERSIO);
	strcpy(tiedosto,"");
	strcpy(kuvatiedosto,"");
	strcpy(nimi, "");
	strcpy(muutos_sprite,"");
	strcpy(bonus_sprite,"");
	strcpy(ammus1_sprite,"");
	strcpy(ammus2_sprite,"");
	
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(aanitiedostot[aani], "");
		aanet[aani] = -1;
	}

	aani_frq		= 22050;
	ammus1			= -1;
	ammus2			= -1;
	animaatioita	= 0;
	avain			= false;
	bonus			= -1;
	bonusten_lkm    = 1;
	energia			= 0;
	este			= false;
	este_ylos		= true;
	este_alas		= true;
	este_oikealle	= true;
	este_vasemmalle	= true;
	frameja			= 0;
	frame_rate		= 0;
	hyokkays1_aika  = 60;
	hyokkays2_aika  = 60;
	indeksi			= 0;
	kuva_x			= 0;
	kuva_y			= 0;
	kuva_frame_leveys  = 0;
	kuva_frame_korkeus = 0;
	korkeus			= 0;
	latausaika		= 0;
	leveys			= 0;
	max_hyppy		= 0;
	max_nopeus		= 3;
	muutos			= -1;
	paino			= 0;
	pallarx_kerroin = 0;
	pisteet			= 0;
	random_frq		= true;
	suojaus			= VAHINKO_EI;
	tarisee         = false;
	tiletarkistus	= true;
	tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	tyyppi			= TYYPPI_EI_MIKAAN;
	vahinko			= 0;
	vahinko_tyyppi	= VAHINKO_ISKU;
	vari			= VARI_NORMAALI;
	vihollinen		= false;

	lapinakyvyys	= false;
	hehkuu			= false;
	tulitauko		= 0;
	liitokyky		= false;
	boss			= false;
	bonus_aina		= false;
	osaa_uida		= false;

	for (int i=0;i<SPRITE_MAX_AI;i++)
	{
		AI[i] = AI_EI;
	}
	
	for (i=0;i<SPRITE_MAX_FRAMEJA;i++)
	{
		if (framet[i] != 0)
			PisteDraw_Buffer_Tuhoa(this->framet[i]);
		
		if (this->framet_peilikuva[i] != 0)
			PisteDraw_Buffer_Tuhoa(this->framet_peilikuva[i]);
	}

	for (i=0;i<SPRITE_MAX_FRAMEJA;i++)
	{
		framet[i] = 0;
		framet_peilikuva[i] = 0;
	}

	for (i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			animaatiot[i].sekvenssi[j] = 0;
		
		animaatiot[i].looppi  = false;
		animaatiot[i].frameja = 0;
	}
}

int PK2Sprite_Prototyyppi::Animaatio_Uusi(int anim_i, UCHAR *sekvenssi, bool looppi)
{
	UCHAR frame_i = 0;
	
	if (anim_i < SPRITE_MAX_ANIMAATIOITA)
	{
		animaatiot[anim_i].frameja = 0;

		while (frame_i < ANIMAATIO_MAX_SEKVENSSEJA && sekvenssi[frame_i] != 0)
		{
			animaatiot[anim_i].sekvenssi[frame_i] = sekvenssi[frame_i];
			animaatiot[anim_i].frameja++;
			frame_i++;
		}
	
		animaatiot[anim_i].looppi = looppi;
	}
	return 0;
}

PK2Sprite_Prototyyppi10 PK2Sprite_Prototyyppi::GetProto10()
{
	PK2Sprite_Prototyyppi10 proto;

	strcpy(proto.kuvatiedosto,kuvatiedosto);
	strcpy(proto.nimi, nimi);
	strcpy(proto.muutos_sprite,muutos_sprite);
	strcpy(proto.bonus_sprite,bonus_sprite);
	strcpy(proto.ammus1_sprite,ammus1_sprite);
	strcpy(proto.ammus2_sprite,ammus2_sprite);

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], aanitiedostot[aani]);
		proto.aanet[aani] = aanet[aani];
	}
	
	proto.animaatioita		= animaatioita;
	proto.avain				= avain;
	proto.energia			= energia;
	proto.este				= este;
	proto.frameja			= frameja;
	proto.frame_rate		= frame_rate;
	proto.kuva_x			= kuva_x;
	proto.kuva_y			= kuva_y;
	proto.kuva_frame_leveys  = kuva_frame_leveys;
	proto.kuva_frame_korkeus = kuva_frame_korkeus;
	proto.korkeus			= korkeus;
	proto.latausaika		= latausaika;
	proto.leveys			= leveys;
	proto.max_hyppy			= max_hyppy;
	proto.max_nopeus		= (UCHAR)max_nopeus;
	proto.paino				= paino;
	proto.pisteet			= pisteet;
	proto.suojaus			= suojaus;
	proto.tuhoutuminen		= tuhoutuminen;
	proto.tyyppi			= tyyppi;
	proto.vahinko			= vahinko;
	proto.vari				= vari;
	proto.vihollinen		= vihollinen;

	for (int i=0;i<5;i++)
	{
		proto.AI[i] = AI[i];
	}
	
	for (i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			proto.animaatiot[i].sekvenssi[j] = animaatiot[i].sekvenssi[j];
		
		proto.animaatiot[i].looppi  = animaatiot[i].looppi;
		proto.animaatiot[i].frameja = animaatiot[i].frameja;
	}	

	return proto;
}

PK2Sprite_Prototyyppi11 PK2Sprite_Prototyyppi::GetProto11()
{
	PK2Sprite_Prototyyppi11 proto;

	strcpy(proto.kuvatiedosto,kuvatiedosto);
	strcpy(proto.nimi, nimi);
	strcpy(proto.muutos_sprite,muutos_sprite);
	strcpy(proto.bonus_sprite,bonus_sprite);
	strcpy(proto.ammus1_sprite,ammus1_sprite);
	strcpy(proto.ammus2_sprite,ammus2_sprite);

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], aanitiedostot[aani]);
		proto.aanet[aani] = aanet[aani];
	}
	
	proto.animaatioita		= animaatioita;
	proto.avain				= avain;
	proto.bonusten_lkm      = bonusten_lkm;
	proto.energia			= energia;
	proto.este				= este;
	proto.frameja			= frameja;
	proto.frame_rate		= frame_rate;
	proto.hyokkays1_aika	= hyokkays1_aika;
	proto.hyokkays2_aika	= hyokkays2_aika;
	proto.kuva_x			= kuva_x;
	proto.kuva_y			= kuva_y;
	proto.kuva_frame_leveys = kuva_frame_leveys;
	proto.kuva_frame_korkeus= kuva_frame_korkeus;
	proto.korkeus			= korkeus;
	proto.latausaika		= latausaika;
	proto.leveys			= leveys;
	proto.max_hyppy			= max_hyppy;
	proto.max_nopeus		= (UCHAR)max_nopeus;
	proto.paino				= paino;
	proto.pallarx_kerroin   = pallarx_kerroin;
	proto.pisteet			= pisteet;
	proto.suojaus			= suojaus;
	proto.tarisee			= tarisee;
	proto.tuhoutuminen		= tuhoutuminen;
	proto.tyyppi			= tyyppi;
	proto.vahinko			= vahinko;
	proto.vahinko_tyyppi	= vahinko_tyyppi;
	proto.vari				= vari;
	proto.vihollinen		= vihollinen;

	for (int i=0;i<5;i++)
	{
		proto.AI[i] = AI[i];
	}
	
	for (i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			proto.animaatiot[i].sekvenssi[j] = animaatiot[i].sekvenssi[j];
		
		proto.animaatiot[i].looppi  = animaatiot[i].looppi;
		proto.animaatiot[i].frameja = animaatiot[i].frameja;
	}	

	return proto;
}

PK2Sprite_Prototyyppi12 PK2Sprite_Prototyyppi::GetProto12()
{
	PK2Sprite_Prototyyppi12 proto;

	strcpy(proto.kuvatiedosto,kuvatiedosto);
	strcpy(proto.nimi, nimi);
	strcpy(proto.muutos_sprite,muutos_sprite);
	strcpy(proto.bonus_sprite,bonus_sprite);
	strcpy(proto.ammus1_sprite,ammus1_sprite);
	strcpy(proto.ammus2_sprite,ammus2_sprite);

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], aanitiedostot[aani]);
		proto.aanet[aani] = aanet[aani];
	}
	
	proto.aani_frq			= aani_frq;
	proto.animaatioita		= animaatioita;
	proto.avain				= avain;
	proto.bonusten_lkm      = bonusten_lkm;
	proto.energia			= energia;
	proto.este				= este;
	proto.este_ylos			= este_ylos;
	proto.este_alas			= este_alas;
	proto.este_oikealle		= este_oikealle;
	proto.este_vasemmalle	= este_vasemmalle;
	proto.frameja			= frameja;
	proto.frame_rate		= frame_rate;
	proto.hyokkays1_aika	= hyokkays1_aika;
	proto.hyokkays2_aika	= hyokkays2_aika;
	proto.kuva_x			= kuva_x;
	proto.kuva_y			= kuva_y;
	proto.kuva_frame_leveys = kuva_frame_leveys;
	proto.kuva_frame_korkeus= kuva_frame_korkeus;
	proto.korkeus			= korkeus;
	proto.latausaika		= latausaika;
	proto.leveys			= leveys;
	proto.max_hyppy			= max_hyppy;
	proto.max_nopeus		= (UCHAR)max_nopeus;
	proto.paino				= paino;
	proto.pallarx_kerroin   = pallarx_kerroin;
	proto.pisteet			= pisteet;
	proto.random_frq		= random_frq;
	proto.suojaus			= suojaus;
	proto.tarisee			= tarisee;
	proto.tiletarkistus		= tiletarkistus;
	proto.tuhoutuminen		= tuhoutuminen;
	proto.tyyppi			= tyyppi;
	proto.vahinko			= vahinko;
	proto.vahinko_tyyppi	= vahinko_tyyppi;
	proto.vari				= vari;
	proto.vihollinen		= vihollinen;

	for (int i=0;i<5;i++)
	{
		proto.AI[i] = AI[i];
	}
	
	for (i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			proto.animaatiot[i].sekvenssi[j] = animaatiot[i].sekvenssi[j];
		
		proto.animaatiot[i].looppi  = animaatiot[i].looppi;
		proto.animaatiot[i].frameja = animaatiot[i].frameja;
	}	

	return proto;
}

PK2Sprite_Prototyyppi13 PK2Sprite_Prototyyppi::GetProto13()
{
	PK2Sprite_Prototyyppi13 proto;

	strcpy(proto.kuvatiedosto,kuvatiedosto);
	strcpy(proto.nimi, nimi);
	strcpy(proto.muutos_sprite,muutos_sprite);
	strcpy(proto.bonus_sprite,bonus_sprite);
	strcpy(proto.ammus1_sprite,ammus1_sprite);
	strcpy(proto.ammus2_sprite,ammus2_sprite);

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], aanitiedostot[aani]);
		proto.aanet[aani] = aanet[aani];
	}
	
	proto.aani_frq			= aani_frq;
	proto.animaatioita		= animaatioita;
	proto.avain				= avain;
	proto.bonusten_lkm      = bonusten_lkm;
	proto.energia			= energia;
	proto.este				= este;
	proto.este_ylos			= este_ylos;
	proto.este_alas			= este_alas;
	proto.este_oikealle		= este_oikealle;
	proto.este_vasemmalle	= este_vasemmalle;
	proto.frameja			= frameja;
	proto.frame_rate		= frame_rate;
	proto.hyokkays1_aika	= hyokkays1_aika;
	proto.hyokkays2_aika	= hyokkays2_aika;
	proto.kuva_x			= kuva_x;
	proto.kuva_y			= kuva_y;
	proto.kuva_frame_leveys = kuva_frame_leveys;
	proto.kuva_frame_korkeus= kuva_frame_korkeus;
	proto.korkeus			= korkeus;
	proto.latausaika		= latausaika;
	proto.leveys			= leveys;
	proto.max_hyppy			= max_hyppy;
	proto.max_nopeus		= max_nopeus;
	proto.paino				= paino;
	proto.pallarx_kerroin   = pallarx_kerroin;
	proto.pisteet			= pisteet;
	proto.random_frq		= random_frq;
	proto.suojaus			= suojaus;
	proto.tarisee			= tarisee;
	proto.tiletarkistus		= tiletarkistus;
	proto.tuhoutuminen		= tuhoutuminen;
	proto.tyyppi			= tyyppi;
	proto.vahinko			= vahinko;
	proto.vahinko_tyyppi	= vahinko_tyyppi;
	proto.vari				= vari;
	proto.vihollinen		= vihollinen;

	proto.lapinakyvyys		= lapinakyvyys;
	proto.hehkuu			= hehkuu;
	proto.tulitauko			= tulitauko;
	proto.liitokyky			= liitokyky;
	proto.boss				= boss;
	proto.bonus_aina		= bonus_aina;
	proto.osaa_uida			= osaa_uida;

	for (int i=0;i<10;i++)
	{
		proto.AI[i] = AI[i];
	}
	
	for (i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			proto.animaatiot[i].sekvenssi[j] = animaatiot[i].sekvenssi[j];
		
		proto.animaatiot[i].looppi  = animaatiot[i].looppi;
		proto.animaatiot[i].frameja = animaatiot[i].frameja;
	}	

	return proto;
}

void PK2Sprite_Prototyyppi::SetProto10(PK2Sprite_Prototyyppi10 &proto)
{
	strcpy(kuvatiedosto,	proto.kuvatiedosto);
	strcpy(nimi,			proto.nimi);
	strcpy(muutos_sprite,	proto.muutos_sprite);
	strcpy(bonus_sprite,	proto.bonus_sprite);
	strcpy(ammus1_sprite,	proto.ammus1_sprite);
	strcpy(ammus2_sprite,	proto.ammus2_sprite);

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(aanitiedostot[aani], proto.aanitiedostot[aani]);
		aanet[aani] = proto.aanet[aani];
	}

	animaatioita		= proto.animaatioita;
	avain				= proto.avain;
	energia				= proto.energia;
	este				= proto.este;
	frameja				= proto.frameja;
	frame_rate			= proto.frame_rate;
	kuva_x				= proto.kuva_x;
	kuva_y				= proto.kuva_y;
	kuva_frame_leveys	= proto.kuva_frame_leveys;
	kuva_frame_korkeus	= proto.kuva_frame_korkeus;
	korkeus				= proto.korkeus;
	latausaika			= proto.latausaika;
	leveys				= proto.leveys;
	max_hyppy			= proto.max_hyppy;
	max_nopeus			= proto.max_nopeus;
	paino				= proto.paino;
	pisteet				= proto.pisteet;
	suojaus				= proto.suojaus;
	tuhoutuminen		= proto.tuhoutuminen;
	tyyppi				= proto.tyyppi;
	vahinko				= proto.vahinko;
	vari				= proto.vari;
	vihollinen			= proto.vihollinen;

	for (int i=0;i<5;i++)
	{
		AI[i] = proto.AI[i];
	}
	
	for (i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];
		
		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frameja = proto.animaatiot[i].frameja;
	}	
}

void PK2Sprite_Prototyyppi::SetProto11(PK2Sprite_Prototyyppi11 &proto)
{
	strcpy(kuvatiedosto,	proto.kuvatiedosto);
	strcpy(nimi,			proto.nimi);
	strcpy(muutos_sprite,	proto.muutos_sprite);
	strcpy(bonus_sprite,	proto.bonus_sprite);
	strcpy(ammus1_sprite,	proto.ammus1_sprite);
	strcpy(ammus2_sprite,	proto.ammus2_sprite);

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(aanitiedostot[aani], proto.aanitiedostot[aani]);
		aanet[aani] = proto.aanet[aani];
	}

	animaatioita		= proto.animaatioita;
	avain				= proto.avain;
	bonusten_lkm        = proto.bonusten_lkm;
	energia				= proto.energia;
	este				= proto.este;
	frameja				= proto.frameja;
	frame_rate			= proto.frame_rate;
	hyokkays1_aika		= proto.hyokkays1_aika;
	hyokkays2_aika		= proto.hyokkays2_aika;
	kuva_x				= proto.kuva_x;
	kuva_y				= proto.kuva_y;
	kuva_frame_leveys	= proto.kuva_frame_leveys;
	kuva_frame_korkeus	= proto.kuva_frame_korkeus;
	korkeus				= proto.korkeus;
	latausaika			= proto.latausaika;
	leveys				= proto.leveys;
	max_hyppy			= proto.max_hyppy;
	max_nopeus			= proto.max_nopeus;
	paino				= proto.paino;
	pallarx_kerroin		= proto.pallarx_kerroin;
	pisteet				= proto.pisteet;
	suojaus				= proto.suojaus;
	tarisee				= proto.tarisee;
	tuhoutuminen		= proto.tuhoutuminen;
	tyyppi				= proto.tyyppi;
	vahinko				= proto.vahinko;
	vahinko_tyyppi		= proto.vahinko_tyyppi;
	vari				= proto.vari;
	vihollinen			= proto.vihollinen;

	for (int i=0;i<5;i++)
	{
		AI[i] = proto.AI[i];
	}
	
	for (i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];
		
		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frameja = proto.animaatiot[i].frameja;
	}	
}

void PK2Sprite_Prototyyppi::SetProto12(PK2Sprite_Prototyyppi12 &proto)
{
	strcpy(kuvatiedosto,	proto.kuvatiedosto);
	strcpy(nimi,			proto.nimi);
	strcpy(muutos_sprite,	proto.muutos_sprite);
	strcpy(bonus_sprite,	proto.bonus_sprite);
	strcpy(ammus1_sprite,	proto.ammus1_sprite);
	strcpy(ammus2_sprite,	proto.ammus2_sprite);

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(aanitiedostot[aani], proto.aanitiedostot[aani]);
		aanet[aani] = proto.aanet[aani];
	}

	aani_frq			= proto.aani_frq;
	animaatioita		= proto.animaatioita;
	avain				= proto.avain;
	bonusten_lkm        = proto.bonusten_lkm;
	energia				= proto.energia;
	este				= proto.este;
	este_ylos			= proto.este_ylos;
	este_alas			= proto.este_alas;
	este_oikealle		= proto.este_oikealle;
	este_vasemmalle		= proto.este_vasemmalle;
	frameja				= proto.frameja;
	frame_rate			= proto.frame_rate;
	hyokkays1_aika		= proto.hyokkays1_aika;
	hyokkays2_aika		= proto.hyokkays2_aika;
	kuva_x				= proto.kuva_x;
	kuva_y				= proto.kuva_y;
	kuva_frame_leveys	= proto.kuva_frame_leveys;
	kuva_frame_korkeus	= proto.kuva_frame_korkeus;
	korkeus				= proto.korkeus;
	latausaika			= proto.latausaika;
	leveys				= proto.leveys;
	max_hyppy			= proto.max_hyppy;
	max_nopeus			= proto.max_nopeus;
	paino				= proto.paino;
	pallarx_kerroin		= proto.pallarx_kerroin;
	pisteet				= proto.pisteet;
	random_frq			= proto.random_frq;
	suojaus				= proto.suojaus;
	tarisee				= proto.tarisee;
	tiletarkistus		= proto.tiletarkistus;
	tuhoutuminen		= proto.tuhoutuminen;
	tyyppi				= proto.tyyppi;
	vahinko				= proto.vahinko;
	vahinko_tyyppi		= proto.vahinko_tyyppi;
	vari				= proto.vari;
	vihollinen			= proto.vihollinen;

	for (int i=0;i<5;i++)
	{
		AI[i] = proto.AI[i];
	}
	
	for (i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];
		
		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frameja = proto.animaatiot[i].frameja;
	}	
}

void PK2Sprite_Prototyyppi::SetProto13(PK2Sprite_Prototyyppi13 &proto)
{
	strcpy(kuvatiedosto,	proto.kuvatiedosto);
	strcpy(nimi,			proto.nimi);
	strcpy(muutos_sprite,	proto.muutos_sprite);
	strcpy(bonus_sprite,	proto.bonus_sprite);
	strcpy(ammus1_sprite,	proto.ammus1_sprite);
	strcpy(ammus2_sprite,	proto.ammus2_sprite);

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(aanitiedostot[aani], proto.aanitiedostot[aani]);
		aanet[aani] = proto.aanet[aani];
	}

	aani_frq			= proto.aani_frq;
	animaatioita		= proto.animaatioita;
	avain				= proto.avain;
	bonusten_lkm        = proto.bonusten_lkm;
	energia				= proto.energia;
	este				= proto.este;
	este_ylos			= proto.este_ylos;
	este_alas			= proto.este_alas;
	este_oikealle		= proto.este_oikealle;
	este_vasemmalle		= proto.este_vasemmalle;
	frameja				= proto.frameja;
	frame_rate			= proto.frame_rate;
	hyokkays1_aika		= proto.hyokkays1_aika;
	hyokkays2_aika		= proto.hyokkays2_aika;
	kuva_x				= proto.kuva_x;
	kuva_y				= proto.kuva_y;
	kuva_frame_leveys	= proto.kuva_frame_leveys;
	kuva_frame_korkeus	= proto.kuva_frame_korkeus;
	korkeus				= proto.korkeus;
	latausaika			= proto.latausaika;
	leveys				= proto.leveys;
	max_hyppy			= proto.max_hyppy;
	max_nopeus			= proto.max_nopeus;
	paino				= proto.paino;
	pallarx_kerroin		= proto.pallarx_kerroin;
	pisteet				= proto.pisteet;
	random_frq			= proto.random_frq;
	suojaus				= proto.suojaus;
	tarisee				= proto.tarisee;
	tiletarkistus		= proto.tiletarkistus;
	tuhoutuminen		= proto.tuhoutuminen;
	tyyppi				= proto.tyyppi;
	vahinko				= proto.vahinko;
	vahinko_tyyppi		= proto.vahinko_tyyppi;
	vari				= proto.vari;
	vihollinen			= proto.vihollinen;

	lapinakyvyys		= proto.lapinakyvyys;
	hehkuu				= proto.hehkuu;
	tulitauko			= proto.tulitauko;
	liitokyky			= proto.liitokyky;
	boss				= proto.boss;
	bonus_aina			= proto.bonus_aina;
	osaa_uida			= proto.osaa_uida;

	for (int i=0;i<10;i++)
	{
		AI[i] = proto.AI[i];
	}
	
	for (i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];
		
		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frameja = proto.animaatiot[i].frameja;
	}	
}

int PK2Sprite_Prototyyppi::Lataa(char *polku, char *tiedoston_nimi)
{
	this->Uusi();
	
	char kuva[_MAX_PATH];
	strcpy(kuva,"");
	strcpy(kuva,polku);
   	
	// Ladataan itse sprite-tiedosto
	
	if (strcmp(polku,"")!=0)
		strcat(polku,tiedoston_nimi);
	else
	{
		//strcpy(polku,"\\");
		strcpy(polku,tiedoston_nimi);
	}

	ifstream *tiedosto = new ifstream(polku, ios::binary | ios::nocreate);
	char versio[4];
	
	if (tiedosto->fail())
	{
		delete (tiedosto);
		return 1;
	}

	tiedosto->read ((char *)versio, 4);

	if (strcmp(versio,"1.0") == 0)
	{
		this->Uusi();
		PK2Sprite_Prototyyppi10 proto;
		tiedosto->read ((char *)&proto, sizeof (proto));
		this->SetProto10(proto);
		strcpy(this->versio,versio);
		strcpy(this->tiedosto,tiedoston_nimi);
	}
	if (strcmp(versio,"1.1") == 0)
	{
		this->Uusi();
		PK2Sprite_Prototyyppi11 proto;
		tiedosto->read ((char *)&proto, sizeof (proto));
		this->SetProto11(proto);
		strcpy(this->versio,versio);
		strcpy(this->tiedosto,tiedoston_nimi);
	}
	if (strcmp(versio,"1.2") == 0)
	{
		this->Uusi();
		PK2Sprite_Prototyyppi12 proto;
		tiedosto->read ((char *)&proto, sizeof (proto));
		this->SetProto12(proto);
		strcpy(this->versio,versio);
		strcpy(this->tiedosto,tiedoston_nimi);
	}
	if (strcmp(versio,"1.3") == 0)
	{
		this->Uusi();
		PK2Sprite_Prototyyppi13 proto;
		tiedosto->read ((char *)&proto, sizeof (proto));
		this->SetProto13(proto);
		strcpy(this->versio,versio);
		strcpy(this->tiedosto,tiedoston_nimi);
	}
	//tiedosto->read ((char *)this, sizeof (*this));

	if (tiedosto->fail())
	{
		delete (tiedosto);
		return 1;
	}
	
	delete (tiedosto);

	// Ladataan kuva jossa spriten grafiikat ovat
	
	strcat(kuva,kuvatiedosto);

	int bufferi = PisteDraw_Buffer_Uusi(640,480,false,255);
	
	if (PisteDraw_Lataa_Kuva(bufferi,kuva,false) == PD_VIRHE)
		return 1;

	if (this->vari != VARI_NORMAALI)
	{
		UCHAR *buffer = NULL;
		DWORD leveys;
		UCHAR vari;
		PisteDraw_Piirto_Aloita(bufferi,*&buffer,leveys);
		
		for (int x=0;x<640;x++)
			for (int y=0;y<480;y++)
				if ((vari = buffer[x+y*leveys]) != 255)
				{
					vari %= 32;
					vari += this->vari;
					buffer[x+y*leveys] = vari;
				}

		PisteDraw_Piirto_Lopeta(bufferi);
	}

	int frame_i = 0,
		frame_x = kuva_x,
		frame_y = kuva_y;

	for (frame_i=0; frame_i<frameja; frame_i++)
	{
		framet[frame_i] = PisteDraw_Buffer_Uusi(kuva_frame_leveys,kuva_frame_korkeus,true,255);
		framet_peilikuva[frame_i] = PisteDraw_Buffer_Uusi(kuva_frame_leveys,kuva_frame_korkeus,true,255);

		PisteDraw_Buffer_Tayta(framet[frame_i],255);
		PisteDraw_Buffer_Tayta(framet_peilikuva[frame_i],255);

		if (frame_x + kuva_frame_leveys > 640)
		{
			frame_y += this->kuva_frame_korkeus + 3;
			frame_x = kuva_x;
		}

		PisteDraw_Buffer_Flip_Nopea(bufferi,framet[frame_i],0,0,
									frame_x,
									frame_y,
									frame_x + kuva_frame_leveys,
									frame_y + kuva_frame_korkeus);

		PisteDraw_Buffer_Flip(framet[frame_i], framet_peilikuva[frame_i], 0, 0, true, false);

		frame_x += this->kuva_frame_leveys + 3;
	}

	PisteDraw_Buffer_Tuhoa(bufferi);

	return 0;
}

void PK2Sprite_Prototyyppi::Tallenna(char *tiedoston_nimi)
{
   	strcpy(this->tiedosto,tiedoston_nimi);
	
	PK2Sprite_Prototyyppi13 proto = GetProto13();
	
	//strcpy(proto.versio ,PK2SPRITE_VIIMEISIN_VERSIO);
	
	ofstream *tiedosto = new ofstream(tiedoston_nimi, ios::binary);
	tiedosto->write (PK2SPRITE_VIIMEISIN_VERSIO, 4);
	tiedosto->write ((char *)&proto, sizeof (proto));
	
	delete (tiedosto);
}

/*
int PK2Sprite_Prototyyppi::Lataa(char *filename)
{
	FILE *tiedosto;
	
	if ((tiedosto = fopen(filename, "r")) == NULL)
	{
		return 1;
	}
	
	char versio[4] = "\0";

	fread(versio, sizeof(this->versio), 1, tiedosto);
	
	fclose(tiedosto);

	// Tutkitaan versionumeroa jos tarvitsee
	
	if ((tiedosto = fopen(filename, "r")) == NULL)
	{
		return(1);
	}
	
	fread(this, sizeof(PK2Sprite_Prototyyppi), 1, tiedosto);
	
	fclose(tiedosto);

	int bufferi = PisteDraw_Buffer_Uusi(640,480,false,255);
	PisteDraw_Lataa_Kuva(bufferi,kuvatiedosto,false);

	int frame_i = 0,
		frame_x = kuva_x;

	for (frame_i=0; frame_i<frameja; frame_i++)
	{
		framet[frame_i] = PisteDraw_Buffer_Uusi(kuva_frame_leveys,kuva_frame_korkeus,true,255);
		framet_peilikuva[frame_i] = PisteDraw_Buffer_Uusi(kuva_frame_leveys,kuva_frame_korkeus,true,255);

		PisteDraw_Buffer_Tayta(framet[frame_i],255);
		PisteDraw_Buffer_Tayta(framet_peilikuva[frame_i],255);

		PisteDraw_Buffer_Flip_Nopea(bufferi,framet[frame_i],0,0,
									frame_x,
									kuva_y,
									frame_x + kuva_frame_leveys,
									kuva_y  + kuva_frame_korkeus);

		PisteDraw_Buffer_Flip(framet[frame_i], framet_peilikuva[frame_i], 0, 0, true, false);

		frame_x += 35;

	}

	PisteDraw_Buffer_Tuhoa(bufferi);

	return 0;
}

int PK2Sprite_Prototyyppi::Tallenna(char *filename)
{
	FILE *tiedosto;
	
	if ((tiedosto = fopen(filename, "w")) == NULL)
	{
		return(1);
	}
	
	fwrite(this, sizeof(PK2Sprite_Prototyyppi), 1, tiedosto);
	fclose(tiedosto);
	
	return 0;
}
*/
int PK2Sprite_Prototyyppi::Piirra(int x, int y, int frame)
{
	PisteDraw_Buffer_Flip_Nopea(framet[frame], PD_TAUSTABUFFER, x, y);

	return 0;
}

bool PK2Sprite_Prototyyppi::Onko_AI(int ai)
{
	for (int i=0;i<SPRITE_MAX_AI;i++)
	{
		if (AI[i] == ai)
			return true;
	}
	return false;
}

/* -------- PK2Sprite  ------------------------------------------------------------------ */



PK2Sprite::PK2Sprite()
{
	this->tyyppi		= NULL;
	this->pelaaja		= 0;
	this->piilota		= true;
	this->x				= 0;
	this->y				= 0;
	this->alku_x		= 0;
	this->alku_y		= 0;
	this->a				= 0;
	this->b				= 0;
	this->hyppy_ajastin	= 0;
	this->kyykky		= false;
	this->energia		= 0;
	this->alkupaino		= 0;
	this->paino			= 0;
	this->kytkinpaino   = 0;
	this->flip_x		= false;
	this->flip_y		= false;
	this->animaatio_index = ANIMAATIO_PAIKALLA;
	this->alas			= true;
	this->ylos			= true;
	this->oikealle		= true;
	this->vasemmalle	= true;
	this->reuna_oikealla	= false;
	this->reuna_vasemmalla	= false;
	this->frame_aika	= 0;
	this->sekvenssi_index = 0;
	this->isku			= 0;
	this->lataus		= 0;
	this->hyokkays1		= 0;
	this->hyokkays2		= 0;
	this->vedessa		= false;
	this->piilossa      = false;
	this->saatu_vahinko = 0;
	this->vihollinen	= false;
	this->ammus1		= -1;
	this->ammus2		= -1;
	this->pelaaja_x		= -1;
	this->pelaaja_y		= -1;
	this->ajastin		= 0;
	this->muutos_ajastin = 0;
}

PK2Sprite::PK2Sprite(PK2Sprite_Prototyyppi *tyyppi, int pelaaja, bool piilota, double x, double y)
{
	if (tyyppi)
	{
		this->tyyppi		= tyyppi;
		this->pelaaja		= pelaaja;
		this->piilota		= piilota;
		this->x				= x;
		this->y				= y;
		this->alku_x		= x;
		this->alku_y		= y;
		this->a				= 0;
		this->b				= 0;
		this->hyppy_ajastin	= 0;
		this->energia		= tyyppi->energia;
		this->alkupaino     = tyyppi->paino;
		this->paino			= this->alkupaino;
		this->kytkinpaino   = 0;
		this->kyykky		= false;
		this->flip_x		= false;
		this->flip_y		= false;
		this->animaatio_index = ANIMAATIO_PAIKALLA;
		this->alas			= true;
		this->ylos			= true;
		this->oikealle		= true;
		this->vasemmalle	= true;
		this->reuna_oikealla	= false;
		this->reuna_vasemmalla	= false;
		this->frame_aika	= 0;
		this->sekvenssi_index = 0;
		this->isku			= 0;
		this->lataus		= 0;
		this->hyokkays1		= 0;
		this->hyokkays2		= 0;
		this->vedessa		= false;
		this->piilossa      = false;
		this->saatu_vahinko = 0;
		this->vihollinen    = tyyppi->vihollinen;
		this->ammus1		= tyyppi->ammus1;
		this->ammus2		= tyyppi->ammus2;
		this->pelaaja_x		= -1;
		this->pelaaja_y		= -1;
		this->ajastin		= 0;
		this->muutos_ajastin = 0;
	}
}

PK2Sprite::~PK2Sprite() {}

bool PK2Sprite::Onko_AI(int ai)
{
	for (int i=0;i<SPRITE_MAX_AI;i++)
	{
		if (tyyppi->AI[i] == ai)
			return true;
	}
	return false;
}

int PK2Sprite::Animaatio(int anim_i, bool nollaus)
{
	if (anim_i != animaatio_index)
	{
		if (nollaus)
			sekvenssi_index = 0;

		animaatio_index = anim_i;
	}
	
	return 0;
}

int PK2Sprite::Animoi()
{
	int frame = 0;

	switch (tyyppi->AI[0])
	{
	case AI_KANA:		Animaatio_Kana();break;
	case AI_PIKKUKANA:	Animaatio_Kana();break;
	case AI_BONUS:		Animaatio_Bonus();break;
	case AI_MUNA:		Animaatio_Muna();break;
	case AI_AMMUS:		Animaatio_Ammus();break;
	case AI_HYPPIJA:	Animaatio_Kana();break;
	case AI_PERUS:		Animaatio_Perus();break;
	case AI_TELEPORTTI:	Animaatio_Perus();break;
	default:			break;
	}

	PK2SPRITE_ANIMAATIO &animaatio = tyyppi->animaatiot[animaatio_index];

	if (sekvenssi_index >= animaatio.frameja)
		sekvenssi_index = 0;
	
	frame = animaatio.sekvenssi[sekvenssi_index]-1;

	// Lasketaan kuinka paljon t‰ll‰ hetkell‰ voimassa olevaa framea viel‰ n‰ytet‰‰n
	if (frame_aika < tyyppi->frame_rate)
		frame_aika++;
	// Jos aika on kulunut loppuun, vaihdetaan seuraava frame t‰m‰nhetkisest‰ animaatiosta
	else
	{
		frame_aika = 0;
		
		// Onko animaatiossa en‰‰ frameja?
		if (sekvenssi_index < animaatio.frameja-1)
			sekvenssi_index++;
		// Jos ei ja animaatio on asetettu luuppaamaan, aloitetaan animaatio alusta.
		else
		{
			if (animaatio.looppi)
				sekvenssi_index = 0;
		}
	}

	if (frame > tyyppi->frameja)
		frame = tyyppi->frameja;

	return frame;
}

int PK2Sprite::Piirra(int kamera_x, int kamera_y)
{
	// Tehd‰‰n apumuuttujia
	int	l = (int)tyyppi->kuva_frame_leveys/2,//leveys
		h = (int)tyyppi->kuva_frame_korkeus/2,
		x = (int)this->x-(kamera_x),
		y = (int)this->y-(kamera_y),
		frame = 0;

	frame = Animoi();

	if (tyyppi->tarisee)
	{
		x+= rand()%2 - rand()%2;
		y+= rand()%2 - rand()%2;
	}
	   	
	if (flip_x)
	{
		if (!flip_y)
			PisteDraw_Buffer_Flip_Nopea(tyyppi->framet_peilikuva[frame], PD_TAUSTABUFFER, x-l-1, y-h);
		else
			PisteDraw_Buffer_Flip(tyyppi->framet_peilikuva[frame], PD_TAUSTABUFFER, x-l-1, y-h, false, true);
	}
	else
	{
		if (!flip_y)
			PisteDraw_Buffer_Flip_Nopea(tyyppi->framet[frame], PD_TAUSTABUFFER, x-l-1, y-h);
		else
			PisteDraw_Buffer_Flip(tyyppi->framet[frame], PD_TAUSTABUFFER, x-l-1, y-h, false, true);
	}

	return 0;
}

int PK2Sprite::AI_Perus()
{
	if (x < 10)
	{
		x = 10;
		vasemmalle = false;
	}

	if (x > 8192)
	{
		x = 8192;
		oikealle = false;
	}

	if (y > 9920)
	{
		y = 9920;
	}

	if (y < -32)
	{
		y = -32;
	}

	if (a < 0)
		flip_x = true;

	if (a > 0)
		flip_x = false;

	ajastin++;

	if (ajastin > 31320) // jaollinen 360:lla
		ajastin = 0;

	return 0;
}

int PK2Sprite::AI_Kaantyy_Esteesta_Hori()
{
	if (energia > 0)
	{
		if (!oikealle)
		{
			a = this->tyyppi->max_nopeus / -3.5;
			return 1;
		}

		if (!vasemmalle)
		{
			a = this->tyyppi->max_nopeus / 3.5;
			return 1;
		}
	}
	return 0;
}

int PK2Sprite::AI_Kaantyy_Esteesta_Vert()
{
	if (energia > 0)
	{
		if (!alas)
		{
			b = this->tyyppi->max_nopeus / -3.5;
			return 1;
		}

		if (!ylos)
		{
			b = this->tyyppi->max_nopeus / 3.5;
			return 1;
		}
	}
	return 0;
}

int PK2Sprite::AI_Kiipeilija()
{
	if (energia > 0)
	{
		if (!alas && vasemmalle)
		{
			b = 0;
			a = this->tyyppi->max_nopeus / -3.5;
			//return 1;
		}

		if (!ylos && oikealle)
		{
			b = 0;
			a = this->tyyppi->max_nopeus / 3.5;
			//b = this->tyyppi->max_nopeus / 3.5;
			//return 1;
		}

		if (!oikealle && alas)
		{
			a = 0;
			b = this->tyyppi->max_nopeus / 3.5;
			//return 1;
		}

		if (!vasemmalle && ylos)
		{
			a = 0;
			b = this->tyyppi->max_nopeus / -3.5;
			//return 1;
		}	
	}
	return 0;
}

int PK2Sprite::AI_Kiipeilija2()
{
	if (energia > 0)
	{
		if (vasemmalle && oikealle && ylos && alas) { 

			if (a < 0) {
				b = this->tyyppi->max_nopeus / 3.5;
				//a = 0;
			}
			else if (a > 0) {
				b = this->tyyppi->max_nopeus / -3.5;
				//a = 0;
			}
			else if (b < 0) {
				a = this->tyyppi->max_nopeus / -3.5;
				//b = 0;
			}
			else if (b > 0) {
				a = this->tyyppi->max_nopeus / 3.5;
				//b = 0;
			}
/*
			if (a != 0)
				b = 0;
*/			
			if (b != 0)
				a = 0;
		}
	}
	return 0;
}

int PK2Sprite::AI_Varoo_Kuoppaa()
{
	double max = tyyppi->max_nopeus / 3.5;

	if (energia > 0)
	{

		if (this->reuna_oikealla && this->a > -max)
		{
			this->a -= 0.13;
		}	
		
		if (this->reuna_vasemmalla && this->a < max)
		{
			this->a += 0.13;
		}
		
		/*
		if (this->reuna_oikealla && this->a > 0)
		{
			this->a = this->a * -1;
			flip_x = true;
		}

		if (this->reuna_vasemmalla && this->a < 0)
		{
			this->a = this->a * -1;	
			flip_x = false;
		}
	*/
	}
	return 0;
}

int PK2Sprite::AI_Random_Hyppy()
{
	if (energia > 0)
	{
		if (rand()%150 == 10 && b == 0 && hyppy_ajastin == 0 && ylos)
		{
			hyppy_ajastin = 1;
		}
	}
	return 0;
}

int PK2Sprite::AI_Sammakko1()
{
	if (energia > 0)
	{
		if (ajastin%100 == 0 && hyppy_ajastin == 0 && ylos)
		{
			hyppy_ajastin = 1;
		}
	}
	return 0;
}

int PK2Sprite::AI_Sammakko2()
{
	if (energia > 0)
	{
		if (ajastin%100 == 0 && ylos)
		{
			hyppy_ajastin = 1;
			
		}

		if (hyppy_ajastin > 0)
		{
			if (!flip_x)
				a = this->tyyppi->max_nopeus / 3.5;
			else
				a = this->tyyppi->max_nopeus / -3.5;
		}
	}
	return 0;
}

int PK2Sprite::AI_Random_Suunnanvaihto_Hori()
{
	if (energia > 0)
	{
		if (rand()%150 == 1)
		{
			int max = (int)tyyppi->max_nopeus / 4;

			while (a == 0 && max > 0)
				a = rand()%max+1 - rand()%max+1;
		}
	}
	return 0;
}

int PK2Sprite::AI_Random_Kaantyminen()
{
	if (energia > 0)
	{
		if (ajastin%400 == 1 && a == 0)
		{
			flip_x = !flip_x;
		}
	}
	return 0;
}

int PK2Sprite::AI_Kaantyy_Jos_Osuttu()
{
	if (isku == VAHINKO_AIKA/* saatu_vahinko*/ > 0 && energia > 0)
	{
		if (a != 0)
			a = -a;
		
		flip_x = !flip_x;
	}
	return 0;
}

int PK2Sprite::AI_Random_Liikahdus_Vert_Hori()
{
	if (energia > 0)
	{
		if (rand()%150 == 1 || ajastin == 1)
		if ((int)a == 0 || (int)b == 0)
		{
			int max = (int)tyyppi->max_nopeus;

			if (max != 0)
			{
				while (a == 0)
					a = rand()%(max+1) - rand()%(max+1);
			
				while (b == 0)
					b = rand()%(max+1) - rand()%(max+1);

				//a /= 3.0;
				//b /= 3.0;
			}
		
		}
	}
	return 0;
}

int PK2Sprite::AI_Seuraa_Pelaajaa(PK2Sprite &pelaaja)
{
	if (energia > 0 && pelaaja.energia > 0)
	{
		double max = tyyppi->max_nopeus / 3.5;

		if (a > -max && x > pelaaja.x)
		{
			a -= 0.1;
		}

		if (a < max && x < pelaaja.x)
		{
			a += 0.1;		
		}

		pelaaja_x = (int)(pelaaja.x+pelaaja.a);
		pelaaja_y = (int)(pelaaja.y+pelaaja.b);		

		if (tyyppi->max_nopeus == 0)
		{
			if (pelaaja.x < x)
				flip_x = true;
			else
				flip_x = false;
		}
	}
	return 0;
}

int PK2Sprite::AI_Pakenee_Pelaajaa_Jos_Nakee(PK2Sprite &pelaaja)
{
	if (energia > 0 && pelaaja.energia > 0)
	{
		if ((pelaaja.x < x && flip_x && !pelaaja.flip_x) || (pelaaja.x > x && !flip_x && pelaaja.flip_x))
			if ((pelaaja.x - x < 300 && pelaaja.x - x > -300) &&
				(pelaaja.y - y < tyyppi->korkeus && pelaaja.y - y > -tyyppi->korkeus))
			{
				double max = tyyppi->max_nopeus / 2.5;
				
				if (x > pelaaja.x) {
					a = max;
					flip_x = false;
				}

				if (x < pelaaja.x) {
					a = max * -1;
					flip_x = true;
				}
			}
	}


	return 0;
}

int PK2Sprite::AI_Seuraa_Pelaajaa_Jos_Nakee(PK2Sprite &pelaaja)
{
	if (energia > 0  && pelaaja.energia > 0)
	{
		double max = tyyppi->max_nopeus / 3.5;
		
		if (pelaaja_x != -1)
		{
			if (a > -max && x > pelaaja_x)
				a -= 0.1;

			if (a < max && x < pelaaja_x)
				a += 0.1;		
		}
		
		if ((pelaaja.x < x && flip_x) || (pelaaja.x > x && ! flip_x))
			if ((pelaaja.x - x < 300 && pelaaja.x - x > -300) &&
				(pelaaja.y - y < tyyppi->korkeus && pelaaja.y - y > -tyyppi->korkeus))
			{
				pelaaja_x = (int)(pelaaja.x+pelaaja.a);
				pelaaja_y = (int)(pelaaja.y+pelaaja.b);
			}
			else
			{
				pelaaja_x = -1;
				pelaaja_y = -1;
			}
	}


	return 0;
}

int PK2Sprite::AI_Jahtaa_Pelaajaa(PK2Sprite &pelaaja)
{
	if (energia > 0 && pelaaja.energia > 0)
	{
		double max = tyyppi->max_nopeus / 3.5;
		
		if (pelaaja_x != -1)
		{
			if (a > -max && x > pelaaja_x)
				a -= 0.1;

			if (a < max && x < pelaaja_x)
				a += 0.1;		
			
			if (x - pelaaja_x > -8 && x - pelaaja_x < 8) // onko sprite saavuttanut pelaajan viime sijainnin 
				pelaaja_x = -1;
		}

		if (pelaaja_y != -1 && paino == 0)
		{
			if (b > -max && y > pelaaja_y)
				b -= 0.1;

			if (b < max && y < pelaaja_y)
				b += 0.1;		
			
			if (y - pelaaja_y > -8 && y - pelaaja_y < 8) // onko sprite saavuttanut pelaajan viime sijainnin 
				pelaaja_y = -1;
		}		

		if (((pelaaja.x < x && !flip_x) || (pelaaja.x > x && flip_x)) && 
			pelaaja.hyokkays2 != pelaaja.tyyppi->latausaika)
			return 0;
		
		if ((pelaaja.x - x < 300 && pelaaja.x - x > -300) &&
			(pelaaja.y - y < tyyppi->korkeus && pelaaja.y - y > -tyyppi->korkeus))
		{
			pelaaja_x = (int)(pelaaja.x+pelaaja.a);
			pelaaja_y = (int)(pelaaja.y+pelaaja.b);
		}
	}


	return 0;
}

int PK2Sprite::AI_Seuraa_Pelaajaa_Vert_Hori(PK2Sprite &pelaaja)
{
	if (energia > 0 && pelaaja.energia > 0)
	{
		double max = tyyppi->max_nopeus / 3.5;

		if (a > -max && x > pelaaja.x)
		{
			a -= 0.1;
		}

		if (a < max && x < pelaaja.x)
		{
			a += 0.1;		
		}

		if (b > -max && y > pelaaja.y)
		{
			b -= 0.4;
		}

		if (b < max && y < pelaaja.y)
		{
			b += 0.4;		
		}

		pelaaja_x = (int)(pelaaja.x+pelaaja.a);
		pelaaja_y = (int)(pelaaja.y+pelaaja.b);		

		if (tyyppi->max_nopeus == 0)
		{
			if (pelaaja.x < x)
				flip_x = true;
			else
				flip_x = false;
		}
	}
	return 0;
}

int PK2Sprite::AI_Seuraa_Pelaajaa_Jos_Nakee_Vert_Hori(PK2Sprite &pelaaja)
{
	if (energia > 0  && pelaaja.energia > 0)
	{
		double max = tyyppi->max_nopeus / 3.5;
		
		if (pelaaja_x != -1)
		{
			if (a > -max && x > pelaaja_x)
				a -= 0.1;

			if (a < max && x < pelaaja_x)
				a += 0.1;		

			if (b > -max && y > pelaaja_y)
				b -= 0.4;

			if (b < max && y < pelaaja_y)
				b += 0.4;		
		}		

		if ((pelaaja.x < x && flip_x) || (pelaaja.x > x && ! flip_x))
			if ((pelaaja.x - x < 300 && pelaaja.x - x > -300) &&
				(pelaaja.y - y < 80 && pelaaja.y - y > -80))
			{
				pelaaja_x = (int)(pelaaja.x+pelaaja.a);
				pelaaja_y = (int)(pelaaja.y+pelaaja.b);
			}
			else
			{
				pelaaja_x = -1;
				pelaaja_y = -1;
			}
	}


	return 0;
}

int PK2Sprite::AI_Muutos_Jos_Energiaa_Alle_2(PK2Sprite_Prototyyppi &muutos)
{
	if (energia < 2 && muutos.indeksi != tyyppi->indeksi)
	{
		tyyppi = &muutos;
		alkupaino = tyyppi->paino;
		//ammus1 = tyyppi->ammus1;
		//ammus2 = tyyppi->ammus2;
		return 1;
	}

	return 0;
}

int PK2Sprite::AI_Muutos_Jos_Energiaa_Yli_1(PK2Sprite_Prototyyppi &muutos)
{
	if (energia > 1 && muutos.indeksi != tyyppi->indeksi)
	{
		tyyppi = &muutos;
		alkupaino = tyyppi->paino;
		//ammus1 = tyyppi->ammus1;
		//ammus2 = tyyppi->ammus2;
		return 1;
	}

	return 0;
}

int PK2Sprite::AI_Muutos_Ajastin(PK2Sprite_Prototyyppi &muutos)
{
	if (energia > 0 && muutos.indeksi != tyyppi->indeksi)
	{
		if (muutos_ajastin/*lataus*/ == 0)
			muutos_ajastin/*lataus*/ = tyyppi->latausaika;

		if (muutos_ajastin/*lataus*/ == 1)
		{
			tyyppi = &muutos;
			alkupaino = tyyppi->paino;

			ammus1 = tyyppi->ammus1;
			ammus2 = tyyppi->ammus2;

			animaatio_index = -1;

			Animaatio(ANIMAATIO_PAIKALLA,true);
		}
		return 1;
	}

	return 0;
}

int PK2Sprite::AI_Muutos_Jos_Osuttu(PK2Sprite_Prototyyppi &muutos)
{
	if (energia > 0 && muutos.indeksi != tyyppi->indeksi)
	{
		if (saatu_vahinko > 0) 
		{
			tyyppi = &muutos;
			alkupaino = tyyppi->paino;

			ammus1 = tyyppi->ammus1;
			ammus2 = tyyppi->ammus2;

			animaatio_index = -1;

			Animaatio(ANIMAATIO_PAIKALLA,true);

			return 1;
		}
	}

	return 0;
}

int PK2Sprite::AI_Tuhoutuu_Jos_Emo_Tuhoutuu(PK2Sprite *spritet)
{
	if (emosprite > -1)
	{
		if (spritet[emosprite].energia < 1 && energia > 0)
		{
			saatu_vahinko = energia;
			this->saatu_vahinko_tyyppi = VAHINKO_KAIKKI;

			return 1;
		}
	}

	return 0;
}

int PK2Sprite::AI_Hyokkays_1_Jos_Osuttu()
{
	if (saatu_vahinko > 0 && energia > 0)
	{
		this->hyokkays1 = this->tyyppi->hyokkays1_aika;
		this->lataus = 0;
		return 1;
	}

	return 0;
}

int PK2Sprite::AI_Hyokkays_2_Jos_Osuttu()
{
	if (saatu_vahinko > 0 && energia > 0)
	{
		this->hyokkays2 = this->tyyppi->hyokkays2_aika;
		this->lataus = 0;
		return 1;
	}

	return 0;
}

int PK2Sprite::AI_Hyokkays_1_Nonstop()
{
	if (this->lataus == 0 && energia > 0)
	{
		this->hyokkays1 = this->tyyppi->hyokkays1_aika;
		return 1;
	}

	return 0;
}

int PK2Sprite::AI_Hyokkays_2_Nonstop()
{
	if (this->lataus == 0 && energia > 0)
	{
		this->hyokkays2 = this->tyyppi->hyokkays2_aika;
		return 1;
	}

	return 0;
}

int PK2Sprite::AI_Hyokkays_1_Jos_Pelaaja_Edessa(PK2Sprite &pelaaja)
{
	if (energia > 0 && isku == 0 && pelaaja.energia > 0)
	{
		if ((pelaaja.x - x < 200 && pelaaja.x - x > -200) &&
			(pelaaja.y - y < tyyppi->korkeus && pelaaja.y - y > -tyyppi->korkeus))
		{
			if ((pelaaja.x < x && flip_x) || (pelaaja.x > x && !flip_x))
			{
				this->hyokkays1 = this->tyyppi->hyokkays1_aika;
				return 1;
			}
		}
	}	
	return 0;
}

int PK2Sprite::AI_Hyokkays_2_Jos_Pelaaja_Edessa(PK2Sprite &pelaaja)
{
	if (energia > 0 && isku == 0 && pelaaja.energia > 0)
	{
		if ((pelaaja.x - x < 200 && pelaaja.x - x > -200) &&
			(pelaaja.y - y < tyyppi->korkeus && pelaaja.y - y > -tyyppi->korkeus))
		{
			if ((pelaaja.x < x && flip_x) || (pelaaja.x > x && !flip_x))
			{
				this->hyokkays2 = this->tyyppi->hyokkays2_aika;
				return 1;
			}
		}
	}	
	return 0;
}

int PK2Sprite::AI_Hyokkays_1_Jos_Pelaaja_Alapuolella(PK2Sprite &pelaaja)
{
	if (energia > 0 && isku == 0 && pelaaja.energia > 0)
	{
		if ((pelaaja.x - x < tyyppi->leveys && pelaaja.x - x > -tyyppi->leveys) &&
			(pelaaja.y > y && pelaaja.y - y < 350))
		{
			this->hyokkays1 = this->tyyppi->hyokkays2_aika;
			return 1;
		}
	}	
	return 0;
}

int PK2Sprite::AI_Hyppy_Jos_Pelaaja_Ylapuolella(PK2Sprite &pelaaja)
{
	if (energia > 0 && hyppy_ajastin == 0 && pelaaja.energia > 0)
	{
		if ((pelaaja.x - x < tyyppi->leveys && pelaaja.x - x > -tyyppi->leveys) &&
			(pelaaja.y < y && y - pelaaja.y < 350))
		{
			hyppy_ajastin = 1;
			return 1;
		}
	}	
	return 0;
}

int PK2Sprite::AI_NonStop()
{
	if (energia > 0)
	{
	
		double max = tyyppi->max_nopeus / 3.5;
	
		if (flip_x)
		{
			if (a > -max)
				a -= 0.1;
		}
		else
		{
			if (a < max)
				a += 0.1;
		}
	}
	
	return 0;
}
int PK2Sprite::AI_Kitka_Vaikuttaa()
{

	if (energia > 0)
	{
		if (!alas)
			a /= 1.07;
		else
			a /= 1.02;
	}
	
	return 0;
}
int PK2Sprite::AI_Piiloutuu()
{

	if (energia > 0 && piilossa)
	{
		a /= 1.02;
		kyykky = true;
	}
	
	return 0;
}

int PK2Sprite::AI_Palaa_Alkuun_X()
{

	if (energia < 1 || pelaaja_x !=  -1)
		return 0;
	
	double max = tyyppi->max_nopeus / 3.5;
		
	if (x < alku_x-16 && a < max)
		a += 0.05;
		
	if (x > alku_x+16 && a > -max)
		a -= 0.05;
	
	return 0;
}

int PK2Sprite::AI_Palaa_Alkuun_Y()
{

	if (energia > 0 && pelaaja_x == -1)
	{
		double max = tyyppi->max_nopeus / 3.5;
		
		if (y < alku_y-16 && b < max)
			b += 0.04;
		
		if (y > alku_y+16 && b > -max)
			b -= 0.04;
	}
	
	return 0;
}

int PK2Sprite::AI_Tippuu_Tarinasta(int tarina)
{

	if (energia > 0 && tarina > 0)
	{
		alkupaino = 0.5;
	}
	
	return 0;
}

int PK2Sprite::AI_Vahingoittuu_Vedesta()
{
	if (energia > 0)
		if (this->vedessa)
			saatu_vahinko++;

	return 0;
}

int PK2Sprite::AI_Tapa_Kaikki()
{
	if (energia > 0)
		this->vihollinen = !this->vihollinen;

	return 0;
}

int PK2Sprite::AI_Hyppija()
{
	if (x < 10)
	{
		x = 10;
	}
	
	if (y > 9920)
	{
		y = 9920;
	}

	if (energia > 0)
	{
		if (!alas && b==0 && hyppy_ajastin == 0)
		{
			hyppy_ajastin = 1;
		}
	}

	if (a < 0)
		flip_x = true;

	if (a > 0)
		flip_x = false;
	
	return 0;
}

int PK2Sprite::AI_Liikkuu_X(double liike)
{
	if (energia > 0)
		this->x = this->alku_x + liike;

	return 0;
}

int PK2Sprite::AI_Liikkuu_Y(double liike)
{
	if (energia > 0)
	{
		this->y = this->alku_y + liike;
	}

	return 0;
}

int PK2Sprite::AI_Tippuu_Jos_Kytkin_Painettu(int kytkin)
{
	if (kytkin > 0)
	{
		alkupaino = 1.5;
	}

	return 0;
}

int PK2Sprite::AI_Liikkuu_Jos_Kytkin_Painettu(int kytkin, int ak, int bk)
{
	if (kytkin > 0)
	{
		if (a == 0 && ak != 0)
		{
			a = tyyppi->max_nopeus / 3.5 * ak; // ak = -1 / 1
		}
		
		if (b == 0 && bk != 0)
			b = tyyppi->max_nopeus / 3.5 * bk; // bk = -1 / 1
	}

	flip_x = false;

	return 0;
}

bool PK2Sprite::AI_Info(PK2Sprite &pelaaja)
{
	if ((pelaaja.x - x < 10 && pelaaja.x - x > -10) &&
		(pelaaja.y - y < tyyppi->korkeus && pelaaja.y - y > -tyyppi->korkeus))
	{
		return true;
	}

	return false;
}

int PK2Sprite::AI_Kana()
{
	if (x < 10)
	{
		x = 10;
	}
	
	if (y > 9920)
	{
		y = 9920;
	}

	if (energia > 0)
	{

		if (rand()%50 == 10 && a != 0)
			a /= 1.1;

		if (rand()%150 == 10 && b == 0 && hyppy_ajastin == 0 && ylos)
		{
			hyppy_ajastin = 1;
			while (a == 0)
				a = rand()%2 - rand()%2;
		}

		if (rand()%20 == 1 && b == 0 && hyppy_ajastin == 0 && !oikealle && !flip_x)
		{
			hyppy_ajastin = 1;
			while (a == 0)
				a = rand()%2;
		}	

		if (rand()%20 == 1 && b == 0 && hyppy_ajastin == 0 && !vasemmalle && flip_x)
		{
			hyppy_ajastin = 1;
			while (a == 0)
				a = rand()%2 * -1;
		}

		if (rand()%200 == 10)
			a = rand()%2 - rand()%2;

		if (hyppy_ajastin == tyyppi->max_hyppy && a == 0)
		{
			while (a == 0)
				a = rand()%2 - rand()%2;
		}
		/* // reunatesti 
		if (rand()%100 == 2)
			a = rand()%2-rand()%2;

		if (reuna_vasemmalla && a < 0)
			a = 0;

		if (reuna_oikealla && a > 0)
			a = 0;*/

		if (a < 0)
			flip_x = true;

		if (a > 0)
			flip_x = false;
	
	}

	return 0;
}

int PK2Sprite::AI_Bonus()
{
	if (x < 10)
	{
		x = 10;
	}
	
	if (y > 9920)
	{
		y = 9920;
	}
	
	return 0;
}

int PK2Sprite::AI_Muna()
{
	if (x < 10)
	{
		x = 10;
	}
	
	if (y > 9920)
	{
		y = 9920;
	}
	
	if (!alas)
		energia = 0;

	//a /= 1.01;

	if (energia == 0 && lataus == 0)
		lataus = tyyppi->latausaika;

	if (this->lataus == 1)
		this->piilota = true;

	return 0;
}

int PK2Sprite::AI_Ammus()
{
	if (x < 10)
	{
		x = 10;
	}
	
	if (y > 9920)
	{
		y = 9920;
	}

	if (a < 0)
		flip_x = true;

	if (a > 0)
		flip_x = false;

	if (this->lataus == 0)
		lataus = this->tyyppi->latausaika;

	if (this->lataus == 1)
	{
		this->saatu_vahinko = this->tyyppi->energia;
		this->saatu_vahinko_tyyppi = VAHINKO_KAIKKI;
	}

	if (energia < 1)
		piilota = true;

	return 0;
}

int PK2Sprite::AI_Pommi()
{
	if (this->lataus == 0)
		lataus = this->tyyppi->latausaika;

	if (this->lataus == 1)
	{
		this->saatu_vahinko = this->energia;
		this->saatu_vahinko_tyyppi = VAHINKO_KAIKKI;
	}

	return 0;
}

int PK2Sprite::AI_Teleportti(int oma_i, PK2Sprite *spritet, int max, PK2Sprite &sprite)
{
	int siirto = 0;

	if (energia > 0 && lataus == 0 && hyokkays1 == 0)
	{
		if (sprite.x <= x + tyyppi->leveys /2 && sprite.x >= x - tyyppi->leveys /2 &&
			sprite.y <= y + tyyppi->korkeus/2 && sprite.y >= y - tyyppi->korkeus/2 )
		{	
	
			int portit[1200];
			int portti_index = 0;
			int porttien_maara = 0;	
	
			// alustetaan portit-taulukko, johon ker‰t‰‰n kaikkien teleportti spritejen indeksit
			for (int i=0;i<max;i++)
				portit[i] = -1;

			// etsit‰‰n SAMANTYYPPISET teleportit
			for (i=0;i<max;i++)
				if (spritet[i].tyyppi != NULL)
					if (spritet[i].tyyppi->tyyppi == TYYPPI_TELEPORTTI && 
						tyyppi->indeksi == spritet[i].tyyppi->indeksi)
					{
						portit[portti_index] = i;
						portti_index++;
						porttien_maara++;
					}

			// jos yht‰‰n samantyyppist‰ ei lˆydy...
			if (porttien_maara == 0)
			{
				// ...etsit‰‰n KAIKKI teleportit
				portti_index = 0;
				porttien_maara = 0;

				for (i=0;i<max;i++)
					if (spritet[i].tyyppi != NULL)
						if (spritet[i].tyyppi->tyyppi == TYYPPI_TELEPORTTI)
						{
							portit[portti_index] = i;
							portti_index++;
							porttien_maara++;
						}
			}
			
			// jos viel‰k‰‰n ei lˆydy yht‰‰n teleporttia (poislukien teleportti itse), poistutaan.
			if (porttien_maara <= 1)
				return 0;

			if (porttien_maara > 1119/*599*/)
				porttien_maara = 1119/*599*/;

			// arvotaan kohdeportti
			i = portit[rand()%porttien_maara];

			while(i == oma_i || i == -1)
				i = portit[rand()%porttien_maara];

			if (spritet[i].tyyppi != NULL)
			{
				sprite.x = spritet[i].x;
				sprite.y = spritet[i].y;
				//lataus    = tyyppi->latausaika;
				//hyokkays1 = tyyppi->hyokkays1_aika;
				//spritet[i].lataus    = spritet[i].tyyppi->latausaika;
				spritet[i].hyokkays1 = spritet[i].tyyppi->hyokkays1_aika;
				lataus = 0;
				spritet[i].lataus = 0;
				siirto = 1;
			}
		}
	}
	
	return siirto;
}
/*
int PK2Sprite::AI_Teleportti(int oma_i, PK2Sprite *spritet, int max, PK2Sprite &pelaaja)
{

	int siirto = 0;
	
	if (energia > 0 && lataus == 0 && hyokkays1 == 0)
	{
		UCHAR todnak = 100; // todenn‰kˆisyys tulla valituksi kohdeteleportiksi

		if (pelaaja.x <= x + tyyppi->leveys /2 && pelaaja.x >= x - tyyppi->leveys /2 &&
			pelaaja.y <= y + tyyppi->korkeus/2 && pelaaja.y >= y - tyyppi->korkeus/2 )
		{
			for (int i=0;i<max;i++)
			{
				if (spritet[i].tyyppi != NULL)
				{
					if (spritet[i].tyyppi->tyyppi == TYYPPI_TELEPORTTI)
					{
						if (tyyppi->indeksi == spritet[i].tyyppi->indeksi && oma_i != i)
						{
							if (rand()%100 < todnak)
							{
								pelaaja.x = spritet[i].x;
								pelaaja.y = spritet[i].y;
								//lataus    = tyyppi->latausaika;
								hyokkays1 = tyyppi->hyokkays1_aika;
								//spritet[i].lataus    = spritet[i].tyyppi->latausaika;
								spritet[i].hyokkays1 = spritet[i].tyyppi->hyokkays1_aika;
								lataus = 0;
								spritet[i].lataus = 0;
								siirto = 1;
							}

							todnak /= 2;
						}
					}
				}
			}
		}
	}	
	return siirto;
}
*/
int PK2Sprite::Animaatio_Perus()
{

	int uusi_animaatio = -1;
	bool alusta = false;

	if (energia < 1 && !alas)
	{
		uusi_animaatio = ANIMAATIO_KUOLEMA;
		alusta = true;		
	}
	else
	{

		if (a > -0.2 && a < 0.2 && b == 0 && hyppy_ajastin <= 0) 
		{
			uusi_animaatio = ANIMAATIO_PAIKALLA;
			alusta = true;
		}

		if ((a < -0.2 || a > 0.2) && hyppy_ajastin <= 0)
		{
			uusi_animaatio = ANIMAATIO_KAVELY;
			alusta = false;
		}

		if (b < 0)//-0.3
		{
			uusi_animaatio = ANIMAATIO_HYPPY_YLOS;
			alusta = false;		
		}
		
		if ((hyppy_ajastin > tyyppi->max_hyppy || b > 1.5) && alas)
		{
			uusi_animaatio = ANIMAATIO_HYPPY_ALAS;
			alusta = false;	
		}

		if (kyykky)
		{
			uusi_animaatio = ANIMAATIO_KYYKKY;
			alusta = true;
		}

		if (hyokkays1 > 0)
		{
			uusi_animaatio = ANIMAATIO_HYOKKAYS1;
			alusta = true;
		}

		if (hyokkays2 > 0)
		{
			uusi_animaatio = ANIMAATIO_HYOKKAYS2;
			alusta = true;
		}

		if (isku > 0)
		{
			uusi_animaatio = ANIMAATIO_VAHINKO;
			alusta = false; 		
		}
	
	}
	
	if (uusi_animaatio != -1)
		Animaatio(uusi_animaatio,alusta);

	return 0;
}

int PK2Sprite::Animaatio_Kana()
{

	int uusi_animaatio = -1;
	bool alusta = false;

	if (energia < 1 && !alas)
	{
		uusi_animaatio = ANIMAATIO_KUOLEMA;
		alusta = true;		
	}
	else
	{

		if (a > -0.2 && a < 0.2 && b == 0 && hyppy_ajastin <= 0) 
		{
			uusi_animaatio = ANIMAATIO_PAIKALLA;
			alusta = true;
		}

		if ((a < -0.2 || a > 0.2) && hyppy_ajastin <= 0)
		{
			uusi_animaatio = ANIMAATIO_KAVELY;
			alusta = false;
		}

		if (b < 0)//-0.3
		{
			uusi_animaatio = ANIMAATIO_HYPPY_YLOS;
			alusta = false;		
		}
		
		if ((hyppy_ajastin > 90+10/*tyyppi->max_hyppy || b > 1.5*/) && alas)
		{
			uusi_animaatio = ANIMAATIO_HYPPY_ALAS;
			alusta = false;	
		}

		if (hyokkays1 > 0)
		{
			uusi_animaatio = ANIMAATIO_HYOKKAYS1;
			alusta = true;
		}

		if (hyokkays2 > 0)
		{
			uusi_animaatio = ANIMAATIO_HYOKKAYS2;
			alusta = true;
		}

		if (kyykky)
		{
			uusi_animaatio = ANIMAATIO_KYYKKY;
			alusta = true;
		}

		if (isku > 0)
		{
			uusi_animaatio = ANIMAATIO_VAHINKO;
			alusta = false; 		
		}
	
	}
	
	if (uusi_animaatio != -1)
		Animaatio(uusi_animaatio,alusta);

	return 0;
}

int PK2Sprite::Animaatio_Bonus()
{
	Animaatio(ANIMAATIO_PAIKALLA, true);
	return 0;
}

int PK2Sprite::Animaatio_Ammus()
{
	Animaatio(ANIMAATIO_PAIKALLA, true);
	return 0;
}

int PK2Sprite::Animaatio_Muna()
{
	int uusi_animaatio = 0;
	bool alusta = false;

	uusi_animaatio = ANIMAATIO_PAIKALLA; 
	alusta = true;
	
	if (energia < tyyppi->energia)
	{
		uusi_animaatio = ANIMAATIO_KUOLEMA; 
		alusta = true;		
	}

	Animaatio(uusi_animaatio,alusta);

	return 0;
}


