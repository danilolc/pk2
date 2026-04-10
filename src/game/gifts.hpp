//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PJson.hpp"
#include "sprites_handler.hpp"
#include "prototypes_handler.hpp"
#include <array>

#define MAX_GIFTS 4


class GiftsHandler{
public:
    GiftsHandler();

    bool add(PrototypeClass* proto);
    void use(SpritesHandler& spritesHandler, SpriteClass* player);
    void clean();
    int count()const{
        return this->giftsNumber;
    }

    PrototypeClass* get(int i){
        return this->gifts_list[i];
    }
    void remove(int i);

    void draw(int x, int y)const;
    void changeOrder();

    int totalScore()const;

    /*friend void to_json(nlohmann::json& j, const GiftsHandler&gh);
    friend void from_json(const nlohmann::json& j, GiftsHandler&gh);*/

    nlohmann::json toJson()const;
    void fromJson(const nlohmann::json& j, PrototypesHandler& prototypes);
private:

    int giftsNumber = 0;
    std::array<PrototypeClass*, MAX_GIFTS> gifts_list; 
};

/*void to_json(nlohmann::json& j, const GiftsHandler&gh);
void from_json(const nlohmann::json& j, GiftsHandler&gh);*/