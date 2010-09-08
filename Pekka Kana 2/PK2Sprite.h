#ifndef PK2SPRITE
#define PK2SPRITE

typedef unsigned char  UCHAR;

const char PK2SPRITE_VIIMEISIN_VERSIO[4] = "1.3";

const int MAX_PROTOTYYPPEJA		= 100;

const int SPRITE_MAX_FRAMEJA = 50;
const int SPRITE_MAX_ANIMAATIOITA = 20;
const int SPRITE_MAX_AI = 10;
const int ANIMAATIO_MAX_SEKVENSSEJA = 10;
const int MAX_AANIA = 7;

const int ANIMAATIO_PAIKALLA	= 0;
const int ANIMAATIO_KAVELY		= 1;
const int ANIMAATIO_HYPPY_YLOS	= 2;
const int ANIMAATIO_HYPPY_ALAS	= 3;
const int ANIMAATIO_KYYKKY		= 4;
const int ANIMAATIO_VAHINKO		= 5;
const int ANIMAATIO_KUOLEMA		= 6;
const int ANIMAATIO_HYOKKAYS1	= 7;
const int ANIMAATIO_HYOKKAYS2	= 8;

const UCHAR AANI_VAHINKO		= 0;
const UCHAR AANI_TUHOUTUMINEN	= 1;
const UCHAR AANI_HYOKKAYS1		= 2;
const UCHAR AANI_HYOKKAYS2		= 3;
const UCHAR AANI_RANDOM			= 4;
const UCHAR AANI_ERIKOIS1		= 5;
const UCHAR AANI_ERIKOIS2		= 6;

const UCHAR AI_EI				= 0;
const UCHAR AI_KANA				= 1;
const UCHAR AI_MUNA				= 2;
const UCHAR AI_PIKKUKANA		= 3;
const UCHAR AI_BONUS			= 4;							//B
const UCHAR AI_HYPPIJA			= 5;
const UCHAR AI_PERUS			= 6;							//B
const UCHAR AI_KAANTYY_ESTEESTA_HORI		= 7;
const UCHAR AI_VAROO_KUOPPAA				= 8;
const UCHAR AI_RANDOM_SUUNNANVAIHTO_HORI	= 9;
const UCHAR AI_RANDOM_HYPPY					= 10;
const UCHAR AI_SEURAA_PELAAJAA				= 11;
const UCHAR AI_RANDOM_ALOITUSSUUNTA_HORI	= 12;
const UCHAR AI_SEURAA_PELAAJAA_JOS_NAKEE	= 13;
const UCHAR AI_MUUTOS_JOS_ENERGIAA_ALLE_2	= 14;				//P
const UCHAR AI_MUUTOS_JOS_ENERGIAA_YLI_1	= 15;				//P
const UCHAR AI_ALOITUSSUUNTA_PELAAJAA_KOHTI = 16;
const UCHAR AI_AMMUS						= 17;
const UCHAR AI_NONSTOP						= 18;
const UCHAR AI_HYOKKAYS_1_JOS_OSUTTU		= 19;
const UCHAR AI_POMMI						= 20;
const UCHAR AI_HYOKKAYS_1_JOS_PELAAJA_EDESSA		= 21;
const UCHAR AI_HYOKKAYS_1_JOS_PELAAJA_ALAPUOLELLA	= 22;
const UCHAR AI_VAHINGOITTUU_VEDESTA					= 23;		//P
const UCHAR AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA		= 24;
const UCHAR AI_TAPA_KAIKKI					= 25;
const UCHAR AI_KITKA_VAIKUTTAA				= 26;
const UCHAR AI_PIILOUTUU					= 27;
const UCHAR AI_PALAA_ALKUUN_X				= 28;
const UCHAR AI_PALAA_ALKUUN_Y				= 29;
const UCHAR AI_TELEPORTTI				    = 30;
const UCHAR AI_HEITTOASE					= 35;
const UCHAR AI_TIPPUU_TARINASTA			    = 36;				//B
const UCHAR AI_VAIHDA_KALLOT_JOS_TYRMATTY   = 37;
const UCHAR AI_VAIHDA_KALLOT_JOS_OSUTTU	    = 38;
const UCHAR AI_TUHOUTUU_JOS_EMO_TUHOUTUU	= 39;

const UCHAR AI_LIIKKUU_X_COS			    = 41;
const UCHAR AI_LIIKKUU_Y_COS			    = 42;
const UCHAR AI_LIIKKUU_X_SIN			    = 43;
const UCHAR AI_LIIKKUU_Y_SIN			    = 44;
const UCHAR AI_LIIKKUU_X_COS_NOPEA		    = 45;
const UCHAR AI_LIIKKUU_Y_SIN_NOPEA		    = 46;
const UCHAR AI_LIIKKUU_X_COS_HIDAS		    = 47;
const UCHAR AI_LIIKKUU_Y_SIN_HIDAS		    = 48;
const UCHAR AI_LIIKKUU_Y_SIN_VAPAA		    = 49;

const UCHAR AI_RANDOM_KAANTYMINEN		    = 50;
const UCHAR AI_HYPPY_JOS_PELAAJA_YLAPUOLELLA= 51;
const UCHAR AI_MUUTOS_AJASTIN				= 52;				//B

