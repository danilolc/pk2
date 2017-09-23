//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################

#ifndef PK2SPRITE
#define PK2SPRITE

#include "types.h"

const char PK2SPRITE_VIIMEISIN_VERSIO[4] = "1.3";

const int MAX_SPRITEJA = 800;
const int MAX_PROTOTYYPPEJA = 100;

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

const BYTE AANI_VAHINKO		= 0;
const BYTE AANI_TUHOUTUMINEN	= 1;
const BYTE AANI_HYOKKAYS1		= 2;
const BYTE AANI_HYOKKAYS2		= 3;
const BYTE AANI_RANDOM			= 4;
const BYTE AANI_ERIKOIS1		= 5;
const BYTE AANI_ERIKOIS2		= 6;

const BYTE AI_EI				= 0;
const BYTE AI_KANA				= 1;
const BYTE AI_MUNA				= 2;
const BYTE AI_PIKKUKANA		= 3;
const BYTE AI_BONUS			= 4;							//B
const BYTE AI_HYPPIJA			= 5;
const BYTE AI_PERUS			= 6;							//B
const BYTE AI_KAANTYY_ESTEESTA_HORI		= 7;
const BYTE AI_VAROO_KUOPPAA				= 8;
const BYTE AI_RANDOM_SUUNNANVAIHTO_HORI	= 9;
const BYTE AI_RANDOM_HYPPY					= 10;
const BYTE AI_SEURAA_PELAAJAA				= 11;
const BYTE AI_RANDOM_ALOITUSSUUNTA_HORI	= 12;
const BYTE AI_SEURAA_PELAAJAA_JOS_NAKEE	= 13;
const BYTE AI_MUUTOS_JOS_ENERGIAA_ALLE_2	= 14;				//P
const BYTE AI_MUUTOS_JOS_ENERGIAA_YLI_1	= 15;				//P
const BYTE AI_ALOITUSSUUNTA_PELAAJAA_KOHTI = 16;
const BYTE AI_AMMUS						= 17;
const BYTE AI_NONSTOP						= 18;
const BYTE AI_HYOKKAYS_1_JOS_OSUTTU		= 19;
const BYTE AI_POMMI						= 20;
const BYTE AI_HYOKKAYS_1_JOS_PELAAJA_EDESSA		= 21;
const BYTE AI_HYOKKAYS_1_JOS_PELAAJA_ALAPUOLELLA	= 22;
const BYTE AI_VAHINGOITTUU_VEDESTA					= 23;		//P
const BYTE AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA		= 24;
const BYTE AI_TAPA_KAIKKI					= 25;
const BYTE AI_KITKA_VAIKUTTAA				= 26;
const BYTE AI_PIILOUTUU					= 27;
const BYTE AI_PALAA_ALKUUN_X				= 28;
const BYTE AI_PALAA_ALKUUN_Y				= 29;
const BYTE AI_TELEPORTTI				    = 30;
const BYTE AI_HEITTOASE					= 35;
const BYTE AI_TIPPUU_TARINASTA			    = 36;				//B
const BYTE AI_VAIHDA_KALLOT_JOS_TYRMATTY   = 37;
const BYTE AI_VAIHDA_KALLOT_JOS_OSUTTU	    = 38;
const BYTE AI_TUHOUTUU_JOS_EMO_TUHOUTUU	= 39;

const BYTE AI_LIIKKUU_X_COS			    = 41;
const BYTE AI_LIIKKUU_Y_COS			    = 42;
const BYTE AI_LIIKKUU_X_SIN			    = 43;
const BYTE AI_LIIKKUU_Y_SIN			    = 44;
const BYTE AI_LIIKKUU_X_COS_NOPEA		    = 45;
const BYTE AI_LIIKKUU_Y_SIN_NOPEA		    = 46;
const BYTE AI_LIIKKUU_X_COS_HIDAS		    = 47;
const BYTE AI_LIIKKUU_Y_SIN_HIDAS		    = 48;
const BYTE AI_LIIKKUU_Y_SIN_VAPAA		    = 49;

const BYTE AI_RANDOM_KAANTYMINEN		    = 50;
const BYTE AI_HYPPY_JOS_PELAAJA_YLAPUOLELLA= 51;
const BYTE AI_MUUTOS_AJASTIN				= 52;				//B

const BYTE AI_TIPPUU_JOS_KYTKIN1_PAINETTU  = 53;				//B
const BYTE AI_TIPPUU_JOS_KYTKIN2_PAINETTU  = 54;				//B
const BYTE AI_TIPPUU_JOS_KYTKIN3_PAINETTU  = 55;				//B

