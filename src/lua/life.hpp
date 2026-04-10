//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief
 * Conway's Game of Life implemented as an easter egg by SaturninTheAlien
 * Accessible only with the Lua 🌜 API.
 * 
 * Learn more about GoL:
 * https://conwaylife.com/wiki/Conway%27s_Game_of_Life
 */

#pragma once

#include "3rd_party/sol.hpp"

namespace PK2lua{

void ExposeGameOfLife(sol::table& PK2_API);
void ClearGameOfLife();
void UpdateGameOfLife();

}