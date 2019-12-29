//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "engine/types.hpp"

namespace PGui {

void init(int w, int h, void* r);
int  create(int x, int y, int w, int h, BYTE alpha, const char* t_path, DWORD* key);
int  activate(int id, bool active);
int  draw(int pd_alpha);
bool check_key(int key);

}