const BYTE AI_LIIKKUU_ALAS_JOS_KYTKIN1_PAINETTU		= 56;
const BYTE AI_LIIKKUU_YLOS_JOS_KYTKIN1_PAINETTU		= 57;
const BYTE AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN1_PAINETTU	= 58;
const BYTE AI_LIIKKUU_OIKEALLE_JOS_KYTKIN1_PAINETTU	= 59;
const BYTE AI_LIIKKUU_ALAS_JOS_KYTKIN2_PAINETTU		= 60;
const BYTE AI_LIIKKUU_YLOS_JOS_KYTKIN2_PAINETTU		= 61;
const BYTE AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN2_PAINETTU	= 62;
const BYTE AI_LIIKKUU_OIKEALLE_JOS_KYTKIN2_PAINETTU	= 63;
const BYTE AI_LIIKKUU_ALAS_JOS_KYTKIN3_PAINETTU		= 64;
const BYTE AI_LIIKKUU_YLOS_JOS_KYTKIN3_PAINETTU		= 65;
const BYTE AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN3_PAINETTU	= 66;
const BYTE AI_LIIKKUU_OIKEALLE_JOS_KYTKIN3_PAINETTU	= 67;

const BYTE AI_KAANTYY_ESTEESTA_VERT		= 70;				//v1.2
const BYTE AI_RANDOM_ALOITUSSUUNTA_VERT	= 71;				//v1.2
const BYTE AI_ALOITUSSUUNTA_PELAAJAA_KOHTI_VERT = 72;			//v1.2
const BYTE AI_KIIPEILIJA					= 73;				//v1.2
const BYTE AI_KIIPEILIJA2					= 74;				//v1.2
const BYTE AI_PAKENEE_PELAAJAA_JOS_NAKEE	= 75;				//v1.2
const BYTE AI_UUSI_JOS_TUHOUTUU			= 76;				//v1.2	//B

const BYTE AI_NUOLI_VASEMMALLE				= 77;				//v1.2
const BYTE AI_NUOLI_OIKEALLE				= 78;				//v1.2
const BYTE AI_NUOLI_YLOS					= 79;				//v1.2
const BYTE AI_NUOLI_ALAS					= 80;				//v1.2
const BYTE AI_NUOLET_VAIKUTTAVAT			= 81;				//v1.2

const BYTE AI_TAUSTA_KUU				    = 101;
const BYTE AI_TAUSTA_LIIKKUU_VASEMMALLE    = 102;
const BYTE AI_TAUSTA_LIIKKUU_OIKEALLE      = 103;

const BYTE AI_BONUS_AIKA			= 120;						 //B
const BYTE AI_BONUS_NAKYMATTOMYYS	= 121;						 //B
const BYTE AI_BONUS_SUPERVAUHTI	= 124;						 //B
const BYTE AI_BONUS_SUPERHYPPY		= 122;						 //B
const BYTE AI_BONUS_SUPERTULITUS	= 123;						 //B

const BYTE AI_MUUTOS_JOS_OSUTTU			= 129;				//P
const BYTE AI_SEURAA_PELAAJAA_VERT_HORI	= 130;
const BYTE AI_SEURAA_PELAAJAA_JOS_NAKEE_VERT_HORI	= 131;
const BYTE AI_RANDOM_LIIKAHDUS_VERT_HORI	= 132;
const BYTE AI_SAMMAKKO1					= 133;
const BYTE AI_SAMMAKKO2					= 134;
const BYTE AI_SAMMAKKO3					= 135;
const BYTE AI_HYOKKAYS_2_JOS_OSUTTU		= 136;
const BYTE AI_HYOKKAYS_1_NONSTOP			= 137;
const BYTE AI_HYOKKAYS_2_NONSTOP			= 138;
const BYTE AI_KAANTYY_JOS_OSUTTU			= 139;
const BYTE AI_EVIL_ONE						= 140;

const BYTE AI_INFO1	= 201; // use doodle attack to knock out enemys.
const BYTE AI_INFO2	= 202; // use egg attack to knock out enemys.
const BYTE AI_INFO3	= 203; // collect all keys to open all locks.
const BYTE AI_INFO4	= 204; // you can activate switches by jumping on them.
const BYTE AI_INFO5	= 205; // green switches activate green blocks.
const BYTE AI_INFO6	= 206; // blue switches activate blue blocks.
const BYTE AI_INFO7	= 207; // orange switches turn off fire.
const BYTE AI_INFO8	= 208; // collect gifts to obtain useful items.
const BYTE AI_INFO9	= 209; // if you are hit, feathers will give you energy.
const BYTE AI_INFO10	= 210; // a drink from a magic bottle will change you...
const BYTE AI_INFO11	= 211; // drink doodle drinks to and get special doodle attacks.
const BYTE AI_INFO12	= 212; // drink hatching-booster and get special egg attacks.
const BYTE AI_INFO13	= 213; // look for hidden bonuses!
const BYTE AI_INFO14	= 214; // you can stomp on enemys, but you will lose energy.
const BYTE AI_INFO15	= 215; // look out for traps!
const BYTE AI_INFO16	= 216; // hurry up! your time is limited!
const BYTE AI_INFO17	= 217; // be aware that some enemys resist certain attacks.
const BYTE AI_INFO18	= 218; // a hen is mightier than the sword.
const BYTE AI_INFO19	= 219; // why did the chicken cross the road?

