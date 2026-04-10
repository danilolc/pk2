//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief
 * Lua utils by SaturninTheAlien
 */

#pragma once

#include "3rd_party/sol.hpp"

namespace PK2lua{

enum{
    LUA_EVENT_SKULL_BLOCKS_CHANGED,
    LUA_EVENT_1,
    LUA_EVENT_2,
    LUA_EVENT_GAME_STARTED,
    LUA_EVENT_GAME_TICK,
    LUA_EVENT_KEYLOCKS_OPENED,
    LUA_EVENT_EXIT_TOUCHED
};


void ExposeEventsAPI(sol::state& lua, sol::table& PK2_API);
void ClearEvents();
void TriggerEventListeners(int event_type);
    
}