//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PRender.hpp"

#include "engine/PDraw.hpp"
#include "engine/PLog.hpp"
#include "engine/types.hpp"
#include "engine/render/PGl.hpp"

#include <SDL.h>

namespace PRender {

int current_mode = MODE_LINEAR;

float cover_width, cover_height;

struct GLRect {
	GLfloat x, y, w, h;
};

const GLint VBO_LOCATION = 0;
const GLint UBO_LOCATION = 1;

const GLfloat ubo_data[] = {
	0.0f,  0.0f,
	1.0f,  1.0f,
	0.0f,  1.0f,
	0.0f,  0.0f,
	1.0f,  0.0f,
	1.0f,  1.0f
};

static GLRect screen_dest = {0.f, 0.f, 1.f, 1.f};

static const char* window_name;
static SDL_Window* window = NULL;
static SDL_GLContext context;

static bool vsync_set = false;

static GLchar Log_message[1024];

GLuint vao, vbo, ubo;

static GLfloat shader_time = 0;
const  GLfloat time_cycle = M_PI * 3 * 4 * 5;// * 7;

// HQX shader
bool hqx_ok = false;

static GLuint hqx_vs;
static GLuint hqx_fs;
static GLuint hqx_program;

static GLint  uniHqxTex;
static GLint  uniHqxIdxRes;
static GLint  uniHqxRes;
static GLint  uniHqxTime;

// CRT shader
bool crt_ok = false;

static GLuint crt_vs;
static GLuint crt_fs;
static GLuint crt_program;

static GLint  uniCrtTex;
static GLint  uniCrtIdxRes;
static GLint  uniCrtRes;
static GLint  uniCrtTime;

// Basic shader
static GLuint basic_vs;
static GLuint basic_fs;
static GLuint basic_program;

static GLint  uniBasicTex;
static GLint  uniBasicIdxRes;
static GLint  uniBasicRes;
static GLint  uniBasicTime;

// Screen shader
static GLuint* screen_vs      = &basic_vs;
static GLuint* screen_fs      = &basic_fs;
static GLuint* screen_program = &basic_program;

static GLint*  uniScreenTex    = &uniBasicTex;
static GLint*  uniScreenIdxRes = &uniBasicIdxRes;
static GLint*  uniScreenRes    = &uniBasicRes;
static GLint*  uniScreenTime   = &uniBasicTime;

// Indexed shader
static GLuint indexed_vs;
static GLuint indexed_fs;
static GLuint indexed_program;

static GLint uniIndexRes;
static GLint uniIndexTime;
static GLint uniIndexPalette;
static GLint uniIndexTex;


static GLuint  indexed_texture;
static GLfloat indexed_palette[256*3];

static GLuint screen_texture;
static GLuint indexed_buffer;
static GLint  screen_buffer;

void set_fullscreen(bool set) {

	#ifndef __ANDROID__

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(window);

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

		screen_dest.w = 1.f;
		screen_dest.h = screen_prop / buff_prop;
		screen_dest.x = 0.f;
		screen_dest.y = (1.f - screen_dest.h) / 2;
	
	} else {

		screen_dest.w = buff_prop / screen_prop;
		screen_dest.h = 1.f;
		screen_dest.x = (1.f - screen_dest.w) / 2;
		screen_dest.y = 0.f;

	}

	float x = screen_dest.x;
	float y = screen_dest.y;
	float a = x + screen_dest.w;
	float b = y + screen_dest.h;

	GLfloat screen_vbo_data[] = {

		x,  b,
		a,  y,
		x,  y,
		x,  b,
		a,  b,
		a,  y

	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screen_vbo_data), screen_vbo_data, GL_DYNAMIC_DRAW);

	cover_width = screen_dest.w * w;
	cover_height = screen_dest.h * h;

}

int set_filter(const char* filter) {

	glBindTexture(GL_TEXTURE_2D, screen_texture);

	if (strcmp(filter, FILTER_LINEAR) == 0) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	} else if (strcmp(filter, FILTER_NEAREST) == 0) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	return 0;
}

