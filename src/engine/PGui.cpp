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

Gui* create_gui(PFile::Path path, int x, int y, int w, int h, u8 alpha) {

    void* tex = NULL;

    if (path.GetFileName().size() > 0) {

        tex = PRender::load_texture(path);
        if (tex == NULL) {

            PLog::Write(PLog::ERR, "PGui", "Can't load gui texture %s", path.c_str());

        }

    }

    Gui* gui = new Gui;

    if (gui == nullptr) {
        PLog::Write(PLog::FATAL, "PGui", "Couldn't alloc gui", path.c_str());
        return nullptr;
    }

    gui->texture = tex;
    gui->x = x;
    gui->y = y;
    gui->w = w;
    gui->h = h;
    
    if (alpha > 255)
        gui->alpha = 255;
    else
        gui->alpha = alpha;
    
    gui->active = false;

    gui_list.push_back(gui);
    return gui;

}

int remove_gui(Gui* gui) {

    if (gui == nullptr)
        return -1;

    gui_list.remove(gui);

    if (gui->texture != NULL)
        PRender::remove_texture(gui->texture);

    delete gui;

    return 0;

}

void update() {

    for(Gui* gui : gui_list){

		if(gui->active && gui->texture != NULL)
            PRender::render_texture(
                gui->texture,
                float(gui->x) / 1920,
                float(gui->y) / 1080,
                float(gui->w) / 1920,
                float(gui->h) / 1080,
                float(gui->alpha) / 255
            );

	}

}

}