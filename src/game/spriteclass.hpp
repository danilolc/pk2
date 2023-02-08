//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PFile.hpp"
#include "engine/types.hpp"

#define PK2SPRITE_CURRENT_VERSION "1.3"

#define MAX_SPRITEJA       850
#define MAX_PROTOTYYPPEJA  100

#define SPRITE_MAX_FRAMEJA      50
#define SPRITE_MAX_ANIMAATIOITA 20
#define SPRITE_MAX_AI           10
#define ANIMATION_SEQUENCE_SIZE 10
#define SPRITE_MAX_SOUNDS        7
#define	DAMAGE_TIME             50

//Spite file values

enum {

    ANIMATION_IDLE,
    ANIMATION_WALKING,
    ANIMATION_JUMP_UP,
    ANIMATION_HYPPY_DOWN,
    ANIMATION_SQUAT,
    ANIMATION_DAMAGE,
    ANIMATION_DEATH,
    ANIMATION_ATTACK1,
    ANIMATION_ATTACK2

};

enum {

    SOUND_DAMAGE,
    SOUND_DESTRUCTION,
    SOUND_ATTACK1,
    SOUND_ATTACK2,
    SOUND_RANDOM,
    SOUND_SPECIAL1,
    SOUND_SPECIAL2

};

enum {
/*

RANDOM_HORIZONTAL_CHANGE_OF_DIRECTION
RANDOM_JUMPING
FOLLOW_THE_PLAYER
RANDOM_HORIZONTAL_CHANGE_OF_DIRECTION
FOLLOW_THE_PLAYER_IF_PLAYER_IS_IN_FRONT
TRANSFORMATION_IF_ENERGY_BELOW_2_(P)
TRANSFORMATION_IF_ENERGY_ABOVE_1_(P)
START_DIRECTIONS_TOWARDS_PLAYER
AMMONITION
NON_STOP
ATTACK_1_IF_DAMAGED
SELF_DESTRUCTION
ATTACK_1_IF_PLAYER_IS_IN_FRONT
ATTACK_1_IF_PLAYER_IS_BELOW
DAMAGED_BY_WATER_(P)
ATTACK_2_IF_PLAYER_IS_IN_FRONT
KILL_'EM_ALL
AFFECTED_BY_FRICTION
HIDE
RETURN_TO_START_POSITION_X
RETURN_TO_START_POSITION_Y
TELEPORT
THROWABLE_WEAPON
FALLS_WHEN_SHAKEN_(B)
CHANGE_TRAP_STONES_IF_KO'ED
CHANGE_TRAP_STONES_IF_DAMAGED
SELF_DESTRUCTS_WITH_MOTHER_SPRITE
MOVES_X_COS
MOVES_Y_COS
MOVES_X_SIN
MOVES_Y_SIN
MOVES_X_COS_FAST
MOVES_Y_SIN_FAST
MOVES_X_COS_SLOW
MOVES_Y_SIN_SLOW
MOVES_Y_SIN_FREE
RANDOM_TURNING
JUMP_IF_PLAYER_IS_ABOVE
TRANSFORMATION_TIMER_(B)
FALLS_IF_SWITCH_1_IS_PRESSED_(B)
FALLS_IF_SWITCH_2_IS_PRESSED_(B)
FALLS_IF_SWITCH_3_IS_PRESSED_(B)
MOVES_DOWN_IF_SWITCH_1_IS_PRESSED
MOVES_UP_IF_SWITCH_1_IS_PRESSED
MOVES_RIGHT_IF_SWITCH_1_IS_PRESSED
MOVES_LEFT_IF_SWITCH_1_IS_PRESSED
MOVES_DOWN_IF_SWITCH_2_IS_PRESSED
MOVES_UP_IF_SWITCH_2_IS_PRESSED
MOVES_RIGHT_IF_SWITCH_2_IS_PRESSED
MOVES_LEFT_IF_SWITCH_2_IS_PRESSED
MOVES_DOWN_IF_SWITCH_3_IS_PRESSED
MOVES_UP_IF_SWITCH_3_IS_PRESSED
MOVES_RIGHT_IF_SWITCH_3_IS_PRESSED
MOVES_LEFT_IF_SWITCH_3_IS_PRESSED
TURNS_VERTICALLY_FROM_OBSTACLE
RANDOM_VERTICAL_STARTING_DIRECTION
STARTING_DIRECTION_TOWARDS_PLAYER
CLIMBER
CLIMBER_TYPE_2
RUNS_AWAY_FROM_PLAYER_IF_SEES_PLAYER
REBORN_(B)
ARROW_LEFT
ARROW_RIGHT
ARROW_UP
ARROW_DOWN
MOVE_TO_ARROWS_DIRECTION
BACKGROUND_SPRITE_MOON
BACKGROUND_SPRITE_MOVES_TO_LEFT
BACKGROUND_SPRITE_MOVES_TO_RIGHT
ADD_TIME_TO_CLOCK
MAKE_PLAYER_INVISIBLE
FOLLOW_THE_PLAYER_VERTIC._AND_HORIZ.
FOLLOW_THE_PLAYER_VERTIC._AND_HORIZ.,_IF_PLAYER_IS_IN_FRONT
RANDOM_MOVE_VERTIC._AND_HORIZ.*/

