//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/platform.hpp"
#include "engine/PFile.hpp"
#include "engine/PRender.hpp"

namespace PGui {

struct Gui {

    bool active;
    PRender::RECT src, dst;
    u8 alpha;

};

Gui* create_gui(PRender::RECT src, PRender::RECT dst, u8 alpha);
int  remove_gui(Gui* gui);
void update();

}