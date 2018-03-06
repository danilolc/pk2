//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################

#ifndef PK2SPRITE
#define PK2SPRITE

#include "types.hpp"

#define PK2SPRITE_VIIMEISIN_VERSIO "1.3"

#define MAX_SPRITEJA       800
#define MAX_PROTOTYYPPEJA  100

#define SPRITE_MAX_FRAMEJA        50
#define SPRITE_MAX_ANIMAATIOITA   20
#define SPRITE_MAX_AI             10
#define ANIMAATIO_MAX_SEKVENSSEJA 10
#define MAX_AANIA                  7
#define	VAHINKO_AIKA              50 //Damage time?

//Spite file values
enum { //Animation
    ANIMAATIO_PAIKALLA,
    ANIMAATIO_KAVELY,
    ANIMAATIO_HYPPY_YLOS,
    ANIMAATIO_HYPPY_ALAS,
    ANIMAATIO_KYYKKY,
    ANIMAATIO_VAHINKO,
    ANIMAATIO_KUOLEMA,
    ANIMAATIO_HYOKKAYS1,
    ANIMAATIO_HYOKKAYS2
};
enum { //Sound
    AANI_VAHINKO,
    AANI_TUHOUTUMINEN,
    AANI_HYOKKAYS1,
    AANI_HYOKKAYS2,
    AANI_RANDOM,
    AANI_ERIKOIS1,
    AANI_ERIKOIS2
};
enum { //AI
    AI_EI,
    AI_KANA,
    AI_MUNA,
    AI_PIKKUKANA,
    AI_BONUS,
    AI_HYPPIJA,
    AI_PERUS,
    AI_KAANTYY_ESTEESTA_HORI,
    AI_VAROO_KUOPPAA,
    AI_RANDOM_SUUNNANVAIHTO_HORI,
    AI_RANDOM_HYPPY,
    AI_SEURAA_PELAAJAA,
    AI_RANDOM_ALOITUSSUUNTA_HORI,
    AI_SEURAA_PELAAJAA_JOS_NAKEE,
    AI_MUUTOS_JOS_ENERGIAA_ALLE_2,
    AI_MUUTOS_JOS_ENERGIAA_YLI_1,
    AI_ALOITUSSUUNTA_PELAAJAA_KOHTI,
    AI_AMMUS,
    AI_NONSTOP,
    AI_HYOKKAYS_1_JOS_OSUTTU,
    AI_POMMI,
    AI_HYOKKAYS_1_JOS_PELAAJA_EDESSA,
    AI_HYOKKAYS_1_JOS_PELAAJA_ALAPUOLELLA,
    AI_VAHINGOITTUU_VEDESTA,
    AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA,
    AI_TAPA_KAIKKI,
    AI_KITKA_VAIKUTTAA,
    AI_PIILOUTUU,
    AI_PALAA_ALKUUN_X,
    AI_PALAA_ALKUUN_Y,
    AI_TELEPORTTI,

    AI_HEITTOASE = 35,
    AI_TIPPUU_TARINASTA,
    AI_VAIHDA_KALLOT_JOS_TYRMATTY,
    AI_VAIHDA_KALLOT_JOS_OSUTTU,
    AI_TUHOUTUU_JOS_EMO_TUHOUTUU,

    AI_LIIKKUU_X_COS = 41,
    AI_LIIKKUU_Y_COS,
    AI_LIIKKUU_X_SIN,
    AI_LIIKKUU_Y_SIN,
    AI_LIIKKUU_X_COS_NOPEA,
    AI_LIIKKUU_Y_SIN_NOPEA,
    AI_LIIKKUU_X_COS_HIDAS,
    AI_LIIKKUU_Y_SIN_HIDAS,
    AI_LIIKKUU_Y_SIN_VAPAA,

    AI_RANDOM_KAANTYMINEN,
    AI_HYPPY_JOS_PELAAJA_YLAPUOLELLA,
    AI_MUUTOS_AJASTIN,

