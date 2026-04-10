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

void ExposePrototypeClass(sol::state& lua);
void ExposeSpriteClass(sol::state& lua);

}