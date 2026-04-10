//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "types.hpp"

#include "PDraw.hpp"
#include "PRender.hpp"
#include "PSound.hpp"
#include "PLang.hpp"
#include "PLog.hpp"
#include "PFile.hpp"
#include "PInput.hpp"

#include <functional>

namespace Piste {

void init(int width, int height, const char* name, const char* icon, int audio_buffer_size);
void terminate();

void loop(std::function<void()> GameLogic);
void stop();
int get_fps();

int set_fps(int fps);
void set_debug(bool set);
void ignore_frame();
bool is_ready();

}
