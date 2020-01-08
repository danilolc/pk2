//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "engine/PInput.hpp"

#include "engine/PGui.hpp"
#include "engine/PDraw.hpp"
#include "engine/PUtils.hpp"
#include "engine/platform.hpp"

#include <SDL.h>

#include <cstring>

#define MOUSE_SPEED 20

namespace PInput {

int mouse_x, mouse_y;

SDL_Haptic *Haptic = nullptr;

static const u8* keymap = nullptr;

const int keylist[] = {

	SDL_SCANCODE_UNKNOWN,

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

const char keynames[][16] = {

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

const char* KeyName(u8 key) {

	if (key >= sizeof(keynames) / 16) 
		return keynames[UNKNOWN];
	
	return keynames[key];

}

u8 GetKey() {
	
	int count = sizeof(keylist)/sizeof(int);

	for(int key = 0; key < count; key++)
		if(keymap[keylist[key]])
			return key;
	
	return UNKNOWN;

}

bool Keydown(int key) {
	
	return keymap[keylist[key]];

}

bool text_editing = false;
char keyboard_text[32];

void StartKeyboard() {

	SDL_StartTextInput();
	text_editing = true;
	keyboard_text[0] = '\0';
	
}

void EndKeyboard() {

	SDL_StopTextInput();
	text_editing = false;
	keyboard_text[0] = '\0';

}

bool Is_Editing() {

	return text_editing;

}

void InjectText(const char* text) {

	strcpy(keyboard_text, text);

}

static bool accept_char(char c) {

	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| c == '.' || c == '!' || c == '?' || c == ' '; //Just number, letter, space dot

}

int ReadKeyboard(char* buffer) {

	strcpy(buffer, keyboard_text);
	keyboard_text[0] = '\0';

	for (int i = 0; i < 32; i++) { //Clean buffer
		char c = buffer[i];

		if (c == '\0')
			return i;
	
		if(!accept_char(c))
			for (int j = i; j < 31; j++) 
				buffer[j] = buffer[j + 1];

	}

	buffer[0] = '\0';

	return 0;
}


int Vibrate(int length) {

	if(Haptic == NULL)
		return -1;
	if (SDL_HapticRumblePlay(Haptic, 0.5, length) != 0)
		return -1;
	
	return 0;
}

#ifdef _WIN32

void SetMousePosition(int x, int y) {
	int wx = 0, wy = 0;
	//PisteDraw2_GetWindowPosition(&wx, &wy); TODO

	SetCursorPos(x + wx, y+wy);
}

#else

void SetMousePosition(int x, int y) {
	//TODO
}

#endif

int GetTouchPos(float& x, float& y) {

	int devices = SDL_GetNumTouchDevices();
	
	SDL_TouchID id = 0;
	for (int i = 0; i == devices; i++) {

		id = SDL_GetTouchDevice(i);
		if (SDL_GetNumTouchFingers(id) != 0) {
			break;
		}
		
	}

	SDL_Finger* finger = SDL_GetTouchFinger(id, 0);
	if (finger == NULL)
			return -1;
	
	x = finger->x;
	y = finger->y;

	return 0;

}

void UpdateMouse(bool keyMove, bool relative) {

	int sw, sh;
	PDraw::get_resolution(&sw, &sh);

	int bw, bh;
	PDraw::get_buffer_size(&bw, &bh);

	int off = PDraw::get_xoffset();

	if (!relative || PUtils::Is_Mobile()) {
		SDL_SetRelativeMouseMode(SDL_FALSE);
		SDL_GetMouseState(&mouse_x, &mouse_y);

		//Problem with fitScreen
		mouse_x *= float(bw) / sw;
		mouse_y *= float(bh) / sh;
		mouse_x -= off;
	
	} else {

		SDL_SetRelativeMouseMode(SDL_TRUE);

		int delta_x, delta_y;
		SDL_GetRelativeMouseState(&delta_x, &delta_y);

		mouse_x += delta_x * 0.8;
		mouse_y += delta_y * 0.8;

		if(keyMove) {

			//mouse_x += PInput::Ohjain_X(PI_PELIOHJAIN_1)/30; //Move mouse with joystick
			//mouse_y += PInput::Ohjain_Y(PI_PELIOHJAIN_1)/30;

			if (Keydown(LEFT))  mouse_x += -3; //Move mouse with keys
			if (Keydown(RIGHT)) mouse_x += 3;
			if (Keydown(UP))    mouse_y += -3;
			if (Keydown(DOWN))  mouse_y += 3;
		
		}
	}
	
	// set limits
	if (mouse_x < -off) mouse_x = -off;
	if (mouse_x > bw - off - 19) mouse_x = bw - off - 19;
	if (mouse_y < 0) mouse_y = 0;
	if (mouse_y > bh - 19) mouse_y = 480 - 19;

}


bool MouseLeft(){
	
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);

}

bool MouseRight(){
	
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);

}

static int init_haptic() {

	for (int i = 0; i < SDL_NumHaptics(); i++) {

		Haptic = SDL_HapticOpen(i);
		SDL_HapticRumbleSupported(Haptic);
		break;

		SDL_HapticClose(Haptic);
		Haptic = nullptr;

	}

	if (Haptic == nullptr)
		return -1;
	
	if (SDL_HapticRumbleInit(Haptic) != 0)
		return -1;

	return 0;

} 

int init() {

	init_haptic();
	
	keymap = SDL_GetKeyboardState(NULL);
	
	return 0;
}


int terminate() {

	if (Haptic != NULL)
		SDL_HapticClose(Haptic);

	return 0;

}

}