    AI_NONE,
    AI_KANA,
    AI_EGG,
    AI_LITTLE_CHICKEN,
    AI_BONUS,
    AI_JUMPER, //?
    AI_BASIC,
    AI_TURNING_HORIZONTALLY,
    AI_LOOK_FOR_CLIFFS,
    AI_RANDOM_CHANGE_DIRECTION_H,
    AI_RANDOM_JUMP,
    AI_FOLLOW_PLAYER,
    AI_RANDOM_START_DIRECTION,
    AI_FOLLOW_PLAYER_IF_IN_FRONT,
    AI_CHANGE_WHEN_ENERGY_UNDER_2,
    AI_CHANGE_WHEN_ENERGY_OVER_1,
    AI_START_DIRECTIONS_TOWARDS_PLAYER,
    AI_AMMUS,
    AI_NONSTOP,
    AI_ATTACK_1_JOS_OSUTTU,
    AI_POMMI,
    AI_ATTACK_1_IF_PLAYER_IN_FRONT,
    AI_ATTACK_1_IF_PLAYER_BELLOW,
    AI_DAMAGED_BY_WATER,
    AI_ATTACK_2_IF_PLAYER_IN_FRONT,
    AI_KILL_EVERYONE,
    AI_KITKA_VAIKUTTAA,
    AI_PIILOUTUU,
    AI_PALAA_ALKUUN_X,
    AI_PALAA_ALKUUN_Y,
    AI_TELEPORT,

    // AI_31 - AI_34

    AI_HEITTOASE = 35,
    AI_TIPPUU_TARINASTA,
    AI_VAIHDA_KALLOT_JOS_TYRMATTY,
    AI_VAIHDA_KALLOT_JOS_OSUTTU,
    AI_TUHOUTUU_JOS_EMO_TUHOUTUU,

    // AI_40

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

    // AI_68, AI_69

    AI_KAANTYY_ESTEESTA_VERT = 70,
    AI_RANDOM_ALOITUSSUUNTA_VERT,
    AI_START_DIRECTIONS_TOWARDS_PLAYER_VERT,
    AI_KIIPEILIJA,
    AI_KIIPEILIJA2,
    AI_PAKENEE_PELAAJAA_JOS_NAKEE,
    AI_UUSI_JOS_TUHOUTUU,

    AI_NUOLI_VASEMMALLE,
    AI_NUOLI_OIKEALLE,
    AI_NUOLI_YLOS,
    AI_NUOLI_ALAS,
    AI_NUOLET_VAIKUTTAVAT,

    // AI_82 - AI_100

    AI_TAUSTA_KUU = 101,
    AI_TAUSTA_LIIKKUU_VASEMMALLE,
    AI_TAUSTA_LIIKKUU_OIKEALLE,

    // AI_104 - AI_119

    AI_BONUS_AIKA = 120,
    AI_BONUS_NAKYMATTOMYYS,
    AI_BONUS_SUPERHYPPY,
    AI_BONUS_SUPERTULITUS,
    AI_BONUS_SUPERVAUHTI,

    // New AI
    AI_BONUS_SUPERMODE,

    // AI_126 - AI_128

    AI_MUUTOS_JOS_OSUTTU = 129,
    AI_FOLLOW_PLAYER_VERT_HORI,
    AI_FOLLOW_PLAYER_IF_IN_FRONT_VERT_HORI,
    AI_RANDOM_LIIKAHDUS_VERT_HORI,
    AI_SAMMAKKO1,
    AI_SAMMAKKO2,
    AI_SAMMAKKO3,
    AI_ATTACK_2_JOS_OSUTTU,
    AI_ATTACK_1_NONSTOP,
    AI_ATTACK_2_NONSTOP,
    AI_KAANTYY_JOS_OSUTTU,
    AI_EVIL_ONE,

    // AI_141 - AI_149

    // New AI
    AI_MAX_SPEED_PLAYER = 150,
    AI_MAX_SPEED_PLAYER_ON_SUPER,
    AI_SWIMMING,
    AI_MAX_SPEED_SWIMMING,

    // AI_154 - AI_200

