//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PRender.hpp"
#include "engine/render/PGlFuncs.hpp"

#include <SDL.h>

class PGl : public PRender::Renderer {

public:

	void load_ui_texture(void* surface) override;
	void render_ui(PRender::FRECT src, PRender::FRECT dst, float alpha) override;

    void clear_screen() override;
    void set_screen(PRender::FRECT screen_dst) override;
    int  set_shader(int mode) override;
    int  set_vsync(bool set) override;
    void update(void* buffer8) override;

    PGl(int width, int height, void* window);
    ~PGl();

private:

    const GLint VBO_LOCATION = 0;
    const GLint UBO_LOCATION = 1;

    const GLfloat ubo_data[12] = {
        0.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f
    };

    SDL_Window* curr_window;
    SDL_GLContext context;

    Uint32 last_palette_version = -1;

    GLchar Log_message[1024];

    GLuint vao, vbo, ubo;

    GLfloat shader_time = 0;
    const GLfloat time_cycle = M_PI * 3 * 4 * 5;// * 7;

    // HQX shader
    bool hqx_ok = false;

    GLuint hqx_vs;
    GLuint hqx_fs;
    GLuint hqx_program;

    GLint  uniHqxTex;
    GLint  uniHqxIdxRes;
    GLint  uniHqxRes;
    GLint  uniHqxTime;

    // CRT shader
    bool crt_ok = false;

    GLuint crt_vs;
    GLuint crt_fs;
    GLuint crt_program;

    GLint  uniCrtTex;
    GLint  uniCrtIdxRes;
    GLint  uniCrtRes;
    GLint  uniCrtTime;

    // Basic shader
    GLuint basic_vs;
    GLuint basic_fs;
    GLuint basic_program;

    GLint  uniBasicTex;
    GLint  uniBasicIdxRes;
    GLint  uniBasicRes;
    GLint  uniBasicTime;

    // Screen shader
    GLuint* screen_vs;
    GLuint* screen_fs;
    GLuint* screen_program;

    GLint*  uniScreenTex;
    GLint*  uniScreenIdxRes;
    GLint*  uniScreenRes;
    GLint*  uniScreenTime;

    // Indexed shader
    GLuint indexed_vs;
    GLuint indexed_fs;
    GLuint indexed_program;

    GLint uniIndexRes;
    GLint uniIndexTime;
    GLint uniIndexPalette;
    GLint uniIndexTex;


    GLuint  indexed_texture;
    GLuint  palette_texture;

    GLuint screen_texture;
    GLuint indexed_buffer;
    GLint  screen_buffer;

    GLchar* load_source(const char* file);
    GLuint load_shader(const char* file_name, GLenum shaderType);
    int create_program(const char* vs_file, const char* fs_file, GLuint* vs, GLuint* fs);
    int create_hqx_program();
    int create_basic_program();
    int create_crt_program();
    int create_indexed_program();
    void load_buffers();

};