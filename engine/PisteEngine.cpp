//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################
#include "PisteEngine.hpp"
#include "platform.hpp"

#include <SDL.h>
#include <string>

namespace Piste {

bool ready = false;
bool running = false;

float avrg_fps = 0;

bool debug = false;
bool draw = true;

void logic();

void init(int width, int height, const char* name, const char* icon) {
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("Unable to init SDL: %s\n", SDL_GetError());
		return;
	}

	PDraw::init(width, height, name, icon);
	PisteInput_Start();
	PSound::init();

	ready = true;

}

void terminate() {
	
	PDraw::terminate();
	PisteInput_Exit();
	PSound::terminate();
	SDL_Quit();
	ready = false;

}

void loop(int (*GameLogic)()) {
	
	int last_time = 0;
	int count = 0; // Count how much frames elapsed
	float real_fps = 0;
	
	running = true;

	while(running) {

		count++;

		GameLogic();
		logic();

		if (draw) {
			real_fps = 1000.f / (SDL_GetTicks() - last_time);
			real_fps *= count;
			avrg_fps = avrg_fps*0.8 + real_fps*0.2;
			last_time = SDL_GetTicks();
			count = 0;
		}

		draw = true; // TODO - Set false if the game gets slow
	}

}

void stop() {
	
	running = false;

}

float get_fps() {
	
	return avrg_fps;

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

void logic() {

	SDL_Event event;

	while( SDL_PollEvent(&event) ) {
		if(event.type == SDL_QUIT)
			running = false;
		if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
			PDraw::adjust_screen();
	}

	PDraw::update(draw);
	PSound::update();
	
	if (debug) {
		fflush(stdout);
	}

}

}