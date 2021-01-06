//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/Piste.hpp"

#include "engine/platform.hpp"

#include <SDL.h>

#include <iostream>

#define UPDATE_FPS 30 //Update FPS each 30 frames

namespace Piste {

static int desired_fps = 60;

static bool ready = false;
static bool running = false;

static float avrg_fps = 0;

static bool debug = false;
static bool draw = true;

static SDL_Thread* game_thread = NULL;
static SDL_mutex* mutex = NULL;
static SDL_cond* process_game = NULL;
static SDL_cond* end_game_process = NULL;
static bool game_process_done = true;

static void wait_frame() {

	static u64 last_time = SDL_GetPerformanceCounter();

	u64 c_frec = SDL_GetPerformanceFrequency();

	u64 one_frame = c_frec / desired_fps;
	u64 exit_time = last_time + one_frame;
	
	u64 curr_time = SDL_GetPerformanceCounter();
	
	if (curr_time > exit_time) {
		last_time = curr_time;
		return;
	}
	
	u64 wait_time = exit_time - curr_time;
	u32 ms = wait_time * 1000 / c_frec;

	SDL_Delay(ms);

	while(1) {

		u64 c = SDL_GetPerformanceCounter();
		
		if (c >= exit_time || c < last_time)
			break;

	}

	last_time = exit_time;

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

	mutex = SDL_CreateMutex();
	process_game = SDL_CreateCond();
	end_game_process = SDL_CreateCond();

	sdl_show();
	
	PDraw::init(width, height, true);
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

static int game_thread_func(void* func) {

	int (*GameLogic)() = (int (*)())func;

	while(1) {

		//PLog::Write(PLog::INFO, "LOG", "Waiting new frame");

		SDL_LockMutex(mutex);
		while(game_process_done)
			SDL_CondWait(process_game, mutex);
		SDL_UnlockMutex(mutex);

		if (!running) break;

		int ret = GameLogic();
		
		SDL_LockMutex(mutex);

		if (ret) running = false;
		game_process_done = true;
		
		SDL_CondSignal(end_game_process);
		SDL_UnlockMutex(mutex);

	}

}

void loop(int (*GameLogic)()) {
	
	static int frame_counter = 0;
	static u64 last_time = 0;

	game_thread = SDL_CreateThread(game_thread_func, "Game Thread", (void*)GameLogic);
	running = true;

	while(running) {

		//PLog::Write(PLog::INFO, "LOG", "New frame");

		const bool will_draw = draw;
		void* buffer8;
		int alpha;

		if (will_draw)
			PDraw::get_buffer_data(&buffer8, &alpha);
		

		SDL_LockMutex(mutex);
		game_process_done = false;
		SDL_CondSignal(process_game);
		SDL_UnlockMutex(mutex);
		
		if (will_draw) {
			//PLog::Write(PLog::INFO, "LOG", "Render on %p", buffer8);
			PRender::update(buffer8, alpha);

		}
		

		//TODO wait game thread
		SDL_LockMutex(mutex);
		while (!game_process_done) {

			SDL_CondWait(end_game_process, mutex);

		}
		SDL_UnlockMutex(mutex);

		// Clear PDraw buffer
		PDraw::update();
		PInput::update();
		PSound::update();

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

		if (!PRender::is_vsync() && (desired_fps > 0) && draw)
			wait_frame();

		if (debug) {
			fflush(stdout);
		}

		frame_counter++;
		if (frame_counter >= UPDATE_FPS) {

			frame_counter = 0;
			u64 elapsed_time = (SDL_GetPerformanceCounter() - last_time);

			avrg_fps = double(SDL_GetPerformanceFrequency() * UPDATE_FPS) / elapsed_time;	

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

	desired_fps = fps;

	// Vsync true
	if (fps == -1)
		PRender::set_vsync(true);
	else
		PRender::set_vsync(false);
	

}

void ignore_frame() {

	draw = false;

}

bool is_ready() {

	return ready;

}

}