const UCHAR AI_TIPPUU_JOS_KYTKIN1_PAINETTU  = 53;				//B
const UCHAR AI_TIPPUU_JOS_KYTKIN2_PAINETTU  = 54;				//B
const UCHAR AI_TIPPUU_JOS_KYTKIN3_PAINETTU  = 55;				//B

const UCHAR AI_LIIKKUU_ALAS_JOS_KYTKIN1_PAINETTU		= 56;
const UCHAR AI_LIIKKUU_YLOS_JOS_KYTKIN1_PAINETTU		= 57;
const UCHAR AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN1_PAINETTU	= 58;
const UCHAR AI_LIIKKUU_OIKEALLE_JOS_KYTKIN1_PAINETTU	= 59;
const UCHAR AI_LIIKKUU_ALAS_JOS_KYTKIN2_PAINETTU		= 60;
const UCHAR AI_LIIKKUU_YLOS_JOS_KYTKIN2_PAINETTU		= 61;
const UCHAR AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN2_PAINETTU	= 62;
const UCHAR AI_LIIKKUU_OIKEALLE_JOS_KYTKIN2_PAINETTU	= 63;
const UCHAR AI_LIIKKUU_ALAS_JOS_KYTKIN3_PAINETTU		= 64;
const UCHAR AI_LIIKKUU_YLOS_JOS_KYTKIN3_PAINETTU		= 65;
const UCHAR AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN3_PAINETTU	= 66;
const UCHAR AI_LIIKKUU_OIKEALLE_JOS_KYTKIN3_PAINETTU	= 67;

const UCHAR AI_KAANTYY_ESTEESTA_VERT		= 70;				//v1.2
const UCHAR AI_RANDOM_ALOITUSSUUNTA_VERT	= 71;				//v1.2
const UCHAR AI_ALOITUSSUUNTA_PELAAJAA_KOHTI_VERT = 72;			//v1.2
const UCHAR AI_KIIPEILIJA					= 73;				//v1.2
const UCHAR AI_KIIPEILIJA2					= 74;				//v1.2
const UCHAR AI_PAKENEE_PELAAJAA_JOS_NAKEE	= 75;				//v1.2
const UCHAR AI_UUSI_JOS_TUHOUTUU			= 76;				//v1.2	//B

const UCHAR AI_NUOLI_VASEMMALLE				= 77;				//v1.2
const UCHAR AI_NUOLI_OIKEALLE				= 78;				//v1.2
const UCHAR AI_NUOLI_YLOS					= 79;				//v1.2
const UCHAR AI_NUOLI_ALAS					= 80;				//v1.2
const UCHAR AI_NUOLET_VAIKUTTAVAT			= 81;				//v1.2

const UCHAR AI_TAUSTA_KUU				    = 101;
const UCHAR AI_TAUSTA_LIIKKUU_VASEMMALLE    = 102;
const UCHAR AI_TAUSTA_LIIKKUU_OIKEALLE      = 103;

const UCHAR AI_BONUS_AIKA			= 120;						 //B
const UCHAR AI_BONUS_NAKYMATTOMYYS	= 121;						 //B
const UCHAR AI_BONUS_SUPERVAUHTI	= 124;						 //B
const UCHAR AI_BONUS_SUPERHYPPY		= 122;						 //B
const UCHAR AI_BONUS_SUPERTULITUS	= 123;						 //B

const UCHAR AI_MUUTOS_JOS_OSUTTU			= 129;				//P
const UCHAR AI_SEURAA_PELAAJAA_VERT_HORI	= 130;
const UCHAR AI_SEURAA_PELAAJAA_JOS_NAKEE_VERT_HORI	= 131;
const UCHAR AI_RANDOM_LIIKAHDUS_VERT_HORI	= 132;
const UCHAR AI_SAMMAKKO1					= 133;
const UCHAR AI_SAMMAKKO2					= 134;
const UCHAR AI_SAMMAKKO3					= 135;
const UCHAR AI_HYOKKAYS_2_JOS_OSUTTU		= 136;
const UCHAR AI_HYOKKAYS_1_NONSTOP			= 137;
const UCHAR AI_HYOKKAYS_2_NONSTOP			= 138;
const UCHAR AI_KAANTYY_JOS_OSUTTU			= 139;
const UCHAR AI_EVIL_ONE						= 140;

const UCHAR AI_INFO1	= 201; // use doodle attack to knock out enemys.
const UCHAR AI_INFO2	= 202; // use egg attack to knock out enemys.
const UCHAR AI_INFO3	= 203; // collect all keys to open all locks.
const UCHAR AI_INFO4	= 204; // you can activate switches by jumping on them.
const UCHAR AI_INFO5	= 205; // green switches activate green blocks.
const UCHAR AI_INFO6	= 206; // blue switches activate blue blocks.
const UCHAR AI_INFO7	= 207; // orange switches turn off fire.
const UCHAR AI_INFO8	= 208; // collect gifts to obtain useful items.
const UCHAR AI_INFO9	= 209; // if you are hit, feathers will give you energy.
const UCHAR AI_INFO10	= 210; // a drink from a magic bottle will change you...
const UCHAR AI_INFO11	= 211; // drink doodle drinks to and get special doodle attacks.
const UCHAR AI_INFO12	= 212; // drink hatching-booster and get special egg attacks.
const UCHAR AI_INFO13	= 213; // look for hidden bonuses!
const UCHAR AI_INFO14	= 214; // you can stomp on enemys, but you will lose energy.
const UCHAR AI_INFO15	= 215; // look out for traps!
const UCHAR AI_INFO16	= 216; // hurry up! your time is limited!
const UCHAR AI_INFO17	= 217; // be aware that some enemys resist certain attacks.
const UCHAR AI_INFO18	= 218; // a hen is mightier than the sword.
const UCHAR AI_INFO19	= 219; // why did the chicken cross the road?

