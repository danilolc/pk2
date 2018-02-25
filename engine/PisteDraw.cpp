//#########################
//PisteEngine - PisteDraw
//by Janne Kivilahti from Piste Gamez
//#########################

#include "PisteDraw.h"
#include "PisteFont.h"

#include <fstream>
#include <algorithm>
#include <ctime>

#include <SDL2/SDL_image.h>

const int MAX_IMAGES = 2000;
const int MAX_FONTS = 20;

const char* PD_WindowName;

SDL_Window*   PD_Window = NULL;
SDL_Renderer* PD_Renderer = NULL;

SDL_Surface*  frameBuffer8 = NULL;

SDL_Surface*  imageList[MAX_IMAGES];
PisteFont2*   fontList[MAX_FONTS];
SDL_Palette*  game_palette = NULL;

int  PD_screen_width;
int  PD_screen_height;
SDL_Rect Screen_dest = {0, 0, 0, 0};
bool ScreenFit = false;

bool PD2_loaded = false;

int PD_fade_speed = 0;
int PD_alpha = 100;

int XOffset = 0;

int findfreeimage(){
	int i;
	for(i=0;i<MAX_IMAGES;i++)
		if(imageList[i]==NULL)
			return i;
	return -1;
}
int findfreefont(){
	int i;
	for(i=0;i<MAX_FONTS;i++)
		if(fontList[i]==NULL)
			return i;
	return -1;
}

bool PisteDraw2_IsFading(){
  if (PD_alpha > 0 && PD_fade_speed < 0)
    return true;

  if (PD_alpha < 100 && PD_fade_speed > 0)
    return true;

  return false;
}
int  PisteDraw2_FadeOut(int speed){
	PD_alpha = 100;
	PD_fade_speed = -speed;
	return 0;
}
int  PisteDraw2_FadeIn(int speed){
	PD_alpha = 0;
	PD_fade_speed = speed;
	return 0;
}
void PisteDraw2_RotatePalette(BYTE start, BYTE end){
	BYTE i;
	SDL_Color* game_colors = game_palette->colors;
	SDL_Color temp_color = game_colors[end];

	for (i=end;i>start;i--)
		game_colors[i] = game_colors[i-1];

	game_colors[start] = temp_color;
}

