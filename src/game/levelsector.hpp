//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once
#include "tileset.hpp"
#include "background.hpp"
#include "sprites_handler.hpp"
#include "gfx/bg_particles.hpp"

enum{
    TILES_COMPRESSION_NONE = 0,
    TILES_OFFSET_NEW = 1,
    TILES_OFFSET_LEGACY = 2
};

class BlockPosition{
public:
    BlockPosition(u32 x, u32 y, u32 sector):x(x),y(y),sector(sector){}
    u32 x = 0;
    u32 y = 0;
    u32 sector = 0;
};

class LevelSector{
public:
    LevelSector(std::size_t width, std::size_t height);
    ~LevelSector();

    /**
     * Do not copy objects
    */
    LevelSector(const LevelSector&)=delete;
    LevelSector& operator=(const LevelSector&)=delete;


    std::size_t getWidth()const{
        return this->mWidth;
    }

    std::size_t getHeight()const{
        return this->mHeight;
    }
    std::size_t size()const{
        return this->mSize;
    }

    bool isCorrectTilePos(int pos_x, int pos_y)const{
        return pos_x>=0 && pos_y>=0 &&
        pos_x<(int)this->getWidth() && pos_y<(int)this->getHeight();
    }

    void calculateColors();
    void calculateEdges();
    void animateTilesets(int animation_timer, int button1_timer){
        this->tileset1->animate(animation_timer, button1_timer, this->fire_color_1, this->fire_color_2);
        if(this->tileset2){
            this->tileset2->animate(animation_timer, button1_timer, this->fire_color_1, this->fire_color_2);
        }
    }


    void drawBackgroundTiles(int camera_x, int camera_y, int block_animation_frame)const;
    void drawForegroundTiles(int camera_x, int camera_y, int block_animation_frame,
    int arrows_block_degree, int button1_timer_y, int button2_timer_y, int button3_timer_y)const;

    void drawBackground(int camera_x, int camera_y){
        this->background->draw(camera_x, camera_y);
    }

    PK2BLOCK getBlock(u32 x, u32 y, const std::array<PK2BLOCK, TILESET_SIZE>& block_types)const;
    void openKeylocks(bool effect);
    void changeSkulls(bool effect);
    void countStartSigns(std::vector<BlockPosition>& vec, u32 sector_id)const;
    void startMusic();


    std::string name;

    u8* background_tiles = nullptr;
    u8* foreground_tiles = nullptr;
    u8* sprite_tiles = nullptr;
    bool * edges = nullptr;

    Tileset* tileset1 = nullptr;
    Tileset* tileset2 = nullptr;
    Background* background = nullptr;

    int gfxTexture = -1;

    std::string gfxTextureName;

    SpritesHandler sprites;

    /**
     * @brief 
     * Temporary solution, redesign this to cache the music
     */
    std::string music_name;


    /*int background_scrolling = BACKGROUND_STATIC; // bg movemant type
    int background_picture = -1;*/


    int weather = WEATHER_NORMAL;                // map climate
    double slipperiness_factor = 1.03;


    /**
     * @brief 
     * The average color of the water, used in splash and bubble effects.
     */
    int      splash_color = -1;

    /**
     * @brief 
     * Fire colors
     */
    int     fire_color_1 = 64;  //red
    int     fire_color_2 = 128; //orange

    int     rain_color = 40;

private:
    std::size_t mWidth, mHeight, mSize;

};