//P = koskee myös pelaajaa
//B = koskee myös bonuksia


const UCHAR VAHINKO_EI		= 0;
const UCHAR VAHINKO_ISKU	= 1;
const UCHAR VAHINKO_PUDOTUS	= 2;
const UCHAR VAHINKO_MELU	= 3;
const UCHAR VAHINKO_TULI	= 4;
const UCHAR VAHINKO_VESI	= 5;
const UCHAR VAHINKO_LUMI	= 6;
const UCHAR VAHINKO_BONUS	= 7;
const UCHAR VAHINKO_SAHKO	= 8;
const UCHAR VAHINKO_ITSARI	= 9;
const UCHAR VAHINKO_PURISTUS= 10;
const UCHAR VAHINKO_HAJU	= 11;
const UCHAR VAHINKO_KAIKKI	= 12;
const UCHAR VAHINKO_PISTO	= 13;

const int PROTOTYYPPI_KANA		= 0;
const int PROTOTYYPPI_MUNA		= 1;
const int PROTOTYYPPI_PIKKUKANA	= 2;
const int PROTOTYYPPI_OMENA		= 3;
const int PROTOTYYPPI_HYPPIJA	= 4;

const int TYYPPI_EI_MIKAAN		= 0;
const int TYYPPI_PELIHAHMO		= 1;
const int TYYPPI_BONUS			= 2;
const int TYYPPI_AMMUS			= 3;
const int TYYPPI_TELEPORTTI		= 4;
const int TYYPPI_TAUSTA			= 5;

const UCHAR VARI_HARMAA			= 0;
const UCHAR VARI_SININEN		= 32;
const UCHAR VARI_PUNAINEN		= 64;
const UCHAR VARI_VIHREA			= 96;
const UCHAR VARI_ORANSSI		= 128;
const UCHAR VARI_VIOLETTI		= 160;
const UCHAR VARI_TURKOOSI		= 192;
const UCHAR VARI_NORMAALI		= 255;

const UCHAR	TUHOUTUMINEN_EI_TUHOUDU			= 0;
const UCHAR	TUHOUTUMINEN_ANIMAATIO			= 100;
const UCHAR	TUHOUTUMINEN_HOYHENET			= 1;
const UCHAR	TUHOUTUMINEN_TAHDET_HARMAA		= 2;
const UCHAR	TUHOUTUMINEN_TAHDET_SININEN		= 3;
const UCHAR	TUHOUTUMINEN_TAHDET_PUNAINEN	= 4;
const UCHAR	TUHOUTUMINEN_TAHDET_VIHREA		= 5;
const UCHAR	TUHOUTUMINEN_TAHDET_ORANSSI		= 6;
const UCHAR	TUHOUTUMINEN_TAHDET_VIOLETTI	= 7;
const UCHAR	TUHOUTUMINEN_TAHDET_TURKOOSI	= 8;
const UCHAR	TUHOUTUMINEN_RAJAHDYS_HARMAA	= 9;
const UCHAR	TUHOUTUMINEN_RAJAHDYS_SININEN	= 10;
const UCHAR	TUHOUTUMINEN_RAJAHDYS_PUNAINEN	= 11;
const UCHAR	TUHOUTUMINEN_RAJAHDYS_VIHREA	= 12;
const UCHAR	TUHOUTUMINEN_RAJAHDYS_ORANSSI	= 13;
const UCHAR	TUHOUTUMINEN_RAJAHDYS_VIOLETTI	= 14;
const UCHAR	TUHOUTUMINEN_RAJAHDYS_TURKOOSI	= 15;
const UCHAR	TUHOUTUMINEN_SAVU_HARMAA		= 16;
const UCHAR	TUHOUTUMINEN_SAVU_SININEN		= 17;
const UCHAR	TUHOUTUMINEN_SAVU_PUNAINEN		= 18;
const UCHAR	TUHOUTUMINEN_SAVU_VIHREA		= 19;
const UCHAR	TUHOUTUMINEN_SAVU_ORANSSI		= 20;
const UCHAR	TUHOUTUMINEN_SAVU_VIOLETTI		= 21;
const UCHAR	TUHOUTUMINEN_SAVU_TURKOOSI		= 22;
const UCHAR	TUHOUTUMINEN_SAVUPILVET			= 23;

const UCHAR BONUSESINE_EI_MITAAN	= 0;
const UCHAR BONUSESINE_AVAIN		= 1;
const UCHAR BONUSESINE_PISTE		= 2;

const int	VAHINKO_AIKA  = 50;
//const int	HYOKKAYS_AIKA = 60;

struct PK2SPRITE_ANIMAATIO
{
	UCHAR		sekvenssi[ANIMAATIO_MAX_SEKVENSSEJA];	// taulukko frame-indekseistä (sekvenssi)
	UCHAR		frameja;								// framejen määrä
	bool		looppi;									// onko looppaava animaatio
};

// Talletusmuoto versiolle 1.0
class PK2Sprite_Prototyyppi10
{
public:
	// Versionumero

