//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "engine/PGui.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include "engine/platform.hpp"


extern int mouse_x; //TODO remove
extern int mouse_y;
#include "engine/PInput.hpp"


namespace PGui {

const int MAX_GUI = 15;

int screen_w, screen_h;
SDL_Renderer* renderer;

struct GUI {

	bool set;
	bool active;

	int pos_x, pos_y;
	int width, height;
	BYTE alpha;
	SDL_Texture* texture;
	DWORD* key;
	bool pressed;
	
};

GUI gui_list[MAX_GUI];

int alloc() {

	int gui_id = -1;
	for(int i = 0; i < MAX_GUI; i++)
		if (!gui_list[i].set)
			gui_id = i;
	return gui_id;

}

int updatedev() {
	GUI* gui;
	
	for(int i = 0; i < MAX_GUI; i++){
		gui = gui_list + i;
		gui->pressed = false;
		if(gui->set && gui->active && PisteInput_Hiiri_Oikea()) {

			int x = mouse_x;
			int y = mouse_y;
			if(x > gui->pos_x && x < gui->width+gui->pos_x && y > gui->pos_y && y < gui->height+gui->pos_y)
				gui->pressed = true;
		}
	}

	return 0;

}

int update(){

	return updatedev();

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
				if(finger == nullptr){
					printf("Can't find finger %i - %s", j, SDL_GetError());
					SDL_ClearError();
				} else{
					int x = finger->x * screen_w;
					int y = finger->y * screen_h;
					if(x > gui->pos_x && x < gui->width+gui->pos_x && y > gui->pos_y && y < gui->height+gui->pos_y)
						gui->pressed = true;
				}
			}
		}
	}
	return 0;
}

void init(int w, int h, void* r) {
	screen_w = w;
	screen_h = h;

	renderer = (SDL_Renderer*) r;
}

int create(int x, int y, int w, int h, BYTE alpha, const char* t_path, DWORD* key){
	int gui_id = alloc();
	if(gui_id == -1)
		return gui_id;

	GUI& gui = gui_list[gui_id];

	float prop_x = (float)screen_w / 1920;
	float prop_y = (float)screen_h / 1080;

	gui.pos_x = x * prop_x;
	gui.pos_y = y * prop_y;

	gui.width = w * prop_x;
	gui.height = h * prop_x;
	gui.alpha = alpha;
	gui.key = key;

	if(gui.texture != NULL){
		SDL_DestroyTexture(gui.texture);
	}
	gui.texture = NULL;
	if(strcmp(t_path, "") != 0){
		SDL_Surface* surface = IMG_Load(t_path);
		if(surface == NULL){
			printf("Can't load image. SDL Image: %s\n",IMG_GetError());
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

int draw(int pd_alpha){
	GUI* gui;
	SDL_Rect rect;
	for(int i = 0; i < MAX_GUI; i++){
		gui = gui_list + i;

		if(gui->set && gui->active && gui->texture != NULL){
			rect.x = gui->pos_x;
			rect.y = gui->pos_y;
			rect.w = gui->width;
			rect.h = gui->height;
			SDL_SetTextureAlphaMod(gui->texture,(gui->alpha * pd_alpha) / 256);
			SDL_RenderCopy(renderer, gui->texture, NULL, &rect);
		}
	}
	return 0;
}

bool check_key(int key) {

	update();
	for(int i = 0; i < MAX_GUI; i++)
		if(gui_list[i].set && gui_list[i].active && gui_list[i].pressed && gui_list[i].key != NULL){
			if(*gui_list[i].key == key)
				return true;
		}
	
	return false;

}

}