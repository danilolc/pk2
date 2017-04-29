//#########################
//PisteEngine - PisteDraw
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef P_DRAW
#define P_DRAW

#include "types.h"

#define		PD_VIRHE		 -1000

#define		PD_TAUSTABUFFER	 0
#define		PD_TAUSTABUFFER2 1

#define		PD_FADE_NOPEA	 15
#define		PD_FADE_NORMAALI 10
#define		PD_FADE_HIDAS	 5

void	PisteDraw_FullScreen();
void 	PisteDraw_GetBounds(int index, int& w, int& h);
int		PisteDraw_SetColor(UCHAR color, int x, int y, int alpha);

int		PisteDraw_Alusta_Debugger();
int		PisteDraw_Alusta(int leveys, int korkeus, char* name);
void	PisteDraw_Aseta_Klipperi(int i, int vasen, int yla, int oikea, int ala);
void	PisteDraw_Aseta_Marginaali_Vasen(int vasen);
void	PisteDraw_Aseta_Marginaali_Yla(int yla);
int		PisteDraw_Aseta_Videomode(int leveys, int korkeus, int bpp, int max_colors);

int		PisteDraw_Buffer_Flip(int i);
int		PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y);

int		PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y, 
									int vasen, int yla, int oikea, int ala);
int		PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y);

int		PisteDraw_Buffer_Flip_Hori(int index);

int		PisteDraw_Buffer_Tayta(int i, UCHAR color);
int		PisteDraw_Buffer_Tayta(int i, const int vasen, const int yla, const int oikea, const int ala, UCHAR color);
bool	PisteDraw_Buffer_Tuhoa(int i);
int		PisteDraw_Buffer_Varaa();

int		PisteDraw_Buffer_Uusi(int leveys, int korkeus, bool video_muisti, UCHAR color);
int		PisteDraw_Buffer_Uusi(int leveys, int korkeus, int colorform);

//int		PisteDraw_Fade_Paletti(void);
void	PisteDraw_Fade_Paletti_In(int laskuri);
void	PisteDraw_Fade_Paletti_Out(int laskuri);
bool	PisteDraw_Fade_Paletti_Valmis(void);

int		PisteDraw_Font_Uusi(int buffer_index, int buffer_x, int buffer_y, int leveys, int korkeus, int lkm);
int		PisteDraw_Font_Uusi(char *polku, char *tiedosto);

int		PisteDraw_Font_Kirjoita(int font_index, char *merkkijono, int buffer_index, int x, int y);
int		PisteDraw_Font_Kirjoita_Lapinakyva(int font_index, char *merkkijono, int buffer_index, 
										   int x, int y, UCHAR pros);
char*	PisteDraw_Locate_Kuva(char *filename);
int		PisteDraw_Lataa_Kuva(int i, char *filename, bool lataa_paletti);
int		PisteDraw_Lopeta();

int		PisteDraw_Palauta_Pinnat();
int		PisteDraw_Paletti_Pyorita(UCHAR eka_vari, UCHAR vika_vari);
int		PisteDraw_Paletti_Set(void);
int		PisteDraw_Piirto_Aloita(int i, UCHAR *&back_buffer, DWORD &lPitch);
int		PisteDraw_Piirto_Lopeta(void);
int		PisteDraw_Piirto_Lopeta(int i);
int		PisteDraw_Reset_Paletti(void);
DWORD	PisteDraw_Videomuistia();
DWORD	PisteDraw_Videomuistia_Max();

#endif
