//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PDraw.hpp"

#include "engine/PFont.hpp"
#include "engine/PLog.hpp"

#include <algorithm>
#include <vector>

#include <SDL_image.h>

namespace PDraw {

// 8-bit indexed surface, it's the game frame buffer
static SDL_Surface* frameBuffer8 = NULL;

// All surfaces will have this palette
// game_colors stores the original colors,
// without alpha modification
static SDL_Palette* game_palette = NULL;
static SDL_Color game_colors[256];
static SDL_Color buff_colors[256];

static std::vector<SDL_Surface*> imageList;
static std::vector<PFont*> fontList;

static bool ready = false;

static int fade_speed = 0;
static int alpha = 100;

static int x_offset = 0;
static int y_offset = 0;

static int offset_width = 0;
static int offset_height = 0;

static int update_palette_alpha() {

    if (alpha == 100)
        return SDL_SetPaletteColors(game_palette, game_colors, 0, 256);

    // Why cant't it be defined here?
    //SDL_Color buff_colors[256];

    for (int i = 0; i < 256; i++) {

        buff_colors[i].r = int(game_colors[i].r) * alpha / 100;
        buff_colors[i].g = int(game_colors[i].g) * alpha / 100;
        buff_colors[i].b = int(game_colors[i].b) * alpha / 100;

    }

    return SDL_SetPaletteColors(game_palette, buff_colors, 0, 256);

}

static int findfreeimage() {
    int size = imageList.size();

    for(int i = 0; i < size; i++)
        if(imageList[i] == nullptr)
            return i;

    imageList.push_back(nullptr);
    return size;
}

static int findfreefont(){
    int size = fontList.size();

    for(int i = 0; i < size; i++)
        if(fontList[i] == nullptr)
            return i;

    fontList.push_back(nullptr);
    return size;
}

bool is_fading(){
  if (alpha > 0 && fade_speed < 0)
    return true;

  if (alpha < 100 && fade_speed > 0)
    return true;

  return false;
}
int  fade_out(int speed){
    alpha = 100;
    fade_speed = -speed;
    update_palette_alpha();
    return 0;
}
int  fade_in(int speed){
    alpha = 0;
    fade_speed = speed;
    update_palette_alpha();
    return 0;
}
void rotate_palette(u8 start, u8 end){

    SDL_Color temp_color = game_colors[end];

    for (uint i = end; i > start; i--)
        game_colors[i] = game_colors[i-1];

    game_colors[start] = temp_color;

    update_palette_alpha();

}

int image_new(int w, int h){
    int index = findfreeimage();
    if (index == -1) return -1;

    imageList[index] = SDL_CreateRGBSurface(0, w, h, 8, 0, 0, 0, 0);
    SDL_SetPixelFormatPalette(imageList[index]->format, game_palette);
    SDL_SetColorKey(imageList[index], SDL_TRUE, 255);

    SDL_FillRect(imageList[index], NULL, 255);
    
    return index;
}
int image_load(PFile::Path path, bool getPalette) {

    int index = findfreeimage();

    if (index == -1) {

        PLog::Write(PLog::ERR, "PDraw", "image_load got index -1");
        return -1;
    
    }

    PFile::RW* rw = path.GetRW("r");
    if (!rw) {

        PLog::Write(PLog::ERR, "PDraw", "Couldn't find %s", path.c_str());
        return -1;

    }

    imageList[index] = IMG_Load_RW((SDL_RWops*) rw, 0);
    rw->close();

    if (imageList[index] == NULL) {

        PLog::Write(PLog::ERR, "PDraw", "Couldn't load %s", path.c_str());
        return -1;
    
    }

    if(imageList[index]->format->BitsPerPixel != 8) {

        PLog::Write(PLog::ERR, "PDraw", "Failed to open %s, just 8bpp indexed images!", path.c_str());
        image_delete(index);
        return -1;
    
    }

    if(getPalette) {

        SDL_Palette* pal = imageList[index]->format->palette;
        SDL_memcpy(game_colors, pal->colors, sizeof(SDL_Color) * 256);
        update_palette_alpha();
    
    }

    SDL_SetPixelFormatPalette(imageList[index]->format, game_palette);
    SDL_SetColorKey(imageList[index], SDL_TRUE, 255);

    return index;

}

int image_load(int& index, PFile::Path path, bool getPalette) {
    
    image_delete(index);
    index = image_load(path, getPalette);

    return index;

}

int image_copy(int src_i, int dst_i) {

    if(src_i < 0 || dst_i < 0)
        return -1;
    
    SDL_FillRect(imageList[dst_i], NULL, 255);
    SDL_BlitSurface(imageList[src_i], NULL, imageList[dst_i], NULL);
    
    return 0;

}

int image_cut(int ImgIndex, int x, int y, int w, int h) {

    RECT area;
    area.x = x; area.y = y;
    area.w = (w <= 0) ? imageList[ImgIndex]->w : w; //If 0 get the entire image
    area.h = (h <= 0) ? imageList[ImgIndex]->h : h;

    return image_cut(ImgIndex, area);

}
int image_cut(int ImgIndex, RECT area) {

    int index = findfreeimage();

    if (index == -1) {

        PLog::Write(PLog::ERR, "PDraw", "image_cut got index -1");
        return -1;
    
    }

    imageList[index] = SDL_CreateRGBSurface(0, area.w, area.h, 8, 0, 0, 0, 0);

    SDL_SetPixelFormatPalette(imageList[index]->format, game_palette);
    SDL_SetColorKey(imageList[index], SDL_TRUE, 255);

    SDL_FillRect(imageList[index], NULL, 255);

    // TODO - BlitScaled?
    SDL_BlitScaled(imageList[ImgIndex], (SDL_Rect*)&area, imageList[index], NULL);

    return index;

}

// Clip on the center of the screen
int image_clip(int index) {

    SDL_Rect dstrect;
    SDL_Surface* image = imageList[index];

    dstrect.x = (frameBuffer8->w - image->w) / 2;
    dstrect.y = (frameBuffer8->h - image->h) / 2;
    dstrect.w = image->w;
    dstrect.h = image->h;
    
    SDL_BlitSurface(image, NULL, frameBuffer8, &dstrect);

    return 0;
}

int image_clip(int index, int x, int y) {

    SDL_Rect dstrect;

    dstrect.x = x + x_offset;
    dstrect.y = y + y_offset;
    dstrect.w = imageList[index]->w;
    dstrect.h = imageList[index]->h;

    SDL_BlitSurface(imageList[index], NULL, frameBuffer8, &dstrect);

    return 0;

}

int image_cliptransparent(int index, int x, int y, int alpha, u8 colorsum) {
    
    return image_cutcliptransparent(
        index,
        0,
        0,
        imageList[index]->w,
        imageList[index]->h, 
        x,
        y,
        alpha,
        colorsum);

}

//TODO - keep a default order (src_x, src_y, src_w, src_h, dst_x, dst_y)
int image_cutclip(int index, int dstx, int dsty, int srcx, int srcy, int oikea, int ala) {

    RECT src = {srcx, srcy, oikea-srcx, ala-srcy};
    RECT dst = {dstx, dsty, oikea-srcx, ala-srcy};
    image_cutclip(index, src, dst);

    return 0;

}

int image_cutclip(int index, RECT srcrect, RECT dstrect) {

    dstrect.x += x_offset;
    dstrect.y += y_offset;
    SDL_BlitSurface(imageList[index], (SDL_Rect*)&srcrect, frameBuffer8, (SDL_Rect*)&dstrect);

    return 0;

}

int image_cutcliptransparent(int index, RECT src, RECT dst, int alpha, u8 colorsum) {

	return image_cutcliptransparent(
        index, 
        src.x, 
        src.y, 
        src.w, 
        src.h,
	    dst.x, 
        dst.y, 
        alpha, 
        colorsum);
    
}

// TODO - REDO THIS FUNCTION
int image_cutcliptransparent(int index, int src_x, int src_y, int src_w, int src_h,
						 int dst_x, int dst_y, int alpha, u8 colorsum) {
    
    dst_x += x_offset;
    dst_y += y_offset;

    if (alpha > 100) alpha = 100;
    if (alpha <= 0) alpha = 0;


    uint x_start = src_x;
    if (dst_x < 0) x_start -= dst_x;

    uint x_end = src_x + src_w;
    int dx = dst_x + (src_w - frameBuffer8->w);
    if (dx > int(x_end)) return -1;
    if (dx > 0) x_end -= dx;

    if (x_start >= x_end) return -1;


    uint y_start = src_y;
    if (dst_y < 0) y_start -= dst_y;

    uint y_end = src_y + src_h;
    int dy = dst_y + (src_h - frameBuffer8->h);
    if (dy > int(y_end)) return -1;
    if (dy > 0) y_end -= dy;

    if (y_start >= y_end) return -1;


    u8 *imagePix = nullptr;
    u8 *screenPix = nullptr;
    u32 imagePitch, screenPitch;

    drawimage_start(index, imagePix, imagePitch);
    drawscreen_start(screenPix, screenPitch);
    
    for (int posx = x_start; posx < int(x_end); posx++)
        for (int posy = y_start; posy < int(y_end); posy++) {

            u8 color1 = imagePix[ posx + imagePitch * posy ];
            if (color1 != 255) {

                int screen_x = posx + dst_x - src_x;
                int screen_y = posy + dst_y - src_y;
                
                int fy = screen_x + screenPitch * screen_y;

                int sum = colorsum;
                if (sum == 255)
                    sum = color1 & 0b11100000;

                u8 color2 = screenPix[fy];
                screenPix[fy] = blend_colors(color1, color2, alpha) + sum;

            }

        }
    
    drawscreen_end();
    drawimage_end(index);
    
    return 0;

}

void image_getsize(int index, int& w, int& h) {

    w = imageList[index]->w;
    h = imageList[index]->h;

}

int image_fliphori(int index) {

    if(index < 0) return -1;

    int h = imageList[index]->h;
    int w = imageList[index]->w;
    int p = imageList[index]->pitch;

    SDL_LockSurface(imageList[index]);

    u8* pix_array  = (u8*)(imageList[index]->pixels);

    for( int i = 0; i < h*p; i += p)
        std::reverse(&pix_array[i], &pix_array[i + w]);

    SDL_UnlockSurface(imageList[index]);

    return 0;

}
int image_snapshot(int index) {

    //image_new(w, h)

    return SDL_BlitSurface(frameBuffer8, NULL, imageList[index], NULL);

}

int image_delete(int& index) {

    if(index < 0)
        return -1;

    if(uint(index) >= imageList.size())
        return -1;
    
    if (imageList[index] == NULL)
        return -1;
    
    SDL_FreeSurface(imageList[index]);

    imageList[index] = NULL;
    index = -1;
    
    return 0;

}

int image_fill(int index, u8 color) {

    return image_fill(index, 0, 0, imageList[index]->w, imageList[index]->h, color);

}

int image_fill(int index, int posx, int posy, int oikea, int ala, u8 color) {

    SDL_Rect r = {posx, posy, oikea-posx, ala-posy};
    return SDL_FillRect(imageList[index], &r, color);

}

int screen_fill(u8 color) {

    return SDL_FillRect(frameBuffer8, NULL, color);

}

int screen_fill(int posx, int posy, int oikea, int ala, u8 color) {

    SDL_Rect r = {posx + x_offset, posy + y_offset, oikea-posx, ala-posy};
    return SDL_FillRect(frameBuffer8, &r, color);

}

void set_mask(int x, int y, int w, int h) {

    SDL_Rect r = {x + x_offset, y + y_offset, w, h};
    SDL_SetClipRect(frameBuffer8, &r);

}

void reset_mask() {

    SDL_SetClipRect(frameBuffer8, NULL);

}

int drawscreen_start(u8* &pixels, u32 &pitch) {

    pixels = (u8*)frameBuffer8->pixels;
    pitch = frameBuffer8->pitch;

    return SDL_LockSurface(frameBuffer8);

}

int drawscreen_end() {

    SDL_UnlockSurface(frameBuffer8);
    return 0;

}

int drawimage_start(int index, u8* &pixels, u32 &pitch) {

    pixels = (u8*)imageList[index]->pixels;
    pitch = imageList[index]->pitch;

    return SDL_LockSurface(imageList[index]);

}

int drawimage_end(int index) {

    SDL_UnlockSurface(imageList[index]);
    return 0;

}

inline u8 blend_colors(u8 color, u8 colBack, int alpha) {
    
    if(alpha > 100) alpha = 100;
    if(alpha < 0) alpha = 0;

    int result = color % 32;
    result = (result*alpha) / 100;
    result += colBack % 32;
    if(result > 31) result = 31;

    return (u8)result;

}

// TODO check
int create_shadow(int index, u32 width, u32 height){

    u32 img_w = imageList[index]->w;
    u32 img_h = imageList[index]->h;

    u32 startx = (img_w - width) / 2;
    u32 starty = (img_h - height) / 2;

    startx += 30;
    starty += 35;

    width  -= 30;
    height -= 35;

    width  -= 32;
    height -= 32;

    u32 endx = width + startx;
    u32 endy = height + starty;

    if (endx >= img_w)
        endx = img_w - 1;
    if (endy >= img_h)
        endy = img_h - 1;

    if (startx >= endx || starty >= endy)
        return 1;

    double factor = 3;

    u8* buffer = NULL;
    u32 leveys;

	if (drawimage_start(index, buffer, leveys) != 0)
		return 2;

	for (u32 y = starty; y < endy; y++) {

		u32 my = y * leveys;

		for(u32 x = startx; x < endx; x++) {

			u32 mx = x + my;

			u8 color = buffer[mx];
			
            u8 color2 = 192; //Turquoise
            color %= 32;

			if (x == startx || x == endx-1 || y == starty || y == endy-1)
				color = int((double)color / (factor / 1.5));
			else
				color = int((double)color / factor);

			color += color2;

			buffer[mx] = color;

		}

		if (factor > 1.005)
			factor = factor - 0.005;
	}

	if (drawimage_end(index) != 0)
		return 2;

	return 0;
    
}

int font_create(int image, int x, int y, int char_w, int char_h, int count) {

    int index = findfreefont();
    if (index == -1) {

        PLog::Write(PLog::ERR, "PDraw", "font_create got index -1");
        return -1;
    
    }

    fontList[index] = new PFont(image, x, y, char_w, char_h, count);
    return index;

}

int font_create(PFile::Path path) {
    
    int index = findfreefont();
    if (index == -1) {

        PLog::Write(PLog::ERR, "PDraw", "font_create got index -1");
        return -1;
    
    }

    fontList[index] = new PFont();

    if (fontList[index]->load(path) == -1) {

        PLog::Write(PLog::ERR, "PDraw", "Can't load a font from file!");
        delete fontList[index];
        fontList[index] = nullptr;
        return -1;
    
    }

    PLog::Write(PLog::DEBUG, "PDraw", "Created font from %s - id %i", path.c_str(), index);
    
    return index;

}

int font_write(int font_index, const char* text, int x, int y) {

    if (font_index < 0)
        return 1;
    
    return fontList[font_index]->write(x, y, text);

}

int font_writealpha(int font_index, const char* text, int x, int y, int alpha) {

    return fontList[font_index]->write_trasparent(x + x_offset, y + y_offset, text, alpha);

}

void set_buffer_size(int w, int h) {

    if (frameBuffer8->w == w && frameBuffer8->h == h)
        return;
    
    SDL_FreeSurface(frameBuffer8);
    
    frameBuffer8 = SDL_CreateRGBSurface(0, w, h, 8, 0, 0, 0, 0);
    SDL_SetPixelFormatPalette(frameBuffer8->format, game_palette);
    SDL_SetColorKey(frameBuffer8, SDL_TRUE, 255);

    set_offset(offset_width, offset_height);
    
}

void get_buffer_size(int* w, int* h) {

    *w = frameBuffer8->w;
    *h = frameBuffer8->h;

}

void get_offset(int* x, int* y) {

    *x = x_offset;
    *y = y_offset;

}

void set_offset(int width, int height) {

    offset_width = width;
    offset_height = height;

    if (width < frameBuffer8->w) {

        x_offset = (frameBuffer8->w - width) / 2;

    } else {

        x_offset = 0;

    }

    if (height < frameBuffer8->h) {

        y_offset = (frameBuffer8->h - height) / 2;

    } else {

        y_offset = 0;

    }

}

int init(int width, int height) {

    if (ready) return -1;

    PLog::Write(PLog::DEBUG, "PDraw", "Initializing graphics");
    PLog::Write(PLog::DEBUG, "PDraw", "Video driver: %s", SDL_GetCurrentVideoDriver());
    
    IMG_Init(IMG_INIT_PNG);

    if (game_palette == NULL)
        game_palette = SDL_AllocPalette(256);

    frameBuffer8 = SDL_CreateRGBSurface(0, width, height, 8, 0, 0, 0, 0);
    SDL_SetPixelFormatPalette(frameBuffer8->format, game_palette);
    SDL_SetColorKey(frameBuffer8, SDL_TRUE, 255);

    SDL_SetClipRect(frameBuffer8, NULL);
    SDL_FillRect(frameBuffer8, NULL, 255);

    ready = true;
    return 0;

}

void clear_fonts() {

    int size = fontList.size();

    for (int i = 0; i < size; i++) {
        if (fontList[i] != nullptr)
            delete fontList[i];
        fontList[i] = NULL;
    }
}

int terminate(){
    if (!ready) return -1;

    int size = imageList.size();

    for (int i = 0; i < size; i++)
        if (imageList[i] != NULL) {
            int j = i;
            image_delete(j);
        }

    clear_fonts();

    SDL_FreeSurface(frameBuffer8);

    if (game_palette->refcount != 1)
        PLog::Write(PLog::ERR, "PDraw", "Missing some palette reference");

    SDL_FreePalette(game_palette);

    IMG_Quit();

    ready = false;
    return 0;

}

void get_buffer_data(void** _buffer8) {

    frameBuffer8->userdata = &alpha;
    *_buffer8 = frameBuffer8;

}

void update() {

    if (is_fading()) {

        alpha += fade_speed;
        if(alpha < 0) alpha = 0;
        if(alpha > 100) alpha = 100;
        update_palette_alpha();
    
    }

    SDL_FillRect(frameBuffer8, NULL, 0);

}


}