    // New AI
    AI_INFOS_BEGIN = 201,
    AI_INFOS_END = 301

};

enum { //Damage

    DAMAGE_NONE,
    DAMAGE_IMPACT,
    DAMAGE_DROP,
    DAMAGE_NOISE,
    DAMAGE_FIRE,
    DAMAGE_WATER,
    DAMAGE_SNOW,
    DAMAGE_BONUS,
    DAMAGE_ELECTRIC,
    DAMAGE_ITSARI,
    DAMAGE_COMPRESSION,
    DAMAGE_SMELL,
    DAMAGE_ALL,
    DAMAGE_STITCH

};

enum { // unused

    PROTOTYPE_KANA,
    PROTOTYPE_MUNA,
    PROTOTYPE_PIKKUKANA,
    PROTOTYPE_OMENA,
    PROTOTYPE_HYPPIJA

};

enum {

    TYPE_NOTHING,
    TYPE_GAME_CHARACTER,
    TYPE_BONUS,
    TYPE_PROJECTILE,
    TYPE_TELEPORT,
    TYPE_BACKGROUND

};

enum {

    COLOR_GRAY      = 0,
    COLOR_BLUE      = 32,
    COLOR_RED       = 64,
    COLOR_GREEN     = 96,
    COLOR_ORANGE    = 128,
    COLOR_VIOLET    = 160,
    COLOR_TURQUOISE = 192,
    COLOR_NORMAL    = 255

};

enum { //Destruction Effect

    FX_DESTRUCT_EI_TUHOUDU,
    FX_DESTRUCT_HOYHENET,
    FX_DESTRUCT_TAHDET_HARMAA,
    FX_DESTRUCT_TAHDET_SININEN,
    FX_DESTRUCT_TAHDET_PUNAINEN,
    FX_DESTRUCT_TAHDET_VIHREA,
    FX_DESTRUCT_TAHDET_ORANSSI,
    FX_DESTRUCT_TAHDET_VIOLETTI,
    FX_DESTRUCT_TAHDET_TURKOOSI,
    FX_DESTRUCT_RAJAHDYS_HARMAA,
    FX_DESTRUCT_RAJAHDYS_SININEN,
    FX_DESTRUCT_RAJAHDYS_PUNAINEN,
    FX_DESTRUCT_RAJAHDYS_VIHREA,
    FX_DESTRUCT_RAJAHDYS_ORANSSI,
    FX_DESTRUCT_RAJAHDYS_VIOLETTI,
    FX_DESTRUCT_RAJAHDYS_TURKOOSI,
    FX_DESTRUCT_SAVU_HARMAA,
    FX_DESTRUCT_SAVU_SININEN,
    FX_DESTRUCT_SAVU_PUNAINEN,
    FX_DESTRUCT_SAVU_VIHREA,
    FX_DESTRUCT_SAVU_ORANSSI,
    FX_DESTRUCT_SAVU_VIOLETTI,
    FX_DESTRUCT_SAVU_TURKOOSI,
    FX_DESTRUCT_SAVUPILVET,
    FX_DESTRUCT_ANIMAATIO = 100

};

enum {

    BONUSITEM_NOTHING,
    BONUSITEM_KEY,
    BONUSITEM_SCORE

};

struct PK2SPRITE_ANIMAATIO {

    u8    sekvenssi[ANIMATION_SEQUENCE_SIZE]; // sequence
    u8    frameja;                            // frames
    bool  looppi;                             // loop

};

//.spr file structures
struct PrototypeClass10{

