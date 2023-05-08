//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include <lua.hpp>

#include "engine/PLua.hpp"
#include "engine/PLog.hpp"

namespace PLua {

int Create_Context(PFile::Path file) {

    PFile::RW* rw = file.GetRW("r");
    if (!rw) {

        PLog::Write(PLog::ERR, "PLua", "Failed to open file: %s", file.c_str());
        return -1;

    }
    
    std::string str;
    rw->read(str);

    lua_State* L = luaL_newstate();
    //luaL_openlibs(L); //http://lua-users.org/wiki/SandBoxes

}

}