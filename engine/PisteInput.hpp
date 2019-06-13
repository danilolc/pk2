//#########################
//PisteEngine - PisteInput
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef P_INPUT
#define P_INPUT

#include "platform.hpp"
#include <SDL.h>
#include <string>

#define		PI_VIRHE -1000

#define		PI_MAX_PELIOHJAIMIA 2

#define		PI_PELIOHJAIN_1	  0
#define		PI_PELIOHJAIN_2	  1

#define		PI_OHJAIN_XY	  100
#define		PI_OHJAIN_NAPPI_1 0
#define		PI_OHJAIN_NAPPI_2 1
#define		PI_OHJAIN_NAPPI_3 2
#define		PI_OHJAIN_NAPPI_4 3
#define		PI_OHJAIN_NAPPI_5 4
#define		PI_OHJAIN_NAPPI_6 5

#define		PI_OHJAIN1_VASEMMALLE	110
#define		PI_OHJAIN1_OIKEALLE		111
#define		PI_OHJAIN1_YLOS			112
#define		PI_OHJAIN1_ALAS			113
#define		PI_OHJAIN1_NAPPI1		114
#define		PI_OHJAIN1_NAPPI2		115
#define		PI_OHJAIN1_NAPPI3		116
#define		PI_OHJAIN1_NAPPI4		117
#define		PI_OHJAIN1_NAPPI5		118
#define		PI_OHJAIN1_NAPPI6		119

enum PI_KEY {
	PI_F1,	PI_F2,	PI_F3,
	PI_F4,	PI_F5,	PI_F6,
	PI_F7,	PI_F8,	PI_F9,
	PI_F10,	PI_F11,	PI_F12,

	PI_ESCAPE,	PI_RETURN,
	PI_BACK,	PI_SPACE,
	PI_DELETE,	PI_END,
	PI_TAB,

	PI_LALT,		PI_RALT,
	PI_LCONTROL,	PI_RCONTROL,
	PI_LSHIFT,		PI_RSHIFT,

	PI_LEFT,	PI_RIGHT,
	PI_UP,		PI_DOWN,

	PI_A,	PI_B,	PI_C,	PI_D,
	PI_E,	PI_F,	PI_G,	PI_H,
	PI_I,	PI_J,	PI_K,	PI_L,
	PI_M,	PI_N,	PI_O,	PI_P,
	PI_Q,	PI_R,	PI_S,	PI_T,
	PI_U,	PI_V,	PI_W,	PI_X,
	PI_Y,	PI_Z
};

enum CONTROLLER_BUTTONS {
	PI_CONTROLLER_A = SDL_CONTROLLER_BUTTON_A,
	PI_CONTROLLER_B = SDL_CONTROLLER_BUTTON_B,
	PI_CONTROLLER_X = SDL_CONTROLLER_BUTTON_X,
	PI_CONTROLLER_Y = SDL_CONTROLLER_BUTTON_Y,

	PI_CONTROLLER_LEFT = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
	PI_CONTROLLER_RIGHT = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
	PI_CONTROLLER_UP = SDL_CONTROLLER_BUTTON_DPAD_UP,
	PI_CONTROLLER_DOWN = SDL_CONTROLLER_BUTTON_DPAD_DOWN,

	PI_CONTROLLER_START = SDL_CONTROLLER_BUTTON_START,
	PI_CONTROLLER_SELECT = SDL_CONTROLLER_BUTTON_BACK,
	PI_CONTROLLER_SHOULDER_LEFT = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
	PI_CONTROLLER_SHOULDER_RIGHT = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
};

struct MOUSE { //RECT
	int x, y;
};

const char* PisteInput_KeyName(int key);
int PisteInput_GetTouchPos(float& x, float& y);
int	PisteInput_GetKey();
bool	PisteInput_Keydown(int key);
MOUSE   PisteInput_UpdateMouse(bool keyMove, bool relative);
int     PisteInput_ActivateWindow(bool active);

int		PisteInput_Vibrate();

int		PisteInput_Start();
int		PisteInput_Exit();
bool	PisteInput_Hae_Hiiri(); //DEP
bool	PisteInput_Hae_Nappaimet(); //DEP
bool	PisteInput_Hae_Ohjaimet();
bool	PisteInput_Hiiri_Oikea();
bool	PisteInput_Hiiri_Vasen();
bool	PisteInput_Lue_Eventti();

char PisteInput_Get_Typed_Char();

bool	PisteInput_Gamepad_Button(int button); // Check if button is pressed
int PisteInput_Gamepad_Get_Pressed();
std::string PisteInput_Gamepad_Name();
std::string PisteInput_ButtonName(int button);

void PisteInput_Gamepad_Pressed(int button, bool pushed);

bool	PisteInput_Onko_Hiiri();
bool	PisteInput_Onko_Ohjain(int ohjain);

int PisteInput_CreateGui(int x, int y, int w, int h, BYTE alpha, const char* t_path, DWORD* key);
int PisteInput_ActiveGui(int id, bool active);
int PisteInput_DrawGui(int pd_alpha);

#endif
