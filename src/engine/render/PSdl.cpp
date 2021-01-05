//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PRender.hpp"

#include "engine/PLog.hpp"

#include "engine/render/PSdl.hpp"

#include <SDL.h>

void PSdl::clear_screen() {

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

}

void PSdl::set_screen(PRender::FRECT screen_dst) {

    int w, h;
    PRender::get_window_size(&w, &h);

    screen_dest.x = screen_dst.x * w;
    screen_dest.y = screen_dst.y * h;
    screen_dest.w = screen_dst.w * w;
    screen_dest.h = screen_dst.h * h;

}

int PSdl::set_mode(int mode) {

    if (mode == PRender::MODE_NEAREST) {
        if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0") == SDL_TRUE)
            return 0;
    } else if (mode == PRender::MODE_LINEAR) {
        if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2") == SDL_TRUE)
            return 0;
    }
    
    return 1;
}

int PSdl::set_vsync(bool set) {

    if (renderer) {

        SDL_DestroyRenderer(renderer);

    }

    Uint32 sync = set? SDL_RENDERER_PRESENTVSYNC : 0;

    renderer = SDL_CreateRenderer(curr_window, -1, SDL_RENDERER_ACCELERATED | sync);
    if (!renderer) {

        PLog::Write(PLog::FATAL, "PSdl", "Couldn't create renderer!");
        return -1;

    }

    SDL_RenderClear(renderer);

    return 0;

}

void PSdl::update(void* _buffer8, int alpha) {

    SDL_Surface* buffer8 = (SDL_Surface*)_buffer8;
    
    SDL_Texture* texture;
    u8 alpha2 = (u8)(alpha*255/100);

    texture = SDL_CreateTextureFromSurface(renderer, buffer8);
    SDL_SetTextureColorMod(texture,alpha2,alpha2,alpha2);

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, NULL, &screen_dest);

    //draw_gui();

    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);

}

PSdl::PSdl(int width, int height, void* window) {

    curr_window = (SDL_Window*)window;

    renderer = SDL_CreateRenderer(curr_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {

        PLog::Write(PLog::FATAL, "PSdl", "Couldn't create renderer!");

    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    SDL_RenderClear(renderer);

}

PSdl::~PSdl() {

    SDL_DestroyRenderer(renderer);
    PLog::Write(PLog::DEBUG, "PSdl", "Terminated");

}
