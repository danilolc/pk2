//#########################
//PisteEngine - PisteInput
//by Janne Kivilahti from Piste Gamez
//#########################

#include "PisteInput.hpp"
#include "PisteDraw.hpp"

#ifdef USE_LOCAL_SDL
#include "SDL.h"
#include "SDL_image.h"
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#ifdef _WIN32
#include <Windows.h>
#endif
#include <cstring>

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

#define MOUSE_SPEED 20

struct PELIOHJAIN{
	SDL_Joystick* dev;
	bool					available;
	char					nimi[80];
};

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
/* VARIABLES ---------------------------------------------------------------------------------*/

PELIOHJAIN				PI_joysticks[PI_MAX_PELIOHJAIMIA];
int						PI_joystick_index = 0;

bool					PI_unload = true;

//Uint8 *m_keymap;
const Uint8 *m_keymap = SDL_GetKeyboardState(nullptr);
MOUSE mouse_pos;

SDL_Renderer* PI_Renderer = nullptr;

SDL_Haptic *PI_haptic;

/* METHODS -----------------------------------------------------------------------------------*/




static int Alloc_GuiId(){
	int gui_id = -1;
	for(int i = 0; i < PI_MAX_GUI; i++)
		if (!gui_list[i].set)
			gui_id = i;
	return gui_id;
}
static int UpdateGui(){
	PI_GUI* gui;
	SDL_Finger* finger = nullptr;
	SDL_TouchID id = SDL_GetTouchDevice(0);

	int fingers = SDL_GetNumTouchFingers(id);

	int x, y;
	for(int i = 0; i < PI_MAX_GUI; i++){
		gui = gui_list + i;
		gui->pressed = false;
		if(gui->set && gui->active && fingers > 0){
			for(int j = 0; j < fingers; j++){

				finger = SDL_GetTouchFinger(id, j);
				if(finger == nullptr){
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

	if(gui.texture != nullptr){
		SDL_DestroyTexture(gui.texture);
	}
	gui.texture = nullptr;
	if(strcmp(t_path, "") != 0){
		SDL_Surface* surface = IMG_Load(t_path);
		if(surface == nullptr){
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

		if(gui->set && gui->active && gui->texture != nullptr){
			rect.x = gui->pos_x;
			rect.y = gui->pos_y;
			rect.w = gui->width;
			rect.h = gui->height;
			SDL_SetTextureAlphaMod(gui->texture,(gui->alpha * pd_alpha) / 256);
			SDL_RenderCopy(PI_Renderer, gui->texture, nullptr, &rect);
		}
	}
	return 0;
}


int PisteInput_Vibrate(){
	if(PI_haptic == nullptr)
		return -1;
	if (SDL_HapticRumblePlay(PI_haptic,0.5,2000) != 0)
		return -1;
	return 0;
}

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
	SDL_Finger* finger = nullptr;
	SDL_TouchID id = SDL_GetTouchDevice(0);
	int fingers = SDL_GetNumTouchFingers(id);
	if (fingers == 0)
		return 1;

	finger = SDL_GetTouchFinger(id, 0);

	x = finger->x;
	y = finger->y;

	return 0;
}

bool PisteInput_Keydown(int key){
	UpdateGui();
	for(int i = 0; i < PI_MAX_GUI; i++)
		if(gui_list[i].set && gui_list[i].active && gui_list[i].pressed && gui_list[i].key != nullptr){
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
	if(SDL_GetTicks() - lastMouseUpdate > MOUSE_SPEED) {
		lastMouseUpdate = SDL_GetTicks();
		SDL_GetRelativeMouseState(&dx, &dy);
		mouse_pos.x += dx;
		mouse_pos.y += dy;
	}
	else {
		mouse_pos.x += dx;
		mouse_pos.y += dy;
		dx = 0;
		dy = 0;
	}

	if(keyMove){
		mouse_pos.x += PisteInput_Ohjain_X(PI_PELIOHJAIN_1)/30; //Move mouse with joystick
		mouse_pos.y += PisteInput_Ohjain_Y(PI_PELIOHJAIN_1)/30;

		if (PisteInput_Keydown(PI_LEFT)) mouse_pos.x -= 3; //Move mouse with keys
		if (PisteInput_Keydown(PI_RIGHT)) mouse_pos.x += 3;
		if (PisteInput_Keydown(PI_UP)) mouse_pos.y -= 3;
		if (PisteInput_Keydown(PI_DOWN)) mouse_pos.y += 3;
	}

	return mouse_pos;
}
int PisteInput_ActivateWindow(bool active) {
	if (active) {
		SDL_GetRelativeMouseState(nullptr, nullptr);
		SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else {
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
	return 0;
}
int PisteInput_Start(){
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);

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
	if (PI_haptic == nullptr)
		return -1;
	if (SDL_HapticRumbleInit(PI_haptic) != 0)
		return -1;
	return 0;
}

bool PisteInput_Hiiri_Vasen(){
	SDL_PumpEvents();
	return SDL_GetMouseState(nullptr, nullptr)&SDL_BUTTON(SDL_BUTTON_LEFT);
}

bool PisteInput_Hiiri_Oikea(){
	SDL_PumpEvents();
	return SDL_GetMouseState(nullptr, nullptr)&SDL_BUTTON(SDL_BUTTON_RIGHT);
}

int PisteInput_Ohjain_X(int ohjain){
	int x = 0;

	if (PI_joysticks[ohjain].available)
		x = SDL_JoystickGetAxis(PI_joysticks[ohjain].dev, 0);

	return x;
}

int PisteInput_Ohjain_Y(int ohjain){
	int y = 0;

	if (PI_joysticks[ohjain].available)
		y = SDL_JoystickGetAxis(PI_joysticks[ohjain].dev, 1);

	return y;
}

bool PisteInput_Ohjain_Nappi(int ohjain, int index){
	bool painettu = false;

	if (PI_joysticks[ohjain].available)
		painettu = SDL_JoystickGetButton(PI_joysticks[ohjain].dev, index);

	return painettu;
}

char *PisteInput_Ohjain_Nimi(int ohjain){
	return PI_joysticks[ohjain].nimi;
}

//Game controller
bool PisteInput_Hae_Ohjaimet(){
	bool ok = false;

	for(int ohjain=0; ohjain < PI_MAX_PELIOHJAIMIA; ohjain++)
	{
/*
		if (PI_joysticks[ohjain].available)
		{
			if (FAILED(PI_joysticks[ohjain].lpdijoy->GetDeviceState(sizeof(DIJOYSTATE),(LPVOID)&PI_joysticks[ohjain].joystick_state)))
			{
				PisteLog_Kirjoita("[Warning] Piste Input: Lost control of game pad! \n");
				PI_joysticks[ohjain].available = false;
			}
			if (PI_joysticks[ohjain].available)
				ok = true;
		}
*/
	}
	return ok;
}


bool PisteInput_Hae_Nappaimet(){
	bool ok = true;

	return ok;
}

bool PisteInput_Hae_Hiiri(){
	return true;
}

char PisteInput_Lue_Nappaimisto(void){
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
	//m_keymap = SDL_GetKeyState(nullptr);
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

int PisteInput_Exit(){
	return 0;
}
