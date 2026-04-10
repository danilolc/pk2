//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief
 * Lua utils by SaturninTheAlien
 */
#include "lua_game_events.hpp"

namespace PK2lua{

class EventListener{
public:
    EventListener(int event_type, sol::protected_function func):
        event_type(event_type), func(func) {}

    int event_type;
    sol::protected_function func;
};

std::vector<EventListener> mEventListeners;

void ClearEvents(){
    mEventListeners.clear();
}

void TriggerEventListeners(int event_type){
    for(const EventListener& listener: mEventListeners){
        if(listener.event_type!=event_type) continue;

        sol::protected_function_result res = listener.func();
        if(!res.valid()){
            throw res.get<sol::error>();
        }
    }
}

void AddEventListener(int event_type, sol::object o){
    if(o.is<sol::function>()){
        mEventListeners.push_back(EventListener(event_type, o));
    }
    else{
        throw std::runtime_error("PK2Lua, not proper event listener function!");
    }
}

void ExposeEventsAPI(sol::state& lua, sol::table& PK2_API){

    /**
     * @brief 
     * Lua events API
     */

    PK2_API["events"] = lua.create_table_with(
        "SKULLS_CHANGED", LUA_EVENT_SKULL_BLOCKS_CHANGED,
        "EVENT1", LUA_EVENT_1,
        "EVENT2", LUA_EVENT_2,
        "GAME_STARTED", LUA_EVENT_GAME_STARTED,
        "GAME_TICK", LUA_EVENT_GAME_TICK,
        "KEYLOCKS_OPENED", LUA_EVENT_KEYLOCKS_OPENED,
        "EXIT_TOUCHED", LUA_EVENT_EXIT_TOUCHED,
        "addListener", AddEventListener,
        "cleanListeners", ClearEvents);

}

    
}