//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief
 * Lua utils by SaturninTheAlien
 */

#include "lua_sprite_commands.hpp"
#include <map>
#include "exceptions.hpp"
#include <sstream>
#include <functional>
#include "game/spriteclass.hpp"

namespace PK2lua{

static std::map<std::string, sol::protected_function> commands_map;


void ClearCommands(){
    commands_map.clear();
}


sol::protected_function GetCommandByName(const std::string& name){
    auto res = commands_map.find(name);
    if(res==commands_map.end()){
        std::ostringstream os;
        os<<"Lua command: \""<<name<<"\" not found!";
        throw PExcept::PException(os.str());
    }

    return res->second;
}


void AddCommand(const std::string& name, sol::object o){
    if(o.is<std::function<bool(SpriteClass*)>>()){
        sol::protected_function f = o;
        commands_map.emplace(std::make_pair(name, sol::protected_function(f)));
    }
    else{
        throw PExcept::PException("This lua object cannot be a sprite command!");
    }
}

void ExposeCommandsAPI(sol::table& PK2_API){
    PK2_API["addCommand"] = AddCommand;
}


}