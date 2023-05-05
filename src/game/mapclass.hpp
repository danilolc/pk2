//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/platform.hpp"
#include "engine/PFile.hpp"

typedef struct {
    int left, top, right, bottom;
} MAP_RECT;

#define PK2MAP_LAST_VERSION "1.3"

#define PK2MAP_MAP_WIDTH  256
#define PK2MAP_MAP_HEIGHT 224
#define PK2MAP_MAP_SIZE   PK2MAP_MAP_WIDTH * PK2MAP_MAP_HEIGHT

#define PK2MAP_MAP_MAX_PROTOTYPES 100

#define SWITCH_INITIAL_VALUE 2000

#define BLOCK_MAX_MASKS 150

enum {

    BLOCK_BARRIER_DOWN = 40,
    BLOCK_LIFT_HORI,
    BLOCK_LIFT_VERT,
    BLOCK_BUTTON2_UP,
    BLOCK_BUTTON3_RIGHT,
    BLOCK_BUTTON2_DOWN,
    BLOCK_BUTTON3_LEFT,
    BLOCK_LOCK,
    BLOCK_SKULL_FOREGROUND,
    BLOCK_SKULL_BACKGROUND,

    BLOCK_ANIM1 = 60,
    BLOCK_ANIM2 = 65,
    BLOCK_ANIM3 = 70,
    BLOCK_ANIM4 = 75,

    BLOCK_DRIFT_LEFT = 140,
    BLOCK_DRIFT_RIGHT,
    BLOCK_SCROLL_UP,
    BLOCK_HIDEOUT,
    BLOCK_FIRE,
    BLOCK_BUTTON1,
    BLOCK_BUTTON2,
    BLOCK_BUTTON3,
    BLOCK_START,
    BLOCK_EXIT,

};

enum {

    WEATHER_NORMAL,
    WEATHER_RAIN,
    WEATHER_LEAVES,
    WEATHER_RAIN_LEAVES,
    WEATHER_SNOW

};

enum {

    BACKGROUND_STATIC,
    BACKGROUND_PARALLAX_VERT,
    BACKGROUND_PARALLAX_HORI,
    BACKGROUND_PARALLAX_VERT_AND_HORI

};

enum BLOCKS {

	BLOCK_BACKGROUND,
	BLOCK_WALL

};


struct PK2BLOCK {

	u8   koodi;
	bool tausta;
	u8   vasemmalle, oikealle, ylos, alas;
	int  vasen, oikea, yla, ala;
	bool water;
	bool border;

};

struct PK2BLOCKMASK {

	int ylos[32];
	int alas[32];
	int vasemmalle[32];
	int oikealle[32];

};

class MapClass {
	private:

	int aste      = 0;  // degree of movable blocks
	int vesiaste  = 0;  // timer for water animation
	int animaatio = 0;  // block animations frame
	u32 button1_timer  = 0;  // button 1 timer
	u32 button2_timer  = 0;  // button 2 timer
	u32 button3_timer  = 0;  // button 3 timer

    public:

    /* Atributs ------------------------*/

    char     versio[5]       = PK2MAP_LAST_VERSION;         // map version. eg "1.3"
    char     palikka_bmp[13] = "blox.bmp";                  // path of block palette .bmp
    char     taustakuva[13]  = "default.bmp";               // path of map bg .bmp
    char     musiikki[13]    = "default.xm";                // path of map music

    char     nimi[40]   = "untitled";                       // map name
    char     tekija[40] = "unknown";                        // map author

    int      jakso          = 0;                            // level of the episode
    int      ilma           = WEATHER_NORMAL;                // map climate
    int      aika           = 0;                            // map time (in (dec)conds)
    u8       extra          = 0;                            // extra config - not used
    u8       tausta         = BACKGROUND_STATIC;            // bg movemant type
    u32      button1_time   = SWITCH_INITIAL_VALUE;         // button 1 time
    u32      button2_time   = SWITCH_INITIAL_VALUE;         // button 2 time
    u32      button3_time   = SWITCH_INITIAL_VALUE;         // button 3 time
    int      pelaaja_sprite = 0;                            // player prototype

    u8       taustat[PK2MAP_MAP_SIZE] = {255};              // map bg tiles 256*224
    u8       seinat [PK2MAP_MAP_SIZE] = {255};              // map fg tiles 256*224
    u8       spritet[PK2MAP_MAP_SIZE] = {255};              // map sprites 256*224
    char     protot [PK2MAP_MAP_MAX_PROTOTYPES][13] = {""}; // map prototype list .spr
    bool     reunat [PK2MAP_MAP_SIZE] = {false};            // map edges - calculated during game

    int      tiles_buffer      = -1;                        // index of block palette
    int      bg_tiles_buffer   = -1;
    int      background_buffer = -1;                        // index of bg image
    int      water_buffer      = -1;                        // index of water palette
    int      bg_water_buffer   = -1;

    int      x = 0;                                         // map icon x pos
	int      y = 0;                                         // map icon x pos
    int      icon = 0;                                      // map icon id

    /* Metodit --------------------------*/

    MapClass();                                             // Oletusmuodostin
    ~MapClass();                                            // Hajoitin

    int Load(PFile::Path path);                             // Load kartta
    int Load_Plain_Data(PFile::Path path);                  // Load kartta ilman grafiikoita

    int Piirra_Taustat(int kamera_x, int kamera_y);
    int Piirra_Seinat (int kamera_x, int kamera_y);

    void Animoi(int degree, int anim, u32 aika1, u32 aika2, u32 aika3);

    void Calculate_Edges();

    private:

    int LoadVersion01(PFile::Path path);
    int LoadVersion10(PFile::Path path);
    int LoadVersion11(PFile::Path path);
    int LoadVersion12(PFile::Path path);
    int LoadVersion13(PFile::Path path);
    
    int Load_BG(PFile::Path path);
    int Load_TilesImage(PFile::Path path);
    int Load_BGSfx(PFile::Path path);

    void Animate_Fire(int tiles);
    void Animate_Waterfall(int tiles);
    void Animate_RollUp(int tiles);
    void Animate_WaterSurface(int tiles);
    void Animate_Water(int tiles, int water_tiles);
    
};