    u32		tyyppi;											// spriten tyyppi
    // Kuvatiedoston tiedot
    char		kuvatiedosto[13];								// .BMP jossa ovat spriten grafiikat
    // Spriteen liittyv�t ��nitehosteet
    char		aanitiedostot[7][13];							// ��nitehostetiedostot
    u32		aanet[7];										// ��nitehosteet (indeksit buffereihin)
    // Spriten kuva- ja animaatio-ominaisuudet
    u8		frameja;										// framejen m��r�
    PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
    u8		animaatioita;									// animaatioiden m��r�
    u8		frame_rate;										// yhden framen kesto
    u32		kuva_x;											// miss� kohtaa kuvaa sprite on
    u32		kuva_y;											// miss� kohtaa kuvaa sprite on
    u32		kuva_frame_leveys;								// yhden framen leveys
    u32		kuva_frame_korkeus;								// yhden framen korkeus
    u32		kuva_frame_vali;								// kahden framen vali
    // Spriten ominaisuudet
    char		nimi[30];										// spriten nimi (n�kyy editorissa)
    u32		leveys;											// spriten leveys
    u32		korkeus;										// spriten korkeus
    double		weight;											// sprite weight (vaikuttaa hyppyyn ja kytkimiin)
    bool		vihollinen;										// onko sprite vihollinen
    u32		energia;										// monta iskua kest��
    u32		vahinko;										// paljon vahinkoa tekee jos osuu
    u32		pisteet;										// paljonko siit� saa pisteit�
    u32		AI[5];											// mit� teko�lyj� k�ytet��n
    u32		suojaus;										// miten suojattu iskuilta
    u8		max_hyppy;										// hypyn maksimikesto
    u8		max_nopeus;										// maksimi nopeus
    u32		charge_time;										// ampumisen j�lkeinen odotus
    u8		vari;											// tehd��nk� spritest� jonkin tietyn v�rinen
    bool		este;											// k�ytt�ytyyk� sprite kuin sein�
    u32		tuhoutuminen;									// miten sprite tuhoutuu
    // Yhteys toisiin spriteihin
    char		muutos_sprite[13];								// Toinen sprite joksi t�m� sprite voi muuttua
    char		bonus_sprite[13];								// Spriten bonuksena j�tt�m� k�ytt�m� sprite
    char		ammus1_sprite[13];								// Spriten ammuksena 1 k�ytt�m� sprite
    char		ammus2_sprite[13];								// Spriten ammuksena 2 k�ytt�m� sprite
    bool		avain;											// Voiko sprite avata lukkoja
};
struct PrototypeClass11{

    u32		tyyppi;											// spriten tyyppi
    // Kuvatiedoston tiedot
    char		kuvatiedosto[13];								// .BMP jossa ovat spriten grafiikat
    // Spriteen liittyv�t ��nitehosteet
    char		aanitiedostot[7][13];							// ��nitehostetiedostot
    u32		aanet[7];										// ��nitehosteet (indeksit buffereihin)
    // Spriten kuva- ja animaatio-ominaisuudet
    u8		frameja;										// framejen m��r�
    PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
    u8		animaatioita;									// animaatioiden m��r�
    u8		frame_rate;										// yhden framen kesto
    u32		kuva_x;											// miss� kohtaa kuvaa sprite on
    u32		kuva_y;											// miss� kohtaa kuvaa sprite on
    u32		kuva_frame_leveys;								// yhden framen leveys
    u32		kuva_frame_korkeus;								// yhden framen korkeus
    u32		kuva_frame_vali;								// kahden framen vali
    // Spriten ominaisuudet
    char		nimi[30];										// spriten nimi (n�kyy editorissa)
    u32		leveys;											// spriten leveys
    u32		korkeus;										// spriten korkeus
    double		weight;											// sprite weight (vaikuttaa hyppyyn ja kytkimiin)
    bool		vihollinen;										// onko sprite vihollinen
    u32		energia;										// monta iskua kest��
    u32		vahinko;										// paljon vahinkoa tekee jos osuu
    u8        vahinko_tyyppi;									// Mink� tyyppist� vahinkoa tekee (1.1)
    u8		suojaus;										// Mink� tyyppiselt� vahingolta on suojattu
    u32		pisteet;										// paljonko siit� saa pisteit�
    u32		AI[5];											// mit� teko�lyj� k�ytet��n
    u8		max_hyppy;										// hypyn maksimikesto
    u8		max_nopeus;										// maksimi nopeus
    u32		charge_time;										// ampumisen j�lkeinen odotus
    u8		vari;											// tehd��nk� spritest� jonkin tietyn v�rinen
    bool		este;											// k�ytt�ytyyk� sprite kuin sein�
    u32		tuhoutuminen;									// miten sprite tuhoutuu
    bool		avain;											// Voiko sprite avata lukkoja
    bool		vibrate;										// Tariseek� sprite satunnaisesti
    u8        bonusten_lkm;									// Bonusten lukum��r�
    u32       attack1_time;									// Hy�kk�ysanimaation 1 kesto (frameja)
    u32       attack2_time;									// Hy�kk�ysanimaation 2 kesto (frameja)
    u32		pallarx_kerroin;								// Vain TYPE_BACKGROUND. Suhde taustakuvaan.
    // Yhteys toisiin spriteihin
    char		muutos_sprite[13];								// Toinen sprite joksi t�m� sprite voi muuttua
    char		bonus_sprite[13];								// Spriten bonuksena j�tt�m� k�ytt�m� sprite
    char		ammus1_sprite[13];								// Spriten ammuksena 1 k�ytt�m� sprite
    char		ammus2_sprite[13];								// Spriten ammuksena 2 k�ytt�m� sprite

};
struct PrototypeClass12{

