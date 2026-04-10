//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "PFile.hpp"

namespace PRender {

class FRECT {
public:
	FRECT() = default;
	FRECT(float x, float y, float w, float h)
	:x(x), y(y), w(w), h(h){}
	float x=0, y=0, w=0, h=0;

};

class RECT {
public:
	RECT() = default;
	RECT(int x, int y, int w, int h)
	:x(x), y(y), w(w), h(h){}
	int x=0, y=0, w=0, h=0;
};

class Renderer {

	public:

	virtual void load_ui_texture(void* surface) = 0;
	virtual void render_ui(FRECT src, FRECT dst, float alpha) = 0;

	virtual void clear_screen() = 0;
	virtual void set_screen(FRECT screen_dst) = 0;
	virtual int  set_shader(int mode) = 0;
	virtual int  set_vsync(bool set) = 0;
	virtual void update(void* _buffer8) = 0;
	virtual ~Renderer() {};

};

enum {

	SHADER_NEAREST,
	SHADER_LINEAR,
	SHADER_CRT,
	SHADER_HQX

};

enum {

	RENDERER_DEFAULT,
	RENDERER_SDL,
	RENDERER_OPENGL,
	RENDERER_OPENGLES,
	//RENDERER_SDL_SOFTWARE,

};

void load_ui_texture(PFile::Path file);
void render_ui(FRECT src, FRECT dst, float alpha);

int  set_shader(int mode);

void set_screen_fill(bool set);
void adjust_screen();

void set_fullscreen(bool set);
void set_pixelperfect();
bool is_fullscreen();

void set_window_size(int w, int h);
void get_window_size(int* w, int* h);
void get_cover_size(int* w, int* h);
void get_window_position(int* x, int* y);

int  set_vsync(bool set);
bool is_vsync();

void init(int width, int height, const char* name, const char* icon);
void terminate();
void update(void* _buffer8);

}