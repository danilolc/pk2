//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "engine/platform.hpp"

#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"
#include "engine/PGui.hpp"
#include "engine/PSound.hpp"
#include "engine/PUtils.hpp"
#include "engine/PLang.hpp"

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