    AI_TIPPUU_JOS_KYTKIN1_PAINETTU,
    AI_TIPPUU_JOS_KYTKIN2_PAINETTU,
    AI_TIPPUU_JOS_KYTKIN3_PAINETTU,

    AI_LIIKKUU_ALAS_JOS_KYTKIN1_PAINETTU,
    AI_LIIKKUU_YLOS_JOS_KYTKIN1_PAINETTU,
    AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN1_PAINETTU,
    AI_LIIKKUU_OIKEALLE_JOS_KYTKIN1_PAINETTU,
    AI_LIIKKUU_ALAS_JOS_KYTKIN2_PAINETTU,
    AI_LIIKKUU_YLOS_JOS_KYTKIN2_PAINETTU,
    AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN2_PAINETTU,
    AI_LIIKKUU_OIKEALLE_JOS_KYTKIN2_PAINETTU,
    AI_LIIKKUU_ALAS_JOS_KYTKIN3_PAINETTU,
    AI_LIIKKUU_YLOS_JOS_KYTKIN3_PAINETTU,
    AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN3_PAINETTU,
    AI_LIIKKUU_OIKEALLE_JOS_KYTKIN3_PAINETTU,

    AI_KAANTYY_ESTEESTA_VERT = 70,
    AI_RANDOM_ALOITUSSUUNTA_VERT,
    AI_ALOITUSSUUNTA_PELAAJAA_KOHTI_VERT,
    AI_KIIPEILIJA,
    AI_KIIPEILIJA2,
    AI_PAKENEE_PELAAJAA_JOS_NAKEE,
    AI_UUSI_JOS_TUHOUTUU,

    AI_NUOLI_VASEMMALLE,
    AI_NUOLI_OIKEALLE,
    AI_NUOLI_YLOS,
    AI_NUOLI_ALAS,
    AI_NUOLET_VAIKUTTAVAT,

    AI_TAUSTA_KUU = 101,
    AI_TAUSTA_LIIKKUU_VASEMMALLE,
    AI_TAUSTA_LIIKKUU_OIKEALLE,

    AI_BONUS_AIKA = 120,
    AI_BONUS_NAKYMATTOMYYS,
    AI_BONUS_SUPERHYPPY,
    AI_BONUS_SUPERTULITUS,
    AI_BONUS_SUPERVAUHTI,

    AI_MUUTOS_JOS_OSUTTU = 129,
    AI_SEURAA_PELAAJAA_VERT_HORI,
    AI_SEURAA_PELAAJAA_JOS_NAKEE_VERT_HORI,
    AI_RANDOM_LIIKAHDUS_VERT_HORI,
    AI_SAMMAKKO1,
    AI_SAMMAKKO2,
    AI_SAMMAKKO3,
    AI_HYOKKAYS_2_JOS_OSUTTU,
    AI_HYOKKAYS_1_NONSTOP,
    AI_HYOKKAYS_2_NONSTOP,
    AI_KAANTYY_JOS_OSUTTU,
    AI_EVIL_ONE,

