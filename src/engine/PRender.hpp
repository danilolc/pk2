//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

namespace PRender {

const char FILTER_NEAREST[] = "0";
const char FILTER_LINEAR[] = "2";

int  set_filter(const char* filter);

void adjust_screen();
void set_fullscreen(bool set);
void change_window_size(int w, int h);
void get_window_size(int* w, int* h);
void get_window_position(int* x, int* y);
void fit_screen(bool fit);

int  set_vsync(bool set);
bool is_vsync();

int  init(int width, int height, const char* name, const char* icon);
void terminate();
void update(void* _buffer8, int alpha);

}