int set_mode(int mode) {

	if (mode == MODE_CRT && !crt_ok)
		return 1;

	if (mode == MODE_HQX && !hqx_ok)
		return 1;

	if (mode == MODE_NEAREST || mode == MODE_HQX) {
		set_filter(FILTER_NEAREST);
	} else {
		set_filter(FILTER_LINEAR);
	}

	if (mode == MODE_NEAREST || mode == MODE_LINEAR) {

		screen_vs      = &basic_vs;
		screen_fs      = &basic_fs;
		screen_program = &basic_program;

		uniScreenTex    = &uniBasicTex;
		uniScreenIdxRes = &uniBasicIdxRes;
		uniScreenRes    = &uniBasicRes;
		uniScreenTime   = &uniBasicTime;

	} else if (mode == MODE_CRT) {

		screen_vs      = &crt_vs;
		screen_fs      = &crt_fs;
		screen_program = &crt_program;

		uniScreenTex    = &uniCrtTex;
		uniScreenIdxRes = &uniCrtIdxRes;
		uniScreenRes    = &uniCrtRes;
		uniScreenTime   = &uniCrtTime;

	} else if (mode == MODE_HQX) {

		screen_vs      = &hqx_vs;
		screen_fs      = &hqx_fs;
		screen_program = &hqx_program;

		uniScreenTex    = &uniHqxTex;
		uniScreenIdxRes = &uniHqxIdxRes;
		uniScreenRes    = &uniHqxRes;
		uniScreenTime   = &uniHqxTime;

	}

	return 0;

}

void get_window_size(int* w, int* h) {

	SDL_GetWindowSize(window, w, h);

}

void get_window_position(int* x, int* y) {

	SDL_GetWindowPosition(window, x, y);

}

int set_vsync(bool set) {

	if (set == vsync_set)
		return 0;

	if (!set) {
		SDL_GL_SetSwapInterval(0);
	} else {
		// Try adaptative vsync
		int ret = SDL_GL_SetSwapInterval(-1);

		// If fail try normal vsync
		if (ret == -1)
			SDL_GL_SetSwapInterval(1);

	}
	vsync_set = set;

	return 0;

}

bool is_vsync() {

	return vsync_set;
	
}


GLchar* load_source(const char* file) {

	SDL_RWops* rw =  SDL_RWFromFile(file, "rb");
	if (!rw) return NULL;

	Sint64 size = SDL_RWsize(rw);
	if (size <= 0) {
		SDL_RWclose(rw);
		return NULL;
	}

	GLchar* ret = (GLchar*)malloc(size + 1);
	if (!ret) {
		SDL_RWclose(rw);
		return NULL;
	}

	SDL_RWread(rw, ret, size, 1);
	ret[size] = '\0';

	SDL_RWclose(rw);
	return ret;

}

GLuint load_shader(const char* file_name, GLenum shaderType) {

	GLuint shader = glCreateShader(shaderType);
	if (!shader) return 0;

	GLchar* source = load_source(file_name);
	if (!source) {
		PLog::Write(PLog::ERR, "PRender", "Can't load %s", file_name);
		return 0;
	}

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	free(source);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		
		glGetShaderInfoLog(shader, 1024, NULL, Log_message);
		PLog::Write(PLog::ERR, "PRender", "Error compiling %s:\n%s", file_name, Log_message);

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
		PLog::Write(PLog::ERR, "PRender", "Can't load vertex shader");
		return 0;
	}

	*fs = load_shader(fs_file, GL_FRAGMENT_SHADER);
	if(!*fs) {
		PLog::Write(PLog::ERR, "PRender", "Can't load frament shader");
		glDeleteShader(*vs);
		return 0;
	}
	
	GLuint program = glCreateProgram();
	if(!program) {
		PLog::Write(PLog::ERR, "PRender", "Can't create program");
		glDeleteShader(*vs);
		glDeleteShader(*fs);
		glDeleteProgram(program);
		return 0;
	}

	glBindAttribLocation(program, UBO_LOCATION, "uv_in");
	glBindAttribLocation(program, VBO_LOCATION, "vbo_in");

	glAttachShader(program, *vs);
	glAttachShader(program, *fs);
	glLinkProgram(program);
	
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked) {
		glGetProgramInfoLog(program, 1024, NULL, Log_message);
		PLog::Write(PLog::ERR, "PRender", "Can't link program\n%s", Log_message);
		glDeleteShader(*vs);
		glDeleteShader(*fs);
		glDeleteProgram(program);
		return 0;
	}

	return program;

}

