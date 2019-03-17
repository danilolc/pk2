//#########################
//PisteEngine - PisteInput
//by Janne Kivilahti from Piste Gamez
//#########################

#include "PisteInput.hpp"
#include "PisteDraw.hpp"
#include "PisteLog.hpp"

#include <SDL_image.h>

#include <cstring>
#include <iostream>

#define PI_MAX_GUI 15

struct PI_GUI{
	bool set;
	bool active;

	int pos_x, pos_y;
	int width, height;
	BYTE alpha;
	SDL_Texture* texture;
	DWORD* key;
	bool pressed;
};

PI_GUI gui_list[PI_MAX_GUI];
int screen_w, screen_h;

#define MOUSE_SPEED 1

SDL_GameController* controller;

const int keylist[] = {
	SDL_SCANCODE_F1,	SDL_SCANCODE_F2,	SDL_SCANCODE_F3,
	SDL_SCANCODE_F4,	SDL_SCANCODE_F5,	SDL_SCANCODE_F6,
	SDL_SCANCODE_F7,	SDL_SCANCODE_F8,	SDL_SCANCODE_F9,
	SDL_SCANCODE_F10,	SDL_SCANCODE_F11,	SDL_SCANCODE_F12,

	SDL_SCANCODE_ESCAPE,	SDL_SCANCODE_RETURN,
	SDL_SCANCODE_BACKSPACE,	SDL_SCANCODE_SPACE,
	SDL_SCANCODE_DELETE,	SDL_SCANCODE_END,
	SDL_SCANCODE_TAB,

	SDL_SCANCODE_LALT,		SDL_SCANCODE_RALT,
	SDL_SCANCODE_LCTRL,		SDL_SCANCODE_RCTRL,
	SDL_SCANCODE_LSHIFT,	SDL_SCANCODE_RSHIFT,

	SDL_SCANCODE_LEFT,	SDL_SCANCODE_RIGHT,
	SDL_SCANCODE_UP,	SDL_SCANCODE_DOWN,

	SDL_SCANCODE_A,	SDL_SCANCODE_B,	SDL_SCANCODE_C,	SDL_SCANCODE_D,
	SDL_SCANCODE_E,	SDL_SCANCODE_F,	SDL_SCANCODE_G,	SDL_SCANCODE_H,
	SDL_SCANCODE_I,	SDL_SCANCODE_J,	SDL_SCANCODE_K,	SDL_SCANCODE_L,
	SDL_SCANCODE_M,	SDL_SCANCODE_N,	SDL_SCANCODE_O,	SDL_SCANCODE_P,
	SDL_SCANCODE_Q,	SDL_SCANCODE_R,	SDL_SCANCODE_S,	SDL_SCANCODE_T,
	SDL_SCANCODE_U,	SDL_SCANCODE_V,	SDL_SCANCODE_W,	SDL_SCANCODE_X,
	SDL_SCANCODE_Y,	SDL_SCANCODE_Z
};

const char keynames[][15] = {
	"unknown key",

	"f1","f2","f3",
	"f4","f5","f6",
	"f7","f8","f9",
	"f10","f11","f12",

	"escape","return",
	"backspace","space",
	"delete","end",
	"tab",

	"left alt","right alt",
	"left ctrl","right ctrl",
	"left shift","right shift",

	"arrow left","arrow right",
	"arrow up","arrow down",

	"a","b","c","d",
	"e","f","g","h",
	"i","j","k","l",
	"m","n","o","p",
	"q","r","s","t",
	"u","v","w","x",
	"y","z"
};

bool					PI_unload = true;

//Uint8 *m_keymap;
const Uint8 *m_keymap = SDL_GetKeyboardState(NULL);
MOUSE mouse_pos;

SDL_Renderer* PI_Renderer = NULL;

SDL_Haptic *PI_haptic;

int Alloc_GuiId(){
	int gui_id = -1;
	for(int i = 0; i < PI_MAX_GUI; i++)
		if (!gui_list[i].set)
			gui_id = i;
	return gui_id;
}

