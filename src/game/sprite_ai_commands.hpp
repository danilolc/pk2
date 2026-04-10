//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
/**
 * @brief 
 * Experimental commands/waypoints AI by SaturninTheAlien
 */

#pragma once

#include <string>
#include <vector>
#include "engine/PJson.hpp"

class SpriteClass;
namespace SpriteCommands{

class Command
{
public:
    Command()=default;
    virtual ~Command()=default;
    virtual bool execute(SpriteClass*sprite)=0;
};

void Parse_Commands(const nlohmann::json& j_in, std::vector<Command*>& commands_v, int prototypeWidth, int prototypeHeight);

}