//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PFile.hpp"
#include <string>

enum {

    BACKGROUND_STATIC,
    BACKGROUND_PARALLAX_VERT,
    BACKGROUND_PARALLAX_HORI,
    BACKGROUND_PARALLAX_VERT_AND_HORI

};

class Background{
public:
    ~Background(){
        this->clear();
    }

    int scrolling = BACKGROUND_STATIC;

    void load(const std::string& name);
    void clear();
    void draw(int camera_x, int camera_y);

    void setPalette();

    int getWidth()const{
        return this->width;        
    }

    int getHeight()const{
        return this->height;
    }

    std::string name;
private:
    int width = 0;
    int height = 0;

    int picture = -1;
    int palette = -1;
};