const BYTE VAHINKO_EI		= 0;
const BYTE VAHINKO_ISKU	= 1;
const BYTE VAHINKO_PUDOTUS	= 2;
const BYTE VAHINKO_MELU	= 3;
const BYTE VAHINKO_TULI	= 4;
const BYTE VAHINKO_VESI	= 5;
const BYTE VAHINKO_LUMI	= 6;
const BYTE VAHINKO_BONUS	= 7;
const BYTE VAHINKO_SAHKO	= 8;
const BYTE VAHINKO_ITSARI	= 9;
const BYTE VAHINKO_PURISTUS= 10;
const BYTE VAHINKO_HAJU	= 11;
const BYTE VAHINKO_KAIKKI	= 12;
const BYTE VAHINKO_PISTO	= 13;

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

const BYTE VARI_HARMAA			= 0;
const BYTE VARI_SININEN		= 32;
const BYTE VARI_PUNAINEN		= 64;
const BYTE VARI_VIHREA			= 96;
const BYTE VARI_ORANSSI		= 128;
const BYTE VARI_VIOLETTI		= 160;
const BYTE VARI_TURKOOSI		= 192;
const BYTE VARI_NORMAALI		= 255;

const BYTE	TUHOUTUMINEN_EI_TUHOUDU			= 0;
const BYTE	TUHOUTUMINEN_ANIMAATIO			= 100;
const BYTE	TUHOUTUMINEN_HOYHENET			= 1;
const BYTE	TUHOUTUMINEN_TAHDET_HARMAA		= 2;
const BYTE	TUHOUTUMINEN_TAHDET_SININEN		= 3;
const BYTE	TUHOUTUMINEN_TAHDET_PUNAINEN	= 4;
const BYTE	TUHOUTUMINEN_TAHDET_VIHREA		= 5;
const BYTE	TUHOUTUMINEN_TAHDET_ORANSSI		= 6;
const BYTE	TUHOUTUMINEN_TAHDET_VIOLETTI	= 7;
const BYTE	TUHOUTUMINEN_TAHDET_TURKOOSI	= 8;
const BYTE	TUHOUTUMINEN_RAJAHDYS_HARMAA	= 9;
const BYTE	TUHOUTUMINEN_RAJAHDYS_SININEN	= 10;
const BYTE	TUHOUTUMINEN_RAJAHDYS_PUNAINEN	= 11;
const BYTE	TUHOUTUMINEN_RAJAHDYS_VIHREA	= 12;
const BYTE	TUHOUTUMINEN_RAJAHDYS_ORANSSI	= 13;
const BYTE	TUHOUTUMINEN_RAJAHDYS_VIOLETTI	= 14;
const BYTE	TUHOUTUMINEN_RAJAHDYS_TURKOOSI	= 15;
const BYTE	TUHOUTUMINEN_SAVU_HARMAA		= 16;
const BYTE	TUHOUTUMINEN_SAVU_SININEN		= 17;
const BYTE	TUHOUTUMINEN_SAVU_PUNAINEN		= 18;
const BYTE	TUHOUTUMINEN_SAVU_VIHREA		= 19;
const BYTE	TUHOUTUMINEN_SAVU_ORANSSI		= 20;
const BYTE	TUHOUTUMINEN_SAVU_VIOLETTI		= 21;
const BYTE	TUHOUTUMINEN_SAVU_TURKOOSI		= 22;
const BYTE	TUHOUTUMINEN_SAVUPILVET			= 23;

const BYTE BONUSESINE_EI_MITAAN	= 0;
const BYTE BONUSESINE_AVAIN		= 1;
const BYTE BONUSESINE_PISTE		= 2;

const int	VAHINKO_AIKA  = 50;
//const int	HYOKKAYS_AIKA = 60;

struct PK2SPRITE_ANIMAATIO{
	BYTE		sekvenssi[ANIMAATIO_MAX_SEKVENSSEJA];	// taulukko frame-indekseist� (sequence)
	BYTE		frameja;								// framejen m��r�
	bool		looppi;									// onko looppaava animaatio
};