int UpdateGui(){
	PI_GUI* gui;
	SDL_Finger* finger = NULL;
	SDL_TouchID id = SDL_GetTouchDevice(0);

	int fingers = SDL_GetNumTouchFingers(id);

	int x, y;
	for(int i = 0; i < PI_MAX_GUI; i++){
		gui = gui_list + i;
		gui->pressed = false;
		if(gui->set && gui->active && fingers > 0){
			for(int j = 0; j < fingers; j++){

				finger = SDL_GetTouchFinger(id, j);
				if(finger == NULL){
					printf("Can't find finger %i - %s", j, SDL_GetError());
					SDL_ClearError();
				} else{
					x = finger->x * screen_w;
					y = finger->y * screen_h;
					if(x > gui->pos_x && x < gui->width+gui->pos_x && y > gui->pos_y && y < gui->height+gui->pos_y)
						gui->pressed = true;
				}
			}
		}
	}
	return 0;
}

int PisteInput_CreateGui(int x, int y, int w, int h, BYTE alpha, const char* t_path, DWORD* key){
	int gui_id = Alloc_GuiId();
	if(gui_id == -1)
		return gui_id;

	PI_GUI& gui = gui_list[gui_id];

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
		gui.texture = SDL_CreateTextureFromSurface(PI_Renderer, surface);
	}
	gui.set = true;
	return gui_id;
}

int PisteInput_ActiveGui(int id, bool active){
	if(id >= PI_MAX_GUI || id < 0)
		return -1;
	if(!gui_list[id].set)
		return -1;
	gui_list[id].active = active;
	return 0;
}

int PisteInput_DrawGui(int pd_alpha){
	PI_GUI* gui;
	SDL_Rect rect;
	for(int i = 0; i < PI_MAX_GUI; i++){
		gui = gui_list + i;

		if(gui->set && gui->active && gui->texture != NULL){
			rect.x = gui->pos_x;
			rect.y = gui->pos_y;
			rect.w = gui->width;
			rect.h = gui->height;
			SDL_SetTextureAlphaMod(gui->texture,(gui->alpha * pd_alpha) / 256);
			SDL_RenderCopy(PI_Renderer, gui->texture, NULL, &rect);
		}
	}
	return 0;
}


int PisteInput_Vibrate(){
	if(PI_haptic == NULL)
		return -1;
	if (SDL_HapticRumblePlay(PI_haptic,0.5,2000) != 0)
		return -1;
	return 0;
}

#ifdef _WIN32
void SetMousePosition(int x, int y) {
	int wx, wy;
	PisteDraw2_GetWindowPosition(&wx, &wy);

	//SetCursorPos(x + wx, y+wy);
}
#else
void SetMousePosition(int x, int y) {
	//TODO
}
#endif


const char* PisteInput_KeyName(BYTE key){
	if(key >= sizeof(keynames) / 15) return keynames[0];
	return keynames[key+1];
}
BYTE PisteInput_GetKey(){
	SDL_PumpEvents();
	int key;
	int count = sizeof(keylist)/sizeof(int);

	for(key=0; key < count; key++)
		if(m_keymap[keylist[key]]) return key;
	return 0;
}
int PisteInput_GetTouchPos(float& x, float& y){
	SDL_Finger* finger = NULL;
	SDL_TouchID id = SDL_GetTouchDevice(0);
	int fingers = SDL_GetNumTouchFingers(id);
	if (fingers == 0)
		return 1;

	finger = SDL_GetTouchFinger(id, 0);

	x = finger->x;
	y = finger->y;

	return 0;
}

bool PisteInput_Gamepad_Button(int button) {
	return SDL_GameControllerGetButton(controller, static_cast<SDL_GameControllerButton>(button));
}

bool PisteInput_Keydown(int key){
	UpdateGui();
	for(int i = 0; i < PI_MAX_GUI; i++)
		if(gui_list[i].set && gui_list[i].active && gui_list[i].pressed && gui_list[i].key != NULL){
			if(*gui_list[i].key == key)
				return true;
		}

	SDL_PumpEvents();
	return m_keymap[keylist[key]];
}