    u32		tyyppi;											// spriten tyyppi
    // Kuvatiedoston tiedot
    char		kuvatiedosto[13];								// .BMP jossa ovat spriten grafiikat
    // Spriteen liittyv�t ��nitehosteet
    char		aanitiedostot[7][13];							// ��nitehostetiedostot
    u32		aanet[7];										// ��nitehosteet (indeksit buffereihin)
    // Spriten kuva- ja animaatio-ominaisuudet
    u8		frameja;										// framejen m��r�
    PK2SPRITE_ANIMAATIO animaatiot[20];							// animaatio sekvenssit
    u8		animaatioita;									// animaatioiden m��r�
    u8		frame_rate;										// yhden framen kesto
    u32		kuva_x;											// miss� kohtaa kuvaa sprite on
    u32		kuva_y;											// miss� kohtaa kuvaa sprite on
    u32		kuva_frame_leveys;								// yhden framen leveys
    u32		kuva_frame_korkeus;								// yhden framen korkeus
    u32		kuva_frame_vali;								// kahden framen vali
    // Spriten ominaisuudet
    char		nimi[30];										// spriten nimi (n�kyy editorissa)
    u32		leveys;											// spriten leveys
    u32		korkeus;										// spriten korkeus
    double		weight;											// sprite weight (vaikuttaa hyppyyn ja kytkimiin)
    bool		vihollinen;										// onko sprite vihollinen
    u32		energia;										// monta iskua kest��
    u32		vahinko;										// paljon vahinkoa tekee jos osuu
    u8        vahinko_tyyppi;									// Mink� tyyppist� vahinkoa tekee (1.1)
    u8		suojaus;										// Mink� tyyppiselt� vahingolta on suojattu
    u32		pisteet;										// paljonko siit� saa pisteit�
    u32		AI[5];											// mit� teko�lyj� k�ytet��n
    u8		max_hyppy;										// hypyn maksimikesto
    u8		max_nopeus;										// maksimi nopeus
    u32		charge_time;										// ampumisen j�lkeinen odotus
    u8		vari;											// tehd��nk� spritest� jonkin tietyn v�rinen
    bool		este;											// k�ytt�ytyyk� sprite kuin sein�
    u32		tuhoutuminen;									// miten sprite tuhoutuu
    bool		avain;											// Voiko sprite avata lukkoja
    bool		vibrate;										// Tariseek� sprite satunnaisesti
    u8        bonusten_lkm;									// Bonusten lukum��r�
    u32       attack1_time;									// Hy�kk�ysanimaation 1 kesto (frameja)
    u32       attack2_time;									// Hy�kk�ysanimaation 2 kesto (frameja)
    u32		pallarx_kerroin;								// Vain TYPE_BACKGROUND. Suhde taustakuvaan.
    // Yhteys toisiin spriteihin
    char		muutos_sprite[13];								// Toinen sprite joksi t�m� sprite voi muuttua
    char		bonus_sprite[13];								// Spriten bonuksena j�tt�m� k�ytt�m� sprite
    char		ammus1_sprite[13];								// Spriten ammuksena 1 k�ytt�m� sprite
    char		ammus2_sprite[13];								// Spriten ammuksena 2 k�ytt�m� sprite

    bool		tiletarkistus;									// t�rm�ileek� tileihin
    u32		aani_frq;										// ��nien perussoittotaajuus (esim. 22050)
    bool		random_frq;										// satunnaisuutta taajuuteen?

    // Jos sprite on este
    bool		este_ylos;
    bool		este_alas;
    bool		este_oikealle;
    bool		este_vasemmalle;

};
struct PrototypeClass13{

    u32    tyyppi;											// sprite type
    char   kuvatiedosto[100];								// bmp path
    char   aanitiedostot[7][100];							// sound path (max 7)
    u32    aanet[7];										// sound types


    u8     frameja;										// number of frames
    PK2SPRITE_ANIMAATIO animaatiot[20];							// animation sequences
    u8     animaatioita;									// number of animations
    u8     frame_rate;										// frame rate
    u32    kuva_x;											// x position of first frame
    u32    kuva_y;											// y position of first frame
    u32    kuva_frame_leveys;								// frame width
    u32    kuva_frame_korkeus;								// frame height
    u32    kuva_frame_vali;								// space between frames


    char   nimi[30];										// name
    u32    leveys;											// width
    u32    korkeus;										// height
    double weight;											// weight (for jump and switches)
    bool   vihollinen;										// if sprite is a enemy
    u32    energia;										//?sprite energy
    u32    vahinko;										//?damage if it got hit
    u8     vahinko_tyyppi;								//?damage type
    u8     suojaus;										  //?protection type
    u32    pisteet;										// how much score
    u32    AI[10];											// AI type (max 10)
    u8     max_hyppy;										// max jump time
    double max_nopeus;										// max speed
    u32    charge_time;										//?wait post shoot
    u8     vari;											// color
    bool   este;											// is a wall
    u32    tuhoutuminen;									// how sprite is destroyed
    bool   avain;											// can sprite open locks
    bool   vibrate;										//?sprite randomly
    u8     bonusten_lkm;									// number of bonuses
    u32    attack1_time;									// attack 1 duration (frames)
    u32    attack2_time;									// attack 2 duration (frames)
    u32    pallarx_kerroin;								// parallax type (just to TYPE_BACKGROUND)