    AI_INFO1 = 201,
    AI_INFO2,
    AI_INFO3,
    AI_INFO4,
    AI_INFO5,
    AI_INFO6,
    AI_INFO7,
    AI_INFO8,
    AI_INFO9,
    AI_INFO10,
    AI_INFO11,
    AI_INFO12,
    AI_INFO13,
    AI_INFO14,
    AI_INFO15,
    AI_INFO16,
    AI_INFO17,
    AI_INFO18,
    AI_INFO19
};
enum { //Damage
    VAHINKO_EI,
    VAHINKO_ISKU,
    VAHINKO_PUDOTUS,
    VAHINKO_MELU,
    VAHINKO_TULI,
    VAHINKO_VESI,
    VAHINKO_LUMI,
    VAHINKO_BONUS,
    VAHINKO_SAHKO,
    VAHINKO_ITSARI,
    VAHINKO_PURISTUS,
    VAHINKO_HAJU,
    VAHINKO_KAIKKI,
    VAHINKO_PISTO
};
enum { //Prototype
    PROTOTYYPPI_KANA,
    PROTOTYYPPI_MUNA,
    PROTOTYYPPI_PIKKUKANA,
    PROTOTYYPPI_OMENA,
    PROTOTYYPPI_HYPPIJA
};
enum { //Type
    TYYPPI_EI_MIKAAN,
    TYYPPI_PELIHAHMO,
    TYYPPI_BONUS,
    TYYPPI_AMMUS,
    TYYPPI_TELEPORTTI,
    TYYPPI_TAUSTA
};
enum { //Color
    VARI_HARMAA   = 0,
    VARI_SININEN  = 32,
    VARI_PUNAINEN = 64,
    VARI_VIHREA   = 96,
    VARI_ORANSSI  = 128,
    VARI_VIOLETTI = 160,
    VARI_TURKOOSI = 192,
    VARI_NORMAALI = 255
};
enum { //Destruction
    TUHOUTUMINEN_EI_TUHOUDU,
    TUHOUTUMINEN_HOYHENET,
    TUHOUTUMINEN_TAHDET_HARMAA,
    TUHOUTUMINEN_TAHDET_SININEN,
    TUHOUTUMINEN_TAHDET_PUNAINEN,
    TUHOUTUMINEN_TAHDET_VIHREA,
    TUHOUTUMINEN_TAHDET_ORANSSI,
    TUHOUTUMINEN_TAHDET_VIOLETTI,
    TUHOUTUMINEN_TAHDET_TURKOOSI,
    TUHOUTUMINEN_RAJAHDYS_HARMAA,
    TUHOUTUMINEN_RAJAHDYS_SININEN,
    TUHOUTUMINEN_RAJAHDYS_PUNAINEN,
    TUHOUTUMINEN_RAJAHDYS_VIHREA,
    TUHOUTUMINEN_RAJAHDYS_ORANSSI,
    TUHOUTUMINEN_RAJAHDYS_VIOLETTI,
    TUHOUTUMINEN_RAJAHDYS_TURKOOSI,
    TUHOUTUMINEN_SAVU_HARMAA,
    TUHOUTUMINEN_SAVU_SININEN,
    TUHOUTUMINEN_SAVU_PUNAINEN,
    TUHOUTUMINEN_SAVU_VIHREA,
    TUHOUTUMINEN_SAVU_ORANSSI,
    TUHOUTUMINEN_SAVU_VIOLETTI,
    TUHOUTUMINEN_SAVU_TURKOOSI,
    TUHOUTUMINEN_SAVUPILVET,
    TUHOUTUMINEN_ANIMAATIO = 100
};
enum { //Bonus Item
    BONUSESINE_EI_MITAAN,
    BONUSESINE_AVAIN,
    BONUSESINE_PISTE
};

struct PK2SPRITE_ANIMAATIO{
    BYTE		sekvenssi[ANIMAATIO_MAX_SEKVENSSEJA];	// sequence
    BYTE		frameja;								// frames
    bool		looppi;									// loop
};

//.spr file structures
struct PK2Sprite_Prototyyppi10{

