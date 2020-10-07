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

const char* window_name;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Surface* window_icon; 

SDL_Surface* frameBuffer8 = NULL;
SDL_Palette* game_palette = NULL;

// All images have its original palette on
// 'userdata' and game_palette on 'palette'.
// The original palette is stored to properly
// free the image on the end. 
std::vector<SDL_Surface*> imageList;
std::vector<PFont*> fontList;

std::vector<Gui*> gui_textures;

int screen_width;
int screen_height;

SDL_Rect screen_dest = {0, 0, 0, 0};
bool screen_fit = false;

bool ready = false;

int fade_speed = 0;
int alpha = 100;

int x_offset = 0;

Gui* create_gui(PFile::Path path, int x, int y, int w, int h, int alpha) {

    SDL_Texture* tex = NULL;

    if (path.GetFileName().size() > 0) {

        PFile::RW* rw = path.GetRW("rb");
        tex = IMG_LoadTexture_RW(renderer, (SDL_RWops*)rw, 0);
        PFile::CloseRW(rw);

        if (tex == NULL) {

            PLog::Write(PLog::ERR, "PDraw", "Can't load gui texture %s", path.c_str());

        }

    }

    Gui* gui = new Gui;

    gui->texture = tex;
    gui->x = x;
    gui->y = y;
    gui->w = w;
    gui->h = h;
    
    if (alpha > 255)
        gui->alpha = 255;
    else
        gui->alpha = alpha;
    
    gui->active = false;

    SDL_SetTextureAlphaMod(tex, gui->alpha);

    gui_textures.push_back(gui);
    return gui;

}

int remove_gui(Gui* gui) {

    if (!gui)
        return -1;

    auto rem = std::remove(gui_textures.begin(), gui_textures.end(), gui);
    gui_textures.erase(rem, gui_textures.end());

    if (gui->texture != NULL)
        SDL_DestroyTexture((SDL_Texture*)gui->texture);

    delete gui;
    

    return 0;

}

int draw_gui() {

	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);
	
	float prop_x = (float)w / 1920;
	float prop_y = (float)h / 1080;

	for(Gui* gui : gui_textures){

		if(gui->active && gui->texture != NULL) {

            SDL_Rect rect;
            rect.x = gui->x * prop_x;
            rect.y = gui->y * prop_y;
            rect.w = gui->w * prop_x;
            rect.h = gui->h * prop_y;

            u8 mod = (alpha * 255) / 100;
            //SDL_SetTextureAlphaMod((SDL_Texture*)gui->texture, mod);
            SDL_SetTextureColorMod((SDL_Texture*)gui->texture, mod, mod, mod);
            SDL_RenderCopy(renderer, (SDL_Texture*)gui->texture, NULL, &rect);
            
		}

	}
    
    return 0;

}

int findfreeimage() {
    int size = imageList.size();

    for(int i = 0; i < size; i++)
        if(imageList[i] == nullptr)
            return i;

    imageList.push_back(nullptr);
    return size;
}
int findfreefont(){
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
    return 0;
}
int  fade_in(int speed){
    alpha = 0;
    fade_speed = speed;
    return 0;
}
void rotate_palette(u8 start, u8 end){
    u8 i;
    SDL_Color* game_colors = game_palette->colors;
    SDL_Color temp_color = game_colors[end];

    for (i=end;i>start;i--)
        game_colors[i] = game_colors[i-1];

    game_colors[start] = temp_color;
}

int image_new(int w, int h){
    int index = findfreeimage();
    if (index == -1) return -1;

    imageList[index] = SDL_CreateRGBSurface(0, w, h, 8, 0, 0, 0, 0);

    SDL_SetColorKey(imageList[index], SDL_TRUE, 255);
    SDL_FillRect(imageList[index], NULL, 255);

    imageList[index]->userdata = (void*)imageList[index]->format->palette;
    imageList[index]->format->palette = game_palette;

    return index;
}
int image_load(PFile::Path path, bool getPalette) {

    int index = findfreeimage();

    if (index == -1) {

        PLog::Write(PLog::ERR, "PDraw", "image_load got index -1");
        return -1;
    
    }

    PFile::RW* rw = path.GetRW("rb");
    if (!rw) {

        PLog::Write(PLog::ERR, "PDraw", "Couldn't find %s", path.c_str());
        return -1;

    }

    imageList[index] = IMG_Load_RW((SDL_RWops*) rw, 0);
    PFile::CloseRW(rw);

    if (imageList[index] == NULL) {

        PLog::Write(PLog::ERR, "PDraw", "Couldn't load %s", path.c_str());
        return -1;
    
    }

    if(imageList[index]->format->BitsPerPixel != 8) {

        PLog::Write(PLog::ERR, "PDraw", "Failed to open %s, just 8bpp indexed images!", path.c_str());
        image_delete(index);
        return -1;
    
    }

    SDL_SetColorKey(imageList[index], SDL_TRUE, 255);

    if(getPalette) {

        SDL_Palette* pal = imageList[index]->format->palette;
        
        for( int i = 0; i < 256; i++)
            game_palette->colors[i] = pal->colors[i];
    
    }

    imageList[index]->userdata = (void*)imageList[index]->format->palette; //Put allocated pallete in userdata
    imageList[index]->format->palette = game_palette;

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

    SDL_SetColorKey(imageList[index], SDL_TRUE, 255);
    SDL_FillRect(imageList[index], NULL, 255);

    imageList[index]->userdata = (void*)imageList[index]->format->palette;
    imageList[index]->format->palette = game_palette;

    SDL_BlitScaled(imageList[ImgIndex], (SDL_Rect*)&area, imageList[index], NULL);

    return index;

}

