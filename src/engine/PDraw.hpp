//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PFile.hpp"
#include "engine/PFont.hpp"
#include "engine/platform.hpp"

namespace PDraw {

struct RECT {

    s32 x, y, w, h;

};

void  rotate_palette(u8 start, u8 end);

int   image_new(int w, int h);
int   image_load(PFile::Path path, bool getPalette);
int   image_load(int& index, PFile::Path path, bool getPalette);
int   image_copy(int image);
int   image_cut(int ImgIndex, int x, int y, int w, int h);
int   image_cut(int ImgIndex, RECT area);
int   image_clip(int index);
int   image_clip(int index, int x, int y);
int   image_cliptransparent(int index, int x, int y, int alpha, u8 colorsum);
int   image_cutclip(int index, int dstx, int dsty, int srcx, int srcy, int oikea, int ala);
int   image_cutclip(int index, RECT srcrect, RECT dstrect);
int   image_cutcliptransparent(int index, RECT srcrect, RECT dstrect, int alpha, u8 colorsum);
int   image_cutcliptransparent(int index, int src_x, int src_y, int src_w, int src_h,
        int dst_x, int dst_y, int alpha, u8 colorsum);
void  image_getsize(int index, int& w, int& h);
int   image_fliphori(int index);
int   image_snapshot(int index);
int   image_delete(int& index);

int   image_fill(int index, u8 color);
int   image_fill(int index, int posx, int posy, int oikea, int ala, u8 color);
int   screen_fill(u8 color);
int   screen_fill(int posx, int posy, int oikea, int ala, u8 color);
void  set_mask(int x, int y, int w, int h);
void  reset_mask();

int   drawscreen_start(u8 *&pixels, u32 &pitch);
int   drawscreen_end();
int   drawimage_start(int index, u8 *&pixels, u32 &pitch);
int   drawimage_end(int index);
int   create_shadow(int index, u32 width, u32 height);

int   font_create(int image, int x, int y, int width, int height, int count);
int   font_create(PFile::Path path);
int   font_write(int font_index, const char* text, int x, int y);
int   font_writealpha(int font_index, const char* text, int x, int y, int alpha);

void  set_buffer_size(int w, int h);
void  get_buffer_size(int* w, int* h);
void  get_offset(int* x, int* y);
void  set_offset(int width, int height);
void  clear_fonts();

void  set_rgb(float r, float g, float b);

void  get_buffer_data(void** _buffer8);
void  update();
int   init(int width, int height);
int   terminate();

}
