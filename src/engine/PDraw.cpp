//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "PDraw.hpp"

#include "PFont.hpp"
#include "PLog.hpp"

#include <algorithm>
#include <vector>
#include <array>

#include <SDL_image.h>

namespace PDraw {

// 8-bit indexed surface, it's the game frame buffer
static SDL_Surface* frameBuffer8 = NULL;

// All surfaces will have this palette
// game_colors stores the original colors,
// without alpha modification
static SDL_Palette* game_palette = NULL;
//static SDL_Color game_colors[256];
//static SDL_Color buff_colors[256];

static std::vector<SDL_Surface*> imageList;
static std::vector<PFont*> fontList;

static bool ready = false;

static int x_offset = 0;
static int y_offset = 0;

static int offset_width = 0;
static int offset_height = 0;

#define IS_UNITY(X) ((X > 0.99) && (X < 1.01))


class Palette{
public:
    SDL_Color colors[256];
    float r = 1;
    float g = 1;
    float b = 1;

    int updateEffect();
    void rotate(u8 start, u8 end);
    void setRGB(float r, float g, float b){
        this->r = r;
        this->g = g;
        this->b = b;
        this->updateEffect();
    }
};

static int mCurrentpaletteIndex = -1;
static std::vector<Palette*> paletteList;

int Palette::updateEffect(){
    SDL_Color buff_colors[256];

    for (int i = 0; i < 256; i++) {

        int r = this->colors[i].r * this->r;
        int g = this->colors[i].g * this->g;
        int b = this->colors[i].b * this->b;

        if (r > 255) r = 255;
        if (r < 0)   r = 0;
        if (g > 255) g = 255;
        if (g < 0)   g = 0;
        if (b > 255) b = 255;
        if (b < 0)   b = 0;

        buff_colors[i].r = r;
        buff_colors[i].g = g;
        buff_colors[i].b = b;
        buff_colors[i].a = 255;
    }

    return SDL_SetPaletteColors(game_palette, buff_colors, 0, 256);
}

void Palette::rotate(u8 start, u8 end){

    SDL_Color temp_color = this->colors[end];

    for (uint i = end; i > start; i--)
        this->colors[i] = this->colors[i-1];

    this->colors[start] = temp_color;
    this->updateEffect();
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

static int findfreepalette(){
    int size = paletteList.size();

    for(int i = 0; i < size; i++)
        if(paletteList[i] == nullptr)
            return i;

    paletteList.push_back(nullptr);
    return size;
}

void palette_set(int index){
    if(index<0 || index>= (int)paletteList.size() || paletteList[index]==nullptr)return;
    mCurrentpaletteIndex = index;
    paletteList[index]->updateEffect();

}

void palette_delete(int& index){
    if(index<0 || index>= (int)paletteList.size() || paletteList[index]==nullptr)return;

    if(index==mCurrentpaletteIndex){
        mCurrentpaletteIndex = -1;
    }

    delete paletteList[index];
    paletteList[index] = nullptr;
}

void rotate_palette(u8 start, u8 end){
    paletteList[mCurrentpaletteIndex]->rotate(start, end);
}

void set_rgb(float r, float g, float b){
    paletteList[mCurrentpaletteIndex]->setRGB(r,g,b);
}

int image_new(int w, int h){
    int index = findfreeimage();
    imageList[index] = SDL_CreateRGBSurface(0, w, h, 8, 0, 0, 0, 0);
    SDL_SetSurfacePalette(imageList[index], game_palette);
    //SDL_SetColorKey(imageList[index], SDL_TRUE, 255);

    SDL_FillRect(imageList[index], NULL, 255);
    
    return index;
}


static int mLoadImage(PFile::Path path, bool hasAlphaColor){

    int index = findfreeimage();

    if (index == -1) {

        PLog::Write(PLog::ERR, "PDraw", "image_load got index -1");
        return -1;
    
    }

    try{
        PFile::RW rw = path.GetRW2("r");
        imageList[index] = IMG_Load_RW((SDL_RWops*)(rw._rwops), 0);
        rw.close();
    }
    catch(const PFile::PFileException& e){
        PLog::Write(PLog::ERR, "PDraw", e.what());
        PLog::Write(PLog::ERR, "PDraw", "Couldn't find %s", path.c_str());
        return -1;
    }

    if (imageList[index] == NULL) {

        PLog::Write(PLog::ERR, "PDraw", "Couldn't load %s, %s", path.c_str(), SDL_GetError());
        return -1;
    
    }

    if(imageList[index]->format->BitsPerPixel != 8) {

        PLog::Write(PLog::ERR, "PDraw", "Failed to open %s, just 8bpp indexed images!", path.c_str());
        image_delete(index);
        return -1;
    }

    if(hasAlphaColor){
        SDL_SetColorKey(imageList[index], SDL_TRUE, ALPHA_COLOR_INDEX);
    }

    return index;
}


int image_load(PFile::Path path, bool hasAlphaColor) {

    int index = mLoadImage(path, hasAlphaColor);
    if(index<0)return index;

    SDL_SetSurfacePalette(imageList[index], game_palette);
    return index;

}

std::pair<int, int> image_load_with_palette(PFile::Path path, bool hasAlphaColor){
    int index = mLoadImage(path, hasAlphaColor);
    if(index<0)return std::make_pair(-1, -1);

    int palIndex = findfreepalette();
    Palette* pal = new Palette();
    paletteList[palIndex] = pal;

    SDL_Palette* sdlPal = imageList[index]->format->palette;
    SDL_memcpy(pal->colors, sdlPal->colors, sizeof(SDL_Color) * 256);

    SDL_SetSurfacePalette(imageList[index], game_palette);
    return std::make_pair(index, palIndex);
}


int image_load(int& index, PFile::Path path, bool hasAlphaColor) {
    
    image_delete(index);
    index = image_load(path, hasAlphaColor);

    return index;

}

void image_load_with_palette(int& img_index, int& pal_index, PFile::Path path, bool hasAlphaColor){

    image_delete(img_index);
    palette_delete(pal_index);

    std::pair p = image_load_with_palette(path, hasAlphaColor);

    img_index = p.first;
    pal_index = p.second;
}

int image_copy(int image) {

    if(image < 0)
        return -1;
    
    SDL_Surface* im = imageList[image];

    int i = image_new(im->w, im->h);
    if(i < 0)
        return -1;
    
    SDL_BlitSurface(im, NULL, imageList[i], NULL);
    return i;

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

    if(game_palette!=nullptr){
        SDL_SetSurfacePalette(imageList[index], game_palette);
    }
    else{
        PLog::Write(PLog::WARN, "PDraw", "Cannot image_cut due to missing palette!");
        return -1;
    }
    
    SDL_SetColorKey(imageList[index], SDL_TRUE, ALPHA_COLOR_INDEX);
    SDL_FillRect(imageList[index], NULL, ALPHA_COLOR_INDEX);

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

    if(index < 0)
        return -1;

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

static u8 blend_colors(u8 color, u8 colBack, u8 alpha) {

    int result = color % 32;
    result = (result*alpha) / 256;
    result += colBack % 32;
    if(result > 31)
        result = 31;

    return (u8)result;

}

// TODO - REDO THIS FUNCTION
int image_cutcliptransparent(int index, int src_x, int src_y, int src_w, int src_h,
						 int dst_x, int dst_y, int alpha, u8 colorsum) {
    
    dst_x += x_offset;
    dst_y += y_offset;

    alpha *= 256;
    alpha /= 100;

    if (alpha > 255) alpha = 255;
    if (alpha <= 0) alpha = 0;
    u8 alpha8 = alpha;

    int x_start = src_x;
    if (dst_x < 0) x_start -= dst_x;

    int x_end = src_x + src_w;
    int dx = dst_x + (src_w - frameBuffer8->w);
    if (dx > int(x_end)) return -1;
    if (dx > 0) x_end -= dx;

    if (x_start >= x_end) return -1;


    int y_start = src_y;
    if (dst_y < 0) y_start -= dst_y;

    int y_end = src_y + src_h;
    int dy = dst_y + (src_h - frameBuffer8->h);
    if (dy > int(y_end)) return -1;
    if (dy > 0) y_end -= dy;

    if (y_start >= y_end) return -1;


    u8 *imagePix = nullptr;
    u8 *screenPix = nullptr;
    u32 imagePitch, screenPitch;

    drawimage_start(index, imagePix, imagePitch);
    drawscreen_start(screenPix, screenPitch);
    for (int posy = y_start; posy < y_end; posy++)
        for (int posx = x_start; posx < x_end; posx++)
         {

            u8 color1 = imagePix[ posx + imagePitch * posy ];
            if (color1 != 255) {

                int screen_x = posx + dst_x - src_x;
                int screen_y = posy + dst_y - src_y;
                
                int fy = screen_x + screenPitch * screen_y;

                u8 sum = colorsum;
                if (sum == 255)
                    sum = color1 & 0b11100000;

                u8 color2 = screenPix[fy];
                screenPix[fy] = blend_colors(color1, color2, alpha8) + sum;

            }

        }
    
    drawscreen_end();
    drawimage_end(index);
    
    return 0;

}

int image_cutclipmirror(int index, int src_x, int src_y, int src_w, int src_h,
    int dst_x, int dst_y);

int   image_clip_mirror(int index, int x, int y){

    return image_cutclipmirror(
        index,
        0,
        0,
        imageList[index]->w,
        imageList[index]->h, 
        x,
        y);
}

//experimental
int image_cutclipmirror(int index, int src_x, int src_y, int src_w, int src_h,
    int dst_x, int dst_y) {

    dst_x += x_offset;
    dst_y += y_offset;

    int x_start = src_x;
    if (dst_x < 0) x_start -= dst_x;

    int x_end = src_x + src_w;
    int dx = dst_x + (src_w - frameBuffer8->w);
    if (dx > int(x_end)) return -1;
    if (dx > 0) x_end -= dx;

    if (x_start >= x_end) return -1;


    int y_start = src_y;
    if (dst_y < 0) y_start -= dst_y;

    int y_end = src_y + src_h;
    int dy = dst_y + (src_h - frameBuffer8->h);
    if (dy > int(y_end)) return -1;
    if (dy > 0) y_end -= dy;

    if (y_start >= y_end) return -1;


    u8 *imagePix = nullptr;
    u8 *screenPix = nullptr;
    u32 imagePitch, screenPitch;

    drawimage_start(index, imagePix, imagePitch);
    drawscreen_start(screenPix, screenPitch);

    for (int posy = y_start; posy < y_end; posy++)
        for (int posx = x_start; posx < x_end; posx++){
        {
            u8 color1 = imagePix[ posx + imagePitch * posy ];
            if(color1 == 255)continue;

            int screen_x = posx + dst_x - src_x;
            int screen_y = posy + dst_y - src_y;
            int fy = screen_x + screenPitch * screen_y;

            if(color1 < 224 || color1 >= 240){                
                screenPix[fy] = imagePix[ posx + imagePitch * posy];
            }
            else if(color1 == 224){
                int y2 = dst_y - posy - src_y;

                if(y2>=0){
                    screenPix[fy] = screenPix[screen_x + screenPitch * y2];
                }
            }
            else if(color1 == 225){
                int x2 = dst_x - posx - src_x;
                if(x2 >= 0){
                    screenPix[fy] = screenPix[x2 + screenPitch * screen_y];
                }
            }
            else if(color1 == 226){

                int x2 = 2*x_end + dst_x - posx - src_x;
                if(x2 < frameBuffer8->w){
                    screenPix[fy] = screenPix[x2 + screenPitch * screen_y];
                }
            }

            else if(color1 == 225){
                int x2 = dst_x - posx - src_x;
                if(x2 >= 0){
                    screenPix[fy] = screenPix[x2 + screenPitch * screen_y];
                }
            }
            else if(color1 == 227){

                int y2 = 2*y_end + dst_y - posy - src_y;
                if(y2 < frameBuffer8->h){
                    screenPix[fy] = screenPix[screen_x + screenPitch * y2];
                }
            }
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

    if (index < 0)
        return -1;
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

std::pair<int, int> font_get_text_size(int font_index, const std::string& text){
    if (font_index < 0 || font_index >= (int)fontList.size())
        return std::make_pair(0, 0);
    
    return fontList[font_index]->getTextSize(text.c_str());
}

int font_write_line(int font_index, const std::string& text, int x, int y) {

    if (font_index < 0 || font_index >= (int)fontList.size())
        return 0;
    
    return fontList[font_index]->write_line(x, y, text.c_str());

}

std::pair<int, int> font_write(int font_index, const std::string& text, int x, int y) {

    if (font_index < 0 || font_index >= (int)fontList.size())
        return std::make_pair(0,0);
    
    return fontList[font_index]->write(x, y, text.c_str());

}

std::pair<int, int> font_writealpha_s(int font_index, const std::string& text, int x, int y, int alpha, int blendMode) {
    if (font_index < 0 || font_index >= (int)fontList.size())
        return std::make_pair(0,0);

    return fontList[font_index]->write_trasparent(x + x_offset, y + y_offset, text.c_str(), alpha, blendMode);

}

bool font_accept_char(int font_index, PString::UTF8_Char u8c){
    if (font_index < 0 || font_index >= (int)fontList.size())
        return false;

    return fontList[font_index]->acceptChar(u8c);
}

void set_buffer_size(int w, int h) {

    if (frameBuffer8->w == w && frameBuffer8->h == h)
        return;
    
    SDL_FreeSurface(frameBuffer8);
    
    frameBuffer8 = SDL_CreateRGBSurface(0, w, h, 8, 0, 0, 0, 0);
    SDL_SetSurfacePalette(frameBuffer8, game_palette);
    //SDL_SetColorKey(frameBuffer8, SDL_TRUE, 255);

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

    PLog::Write(PLog::DEBUG, "PDraw", "Initializing buffers");
    
    IMG_Init(IMG_INIT_PNG);

    if (game_palette == NULL)
        game_palette = SDL_AllocPalette(256);

    frameBuffer8 = SDL_CreateRGBSurface(0, width, height, 8, 0, 0, 0, 0);
    SDL_SetSurfacePalette(frameBuffer8, game_palette);
    //SDL_SetColorKey(frameBuffer8, SDL_TRUE, 255);

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

    //for(int i =0)
    SDL_FreePalette(game_palette);

    for(int i=0;i<(int)paletteList.size();++i){
        palette_delete(i);
    }

    IMG_Quit();

    ready = false;
    return 0;

}

void get_buffer_data(void** _buffer8) {

    *_buffer8 = frameBuffer8;

}

void update() {

    SDL_FillRect(frameBuffer8, NULL, 0);

}


void  take_screenshot(const std::string& name){
    SDL_LockSurface(frameBuffer8);
    IMG_SavePNG(frameBuffer8, name.c_str());
    SDL_UnlockSurface(frameBuffer8);
}


}
