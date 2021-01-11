//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PRender.hpp"

#include "engine/PLog.hpp"

#include "engine/render/PSdlSoft.hpp"

#include <SDL.h>

void PSdlSoft::clear_screen() {

    SDL_Surface* sur = SDL_GetWindowSurface(curr_window);

    SDL_FillRect(sur, NULL, 0);

    SDL_UpdateWindowSurface(curr_window);

}

void PSdlSoft::set_screen(PRender::FRECT screen_dst) {

    // Does nothing

}

int PSdlSoft::set_shader(int mode) {

    if (mode != PRender::SHADER_LINEAR)
        return 1;
    
    return 0;

}

int PSdlSoft::set_vsync(bool set) {

    if (set)
        return -1;

    return 0;

}

void PSdlSoft::update(void* _buffer8, int alpha) {

    SDL_Surface* buffer8 = (SDL_Surface*)_buffer8;

    SDL_Surface* sur = SDL_GetWindowSurface(curr_window);

    SDL_Rect dst;
    dst.x = (sur->w - buffer8->w) / 2;
    dst.y = (sur->h - buffer8->h) / 2;
    dst.w = buffer8->w;
    dst.h = buffer8->h;

    SDL_BlitSurface(buffer8, NULL, sur, &dst);

    SDL_UpdateWindowSurface(curr_window);

}

PSdlSoft::PSdlSoft(int width, int height, void* window) {

    curr_window = (SDL_Window*)window;
    clear_screen();

}

PSdlSoft::~PSdlSoft() {

    PLog::Write(PLog::DEBUG, "PSdlSoft", "Terminated");

}
