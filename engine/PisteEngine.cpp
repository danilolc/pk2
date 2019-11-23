//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################
#include "PisteEngine.hpp"

#include <SDL.h>

#include <string>

#ifdef _WIN32
	#include <direct.h>
	#include "stdio.h"
	#include "stdlib.h"
	#include "winlite.h"
#else
	#include <unistd.h>
	#include <limits.h>
#endif

using namespace Piste;


Game::Game(int width, int height, const char* name, const char* icon) {
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("Unable to init SDL: %s\n", SDL_GetError());
		return;
	}

	PisteDraw2_Start(width, height, name, icon);
	PisteInput_Start();
	PisteSound_Start();

	ready = true;

}

Game::~Game() {
	
	PisteDraw2_Exit();
	PisteInput_Exit();
	PisteSound_End();
	SDL_Quit();
	ready = false;

}

void Game::loop(int (*GameLogic)()) {
	
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

void Game::stop() {
	
	running = false;

}

float Game::get_fps() {
	
	return avrg_fps;

}

void Game::set_debug(bool set) {

	debug = set;

}

void Game::ignore_frame() {

	draw = false;

}

bool Game::is_ready() {

	return ready;

}

void Game::logic() {

	SDL_Event event;

	while( SDL_PollEvent(&event) ) {
		if(event.type == SDL_QUIT)
			running = false;
		if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
			PisteDraw2_AdjustScreen();
	}

	PisteSound_Update();
	PisteDraw2_Update(draw);

	if (debug) {
		//if ( PisteInput_Keydown(PI_Q) ) GDB_Break();
		fflush(stdout);
	}

}
