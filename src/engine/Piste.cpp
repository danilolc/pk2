//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "engine/Piste.hpp"

#include "engine/platform.hpp"

#include <SDL.h>

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
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("But we are linking against SDL version %d.%d.%d.\n",
       linked.major, linked.minor, linked.patch);
	
}


void init(int width, int height, const char* name, const char* icon) {
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("Unable to init SDL: %s\n", SDL_GetError());
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
	
	int error = 0;
	int last_time = 0;
	int count = 0; // Count how much frames elapsed
	float real_fps = 0;
	
	running = true;

	while(running) {

		count++;

		error = GameLogic();
		if (error) break;
		
		logic();

		// Count fps
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

}