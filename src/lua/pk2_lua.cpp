//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief
 * Lua utils by SaturninTheAlien
 */

#include "pk2_lua.hpp"
#include <string>

#include "lua_level.hpp"
#include "lua_sprite_class.hpp"
#include "lua_sprites_list.hpp"

#include "lua_game_events.hpp"
#include "lua_sprite_commands.hpp"
#include "lua_misc.hpp"

#include "life.hpp"

#include "engine/PLog.hpp"
#include "engine/PFile.hpp"
#include "engine/PFilesystem.hpp"

#include "game/game.hpp"

#include "system.hpp"

namespace PK2lua{

sol::object PK2Require(sol::state& lua, const std::string&moduleName, sol::function _require_fallback){

    sol::table loaded = lua["package"]["loaded"];

    // cache hit
    sol::object cached = loaded[moduleName];
    if (cached.valid()) {
        return cached;
    }

    // pk2 API
    if (moduleName == "pk2") {
        sol::table PK2_API = lua.create_table();
        ExposeSectorClass(lua, PK2_API);
        ExposePrototypeClass(lua);
        ExposeSpriteClass(lua);
        ExposeSpriteListAPI(PK2_API);
        ExposeCommandsAPI(PK2_API);
        ExposeEventsAPI(lua, PK2_API);
        ExposeMiscAPI(PK2_API);
        ExposeGameOfLife(PK2_API);


        loaded[moduleName] = PK2_API;
        return PK2_API;
    }

    std::string name = moduleName;
    if (name.size() <= 4 || name.substr(name.size() - 4) != ".lua") {
        name += ".lua";
        cached = loaded[name];
        if (cached.valid()) {
            return cached;
        }
    }    

    std::optional<PFile::Path> file = PFilesystem::FindAsset(name, PFilesystem::LUA_DIR);
    if(file.has_value()){
        sol::object tmp = lua.safe_script(file->GetContentAsString(), sol::script_throw_on_error, "@pk2://" + name);
        if (!tmp.valid() || tmp == sol::lua_nil) {
            tmp = sol::make_object(lua, true);
        }

        loaded[name] = tmp;
        return tmp;
    }
    else if(_require_fallback != sol::lua_nil){
        return _require_fallback(moduleName);
    }
    else{
        return sol::lua_nil;
    }
}

sol::state* CreateGameLuaVM(const std::string& main_lua_script){
    if(main_lua_script.empty()){
        PLog::Write(PLog::INFO, "PK2lua", "No Lua scripting in this level");
        return nullptr;
    }
       
    PLog::Write(PLog::INFO, "PK2lua", "Creating lua VM");
    sol::state* lua = new sol::state();
    lua->open_libraries(sol::lib::base,
            sol::lib::string,
            sol::lib::table,
            sol::lib::math,
            sol::lib::coroutine,
            sol::lib::utf8,
            sol::lib::package);

    sol::function _require_fallback = (*lua)["require"];
    (*lua)["require"] = [&](const std::string& moduleName){
        return PK2Require(*lua, moduleName, _require_fallback);
    };

    PLog::Write(PLog::INFO, "PK2lua", "Running %s", main_lua_script.c_str());

    PK2Require(*lua, main_lua_script, _require_fallback);
    return lua;
}

void UpdateLua(){
    UpdateGameOfLife();
    TriggerEventListeners(LUA_EVENT_GAME_TICK);
}

void DestroyGameLuaVM(sol::state *& lua){
    ClearEvents();
    ClearCommands();
    ClearGameOfLife();
    delete lua;
    lua = nullptr;
}

}