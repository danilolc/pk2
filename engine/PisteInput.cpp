//#########################
//PisteEngine - PisteInput
//by Janne Kivilahti from Piste Gamez
//#########################

#include "PisteInput.h"

#include <SDL2/SDL.h>

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
/* VARIABLES ---------------------------------------------------------------------------------*/

PELIOHJAIN				PI_joysticks[PI_MAX_PELIOHJAIMIA];
int						PI_joystick_index = 0;

bool					PI_unload = true;

//Uint8 *m_keymap;
const Uint8 *m_keymap = SDL_GetKeyboardState(NULL);


/* METHODS -----------------------------------------------------------------------------------*/



bool PisteInput_Alusta_Ohjaimet(){
/*
SDL_InitSubSystem(SDL_INIT_JOYSTICK);

// Check for joystick
if(SDL_NumJoysticks()>0){
  // Open joystick
  joy=SDL_JoystickOpen(0);
  
  if(joy)
  {
    printf("Opened Joystick 0\n");
    printf("Name: %s\n", SDL_JoystickName(0));
    printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
    printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
    printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
  }
  else
    printf("Couldn't open Joystick 0\n");
  
  if(SDL_JoystickOpened(0))
    SDL_JoystickClose(joy);
	*/
	return true;
}

int PisteInput_Alusta(){
	return 0;
}

bool PisteInput_Keydown(int key){
	SDL_PumpEvents();
	return m_keymap[keylist[key]];
}

bool PisteInput_Hiiri_Vasen(){
	SDL_PumpEvents();
	return SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_LEFT);
}

bool PisteInput_Hiiri_Oikea(){
	SDL_PumpEvents();
	return SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_RIGHT);
}

MOUSE PisteInput_Hiiri(){
	SDL_PumpEvents();
	MOUSE pos;
	SDL_GetMouseState(&pos.x, &pos.y);

	return pos;
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
	while(SDL_PollEvent(&event)) {
		if(event.type==SDL_KEYDOWN) return event.key.keysym.sym;
	}
	return('\0');
}

char PisteInput_Lue_Kontrolli(){
	return 0;
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

bool PisteInput_Lue_Kontrolli(int k){
	/*
	bool pressed = false;
	for(int i=0; i<SDLK_LAST; i++)
	if(m_keymap[i])
	{
		printf("%i,",i);
		pressed = true;
	}
	if(pressed) printf(" <- is %i here?\n",k);
	*/
	//return m_keymap[k];
	return PisteInput_Keydown(k);
}

char* PisteInput_Lue_Kontrollin_Nimi(unsigned char k){
//	SDL_GetKeyName( key->keysym.sym )
	return " ";
}

int PisteInput_Lopeta(){
	return 0;
}
