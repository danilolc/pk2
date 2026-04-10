//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once
#include <string>
#include "engine/PFile.hpp"

#include <random>

#define TILESET_SIZE 150

enum {

    BLOCK_BARRIER_DOWN = 40,
    BLOCK_LIFT_HORI, //41
    BLOCK_LIFT_VERT, //42
    BLOCK_BUTTON2_UP, //43
    BLOCK_BUTTON3_RIGHT, //44
    BLOCK_BUTTON2_DOWN, //45
    BLOCK_BUTTON3_LEFT, //46
    BLOCK_LOCK, //47
    BLOCK_SKULL_FOREGROUND, //48
    BLOCK_SKULL_BACKGROUND, //49

    BLOCK_ANIM1 = 60,
    BLOCK_ANIM2 = 65,
    BLOCK_ANIM3 = 70,
    BLOCK_ANIM4 = 75,

    BLOCK_DRIFT_LEFT = 140,
    BLOCK_DRIFT_RIGHT, //141
    BLOCK_SCROLL_UP, //142
    BLOCK_HIDEOUT, //143
    BLOCK_FIRE, //144
    BLOCK_BUTTON1, //145
    BLOCK_BUTTON2, //146
    BLOCK_BUTTON3, //147
    BLOCK_START, //148
    BLOCK_EXIT, //149

};


enum BLOCKS {

	BLOCK_BACKGROUND,
	BLOCK_WALL

};

struct PK2BLOCK {

	u8   id;
	u8   left_side, right_side, top_side, bottom_side;
	int  left, right, top, bottom;
	bool water;
	bool border;

};

struct PK2BLOCKMASK {
	int ylos[32];
	int alas[32];
};

class Tileset{
public:
    Tileset()=default;
    ~Tileset(){
        this->clear();
    }
    /**
     * Do not allow copying
    */
    Tileset(const Tileset& ) = delete;
    Tileset& operator= (const Tileset& ) = delete;

    int calculateSplashColor();

    void clear();
    void loadImage(const std::string& name);

    int getImage()const{
        return this->tiles;
    }

    void animate(int animation_timer, int button1_timer, int fire_color1, int fire_color2);

    operator bool()const{
        return this->tiles != -1;
    }

    std::string name;
    std::array<PK2BLOCKMASK, TILESET_SIZE> block_masks;
private:

    void animateFire(int button1_timer, int color1, int color2);
    void animateWaterfall();
    void animateWaterSurface();
    void animateWater(int animation_timer);
    void animateRollUp();


    void calculateBlockMasks();
    void make254Transparent();

    int tiles = -1;
    int water_tiles = -1;

    /**
     * @brief 
     * Pseudo-random generator with a constant seed to animate waterfall and fire
     * To fix "unsynced" BG / FG waterfall and fire tiles
     */
    std::mt19937 anim_rand = std::mt19937(13);
};