//#########################
//PisteEngine - PisteDraw
//by Janne Kivilahti from Piste Gamez
//#########################

#pragma once

#include <SDL_surface.h>
#include <SDL_render.h>
#include <SDL_pixels.h>
#include <SDL_video.h>
#include <vector>
#include <string>

#include "PisteFont.hpp"
#include "platform.hpp"

#define		PD_FADE_FAST   5
#define		PD_FADE_NORMAL 2
#define		PD_FADE_SLOW   1

#define		PD_FILTER_NEAREST  "0"
#define		PD_FILTER_BILINEAR "1"

namespace Piste {

struct RECT{
	DWORD x, y, w, h;
};

class Draw {

private:

	const char* window_name;

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_Surface* window_icon; 

	SDL_Surface* frameBuffer8 = NULL;

	std::vector<SDL_Surface*> imageList;
	std::vector<Piste::Font*> fontList;
	SDL_Palette* game_palette = NULL;

	int screen_width;
	int screen_height;
	Piste::RECT screen_dest = {0, 0, 0, 0};
	bool screenfit = false;

	bool ready = false;

	int fade_speed = 0;
	int alpha = 100;

	int x_offset = 0;

public:

	bool is_fading();
	int fade_out(int speed);
	int fade_in(int speed);
	void rotate_palette(BYTE start, BYTE end);

	int image_new(int w, int h);
	int image_load(const char* filename, bool getPalette);
	int image_copy(int src_i, int dst_i);
	int image_cut(int ImgIndex, int x, int y, int w, int h);
	int image_cut(int ImgIndex, Piste::RECT area);
	int image_clip(int index, int x, int y);
	int image_cliptransparent(int index, int x, int y, int alpha);
	int image_cutclip(int index, int dstx, int dsty, int srcx, int srcy, int oikea, int ala);
	int image_cutclip(int index, Piste::RECT srcrect, Piste::RECT dstrect);
	int image_cutcliptransparent(int index, Piste::RECT srcrect, Piste::RECT dstrect, int alpha);
	int image_cutcliptransparent(int index, Piste::RECT srcrect, Piste::RECT dstrect, int alpha, int colorsum);
	void image_getsize(int index, int& w, int& h);
	int image_fliphori(int index);
	int image_snapshot(int index);
	int image_delete(int& index);

	int image_fill(int index, BYTE color);
	int image_fill(int index, int posx, int posy, int oikea, int ala, BYTE color);
	int screen_fill(BYTE color);
	int screen_fill(int posx, int posy, int oikea, int ala, BYTE color);
	void set_mask(int x, int y, int w, int h);

	int drawscreen_start(BYTE *&pixels, DWORD &pitch);
	int drawscreen_end();
	int drawimage_start(int index, BYTE *&pixels, DWORD &pitch);
	int drawimage_end(int index);
	BYTE blend_colors(BYTE color, BYTE colBack,int alpha);

	int font_create(int image, int x, int y, int width, int height, int count);
	int font_create(char* path, char* file);
	int font_write(int font_index, const char* text, int x, int y);
	int font_writealpha(int font_index, const char* text, int x, int y, BYTE alpha);

	int set_filter(const char* filter);
	void set_fullscreen(bool set);
	void adjust_screen();
	void fit_screen(bool fit);
	void change_resolution(int w, int h);

	void get_window_position(int* x, int* y);

	int get_x_offset();
	void set_xoffset(int x);
	void clear_fonts();
	void update(bool draw);
	void* get_renderer();
	
	Draw(int width, int height, const char* name, const char* icon);
	~Draw();

};

}


struct PD_RECT{
	DWORD x, y, w, h;
};

bool PisteDraw2_IsFading();
int PisteDraw2_FadeOut(int speed);
int PisteDraw2_FadeIn(int speed);
void PisteDraw2_RotatePalette(BYTE start, BYTE end);

int PisteDraw2_Image_New(int w, int h);
int PisteDraw2_Image_Load(const char* filename, bool getPalette);
int PisteDraw2_Image_Copy(int src_i, int dst_i);
int PisteDraw2_Image_Cut(int ImgIndex, int x, int y, int w, int h);
int PisteDraw2_Image_Cut(int ImgIndex, PD_RECT area);
int PisteDraw2_Image_Clip(int index, int x, int y);
int PisteDraw2_Image_ClipTransparent(int index, int x, int y, int alpha);
int PisteDraw2_Image_CutClip(int index, int dstx, int dsty, int srcx, int srcy, int oikea, int ala);
int PisteDraw2_Image_CutClip(int index, PD_RECT srcrect, PD_RECT dstrect);
int PisteDraw2_Image_CutClipTransparent(int index, PD_RECT srcrect, PD_RECT dstrect, int alpha);
int PisteDraw2_Image_CutClipTransparent(int index, PD_RECT srcrect, PD_RECT dstrect, int alpha, int colorsum);
void PisteDraw2_Image_GetSize(int index, int& w, int& h);
int PisteDraw2_Image_FlipHori(int index);
int PisteDraw2_Image_Snapshot(int index);
int PisteDraw2_Image_Delete(int& index);

int PisteDraw2_ImageFill(int index, BYTE color);
int PisteDraw2_ImageFill(int index, int posx, int posy, int oikea, int ala, BYTE color);
int PisteDraw2_ScreenFill(BYTE color);
int PisteDraw2_ScreenFill(int posx, int posy, int oikea, int ala, BYTE color);
void PisteDraw2_SetMask(int x, int y, int w, int h);

int PisteDraw2_DrawScreen_Start(BYTE *&pixels, DWORD &pitch);
int PisteDraw2_DrawScreen_End();
int PisteDraw2_DrawImage_Start(int index, BYTE *&pixels, DWORD &pitch);
int PisteDraw2_DrawImage_End(int index);
BYTE PisteDraw2_BlendColors(BYTE color, BYTE colBack,int alpha);

int PisteDraw2_Font_Create(int image, int x, int y, int width, int height, int count);
int PisteDraw2_Font_Create(char* path, char* file);
int PisteDraw2_Font_Write(int font_index, std::string text, int x, int y);
int PisteDraw2_Font_WriteAlpha(int font_index, const char* text, int x, int y, BYTE alpha);

int PisteDraw2_SetFilter(const char* filter);
void PisteDraw2_FullScreen(bool set);
void PisteDraw2_AdjustScreen();
void PisteDraw2_FitScreen(bool fit);
void PisteDraw2_ChangeResolution(int w, int h);

void PisteDraw2_GetWindowPosition(int* x, int* y);

int PisteDraw2_GetXOffset();
void PisteDraw2_SetXOffset(int x);
int PisteDraw2_Start(int width, int height, const char* name, const char* icon);
void PisteDraw2_Clear_Fonts();
int PisteDraw2_Exit();
void PisteDraw2_Update(bool draw);
void* PisteDraw2_GetRenderer();
