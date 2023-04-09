//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/render/PGl.hpp"

//#include <SDL_opengles2.h>
#include "engine/render/PGlFuncs.hpp"
#include "engine/PLog.hpp"

#include <SDL.h>

void PGl::load_ui_texture(void* surface) {

    PLog::Write(PLog::ERR, "PGl", "Texture not implemented!");
	SDL_FreeSurface((SDL_Surface*) surface);

}

void PGl::render_ui(PRender::FRECT src, PRender::FRECT dst, float alpha) {

}

void PGl::clear_screen() {

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(curr_window);
	
}

void PGl::set_screen(PRender::FRECT screen_dst) {

	float x = screen_dst.x;
	float y = screen_dst.y;
	float a = x + screen_dst.w;
	float b = y + screen_dst.h;

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

}

int PGl::set_shader(int mode) {

	if (mode == PRender::SHADER_CRT && !crt_ok)
		return 1;

	if (mode == PRender::SHADER_HQX && !hqx_ok)
		return 1;
    
    glBindTexture(GL_TEXTURE_2D, screen_texture);

	if (mode == PRender::SHADER_NEAREST || mode == PRender::SHADER_HQX) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	} else if (mode == PRender::SHADER_LINEAR || mode == PRender::SHADER_CRT) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	if (mode == PRender::SHADER_NEAREST || mode == PRender::SHADER_LINEAR) {

		screen_vs      = &basic_vs;
		screen_fs      = &basic_fs;
		screen_program = &basic_program;

		uniScreenTex    = &uniBasicTex;
		uniScreenIdxRes = &uniBasicIdxRes;
		uniScreenRes    = &uniBasicRes;
		uniScreenTime   = &uniBasicTime;

	} else if (mode == PRender::SHADER_CRT) {

		screen_vs      = &crt_vs;
		screen_fs      = &crt_fs;
		screen_program = &crt_program;

		uniScreenTex    = &uniCrtTex;
		uniScreenIdxRes = &uniCrtIdxRes;
		uniScreenRes    = &uniCrtRes;
		uniScreenTime   = &uniCrtTime;

	} else if (mode == PRender::SHADER_HQX) {

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

int PGl::set_vsync(bool set) {

    int ret = 0;

	if (!set) {
		SDL_GL_SetSwapInterval(0);
	} else {
		// Try adaptative vsync
		ret = SDL_GL_SetSwapInterval(-1);

		// If fail try normal vsync
		if (ret == -1)
			ret = SDL_GL_SetSwapInterval(1);
		
	}

	return ret;

}

GLchar* PGl::load_source(const char* file) {

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

GLuint PGl::load_shader(const char* file_name, GLenum shaderType) {

	GLuint shader = glCreateShader(shaderType);
	if (!shader) return 0;

	GLchar* source = load_source(file_name);
	if (!source) {
		PLog::Write(PLog::ERR, "PGl", "Can't load %s", file_name);
		return 0;
	}

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	free(source);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		
		glGetShaderInfoLog(shader, 1024, NULL, Log_message);
		PLog::Write(PLog::ERR, "PGl", "Error compiling %s:\n%s", file_name, Log_message);

		glDeleteShader(shader);
		return 0;

	}

	return shader;

}

int PGl::create_program(const char* vs_file, const char* fs_file, GLuint* vs, GLuint* fs) {

	GLuint _vs, _fs;
	if (!vs) vs = &_vs;
	if (!fs) fs = &_fs;

	*vs = load_shader(vs_file, GL_VERTEX_SHADER);
	if(!*vs) {
		PLog::Write(PLog::ERR, "PGl", "Can't load vertex shader");
		return 0;
	}

	*fs = load_shader(fs_file, GL_FRAGMENT_SHADER);
	if(!*fs) {
		PLog::Write(PLog::ERR, "PGl", "Can't load frament shader");
		glDeleteShader(*vs);
		return 0;
	}
	
	GLuint program = glCreateProgram();
	if(!program) {
		PLog::Write(PLog::ERR, "PGl", "Can't create program");
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
		PLog::Write(PLog::ERR, "PGl", "Can't link program\n%s", Log_message);
		glDeleteShader(*vs);
		glDeleteShader(*fs);
		glDeleteProgram(program);
		return 0;
	}

	return program;

}

int PGl::create_basic_program() {

	basic_program = create_program("shaders/basic.vs", "shaders/basic.fs", &basic_vs, &basic_fs);
	if (!basic_program) {
		PLog::Write(PLog::FATAL, "PGl", "Can't create basic program");
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

int PGl::create_hqx_program() {

	hqx_program = create_program("shaders/hqx.vs", "shaders/hqx.fs", &hqx_vs, &hqx_fs);
	if (!hqx_program) {
		PLog::Write(PLog::ERR, "PGl", "Can't create hqx program");
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

int PGl::create_crt_program() {

	crt_program = create_program("shaders/crt.vs", "shaders/crt.fs", &hqx_vs, &hqx_fs);
	if (!crt_program) {
		PLog::Write(PLog::ERR, "PGl", "Can't create screen program");
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

int PGl::create_indexed_program() {

	indexed_program = create_program("shaders/indexed.vs", "shaders/indexed.fs", &indexed_vs, &indexed_fs);
	if (!indexed_program) {
		PLog::Write(PLog::ERR, "PGl", "Can't create indexed program");
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

void PGl::load_buffers() {
	
	// Indexed texture (recieve game buffer)
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &indexed_texture);
	glBindTexture(GL_TEXTURE_2D, indexed_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &palette_texture);
	glBindTexture(GL_TEXTURE_1D, palette_texture);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE0);
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

		PLog::Write(PLog::ERR, "PGl", "Error creating frame buffer");
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

void PGl::update(void* _buffer8) {
	
	SDL_Surface* buffer8 = (SDL_Surface*)_buffer8;

	//TODO - fps independent
	shader_time += 1.f/60;
	if (shader_time > time_cycle)
		shader_time -= time_cycle;
	
	///////////////////////////////////////////
	// Send game indexed buffer to texture
	///////////////////////////////////////////

	SDL_LockSurface(buffer8);

	int buffer_w = buffer8->w;
	int buffer_h = buffer8->h;
	
	glViewport(0, 0, buffer_w, buffer_h);
	glBindFramebuffer(GL_FRAMEBUFFER, indexed_buffer);
	glUseProgram(indexed_program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, indexed_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, buffer8->w, buffer8->h, 0, GL_RED, GL_UNSIGNED_BYTE, buffer8->pixels);
	glUniform1i(uniIndexTex, 0); //

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, palette_texture);
	glUniform1i(uniIndexPalette, 1); //

	Uint32 current_palette_version = buffer8->format->palette->version;
	
	if (current_palette_version != last_palette_version) {

		// Update palette
		SDL_Color* bufer8_colors = buffer8->format->palette->colors;
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufer8_colors);
		last_palette_version = current_palette_version;
	
	}

	SDL_UnlockSurface(buffer8);

	glUniform1f(uniIndexTime, shader_time);
	glUniform2f(uniIndexRes, buffer_w, buffer_h); //
	glDrawArrays(GL_TRIANGLES, 0, 6);

	///////////////////////////////////////////
	// Draw the RGB texture to the screen
	///////////////////////////////////////////

	int w, h;
	PRender::get_window_size(&w, &h); //

	int cw, ch;
	PRender::get_cover_size(&cw, &ch); //
	
	glViewport(0, 0, w, h);
	glBindFramebuffer(GL_FRAMEBUFFER, screen_buffer);
	glUseProgram(*screen_program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screen_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, buffer_w, buffer_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); //
	glUniform1i(*uniScreenTex, 0); //
	
	glUniform2f(*uniScreenIdxRes, buffer_w, buffer_h); //
	glUniform2f(*uniScreenRes, cw, ch); //
	glUniform1f(*uniScreenTime, shader_time);
	
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	///////////////////////////////////////////
	// Swap window
	///////////////////////////////////////////

	SDL_GL_SwapWindow(curr_window);

}

PGl::PGl(int width, int height, void* window) {

    curr_window = (SDL_Window*)window;

	context = SDL_GL_CreateContext(curr_window);

	glDisable(GL_DITHER);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_1D);

	load_buffers();

	if (create_basic_program() != 0)
	;//	return -1;
	if (create_indexed_program() != 0)
	;//	return -1;

	create_hqx_program();
	create_crt_program();

    screen_vs      = &basic_vs;
    screen_fs      = &basic_fs;
    screen_program = &basic_program;

    uniScreenTex    = &uniBasicTex;
    uniScreenIdxRes = &uniBasicIdxRes;
    uniScreenRes    = &uniBasicRes;
    uniScreenTime   = &uniBasicTime;

}

PGl::~PGl() {

	glDeleteTextures(1, &indexed_texture);
	glDeleteTextures(1, &palette_texture);
	glDeleteTextures(1, &screen_texture);
	glDeleteFramebuffers(1, &indexed_buffer);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ubo);

	glDeleteProgram(basic_program);
	glDeleteProgram(indexed_program);
	
	if (crt_ok) glDeleteProgram(crt_program);
	if (hqx_ok) glDeleteProgram(hqx_program);
	
	SDL_GL_DeleteContext(context);
	PGlFuncs::free_lib();

	PLog::Write(PLog::DEBUG, "PGl", "Terminated");

}