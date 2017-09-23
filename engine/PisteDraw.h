//#########################
//PisteEngine - PisteDraw
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef P_DRAW
#define P_DRAW

#include <stdint.h>

typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;

struct PD_RECT{
	int x, y, w, h;
};

bool PisteDraw2_IsFading();
int PisteDraw2_FadeOut(int speed);
int PisteDraw2_FadeIn(int speed);

int PisteDraw2_Image_New(int w, int h);
int PisteDraw2_Image_Load(const char* filename, bool getPalette);
int PisteDraw2_Image_Copy(int src_i, int dst_i);
int PisteDraw2_Image_Cut(int ImgIndex, int x, int y, int w, int h);
int PisteDraw2_Image_Cut(int ImgIndex, PD_RECT area);
int PisteDraw2_Image_Clip(int index, int x, int y);
int PisteDraw2_Image_ClipTransparent(int index, int x, int y, int alpha);
int PisteDraw2_Image_CutClip(int index, int dstx, int dsty, int srcx, int srcy, int oikea, int ala);
int PisteDraw2_Image_CutClip(int index, PD_RECT srcrect, PD_RECT dstrect);
int PisteDraw2_Image_CutClipTransparent(int index, PD_RECT srcrect, PD_RECT dstrect, int alpha);
void PisteDraw2_Image_GetSize(int index, int& w, int& h);
int PisteDraw2_Image_FlipHori(int index);
int PisteDraw2_Image_Delete(int& index);

int PisteDraw2_Font_Create(int image, int x, int y, int width, int height, int count);
int PisteDraw2_Font_Create(char* path, char* file);
int PisteDraw2_Font_Write(int font_index, const char* text, int x, int y);
int PisteDraw2_Font_WriteAlpha(int font_index, const char* text, int x, int y, BYTE alpha);

int PisteDraw2_DrawScreen_Start(BYTE *&pixels, DWORD &pitch);
int PisteDraw2_DrawScreen_End();
int PisteDraw2_DrawImage_Start(int index, BYTE *&pixels, DWORD &pitch);
int PisteDraw2_DrawImage_End(int index);
BYTE PisteDraw2_BlendColors(BYTE color, BYTE colBack,int alpha);

int PisteDraw2_ImageFill(int index, BYTE color);
int PisteDraw2_ImageFill(int index, int posx, int posy, int oikea, int ala, BYTE color);
int PisteDraw2_ScreenFill(BYTE color);
int PisteDraw2_ScreenFill(int posx, int posy, int oikea, int ala, BYTE color);

void PisteDraw2_FullScreen();
int PisteDraw2_Start(int width, int height, const char* name);
int PisteDraw2_Exit();
void PisteDraw2_Update(bool draw,int pc, int fps);





//#########################





#define		PD_VIRHE			-1000

#define		PD_FADE_NOPEA		15
#define		PD_FADE_NORMAALI	10
#define		PD_FADE_HIDAS		5

#define		PD_TAUSTABUFFER		0

void	PisteDraw_FullScreen();
void 	PisteDraw_GetBounds(int index, int& w, int& h);
int		PisteDraw_SetColor(BYTE color, int x, int y, int alpha);

void PisteDraw_Paivita_Naytto(bool draw);

int		PisteDraw_Start_Debugger();
int		PisteDraw_Start(int width, int height, char* name);
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

int		PisteDraw_Buffer_Tayta(int i, BYTE color);
int		PisteDraw_Buffer_Tayta(int i, const int vasen, const int yla, const int oikea, const int ala, BYTE color);
bool	PisteDraw_Buffer_Tuhoa(int i);
int		PisteDraw_Buffer_Varaa();

int		PisteDraw_Buffer_Uusi(int leveys, int korkeus, bool video_muisti, BYTE color);
int		PisteDraw_Buffer_Uusi(int leveys, int korkeus, int colorform);

void	PisteDraw_Fade_Paletti_In(int laskuri);
void	PisteDraw_Fade_Paletti_Out(int laskuri);
bool	PisteDraw_Fade_Paletti_Valmis(void);

int		PisteDraw_Font_Uusi(int buffer_index, int buffer_x, int buffer_y, int leveys, int korkeus, int lkm);
int		PisteDraw_Font_Uusi(char *polku, char *tiedosto);

int		PisteDraw_Font_Kirjoita(int font_index, char *merkkijono, int buffer_index, int x, int y);
int		PisteDraw_Font_Kirjoita_Lapinakyva(int font_index, char *merkkijono, int buffer_index,
										   int x, int y, BYTE pros);
char*	PisteDraw_Locate_Kuva(char *filename);
int		PisteDraw_Lataa_Kuva(int i, char *filename, bool lataa_paletti);
int		PisteDraw_Lopeta();

int		PisteDraw_Palauta_Pinnat();
int		PisteDraw_Paletti_Pyorita(BYTE eka_vari, BYTE vika_vari);
int		PisteDraw_Paletti_Set(void);
int		PisteDraw_Piirto_Aloita(int i, BYTE *&back_buffer, DWORD &lPitch);
int		PisteDraw_Piirto_Lopeta(void);
int		PisteDraw_Piirto_Lopeta(int i);
int		PisteDraw_Reset_Paletti(void);
DWORD	PisteDraw_Videomuistia();
DWORD	PisteDraw_Videomuistia_Max();

#endif
