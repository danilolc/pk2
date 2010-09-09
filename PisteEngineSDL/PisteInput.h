/* PisteEngine PisteInput v2.0 */

#ifndef P_INPUT
#define P_INPUT

/* INCLUDES ----------------------------------------------------------------------------------*/

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


typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

typedef unsigned char  UCHAR;
typedef unsigned long  HWND;
typedef unsigned long  HINSTANCE;

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
bool PisteInput_Lue_Eventti();
bool	PisteInput_Lue_Kontrolli(int kontrolli); // Palauttaa TRUE jos käyttäjä on aktivoinut kontrollin.
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
