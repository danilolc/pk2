//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PInput.hpp"

#include "engine/PLog.hpp"
#include "engine/PDraw.hpp"
#include "engine/PRender.hpp"
#include "engine/PUtils.hpp"
#include "engine/platform.hpp"

#include <SDL.h>

#include <cstring>

#define MOUSE_SPEED 20

namespace PInput {

std::vector<touch_t> touchlist;

u16 vibration = 0;
float mouse_x, mouse_y;
int mouse_key;

SDL_Haptic *Haptic = nullptr;
SDL_GameController *Controller = nullptr;

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
	SDL_SCANCODE_Y,	SDL_SCANCODE_Z,

	SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4, 
	SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7, SDL_SCANCODE_8, 
	SDL_SCANCODE_9, SDL_SCANCODE_0,

	SDL_CONTROLLER_BUTTON_INVALID,

	SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B,
	SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_Y,
	
	SDL_CONTROLLER_BUTTON_BACK,
	SDL_CONTROLLER_BUTTON_GUIDE,
	SDL_CONTROLLER_BUTTON_START,
	
	SDL_CONTROLLER_BUTTON_LEFTSTICK,
	SDL_CONTROLLER_BUTTON_RIGHTSTICK,
	SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
	SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
	
	SDL_CONTROLLER_BUTTON_DPAD_UP,
	SDL_CONTROLLER_BUTTON_DPAD_DOWN,
	SDL_CONTROLLER_BUTTON_DPAD_LEFT,
	SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
	
	SDL_CONTROLLER_BUTTON_MAX

};

const char keynames[][20] = {

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
	"y","z",

	"1","2","3","4",
	"5","6","7","8",
	"9","0",

	"unknown joy key",

	"joy a", "joy b", "joy x", "joy y",
	"joy back", "joy guide", "joy start",
	"joy left stick", "joy right stick",
	"joy left shoulder", "joy right shoulder",
	"joy up", "joy down", "joy left", "joy right",

	"joy max"

};

const char* KeyName(u8 key) {

	if (key >= sizeof(keynames) / 16) 
		return keynames[UNKNOWN];
	
	return keynames[key];

}

u8 GetKeyKeyboard() {
	
	for(int key = 0; key < END_KEYBOARD; key++)
		if (Keydown(key))
			return key;
	
	return UNKNOWN;

}

u8 GetKeyController() {

	int count = sizeof(keylist)/sizeof(int);

	for(int key = END_KEYBOARD; key < count; key++)
		if (Keydown(key))
			return key;
	
	return UNKNOWN;
	
}

bool Keydown(u32 key) {
	
	if (key < END_KEYBOARD) {
	
		return keymap[keylist[key]];
	
	}

	if (key >= sizeof(keylist)/sizeof(int)) {

		PLog::Write(PLog::ERR, "PInput", "Unknown key %u", key);
		return false;

	}

	return SDL_GameControllerGetButton(Controller, (SDL_GameControllerButton)keylist[key]);

}

bool text_editing = false;
char keyboard_text[32];
int keyboard_key = 0;

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

void InjectKey(int key) {
	
	keyboard_key = key;

}

static bool accept_char(char c) {

	const char* chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.!? ";

	return strchr(chars, c);
	//return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
	//	|| c == '.' || c == '!' || c == '?' || c == ' '; //Just number, letter, space dot

}

// TODO - return just the key
int ReadKeyboard(char* c) {

	if (accept_char(keyboard_text[0])) {
		
		*c = keyboard_text[0];
	
	} else {

		*c = '\0';

	}
	
	keyboard_text[0] = '\0';

	int key = 0;

	if (keyboard_key == SDL_SCANCODE_DELETE)
		key = DEL;
	if (keyboard_key == SDL_SCANCODE_BACKSPACE)
		key = BACK;
	if (keyboard_key == SDL_SCANCODE_RETURN)
		key = RETURN;
	if (keyboard_key == SDL_SCANCODE_LEFT)
		key = LEFT;
	if (keyboard_key == SDL_SCANCODE_RIGHT)
		key = RIGHT;

	keyboard_key = 0;

	return key;

}