	int			tyyppi;											// spriten tyyppi
	// Kuvatiedoston tiedot
	char		kuvatiedosto[13];								// .BMP jossa ovat spriten grafiikat
	// Spriteen liittyvät äänitehosteet
	char		aanitiedostot[7][13];							// Äänitehostetiedostot
	int			aanet[7];										// Äänitehosteet (indeksit buffereihin)
	// Spriten kuva- ja animaatio-ominaisuudet
	UCHAR		frameja;										// framejen määrä
	PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
	UCHAR		animaatioita;									// animaatioiden määrä
	UCHAR		frame_rate;										// yhden framen kesto
	int			kuva_x;											// missä kohtaa kuvaa sprite on
	int			kuva_y;											// missä kohtaa kuvaa sprite on
	int			kuva_frame_leveys;								// yhden framen leveys
	int			kuva_frame_korkeus;								// yhden framen korkeus
	int			kuva_frame_vali;								// kahden framen vali
	// Spriten ominaisuudet
	char		nimi[30];										// spriten nimi (näkyy editorissa)
	int			leveys;											// spriten leveys
	int			korkeus;										// spriten korkeus
	double		paino;											// sprite paino (vaikuttaa hyppyyn ja kytkimiin)
	bool		vihollinen;										// onko sprite vihollinen
	int			energia;										// monta iskua kestää
	int			vahinko;										// paljon vahinkoa tekee jos osuu
	int			pisteet;										// paljonko siitä saa pisteitä
	int			AI[5];											// mitä tekoälyjä käytetään
	int			suojaus;										// miten suojattu iskuilta 
	UCHAR		max_hyppy;										// hypyn maksimikesto
	UCHAR		max_nopeus;										// maksimi nopeus
	int			latausaika;										// ampumisen jälkeinen odotus
	UCHAR		vari;											// tehdäänkö spritestä jonkin tietyn värinen
	bool		este;											// käyttäytyykö sprite kuin seinä
	int			tuhoutuminen;									// miten sprite tuhoutuu
	// Yhteys toisiin spriteihin
	char		muutos_sprite[13];								// Toinen sprite joksi tämä sprite voi muuttua
	char		bonus_sprite[13];								// Spriten bonuksena jättämä käyttämä sprite
	char		ammus1_sprite[13];								// Spriten ammuksena 1 käyttämä sprite
	char		ammus2_sprite[13];								// Spriten ammuksena 2 käyttämä sprite
	bool		avain;											// Voiko sprite avata lukkoja
};

// Talletusmuoto versiolle 1.1
class PK2Sprite_Prototyyppi11
{
public:
	// Versionumero

	int			tyyppi;											// spriten tyyppi
	// Kuvatiedoston tiedot
	char		kuvatiedosto[13];								// .BMP jossa ovat spriten grafiikat
	// Spriteen liittyvät äänitehosteet
	char		aanitiedostot[7][13];							// Äänitehostetiedostot
	int			aanet[7];										// Äänitehosteet (indeksit buffereihin)
	// Spriten kuva- ja animaatio-ominaisuudet
	UCHAR		frameja;										// framejen määrä
	PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
	UCHAR		animaatioita;									// animaatioiden määrä
	UCHAR		frame_rate;										// yhden framen kesto
	int			kuva_x;											// missä kohtaa kuvaa sprite on
	int			kuva_y;											// missä kohtaa kuvaa sprite on
	int			kuva_frame_leveys;								// yhden framen leveys
	int			kuva_frame_korkeus;								// yhden framen korkeus
	int			kuva_frame_vali;								// kahden framen vali
	// Spriten ominaisuudet
	char		nimi[30];										// spriten nimi (näkyy editorissa)
	int			leveys;											// spriten leveys
	int			korkeus;										// spriten korkeus
	double		paino;											// sprite paino (vaikuttaa hyppyyn ja kytkimiin)
	bool		vihollinen;										// onko sprite vihollinen
	int			energia;										// monta iskua kestää
	int			vahinko;										// paljon vahinkoa tekee jos osuu
	UCHAR       vahinko_tyyppi;									// Minkä tyyppistä vahinkoa tekee (1.1)
	UCHAR		suojaus;										// Minkä tyyppiseltä vahingolta on suojattu
	int			pisteet;										// paljonko siitä saa pisteitä
	int			AI[5];											// mitä tekoälyjä käytetään
	UCHAR		max_hyppy;										// hypyn maksimikesto
	UCHAR		max_nopeus;										// maksimi nopeus
	int			latausaika;										// ampumisen jälkeinen odotus
	UCHAR		vari;											// tehdäänkö spritestä jonkin tietyn värinen
	bool		este;											// käyttäytyykö sprite kuin seinä
	int			tuhoutuminen;									// miten sprite tuhoutuu
	bool		avain;											// Voiko sprite avata lukkoja
	bool		tarisee;										// Tariseekö sprite satunnaisesti
	UCHAR       bonusten_lkm;									// Bonusten lukumäärä
	int         hyokkays1_aika;									// Hyökkäysanimaation 1 kesto (frameja)
	int         hyokkays2_aika;									// Hyökkäysanimaation 2 kesto (frameja)
	int			pallarx_kerroin;								// Vain TYYPPI_TAUSTA. Suhde taustakuvaan.
	// Yhteys toisiin spriteihin
	char		muutos_sprite[13];								// Toinen sprite joksi tämä sprite voi muuttua
	char		bonus_sprite[13];								// Spriten bonuksena jättämä käyttämä sprite
	char		ammus1_sprite[13];								// Spriten ammuksena 1 käyttämä sprite
	char		ammus2_sprite[13];								// Spriten ammuksena 2 käyttämä sprite
	
};