class PK2Sprite_Prototyyppi10{
	public:
	// Versionumero

	int			tyyppi;											// spriten tyyppi
	// Kuvatiedoston tiedot
	char		kuvatiedosto[13];								// .BMP jossa ovat spriten grafiikat
	// Spriteen liittyv�t ��nitehosteet
	char		aanitiedostot[7][13];							// ��nitehostetiedostot
	int			aanet[7];										// ��nitehosteet (indeksit buffereihin)
	// Spriten kuva- ja animaatio-ominaisuudet
	BYTE		frameja;										// framejen m��r�
	PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
	BYTE		animaatioita;									// animaatioiden m��r�
	BYTE		frame_rate;										// yhden framen kesto
	int			kuva_x;											// miss� kohtaa kuvaa sprite on
	int			kuva_y;											// miss� kohtaa kuvaa sprite on
	int			kuva_frame_leveys;								// yhden framen leveys
	int			kuva_frame_korkeus;								// yhden framen korkeus
	int			kuva_frame_vali;								// kahden framen vali
	// Spriten ominaisuudet
	char		nimi[30];										// spriten nimi (n�kyy editorissa)
	int			leveys;											// spriten leveys
	int			korkeus;										// spriten korkeus
	double		paino;											// sprite paino (vaikuttaa hyppyyn ja kytkimiin)
	bool		vihollinen;										// onko sprite vihollinen
	int			energia;										// monta iskua kest��
	int			vahinko;										// paljon vahinkoa tekee jos osuu
	int			pisteet;										// paljonko siit� saa pisteit�
	int			AI[5];											// mit� teko�lyj� k�ytet��n
	int			suojaus;										// miten suojattu iskuilta
	BYTE		max_hyppy;										// hypyn maksimikesto
	BYTE		max_nopeus;										// maksimi nopeus
	int			latausaika;										// ampumisen j�lkeinen odotus
	BYTE		vari;											// tehd��nk� spritest� jonkin tietyn v�rinen
	bool		este;											// k�ytt�ytyyk� sprite kuin sein�
	int			tuhoutuminen;									// miten sprite tuhoutuu
	// Yhteys toisiin spriteihin
	char		muutos_sprite[13];								// Toinen sprite joksi t�m� sprite voi muuttua
	char		bonus_sprite[13];								// Spriten bonuksena j�tt�m� k�ytt�m� sprite
	char		ammus1_sprite[13];								// Spriten ammuksena 1 k�ytt�m� sprite
	char		ammus2_sprite[13];								// Spriten ammuksena 2 k�ytt�m� sprite
	bool		avain;											// Voiko sprite avata lukkoja
};
class PK2Sprite_Prototyyppi11{
	public:
	// Versionumero

	int			tyyppi;											// spriten tyyppi
	// Kuvatiedoston tiedot
	char		kuvatiedosto[13];								// .BMP jossa ovat spriten grafiikat
	// Spriteen liittyv�t ��nitehosteet
	char		aanitiedostot[7][13];							// ��nitehostetiedostot
	int			aanet[7];										// ��nitehosteet (indeksit buffereihin)
	// Spriten kuva- ja animaatio-ominaisuudet
	BYTE		frameja;										// framejen m��r�
	PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
	BYTE		animaatioita;									// animaatioiden m��r�
	BYTE		frame_rate;										// yhden framen kesto
	int			kuva_x;											// miss� kohtaa kuvaa sprite on
	int			kuva_y;											// miss� kohtaa kuvaa sprite on
	int			kuva_frame_leveys;								// yhden framen leveys
	int			kuva_frame_korkeus;								// yhden framen korkeus
	int			kuva_frame_vali;								// kahden framen vali
	// Spriten ominaisuudet
	char		nimi[30];										// spriten nimi (n�kyy editorissa)
	int			leveys;											// spriten leveys
	int			korkeus;										// spriten korkeus
	double		paino;											// sprite paino (vaikuttaa hyppyyn ja kytkimiin)
	bool		vihollinen;										// onko sprite vihollinen
	int			energia;										// monta iskua kest��
	int			vahinko;										// paljon vahinkoa tekee jos osuu
	BYTE       vahinko_tyyppi;									// Mink� tyyppist� vahinkoa tekee (1.1)
	BYTE		suojaus;										// Mink� tyyppiselt� vahingolta on suojattu
	int			pisteet;										// paljonko siit� saa pisteit�
	int			AI[5];											// mit� teko�lyj� k�ytet��n
	BYTE		max_hyppy;										// hypyn maksimikesto
	BYTE		max_nopeus;										// maksimi nopeus
	int			latausaika;										// ampumisen j�lkeinen odotus
	BYTE		vari;											// tehd��nk� spritest� jonkin tietyn v�rinen
	bool		este;											// k�ytt�ytyyk� sprite kuin sein�
	int			tuhoutuminen;									// miten sprite tuhoutuu
	bool		avain;											// Voiko sprite avata lukkoja
	bool		tarisee;										// Tariseek� sprite satunnaisesti
	BYTE       bonusten_lkm;									// Bonusten lukum��r�
	int         hyokkays1_aika;									// Hy�kk�ysanimaation 1 kesto (frameja)
	int         hyokkays2_aika;									// Hy�kk�ysanimaation 2 kesto (frameja)
	int			pallarx_kerroin;								// Vain TYYPPI_TAUSTA. Suhde taustakuvaan.
	// Yhteys toisiin spriteihin
	char		muutos_sprite[13];								// Toinen sprite joksi t�m� sprite voi muuttua
	char		bonus_sprite[13];								// Spriten bonuksena j�tt�m� k�ytt�m� sprite
	char		ammus1_sprite[13];								// Spriten ammuksena 1 k�ytt�m� sprite
	char		ammus2_sprite[13];								// Spriten ammuksena 2 k�ytt�m� sprite

};
class PK2Sprite_Prototyyppi12{
	public:
	// Versionumero

