//#########################
//PisteEngine - PisteDraw
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef P_DRAW
#define P_DRAW

#include "types.h"

#define		PD_FADE_FAST		15
#define		PD_FADE_NORMAL	10
#define		PD_FADE_SLOW		5

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

int PisteDraw2_ImageFill(int index, BYTE color);
int PisteDraw2_ImageFill(int index, int posx, int posy, int oikea, int ala, BYTE color);
int PisteDraw2_ScreenFill(BYTE color);
int PisteDraw2_ScreenFill(int posx, int posy, int oikea, int ala, BYTE color);
void PisteDraw2_SetMask(int x, int y, int w, int h);

int PisteDraw2_DrawScreen_Start(BYTE *&pixels, DWORD &pitch);
int PisteDraw2_DrawScreen_End();
int PisteDraw2_DrawImage_Start(int index, BYTE *&pixels, DWORD &pitch);
int PisteDraw2_DrawImage_End(int index);
BYTE PisteDraw2_BlendColors(BYTE color, BYTE colBack,int alpha);

int PisteDraw2_Font_Create(int image, int x, int y, int width, int height, int count);
int PisteDraw2_Font_Create(char* path, char* file);
int PisteDraw2_Font_Write(int font_index, const char* text, int x, int y);
int PisteDraw2_Font_WriteAlpha(int font_index, const char* text, int x, int y, BYTE alpha);

void PisteDraw2_FullScreen();
int PisteDraw2_Start(int width, int height, const char* name);
int PisteDraw2_Exit();
void PisteDraw2_Update(bool draw,int pc, int fps);

#endif
