//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################
#include "PisteEngine.hpp"

#ifdef USE_LOCAL_SDL
#include "SDL.h"
#else
#include <SDL.h>
#endif

#include <SDL_mixer.h>

#include <string>

#ifdef _WIN32
	#include <direct.h>
	#include <cstdio>
	#include <cstdlib>
	#include "../include/winlite.h"
#else
	#include <unistd.h>
	#include <limits.h>
#endif

#include <iostream>

#include "PisteTimer.h"

using namespace Piste;

const int FPS = 60;
const int TICKS_PER_FRAME = 1000 / FPS;

PisteTimer fpsTimer;
PisteTimer capTimer;

Game::Game(int width, int height, const char* name, const char* icon) {
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
		printf("Unable to init SDL: %s\n", SDL_GetError());
		return;
	}

	PisteDraw2_Start(width, height, name, icon);
	PisteInput_Start();
	PisteSound_Start();

	ready = true;
	running = true;

	fpsTimer.start();
}

Game::~Game() {
	
	PisteDraw2_Exit();
	PisteInput_Exit();
	PisteSound_End();

	Mix_Quit();
	SDL_Quit();
	ready = false;

}

int countedFrames = 0;

void Game::loop(int (*GameLogic)()) {
	int last_time = 0;
	int count = 0; // Count how much frames elapsed
	float real_fps = 0;
	
	//running = true;

	while(running) {
		capTimer.start();

		avgFps = countedFrames / (fpsTimer.getTicks() / 1000.f);

		if (avgFps > 2000000) {
			avgFps = 0;
		}

		GameLogic();
		logic();
		draw = true;

		int frameTicks = capTimer.getTicks();
		if (frameTicks < TICKS_PER_FRAME) {
			SDL_Delay(TICKS_PER_FRAME - frameTicks);
		}

		++countedFrames;

		/*
		if (draw) {
			real_fps = 1000.f / (SDL_GetTicks() - last_time);
			real_fps *= count;
			avrg_fps = avrg_fps * 0.8 + real_fps * 0.2;
			last_time = SDL_GetTicks();
			count = 0;
		}

		draw = true; // TODO - Set false if the game gets slow*/
	}

}

void Game::stop() {
	
	running = false;

}

float Game::get_fps() {
	
	return avgFps;

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
		if (event.type == SDL_CONTROLLERBUTTONDOWN) {
			PisteInput_Gamepad_Pressed(event.cbutton.button, true);
		} else if (event.type == SDL_CONTROLLERBUTTONUP) {
			PisteInput_Gamepad_Pressed(event.cbutton.button, false);
		}
	}

	PisteSound_Update();
	PisteDraw2_Update(draw);

	/*
	if (debug) {
		//if ( PisteInput_Keydown(PI_Q) ) GDB_Break();
		fflush(stdout);
	}*/

}