	int			tyyppi;											// spriten tyyppi
	// Kuvatiedoston tiedot
	char		kuvatiedosto[13];								// .BMP jossa ovat spriten grafiikat
	// Spriteen liittyv�t ��nitehosteet
	char		aanitiedostot[7][13];							// ��nitehostetiedostot
	int			aanet[7];										// ��nitehosteet (indeksit buffereihin)
	// Spriten kuva- ja animaatio-ominaisuudet
	BYTE		frameja;										// framejen m��r�
	PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
	BYTE		animaatioita;									// animaatioiden m��r�
	BYTE		frame_rate;										// yhden framen kesto
	int			kuva_x;											// miss� kohtaa kuvaa sprite on
	int			kuva_y;											// miss� kohtaa kuvaa sprite on
	int			kuva_frame_leveys;								// yhden framen leveys
	int			kuva_frame_korkeus;								// yhden framen korkeus
	int			kuva_frame_vali;								// kahden framen vali
	// Spriten ominaisuudet
	char		nimi[30];										// spriten nimi (n�kyy editorissa)
	int			leveys;											// spriten leveys
	int			korkeus;										// spriten korkeus
	double		paino;											// sprite paino (vaikuttaa hyppyyn ja kytkimiin)
	bool		vihollinen;										// onko sprite vihollinen
	int			energia;										// monta iskua kest��
	int			vahinko;										// paljon vahinkoa tekee jos osuu
	BYTE       vahinko_tyyppi;									// Mink� tyyppist� vahinkoa tekee (1.1)
	BYTE		suojaus;										// Mink� tyyppiselt� vahingolta on suojattu
	int			pisteet;										// paljonko siit� saa pisteit�
	int			AI[5];											// mit� teko�lyj� k�ytet��n
	BYTE		max_hyppy;										// hypyn maksimikesto
	BYTE		max_nopeus;										// maksimi nopeus
	int			latausaika;										// ampumisen j�lkeinen odotus
	BYTE		vari;											// tehd��nk� spritest� jonkin tietyn v�rinen
	bool		este;											// k�ytt�ytyyk� sprite kuin sein�
	int			tuhoutuminen;									// miten sprite tuhoutuu
	bool		avain;											// Voiko sprite avata lukkoja
	bool		tarisee;										// Tariseek� sprite satunnaisesti
	BYTE       bonusten_lkm;									// Bonusten lukum��r�
	int         hyokkays1_aika;									// Hy�kk�ysanimaation 1 kesto (frameja)
	int         hyokkays2_aika;									// Hy�kk�ysanimaation 2 kesto (frameja)
	int			pallarx_kerroin;								// Vain TYYPPI_TAUSTA. Suhde taustakuvaan.
	// Yhteys toisiin spriteihin
	char		muutos_sprite[13];								// Toinen sprite joksi t�m� sprite voi muuttua
	char		bonus_sprite[13];								// Spriten bonuksena j�tt�m� k�ytt�m� sprite
	char		ammus1_sprite[13];								// Spriten ammuksena 1 k�ytt�m� sprite
	char		ammus2_sprite[13];								// Spriten ammuksena 2 k�ytt�m� sprite

	bool		tiletarkistus;									// t�rm�ileek� tileihin
	DWORD		aani_frq;										// ��nien perussoittotaajuus (esim. 22050)
	bool		random_frq;										// satunnaisuutta taajuuteen?

