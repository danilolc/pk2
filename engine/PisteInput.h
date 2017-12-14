//#########################
//PisteEngine - PisteInput
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef P_INPUT
#define P_INPUT

#include "types.h"

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

enum PI_KEY{
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

struct MOUSE { //RECT
	int x, y;
};

const char* PisteInput_KeyName(BYTE key);
BYTE	PisteInput_GetKey();
bool	PisteInput_Keydown(int key);
MOUSE   PisteInput_UpdateMouse(bool keyMove, bool relative);
int     PisteInput_ActivateWindow(bool active);

int		PisteInput_Alusta();
int		PisteInput_Lopeta(); //DEP
bool	PisteInput_Hae_Hiiri(); //DEP
bool	PisteInput_Hae_Nappaimet(); //DEP
bool	PisteInput_Hae_Ohjaimet();
bool	PisteInput_Hiiri_Oikea();
bool	PisteInput_Hiiri_Vasen();
bool	PisteInput_Lue_Eventti();

char*	PisteInput_Lue_Kontrollin_Nimi(unsigned char kontrolli);
char	PisteInput_Lue_Nappaimisto(void);
bool	PisteInput_Ohjain_Nappi(int ohjain, int index);
char*	PisteInput_Ohjain_Nimi(int ohjain);
int		PisteInput_Ohjain_X(int ohjain);
int		PisteInput_Ohjain_Y(int ohjain);
bool	PisteInput_Onko_Hiiri();
bool	PisteInput_Onko_Ohjain(int ohjain);

#endif
