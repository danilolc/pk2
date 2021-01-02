//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/Piste.hpp"

#include "engine/platform.hpp"

#include <SDL.h>

#include <chrono>
#include <thread>
#include <iostream>

#define UPDATE_FPS 30 //Update FPS each 30 frames

namespace Piste {

static int desired_fps = 60;

static bool ready = false;
static bool running = false;

static float avrg_fps = 0;

static bool debug = false;
static bool draw = true;

#ifndef PK2_NO_THREAD

static void wait_frame() {

	using namespace std;
	using namespace std::chrono;

	static auto last_time = high_resolution_clock::now();

	auto delta_time = high_resolution_clock::now() - last_time;
	auto frame_time = chrono::nanoseconds(1000000000ll / desired_fps);

	auto sleep_time = frame_time - delta_time;

	if (sleep_time.count() > 100000)
		this_thread::sleep_for(sleep_time);
	
	last_time = high_resolution_clock::now();

}

#endif

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
		void* buffer8;
		int alpha;
		PDraw::get_buffer_data(&buffer8, &alpha);
		PRender::update(buffer8, alpha);
	}

	// Clear PDraw buffer
	PDraw::update();

	#ifndef PK2_NO_THREAD
	if (!PRender::is_vsync() && (desired_fps > 0) && draw)
		wait_frame();
	#endif

	PInput::update();
	PSound::update();
	
	if (debug) {
		fflush(stdout);
	}

}

static void sdl_show() {

	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	
	PLog::Write(PLog::DEBUG, "Piste", "We compiled against SDL version %d.%d.%d ...",
		compiled.major, compiled.minor, compiled.patch);
	
	PLog::Write(PLog::DEBUG, "Piste", "But we are linking against SDL version %d.%d.%d.",
		linked.major, linked.minor, linked.patch);
	
}


void init(int width, int height, const char* name, const char* icon, int audio_buffer_size) {
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

		PLog::Write(PLog::FATAL, "Piste", "Unable to init SDL: %s", SDL_GetError());
		return;
		
	}

	sdl_show();
	
	PDraw::init(width, height);
	PRender::init(width, height, name, icon);
	PInput::init();
	PSound::init(audio_buffer_size);

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

void set_fps(int fps) {

	// Vsync true
	if (fps == -1) {

		PRender::set_vsync(true);
		return;

	}

	PRender::set_vsync(false);
	desired_fps = fps;

}

void ignore_frame() {

	draw = false;

}

bool is_ready() {

	return ready;

}

}
