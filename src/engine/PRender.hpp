//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

namespace PRender {

struct FRECT {
	
	float x, y, w, h;

};

class Renderer {

	public:

	virtual void clear_screen() = 0;
	virtual void set_screen(FRECT screen_dst) = 0;
	virtual int  set_mode(int mode) = 0;
	virtual int  set_vsync(bool set) = 0;
	virtual void update(void* _buffer8, int alpha) = 0;

	//virtual Renderer(int width, int height, void* window) = 0;
	virtual ~Renderer() {};

};

enum {

	MODE_NEAREST,
	MODE_LINEAR,
	MODE_CRT,
	MODE_HQX

};

int  set_filter(const char* filter);
int  set_mode(int mode);

void adjust_screen();
void set_fullscreen(bool set);
void get_window_size(int* w, int* h);
void get_cover_size(int* w, int* h);
void get_window_position(int* x, int* y);

int  set_vsync(bool set);
bool is_vsync();

int  init(int width, int height, const char* name, const char* icon);
void terminate();
void update(void* _buffer8, int alpha);

}