    DWORD		tyyppi;											// spriten tyyppi
    // Kuvatiedoston tiedot
    char		kuvatiedosto[13];								// .BMP jossa ovat spriten grafiikat
    // Spriteen liittyv�t ��nitehosteet
    char		aanitiedostot[7][13];							// ��nitehostetiedostot
    DWORD		aanet[7];										// ��nitehosteet (indeksit buffereihin)
    // Spriten kuva- ja animaatio-ominaisuudet
    BYTE		frameja;										// framejen m��r�
    PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
    BYTE		animaatioita;									// animaatioiden m��r�
    BYTE		frame_rate;										// yhden framen kesto
    DWORD		kuva_x;											// miss� kohtaa kuvaa sprite on
    DWORD		kuva_y;											// miss� kohtaa kuvaa sprite on
    DWORD		kuva_frame_leveys;								// yhden framen leveys
    DWORD		kuva_frame_korkeus;								// yhden framen korkeus
    DWORD		kuva_frame_vali;								// kahden framen vali
    // Spriten ominaisuudet
    char		nimi[30];										// spriten nimi (n�kyy editorissa)
    DWORD		leveys;											// spriten leveys
    DWORD		korkeus;										// spriten korkeus
    double		paino;											// sprite paino (vaikuttaa hyppyyn ja kytkimiin)
    bool		vihollinen;										// onko sprite vihollinen
    DWORD		energia;										// monta iskua kest��
    DWORD		vahinko;										// paljon vahinkoa tekee jos osuu
    DWORD		pisteet;										// paljonko siit� saa pisteit�
    DWORD		AI[5];											// mit� teko�lyj� k�ytet��n
    DWORD		suojaus;										// miten suojattu iskuilta
    BYTE		max_hyppy;										// hypyn maksimikesto
    BYTE		max_nopeus;										// maksimi nopeus
    DWORD		latausaika;										// ampumisen j�lkeinen odotus
    BYTE		vari;											// tehd��nk� spritest� jonkin tietyn v�rinen
    bool		este;											// k�ytt�ytyyk� sprite kuin sein�
    DWORD		tuhoutuminen;									// miten sprite tuhoutuu
    // Yhteys toisiin spriteihin
    char		muutos_sprite[13];								// Toinen sprite joksi t�m� sprite voi muuttua
    char		bonus_sprite[13];								// Spriten bonuksena j�tt�m� k�ytt�m� sprite
    char		ammus1_sprite[13];								// Spriten ammuksena 1 k�ytt�m� sprite
    char		ammus2_sprite[13];								// Spriten ammuksena 2 k�ytt�m� sprite
    bool		avain;											// Voiko sprite avata lukkoja
};
struct PK2Sprite_Prototyyppi11{

    DWORD		tyyppi;											// spriten tyyppi
    // Kuvatiedoston tiedot
    char		kuvatiedosto[13];								// .BMP jossa ovat spriten grafiikat
    // Spriteen liittyv�t ��nitehosteet
    char		aanitiedostot[7][13];							// ��nitehostetiedostot
    DWORD		aanet[7];										// ��nitehosteet (indeksit buffereihin)
    // Spriten kuva- ja animaatio-ominaisuudet
    BYTE		frameja;										// framejen m��r�
    PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
    BYTE		animaatioita;									// animaatioiden m��r�
    BYTE		frame_rate;										// yhden framen kesto
    DWORD		kuva_x;											// miss� kohtaa kuvaa sprite on
    DWORD		kuva_y;											// miss� kohtaa kuvaa sprite on
    DWORD		kuva_frame_leveys;								// yhden framen leveys
    DWORD		kuva_frame_korkeus;								// yhden framen korkeus
    DWORD		kuva_frame_vali;								// kahden framen vali
    // Spriten ominaisuudet
    char		nimi[30];										// spriten nimi (n�kyy editorissa)
    DWORD		leveys;											// spriten leveys
    DWORD		korkeus;										// spriten korkeus
    double		paino;											// sprite paino (vaikuttaa hyppyyn ja kytkimiin)
    bool		vihollinen;										// onko sprite vihollinen
    DWORD		energia;										// monta iskua kest��
    DWORD		vahinko;										// paljon vahinkoa tekee jos osuu
    BYTE        vahinko_tyyppi;									// Mink� tyyppist� vahinkoa tekee (1.1)
    BYTE		suojaus;										// Mink� tyyppiselt� vahingolta on suojattu
    DWORD		pisteet;										// paljonko siit� saa pisteit�
    DWORD		AI[5];											// mit� teko�lyj� k�ytet��n
    BYTE		max_hyppy;										// hypyn maksimikesto
    BYTE		max_nopeus;										// maksimi nopeus
    DWORD		latausaika;										// ampumisen j�lkeinen odotus
    BYTE		vari;											// tehd��nk� spritest� jonkin tietyn v�rinen
    bool		este;											// k�ytt�ytyyk� sprite kuin sein�
    DWORD		tuhoutuminen;									// miten sprite tuhoutuu
    bool		avain;											// Voiko sprite avata lukkoja
    bool		tarisee;										// Tariseek� sprite satunnaisesti
    BYTE        bonusten_lkm;									// Bonusten lukum��r�
    DWORD       hyokkays1_aika;									// Hy�kk�ysanimaation 1 kesto (frameja)
    DWORD       hyokkays2_aika;									// Hy�kk�ysanimaation 2 kesto (frameja)
    DWORD		pallarx_kerroin;								// Vain TYYPPI_TAUSTA. Suhde taustakuvaan.
    // Yhteys toisiin spriteihin
    char		muutos_sprite[13];								// Toinen sprite joksi t�m� sprite voi muuttua
    char		bonus_sprite[13];								// Spriten bonuksena j�tt�m� k�ytt�m� sprite
    char		ammus1_sprite[13];								// Spriten ammuksena 1 k�ytt�m� sprite
    char		ammus2_sprite[13];								// Spriten ammuksena 2 k�ytt�m� sprite

};
struct PK2Sprite_Prototyyppi12{

