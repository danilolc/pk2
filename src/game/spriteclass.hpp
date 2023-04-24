//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PFile.hpp"
#include "engine/types.hpp"

#include <list>

#define PK2SPRITE_CURRENT_VERSION "1.3"

#define MAX_PROTOTYYPPEJA  256

#define SPRITE_MAX_FRAMEJA      50
#define SPRITE_MAX_ANIMAATIOITA 20
#define SPRITE_MAX_AI           10
#define ANIMATION_SEQUENCE_SIZE 10
#define SPRITE_MAX_SOUNDS        7
#define DAMAGE_TIME             50

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

    EFFECT_NONE,
    EFFECT_STARS,
    EFFECT_SMOKE,
    EFFECT_THUNDER,

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

    AI_LIIKKUU_DOWN_JOS_KYTKIN1_PAINETTU,
    AI_LIIKKUU_UP_JOS_KYTKIN1_PAINETTU,
    AI_LIIKKUU_LEFT_JOS_KYTKIN1_PAINETTU,
    AI_LIIKKUU_RIGHT_JOS_KYTKIN1_PAINETTU,
    AI_LIIKKUU_DOWN_JOS_KYTKIN2_PAINETTU,
    AI_LIIKKUU_UP_JOS_KYTKIN2_PAINETTU,
    AI_LIIKKUU_LEFT_JOS_KYTKIN2_PAINETTU,
    AI_LIIKKUU_RIGHT_JOS_KYTKIN2_PAINETTU,
    AI_LIIKKUU_DOWN_JOS_KYTKIN3_PAINETTU,
    AI_LIIKKUU_UP_JOS_KYTKIN3_PAINETTU,
    AI_LIIKKUU_LEFT_JOS_KYTKIN3_PAINETTU,
    AI_LIIKKUU_RIGHT_JOS_KYTKIN3_PAINETTU,

    // AI_68, AI_69

    AI_KAANTYY_ESTEESTA_VERT = 70,
    AI_RANDOM_ALOITUSSUUNTA_VERT,
    AI_START_DIRECTIONS_TOWARDS_PLAYER_VERT,
    AI_KIIPEILIJA,
    AI_KIIPEILIJA2,
    AI_PAKENEE_PELAAJAA_JOS_NAKEE,
    AI_UUSI_JOS_TUHOUTUU,

    AI_NUOLI_LEFT,
    AI_NUOLI_RIGHT,
    AI_NUOLI_UP,
    AI_NUOLI_DOWN,
    AI_NUOLET_VAIKUTTAVAT,

    // AI_82 - AI_100

    AI_TAUSTA_KUU = 101,
    AI_TAUSTA_LIIKKUU_LEFT,  // unused
    AI_TAUSTA_LIIKKUU_RIGHT, // unused

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

    AI_EGG2 = 170,

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

    u8    sekvenssi[ANIMATION_SEQUENCE_SIZE] = {0}; // sequence
    u8    frameja = 0;                              // frames
    bool  looppi = false;                           // loop

};

//.spr file structures
struct PrototypeClass10{

    u32     tyyppi;

    char    kuvatiedosto[13];

    char    aanitiedostot[7][13];
    u32     aanet[7];

    u8      frameja;
    PK2SPRITE_ANIMAATIO animaatiot[20];
    u8      animaatioita;
    u8      frame_rate;
    u32     kuva_x;
    u32     kuva_y;
    u32     kuva_frame_leveys;
    u32     kuva_frame_korkeus;
    u32     kuva_frame_vali;

    char    nimi[30];
    u32     leveys;
    u32     korkeus;
    double  weight;
    bool    vihollinen;
    u32     energia;
    u32     vahinko;
    u32     pisteet;
    u32     AI[5];
    u32     suojaus;
    u8      max_hyppy;
    u8      max_nopeus;
    u32     charge_time;
    u8      vari;
    bool    este;
    u32     tuhoutuminen;

    char    muutos_sprite[13];
    char    bonus_sprite[13];
    char    ammus1_sprite[13];
    char    ammus2_sprite[13];
    bool    avain;
};
struct PrototypeClass11{

    u32     tyyppi;

    char    kuvatiedosto[13];

    char    aanitiedostot[7][13];
    u32     aanet[7];

