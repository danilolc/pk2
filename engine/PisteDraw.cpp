//#########################
//PisteEngine - PisteDraw
//by Janne Kivilahti from Piste Gamez
//#########################

#include "PisteDraw.h"
#include "PisteFont.h"

#include <cstdio>
#include <fstream>
#include <sys/stat.h>
#include <algorithm>

#include <SDL2/SDL_image.h>

#ifdef _WIN32
#include "win32hacks.h"
#endif

const int MAX_IMAGES = 200;
const int MAX_FONTS = 20;

SDL_Window* 	PD_Window = NULL;
SDL_Renderer* PD_Renderer = NULL;
SDL_Texture*	PD_Texture = NULL;

SDL_Surface*	frameBuffer = NULL;

SDL_Surface*	imageList[MAX_IMAGES];
PisteFont2*		fontList[MAX_FONTS];

int				PD_screen_width;
int				PD_screen_height;
bool			PD2_loaded = false;

int				PD_fade_speed = 0;
UCHAR 		PD_alpha = 0;

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
  if (PD_alpha > 0   && PD_fade_speed < 0)
    return true;

  if (PD_alpha < 255 && PD_fade_speed > 0)
    return true;

  return false;
}
int PisteDraw2_FadeOut(int speed){
	PD_alpha    =  254;
	PD_fade_speed =  -speed;

	return 0;
}
int PisteDraw2_FadeIn(int speed){
	PD_alpha    =  1;
	PD_fade_speed = speed;

	return 0;
}