    DWORD		tyyppi;											// spriten tyyppi
    // Kuvatiedoston tiedot
    char		kuvatiedosto[13];								// .BMP jossa ovat spriten grafiikat
    // Spriteen liittyv�t ��nitehosteet
    char		aanitiedostot[7][13];							// ��nitehostetiedostot
    DWORD		aanet[7];										// ��nitehosteet (indeksit buffereihin)
    // Spriten kuva- ja animaatio-ominaisuudet
    BYTE		frameja;										// framejen m��r�
    PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
    BYTE		animaatioita;									// animaatioiden m��r�
    BYTE		frame_rate;										// yhden framen kesto
    DWORD		kuva_x;											// miss� kohtaa kuvaa sprite on
    DWORD		kuva_y;											// miss� kohtaa kuvaa sprite on
    DWORD		kuva_frame_leveys;								// yhden framen leveys
    DWORD		kuva_frame_korkeus;								// yhden framen korkeus
    DWORD		kuva_frame_vali;								// kahden framen vali
    // Spriten ominaisuudet
    char		nimi[30];										// spriten nimi (n�kyy editorissa)
    DWORD		leveys;											// spriten leveys
    DWORD		korkeus;										// spriten korkeus
    double		paino;											// sprite paino (vaikuttaa hyppyyn ja kytkimiin)
    bool		vihollinen;										// onko sprite vihollinen
    DWORD		energia;										// monta iskua kest��
    DWORD		vahinko;										// paljon vahinkoa tekee jos osuu
    BYTE        vahinko_tyyppi;									// Mink� tyyppist� vahinkoa tekee (1.1)
    BYTE		suojaus;										// Mink� tyyppiselt� vahingolta on suojattu
    DWORD		pisteet;										// paljonko siit� saa pisteit�
    DWORD		AI[5];											// mit� teko�lyj� k�ytet��n
    BYTE		max_hyppy;										// hypyn maksimikesto
    BYTE		max_nopeus;										// maksimi nopeus
    DWORD		latausaika;										// ampumisen j�lkeinen odotus
    BYTE		vari;											// tehd��nk� spritest� jonkin tietyn v�rinen
    bool		este;											// k�ytt�ytyyk� sprite kuin sein�
    DWORD		tuhoutuminen;									// miten sprite tuhoutuu
    bool		avain;											// Voiko sprite avata lukkoja
    bool		tarisee;										// Tariseek� sprite satunnaisesti
    BYTE        bonusten_lkm;									// Bonusten lukum��r�
    DWORD       hyokkays1_aika;									// Hy�kk�ysanimaation 1 kesto (frameja)
    DWORD       hyokkays2_aika;									// Hy�kk�ysanimaation 2 kesto (frameja)
    DWORD		pallarx_kerroin;								// Vain TYYPPI_TAUSTA. Suhde taustakuvaan.
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
struct PK2Sprite_Prototyyppi13{