int image_clip(int index, int x, int y) {

    SDL_Rect dstrect;

    dstrect.x = x + x_offset;
    dstrect.y = y;
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

    RECT src = {(u32)srcx, (u32)srcy, (u32)oikea-srcx, (u32)ala-srcy};
    RECT dst = {(u32)dstx, (u32)dsty, (u32)oikea-srcx, (u32)ala-srcy};
    image_cutclip(index, src, dst);

    return 0;

}

int image_cutclip(int index, RECT srcrect, RECT dstrect) {

    dstrect.x += x_offset;
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

    if (alpha > 100) alpha = 100;
    if (alpha <= 0) alpha = 0;

    int x_start = src_x;
    if (dst_x < 0)
        x_start -= dst_x;

    int x_end = src_x + src_w;
    if (int(dst_x + src_w) > screen_width)
        x_end -= src_w - screen_width + dst_x;

    int y_start = src_y;
    if (dst_y < 0)
        y_start -= dst_y;

    int y_end = src_y + src_h;
    if (dst_y + src_h > screen_height)
        y_end -= src_h - screen_height + dst_y;

    if (x_start >= x_end || y_start >= y_end) return -1;    
    
    u8 *imagePix = nullptr;
    u8 *screenPix = nullptr;
    u32 imagePitch, screenPitch;

    drawimage_start(index, *&imagePix, (u32 &)imagePitch);
    drawscreen_start(*&screenPix, (u32 &)screenPitch);
    
    for (int posx = x_start; posx < x_end; posx++)
        for (int posy = y_start; posy < y_end; posy++) {

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

    SDL_FillRect(imageList[index], NULL, 0); //needed?

    return SDL_BlitSurface(frameBuffer8, NULL, imageList[index], NULL);

}

int image_delete(int& index) {

    if(index < 0)
        return -1;
    
    if (imageList[index] == NULL)
        return -1;
    
    imageList[index]->format->palette = (SDL_Palette*)imageList[index]->userdata; //Return to the original pallete
    
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

    SDL_Rect r = {posx + x_offset, posy, oikea-posx, ala-posy};
    return SDL_FillRect(frameBuffer8, &r, color);

}

void set_mask(int x, int y, int w, int h) {

    SDL_Rect r = {x, y, w, h};
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

u8 blend_colors(u8 color, u8 colBack, int alpha) {
    
    if(alpha > 100) alpha = 100;
    if(alpha < 0) alpha = 0;

    int result = color % 32;
    result = (result*alpha) / 100;
    result += colBack % 32;
    if(result > 31) result = 31;

    return (u8)result;

}

int create_shadow(int index, u32 width, u32 height, int startx){
	
    u8* buffer = NULL;
    u32 leveys;

	if (drawimage_start(index, *&buffer, (u32&)leveys) != 0)
		return 1;

	if (width > leveys)
		width = leveys;

	height -= 2;
	width  -= 2;

	width += startx - 30;

	double factor = 3;

	for (u32 y = 35; y < height - 30; y++) {

		u32 my = y * leveys;

		for(u32 x = startx; x < width - 30; x++) {

			u32 mx = x + my;

			u8 color = buffer[mx];
			
            u8 color2 = 192; //Turquoise
            color %= 32;

			if (x == startx || x == width-31 || y == 35 || y == height-31)
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
		return 1;

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

    return fontList[font_index]->write_trasparent(x + x_offset, y, text, alpha);

}

int set_filter(const char* filter) {

    if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, filter) == SDL_TRUE)
        return 0;

    return 1;
}

void set_fullscreen(bool set) {
    #ifndef __ANDROID__
    if(set)
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    else {
        SDL_SetWindowFullscreen(window, 0);
        SDL_SetWindowSize(window, screen_width, screen_height);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED);
        //TODO - adjust dst_rect too and turn off filters
    }

    adjust_screen();
    #endif
}

void adjust_screen() {

    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    float screen_prop = (float)w / h;
    float buff_prop   = (float)screen_width / screen_height;

    if (buff_prop > screen_prop) {

        screen_dest.w = w;
        screen_dest.h = (int)(h / buff_prop);
        screen_dest.x = 0;
        screen_dest.y = (int)((h - screen_dest.h) / 2);
    
    }else {

        screen_dest.w = (int)(buff_prop * h);
        screen_dest.h = h;
        screen_dest.x = (int)((w - screen_dest.w) / 2);
        screen_dest.y = 0;

    }
}

void fit_screen(bool fit) {

    screen_fit = fit;

}

void change_resolution(int w, int h) {

    if (w == screen_width && h == screen_height)
        return;

    frameBuffer8->format->palette = (SDL_Palette *)frameBuffer8->userdata;
    SDL_FreeSurface(frameBuffer8);
    
    frameBuffer8 = SDL_CreateRGBSurface(0, w, h, 8, 0, 0, 0, 0);
    frameBuffer8->userdata = (void *)frameBuffer8->format->palette;
    frameBuffer8->format->palette = game_palette;

    SDL_SetColorKey(frameBuffer8, SDL_TRUE, 255);
    SDL_FillRect(frameBuffer8, NULL, 255);

    //SDL_Rect r = {0, 0, w, h};
    //SDL_SetClipRect(frameBuffer8, &r);

    screen_width = w;
    screen_height = h;
    SDL_SetWindowSize(window, w, h);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    adjust_screen();
    SDL_RenderClear(renderer);

}

int get_resolution(int* w, int* h) {

    SDL_GetWindowSize(window, w, h);

    return 0;

}

int get_buffer_size(int* w, int* h) {

    *w = screen_width;
    *h = screen_height;

    return 0;

}

void get_windowposition(int* x, int* y) {

    SDL_GetWindowPosition(window, x, y);

}

int get_xoffset() {

    return x_offset;

}

//TODO - change to set_wide
void set_xoffset(int x) {

    x_offset = x;

    //SDL_Rect r = {x, 0, screen_width-2*x, screen_height};
    //SDL_SetClipRect(frameBuffer8, &r);

}

int init(int width, int height, const char* name, const char* icon) {

    if (ready) return -1;

    if (game_palette == NULL) {
        
        game_palette = SDL_AllocPalette(256);

        u8 i = 0;
        do game_palette->colors[i] = { i, i, i, i };
        while(i++ != 255);

    }

    window_name = name;

    #ifdef __ANDROID__

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto Width = DM.w;
    auto Height = DM.h;
    auto x = Height;
    if(Width < Height){
        Height = Width;
        Width = x;
    }

    window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_SHOWN);

    #else

    window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN );

    window_icon = SDL_LoadBMP(icon);
    SDL_SetWindowIcon(window, window_icon);
    SDL_ShowCursor(SDL_DISABLE);

    #endif

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
    SDL_RenderClear(renderer);

    frameBuffer8 = SDL_CreateRGBSurface(0, width, height, 8, 0, 0, 0, 0);
    frameBuffer8->userdata = (void *)frameBuffer8->format->palette;
    frameBuffer8->format->palette = game_palette;
    SDL_SetColorKey(frameBuffer8, SDL_TRUE, 255);
    SDL_FillRect(frameBuffer8, NULL, 255);

    //SDL_Rect r = {0, 0, width, height};
    //SDL_SetClipRect(frameBuffer8, &r);

    screen_width = width;
    screen_height = height;
    adjust_screen();

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

    SDL_FreePalette(game_palette);

    frameBuffer8->format->palette = (SDL_Palette *)frameBuffer8->userdata;
    SDL_FreeSurface(frameBuffer8);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    if(window_icon != NULL) SDL_FreeSurface(window_icon);

    ready = false;
    return 0;

}

void update(bool draw) {
    if(!ready) return;

    if(draw) {
        
        SDL_Texture* texture;
        u8 alpha2 = (u8)(alpha*255/100);

        texture = SDL_CreateTextureFromSurface(renderer,frameBuffer8);
        SDL_SetTextureColorMod(texture,alpha2,alpha2,alpha2);

        SDL_RenderClear(renderer);

        if(screen_fit)
            SDL_RenderCopy(renderer, texture, NULL, NULL);
        else
            SDL_RenderCopy(renderer, texture, NULL, &screen_dest);

        draw_gui();

        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(texture);
    
    }

    if (is_fading()) {

        alpha += fade_speed;
        if(alpha < 0) alpha = 0;
        if(alpha > 255) alpha = 255;
    
    }

    //SDL_Rect r = {0, 0, x_offset, screen_height}; // Fill the unused borders
    //SDL_FillRect(frameBuffer8, &r, 0);
    //r.x = screen_width - x_offset;
    //SDL_FillRect(frameBuffer8, &r, 0);
    SDL_FillRect(frameBuffer8, NULL, 0);

}


}
