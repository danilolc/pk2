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

	void* create_texture(void* surface) override;
	void remove_texture(void* texture) override;
	void render_texture(void* texture, float x, float y, float w, float h, float alpha) override;

    void clear_screen() override;
    void set_screen(PRender::FRECT screen_dst) override;
    int  set_shader(int mode) override;
    int  set_vsync(bool set) override;
    void update(void* buffer8) override;

    PSdl(int width, int height, void* window);
    ~PSdl();

private:

    SDL_Window* curr_window;
    SDL_Renderer* renderer;

    SDL_Rect screen_dest;
    
    struct RenderOptions {
        void* texture;
        float x, y, w, h, alpha;
    };

    std::vector<RenderOptions> render_list;

};