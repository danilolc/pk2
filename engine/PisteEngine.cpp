//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################

#include <SDL2/SDL.h>
#include <string>
#include "PisteEngine.h"

using namespace std;

#ifdef WINDOWS
	#include <windows.h>
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
	while(SDL_PollEvent(&event))
		if(event.type == SDL_QUIT)
				running = false;

	if(PisteInput_Lue_Kontrolli(PI_R))
		Piste_IgnoreFrame();

	//TODO - Test PisteDraw2
	PisteDraw_Paivita_Naytto(draw);
	PisteDraw2_Update(draw, d_time, real_fps);
	draw = true;

	if (debug){
		if(PisteInput_Lue_Kontrolli(PI_Q)) GDB_Break();
		fflush(stdout);
	}
	return 0;
}
int Setcwd(){
	char exepath[_MAX_PATH];
	int count, find;

	#ifdef WINDOWS
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
	FPS_ms = (int)1000.f/fps;
}

int Piste_Init(){

	Setcwd();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
		printf("Unable to init SDL: %s\n", SDL_GetError());
		return -1;
	}
	atexit(SDL_Quit);
	//TODO - Start PisteDraw2
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
	}
	return 0;
}
int Piste_Quit(){
	PisteDraw_Lopeta();
	PisteDraw2_Exit();
	PisteSound_End();
	SDL_Quit();
	ready = false;
	return 0;
}
