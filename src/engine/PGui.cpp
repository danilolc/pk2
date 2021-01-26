//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PGui.hpp"

#include "engine/PLog.hpp"
#include "engine/PRender.hpp"

#include <list>

namespace PGui {

static std::list<Gui*> gui_list;

Gui* create_gui(PRender::RECT src, PRender::RECT dst, u8 alpha) {

    Gui* gui = new Gui;

    if (gui == nullptr) {
        PLog::Write(PLog::FATAL, "PGui", "Couldn't alloc gui");
        return nullptr;
    }

    gui->src = src;
    gui->dst = dst;

    gui->alpha = alpha;
    gui->active = false;

    gui_list.push_back(gui);
    return gui;

}

int remove_gui(Gui* gui) {

    if (gui == nullptr)
        return -1;

    gui_list.remove(gui);

    delete gui;

    return 0;

}

void update() {

    for(Gui* gui : gui_list){

		if(gui->active) {

            if (gui->src.w == 0 || gui->src.h == 0)
                continue;

            PRender::FRECT dst;
            dst.x = float(gui->dst.x) / 1920;
            dst.y = float(gui->dst.y) / 1080;
            dst.w = float(gui->dst.w) / 1920;
            dst.h = float(gui->dst.h) / 1080;

            PRender::FRECT src;
            src.x = float(gui->src.x) / 1024;
            src.y = float(gui->src.y) / 1024;
            src.w = float(gui->src.w) / 1024;
            src.h = float(gui->src.h) / 1024;

            PRender::render_ui(src, dst, float(gui->alpha) / 255);

        }

	}

}

}