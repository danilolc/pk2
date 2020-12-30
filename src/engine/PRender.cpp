//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PRender.hpp"

#include "engine/PDraw.hpp"
#include "engine/PLog.hpp"
#include "engine/types.hpp"

#include <SDL.h>

namespace PRender {

static SDL_Rect screen_dest = {0, 0, 0, 0};
static bool screen_fit = false;

static const char* window_name;
static SDL_Window* window = NULL;

static SDL_Renderer* renderer = NULL;

static bool vsync_set = true;

void set_fullscreen(bool set) {

    #ifndef __ANDROID__
    if(set)
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    else {
        int buf_w, buf_h;
        PDraw::get_buffer_size(&buf_w, &buf_h);

        SDL_SetWindowFullscreen(window, 0);
        SDL_SetWindowSize(window, buf_w, buf_h);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED);
        //TODO - adjust dst_rect too and turn off filters
    }

    adjust_screen();
    #endif

}

void adjust_screen() {

    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    int buf_w, buf_h;
    PDraw::get_buffer_size(&buf_w, &buf_h);

    float screen_prop = (float)w / h;
    float buff_prop   = (float)buf_w / buf_h;

    if (buff_prop > screen_prop) {

        screen_dest.w = w;
        screen_dest.h = (int)(h / buff_prop);
        screen_dest.x = 0;
        screen_dest.y = (int)((h - screen_dest.h) / 2);
    
    } else {

        screen_dest.w = (int)(buff_prop * h);
        screen_dest.h = h;
        screen_dest.x = (int)((w - screen_dest.w) / 2);
        screen_dest.y = 0;

    }
}

void change_window_size(int w, int h) {

    SDL_SetWindowSize(window, w, h);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    adjust_screen();

}

int set_filter(const char* filter) {

    if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, filter) == SDL_TRUE)
        return 0;

    return 1;
}

void get_window_size(int* w, int* h) {

    SDL_GetWindowSize(window, w, h);

}

void get_window_position(int* x, int* y) {

    SDL_GetWindowPosition(window, x, y);

}

void fit_screen(bool fit) {

    screen_fit = fit;

}

int set_vsync(bool set) {

    #ifndef PK2_NO_THREAD

    if (set == vsync_set)
        return 0;

    if (renderer) {

        SDL_DestroyRenderer(renderer);

    }

    Uint32 sync = set? SDL_RENDERER_PRESENTVSYNC : 0;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | sync);
    if (!renderer) {

        PLog::Write(PLog::FATAL, "PRender", "Couldn't create renderer!");
        return -1;

    }

    vsync_set = set;

    SDL_RenderClear(renderer);
    return 0;

    #else

    return -1;

    #endif

}

bool is_vsync() {

    return vsync_set;
    
}

int init(int width, int height, const char* name, const char* icon) {

    window_name = name;

    window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {

        PLog::Write(PLog::FATAL, "PRender", "Couldn't create window!");
        return -2;

    }

    #ifndef __ANDROID__

    SDL_Surface* window_icon = SDL_LoadBMP(icon);
    if (window_icon) {
        SDL_SetWindowIcon(window, window_icon);
        SDL_FreeSurface(window_icon);
    }
    
    SDL_ShowCursor(SDL_DISABLE);
    
    #endif

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {

        PLog::Write(PLog::FATAL, "PRender", "Couldn't create renderer!");
        return -3;

    }

    SDL_RenderClear(renderer);
    adjust_screen();

}

void terminate() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

}

void update(void* _buffer8, int alpha) {

    SDL_Surface* buffer8 = (SDL_Surface*)_buffer8;
    
    SDL_Texture* texture;
    u8 alpha2 = (u8)(alpha*255/100);

    texture = SDL_CreateTextureFromSurface(renderer, buffer8);
    SDL_SetTextureColorMod(texture,alpha2,alpha2,alpha2);

    SDL_RenderClear(renderer);

    if(screen_fit)
        SDL_RenderCopy(renderer, texture, NULL, NULL);
    else
        SDL_RenderCopy(renderer, texture, NULL, &screen_dest);

    //draw_gui();

    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);

}

}