int PisteDraw2_Image_New(int w, int h){
	int index = findfreeimage();
	if (index == -1) return -1;

	imageList[index] = SDL_CreateRGBSurface(0, w, h, 8, 0, 0, 0, 0);

	SDL_SetColorKey(imageList[index], SDL_TRUE, 255);
	SDL_FillRect(imageList[index], NULL, 255);

	imageList[index]->userdata = (void*)imageList[index]->format->palette;
	imageList[index]->format->palette = game_palette;

	return index;
}
int PisteDraw2_Image_Load(const char* filename, bool getPalette){
	int index, i;
	SDL_Palette* pal;

	index = findfreeimage();
	if (index==-1){
		printf("Error - Got index -1");
		return -1;
	}

	imageList[index] = SDL_LoadBMP(filename);
	if(imageList[index]==NULL){
		printf("PD     - Error loading %s\n",filename);
		return -1;
	}
	if(imageList[index]->format->BitsPerPixel != 8){
		printf("PD     - Failed to open %s, just 8bpp indexed images!\n",filename);
		PisteDraw2_Image_Delete(index);
		return -1;
	}
	SDL_SetColorKey(imageList[index], SDL_TRUE, 255);

	if(getPalette){
		pal = imageList[index]->format->palette;
		for(i=0;i<256;i++)
			game_palette->colors[i] = pal->colors[i];
	}

	imageList[index]->userdata = (void*)imageList[index]->format->palette; //Put allocated pallete in userdata
	imageList[index]->format->palette = game_palette;

	return index;
}
int PisteDraw2_Image_Copy(int src_i, int dst_i){
	if(src_i < 0 || dst_i < 0) return -1;
	SDL_FillRect(imageList[dst_i], NULL, 255);
	SDL_BlitSurface(imageList[src_i], NULL, imageList[dst_i], NULL);
	return 0;
}
int PisteDraw2_Image_Cut(int ImgIndex, int x, int y, int w, int h){ //Create a new image from a existing image
	PD_RECT area;
	area.x = x; area.y = y;
	area.w = (w <= 0) ? imageList[ImgIndex]->w : w; //If 0 get the entire image
	area.h = (h <= 0) ? imageList[ImgIndex]->h : h;
	return PisteDraw2_Image_Cut(ImgIndex, area);
}
int PisteDraw2_Image_Cut(int ImgIndex, PD_RECT area){
	int index;

	index = findfreeimage();
	if (index==-1){
		printf("PD     - PisteDraw has run out of free images!");
		return -1;
	}

	imageList[index] = SDL_CreateRGBSurface(0, area.w, area.h, 8, 0, 0, 0, 0);

	SDL_SetColorKey(imageList[index], SDL_TRUE, 255);
	SDL_FillRect(imageList[index], NULL, 255);

	imageList[index]->userdata = (void*)imageList[index]->format->palette;
	imageList[index]->format->palette = game_palette;

	SDL_BlitScaled(imageList[ImgIndex], (SDL_Rect*)&area, imageList[index], NULL);

	return index;
}
int PisteDraw2_Image_Clip(int index, int x, int y){
	SDL_Rect dstrect;

	dstrect.x = x + XOffset;
	dstrect.y = y;

	SDL_BlitSurface(imageList[index], NULL, frameBuffer8, &dstrect);

	return 0;
}
int PisteDraw2_Image_ClipTransparent(int index, int x, int y, int alpha){
	PD_RECT srcrect, dstrect;
	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = imageList[index]->w;
	srcrect.h = imageList[index]->h;
	dstrect.x = x;
	dstrect.y = y;
	return PisteDraw2_Image_CutClipTransparent(index, srcrect, dstrect, alpha);
}
int PisteDraw2_Image_CutClip(int index, int dstx, int dsty, int srcx, int srcy, int oikea, int ala){ //TODO - fix names
	PD_RECT src = {(DWORD)srcx, (DWORD)srcy, (DWORD)oikea-srcx, (DWORD)ala-srcy};
	PD_RECT dst = {(DWORD)dstx, (DWORD)dsty, (DWORD)oikea-srcx, (DWORD)ala-srcy};
	PisteDraw2_Image_CutClip(index, src, dst);
	return 0;
}
int PisteDraw2_Image_CutClip(int index, PD_RECT srcrect, PD_RECT dstrect){
	dstrect.x += XOffset;
	SDL_BlitSurface(imageList[index], (SDL_Rect*)&srcrect, frameBuffer8, (SDL_Rect*)&dstrect);
	return 0;
}
int PisteDraw2_Image_CutClipTransparent(int index, PD_RECT srcrect, PD_RECT dstrect, int alpha){
	return PisteDraw2_Image_CutClipTransparent(index, srcrect, dstrect, alpha, 0);
}
int PisteDraw2_Image_CutClipTransparent(int index, PD_RECT srcrect, PD_RECT dstrect, int alpha, int colorsum){
	BYTE *imagePix = NULL;
	BYTE *screenPix = NULL;
	BYTE color1, color2;
	DWORD imagePitch, screenPitch;
	int posx, posy;

	int x_start = dstrect.x + XOffset;
	int	x_end = dstrect.x + srcrect.w;
	int	y_start = dstrect.y;
	int	y_end = dstrect.y + srcrect.h;

	if (alpha > 100) alpha = 100;
	if (alpha < 0) alpha = 0;
	if (x_end > PD_screen_width) x_end = PD_screen_width;
	if (y_end > PD_screen_height) y_end = PD_screen_height;
	if (x_start > x_end || y_start > y_end) return -1;

	PisteDraw2_DrawImage_Start(index, *&imagePix, (DWORD &)imagePitch);
	PisteDraw2_DrawScreen_Start(*&screenPix, (DWORD &)screenPitch);
	for (posx = x_start; posx < x_end; posx++)
		for (posy = y_start; posy < y_end; posy++) {
			color1 = imagePix[(posx-x_start+srcrect.x)+imagePitch*(posy-y_start+srcrect.y)];
			if (color1 != 255) {
				color2 = screenPix[posx+screenPitch*posy];
				screenPix[posx+screenPitch*posy] = PisteDraw2_BlendColors(color1, color2, alpha) + colorsum;
			}
		}
	PisteDraw2_DrawScreen_End();
	PisteDraw2_DrawImage_End(index);
	return 0;
}
void PisteDraw2_Image_GetSize(int index, int& w, int& h){
	w = imageList[index]->w;
	h = imageList[index]->h;
}
int PisteDraw2_Image_FlipHori(int index){
	int i, h, w, p;
	BYTE* pix_array;

	if(index < 0) return -1;

	h = imageList[index]->h;
	w = imageList[index]->w;
	p = imageList[index]->pitch;

	SDL_LockSurface(imageList[index]);

	pix_array  = (BYTE*)(imageList[index]->pixels);

	for(i=0; i<h*p; i+=p)
		std::reverse(&pix_array[i],&pix_array[i + w]);

	SDL_UnlockSurface(imageList[index]);
	return 0;
}
int PisteDraw2_Image_Snapshot(int index){
	SDL_FillRect(imageList[index], NULL, 0);
	return SDL_BlitSurface(frameBuffer8, NULL, imageList[index], NULL);
}
int PisteDraw2_Image_Delete(int& index){
	if(index < 0) return -1;
	if (imageList[index] == NULL) return -1;
	imageList[index]->format->palette = (SDL_Palette*)imageList[index]->userdata; //Return to the original pallete
	SDL_FreeSurface(imageList[index]);
	imageList[index] = NULL;
	index = -1;
	return 0;
}

