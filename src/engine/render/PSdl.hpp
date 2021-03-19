//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PRender.hpp"

#include <SDL.h>
#include <vector>

class PSdl : public PRender::Renderer {

public:

	void load_ui_texture(void* surface) override;
	void render_ui(PRender::FRECT src, PRender::FRECT dst, float alpha) override;

    void clear_screen() override;
    void set_screen(PRender::FRECT screen_dst) override;
    int  set_shader(int mode) override;
    int  set_vsync(bool set) override;
    void update(void* buffer8) override;

    PSdl(int width, int height, void* window);
    ~PSdl();

private:

    SDL_Window* curr_window = NULL;
    SDL_Renderer* renderer = NULL;

    SDL_Texture* ui_texture = NULL;
    SDL_Surface* ui_surface = NULL;

    SDL_Rect screen_dest;
    
    struct RenderOptions {
        PRender::FRECT src, dst;
        float alpha;
    };

    std::vector<RenderOptions> render_list;

};