    char   muutos_sprite[100];								// another sprite that this sprite may change
    char   bonus_sprite[100];								// bonus that this sprite gives
    char   ammus1_sprite[100];								// ammo 1 sprite
    char   ammus2_sprite[100];								// ammo 2 sprite


    bool   tiletarkistus;									//?make sounds?
    u32	   aani_frq;										// sound frequency (def. 22050)
    bool   random_frq;										// use random frequency?


    bool   este_ylos;                                      // if is wall at up
    bool   este_alas;                                      // if is wall at down
    bool   este_oikealle;                                  // if is wall at right
    bool   este_vasemmalle;                                // if is wall at left


    u8     lapinakyvyys;									// transparency //unused
    bool   hehkuu;											// if it is transparent //unused
    u32    tulitauko;										//*ammuspriten ampujalle aiheuttama charge_time
    bool   can_glide;										// can drip quietly down?
    bool   boss;											// if it is a boss //unused
    bool   bonus_always;									// if not there is 1/4 chance of droping bonus
    bool   can_swim;										// walk fast under water

};

//Classes used in game
class PrototypeClass{
    public:

    char    versio[4] = PK2SPRITE_CURRENT_VERSION; //Version
    char    tiedosto[255] = ""; //.spr filename
    
    int     indeksi = 0; //Prototype index
    
    int     tyyppi = TYPE_NOTHING; //Sprite type

    
    char    kuvatiedosto[100] = ""; //.bmp filename

    // Spriteen liittyv�t ��nitehosteet

    char    aanitiedostot[SPRITE_MAX_SOUNDS][100];                    // ��nitehostetiedostot
    int     aanet[SPRITE_MAX_SOUNDS];                                // ��nitehosteet (indeksit buffereihin)

    // Spriten kuva- ja animaatio-ominaisuudet
    int     framet[SPRITE_MAX_FRAMEJA];                        // spriten framet (bitm�pit)
    int     framet_peilikuva[SPRITE_MAX_FRAMEJA];            // spriten framet peilikuvina
    u8      frameja = 0;                                        // framejen m��r�
    PK2SPRITE_ANIMAATIO animaatiot[SPRITE_MAX_ANIMAATIOITA];    // animaatio sekvenssit
    u8      animaatioita = 0;                                    // animaatioiden m��r�
    u8      frame_rate = 0;                                        // yhden framen kesto
    int     kuva_x = 0;                                            // miss� kohtaa kuvaa sprite on
    int     kuva_y = 0;                                            // miss� kohtaa kuvaa sprite on
    int     kuva_frame_leveys = 0;                                // yhden framen leveys
    int     kuva_frame_korkeus = 0;                                // yhden framen korkeus
    int     kuva_frame_vali;                                // kahden framen vali (unused)

    // Spriten ominaisuudet
    char    nimi[100] = "";                                        // spriten nimi (n�kyy editorissa)
    int     leveys = 0;                                            // spriten leveys
    int     korkeus = 0;                                        // spriten korkeus
    double  weight = 0;                                            // sprite weight (vaikuttaa hyppyyn ja kytkimiin)

    bool    vihollinen = false;                                        // onko sprite vihollinen
    int     energia = 0;                                        // monta iskua kest��
    int     vahinko = 0;                                        // paljon vahinkoa tekee jos osuu
    u8      vahinko_tyyppi = DAMAGE_IMPACT;                                    // mink� tyyppist� vahinkoa tekee (1.1)
    u8      suojaus = DAMAGE_NONE;                                        // mink� tyyppiselt� vahingolta on suojattu (1.1)
    int     pisteet = 0;                                        // paljonko siit� saa pisteit�

    int     AI[SPRITE_MAX_AI];                                // mit� teko�lyj� k�ytet��n

    u8      max_hyppy = 0;                                        // hypyn maksimikesto
    double  max_nopeus = 3;                                        // maksiminopeus
    int     charge_time = 0;                                        // time increment (in (dec)conds)
    u8      vari = COLOR_NORMAL;                                            // tehd��nk� spritest� jonkin tietyn v�rinen
    bool    este = false;                                            // k�ytt�ytyyk� sprite kuin sein�
    int     tuhoutuminen = FX_DESTRUCT_ANIMAATIO;                                    // miten sprite tuhoutuu
    bool    avain = false;                                            // Voiko sprite avata lukkoja
    bool    vibrate = false;                                        // T�riseek� sprite satunnaisesti
    u8      bonusten_lkm = 1;                                    // Bonusten lukum��r�
    int     attack1_time = 60;                                    // Hy�kk�ysanimaation 1 kesto (frameja)
    int     attack2_time = 60;                                    // Hy�kk�ysanimaation 2 kesto (frameja)

