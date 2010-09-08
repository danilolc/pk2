/* PisteEngine PisteInput v2.0 */

#ifndef P_INPUT
#define P_INPUT

/* INCLUDES ----------------------------------------------------------------------------------*/

#define DIRECTINPUT_VERSION 0x0700
#include "D:\DXSDK\Include\dinput.h" // MUISTA KORJATA POLKU OIKEIN!!!!
#include <windows.h> 

/* DEFINES -----------------------------------------------------------------------------------*/

#define		PI_VIRHE -1000

#define		PI_MAX_PELIOHJAIMIA 2

#define		PI_PELIOHJAIN_1	  0
#define		PI_PELIOHJAIN_2	  1

#define		PI_OHJAIN_XY	  100		//Maksimi (+/-) arvo, jonka Ohjain_X() ja Ohjain_Y() palauttavat
#define		PI_OHJAIN_NAPPI_1 0
#define		PI_OHJAIN_NAPPI_2 1
#define		PI_OHJAIN_NAPPI_3 2
#define		PI_OHJAIN_NAPPI_4 3
#define		PI_OHJAIN_NAPPI_5 4
#define		PI_OHJAIN_NAPPI_6 5

// Kontrollien koodit
#define		PI_KB_A  1
#define		PI_KB_B  2
#define		PI_KB_C  3
#define		PI_KB_D  4
#define		PI_KB_E  5
#define		PI_KB_F  6
#define		PI_KB_G  7
#define		PI_KB_H  8
#define		PI_KB_I  9
#define		PI_KB_J  10
#define		PI_KB_K  12
#define		PI_KB_L  13
#define		PI_KB_M  14
#define		PI_KB_N  15
#define		PI_KB_O  16
#define		PI_KB_P  17
#define		PI_KB_Q  18
#define		PI_KB_R  19
#define		PI_KB_S  20
#define		PI_KB_T  21
#define		PI_KB_U  22
#define		PI_KB_V  23
#define		PI_KB_W  24
#define		PI_KB_X  25
#define		PI_KB_Y  26
#define		PI_KB_Z  27

#define		PI_KB_0  30
#define		PI_KB_1  31
#define		PI_KB_2  32
#define		PI_KB_3  33
#define		PI_KB_4  34
#define		PI_KB_5  35
#define		PI_KB_6  36
#define		PI_KB_7  37
#define		PI_KB_8  38
#define		PI_KB_9  39

#define		PI_KB_SPACE		40
#define		PI_KB_RSHIFT	41
#define		PI_KB_LSHIFT	42
#define		PI_KB_RALT		43
#define		PI_KB_LALT		44
#define		PI_KB_RCONTROL	45
#define		PI_KB_LCONTROL	46
#define		PI_KB_ENTER		47
#define		PI_KB_PAGEUP	48
#define		PI_KB_PAGEDOWN	49
#define		PI_KB_BACKSPACE	50
#define		PI_KB_NUMPADENTER 51

#define		PI_KB_UP		52
#define		PI_KB_DOWN		53
#define		PI_KB_LEFT		54
#define		PI_KB_RIGHT		55

#define		PI_KB_F1		60
#define		PI_KB_F2		61
#define		PI_KB_F3		62
#define		PI_KB_F4		63
#define		PI_KB_F5		64
#define		PI_KB_F6		65
#define		PI_KB_F7		66
#define		PI_KB_F8		67
#define		PI_KB_F9		68
#define		PI_KB_F10		69
#define		PI_KB_F11		70
#define		PI_KB_F12		71

#define		PI_KB_NUMPAD_0  80
#define		PI_KB_NUMPAD_1  81
#define		PI_KB_NUMPAD_2  82
#define		PI_KB_NUMPAD_3  83
#define		PI_KB_NUMPAD_4  84
#define		PI_KB_NUMPAD_5  85
#define		PI_KB_NUMPAD_6  86
#define		PI_KB_NUMPAD_7  87
#define		PI_KB_NUMPAD_8  88
#define		PI_KB_NUMPAD_9  89