//TODO - change names - fullscreen uses relative mouse
MOUSE PisteInput_UpdateMouse(bool keyMove, bool relative){
	static int was_relative = -1;

	if (was_relative == -1) { //Was just initialized
		PisteInput_ActivateWindow(relative);
		was_relative = (int)relative;
	}
	if (was_relative == 1 && !relative) { //Was relative but now it isn't
		PisteInput_ActivateWindow(false);
		was_relative = 0;
	}
	if (was_relative == 0 && relative) { //Wasn't relative but now it is
		PisteInput_ActivateWindow(true);
		was_relative = 1;
	}

	if (!relative) {
		SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
		return mouse_pos;
	}

	static int lastMouseUpdate = 0, dx = 0, dy = 0;
	if (SDL_GetTicks() - lastMouseUpdate > MOUSE_SPEED) {
		lastMouseUpdate = SDL_GetTicks();
		SDL_GetRelativeMouseState(&dx, &dy);

		mouse_pos.x += dx;
		mouse_pos.y += dy;
	} else {
		mouse_pos.x += dx;
		mouse_pos.y += dy;

		dx = 0;
		dy = 0;
	}

	if(keyMove){
		if (PisteInput_Keydown(PI_LEFT)) mouse_pos.x -= 1;
		if (PisteInput_Keydown(PI_RIGHT)) mouse_pos.x += 1;
		if (PisteInput_Keydown(PI_UP)) mouse_pos.y -= 1;
		if (PisteInput_Keydown(PI_DOWN)) mouse_pos.y += 1;

		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) mouse_pos.x -= 1;
		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) mouse_pos.x += 1;
		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP)) mouse_pos.y -= 1;
		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) mouse_pos.y += 1;
	}

	return mouse_pos;
}
int PisteInput_ActivateWindow(bool active) {
	if (active) {
		SDL_GetRelativeMouseState(NULL, NULL);
		SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else {
		SDL_SetRelativeMouseMode(SDL_FALSE);
		SetMousePosition(mouse_pos.x, mouse_pos.y);
	}
	return 0;
}

int PisteInput_Start(){
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);

	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		if (SDL_IsGameController(i)) {
			controller = SDL_GameControllerOpen(i);

			if (controller != nullptr) {
				PisteLog_Write("PisteInput", "Gamepad connect", TYPE::T_INFO);
			}
		}
	}

	screen_w = (int)DM.w;
	screen_h = (int)DM.h;
	int x = screen_h;
	if(screen_w < screen_h){
		screen_h = screen_w;
		screen_w = x;
	}

	PI_Renderer = (SDL_Renderer*) PisteDraw2_GetRenderer();
	for(int i = 0; i < PI_MAX_GUI; i++)
		gui_list[i].set = false;

	PI_haptic = SDL_HapticOpen(0);
	if (PI_haptic == NULL)
		return -1;
	if (SDL_HapticRumbleInit(PI_haptic) != 0)
		return -1;
	return 0;
}

bool PisteInput_Hiiri_Vasen(){
	SDL_PumpEvents();
	return SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_LEFT);
}

bool PisteInput_Hiiri_Oikea(){
	SDL_PumpEvents();
	return SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_RIGHT);
}

std::string PisteInput_Gamepad_Name(){
	return SDL_GameControllerName(controller);
}

bool PisteInput_Hae_Nappaimet(){
	bool ok = true;

	return ok;
}

bool PisteInput_Hae_Hiiri(){
	return true;
}

char PisteInput_Get_Typed_Char() {
	SDL_Event event;
	char c, str[5];
	while(SDL_PollEvent(&event)) {
		if(event.type== SDL_TEXTINPUT) {
			strcpy(str, event.text.text);
			c = str[0];
			if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
				|| c == '.' || c == '!' || c == '?' || c == ' ') //Just number, letter, space dot
				return c;
		}
	}
	return('\0');
}



bool PisteInput_Lue_Eventti(){
	SDL_PumpEvents();
	//m_keymap = SDL_GetKeyState(NULL);
/*
	m_event.key.keysym.sym = SDLK_UNKNOWN;
	while(SDL_PollEvent(&m_event)) {
		if(m_event.type == SDL_KEYDOWN) {
			printf("presed %i ralt:%i\n", m_event.key.keysym.sym, SDLK_RALT);
			return true;
		}
	}
*/
	return false;
}

char* PisteInput_Lue_Kontrollin_Nimi(unsigned char k){
//	SDL_GetKeyName( key->keysym.sym )
	return " ";
}

int PisteInput_Exit(){
	SDL_GameControllerClose(controller);

	return 0;
}
