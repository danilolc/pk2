//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/platform.hpp"
#include "engine/PFile.hpp"

namespace PGui {

struct Gui {

    bool active;
    void* texture;
    int x, y, w, h;
    u8 alpha;

};

Gui* create_gui(PFile::Path path, int x, int y, int w, int h, u8 alpha);
int remove_gui(Gui* gui);
void update();

}