    u8      frameja;
    PK2SPRITE_ANIMAATIO animaatiot[20];
    u8      animaatioita;
    u8      frame_rate;
    u32     kuva_x;
    u32     kuva_y;
    u32     kuva_frame_leveys;
    u32     kuva_frame_korkeus;
    u32     kuva_frame_vali;

    char    nimi[30];
    u32     leveys;
    u32     korkeus;
    double  weight;
    bool    vihollinen;
    u32     energia;
    u32     vahinko;
    u8      vahinko_tyyppi;
    u8      suojaus;
    u32     pisteet;
    u32     AI[5];
    u8      max_hyppy;
    u8      max_nopeus;
    u32     charge_time;
    u8      vari;
    bool    este;
    u32     tuhoutuminen;
    bool    avain;
    bool    vibrate;
    u8      bonusten_lkm;
    u32     attack1_time;
    u32     attack2_time;
    u32     pallarx_kerroin;

    char    muutos_sprite[13];
    char    bonus_sprite[13];
    char    ammus1_sprite[13];
    char    ammus2_sprite[13];

};
struct PrototypeClass12{

    u32     tyyppi;

    char    kuvatiedosto[13];

    char    aanitiedostot[7][13];
    u32     aanet[7];

    u8      frameja;
    PK2SPRITE_ANIMAATIO animaatiot[20];
    u8      animaatioita;
    u8      frame_rate;
    u32     kuva_x;
    u32     kuva_y;
    u32     kuva_frame_leveys;
    u32     kuva_frame_korkeus;
    u32     kuva_frame_vali;

    char    nimi[30];
    u32     leveys;
    u32     korkeus;
    double  weight;
    bool    vihollinen;
    u32     energia;
    u32     vahinko;
    u8      vahinko_tyyppi;
    u8      suojaus;
    u32     pisteet;
    u32     AI[5];
    u8      max_hyppy;
    u8      max_nopeus;
    u32     charge_time;
    u8      vari;
    bool    este;
    u32     tuhoutuminen;
    bool    avain;
    bool    vibrate;
    u8      bonusten_lkm;
    u32     attack1_time;
    u32     attack2_time;
    u32     pallarx_kerroin;

    char    muutos_sprite[13];
    char    bonus_sprite[13];
    char    ammus1_sprite[13];
    char    ammus2_sprite[13];

    bool    tiletarkistus;
    u32     aani_frq;
    bool    random_frq;

    bool    este_ylos;
    bool    este_alas;
    bool    este_oikealle;
    bool    este_vasemmalle;

};
struct PrototypeClass13{

    u32     tyyppi;                             // sprite type
    char    kuvatiedosto[100];                  // bmp path
    char    aanitiedostot[7][100];              // sound path (max 7)
    u32     aanet[7];                           // sound types

    u8      frameja;                            // number of frames
    PK2SPRITE_ANIMAATIO animaatiot[20];         // animation sequences
    u8      animaatioita;                       // number of animations
    u8      frame_rate;                         // frame rate
    u32     kuva_x;                             // x position of first frame
    u32     kuva_y;                             // y position of first frame
    u32     kuva_frame_leveys;                  // frame width
    u32     kuva_frame_korkeus;                 // frame height
    u32     kuva_frame_vali;                    // space between frames //unused


    char    nimi[30];                           // name
    u32     leveys;                             // width
    u32     korkeus;                            // height
    double  weight;                             // weight (for jump and switches)
    bool    vihollinen;                         // if sprite is a enemy
    u32     energia;                            //?sprite energy
    u32     vahinko;                            //?damage if it got hit
    u8      vahinko_tyyppi;                     //?damage type
    u8      suojaus;                            //?protection type
    u32     pisteet;                            // how much score
    u32     AI[10];                             // AI type (max 10)
    u8      max_hyppy;                          // max jump time
    double  max_nopeus;                         // max speed
    u32     charge_time;                        //?wait post shoot
    u8      vari;                               // color
    bool    este;                               // is a wall
    u32     tuhoutuminen;                       // how sprite is destroyed
    bool    avain;                              // can sprite open locks
    bool    vibrate;                            //?sprite randomly
    u8      bonusten_lkm;                       // number of bonuses
    u32     attack1_time;                       // attack 1 duration (frames)
    u32     attack2_time;                       // attack 2 duration (frames)
    u32     pallarx_kerroin;                    // parallax type (just to TYPE_BACKGROUND)