// Talletusmuoto versiolle 1.2
class PK2Sprite_Prototyyppi12
{
public:
	// Versionumero

	int			tyyppi;											// spriten tyyppi
	// Kuvatiedoston tiedot
	char		kuvatiedosto[13];								// .BMP jossa ovat spriten grafiikat
	// Spriteen liittyvät äänitehosteet
	char		aanitiedostot[7][13];							// Äänitehostetiedostot
	int			aanet[7];										// Äänitehosteet (indeksit buffereihin)
	// Spriten kuva- ja animaatio-ominaisuudet
	UCHAR		frameja;										// framejen määrä
	PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
	UCHAR		animaatioita;									// animaatioiden määrä
	UCHAR		frame_rate;										// yhden framen kesto
	int			kuva_x;											// missä kohtaa kuvaa sprite on
	int			kuva_y;											// missä kohtaa kuvaa sprite on
	int			kuva_frame_leveys;								// yhden framen leveys
	int			kuva_frame_korkeus;								// yhden framen korkeus
	int			kuva_frame_vali;								// kahden framen vali
	// Spriten ominaisuudet
	char		nimi[30];										// spriten nimi (näkyy editorissa)
	int			leveys;											// spriten leveys
	int			korkeus;										// spriten korkeus
	double		paino;											// sprite paino (vaikuttaa hyppyyn ja kytkimiin)
	bool		vihollinen;										// onko sprite vihollinen
	int			energia;										// monta iskua kestää
	int			vahinko;										// paljon vahinkoa tekee jos osuu
	UCHAR       vahinko_tyyppi;									// Minkä tyyppistä vahinkoa tekee (1.1)
	UCHAR		suojaus;										// Minkä tyyppiseltä vahingolta on suojattu
	int			pisteet;										// paljonko siitä saa pisteitä
	int			AI[5];											// mitä tekoälyjä käytetään
	UCHAR		max_hyppy;										// hypyn maksimikesto
	UCHAR		max_nopeus;										// maksimi nopeus
	int			latausaika;										// ampumisen jälkeinen odotus
	UCHAR		vari;											// tehdäänkö spritestä jonkin tietyn värinen
	bool		este;											// käyttäytyykö sprite kuin seinä
	int			tuhoutuminen;									// miten sprite tuhoutuu
	bool		avain;											// Voiko sprite avata lukkoja
	bool		tarisee;										// Tariseekö sprite satunnaisesti
	UCHAR       bonusten_lkm;									// Bonusten lukumäärä
	int         hyokkays1_aika;									// Hyökkäysanimaation 1 kesto (frameja)
	int         hyokkays2_aika;									// Hyökkäysanimaation 2 kesto (frameja)
	int			pallarx_kerroin;								// Vain TYYPPI_TAUSTA. Suhde taustakuvaan.
	// Yhteys toisiin spriteihin
	char		muutos_sprite[13];								// Toinen sprite joksi tämä sprite voi muuttua
	char		bonus_sprite[13];								// Spriten bonuksena jättämä käyttämä sprite
	char		ammus1_sprite[13];								// Spriten ammuksena 1 käyttämä sprite
	char		ammus2_sprite[13];								// Spriten ammuksena 2 käyttämä sprite
	
	bool		tiletarkistus;									// törmäileekö tileihin
	DWORD		aani_frq;										// äänien perussoittotaajuus (esim. 22050)
	bool		random_frq;										// satunnaisuutta taajuuteen?
	
	// Jos sprite on este
	bool		este_ylos;
	bool		este_alas;
	bool		este_oikealle;
	bool		este_vasemmalle;

};

// Talletusmuoto versiolle 1.3
class PK2Sprite_Prototyyppi13
{
public:
	// Versionumero