void SetVibration(u16 vib) {

	vibration = vib;

}

bool ControllerFound() {
	
	return Controller != nullptr;

}

int Vibrate(int length) {

	if (Controller != nullptr)
		if (SDL_GameControllerRumble(Controller, vibration, vibration, length) != 0)
			return -1;

	if (Haptic != nullptr)
		if (SDL_HapticRumblePlay(Haptic, 0.5, length) != 0)
			return -1;
	
	return 0;

}

#ifdef _WIN32

//TODO
void SetMousePosition(int x, int y) {
	/*int wx = 0, wy = 0;

	GetWindowPosition(&wx, &wy);
	SetCursorPos(x + wx, y+wy);*/
}


#else

void SetMousePosition(int x, int y) {
	//TODO
}

#endif

void GetWindowPosition(int* x, int* y) {

	//SDL_GetWindowPosition(x, y);
	
}

void UpdateTouch(){

	touchlist.clear();

	SDL_Finger* finger = nullptr;
	
	int devices = SDL_GetNumTouchDevices();

	for (int i = 0; i < devices; i++) {
		
		SDL_TouchID id = SDL_GetTouchDevice(i);

		int fingers = SDL_GetNumTouchFingers(id);

		for(int j = 0; j < fingers; j++){

			finger = SDL_GetTouchFinger(id, j);
			if(finger == nullptr) {

				PLog::Write(PLog::ERR, "PInput", SDL_GetError());
				SDL_ClearError();
			
			} else {

				touch_t touch;
				touch.id = finger->id;
				touch.pos_x = finger->x;
				touch.pos_y = finger->y;
				touchlist.push_back(touch);
				
			}
		}

	}

	#ifndef __ANDROID__
	if (mouse_key & SDL_BUTTON(SDL_BUTTON_LEFT)) {

		int w, h;
		PDraw::get_buffer_size(&w, &h);
		
		touch_t touch;
		touch.id = -1;
		touch.pos_x = mouse_x / w;
		touch.pos_y = mouse_y / h;
		touchlist.push_back(touch);

	}
	#endif

}

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

