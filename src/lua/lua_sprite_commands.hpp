//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief
 * Lua utils by SaturninTheAlien
 */

#pragma once

#include <string>
#include "3rd_party/sol.hpp"

namespace PK2lua{

void ExposeCommandsAPI(sol::table& PK2_API);
void ClearCommands();
sol::protected_function GetCommandByName(const std::string& name);

}