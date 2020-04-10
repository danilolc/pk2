//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/Piste.hpp"

#include "engine/platform.hpp"

#include <SDL.h>

#define UPDATE_FPS 30 //Update FPS each 30 frames

namespace Piste {

static bool ready = false;
static bool running = false;

static float avrg_fps = 0;

static bool debug = false;
static bool draw = true;

static void logic() {
	
	SDL_Event event;

	while( SDL_PollEvent(&event) ) {
		
		if(event.type == SDL_QUIT)
			running = false;
		if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
			PDraw::adjust_screen();
		if(event.type == SDL_TEXTINPUT && PInput::Is_Editing())
			PInput::InjectText(event.text.text);
		if(event.type == SDL_KEYDOWN && PInput::Is_Editing())
			PInput::InjectKey(event.key.keysym.scancode);
		
	}

	PDraw::update(draw);
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


void init(int width, int height, const char* name, const char* icon) {
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

		PLog::Write(PLog::FATAL, "Piste", "Unable to init SDL: %s", SDL_GetError());
		return;
		
	}

	sdl_show();
	
	PDraw::init(width, height, name, icon);
	PInput::init();
	PSound::init();

	ready = true;

}

void terminate() {
	
	PDraw::terminate();
	PInput::terminate();
	PSound::terminate();

	SDL_Quit();

	ready = false;

}

void loop(int (*GameLogic)()) {
	
	static int frame_counter = 0;
	static int last_time = 0;
		
	int error = 0;

	running = true;

	while(running) {

		error = GameLogic();
		if (error) break;
		
		logic();

		frame_counter++;
		if (frame_counter >= UPDATE_FPS) {

			frame_counter = 0;
			int elapsed_time = (SDL_GetTicks() - last_time);

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

void ignore_frame() {

	draw = false;

}

bool is_ready() {

	return ready;

}

}