	int			tyyppi;											// spriten tyyppi
	// Kuvatiedoston tiedot
	char		kuvatiedosto[100];								// .BMP jossa ovat spriten grafiikat
	// Spriteen liittyvät äänitehosteet
	char		aanitiedostot[7][100];							// Äänitehostetiedostot
	int			aanet[7];										// Äänitehosteet (indeksit buffereihin)
	// Spriten kuva- ja animaatio-ominaisuudet
	UCHAR		frameja;										// framejen määrä
	PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
	UCHAR		animaatioita;									// animaatioiden määrä
	UCHAR		frame_rate;										// yhden framen kesto
	int			kuva_x;											// missä kohtaa kuvaa sprite on
	int			kuva_y;											// missä kohtaa kuvaa sprite on
	int			kuva_frame_leveys;								// yhden framen leveys
	int			kuva_frame_korkeus;								// yhden framen korkeus
	int			kuva_frame_vali;								// kahden framen vali
	// Spriten ominaisuudet
	char		nimi[30];										// spriten nimi (näkyy editorissa)
	int			leveys;											// spriten leveys
	int			korkeus;										// spriten korkeus
	double		paino;											// sprite paino (vaikuttaa hyppyyn ja kytkimiin)
	bool		vihollinen;										// onko sprite vihollinen
	int			energia;										// monta iskua kestää
	int			vahinko;										// paljon vahinkoa tekee jos osuu
	UCHAR       vahinko_tyyppi;									// Minkä tyyppistä vahinkoa tekee (1.1)
	UCHAR		suojaus;										// Minkä tyyppiseltä vahingolta on suojattu
	int			pisteet;										// paljonko siitä saa pisteitä
	int			AI[10];											// mitä tekoälyjä käytetään
	UCHAR		max_hyppy;										// hypyn maksimikesto
	double		max_nopeus;										// maksimi nopeus
	int			latausaika;										// ampumisen jälkeinen odotus
	UCHAR		vari;											// tehdäänkö spritestä jonkin tietyn värinen
	bool		este;											// käyttäytyykö sprite kuin seinä
	int			tuhoutuminen;									// miten sprite tuhoutuu
	bool		avain;											// Voiko sprite avata lukkoja
	bool		tarisee;										// Tariseekö sprite satunnaisesti
	UCHAR       bonusten_lkm;									// Bonusten lukumäärä
	int         hyokkays1_aika;									// Hyökkäysanimaation 1 kesto (frameja)
	int         hyokkays2_aika;									// Hyökkäysanimaation 2 kesto (frameja)
	int			pallarx_kerroin;								// Vain TYYPPI_TAUSTA. Suhde taustakuvaan.
	// Yhteys toisiin spriteihin
	char		muutos_sprite[100];								// Toinen sprite joksi tämä sprite voi muuttua
	char		bonus_sprite[100];								// Spriten bonuksena jättämä käyttämä sprite
	char		ammus1_sprite[100];								// Spriten ammuksena 1 käyttämä sprite
	char		ammus2_sprite[100];								// Spriten ammuksena 2 käyttämä sprite
	
	bool		tiletarkistus;									// törmäileekö tileihin
	DWORD		aani_frq;										// äänien perussoittotaajuus (esim. 22050)
	bool		random_frq;										// satunnaisuutta taajuuteen?
	
	// Jos sprite on este
	bool		este_ylos;
	bool		este_alas;
	bool		este_oikealle;
	bool		este_vasemmalle;

	UCHAR		lapinakyvyys;									// 0 = ei näy - 100 = ei läpinäkyvä
	bool		hehkuu;											// hehkuuko (täytyy olla myös läpinäkyvä)
	int			tulitauko;										// ammuspriten ampujalle aiheuttama latausaika
	bool		liitokyky;										// voiko tippua hiljaa alas?
	bool		boss;											// onko johtaja
	bool		bonus_aina;										// jättää aina bonuksen tuhoutuessa
	bool		osaa_uida;										// vaikuttaako painovoima vedessä?
};

// Varsinaiset "oikeat" luokat ----------------------------------------------------------------------

class PK2Sprite_Prototyyppi
{
	
public:
	
	// Versionumero
	char		versio[4];										// versionumero (latausta varten)
	char		tiedosto[255];									// spriten oma tiedosto (.spr)
	int			indeksi;										// monesko prototyyppi on taulukossa

	int			tyyppi;											// spriten tyyppi

	// Kuvatiedoston tiedot
	char		kuvatiedosto[100];								// .BMP jossa ovat spriten grafiikat

	// Spriteen liittyvät äänitehosteet

	char		aanitiedostot[MAX_AANIA][100];					// Äänitehostetiedostot
	int			aanet[MAX_AANIA];								// Äänitehosteet (indeksit buffereihin)

	// Spriten kuva- ja animaatio-ominaisuudet
	int			framet[SPRITE_MAX_FRAMEJA];						// spriten framet (bitmäpit)
	int			framet_peilikuva[SPRITE_MAX_FRAMEJA];			// spriten framet peilikuvina
	UCHAR		frameja;										// framejen määrä
	PK2SPRITE_ANIMAATIO animaatiot[SPRITE_MAX_ANIMAATIOITA];	// animaatio sekvenssit
	UCHAR		animaatioita;									// animaatioiden määrä
	UCHAR		frame_rate;										// yhden framen kesto
	int			kuva_x;											// missä kohtaa kuvaa sprite on
	int			kuva_y;											// missä kohtaa kuvaa sprite on
	int			kuva_frame_leveys;								// yhden framen leveys
	int			kuva_frame_korkeus;								// yhden framen korkeus
	int			kuva_frame_vali;								// kahden framen vali
	
	// Spriten ominaisuudet
	char		nimi[30];										// spriten nimi (näkyy editorissa)
	int			leveys;											// spriten leveys
	int			korkeus;										// spriten korkeus
	double		paino;											// sprite paino (vaikuttaa hyppyyn ja kytkimiin)

	bool		vihollinen;										// onko sprite vihollinen
	int			energia;										// monta iskua kestää
	int			vahinko;										// paljon vahinkoa tekee jos osuu
	UCHAR       vahinko_tyyppi;									// minkä tyyppistä vahinkoa tekee (1.1)									
	UCHAR		suojaus;										// minkä tyyppiseltä vahingolta on suojattu (1.1)
	int			pisteet;										// paljonko siitä saa pisteitä
	
	int			AI[SPRITE_MAX_AI];								// mitä tekoälyjä käytetään
	