    DWORD		tyyppi;											// sprite type
    char		kuvatiedosto[100];								// bmp path
    char		aanitiedostot[7][100];							// sound path (max 7)
    DWORD		aanet[7];										// sound types


    BYTE		frameja;										// number of frames
    PK2SPRITE_ANIMAATIO animaatiot[20];							// animation sequences
    BYTE		animaatioita;									// number of animations
    BYTE		frame_rate;										// frame rate
    DWORD		kuva_x;											// x position of first frame
    DWORD		kuva_y;											// y position of first frame
    DWORD		kuva_frame_leveys;								// frame width
    DWORD		kuva_frame_korkeus;								// frame height
    DWORD		kuva_frame_vali;								// space between frames


    char		nimi[30];										// name
    DWORD		leveys;											// width
    DWORD		korkeus;										// height
    double		paino;											// weight (for jump and switches)
    bool		vihollinen;										// if sprite is a enemy
    DWORD		energia;										//?sprite energy
    DWORD		vahinko;										//?damage if hitted
    BYTE        vahinko_tyyppi;									//?damage type
    BYTE		suojaus;										//?protection type
    DWORD		pisteet;										// how much score
    DWORD		AI[10];											// AI type (max 10)
    BYTE		max_hyppy;										// max jump time
    double		max_nopeus;										// max speed
    DWORD		latausaika;										//?wait post shoot
    BYTE		vari;											// color
    bool		este;											// is a wall
    DWORD		tuhoutuminen;									// how sprite is destroyed
    bool		avain;											// can sprite open locks
    bool		tarisee;										//?sprite randomly
    BYTE        bonusten_lkm;									// number of bonuses
    DWORD       hyokkays1_aika;									// attack 1 duration (frames)
    DWORD       hyokkays2_aika;									// attack 2 duration (frames)
    DWORD		pallarx_kerroin;								// parallax type (just to TYYPPI_TAUSTA)


    char		muutos_sprite[100];								// another sprite that this sprite may change
    char		bonus_sprite[100];								// bonus that this sprite gives
    char		ammus1_sprite[100];								// ammo 1 sprite
    char		ammus2_sprite[100];								// ammo 2 sprite


    bool		tiletarkistus;									//?make sounds?
    DWORD		aani_frq;										// sound frequency (def. 22050)
    bool		random_frq;										// use random frequency?


    bool		este_ylos;                                      // if is wall at up
    bool		este_alas;                                      // if is wall at down
    bool		este_oikealle;                                  // if is wall at right
    bool		este_vasemmalle;                                // if is wall at left


    BYTE		lapinakyvyys;									// transparency
    bool		hehkuu;											// is transparent?
    DWORD		tulitauko;										//*ammuspriten ampujalle aiheuttama latausaika
    bool		liitokyky;										//*voiko tippua hiljaa alas?
    bool		boss;											// if it is a boss
    bool		bonus_aina;										// allways gives bonus
    bool		osaa_uida;										// be alive in water
};

//Classes used in game
class PK2Sprite_Prototyyppi{
    public:

    //Version
    char		versio[4];
    //.spr filename
    char		tiedosto[255];
    //Prototype index
    int			indeksi;
    //Sprite type
    int			tyyppi;

    //.bmp filename
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

    // Methods
    void Uusi();
    void Kopioi(const PK2Sprite_Prototyyppi &proto);
    int  Animaatio_Uusi(int anim_i, BYTE *sekvenssi, bool looppi);
    int  Lataa(char *polku, char *tiedoston_nimi);
    void Tallenna(char *tiedoston_nimi);
    int  Piirra(int x, int y, int frame);
    bool Onko_AI(int AI);

    void SetProto10(PK2Sprite_Prototyyppi10 &proto);
    void SetProto11(PK2Sprite_Prototyyppi11 &proto);
    void SetProto12(PK2Sprite_Prototyyppi12 &proto);
    void SetProto13(PK2Sprite_Prototyyppi13 &proto);
    PK2Sprite_Prototyyppi13 GetProto13();
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
    double		a;					// horizontal speed
    double		b;					// vertical speed
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

    //AI_Functions
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