    char    muutos_sprite[100];                 // another sprite that this sprite may change
    char    bonus_sprite[100];                  // bonus that this sprite gives
    char    ammus1_sprite[100];                 // ammo 1 sprite
    char    ammus2_sprite[100];                 // ammo 2 sprite


    bool    tiletarkistus;                      //?make sounds?
    u32     aani_frq;                           // sound frequency (def. 22050)
    bool    random_frq;                         // use random frequency?


    bool    este_ylos;                          // if is wall at up
    bool    este_alas;                          // if is wall at down
    bool    este_oikealle;                      // if is wall at right
    bool    este_vasemmalle;                    // if is wall at left


    u8      effect;                             // sprite effect
    bool    hehkuu;                             // if it is transparent //unused
    u32     tulitauko;                          //?charge_time inflicted by projectile sprites on the shooter
    bool    can_glide;                          // can drip quietly down?
    bool    boss;                               // if it is a boss //unused
    bool    bonus_always;                       // if not there is 1/4 chance of droping bonus
    bool    can_swim;                           // walk fast under water

};

//Classes used in game
class PrototypeClass{
    public:

    char    versio[4]         = PK2SPRITE_CURRENT_VERSION;
    char    tiedosto[255]     = "";
    char    kuvatiedosto[100] = "";
    
    int     type = TYPE_NOTHING;

    char    aanitiedostot[SPRITE_MAX_SOUNDS][100] = {""};
    int     aanet[SPRITE_MAX_SOUNDS] = {-1};

    int     framet[SPRITE_MAX_FRAMEJA] = {0};
    int     framet_peilikuva[SPRITE_MAX_FRAMEJA] = {0};
    u8      frameja            = 0;
    PK2SPRITE_ANIMAATIO animaatiot[SPRITE_MAX_ANIMAATIOITA] = {};
    u8      animaatioita       = 0;
    u8      frame_rate         = 0;
    int     kuva_x             = 0;
    int     kuva_y             = 0;
    int     kuva_frame_leveys  = 0;
    int     kuva_frame_korkeus = 0;
    int     kuva_frame_vali    = 0;

    char    nimi[100] = "";
    int     leveys    = 0;
    int     korkeus   = 0;
    double  weight    = 0;

    bool    vihollinen     = false;
    int     energia        = 0;
    int     vahinko        = 0;
    u8      vahinko_tyyppi = DAMAGE_IMPACT;
    u8      suojaus        = DAMAGE_NONE;
    int     pisteet        = 0;

    int     AI[SPRITE_MAX_AI] = {AI_NONE};

    u8      max_hyppy    = 0;
    double  max_nopeus   = 3;
    int     charge_time  = 0;
    u8      vari         = COLOR_NORMAL;
    bool    este         = false;
    int     tuhoutuminen = FX_DESTRUCT_ANIMAATIO;
    bool    avain        = false;
    bool    vibrate      = false;
    u8      bonusten_lkm = 1;
    int     attack1_time = 60;
    int     attack2_time = 60;

    int     pallarx_kerroin = 0;

    char    muutos_sprite[100] = "";
    char    bonus_sprite[100]  = "";
    char    ammus1_sprite[100] = "";
    char    ammus2_sprite[100] = "";
    PrototypeClass* muutos     = nullptr;
    PrototypeClass* bonus      = nullptr;
    PrototypeClass* ammus1     = nullptr;
    PrototypeClass* ammus2     = nullptr;

    bool    tiletarkistus = true;
    u32     aani_frq      = 22050;
    bool    random_frq    = true;

    bool    este_ylos       = true;
    bool    este_alas       = true;
    bool    este_oikealle   = true;
    bool    este_vasemmalle = true;

    u8      effect       = EFFECT_NONE;
    bool    hehkuu       = false;
    int     tulitauko    = 0;
    bool    can_glide    = false;
    bool    boss         = false;
    bool    bonus_always = false;
    bool    can_swim     = false;

    PrototypeClass();
    ~PrototypeClass();

    int     Load(PFile::Path path);
    int     Piirra(int x, int y, int frame);
    bool    Onko_AI(int AI);

    void    SetProto10(PrototypeClass10 &proto);
    void    SetProto11(PrototypeClass11 &proto);
    void    SetProto12(PrototypeClass12 &proto);
    void    SetProto13(PrototypeClass13 &proto);
};
class SpriteClass{
    public:

    bool    aktiivinen       = false;           // if the sprite is acting
    int     pelaaja          = 0;               // 0 = isn't player, 1 = is player
    PrototypeClass *tyyppi   = nullptr;         // the sprite prototype
    bool    piilota          = true;            // the sprite was removed
    double  alku_x           = 0;               // original x location
    double  alku_y           = 0;               // original y location
    double  x                = 0;               // sprite x location
    double  y                = 0;               // sprite y location
    double  a                = 0;               // horizontal speed
    double  b                = 0;               // vertical speed
    bool    flip_x           = false;           // if it is flipped horizontally
    bool    flip_y           = false;           // if it is flipped vertically
    int     jump_timer       = 0;               // jump times: = 0 not jumping; > 0 jumping; < 0 falling
    bool    ylos             = true;            // can sprite move up now?
    bool    alas             = true;            // can sprite move down now?
    bool    oikealle         = true;            // can sprite move right now?
    bool    vasemmalle       = true;            // can sprite move left now?
    bool    reuna_vasemmalla = false;           // is there a pit on the left side of the sprite?
    bool    reuna_oikealla   = false;           // is there a pit on the right side of the sprite?
    int     energia          = 0;               // the sprite energy
    SpriteClass *emosprite   = nullptr;         // the sprite's parent
    double  weight           = 0;               // sprite weight
    double  kytkinpaino      = 0;               // sprite weight + weight above him (why it doesn't work?)
    bool    crouched         = false;           // if the sprite is crouched
    int     damage_timer     = 0;               // damage timer
    int     invisible_timer  = 0;               // invisibility timer
    int     super_mode_timer = 0;               // super mode timer
    int     charging_timer   = 0;               // charging time for the attacks
    int     attack1_timer    = 0;               // timer after attack 1
    int     attack2_timer    = 0;               // timer after attack 2
    bool    vedessa          = false;           // if the sprite is inside water
    bool    piilossa         = false;           // if the sprite is hidden
    double  initial_weight   = 0;               // sprite's original weight - the same as that of the prototype
    int     saatu_vahinko    = 0;               // damage taken
    u8      saatu_vahinko_tyyppi = DAMAGE_NONE; // damage taken type (e.g. snow).
    bool    vihollinen       = false;           // if it is a enemy
    PrototypeClass* ammus1   = nullptr;         // projectile 1
    PrototypeClass* ammus2   = nullptr;         // projectile 2

    int     seen_player_x    = -1;              // where the player was last seen x
    int     seen_player_y    = -1;              // where the player was last seen y

    int     action_timer     = 0;               // timer for some AI actions. vary from 1 to 32000
 
    u8      animation_index  = ANIMATION_IDLE;  // animation index
    u8      current_sequence = 0;               // current sequence
    u8      frame_timer      = 0;               // frame times
    int     mutation_timer   = 0;               // the mutation timer

    
    SpriteClass();
    SpriteClass(PrototypeClass *tyyppi, int pelaaja, double x, double y);
    ~SpriteClass();

    int  Piirra(int kamera_x, int kamera_y);   // animate and draw the sprite
    int  Animaatio(int anim_i, bool nollaus);  // set sprite animation
    int  Animoi();                             // animate the sprite
    void HandleEffects();                      // create sprite effects
    bool Onko_AI(int AI);                      // if the sprite has a AI

    //AI_Functions
    int AI_Kana();
    int AI_Bonus();
    int AI_Egg();
    int AI_Egg2();
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
    int AI_Change_When_Energy_Under_2(PrototypeClass *muutos);
    int AI_Change_When_Energy_Over_1(PrototypeClass *muutos);
    int AI_Muutos_Ajastin(PrototypeClass *muutos);
    int AI_Muutos_Jos_Osuttu(PrototypeClass *muutos);
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
    int AI_Teleportti(std::list<SpriteClass*> spritet, SpriteClass &pelaaja);
    int AI_Kiipeilija();
    int AI_Kiipeilija2();
    bool AI_Info(SpriteClass &pelaaja);
    int AI_Tuhoutuu_Jos_Emo_Tuhoutuu();

    int Animation_Perus();
    int Animation_Kana();
    int Animation_Bonus();
    int Animation_Egg();
    int Animation_Ammus();
};