int PisteDraw2_ImageFill(int index, BYTE color){
	return PisteDraw2_ImageFill(index, 0, 0, imageList[index]->w, imageList[index]->h, color);
}
int PisteDraw2_ImageFill(int index, int posx, int posy, int oikea, int ala, BYTE color){
	SDL_Rect r = {posx, posy, oikea-posx, ala-posy};
	return SDL_FillRect(imageList[index], &r, color);
}
int PisteDraw2_ScreenFill(BYTE color){
	return SDL_FillRect(frameBuffer8, NULL, color);
}
int PisteDraw2_ScreenFill(int posx, int posy, int oikea, int ala, BYTE color){
	SDL_Rect r = {posx + XOffset, posy, oikea-posx, ala-posy};
	return SDL_FillRect(frameBuffer8, &r, color);
}
void PisteDraw2_SetMask(int x, int y, int w, int h){
	SDL_Rect r = {x, y, w, h};
	SDL_SetClipRect(frameBuffer8, &r);
}

int PisteDraw2_DrawScreen_Start(BYTE* &pixels, DWORD &pitch){
	pixels = (BYTE*)frameBuffer8->pixels;
	pitch = frameBuffer8->pitch;
	return SDL_LockSurface(frameBuffer8);
}
int PisteDraw2_DrawScreen_End(){
	SDL_UnlockSurface(frameBuffer8);
	return 0;
}
int PisteDraw2_DrawImage_Start(int index, BYTE* &pixels, DWORD &pitch){
	pixels = (BYTE*)imageList[index]->pixels;
	pitch = imageList[index]->pitch;
	return SDL_LockSurface(imageList[index]);
}
int PisteDraw2_DrawImage_End(int index){
	SDL_UnlockSurface(imageList[index]);
	return 0;
}
BYTE PisteDraw2_BlendColors(BYTE color, BYTE colBack, int alpha){
	int result;

	if(alpha>100) alpha = 100;
	if(alpha<0) alpha = 0;

	result = color%32;
	result = (result*alpha)/100;
	result += colBack%32;
	if(result>31) result = 31;

	return (BYTE)result;//+32*col
}

int PisteDraw2_Font_Create(int image, int x, int y, int char_w, int char_h, int count){
	int index;

	index = findfreefont();
	if (index==-1){
		printf("PD    - PisteDraw has run out of free fonts!");
		return -1;
	}

  fontList[index] = new PisteFont2(image, x, y, char_w, char_h, count);
  return index;
}
int PisteDraw2_Font_Create(char* path, char* file){
	printf("PD     - Created font with path %s e file %s\n",path,file);
	int index;

	index = findfreefont();
	if (index==-1){
		printf("PD    - PisteDraw has run out of free fonts!");
		return -1;
	}

	fontList[index] = new PisteFont2();
  if (fontList[index]->LoadFile(path,file) == -1){
    printf("PD    - PisteDraw can't load a font from file!");
		delete fontList[index];
    return -1;
  }

  return index;
}
int PisteDraw2_Font_Write(int font_index, const char* text, int x, int y){
	return fontList[font_index]->Write_Text(x, y, text);
}
int PisteDraw2_Font_WriteAlpha(int font_index, const char* text, int x, int y, BYTE alpha){
	return fontList[font_index]->Write_TextTrasparent(x + XOffset, y, text, alpha);
}

