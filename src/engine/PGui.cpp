//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "engine/PGui.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include "engine/platform.hpp"
#include "engine/PLog.hpp"

namespace PGui {

const int MAX_GUI = 15;

SDL_Renderer* renderer;

struct GUI {

	bool set;
	bool active;

	int pos_x, pos_y;
	int width, height;
	u8 alpha;
	SDL_Texture* texture;
	u32* key;
	bool pressed;
	
};

GUI gui_list[MAX_GUI];

int find_free() {

	int gui_id = -1;
	
	for(int i = 0; i < MAX_GUI; i++)
		if (!gui_list[i].set)
			return i;
	
	return -1;

}

int update(){

	GUI* gui;
	SDL_Finger* finger = nullptr;
	SDL_TouchID id = SDL_GetTouchDevice(0);

	int fingers = SDL_GetNumTouchFingers(id);

	for(int i = 0; i < MAX_GUI; i++){
		gui = gui_list + i;
		gui->pressed = false;
		if(gui->set && gui->active && fingers > 0){
			for(int j = 0; j < fingers; j++){

				finger = SDL_GetTouchFinger(id, j);
				if(finger == nullptr) {

					PLog::Write(PLog::ERROR, "PGui", SDL_GetError());
					SDL_ClearError();
				
				} else {

					int x = finger->x;
					int y = finger->y;
					if(x > gui->pos_x && x < gui->width+gui->pos_x && y > gui->pos_y && y < gui->height+gui->pos_y)
						gui->pressed = true;
					
				}
			}
		}
	}

	return 0;

}

void init(int w, int h, void* r) {
	
	renderer = (SDL_Renderer*) r;

}

int create(int x, int y, int w, int h, u8 alpha, PFile::Path path, u32* key) {

	int gui_id = find_free();
	if(gui_id == -1)
		return gui_id;

	GUI& gui = gui_list[gui_id];

	gui.pos_x = x;
	gui.pos_y = y;

	gui.width = w;
	gui.height = h;
	gui.alpha = alpha;
	gui.key = key;

	if(gui.texture != NULL) {

		SDL_DestroyTexture(gui.texture);
		gui.texture = NULL;
	
	}
	
	if(path != "") {

		PFile::RW* rw = path.GetRW("rb");
		SDL_Surface* surface = IMG_Load_RW((SDL_RWops*) rw, 0);
		PFile::CloseRW(rw);
		
		if(surface == NULL) {

			PLog::Write(PLog::ERROR, "PGui", IMG_GetError());
			SDL_ClearError();
			gui.set = false;
			return -1;
		
		}
		
		gui.texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	}

	gui.set = true;
	return gui_id;

}

int activate(int id, bool active){
	
	if(id >= MAX_GUI || id < 0)
		return -1;
	
	if(!gui_list[id].set)
		return -1;

	gui_list[id].active = active;
	return 0;

}

int draw(int pd_alpha) {

	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);
	
	float prop_x = (float)w / 1920;
	float prop_y = (float)h / 1080;

	for(int i = 0; i < MAX_GUI; i++){
		GUI* gui = &gui_list[i];

		if(gui->set && gui->active && gui->texture != NULL) {
			SDL_Rect rect;
			rect.x = gui->pos_x * prop_x;
			rect.y = gui->pos_y * prop_y;
			rect.w = gui->width * prop_x;
			rect.h = gui->height * prop_y;
			SDL_SetTextureAlphaMod(gui->texture, (gui->alpha * pd_alpha) / 256);
			SDL_RenderCopy(renderer, gui->texture, NULL, &rect);
		}
	}
	return 0;

}

bool check_key(u32 key) {

	update();
	for(int i = 0; i < MAX_GUI; i++)
		if(gui_list[i].set && gui_list[i].active && gui_list[i].pressed && gui_list[i].key != NULL){
			if(*gui_list[i].key == key)
				return true;
		}
	
	return false;

}

}