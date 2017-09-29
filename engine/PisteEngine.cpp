//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################

#include <SDL2/SDL.h>
#include <string>
#ifdef _WIN32
#include <direct.h>
#endif
#include "PisteEngine.h"

using namespace std;

#ifdef _WIN32
	#include "stdio.h"
	#include "stdlib.h"
	#include "winlite.h"
#else
	#define _MAX_PATH PATH_MAX
	#include <unistd.h>
	#include <limits.h>
#endif

SDL_Event event;

DWORD counter = 0;
DWORD last_time = 0;
float FPS_ms = 16.667;

int real_fps;
int d_time;

bool debug = true;
bool draw = true;
bool ready = false;

char* winName;

void GDB_Break(){
	//Empty function called when press Q to use in GDB ("break GDB_Break()")
}
int EngineLogic(bool &running){
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT)
			running = false;
		if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
			PisteDraw2_AdjustScreen();
	}

	PisteDraw2_Update(draw, d_time, real_fps);

	if (debug){
		if(PisteInput_Keydown(PI_Q)) GDB_Break();
		if(PisteInput_Keydown(PI_R)) Piste_IgnoreFrame();
		fflush(stdout);
	}
	return 0;
}
int Setcwd(){
	char exepath[_MAX_PATH];
	int count, find;

	#ifdef _WIN32
		string(exepath, GetModuleFileName(NULL, exepath, _MAX_PATH));
	#else
		count = readlink("/proc/self/exe", exepath, _MAX_PATH);
		if(count > 0) exepath[count] = '\0';
	#endif

	find = string(exepath).find_last_of("/\\");
  exepath[find] = '\0';

	chdir(exepath);
}

void Piste_IgnoreFrame(){
	draw = false;
}
void Piste_SetFPS(int fps){
	FPS_ms = (float)1000.f/fps;
}

int Piste_Init(int width, int height, const char* name){

	Setcwd();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
		printf("Unable to init SDL: %s\n", SDL_GetError());
		return -1;
	}
	atexit(SDL_Quit);

	PisteDraw2_Start(width, height, name);

	PisteInput_Alusta();

	PisteSound_Start();

	counter = SDL_GetTicks();
	ready = true;
	return 0;
}
int Piste_Loop(bool &running, int (*GameLogic)()){
	int time_1, time_2, time_3;

	while(running){
		time_1 = SDL_GetTicks();

			GameLogic();

		time_2 = SDL_GetTicks();

			EngineLogic(running);

		time_3 = SDL_GetTicks();

		counter = time_3 - time_1;
		if (counter < FPS_ms && draw)
			SDL_Delay(FPS_ms - counter);

		real_fps = (int)(1000.f/(SDL_GetTicks()-last_time));
		d_time = 100*(float)(time_3-time_2)/(SDL_GetTicks()-last_time);
		last_time = SDL_GetTicks();

		draw = true;
	}
	return 0;
}
int Piste_Quit(){
	PisteDraw2_Exit();
	PisteSound_End();
	SDL_Quit();
	ready = false;
	return 0;
}