int PisteDraw2_SetFilter(const char* filter){
	if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,filter) == SDL_TRUE)
		return 0;

	return 1;
}
void PisteDraw2_FullScreen(bool set){
	if(set)
		SDL_SetWindowFullscreen(PD_Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else {
		SDL_SetWindowFullscreen(PD_Window, 0);
		SDL_SetWindowSize(PD_Window, PD_screen_width, PD_screen_height);
		SDL_SetWindowPosition(PD_Window, SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED);
	}
}
void PisteDraw2_AdjustScreen(){
	int w, h;
	SDL_GetWindowSize(PD_Window, &w, &h);

	float screen_prop = (float)w / h;
	float buff_prop   = (float)PD_screen_width / PD_screen_height;
	if (buff_prop > screen_prop) {
		Screen_dest.w = w;
		Screen_dest.h = (int)(h / buff_prop);
		Screen_dest.x = 0;
		Screen_dest.y = (int)((h - Screen_dest.h) / 2);
	}
	else {
		Screen_dest.w = (int)(buff_prop * h);
		Screen_dest.h = h;
		Screen_dest.x = (int)((w - Screen_dest.w) / 2);
		Screen_dest.y = 0;
	}
}
void PisteDraw2_FitScreen(bool fit){
	ScreenFit = fit;
}
void PisteDraw2_ChangeResolution(int w, int h) {
	if (w == PD_screen_width && h == PD_screen_height)
		return;

	frameBuffer8->format->palette = (SDL_Palette *)frameBuffer8->userdata;
	SDL_FreeSurface(frameBuffer8);
	SDL_DestroyRenderer(PD_Renderer);
	SDL_DestroyWindow(PD_Window);
	PD2_loaded = false;

	PisteDraw2_Start(w, h, PD_WindowName);
}

void PisteDraw2_GetWindowPosition(int* x, int* y) {
	SDL_GetWindowPosition(PD_Window, x, y);
}

int PisteDraw2_GetXOffset() {
	return XOffset;
}
void PisteDraw2_SetXOffset(int x) {
	XOffset = x;
}

int PisteDraw2_Start(int width, int height, const char* name) {
	if (PD2_loaded) return -1;

	if (game_palette == NULL) {
        game_palette = SDL_AllocPalette(256);
        for(int i = 0; i < 256; i++) game_palette->colors[i] = {(Uint8)i,(Uint8)i,(Uint8)i,(Uint8)i};
    }

	PD_WindowName = name;
	PD_Window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	PD_Renderer = SDL_CreateRenderer(PD_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	frameBuffer8 = SDL_CreateRGBSurface(0, width, height, 8, 0, 0, 0, 0);
	frameBuffer8->userdata = (void *)frameBuffer8->format->palette;
	frameBuffer8->format->palette = game_palette;
	SDL_SetColorKey(frameBuffer8, SDL_TRUE, 255);
	SDL_FillRect(frameBuffer8, NULL, 255);

	SDL_RenderClear(PD_Renderer);

	SDL_Rect r = {0, 0, width, height};
	SDL_SetClipRect(frameBuffer8, &r);

	PD_screen_width = width;
	PD_screen_height = height;
	PisteDraw2_AdjustScreen();

	PD2_loaded = true;
	return 0;
}
int PisteDraw2_Exit(){
  if (!PD2_loaded) return -1;

  int i,j;

  for (i=0; i<MAX_IMAGES; i++)
    if (imageList[i] != NULL){
	  j = i;
      PisteDraw2_Image_Delete(j);
    }

  for (i=0; i<MAX_FONTS; i++){
    if (fontList[i] != NULL)
      delete fontList[i];
    fontList[i] = NULL;
  }

  frameBuffer8->format->palette = (SDL_Palette *)frameBuffer8->userdata;
  SDL_FreeSurface(frameBuffer8);
  SDL_DestroyRenderer(PD_Renderer);
  SDL_DestroyWindow(PD_Window);

  PD2_loaded = false;
  return 0;
}
void PisteDraw2_Update(bool draw){
	if(!PD2_loaded) return;

	if(draw){
		SDL_Texture* texture;
		BYTE alpha = (BYTE)(PD_alpha*255/100);

		texture = SDL_CreateTextureFromSurface(PD_Renderer,frameBuffer8);
		SDL_SetTextureColorMod(texture,alpha,alpha,alpha);

		SDL_RenderClear(PD_Renderer);

		if(ScreenFit)
			SDL_RenderCopy(PD_Renderer, texture, NULL, NULL);
		else
			SDL_RenderCopy(PD_Renderer, texture, NULL, &Screen_dest);
		SDL_RenderPresent(PD_Renderer);

		SDL_DestroyTexture(texture);
	}

	if (PisteDraw2_IsFading()){
		PD_alpha += PD_fade_speed;
		if(PD_alpha < 0) PD_alpha = 0;
		if(PD_alpha > 255) PD_alpha = 255;
	}

	SDL_Rect r = {0, 0, XOffset, PD_screen_height}; // Fill the unused borders
	SDL_FillRect(frameBuffer8, &r, 0);
	r.x = PD_screen_width - XOffset;
	SDL_FillRect(frameBuffer8, &r, 0);
}
