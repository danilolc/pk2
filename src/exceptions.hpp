//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief 
 * Exception utils by SaturninTheAlien.
 */
#pragma once

#include <stdexcept>
#include <string>

namespace PExcept{

enum{
    MISSING_SPRITE_PROTOTYPE=0,
    MISSING_SPRITE_TEXTURE=1,
    MISSING_SPRITE_SOUND=2,

    MISSING_BACKGROUND=3,
    MISSING_TILESET=4,
    MISSING_MUSIC=5,
    MISSING_LEVEL=6,
    MISSING_LANG=7,
    MISSING_SFX=8,
    MISSING_GFX=9
};

class PException: public std::exception{
public:
    PException(const std::string& message):
    message(message){}
    const char* what() const noexcept{
        return message.c_str();
    }
private:
    std::string message;
};


class FileNotFoundException: public std::exception{
public:
    FileNotFoundException(const std::string& filename, int type);
    const char* what() const noexcept{
        return message.c_str();
    }
    const std::string filename;
    const int type;
private:
    std::string message;
};


}