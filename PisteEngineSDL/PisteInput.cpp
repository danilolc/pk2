/* 
 PisteEngineSDL - PisteInput
 <samuli.tuomola@gmail.com>
*/

/* INCLUDES ----------------------------------------------------------------------------------*/

#include "PisteInput.h"
#include "PisteLog.h"

#include <SDL/SDL.h>

/* DEFINES -----------------------------------------------------------------------------------*/

/* TYPE DEFINITIONS --------------------------------------------------------------------------*/

struct PELIOHJAIN
{
	SDL_Joystick* dev;
	bool					available;
	char					nimi[80];
};

/* VARIABLES ---------------------------------------------------------------------------------*/

PELIOHJAIN				PI_joysticks[PI_MAX_PELIOHJAIMIA];
int						PI_joystick_index = 0;

bool					PI_unload = true;

Uint8 *m_keymap;

/* METHODS -----------------------------------------------------------------------------------*/

bool PisteInput_Alusta_Ohjaimet()
{
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

int PisteInput_Alusta(HWND &main_window_handle, HINSTANCE &hinstance_app)
{
	if (PI_unload) {
	}

	return 0;
}

bool PisteInput_Keydown(int key)
{
	SDL_PumpEvents();
	m_keymap = SDL_GetKeyState(NULL);
	return m_keymap[key];
	//return key == m_event.key.keysym.sym;
}

bool PisteInput_Hiiri_Vasen()
{
	SDL_PumpEvents();
	return SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1);
}

bool PisteInput_Hiiri_Oikea()
{
	SDL_PumpEvents();
	return SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(2);
}

int lastMouseUpdate=0, mx=0, my=0;

int PisteInput_Hiiri_X(int x)
{
	// TODO: a bit hackish just to retain function structure
	if(SDL_GetTicks() - lastMouseUpdate > 50) {
		lastMouseUpdate = SDL_GetTicks();
		SDL_GetRelativeMouseState(&mx, &my);
		return x+mx;
	}
	int r = mx;
	mx = 0;
	return x+r;
}

int PisteInput_Hiiri_Y(int y)
{
	if(SDL_GetTicks() - lastMouseUpdate > 50) {
		lastMouseUpdate = SDL_GetTicks();
		SDL_GetRelativeMouseState(&mx, &my);
		return y+my;
	}
	int r = my;
	my = 0;
	return y+r;
}

int PisteInput_Ohjain_X(int ohjain)
{
	int x = 0;

	if (PI_joysticks[ohjain].available)
		x = SDL_JoystickGetAxis(PI_joysticks[ohjain].dev, 0);

	return x;
}

int PisteInput_Ohjain_Y(int ohjain)
{
	int y = 0;
	
	if (PI_joysticks[ohjain].available)
		y = SDL_JoystickGetAxis(PI_joysticks[ohjain].dev, 1);

	return y;
}

bool PisteInput_Ohjain_Nappi(int ohjain, int index)
{
	bool painettu = false;
	
	if (PI_joysticks[ohjain].available)
		painettu = SDL_JoystickGetButton(PI_joysticks[ohjain].dev, index);

	return painettu;
}

char *PisteInput_Ohjain_Nimi(int ohjain)
{
	return PI_joysticks[ohjain].nimi;
}

bool PisteInput_Hae_Ohjaimet()
{
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


bool PisteInput_Hae_Nappaimet()
{
	bool ok = true;
	
	return ok;
}

bool PisteInput_Hae_Hiiri()
{
	return true;
}

char PisteInput_Lue_Nappaimisto(void)
{
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		if(event.type==SDL_KEYDOWN) return event.key.keysym.sym;
	}
	return('\0');
}

UCHAR PisteInput_Lue_Kontrolli()
{
	return 0;
}

bool PisteInput_Lue_Eventti()
{
	SDL_PumpEvents();
	m_keymap = SDL_GetKeyState(NULL);
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

bool PisteInput_Lue_Kontrolli(int k)
{
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

char *PisteInput_Lue_Kontrollin_Nimi(UCHAR k)
{
//	SDL_GetKeyName( key->keysym.sym )
	return " ";
}

int PisteInput_Lopeta()
{
	return 0;
}