int PisteDraw2_Image_Load(const char* filename, bool getPalette){
	int index, i;
	SDL_Palette* pal;

	index = findfreeimage();
	if (index==-1){
		printf("PisteEngine has run out of free images!");
		return -1;
	}

	imageList[index] = SDL_LoadBMP(filename);
	if(imageList[index]->format->BitsPerPixel != 8){
		printf("Failed to open %s, just 8bpp indexed images!\n",filename);
		PisteDraw2_Image_Delete(index);
		return -1;
	}
	SDL_SetColorKey(imageList[index], SDL_TRUE, 255);

	if(getPalette){
		pal = imageList[index]->format->palette;
		for(i=0;i<256;i++)
			frameBuffer->format->palette->colors[i] = pal->colors[i];
	}

	imageList[index]->userdata = (void*)imageList[index]->format->palette;
	imageList[index]->format->palette = frameBuffer->format->palette;

	return 0;
}
int PisteDraw2_Image_Cut(int ImgIndex, int x, int y, int w, int h){
	PD_RECT area;
	area.x = x; area.y = y;
	area.w = w; area.h = h;
	return PisteDraw2_Image_Cut(ImgIndex, area);
}
int PisteDraw2_Image_Cut(int ImgIndex, PD_RECT area){
	int index;

	index = findfreeimage();
	if (index==-1){
		printf("PisteEngine has run out of free images!");
		return -1;
	}

	imageList[index] = SDL_CreateRGBSurface(0, area.w, area.h, 8, 0, 0, 0, 0);

	SDL_SetColorKey(imageList[index], SDL_TRUE, 255);
	SDL_FillRect(imageList[index], NULL, 255);

	imageList[index]->userdata = (void*)imageList[index]->format->palette;
	imageList[index]->format->palette = frameBuffer->format->palette;

	SDL_BlitScaled(imageList[ImgIndex], (SDL_Rect*)&area, imageList[index], NULL);

	return index;
}
int PisteDraw2_Image_Clip(int index, int x, int y){
	SDL_Rect dstrect;

	dstrect.x = x;
	dstrect.y = y;

	SDL_BlitSurface(imageList[index], NULL, frameBuffer, &dstrect);

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
int PisteDraw2_Image_CutClip(int index, PD_RECT srcrect, PD_RECT dstrect){
	SDL_BlitSurface(imageList[index], (SDL_Rect*)&srcrect, frameBuffer, (SDL_Rect*)&dstrect);

	return 0;
}
int PisteDraw2_Image_CutClipTransparent(int index, PD_RECT srcrect, PD_RECT dstrect, int alpha){
	UCHAR *imagePix = NULL;
	UCHAR *screenPix = NULL;
	UCHAR color1, color2;
	int imagePitch, screenPitch;
	int posx, posy;

	int x_start = dstrect.x;
	int	x_end = dstrect.x + srcrect.w;
	int	y_start = dstrect.y;
	int	y_end = dstrect.y + srcrect.h;

	if (alpha > 100) alpha = 100;
	if (alpha < 0) alpha = 0;
	if (x_end > PD_screen_width) x_end = PD_screen_width;
	if (y_end > PD_screen_height) y_end = PD_screen_height;
	if (x_start > x_end || y_start > y_end) return -1;

	imagePix = (UCHAR*)imageList[index]->pixels;
	imagePitch = imageList[index]->pitch;

	PisteDraw2_DrawScreen_Start(screenPix, screenPitch);
	for (posx = x_start; posx < x_end; posx++)
		for (posy = y_start; posy < y_end; posy++) {
			color1 = imagePix[(posx-x_start+srcrect.x)+imagePitch*(posy-y_start+srcrect.y)];
			if (color1 != 255) {
				color2  = screenPix[posx+screenPitch*posy];
				screenPix[posx+screenPitch*posy] = PisteDraw2_BlendColors(color1, color2, alpha);
			}
		}
	PisteDraw2_DrawScreen_End();

	return 0;
}
void PisteDraw2_Image_GetSize(int index, int& w, int& h){
	w = imageList[index]->w;
	h = imageList[index]->h;
}
int PisteDraw2_Image_FlipHori(int index){
	int i, h, w, p;
	UCHAR* pix_array;

	h = imageList[index]->h;
	w = imageList[index]->w;
	p = imageList[index]->pitch;

	SDL_LockSurface(imageList[index]);

	pix_array  = (UCHAR*)(imageList[index]->pixels);

	for(i=0; i<h*p; i+=p)
		std::reverse(&pix_array[i],&pix_array[i + w]);

	SDL_UnlockSurface(imageList[index]);
	return 0;
}
int PisteDraw2_Image_Delete(int& index){
	if (imageList[index] == NULL) return -1;
	imageList[index]->format->palette = (SDL_Palette*)imageList[index]->userdata;
	SDL_FreeSurface(imageList[index]);
	imageList[index] = NULL;
	index = -1;
	return 0;
}

int PisteDraw2_Font_Create(int image, int x, int y, int char_w, int char_h, int count){
  int index;

	index = findfreefont();
	if (index==-1){
		printf("PisteEngine has run out of free fonts!");
		return -1;
	}

  fontList[index] = new PisteFont2(image, x, y, char_w, char_h, count);
  return index;
}
int PisteDraw2_Font_Create(char* path, char* file){
	int index;

	index = findfreefont();
	if (index==-1){
		printf("PisteEngine has run out of free fonts!");
		return -1;
	}

	fontList[index] = new PisteFont2();
  if (fontList[index]->LoadFile(path,file) == -1){
    printf("PisteEngine can't load a font from file!");
		delete fontList[index];
    return -1;
  }

  return index;
}
int PisteDraw2_Font_Write(int font_index, const char* text, int x, int y){
	return fontList[font_index]->Write_Text(x, y, text);
}
int PisteDraw2_Font_WriteAlpha(int font_index, const char* text, int x, int y, int alpha){
	return fontList[font_index]->Write_TextTrasparent(x, y, text, alpha);
}

int PisteDraw2_DrawScreen_Start(UCHAR* &pixels, int &pitch){
	pixels = (UCHAR*)frameBuffer->pixels;
	pitch = frameBuffer->pitch;
	return SDL_LockSurface(frameBuffer);
}
int PisteDraw2_DrawScreen_End(){
	SDL_UnlockSurface(frameBuffer);
	return 0;
}
int PisteDraw2_DrawImage_Start(int index, UCHAR* &pixels, int &pitch){
	pixels = (UCHAR*)imageList[index]->pixels;
	pitch = imageList[index]->pitch;
	return SDL_LockSurface(imageList[index]);
}
int PisteDraw2_DrawImage_End(int index){
	SDL_UnlockSurface(imageList[index]);

	return 0;
}
UCHAR PisteDraw2_BlendColors(UCHAR color, UCHAR colBack, int alpha){
	int result;

	if(alpha>100) alpha = 100;
	if(alpha<0) alpha = 0;

	result = color%32;
	result = (result*alpha)/100;
	result += colBack%32;
	if(result>31) result = 31;

	/*
	//For text
	UCHAR color1,color2,color3;
	color1 = color;
	color2 = colBack;
	color3 = (color2>>5)<<5;
	color2 -= color3;
	color1 = (color1 * (alpha) + color2 * (100-alpha))/100;
	result = color1 + color3;
	*/

	return result;//+32*col
}

int PisteDraw2_Start(int width, int height, const char* name){
	if(PD2_loaded) return -1;

	PD_Window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	PD_Renderer = SDL_CreateRenderer(PD_Window, -1, SDL_RENDERER_ACCELERATED);//| SDL_RENDERER_PRESENTVSYNC);
	PD_Texture = SDL_CreateTexture(PD_Renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width, height);

	frameBuffer = SDL_CreateRGBSurface(0, width, height, 8, 0, 0, 0, 0);

	SDL_ShowCursor(false);

	PD_screen_width = width;
	PD_screen_height = height;
	PD2_loaded = true;
	return 0;
}
int PisteDraw2_Exit(){
  if (!PD2_loaded) return -1;

  int i,j;

	for (i=0; i<MAX_IMAGES; i++)
    if (imageList[i] != NULL ){
			j = i;
      PisteDraw2_Image_Delete(j);
		}

  for (i=0; i<MAX_FONTS; i++){
      if (fontList[i] != NULL )
        delete fontList[i];
      fontList[i] = NULL;
  }

	SDL_FreeSurface(frameBuffer);
	SDL_DestroyTexture(PD_Texture);
	SDL_RenderClear(PD_Renderer);
	SDL_DestroyWindow(PD_Window);

  PD2_loaded = false;
  return 0;
}
void PisteDraw2_Update(bool draw){
	if(!PD2_loaded) return;

	if (PisteDraw2_IsFading()){
		PD_alpha += PD_fade_speed;
		if(PD_alpha < 0) PD_alpha = 0;
		if(PD_alpha > 255) PD_alpha = 255;
	}

	if(draw){
		void *Texturepix, *Framepix;
		int Texturepitch, Framepitch;

		SDL_Color* colors;
		UCHAR pal;
		int x, y;

		Framepitch = frameBuffer->pitch;
		Framepix = frameBuffer->pixels;
		colors = frameBuffer->format->palette->colors;

		SDL_LockTexture(PD_Texture, NULL, &Texturepix, &Texturepitch);
		for(x=0;x<PD_screen_width;x++)
			for(y=0;y<PD_screen_height;y++){
				pal = ((UCHAR*)Framepix)[x+Framepitch*y];
				((UCHAR*)Texturepix)[3*x+Texturepitch*y+0] = colors[pal].r;
				((UCHAR*)Texturepix)[3*x+Texturepitch*y+1] = colors[pal].g;
				((UCHAR*)Texturepix)[3*x+Texturepitch*y+2] = colors[pal].b;
			}
		SDL_UnlockTexture(PD_Texture);

		if(PD_alpha>0)
			SDL_SetTextureAlphaMod(PD_Texture, 256 - PD_alpha);

		SDL_RenderClear(PD_Renderer);
		SDL_RenderCopy(PD_Renderer, PD_Texture, NULL, NULL);
		SDL_RenderPresent(PD_Renderer);
	}
	SDL_FillRect(frameBuffer,NULL,0);
}






//##############################################################################








SDL_Window* 	PD_window = NULL;
SDL_Renderer* PD_renderer = NULL;
SDL_Texture* 	PD_texture = NULL;
SDL_Palette* 	PD_palette = NULL;
bool			PD_loaded = false;
const int		MAX_BUFFEREITA  = 2000;//depr
const int		MAX_FONTTEJA  = 10;//depr
SDL_Surface*	PD_buffers[MAX_BUFFEREITA]; //depr
int				PD_ruudun_leveys; //depr
int				PD_ruudun_korkeus; //depr
int				vasen_marginaali = 0; //depr
int				yla_marginaali = 0; //depr
int				PD_paletti_fade_pros  = 100; //depr
int				PD_paletti_fade_laskuri = 0; //depr
PisteFont*		PD_fontit[MAX_FONTTEJA]; //depr
int				PD_max_varit;
int				PD_font_korkein = 0;
int				PD_buffereita_varattu = 2;
char			virhe[60];

//A turma dos lixos
void PisteDraw_FullScreen(){
	SDL_SetWindowFullscreen(PD_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}
int	PisteDraw_SetColor(UCHAR color, int x, int y, int alpha){

	UCHAR* buffer_screen;
	int w;
	double a;

	buffer_screen = (UCHAR*)PD_buffers[PD_TAUSTABUFFER]->pixels;
	w = PD_buffers[PD_TAUSTABUFFER]->w;

	buffer_screen = &buffer_screen[4*(x+y*w)];

	if(alpha>=100){
		buffer_screen[0] = PD_palette->colors[color].b;
		buffer_screen[1] = PD_palette->colors[color].g;
		buffer_screen[2] = PD_palette->colors[color].r;
	} else{
		a = (double)alpha / 100;
		buffer_screen[0] = (UCHAR)((a * PD_palette->colors[color].b) + ((1-a) * buffer_screen[0]));
		buffer_screen[1] = (UCHAR)((a * PD_palette->colors[color].g) + ((1-a) * buffer_screen[1]));
		buffer_screen[2] = (UCHAR)((a * PD_palette->colors[color].r) + ((1-a) * buffer_screen[2]));
	}

	return 0;
}
char *PisteDraw_Locate_Kuva(char *filename){
  struct stat st;
  char *ret = strdup(filename);

  // expecting it to be 6+3 dos filename
  char *ext = strrchr(ret, '.');
  if(ext == NULL) return NULL;

  // cut up the path and file base components
  char *base = strrchr(ret, '/');
  // just a filename without dir
  if(base == NULL) {
    base = ret;
  }

	strcpy(ext, ".png");
  if(stat(ret, &st) == 0)
    return ret;
  else
  {
		char *c = base;
  	while(c != ext) *c++ = toupper(*c);

    if(stat(ret, &st) == 0)
      return ret;
    else
    {
			strcpy(ext, ".bmp");
      if(stat(ret, &st) == 0)
        return ret;
    }
  }
  return NULL;
}
int	PisteDraw_Start(int width, int height, char* name){

	PD_ruudun_leveys = width;
	PD_ruudun_korkeus = height;

	//------

	PD_window = SDL_CreateWindow(name,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_SHOWN);
	PD_renderer = SDL_CreateRenderer(PD_window, -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);

	int i;
	for(i=0;i<MAX_BUFFEREITA;i++)
		PD_buffers[i]=NULL;

	PD_buffers[PD_TAUSTABUFFER] = SDL_CreateRGBSurface(0, width, height, 32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0xFF000000);

	PD_texture = SDL_CreateTexture(PD_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);

	SDL_SetTextureBlendMode(PD_texture, SDL_BLENDMODE_BLEND);

	PD_palette = SDL_AllocPalette(256);
	//delete PD_palette->colors;


	SDL_ShowCursor(false);

	//------

	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,width,height);

	//PD_fontbuffer = PisteDraw_Buffer_Uusi(PD_ruudun_leveys,10,true,255);
	PD_loaded = true;

	return 0;
}
void PisteDraw_Paivita_Naytto(bool draw){
	//PisteDraw_Update
	if(!PD_loaded) return;

	bool isfading;
	isfading  = PD_paletti_fade_pros < 250 && PD_paletti_fade_laskuri > 0;
	isfading |= PD_paletti_fade_pros > 0   && PD_paletti_fade_laskuri < 0;

	if (isfading){
		PD_paletti_fade_pros += PD_paletti_fade_laskuri;
		PD_alpha = PD_paletti_fade_pros;
	}

	if(draw){
		SDL_UpdateTexture(PD_texture, NULL, PD_buffers[PD_TAUSTABUFFER]->pixels, PD_buffers[PD_TAUSTABUFFER]->pitch);

		if(PD_alpha>0)
			SDL_SetTextureAlphaMod(PD_texture, 255 - PD_alpha);

		SDL_RenderClear(PD_renderer);
		SDL_RenderCopy(PD_renderer, PD_texture, NULL, NULL);
		SDL_RenderPresent(PD_renderer);
	}
}
int PisteDraw_Lopeta(){
	//PisteDraw_Exit
  if (PD_loaded) {

    int i;

    for (i=0; i<MAX_FONTTEJA; i++){
      if (PD_fontit[i] != NULL )
        delete PD_fontit[i];
      PD_fontit[i] = NULL;
    }

    for (i=2;i<MAX_BUFFEREITA;i++)    // 0 ja 1 on varattu taustapuskureille
      PisteDraw_Buffer_Tuhoa(i);

    PisteDraw_Buffer_Tuhoa(PD_TAUSTABUFFER);

		SDL_FreePalette(PD_palette);

    PD_loaded = false;
  }

  return 0;
}
int PisteDraw_Buffer_Varaa(){
	//PisteDraw_FindFreeBuffer
	int i;
	for(i=0;i<MAX_BUFFEREITA;i++)
		if(PD_buffers[i]==NULL){
			PD_buffereita_varattu++;
			return i;
		}
	return -1;
}
int PisteDraw_Buffer_Uusi(int leveys, int korkeus, bool video_muisti, UCHAR color){
	//PisteDraw_CreateBuffer
	int index = PisteDraw_Buffer_Varaa();
	//int i;

	if(index<0) return index;

	PD_buffers[index] = SDL_CreateRGBSurface(0, leveys, korkeus, 8, 0, 0, 0, 0); //Dont create with SDL_CreateRGBSurface
	SDL_FreePalette(PD_buffers[index]->format->palette);
	PD_buffers[index]->format->palette = PD_palette;
	SDL_SetColorKey(PD_buffers[index], SDL_TRUE, 255);

	SDL_SetSurfaceBlendMode(PD_buffers[index], SDL_BLENDMODE_NONE); //TODO
	PisteDraw_Buffer_Tayta(index,255);

	return index;
}
int PisteDraw_Buffer_Uusi(int leveys, int korkeus, int colorfrom){
	return PisteDraw_Buffer_Uusi(leveys, korkeus, true, colorfrom);
	int index = PisteDraw_Buffer_Varaa();

	//printf("WARNING: Called Uusi 2 to %i\n",index);

	PD_buffers[index] = SDL_CreateRGBSurface(0, leveys, korkeus, 8/*bpp*/, 0, 0, 0, 0);
	if(PD_buffers[index] == NULL){
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_Color c = PD_buffers[colorfrom]->format->palette->colors[255];
	Uint32 transcolor = SDL_MapRGB(PD_buffers[index]->format, c.r,c.g,c.b);
	//printf("trans: %i %i,%i,%i\n", transcolor, c.r,c.g,c.b);
	SDL_SetColorKey(PD_buffers[index], SDL_TRUE, transcolor);

	return index;
}
bool PisteDraw_Buffer_Tuhoa(int i){
	//Destroy buffer
  if (i < 0 || i > MAX_BUFFEREITA || PD_buffers[i] == NULL)
    return false;
  SDL_Palette* temp = SDL_AllocPalette(1);
  PD_buffers[i]->format->palette = temp;
  SDL_FreeSurface(PD_buffers[i]);
  PD_buffers[i] = NULL;
  PD_buffereita_varattu--;

  return true;
}
void PisteDraw_GetBounds(int index, int& w, int& h){
	w = PD_buffers[index]->w;
	h = PD_buffers[index]->h;
}
int PisteDraw_Font_Uusi(int buffer_index, int buffer_x, int buffer_y, int leveys, int korkeus, int lkm){
  int index = 0;
  bool found = false;

  while (index<MAX_FONTTEJA && !found){
    if (PD_fontit[index] == NULL){
      PD_fontit[index] = new PisteFont(korkeus,leveys,lkm);
      //printf("PisteDraw_Font_Uusi %i bx:%i by:%i w:%i h:%i lkm:%i\n",
      //    buffer_index,buffer_x,buffer_y,leveys,korkeus,lkm);

      PD_fontit[index]->Get_bitmap(buffer_x,buffer_y,0/*unused*/,buffer_index);
      found = true;
    }
    else index++;
  }

  if (!found){
    printf("PisteEngine has run out of free fonts!");
    index = PD_VIRHE;
  }

  return index;
}
int PisteDraw_Font_Uusi(char *polku, char *tiedosto){
  int index = 0;
  bool found = false;

  while (index<MAX_FONTTEJA && !found){
    if (PD_fontit[index] == NULL){

      PD_fontit[index] = new PisteFont();

      if (PD_fontit[index]->LataaTiedostosta(polku,tiedosto) == -1){
        printf("PisteEngine can't load a font from file!");
        //printf("fon? %s %s\n", polku, tiedosto);

        return PD_VIRHE;
      }

      found = true;

      if (PD_fontit[index]->Korkeus() > PD_font_korkein)
		PD_font_korkein = PD_fontit[index]->Korkeus();
    }
    else index++;
  }

  if (!found){
    strcpy(virhe,"PisteEngine has run out of free fonts!");
    index = PD_VIRHE;
  }

  return index;
}
int PisteDraw_Font_Kirjoita(int font_index, char *text, int buffer_index, int x, int y){
	return PD_fontit[font_index]->Piirra_merkkijono(text, x, y, buffer_index);
}
int PisteDraw_Font_Kirjoita_Lapinakyva(int font_index, char *text, int buffer_index, int x, int y, UCHAR alpha){
	return PD_fontit[font_index]->Piirra_merkkijono_lapinakyva(text, x, y, buffer_index, alpha);
}
int PisteDraw_Lataa_Kuva(int index, char *filename, bool lataa_paletti){
	//Load Texture
	int i;
	//if(lataa_paletti) printf("Load palette from %i\n",index);
	PD_buffers[index] = SDL_LoadBMP(filename); //Load image from BMP

	if(!PD_buffers[index]){ //If don't exist, search
		char *altfile = PisteDraw_Locate_Kuva(filename);
		if(altfile != NULL)
			PD_buffers[index] = SDL_LoadBMP(altfile);
		if(!PD_buffers[index]){
			printf("IMG_Load: %s\n", IMG_GetError());
			return PD_VIRHE;
		}
	}
	//printf("Image loaded to buffer %i\n",index);

	//The palette allocated from BMP
	SDL_Palette* pal = PD_buffers[index]->format->palette;

	if(PD_buffers[index]->format->BitsPerPixel == 8){
  		if(lataa_paletti){ //If load pallete, change the PD_palette colors
			for(i=0;i<256;i++){
				PD_palette->colors[i] = pal->colors[i];
				PD_palette->colors[i].a = 255;
			}
			//Color_is_from = index;
		}

		//Delete the palette from BMP
		SDL_FreePalette(pal);

		//The buffers's palette now is the PD_palette
		PD_buffers[index]->format->palette = PD_palette;

		//Set color 255 to transparent
		SDL_SetColorKey(PD_buffers[index], SDL_TRUE, 255);
	}

	return 0;
}
int PisteDraw_Buffer_Flip(int i){
  SDL_BlitSurface(PD_buffers[i], NULL, PD_buffers[PD_TAUSTABUFFER], NULL);
  return 0;
}
int PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y){
	//Used by sprites.cpp
  return 0;
}
int PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y){
  SDL_Rect dst = {x, y, PD_buffers[lahde_index]->w, PD_buffers[lahde_index]->h};
  SDL_BlitSurface(PD_buffers[lahde_index], &PD_buffers[lahde_index]->clip_rect, PD_buffers[kohde_index], &dst);
  return 0;
}
int PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y,
                  int vasen, int yla, int oikea, int ala){
  SDL_Rect src = {vasen, yla, oikea-vasen,ala-yla};//PD_buffers[lahde_index]->w, PD_buffers[lahde_index]->h};
  SDL_Rect dest = {x, y, oikea-vasen, ala-yla};
  SDL_BlitSurface(PD_buffers[lahde_index], &src, PD_buffers[kohde_index], &dest);
  return 0;
}
int PisteDraw_Buffer_Flip_Vert(int index){
	int i;
	int height;
	int weight;
	UCHAR *col_buffer;
	UCHAR *pix_array;

	if (PD_buffers[index]->format->BitsPerPixel == 8){

		//printf("Virou %i\n", index);

		height = PD_buffers[index]->h;
		weight = PD_buffers[index]->w;

		col_buffer = new UCHAR[height];
		pix_array  = (UCHAR *)PD_buffers[index]->pixels;

		SDL_LockSurface(PD_buffers[index]);

		for(i=0; i<weight/2; i+= height){
		    memcpy(col_buffer          , &pix_array[i]       , height);
		    memcpy(&pix_array[i]       , &pix_array[weight-i], height); //TODO - fix
		    memcpy(&pix_array[weight-i], col_buffer          , height);
		}

		SDL_UnlockSurface(PD_buffers[index]);

		delete col_buffer;
		return 0;
	}
	return -1;
}
int PisteDraw_Buffer_Flip_Hori(int index){
	int i;
	int height;
	int weight;
	int pitch;
	UCHAR* pix_array;

	if (PD_buffers[index]->format->BitsPerPixel == 8){

		height = PD_buffers[index]->h;
		weight = PD_buffers[index]->w;
		pitch  = PD_buffers[index]->pitch;

		pix_array  = (UCHAR*)(PD_buffers[index]->pixels);

		SDL_LockSurface(PD_buffers[index]);

		for(i=0; i<height*pitch; i+=pitch)
			std::reverse(&pix_array[i],&pix_array[i + weight]);

		SDL_UnlockSurface(PD_buffers[index]);

		return 0;
	}

	return -1;
}
int PisteDraw_Buffer_Tayta(int i, UCHAR color){
	//Preencher Buffer
	return PisteDraw_Buffer_Tayta(i, 0, 0, PD_buffers[i]->w, PD_buffers[i]->h, color);
}
int PisteDraw_Buffer_Tayta(int i, int vasen, int yla, int oikea, int ala, UCHAR color){
	SDL_Rect r = {vasen, yla, oikea-vasen, ala-yla};
	SDL_Color col;
	if(PD_buffers[i]->format->BitsPerPixel == 8)
		return SDL_FillRect(PD_buffers[i], &r, color); //TODO Doing

	col = PD_palette->colors[color];
	return SDL_FillRect(PD_buffers[i], &r, SDL_MapRGB(PD_buffers[i]->format, col.r, col.g, col.b));
}
void PisteDraw_Aseta_Marginaali_Vasen(int vasen){
  vasen_marginaali = vasen;
}
void PisteDraw_Aseta_Marginaali_Yla(int yla){
  yla_marginaali = yla;
}
int PisteDraw_Piirto_Aloita(int i, UCHAR *&back_buffer, DWORD &lPitch){
  back_buffer = (UCHAR*)PD_buffers[i]->pixels;
  // should it be (y*pitch) + x*bpp
  //lPitch = PD_buffers[i]->format->BytesPerPixel;
  lPitch = PD_buffers[i]->w; //Danilo
  return SDL_LockSurface(PD_buffers[i]);
}
int PisteDraw_Piirto_Lopeta(void){
  int i=0;
  while (i<MAX_BUFFEREITA /*PD_buffereita_varattu*/)
  {
    if (PD_buffers[i]->pixels != NULL)
      SDL_UnlockSurface(PD_buffers[i]);
    i++;
  }
  return 0;
}
int PisteDraw_Piirto_Lopeta(int i){
  SDL_UnlockSurface(PD_buffers[i]);
  return 0;
}
void PisteDraw_Aseta_Klipperi(int i, int vasen, int yla, int oikea, int ala){
  SDL_Rect r = {vasen, yla, oikea-vasen, ala-yla};
  SDL_SetClipRect(PD_buffers[i], &r);
}
int PisteDraw_Paletti_Pyorita(UCHAR eka_vari, UCHAR vika_vari){
	UCHAR i;

	SDL_Color *colors;
	//colors = PD_paletti_nyt;
	colors = PD_palette->colors;

	SDL_Color temp_vari;

	temp_vari = colors[vika_vari];

	for (i = vika_vari ; i>eka_vari ; i--) //TODO
		colors[i] = colors[i-1];

	colors[eka_vari] = temp_vari;

	return 0;
}
void PisteDraw_Fade_Paletti_In(int laskuri){
  PD_paletti_fade_pros    =  250;
  PD_paletti_fade_laskuri = -laskuri;
}
void PisteDraw_Fade_Paletti_Out(int laskuri){
  PD_paletti_fade_pros    =  0;
  PD_paletti_fade_laskuri =  laskuri;
}
bool PisteDraw_Fade_Paletti_Valmis(void){
  if (PD_paletti_fade_pros > 0   && PD_paletti_fade_laskuri < 0)
    return false;

  if (PD_paletti_fade_pros < 250 && PD_paletti_fade_laskuri > 0)
    return false;

  return true;
}
int PisteDraw_Reset_Paletti(void){
  return 0;
}
int PisteDraw_Paletti_Set(void){
  //if (FAILED(PD_lpddpal->SetEntries(0,0,255,PD_paletti_nyt))) {

  return 0;
}