int create_basic_program() {

	basic_program = create_program("shaders/basic.vs", "shaders/basic.fs", &basic_vs, &basic_fs);
	if (!basic_program) {
		PLog::Write(PLog::FATAL, "PRender", "Can't create basic program");
		return 1;
	}

	glUseProgram(basic_program);

	uniBasicTex    = glGetUniformLocation(basic_program, "screen_tex");
	uniBasicIdxRes = glGetUniformLocation(basic_program, "buffer_res");
	uniBasicRes    = glGetUniformLocation(basic_program, "screen_res");
	uniBasicTime   = glGetUniformLocation(basic_program, "time");

	glBindFragDataLocation(basic_program, 0, "color");

	return 0;

}

int create_hqx_program() {

	hqx_program = create_program("shaders/hqx.vs", "shaders/hqx.fs", &hqx_vs, &hqx_fs);
	if (!hqx_program) {
		PLog::Write(PLog::ERR, "PRender", "Can't create hqx program");
		return 1;
	}

	hqx_ok = true;
	glUseProgram(hqx_program);

	uniHqxTex    = glGetUniformLocation(hqx_program, "screen_tex");
	uniHqxIdxRes = glGetUniformLocation(hqx_program, "buffer_res");
	uniHqxRes    = glGetUniformLocation(hqx_program, "screen_res");
	uniHqxTime   = glGetUniformLocation(hqx_program, "time");

	glBindFragDataLocation(hqx_program, 0, "color");

	return 0;

}

int create_crt_program() {

	crt_program = create_program("shaders/crt.vs", "shaders/crt.fs", &hqx_vs, &hqx_fs);
	if (!crt_program) {
		PLog::Write(PLog::ERR, "PRender", "Can't create screen program");
		return 1;
	}

	crt_ok = true;
	glUseProgram(crt_program);

	uniCrtTex    = glGetUniformLocation(crt_program, "screen_tex");
	uniCrtIdxRes = glGetUniformLocation(crt_program, "buffer_res");
	uniCrtRes    = glGetUniformLocation(crt_program, "screen_res");
	uniCrtTime   = glGetUniformLocation(crt_program, "time");

	glBindFragDataLocation(crt_program, 0, "color");

	return 0;
}

int create_indexed_program() {

	indexed_program = create_program("shaders/indexed.vs", "shaders/indexed.fs", &indexed_vs, &indexed_fs);
	if (!indexed_program) {
		PLog::Write(PLog::ERR, "PRender", "Can't create indexed program");
		return 1;
	}

	glUseProgram(indexed_program);

	uniIndexPalette = glGetUniformLocation(indexed_program, "palette");
	uniIndexTex     = glGetUniformLocation(indexed_program, "indexed_tex");
	uniIndexRes     = glGetUniformLocation(indexed_program, "indexed_res");
	uniIndexTime    = glGetUniformLocation(indexed_program, "time");

	glBindFragDataLocation(indexed_program, 0, "color");

	return 0;

}