#define		PI_HIIRI_VASEN_NAPPI	100
#define		PI_HIIRI_OIKEA_NAPPI	101
#define		PI_HIIRI_OIKEALLE		102
#define		PI_HIIRI_VASEMMALLE		103
#define		PI_HIIRI_YLOS			104
#define		PI_HIIRI_ALAS			105

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

#define		PI_OHJAIN2_VASEMMALLE	130	
#define		PI_OHJAIN2_OIKEALLE		131
#define		PI_OHJAIN2_YLOS			132
#define		PI_OHJAIN2_ALAS			133
#define		PI_OHJAIN2_NAPPI1		134
#define		PI_OHJAIN2_NAPPI2		135
#define		PI_OHJAIN2_NAPPI3		136
#define		PI_OHJAIN2_NAPPI4		137
#define		PI_OHJAIN2_NAPPI5		138
#define		PI_OHJAIN2_NAPPI6		139

/* PROTOTYPES --------------------------------------------------------------------------------*/

int		PisteInput_Alusta(HWND &main_window_handle, HINSTANCE &hinstance_app);
//-----------------------------------------------------------------------------------------------------------
bool	PisteInput_Hae_Hiiri();		// Päivittää hiiren tiedot (onko liikuteltu, mitä nappeja paineltu...)
//-----------------------------------------------------------------------------------------------------------
bool	PisteInput_Hae_Nappaimet();	// Päivittää näppäimistön tiedot.
//-----------------------------------------------------------------------------------------------------------
bool	PisteInput_Hae_Ohjaimet();	// Päivittää peliohjainten tiedot.
//-----------------------------------------------------------------------------------------------------------
bool	PisteInput_Hiiri_Oikea();	// Onko hiiren oikeaa korvaa painettu? true = kyllä
//-----------------------------------------------------------------------------------------------------------
bool	PisteInput_Hiiri_Vasen();	// Onko hiiren vasenta korvaa painettu? true = kyllä
//-----------------------------------------------------------------------------------------------------------
int		PisteInput_Hiiri_X(int x);	// Kertoo paljonko hiiren kursorin on liikkunut vaakatasossa.
									// Jos haluat saada vastauksena hiiren tämän hetkisen x-kordinaatin,
									// anna x:n arvoksi hiiren edellinen x -kordinaatti. Jos haluat pelkän
									// muutoksen, anna x:n arvoksi 0.
//-----------------------------------------------------------------------------------------------------------
int		PisteInput_Hiiri_Y(int y);	// Kertoo paljonko hiiren kursorin on liikkunut pystytasossa.
									// Katso PisteInput_Hiiri_X(int x) selitys. Toimii samanlailla.
//-----------------------------------------------------------------------------------------------------------
bool	PisteInput_Keydown(int key);// Kertoo onko kyseltyä näppäintä painettu näppäimistöstä.
									// key = DirectInput vakio. Esim. A -näppäin on DIK_A. DirectInput
									// vakiot on listattu tämän headerin loppussa.
//-----------------------------------------------------------------------------------------------------------
int		PisteInput_Lopeta();		// Suljetaan PI.
//-----------------------------------------------------------------------------------------------------------
bool	PisteInput_Lue_Kontrolli(UCHAR kontrolli); // Palauttaa TRUE jos käyttäjä on aktivoinut kontrollin.
//-----------------------------------------------------------------------------------------------------------
UCHAR	PisteInput_Lue_Kontrolli(); // Palauttaa sen kontrollin, joka syötettiin viimeksi.
//-----------------------------------------------------------------------------------------------------------
char   *PisteInput_Lue_Kontrollin_Nimi(UCHAR kontrolli); // Palauttaa kontrollin nimen. Esim. 'arrow left'
//-----------------------------------------------------------------------------------------------------------
char	PisteInput_Lue_Nappaimisto(void); // Palauttaa sen merkin, joka syötettiin näppäimistöltä viimeksi.
//-----------------------------------------------------------------------------------------------------------
bool	PisteInput_Ohjain_Nappi(int ohjain, int index);	
									// true, jos valitun ohjaimen valittua nappia on
									// painettu. Esim. peliohjaimen 1 nappi 1:
									// PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1);