    int     pallarx_kerroin = 0;                                // Vain TYPE_BACKGROUND. Suhde taustakuvaan.

    // Yhteys toisiin spriteihin

    char    muutos_sprite[100] = "";                                // Toinen sprite joksi t�m� sprite voi muuttua
    char    bonus_sprite[100] = "";                                // Spriten bonuksena j�tt�m� k�ytt�m� sprite
    char    ammus1_sprite[100] = "";                                // Spriten ammuksena 1 k�ytt�m� sprite
    char    ammus2_sprite[100] = "";                                // Spriten ammuksena 2 k�ytt�m� sprite
    int     muutos = -1;                                            // Muutosspriten prototyypin indeksi. -1 jos ei ole
    int     bonus = -1;                                            // Bonusspriten prototyypin indeksi. -1 jos ei ole
    int     ammus1 = -1;                                            // Ammussprite1 prototyypin indeksi. -1 jos ei ole
    int     ammus2 = -1;                                            // Ammussprite1 prototyypin indeksi. -1 jos ei ole

    // Lis�ykset 1.2 versiossa
    bool    tiletarkistus = true;                                    // t�rm�ileek� tileihin
    u32     aani_frq = 22050;                                        // ��nien perussoittotaajuus (esim. 22050)
    bool    random_frq = true;                                        // satunnaisuutta taajuuteen?

    // Jos sprite on este
    bool    este_ylos = true;
    bool    este_alas = true;
    bool    este_oikealle = true;
    bool    este_vasemmalle = true;

    // Lis�ykset 1.3 versiossa
    u8      lapinakyvyys = false;                                    // 0 = ei n�y - 100 = ei l�pin�kyv�
    bool    hehkuu = false;                                            // 0 = ei hehku (t�ytyy olla l�pin�kyv�)
    int     tulitauko = 0;                                        // ammuspriten ampujalle aiheuttama charge_time
    bool    can_glide = false;                                        // voiko tippua hiljaa alas?
    bool    boss = false;                                            // onko johtaja
    bool    bonus_always = false;                                        // j�tt�� aina bonuksen tuhoutuessa
    bool    can_swim = false;                                        // vaikuttaako painovoima vedess�?

    // Muodostimet
    PrototypeClass();
    ~PrototypeClass();

    // Methods
    void Kopioi(const PrototypeClass &proto);
    int  Load(PFile::Path path);
    int  Piirra(int x, int y, int frame);
    bool Onko_AI(int AI);

    void SetProto10(PrototypeClass10 &proto);
    void SetProto11(PrototypeClass11 &proto);
    void SetProto12(PrototypeClass12 &proto);
    void SetProto13(PrototypeClass13 &proto);
    PrototypeClass13 GetProto13();
};
class SpriteClass{
    public:

    bool   aktiivinen;			// true / false
    int    pelaaja;			// 0 = isn't player, 1 = is player
    PrototypeClass *tyyppi;	// osoitin spriten prototyyppiin
    bool   piilota;			// true = ei toiminnassa, false = toiminnassa
    double alku_x;				// spriten alkuper�inen x sijainti
    double alku_y;				// spriten alkuper�inen y sijainti
    double x;					// x-kordinaatti pelikent�ll�
    double y;					// y-kordinaatti pelikent�ll�
    double a;					// horizontal speed
    double b;					// vertical speed
    bool   flip_x;				// true = peilikuva sivusuunnassa
    bool   flip_y;				// true = peilikuva pystysuunnassa
    int    jump_timer;		// hypyn kesto: 0 = ei hypyss�, > 0 = hypyss�, < 0 = tullut alas
    bool   ylos;				// voiko sprite juuri nyt liikkua yl�s
    bool   alas;				// voiko sprite juuri nyt liikkua ....
    bool   oikealle;			// voiko sprite juuri nyt liikkua ....
    bool   vasemmalle;			// voiko sprite juuri nyt liikkua ....
    bool   reuna_vasemmalla;	// onko spriten vasemmalla puolella kuoppa?
    bool   reuna_oikealla;		// onko spriten vasemmalla puolella kuoppa?
    int    energia;			// monta osumaa sprite viel� kest��
    int    emosprite;			// jos spriten on luonut jokin toinen sprite
    double weight;				//  sprite weight
    double kytkinpaino;		// sprite weight + weight above him (why it doesn't work?)
    bool   crouched;				// onko sprite kyykyss�
    int    damage_timer;				// damage timer
    int    invisible_timer;           // invisibility timer
    int    super_mode_timer;          // super mode timer
    int    charging_timer;				// jos on ammuttu, odotetaan
    int    attack1_timer;			// timer joka laskee hy�kk�ys 1:n j�lkeen
    int    attack2_timer;			// timer joka laskee hy�kk�ys 2:n j�lkeen
    bool   vedessa;			// onko sprite vedess�
    bool   piilossa;			// onko sprite piilossa
    double initial_weight;			// spriten alkuper�inen weight - sama kuin prototyypin
    int    saatu_vahinko;		// v�hennet��n energiasta jos erisuuri kuin 0
    u8     saatu_vahinko_tyyppi; // saadun vahingon tyyppi (esim. lumi).
    bool   vihollinen;			// if it is a enemy
    int    ammus1;				// projectile 1 index
    int    ammus2;				// projectile 2 index