void load_buffers() {
	
	// Indexed texture (recieve game buffer)
	glGenTextures(1, &indexed_texture);
	glBindTexture(GL_TEXTURE_2D, indexed_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);

	glGenTextures(1, &screen_texture);
	glBindTexture(GL_TEXTURE_2D, screen_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 480, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);


	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &screen_buffer);
	glGenFramebuffers(1, &indexed_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, indexed_buffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_texture, 0);

	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

		PLog::Write(PLog::ERR, "PRender", "Error creating frame buffer");
		return;

	}

	glBindFramebuffer(GL_FRAMEBUFFER, screen_buffer);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VBO_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VBO_LOCATION);

	glGenBuffers(1, &ubo);
	glBindBuffer(GL_ARRAY_BUFFER, ubo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ubo_data), ubo_data, GL_STATIC_DRAW);
	glVertexAttribPointer(UBO_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(UBO_LOCATION);

}

int init(int width, int height, const char* name, const char* icon) {

	PLog::Write(PLog::DEBUG, "PRender", "Initializing renderer");

	window_name = name;

	window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
	load_lib();

	set_vsync(true);
	
	PLog::Write(PLog::DEBUG, "PRender", "OpenGL version: %s", glGetString(GL_VERSION));
	PLog::Write(PLog::DEBUG, "PRender", "GLSL version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//glDisable(GL_LIGHTING);
	glDisable(GL_DITHER);
	//glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	load_buffers();

	if (create_basic_program() != 0)
		return -1;
	if (create_indexed_program() != 0)
		return -1;

	create_hqx_program();
	create_crt_program();

	adjust_screen();

	return 0;

}

void terminate() {

	SDL_DestroyWindow(window);

	glDeleteTextures(1, &screen_texture);
	glDeleteTextures(1, &indexed_texture);
	glDeleteFramebuffers(1, &indexed_buffer);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ubo);

	glDeleteProgram(basic_program);
	glDeleteProgram(indexed_program);
	
	if (crt_ok) glDeleteProgram(crt_program);
	if (hqx_ok) glDeleteProgram(hqx_program);
	
	SDL_GL_DeleteContext(context);
	free_lib();

}

void update(void* _buffer8, int alpha) {

	//TODO - fps independent
	shader_time += 1.f/60;
	if (shader_time > time_cycle)
		shader_time -= time_cycle;

	SDL_Surface* buffer8 = (SDL_Surface*)_buffer8;

	SDL_LockSurface(buffer8);

	float a = float(alpha) / 100;

	// Set uniforms and size only if changed?

	for (int i = 0; i < 256; i++) {
		indexed_palette[3*i] = float(buffer8->format->palette->colors[i].r) * a / 256;
		indexed_palette[3*i + 1] = float(buffer8->format->palette->colors[i].g) * a / 256;
		indexed_palette[3*i + 2] = float(buffer8->format->palette->colors[i].b) * a / 256;
	}
	
	glViewport(0, 0, buffer8->w, buffer8->h);
	glBindFramebuffer(GL_FRAMEBUFFER, indexed_buffer);
	glUseProgram(indexed_program);
	glBindTexture(GL_TEXTURE_2D, indexed_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, buffer8->w, buffer8->h, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, buffer8->pixels);
	glUniform1i(uniIndexTex, 0); //
	glUniform3fv(uniIndexPalette, 256, indexed_palette); //
	glUniform1f(uniIndexTime, shader_time);
	glUniform2f(uniIndexRes, buffer8->w, buffer8->h); //
	glDrawArrays(GL_TRIANGLES, 0, 6);

	int w, h;
	SDL_GetWindowSize(window, &w, &h); //
	glViewport(0, 0, w, h);
	glBindFramebuffer(GL_FRAMEBUFFER, screen_buffer);
	glUseProgram(*screen_program);
	glBindTexture(GL_TEXTURE_2D, screen_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, buffer8->w, buffer8->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); //
	glUniform1i(*uniScreenTex, 0); //
	glUniform2f(*uniScreenIdxRes, buffer8->w, buffer8->h); //
	glUniform2f(*uniScreenRes, cover_width, cover_height); //
	glUniform1f(*uniScreenTime, shader_time);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	SDL_UnlockSurface(buffer8);
	SDL_GL_SwapWindow(window);

}

}