//-----------------------------------------------------------------------------------------------------------
char   *PisteInput_Ohjain_Nimi(int ohjain);	// Palauttaa ohjaimen nimen
//-----------------------------------------------------------------------------------------------------------
int		PisteInput_Ohjain_X(int ohjain);	// Kertoo onko ohjainta painettu vasemmalle tai oikealle.
											// Pienempi kuin nolla on vasemmalle, suurempi kuin nolla
											// on oikealle.
											// ohjain = indeksi peliohjaimeen 1 tai 2
//-----------------------------------------------------------------------------------------------------------
int		PisteInput_Ohjain_Y(int ohjain);	// Kertoo onko ohjainta painettu ylös tai alas.
											// Pienempi kuin nolla on ylös, suurempi kuin nolla
											// on alas.
											// ohjain = indeksi peliohjaimeen 1 tai 2 
//-----------------------------------------------------------------------------------------------------------
bool	PisteInput_Onko_Hiiri();			// true jos järjestelmästä löytyy hiiri
//-----------------------------------------------------------------------------------------------------------
bool	PisteInput_Onko_Ohjain(int ohjain); // true jos järjestelmästä löytyy peliohjain.
											// ohjain = indeksi peliohjaimeen 1 tai 2
//-----------------------------------------------------------------------------------------------------------

#endif

/* Lista DirectInputin:n näppäin koodeista:
	
	Näppäin					Koodi
	-------					----------

	ESC						DIK_ESCAPE
	ENTER					DIK_RETURN
	VASEN CTRL				DIK_LCONTROL 
	OIKEA CTRL				DIK_RCONTROL 
	VASEN SHIFT				DIK_LSHIFT 
	OIKEA SHIFT				DIK_RSHIFT
	VÄLILYÖNTI				DIK_SPACE 
	VASEN ALT				DIK_LALT
	OIKEA ALT				DIK_RALT

  
	OIKEA					DIK_RIGHT
	VASEN					DIK_LEFT
	YLÖS					DIK_UP
	ALAS					DIK_DOWN


	F1						DIK_F1 
	F2						DIK_F2
	F3						DIK_F3
	...						...
	F10						DIK_F10


	0						DIK_0
	1						DIK_1
	2						DIK_2
	3						DIK_3
	...						...
	9						DIK_9
  

	A						DIK_A
	B						DIK_B
	C						DIK_C
	...						...
	Y						DIK_Y
	X						DIK_X
	

	1 (numpad)				DIK_NUMPAD1 
	2 (numpad)				DIK_NUMPAD2
	3 (numpad)				DIK_NUMPAD3
	...						...
	9 (numpad)				DIK_NUMPAD9


	+ (numpad)				DIK_ADD					
	- (numpad)				DIK_SUBTRACT 
	, (numpad)				DIK_DECIMAL 

	ENTER (numpad)			DIK_NUMPADENTER 

	*						DIK_MULTIPLY 
	-						DIK_MINUS
	=						DIK_EQUALS
	BACKSPACE				DIK_BACK
	TAB						DIK_TAB
	DELETE					DIK_DELETE 
	INSERT					DIK_INSERT
	PAGE DOWN				DIK_NEXT 
	PAGE UP					DIK_PRIOR
	[						DIK_LBRACKET 
	]						DIK_RBRACKET 
	:						DIK_SEMICOLON 
	'						DIK_APOSTROPHE 
	.						DIK_PERIOD 
	,						DIK_COMMA 
	
*/