	// Jos sprite on este
	bool		este_ylos;
	bool		este_alas;
	bool		este_oikealle;
	bool		este_vasemmalle;

};
class PK2Sprite_Prototyyppi13{
	public:
	// Versionumero

	int			tyyppi;											// spriten tyyppi
	// Kuvatiedoston tiedot
	char		kuvatiedosto[100];								// .BMP jossa ovat spriten grafiikat
	// Spriteen liittyv�t ��nitehosteet
	char		aanitiedostot[7][100];							// ��nitehostetiedostot
	int			aanet[7];										// ��nitehosteet (indeksit buffereihin)
	// Spriten kuva- ja animaatio-ominaisuudet
	BYTE		frameja;										// framejen m��r�
	PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
	BYTE		animaatioita;									// animaatioiden m��r�
	BYTE		frame_rate;										// yhden framen kesto
	int			kuva_x;											// miss� kohtaa kuvaa sprite on
	int			kuva_y;											// miss� kohtaa kuvaa sprite on
	int			kuva_frame_leveys;								// yhden framen leveys
	int			kuva_frame_korkeus;								// yhden framen korkeus
	int			kuva_frame_vali;								// kahden framen vali
	// Spriten ominaisuudet
	char		nimi[30];										// spriten nimi (n�kyy editorissa)
	int			leveys;											// spriten leveys
	int			korkeus;										// spriten korkeus
	double		paino;											// sprite paino (vaikuttaa hyppyyn ja kytkimiin)
	bool		vihollinen;										// onko sprite vihollinen
	int			energia;										// monta iskua kest��
	int			vahinko;										// paljon vahinkoa tekee jos osuu
	BYTE       vahinko_tyyppi;									// Mink� tyyppist� vahinkoa tekee (1.1)
	BYTE		suojaus;										// Mink� tyyppiselt� vahingolta on suojattu
	int			pisteet;										// paljonko siit� saa pisteit�
	int			AI[10];											// mit� teko�lyj� k�ytet��n
	BYTE		max_hyppy;										// hypyn maksimikesto
	double		max_nopeus;										// maksimi nopeus
	int			latausaika;										// ampumisen j�lkeinen odotus
	BYTE		vari;											// tehd��nk� spritest� jonkin tietyn v�rinen
	bool		este;											// k�ytt�ytyyk� sprite kuin sein�
	int			tuhoutuminen;									// miten sprite tuhoutuu
	bool		avain;											// Voiko sprite avata lukkoja
	bool		tarisee;										// Tariseek� sprite satunnaisesti
	BYTE       bonusten_lkm;									// Bonusten lukum��r�
	int         hyokkays1_aika;									// Hy�kk�ysanimaation 1 kesto (frameja)
	int         hyokkays2_aika;									// Hy�kk�ysanimaation 2 kesto (frameja)
	int			pallarx_kerroin;								// Vain TYYPPI_TAUSTA. Suhde taustakuvaan.
	// Yhteys toisiin spriteihin
	char		muutos_sprite[100];								// Toinen sprite joksi t�m� sprite voi muuttua
	char		bonus_sprite[100];								// Spriten bonuksena j�tt�m� k�ytt�m� sprite
	char		ammus1_sprite[100];								// Spriten ammuksena 1 k�ytt�m� sprite
	char		ammus2_sprite[100];								// Spriten ammuksena 2 k�ytt�m� sprite

	bool		tiletarkistus;									// t�rm�ileek� tileihin
	DWORD		aani_frq;										// ��nien perussoittotaajuus (esim. 22050)
	bool		random_frq;										// satunnaisuutta taajuuteen?

	// Jos sprite on este
	bool		este_ylos;
	bool		este_alas;
	bool		este_oikealle;
	bool		este_vasemmalle;

	BYTE		lapinakyvyys;									// 0 = ei n�y - 100 = ei l�pin�kyv�
	bool		hehkuu;											// hehkuuko (t�ytyy olla my�s l�pin�kyv�)
	int			tulitauko;										// ammuspriten ampujalle aiheuttama latausaika
	bool		liitokyky;										// voiko tippua hiljaa alas?
	bool		boss;											// onko johtaja
	bool		bonus_aina;										// j�tt�� aina bonuksen tuhoutuessa
	bool		osaa_uida;										// vaikuttaako painovoima vedess�?
};

// Varsinaiset "oikeat" luokat ----------------------------------------------------------------------

class PK2Sprite_Prototyyppi{
	public:

	// Versionumero
	char		versio[4];										// versionumero (latausta varten)
	char		tiedosto[255];									// spriten oma tiedosto (.spr)
	int			indeksi;										// monesko prototyyppi on taulukossa

	int			tyyppi;											// spriten tyyppi

