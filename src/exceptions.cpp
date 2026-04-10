//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief 
 * Exception utils by SaturninTheAlien.
 */
#include "exceptions.hpp"
#include <sstream>

namespace PExcept{

FileNotFoundException::FileNotFoundException(const std::string& filename, int type):
filename(filename), type(type){
    std::ostringstream os;

    os<<"Missing file: \""<<filename<<"\" (";
    switch (type)
    {
    case MISSING_SPRITE_PROTOTYPE:{
        os<<"Prototype";

    }
    break;
    case MISSING_SPRITE_TEXTURE:{
        os<<"Texture";
    }
    break;
    case MISSING_SPRITE_SOUND:{
        os<<"Sprite sound";

    }
    break;
    case MISSING_BACKGROUND:{
        os<<"Background";
    }
    break;
    case MISSING_TILESET:{
        os<<"Tileset";
    }
    break;
    case MISSING_MUSIC:{
        os<<"Music";
    }
    break;
    case MISSING_LEVEL:{
        os<<"Level";
    }
    break;
    case MISSING_LANG:{
        os<<"Language file";        
    }
    break;
    case MISSING_SFX:{
        os<<"SFX";        
    }
    break;
    case MISSING_GFX:{
        os<<"GFX texture";        
    }
    break;

    default:
        os<<"Unknown";
        break;
    }

    os<<")";

    this->message = os.str();
}
}