//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief
 * Lua utils by SaturninTheAlien
 */
#include "lua_level.hpp"
#include "game/levelsector.hpp"

namespace PK2lua{

int getBGTile(LevelSector*sector, int pos_x, int pos_y){
    if(sector!=nullptr && sector->isCorrectTilePos(pos_x, pos_y)){
        return sector->background_tiles[sector->getWidth()*pos_y + pos_x];
    }
    return 255;
}

int getFGTile(LevelSector*sector, int pos_x, int pos_y){
    if(sector!=nullptr && sector->isCorrectTilePos(pos_x, pos_y)){
        return sector->foreground_tiles[sector->getWidth()*pos_y + pos_x];
    }
    return 255;
}

void setBGTile(LevelSector*sector, int pos_x, int pos_y, int value){
    if(sector!=nullptr && sector->isCorrectTilePos(pos_x, pos_y)){
        sector->background_tiles[sector->getWidth()*pos_y + pos_x] = (u8)value;
    } 
}

void setFGTile(LevelSector*sector, int pos_x, int pos_y, int value){
    if(sector!=nullptr && sector->isCorrectTilePos(pos_x, pos_y)){
        sector->foreground_tiles[sector->getWidth()*pos_y + pos_x] = (u8)value;
    } 
}

void setWeather(LevelSector*sector, int value){
    if(sector!=nullptr && sector->weather != value){
        sector->weather = value;
        BG_Particles::Init(value, sector->rain_color);
    }
}

SpriteClass* AddSprite3(LevelSector*sector, PrototypeClass* prototype, double x, double y, SpriteClass*parent){
    if(sector!=nullptr && prototype!=nullptr){
        return sector->sprites.addLuaSprite(prototype, x, y, parent);
    }
    return nullptr;
}

SpriteClass* AddSprite4(LevelSector*sector, PrototypeClass* prototype, double x, double y){
    if(sector!=nullptr && prototype!=nullptr){
        return sector->sprites.addLuaSprite(prototype, x, y);
    }
    return nullptr;
}


void ExposeSectorClass(sol::state& lua, sol::table& PK2_API){

    PK2_API["weather"] = lua.create_table_with(
        "NORMAL", WEATHER_NORMAL,
        "RAIN", WEATHER_RAIN,
        "RAIN_AND_LEAVES", WEATHER_RAIN_LEAVES,
        "LEAVES", WEATHER_LEAVES,
        "SNOW", WEATHER_SNOW,
        "DANDELIONS", WEATHER_DANDELIONS);


    lua.new_usertype<LevelSector>(
        "LevelSector",
        sol::no_constructor,
        "getBgTile", getBGTile,
        "getFgTile", getFGTile,
        "setBgTile", setBGTile,
        "setFgTile", setFGTile,
        "getWidth", &LevelSector::getWidth,
        "getHeight", &LevelSector::getHeight,
        "addSprite", sol::overload(AddSprite3, AddSprite4),

        "weather", sol::readonly(&LevelSector::weather),
        "setWeather", setWeather
    );
}

}