	// Kuvatiedoston tiedot
	char		kuvatiedosto[100];								// .BMP jossa ovat spriten grafiikat

	// Spriteen liittyv�t ��nitehosteet

	char		aanitiedostot[MAX_AANIA][100];					// ��nitehostetiedostot
	int			aanet[MAX_AANIA];								// ��nitehosteet (indeksit buffereihin)

	// Spriten kuva- ja animaatio-ominaisuudet
	int			framet[SPRITE_MAX_FRAMEJA];						// spriten framet (bitm�pit)
	int			framet_peilikuva[SPRITE_MAX_FRAMEJA];			// spriten framet peilikuvina
	BYTE		frameja;										// framejen m��r�
	PK2SPRITE_ANIMAATIO animaatiot[SPRITE_MAX_ANIMAATIOITA];	// animaatio sekvenssit
	BYTE		animaatioita;									// animaatioiden m��r�
	BYTE		frame_rate;										// yhden framen kesto
	int			kuva_x;											// miss� kohtaa kuvaa sprite on
	int			kuva_y;											// miss� kohtaa kuvaa sprite on
	int			kuva_frame_leveys;								// yhden framen leveys
	int			kuva_frame_korkeus;								// yhden framen korkeus
	int			kuva_frame_vali;								// kahden framen vali

	// Spriten ominaisuudet
	char		nimi[30];										// spriten nimi (n�kyy editorissa)
	int			leveys;											// spriten leveys
	int			korkeus;										// spriten korkeus
	double		paino;											// sprite paino (vaikuttaa hyppyyn ja kytkimiin)

	bool		vihollinen;										// onko sprite vihollinen
	int			energia;										// monta iskua kest��
	int			vahinko;										// paljon vahinkoa tekee jos osuu
	BYTE       vahinko_tyyppi;									// mink� tyyppist� vahinkoa tekee (1.1)
	BYTE		suojaus;										// mink� tyyppiselt� vahingolta on suojattu (1.1)
	int			pisteet;										// paljonko siit� saa pisteit�

	int			AI[SPRITE_MAX_AI];								// mit� teko�lyj� k�ytet��n

	BYTE		max_hyppy;										// hypyn maksimikesto
	double		max_nopeus;										// maksiminopeus
	int			latausaika;										// ampumisen j�lkeinen odotus
	BYTE		vari;											// tehd��nk� spritest� jonkin tietyn v�rinen
	bool		este;											// k�ytt�ytyyk� sprite kuin sein�
	int			tuhoutuminen;									// miten sprite tuhoutuu
	bool		avain;											// Voiko sprite avata lukkoja
	bool		tarisee;										// T�riseek� sprite satunnaisesti
	BYTE       bonusten_lkm;									// Bonusten lukum��r�
	int         hyokkays1_aika;									// Hy�kk�ysanimaation 1 kesto (frameja)
	int         hyokkays2_aika;									// Hy�kk�ysanimaation 2 kesto (frameja)

	int			pallarx_kerroin;								// Vain TYYPPI_TAUSTA. Suhde taustakuvaan.

	// Yhteys toisiin spriteihin

	char		muutos_sprite[100];								// Toinen sprite joksi t�m� sprite voi muuttua
	char		bonus_sprite[100];								// Spriten bonuksena j�tt�m� k�ytt�m� sprite
	char		ammus1_sprite[100];								// Spriten ammuksena 1 k�ytt�m� sprite
	char		ammus2_sprite[100];								// Spriten ammuksena 2 k�ytt�m� sprite
	int			muutos;											// Muutosspriten prototyypin indeksi. -1 jos ei ole
	int			bonus;											// Bonusspriten prototyypin indeksi. -1 jos ei ole
	int			ammus1;											// Ammussprite1 prototyypin indeksi. -1 jos ei ole
	int			ammus2;											// Ammussprite1 prototyypin indeksi. -1 jos ei ole

	// Lis�ykset 1.2 versiossa
	bool		tiletarkistus;									// t�rm�ileek� tileihin
	DWORD		aani_frq;										// ��nien perussoittotaajuus (esim. 22050)
	bool		random_frq;										// satunnaisuutta taajuuteen?

	// Jos sprite on este
	bool		este_ylos;
	bool		este_alas;
	bool		este_oikealle;
	bool		este_vasemmalle;

	// Lis�ykset 1.3 versiossa
	BYTE		lapinakyvyys;									// 0 = ei n�y - 100 = ei l�pin�kyv�
	bool		hehkuu;											// 0 = ei hehku (t�ytyy olla l�pin�kyv�)
	int			tulitauko;										// ammuspriten ampujalle aiheuttama latausaika
	bool		liitokyky;										// voiko tippua hiljaa alas?
	bool		boss;											// onko johtaja
	bool		bonus_aina;										// j�tt�� aina bonuksen tuhoutuessa
	bool		osaa_uida;										// vaikuttaako painovoima vedess�?