float GetAxis(int axis) {

	float fac = 1.f/32768;

	if (axis == 0)
		fac *= SDL_GameControllerGetAxis(Controller, SDL_CONTROLLER_AXIS_LEFTX);
	else if (axis == 1)
		fac *= SDL_GameControllerGetAxis(Controller, SDL_CONTROLLER_AXIS_LEFTY);
	else if (axis == 2)
		fac *= SDL_GameControllerGetAxis(Controller, SDL_CONTROLLER_AXIS_RIGHTX);
	else if (axis == 3)
		fac *= SDL_GameControllerGetAxis(Controller, SDL_CONTROLLER_AXIS_RIGHTY);
	else if (axis == 4)
		fac *= SDL_GameControllerGetAxis(Controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	else if (axis == 5)
		fac *= SDL_GameControllerGetAxis(Controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	else
		return 0.f;
	
	if (abs(fac) < 0.15)
		fac = 0.f;

	return fac;

}

void UpdateMouse(bool keyMove, bool relative) {

	static int last_x, last_y;
	static bool ignore_mouse = false;

	int sw, sh;
	PRender::get_window_size(&sw, &sh);

	int bw, bh;
	PDraw::get_buffer_size(&bw, &bh);

	int off_x, off_y;
	PDraw::get_offset(&off_x, &off_y);

	if (!relative || PUtils::Is_Mobile()) {
		SDL_SetRelativeMouseMode(SDL_FALSE);

		int tmpx, tmpy;
		mouse_key = SDL_GetMouseState(&tmpx, &tmpy);

		//Problem with fitScreen
		tmpx *= float(bw) / sw;
		tmpy *= float(bh) / sh;

		tmpx -= off_x;
		tmpy -= off_y;

		// Mouse moved
		if (abs(last_x - tmpx) > 0 || abs(last_y - tmpy) > 0)
			ignore_mouse = false;

		last_x = tmpx;
		last_y = tmpy;
		
		if (!ignore_mouse) {
			mouse_x = float(tmpx);
			mouse_y = float(tmpy);
		}
	
	} else {
		SDL_SetRelativeMouseMode(SDL_TRUE);

		ignore_mouse = false;

		int delta_x, delta_y;
		mouse_key = SDL_GetRelativeMouseState(&delta_x, &delta_y);

		mouse_x += 0.4 * delta_x;
		mouse_y += 0.4 * delta_y;

	}

	if(keyMove) {

		float delta_x = 0;
		float delta_y = 0;

		delta_x += GetAxis(0) * 3;
		delta_y += GetAxis(1) * 3;

		if (Keydown(JOY_LEFT))  delta_x += -3; //Move mouse with d-pad
		if (Keydown(JOY_RIGHT)) delta_x += 3;
		if (Keydown(JOY_UP))    delta_y += -3;
		if (Keydown(JOY_DOWN))  delta_y += 3;

		if (Keydown(LEFT))  delta_x += -3; //Move mouse with keys
		if (Keydown(RIGHT)) delta_x += 3;
		if (Keydown(UP))    delta_y += -3;
		if (Keydown(DOWN))  delta_y += 3;

		if (delta_x > 0.1 || delta_x < -0.1 || 
		    delta_y > 0.1 || delta_y < -0.1)
			ignore_mouse = true;

		mouse_x += delta_x;
		mouse_y += delta_y;
		
	}

	// set limits
	if (mouse_x < -off_x) mouse_x = -off_x;
	if (mouse_x > bw - off_x - 19) mouse_x = bw - off_x - 19;
	if (mouse_y < -off_y) mouse_y = -off_y;
	if (mouse_y > bh - off_y - 19) mouse_y = bh - off_y - 19;

}


bool MouseLeft(){
	
	return mouse_key & SDL_BUTTON(SDL_BUTTON_LEFT);

}

bool MouseRight(){
	
	return mouse_key & SDL_BUTTON(SDL_BUTTON_RIGHT);

}

static int init_haptic() {

	for (int i = 0; i < SDL_NumHaptics(); i++) {

		Haptic = SDL_HapticOpen(i);
		if (Haptic == nullptr)
			continue;
		
		if (SDL_HapticRumbleSupported(Haptic))
			if (SDL_HapticRumbleInit(Haptic) != 0)
				break;

		SDL_HapticClose(Haptic);
		Haptic = nullptr;

	}

	if (Haptic == nullptr) {
		
		PLog::Write(PLog::INFO, "PInput", "No haptic found");
		return -1;

	}

	return 0;

}


static int init_controller() {

	int start = 0;

	char* cont = getenv("PK2_CONTROLLER");
	if (cont)
		start = atoi(cont);

	for (int i = start; i < SDL_NumJoysticks(); ++i) {
		if (SDL_IsGameController(i)) {
			Controller = SDL_GameControllerOpen(i);
			if (Controller) {
				PLog::Write(PLog::DEBUG, "PInput", "Controller found: %s", SDL_GameControllerName(Controller));
				return 0;
			}
		}
	}

	PLog::Write(PLog::INFO, "PInput", "No Controller found");
	return -1;

}

int init() {

	init_haptic();
	init_controller();
	
	keymap = SDL_GetKeyboardState(NULL);

	return 0;
}

int update() {

	UpdateTouch();

	return 0;

}

int terminate() {

	if (Haptic != nullptr)
		SDL_HapticClose(Haptic);

	if (Controller != nullptr)
		SDL_GameControllerClose(Controller);

	return 0;

}

}