    int    seen_player_x;			// where the player was last seen
    int    seen_player_y;

    int    action_timer;			// timer jonka arvo py�rii v�lill� 1 - 32000 (timer)
 
    u8     animation_index;	// animation index
    u8     current_sequence;	// current sequence
    u8     frame_timer;			// frame times
    int    mutation_timer;		// sprite muuttuu muutosspriteksi kun t�m� nollautuu

    SpriteClass();
    SpriteClass(PrototypeClass *tyyppi, int pelaaja, bool piilota, double x, double y);
    ~SpriteClass();
    int Piirra(int kamera_x, int kamera_y);		// Animoi ja piirt�� spriten
    int Animaatio(int anim_i, bool nollaus);	// Vaihtaa spriten animaation
    int Animoi();								// Animoi muttei piirr� sprite�
    bool Onko_AI(int AI);						// Palauttaa true, jos spritell� on ko. AI

    //AI_Functions
    int AI_Kana();
    int AI_Bonus();
    int AI_Egg();
    int AI_Ammus();
    int AI_Jumper();
    int AI_Sammakko1();
    int AI_Sammakko2();
    int AI_Basic();
    int AI_Kaantyy_Esteesta_Hori();
    int AI_Kaantyy_Esteesta_Vert();
    int AI_Varoo_Kuoppaa();
    int AI_Random_Kaantyminen();
    int AI_Random_Suunnanvaihto_Hori();
    int AI_Random_Hyppy();
    int AI_Random_Liikahdus_Vert_Hori();
    int AI_Seuraa_Pelaajaa(SpriteClass &pelaaja);
    int AI_Seuraa_Pelaajaa_Jos_Nakee(SpriteClass &pelaaja);
    int AI_Seuraa_Pelaajaa_Jos_Nakee_Vert_Hori(SpriteClass &pelaaja);
    int AI_Seuraa_Pelaajaa_Vert_Hori(SpriteClass &pelaaja);
    int AI_Jahtaa_Pelaajaa(SpriteClass &pelaaja);
    int AI_Pakenee_Pelaajaa_Jos_Nakee(SpriteClass &pelaaja);
    int AI_Change_When_Energy_Under_2(PrototypeClass &muutos);
    int AI_Change_When_Energy_Over_1(PrototypeClass &muutos);
    int AI_Muutos_Ajastin(PrototypeClass &muutos);
    int AI_Muutos_Jos_Osuttu(PrototypeClass &muutos);
    int AI_Attack_1_Jos_Osuttu();
    int AI_Attack_2_Jos_Osuttu();
    int AI_Attack_1_Nonstop();
    int AI_Attack_2_Nonstop();
    int AI_Attack_1_if_Player_in_Front(SpriteClass &pelaaja);
    int AI_Attack_2_if_Player_in_Front(SpriteClass &pelaaja);
    int AI_Attack_1_if_Player_Bellow(SpriteClass &pelaaja);
    int AI_NonStop();
    int AI_Hyppy_Jos_Pelaaja_Ylapuolella(SpriteClass &pelaaja);
    int AI_Pommi();
    int AI_Damaged_by_Water();
    int AI_Kill_Everyone();
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
    int AI_Teleportti(int i, SpriteClass *spritet, int max, SpriteClass &pelaaja);
    int AI_Kiipeilija();
    int AI_Kiipeilija2();
    bool AI_Info(SpriteClass &pelaaja);
    int AI_Tuhoutuu_Jos_Emo_Tuhoutuu(SpriteClass *spritet);

    int Animation_Perus();
    int Animation_Kana();
    int Animation_Bonus();
    int Animation_Egg();
    int Animation_Ammus();
};