	UCHAR		max_hyppy;										// hypyn maksimikesto
	double		max_nopeus;										// maksiminopeus
	int			latausaika;										// ampumisen jälkeinen odotus
	UCHAR		vari;											// tehdäänkö spritestä jonkin tietyn värinen
	bool		este;											// käyttäytyykö sprite kuin seinä
	int			tuhoutuminen;									// miten sprite tuhoutuu
	bool		avain;											// Voiko sprite avata lukkoja
	bool		tarisee;										// Täriseekö sprite satunnaisesti
	UCHAR       bonusten_lkm;									// Bonusten lukumäärä
	int         hyokkays1_aika;									// Hyökkäysanimaation 1 kesto (frameja)
	int         hyokkays2_aika;									// Hyökkäysanimaation 2 kesto (frameja)

	int			pallarx_kerroin;								// Vain TYYPPI_TAUSTA. Suhde taustakuvaan.

	// Yhteys toisiin spriteihin

	char		muutos_sprite[100];								// Toinen sprite joksi tämä sprite voi muuttua
	char		bonus_sprite[100];								// Spriten bonuksena jättämä käyttämä sprite
	char		ammus1_sprite[100];								// Spriten ammuksena 1 käyttämä sprite
	char		ammus2_sprite[100];								// Spriten ammuksena 2 käyttämä sprite
	int			muutos;											// Muutosspriten prototyypin indeksi. -1 jos ei ole
	int			bonus;											// Bonusspriten prototyypin indeksi. -1 jos ei ole
	int			ammus1;											// Ammussprite1 prototyypin indeksi. -1 jos ei ole
	int			ammus2;											// Ammussprite1 prototyypin indeksi. -1 jos ei ole
	
	// Lisäykset 1.2 versiossa
	bool		tiletarkistus;									// törmäileekö tileihin
	DWORD		aani_frq;										// äänien perussoittotaajuus (esim. 22050)
	bool		random_frq;										// satunnaisuutta taajuuteen?

	// Jos sprite on este
	bool		este_ylos;
	bool		este_alas;
	bool		este_oikealle;
	bool		este_vasemmalle;

	// Lisäykset 1.3 versiossa
	UCHAR		lapinakyvyys;									// 0 = ei näy - 100 = ei läpinäkyvä
	bool		hehkuu;											// 0 = ei hehku (täytyy olla läpinäkyvä)
	int			tulitauko;										// ammuspriten ampujalle aiheuttama latausaika
	bool		liitokyky;										// voiko tippua hiljaa alas?
	bool		boss;											// onko johtaja
	bool		bonus_aina;										// jättää aina bonuksen tuhoutuessa
	bool		osaa_uida;										// vaikuttaako painovoima vedessä?
	
	// Muodostimet
	PK2Sprite_Prototyyppi();
	~PK2Sprite_Prototyyppi();
	
	// Metodit
	void Uusi();
	void Kopioi(const PK2Sprite_Prototyyppi &proto);
	int  Animaatio_Uusi(int anim_i, UCHAR *sekvenssi, bool looppi);
	int  Lataa(char *polku, char *tiedoston_nimi);
	void Tallenna(char *tiedoston_nimi);
	int  Piirra(int x, int y, int frame);
	bool Onko_AI(int AI);

	PK2Sprite_Prototyyppi10 GetProto10();
	void SetProto10(PK2Sprite_Prototyyppi10 &proto);
	PK2Sprite_Prototyyppi11 GetProto11();
	void SetProto11(PK2Sprite_Prototyyppi11 &proto);	
	PK2Sprite_Prototyyppi12 GetProto12();
	void SetProto12(PK2Sprite_Prototyyppi12 &proto);
	PK2Sprite_Prototyyppi13 GetProto13();
	void SetProto13(PK2Sprite_Prototyyppi13 &proto);
	
};

class PK2Sprite
{
	public:

	bool		aktiivinen;			// true / false
	int			pelaaja;			// 0 = ei pelaaja, 1 = pelaaja
	PK2Sprite_Prototyyppi *tyyppi;	// osoitin spriten prototyyppiin
	bool		piilota;			// true = ei toiminnassa, false = toiminnassa		
	double		alku_x;				// spriten alkuperäinen x sijainti
	double		alku_y;				// spriten alkuperäinen y sijainti
	double		x;					// x-kordinaatti pelikentällä
	double		y;					// y-kordinaatti pelikentällä
	double		a;					// liike sivusuunnassa
	double		b;					// liike pystysuunnassa
	bool		flip_x;				// true = peilikuva sivusuunnassa
	bool		flip_y;				// true = peilikuva pystysuunnassa
	int			hyppy_ajastin;		// hypyn kesto: 0 = ei hypyssä, > 0 = hypyssä, < 0 = tullut alas 
	bool		ylos;				// voiko sprite juuri nyt liikkua ylös
	bool		alas;				// voiko sprite juuri nyt liikkua ....
	bool		oikealle;			// voiko sprite juuri nyt liikkua ....
	bool		vasemmalle;			// voiko sprite juuri nyt liikkua ....
	bool		reuna_vasemmalla;	// onko spriten vasemmalla puolella kuoppa?
	bool		reuna_oikealla;		// onko spriten vasemmalla puolella kuoppa?
	int			energia;			// monta osumaa sprite vielä kestää
	int			emosprite;			// jos spriten on luonut jokin toinen sprite
	double		paino;				// spriten oma yksilöllinen paino
	double		kytkinpaino;		// spriten paino + muiden spritejen panot, joihin kosketaan
	bool		kyykky;				// onko sprite kyykyssä
	int			isku;				// onko sprite saanut vahinkoa
	int			lataus;				// jos on ammuttu, odotetaan
	int			hyokkays1;			// ajastin joka laskee hyökkäys 1:n jälkeen
	int			hyokkays2;			// ajastin joka laskee hyökkäys 2:n jälkeen
	bool		vedessa;			// onko sprite vedessä
	bool		piilossa;			// onko sprite piilossa
	double      alkupaino;			// spriten alkuperäinen paino - sama kuin prototyypin 
	int			saatu_vahinko;		// vähennetään energiasta jos erisuuri kuin 0
	UCHAR       saatu_vahinko_tyyppi; // saadun vahingon tyyppi (esim. lumi).
	bool		vihollinen;			// prototyypistä saatu tieto, onko vihollinen
	int			ammus1;				// spriten käyttämän ammus-prototyypin indeksi
	int			ammus2;				//

