// http://lua-users.org/wiki/SandBoxes
#include "screens/screens.hpp"

#include "gui.hpp"

#include "engine/PLog.hpp"
#include "engine/PUtils.hpp"
#include "engine/PDraw.hpp"

#include <lua.hpp>

PFile::Path lua_script_path("");

static lua_State* L = nullptr;

/*static const luaL_Reg loadedlibs[] = {
  {LUA_GNAME, luaopen_base},
  //{LUA_LOADLIBNAME, luaopen_package},
  {LUA_COLIBNAME, luaopen_coroutine},
  {LUA_TABLIBNAME, luaopen_table},
  //{LUA_IOLIBNAME, luaopen_io},
  //{LUA_OSLIBNAME, luaopen_os},
  {LUA_STRLIBNAME, luaopen_string},
  {LUA_MATHLIBNAME, luaopen_math},
  {LUA_UTF8LIBNAME, luaopen_utf8},
  //{LUA_DBLIBNAME, luaopen_debug},
  {NULL, NULL}
};*/

static int Load_Lua() {

    PFile::RW* rw = lua_script_path.GetRW("r");
    if (!rw) {

        PLog::Write(PLog::FATAL, "PLua", "Failed to open file: %s", lua_script_path.c_str());
        return -1;

    }

    std::string str;
    rw->read(str);

    L = luaL_newstate();
    if (!L) {

        PLog::Write(PLog::FATAL, "PLua", "Failed to create lua context");
        return -1;

    }
    luaL_openlibs(L); //http://lua-users.org/wiki/SandBoxes

    return 0;

}


int Screen_Lua_Init() {

    if (Load_Lua() != 0)
        return -1;

	if(PUtils::Is_Mobile())
		GUI_Change(UI_TOUCH_TO_START);

	PDraw::set_offset(640, 480);


    return 0;

}

int Screen_Lua() {

    return 0;

}