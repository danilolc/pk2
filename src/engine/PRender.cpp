//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PRender.hpp"

#include "engine/PDraw.hpp"
#include "engine/PLog.hpp"
#include "engine/types.hpp"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <SDL.h>

namespace PRender {

static SDL_Rect screen_dest = {0, 0, 0, 0};
static bool screen_fit = false;

static const char* window_name;
static SDL_Window* window = NULL;
static SDL_GLContext context;

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

    int sync = set? -1 : 0;

	printf("%i AAA\n", sync);

	if (!set)
		SDL_GL_SetSwapInterval(0);
	else {
		// Try adaptative vsync
		int ret = SDL_GL_SetSwapInterval(-1);

		// If fail try normal vsync
		if (ret == -1)
			SDL_GL_SetSwapInterval(1);

	}
	vsync_set = set;

    return 0;

    #else

    return -1;

    #endif

}

bool is_vsync() {

    return vsync_set;
    
}





GLchar Log_message[1024];

GLchar* load_source(const char* file) {

	SDL_RWops* rw =  SDL_RWFromFile(file, "rb");
	if (!rw) return NULL;

	Sint64 size = SDL_RWsize(rw);
	if (size <= 0) {
		SDL_RWclose(rw);
		return NULL;
	}

	Uint8* ret = (Uint8*)malloc(size + 1);
	if (!ret) {
		SDL_RWclose(rw);
		return NULL;
	}

	SDL_RWread(rw, ret, size, 1);
	ret[size] = '\0';

	SDL_RWclose(rw);
	return (GLchar*)ret;

}

GLuint load_shader(const char* file_name, GLenum shaderType) {

	GLuint shader = glCreateShader(shaderType);
	if (!shader) return 0;

	GLchar* source = load_source(file_name);
	if (!source) {
		printf("Can't load %s\n", file_name);
		return 0;
	}

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	free(source);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		
	    glGetShaderInfoLog(shader, 1024, NULL, Log_message);
		printf("Error compiling %s:\n%s\n", file_name, Log_message);

		glDeleteShader(shader);
		return 0;

	}

	return shader;

}

int create_program(const char* vs_file, const char* fs_file, GLuint* vs, GLuint* fs) {

	GLuint _vs, _fs;
	if (!vs) vs = &_vs;
	if (!fs) fs = &_fs;

	*vs = load_shader(vs_file, GL_VERTEX_SHADER);
	if(!*vs) {
		printf("Can't load vertex shader\n");
		return 0;
	}

	*fs = load_shader(fs_file, GL_FRAGMENT_SHADER);
	if(!*fs) {
		printf("Can't load frament shader\n");
		glDeleteShader(*vs);
		return 0;
	}
	
	GLuint program = glCreateProgram();
	if(!program) {
		printf("Can't create program\n");
		glDeleteShader(*vs);
		glDeleteShader(*fs);
		glDeleteProgram(program);
		return 0;
	}

	glAttachShader(program, *vs);
	glAttachShader(program, *fs);
	glLinkProgram(program);
	
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked) {
		glGetProgramInfoLog(program, 1024, NULL, Log_message);
		printf("Can't link program\n%s\n", Log_message);
		glDeleteShader(*vs);
		glDeleteShader(*fs);
		glDeleteProgram(program);
		return 0;
	}

	return program;

}

GLuint screen_vs;
GLuint screen_fs;
GLuint screen_program;

GLint  uniScreenTex;
GLuint screen_texture;

void create_screen_program() {

	screen_program = create_program("shader/screen.vs", "shader/screen.fs", &screen_vs, &screen_fs);
	if (!screen_program) {
		printf("Can't create screen program\n");
		return;
	}

	glUseProgram(screen_program);

	GLint vbo_att = glGetAttribLocation(screen_program, "vbo");
	glEnableVertexAttribArray(vbo_att);
	glVertexAttribPointer(vbo_att, 2, GL_FLOAT, GL_FALSE, 0, 0);

	uniScreenTex = glGetUniformLocation(screen_program, "screen_tex");

	glBindFragDataLocation(screen_program, 0, "color");

}