	int			pelaaja_x,			// tieto siitä, missä pelaaja on nähty viimeksi
				pelaaja_y;

	int			ajastin;			// ajastin jonka arvo pyörii välillä 1 - 32000

	UCHAR		animaatio_index;	// tämänhetkinen pyörivä animaatio
	UCHAR		sekvenssi_index;	// tämänhetkisen animaation frame
	UCHAR		frame_aika;			// kuinka kauan frame on näkynyt
	int			muutos_ajastin;		// sprite muuttuu muutosspriteksi kun tämä nollautuu

	PK2Sprite();
	PK2Sprite(PK2Sprite_Prototyyppi *tyyppi, int pelaaja, bool piilota, double x, double y);
	~PK2Sprite();
	int Piirra(int kamera_x, int kamera_y);		// Animoi ja piirtää spriten
	int Animaatio(int anim_i, bool nollaus);	// Vaihtaa spriten animaation
	int Animoi();								// Animoi muttei piirrä spriteä
	bool Onko_AI(int AI);						// Palauttaa true, jos spritellä on ko. AI
	int AI_Kana();
	int AI_Bonus();
	int AI_Muna();
	int AI_Ammus();
	int AI_Hyppija();
	int AI_Sammakko1();
	int AI_Sammakko2();
	int AI_Perus();
	int AI_Kaantyy_Esteesta_Hori();
	int AI_Kaantyy_Esteesta_Vert();
	int AI_Varoo_Kuoppaa();
	int AI_Random_Kaantyminen();
	int AI_Random_Suunnanvaihto_Hori();
	int AI_Random_Hyppy();
	int AI_Random_Liikahdus_Vert_Hori();
	int AI_Seuraa_Pelaajaa(PK2Sprite &pelaaja);
	int AI_Seuraa_Pelaajaa_Jos_Nakee(PK2Sprite &pelaaja);
	int AI_Seuraa_Pelaajaa_Jos_Nakee_Vert_Hori(PK2Sprite &pelaaja);
	int AI_Seuraa_Pelaajaa_Vert_Hori(PK2Sprite &pelaaja);
	int AI_Jahtaa_Pelaajaa(PK2Sprite &pelaaja);
	int AI_Pakenee_Pelaajaa_Jos_Nakee(PK2Sprite &pelaaja);
	int AI_Muutos_Jos_Energiaa_Alle_2(PK2Sprite_Prototyyppi &muutos);
	int AI_Muutos_Jos_Energiaa_Yli_1(PK2Sprite_Prototyyppi &muutos);
	int AI_Muutos_Ajastin(PK2Sprite_Prototyyppi &muutos);
	int AI_Muutos_Jos_Osuttu(PK2Sprite_Prototyyppi &muutos);
	int AI_Hyokkays_1_Jos_Osuttu();
	int AI_Hyokkays_2_Jos_Osuttu();
	int AI_Hyokkays_1_Nonstop();
	int AI_Hyokkays_2_Nonstop();
	int AI_Hyokkays_1_Jos_Pelaaja_Edessa(PK2Sprite &pelaaja);
	int AI_Hyokkays_2_Jos_Pelaaja_Edessa(PK2Sprite &pelaaja);
	int AI_Hyokkays_1_Jos_Pelaaja_Alapuolella(PK2Sprite &pelaaja);
	int AI_NonStop();
	int AI_Hyppy_Jos_Pelaaja_Ylapuolella(PK2Sprite &pelaaja);
	int AI_Pommi();
	int AI_Vahingoittuu_Vedesta();
	int AI_Tapa_Kaikki();
	int AI_Kitka_Vaikuttaa();
	int AI_Piiloutuu();
	int AI_Palaa_Alkuun_X();
	int AI_Palaa_Alkuun_Y();
	int AI_Kaantyy_Jos_Osuttu();
	int AI_Tippuu_Tarinasta(int tarina);
	int AI_Liikkuu_X(double liike);
	int AI_Liikkuu_Y(double liike);
	int AI_Tippuu_Jos_Kytkin_Painettu(int kytkin);
	int AI_Liikkuu_Jos_Kytkin_Painettu(int kytkin, int ak, int bk);
	int AI_Teleportti(int i, PK2Sprite *spritet, int max, PK2Sprite &pelaaja);
	int AI_Kiipeilija();
	int AI_Kiipeilija2();
	//int AI_Nuolet_Vaikuttavat();
	bool AI_Info(PK2Sprite &pelaaja);
	int AI_Tuhoutuu_Jos_Emo_Tuhoutuu(PK2Sprite *spritet);

	int Animaatio_Perus();
	int Animaatio_Kana();
	int Animaatio_Bonus();
	int Animaatio_Muna();
	int Animaatio_Ammus();
};

#endif