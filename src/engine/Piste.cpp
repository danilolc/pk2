//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/Piste.hpp"

#include "engine/platform.hpp"

#include <SDL.h>

#include <iostream>

#define UPDATE_FPS 30 //Update FPS each 30 frames

namespace Piste {

static int desired_fps = 60;

static bool ready = false;
static bool running = false;

static float avrg_fps = 0;

static bool debug = false;
static bool draw = true;

static void wait_frame() {

	static u64 last_time = SDL_GetPerformanceCounter();

	u64 c_frec = SDL_GetPerformanceFrequency();

	u64 one_frame = c_frec / desired_fps;
	u64 exit_time = last_time + one_frame;
	
	u64 curr_time = SDL_GetPerformanceCounter();
	
	if (curr_time > exit_time) {
		last_time = curr_time;
		return;
	}
	
	u64 wait_time = exit_time - curr_time;
	u32 ms = wait_time * 1000 / c_frec;

	SDL_Delay(ms);

	while(1) {

		u64 c = SDL_GetPerformanceCounter();
		
		if (c >= exit_time || c < last_time)
			break;

	}

	last_time = exit_time;

}

static void logic() {
	
	SDL_Event event;

	while( SDL_PollEvent(&event) ) {
		
		if(event.type == SDL_QUIT)
			running = false;
		else if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
			PRender::adjust_screen();
		else if(event.type == SDL_TEXTINPUT && PInput::Is_Editing())
			PInput::InjectText(event.text.text);
		else if(event.type == SDL_KEYDOWN && PInput::Is_Editing())
			PInput::InjectKey(event.key.keysym.scancode);
		
	}

	// Pass PDraw informations do PRender
	if (draw) {
		PGui::update();
		void* buffer8;
		PDraw::get_buffer_data(&buffer8);
		PRender::update(buffer8);
	}

	// Clear PDraw buffer
	PDraw::update();

	if (!PRender::is_vsync() && (desired_fps > 0) && draw)
		wait_frame();

	PInput::update();
	PSound::update();
	
	if (debug) {
		fflush(stdout);
	}

}

static void sdl_show_version() {

	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	
	PLog::Write(PLog::DEBUG, "Piste", "We compiled against SDL version %d.%d.%d ...",
		compiled.major, compiled.minor, compiled.patch);
	
	PLog::Write(PLog::DEBUG, "Piste", "But we are linking against SDL version %d.%d.%d.",
		linked.major, linked.minor, linked.patch);
	
}


void init(int width, int height, const char* name, const char* icon, int render_method, int audio_buffer_size, bool audio_multi_thread) {
	
	u32 flags = SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | \
                SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER /*| SDL_INIT_SENSOR*/;
	
	if (SDL_Init(flags) < 0) {

		PLog::Write(PLog::FATAL, "Piste", "Unable to init SDL: %s", SDL_GetError());
		return;
		
	}

	sdl_show_version();
	
	PDraw::init(width, height);
	PRender::init(width, height, name, icon, render_method);
	PInput::init();
	PSound::init(audio_buffer_size, audio_multi_thread);

	ready = true;

}

void terminate() {
	
	PDraw::terminate();
	PRender::terminate();
	PInput::terminate();
	PSound::terminate();

	SDL_Quit();

	ready = false;

}

void loop(int (*GameLogic)()) {
	
	static int frame_counter = 0;
	static u32 last_time = 0;
		
	int error = 0;

	running = true;

	while(running) {
		
		error = GameLogic();
		if (error) break;
		
		logic();

		frame_counter++;
		if (frame_counter >= UPDATE_FPS) {

			frame_counter = 0;
			u32 elapsed_time = (SDL_GetTicks() - last_time);

			avrg_fps = 1000.f * UPDATE_FPS / elapsed_time;	

			last_time += elapsed_time;
		
		}
		
		draw = true;
	
	}

}

void stop() {
	
	running = false;

}

int get_fps() {
	
	return (int)avrg_fps;

}

void set_debug(bool set) {

	debug = set;

}

int set_fps(int fps) {

	desired_fps = fps;

	PLog::Write(PLog::DEBUG, "Piste", "FPS set to %i", fps);
	
	// Vsync true
	if (fps == -1)
		return PRender::set_vsync(true);
	
	return PRender::set_vsync(false);

}

void ignore_frame() {

	draw = false;

}

bool is_ready() {

	return ready;

}

}
