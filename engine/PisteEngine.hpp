//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################
#pragma once

#include "platform.hpp"

#include "PisteDraw.hpp"
#include "PisteInput.hpp"
#include "PisteGui.hpp"
#include "PisteSound.hpp"
#include "PisteUtils.hpp"
#include "PisteLanguage.hpp"

namespace Piste {

void init(int width, int height, const char* name, const char* icon);
void terminate();

void loop(int (*GameLogic)());
void stop();
float get_fps();

void set_debug(bool set);
void ignore_frame();
bool is_ready();

}
