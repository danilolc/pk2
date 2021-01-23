//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PRender.hpp"

#include "engine/PLog.hpp"

#include "engine/render/PSdl.hpp"

#include <SDL.h>

void* PSdl::create_texture(void* surface) {

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, (SDL_Surface*)surface);
    if (tex == NULL) {

        PLog::Write(PLog::ERR, "PSdl", "Couldn't load texture!");

    }

    return tex;

}
void PSdl::remove_texture(void* texture) {

    SDL_DestroyTexture((SDL_Texture*) texture);

}
void PSdl::render_texture(void* texture, float x, float y, float w, float h, float alpha) {

    RenderOptions opt;
    opt.texture = texture;
    opt.x = x;
    opt.y = y;
    opt.w = w;
    opt.h = h;
    opt.alpha = alpha;

    render_list.push_back(opt);

}

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

int PSdl::set_shader(int mode) {

    if (mode == PRender::SHADER_NEAREST) {
        if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0") == SDL_TRUE)
            return 0;
    } else if (mode == PRender::SHADER_LINEAR) {
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

void PSdl::update(void* _buffer8) {

    SDL_Surface* buffer8 = (SDL_Surface*)_buffer8;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, buffer8);

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, NULL, &screen_dest);
    
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    float prop_x = (float)w;
    float prop_y = (float)h;
    
    for (auto opt : render_list) {

        SDL_Texture* tex = (SDL_Texture*)opt.texture;
        if (tex == NULL)
            continue;
        
        u8 mod = opt.alpha * 256;
        if (mod == 0)
            continue;
        
        SDL_SetTextureAlphaMod(tex, mod);
        
        SDL_Rect rect;
        rect.x = opt.x * prop_x;
        rect.y = opt.y * prop_y;
        rect.w = opt.w * prop_x;
        rect.h = opt.h * prop_y;

        SDL_RenderCopy(renderer, tex, NULL, &rect);
        
    }

    render_list.clear();

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