GLuint indexed_vs;
GLuint indexed_fs;
GLuint indexed_program;

GLint uniPalette;
GLint uniIndexTex;

void create_indexed_program() {

	indexed_program = create_program("shader/indexed.vs", "shader/indexed.fs", &indexed_vs, &indexed_fs);
	if (!indexed_program) {
		printf("Can't create indexed program\n");
		return;
	}

	glUseProgram(indexed_program);

	GLint vbo_att = glGetAttribLocation(indexed_program, "vbo");
	glEnableVertexAttribArray(vbo_att);
	glVertexAttribPointer(vbo_att, 2, GL_FLOAT, GL_FALSE, 0, 0);

	uniPalette  = glGetUniformLocation(indexed_program, "palette");
	uniIndexTex = glGetUniformLocation(indexed_program, "indexed_tex");

	glBindFragDataLocation(indexed_program, 0, "color");

}

GLuint indexed_texture;
GLfloat indexed_palette[256*3];

GLuint indexed_buffer;
GLint screen_buffer;

void load_buffers() {

	glGenTextures(1, &screen_texture);
	glBindTexture(GL_TEXTURE_2D, screen_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenTextures(1, &indexed_texture);
	glBindTexture(GL_TEXTURE_2D, indexed_texture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, 800, 480, 0,  GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &screen_buffer);
	glGenFramebuffers(1, &indexed_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, indexed_buffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, screen_texture, 0);

	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

		printf("Error creating frame buffer\n");
		return;

	}

	glBindFramebuffer(GL_FRAMEBUFFER, screen_buffer);

}

void create_arrays() {

	const float Vertices[] = {
		 0.0f,  1.0f,
		 1.0f,  0.0f,
		 0.0f,  0.0f,
		 0.0f,  1.0f,
		 1.0f,  1.0f,
		 1.0f,  0.0f
	};

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

}

int init(int width, int height, const char* name, const char* icon) {

    window_name = name;

    window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
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

    context = SDL_GL_CreateContext(window);
	   
	glViewport(0, 0, 800, 480);
	glDisable(GL_LIGHTING);
	glDisable(GL_DITHER);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

    load_buffers();
    create_arrays();

    create_screen_program();
	create_indexed_program();

    adjust_screen();

    return 0;

}

void terminate() {

    SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(context);

}

void update(void* _buffer8, int alpha) {

    SDL_Surface* buffer8 = (SDL_Surface*)_buffer8;

    SDL_LockSurface(buffer8);

    // Only if changed?
    float a = float(alpha) / 100;
    for (int i = 0; i < 256; i++) {
		indexed_palette[3*i] = (float)buffer8->format->palette->colors[i].r * a / 256;
		indexed_palette[3*i + 1] = (float)buffer8->format->palette->colors[i].g * a / 256;
		indexed_palette[3*i + 2] = (float)buffer8->format->palette->colors[i].b * a / 256;
	}
    
	glViewport(0, 0, 800, 480);
	glBindFramebuffer(GL_FRAMEBUFFER, indexed_buffer);
	glUseProgram(indexed_program);
	glBindTexture(GL_TEXTURE_2D, indexed_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, buffer8->w, buffer8->h, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, buffer8->pixels);
	glUniform1i(uniIndexTex, 0);
	glUniform3fv(uniPalette, 256, indexed_palette);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Only if changed?
	int w, h;
    SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glBindFramebuffer(GL_FRAMEBUFFER, screen_buffer);
	glUseProgram(screen_program);
	glBindTexture(GL_TEXTURE_2D, screen_texture);
	glUniform1i(uniScreenTex, 0);
	glClearColor(0.1f, 0.05f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	SDL_UnlockSurface(buffer8);
    SDL_GL_SwapWindow(window);

}

}