	// Muodostimet
	PK2Sprite_Prototyyppi();
	~PK2Sprite_Prototyyppi();

	// Metodit
	void Uusi();
	void Kopioi(const PK2Sprite_Prototyyppi &proto);
	int  Animaatio_Uusi(int anim_i, BYTE *sekvenssi, bool looppi);
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

class PK2Sprite{
	public:

	bool		aktiivinen;			// true / false
	int			pelaaja;			// 0 = ei pelaaja, 1 = pelaaja
	PK2Sprite_Prototyyppi *tyyppi;	// osoitin spriten prototyyppiin
	bool		piilota;			// true = ei toiminnassa, false = toiminnassa
	double		alku_x;				// spriten alkuper�inen x sijainti
	double		alku_y;				// spriten alkuper�inen y sijainti
	double		x;					// x-kordinaatti pelikent�ll�
	double		y;					// y-kordinaatti pelikent�ll�
	double		a;					// liike sivusuunnassa
	double		b;					// liike pystysuunnassa
	bool		flip_x;				// true = peilikuva sivusuunnassa
	bool		flip_y;				// true = peilikuva pystysuunnassa
	int			hyppy_ajastin;		// hypyn kesto: 0 = ei hypyss�, > 0 = hypyss�, < 0 = tullut alas
	bool		ylos;				// voiko sprite juuri nyt liikkua yl�s
	bool		alas;				// voiko sprite juuri nyt liikkua ....
	bool		oikealle;			// voiko sprite juuri nyt liikkua ....
	bool		vasemmalle;			// voiko sprite juuri nyt liikkua ....
	bool		reuna_vasemmalla;	// onko spriten vasemmalla puolella kuoppa?
	bool		reuna_oikealla;		// onko spriten vasemmalla puolella kuoppa?
	int			energia;			// monta osumaa sprite viel� kest��
	int			emosprite;			// jos spriten on luonut jokin toinen sprite
	double		paino;				// spriten oma yksil�llinen paino
	double		kytkinpaino;		// spriten paino + muiden spritejen panot, joihin kosketaan
	bool		kyykky;				// onko sprite kyykyss�
	int			isku;				// onko sprite saanut vahinkoa
	int			lataus;				// jos on ammuttu, odotetaan
	int			hyokkays1;			// ajastin joka laskee hy�kk�ys 1:n j�lkeen
	int			hyokkays2;			// ajastin joka laskee hy�kk�ys 2:n j�lkeen
	bool		vedessa;			// onko sprite vedess�
	bool		piilossa;			// onko sprite piilossa
	double      alkupaino;			// spriten alkuper�inen paino - sama kuin prototyypin
	int			saatu_vahinko;		// v�hennet��n energiasta jos erisuuri kuin 0
	BYTE       saatu_vahinko_tyyppi; // saadun vahingon tyyppi (esim. lumi).
	bool		vihollinen;			// prototyypist� saatu tieto, onko vihollinen
	int			ammus1;				// spriten k�ytt�m�n ammus-prototyypin indeksi
	int			ammus2;				//

	int			pelaaja_x,			// tieto siit�, miss� pelaaja on n�hty viimeksi
				pelaaja_y;

	int			ajastin;			// ajastin jonka arvo py�rii v�lill� 1 - 32000

	BYTE		animaatio_index;	// t�m�nhetkinen py�riv� animaatio
	BYTE		sekvenssi_index;	// t�m�nhetkisen animaation frame
	BYTE		frame_aika;			// kuinka kauan frame on n�kynyt
	int			muutos_ajastin;		// sprite muuttuu muutosspriteksi kun t�m� nollautuu

	PK2Sprite();
	PK2Sprite(PK2Sprite_Prototyyppi *tyyppi, int pelaaja, bool piilota, double x, double y);
	~PK2Sprite();
	int Piirra(int kamera_x, int kamera_y);		// Animoi ja piirt�� spriten
	int Animaatio(int anim_i, bool nollaus);	// Vaihtaa spriten animaation
	int Animoi();								// Animoi muttei piirr� sprite�
	bool Onko_AI(int AI);						// Palauttaa true, jos spritell� on ko. AI
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
	bool AI_Info(PK2Sprite &pelaaja);
	int AI_Tuhoutuu_Jos_Emo_Tuhoutuu(PK2Sprite *spritet);

	int Animaatio_Perus();
	int Animaatio_Kana();
	int Animaatio_Bonus();
	int Animaatio_Muna();
	int Animaatio_Ammus();
};

#endif
