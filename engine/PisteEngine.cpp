//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################
#include "PisteEngine.hpp"

#ifdef USE_LOCAL_SDL
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

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

using namespace std;

float avrg_fps;

bool debug = false;
bool draw = true;
bool ready = false;

void GDB_Break() {
	//Empty function called when press Q to use in GDB ("break GDB_Break()")
}

void EngineLogic(bool &running) {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT)
			running = false;
		if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
			PisteDraw2_AdjustScreen();
	}

	PisteSound_Update();
	PisteDraw2_Update(draw);

	if (debug) {
		if(PisteInput_Keydown(PI_Q)) GDB_Break();
		fflush(stdout);
	}
}

void Piste_IgnoreFrame() {
	draw = false;
}

float Piste_GetFPS() {
	return avrg_fps;
}

void Piste_SetDebug(bool set) {
	debug = set;
}

int Piste_Init(int width, int height, const char* name, const char* icon) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("Unable to init SDL: %s\n", SDL_GetError());
		return -1;
	}

	PisteDraw2_Start(width, height, name, icon);
	PisteInput_Start();
	PisteSound_Start();

	ready = true;
	return 0;
}

int Piste_Loop(bool &running, int (*GameLogic)()) {
	int last_time = 0;
	int count = 0; // Count how much frames elapsed
	float real_fps = 0;
	
	while(running) {
		count++;

		GameLogic();
		EngineLogic(running);

		if (draw) {
			real_fps = 1000.f / (SDL_GetTicks() - last_time);
			real_fps *= count;
			avrg_fps = avrg_fps*0.8 + real_fps*0.2;
			last_time = SDL_GetTicks();
			count = 0;
		}

		draw = true; // Set false if the game gets slow
	}

	return 0;
}

int Piste_Quit() {
	PisteDraw2_Exit();
	PisteInput_Exit();
	PisteSound_End();
	SDL_Quit();
	ready = false;
	return 0;
}
