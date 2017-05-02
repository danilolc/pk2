//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################

#include <SDL2/SDL.h>
#include "PisteEngine.h"

SDL_Event event;

DWORD counter = 0;
DWORD last_time = 0;
int FPS = 60;

bool draw = true;
bool ready = false;

void GDB_Break(){
	//Empty function called when press Q to use in GDB ("break GDB_Break()")
}
bool Piste_GDB = true;

void PisteDraw_Paivita_Naytto(bool draw);

int Piste_Init(){
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

int Piste_Loop(bool &running,int (*Game)()){
	while(running){

		Game();

		while(SDL_PollEvent(&event))
			if(event.type == SDL_QUIT)
					running = false;

		if(PisteInput_Lue_Kontrolli(PI_R))
			Piste_IgnoreFrame();


		last_time = SDL_GetTicks();
		if (last_time < counter + (1000 / FPS) && draw)
			SDL_Delay(counter + (1000 / FPS) - last_time);

		//TODO - Test PisteDraw2
		PisteDraw_Paivita_Naytto(draw);
		PisteDraw2_Update(draw);

		draw = true;

		if(PisteInput_Lue_Kontrolli(PI_Q) && Piste_GDB)
			GDB_Break();

		counter = SDL_GetTicks();

		fflush(stdout);
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

int Piste_IgnoreFrame(){
	draw = false;
	return 0;
}

void Piste_SetFPS(int fps){
	FPS = fps;
}
