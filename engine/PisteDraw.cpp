//#########################
//PisteEngine - PisteDraw
//by Janne Kivilahti from Piste Gamez
//#########################

#include "PisteDraw.h"
#include "PisteFont.h"
#include "PisteInput.h" //Just for debugger

#include <cstdio>
#include <fstream>
#include <sys/stat.h>
#include <algorithm>

#include <SDL2/SDL_image.h>

const int		MAX_BUFFEREITA  = 2000;
const int		MAX_FONTTEJA  = 10;

SDL_Window* 	PD_window = NULL;
SDL_Renderer* 	PD_renderer = NULL;
SDL_Texture* 	PD_texture = NULL;

SDL_Surface*	PD_buffers[MAX_BUFFEREITA]; // taulukko kuvabuffereista
SDL_Palette* 	PD_palette;
int 			Color_is_from = -1; //Dont need

//Create a function (Load Image to pallete)
//All palletes will be from this image

int				PD_ruudun_leveys;
int				PD_ruudun_korkeus;
int				PD_max_varit;
int				PD_buffereita_varattu = 2;

int				vasen_marginaali = 0,
				yla_marginaali = 0;

int				PD_paletti_fade_pros  = 100;
int				PD_paletti_fade_laskuri = 0;

PisteFont*		PD_fontit[MAX_FONTTEJA];
int				PD_font_korkein = 0;

char			virhe[60];

bool			PD_unload = true;

int 			PD_alpha = 0;

// Edited --

void PisteDraw_FullScreen(){
	SDL_SetWindowFullscreen(PD_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

int	PisteDraw_SetColor(UCHAR color, int x, int y, int alpha){

	//PD_buffers[PD_TAUSTABUFFER] = SDL_CreateRGBSurface(0, leveys, korkeus, 32,
	//	0x00FF0000,
	//	0x0000FF00,
	//	0x000000FF,
	//	0xFF000000);
	UCHAR* buffer_screen;
	int w;
	double a;

	//printf("x - %i y - %i\n",x,y);
	//fflush(stdin);

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


/* Debugger --------------------------------------------------------------------------------*/

const int		DEB_PALETTE = 0;
const int		DEB_BUFFERS = 1;

int				deb_menu = 0;

SDL_Window* 	debug_window = NULL;
SDL_Surface* 	debug_surface = NULL;
SDL_Texture* 	debug_texture = NULL;
SDL_Renderer*	debug_renderer = NULL;

bool			debug = false;

SDL_Palette* 	debug_palette;

MOUSE mouse;

int PisteDraw_Alusta_Debugger(){
	int i;

	if(debug == true) return -1;

    debug_window = SDL_CreateWindow( "PD Debugger",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                400, 300, SDL_WINDOW_SHOWN);

	debug_renderer = SDL_CreateRenderer(debug_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	debug_surface = SDL_CreateRGBSurface(0, 400, 300, 32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0xFF000000);
	
	debug_texture = SDL_CreateTexture(debug_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		400, 300);	

	debug_palette = SDL_AllocPalette(256);
	for(i=0;i<256;i++){
		debug_palette->colors[i].r = i;
		debug_palette->colors[i].g = i;
		debug_palette->colors[i].b = i;
		debug_palette->colors[i].a = 255;
	}

	debug = true;
	return 0;
}

int keydelay;
int buf = 0;

int Update_Debugger_Buffers(){
	SDL_Rect but1;
	SDL_Rect but2;
	SDL_Rect dest;
	int col = 0x0011FF00;
	
	but1.x = 10;
	but1.y = 50;
	but1.w = 50;
	but1.h = 10;

	if (mouse.x > but1.x && mouse.x < but1.x + but1.w &&
		mouse.y > but1.y && mouse.y < but1.y + but1.h){

		col = 255;
		if (PisteInput_Hiiri_Vasen() && keydelay==0){
			keydelay = 5;
			buf++;
			printf("buffer %i\n",buf);
		}
		if (PisteInput_Hiiri_Oikea() && keydelay==0){
			keydelay = 5;
			buf--;
			printf("buffer %i\n",buf);
		}
	}
	SDL_FillRect(debug_surface, &but1,col);
	
	but2.x = 10;
	but2.y = 70;
	but2.w = 50;
	but2.h = 10;
	
	if (mouse.x > but2.x && mouse.x < but2.x + but2.w &&
		mouse.y > but2.y && mouse.y < but2.y + but2.h){

		if (PisteInput_Hiiri_Oikea() && keydelay==0){
			deb_menu = DEB_PALETTE;
		}
	}
	SDL_FillRect(debug_surface, &but2,5600);
	
	dest.x = 70;
	dest.y = 0;
	dest.w = 300;
	dest.h = 330;
	
	SDL_BlitSurface(PD_buffers[buf], NULL, debug_surface, &dest);
	
	if(keydelay>0) keydelay--;
	
}

void Update_Debugger_Palette(){
	int i;
	int col = 0;
	SDL_Rect pal;
	SDL_Rect but1;
	SDL_Rect but2;
	SDL_Palette* buf;
	
	for(i=0;i<256;i++){
		pal.x = 13 * (i%16) + 96;
		pal.y = 13 * (int)(i/16) + 46;
		pal.w = 11;
		pal.h = 11;
		SDL_FillRect(debug_surface, &pal, 
			PD_palette->colors[i].r<<16 |
			PD_palette->colors[i].g<<8  |
			PD_palette->colors[i].b);
	}

	but1.x = 10;
	but1.y = 50;
	but1.w = 50;
	but1.h = 10;

	if (mouse.x > but1.x && mouse.x < but1.x + but1.w &&
		mouse.y > but1.y && mouse.y < but1.y + but1.h){

		col = 255;
		//Swap the address of debug_palette and PD_palette
		if (PisteInput_Hiiri_Vasen()){
			buf = PD_palette;
			PD_palette = debug_palette;
			debug_palette = PD_palette;
		}
	}
	SDL_FillRect(debug_surface, &but1,col);


	but2.x = 10;
	but2.y = 70;
	but2.w = 50;
	but2.h = 10;
	if (mouse.x > but2.x && mouse.x < but2.x + but2.w &&
		mouse.y > but2.y && mouse.y < but2.y + but2.h)
		if (PisteInput_Hiiri_Vasen())
			deb_menu = DEB_BUFFERS;
	SDL_FillRect(debug_surface, &but2,0x00FF0000);
}

int Update_Debugger(){

	mouse = PisteInput_Hiiri();

	unsigned int* pix;

	SDL_Rect 	top = {0,0  ,400,40 },
				cen = {0,40 ,400,260},
				bot = {0,260,400,40 };

	SDL_FillRect(debug_surface, &top, 0x000010FF);
	SDL_FillRect(debug_surface, &cen, 0x00808080);
	SDL_FillRect(debug_surface, &bot, 0x000010FF);

	if(deb_menu == DEB_PALETTE)
		Update_Debugger_Palette();
	if(deb_menu == DEB_BUFFERS)
		Update_Debugger_Buffers();


	pix = (unsigned int*)debug_surface->pixels;
	if (mouse.x<400 && mouse.y<300)
		pix[mouse.x+mouse.y*debug_surface->w] = 0;

	SDL_UpdateTexture(debug_texture, NULL, debug_surface->pixels, debug_surface->pitch);

	SDL_RenderClear(debug_renderer);
	SDL_RenderCopy(debug_renderer, debug_texture, NULL, NULL);
	SDL_RenderPresent(debug_renderer);

}


/* METHODS -----------------------------------------------------------------------------------*/

//PisteDraw_Start
int PisteDraw_Alusta(int leveys, int korkeus, char* name){
	PD_ruudun_leveys = leveys;
	PD_ruudun_korkeus = korkeus;

	//------

	PD_window = SDL_CreateWindow(name,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		leveys, korkeus, SDL_WINDOW_SHOWN);
	PD_renderer = SDL_CreateRenderer(PD_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	int i;
	for(i=0;i<MAX_BUFFEREITA;i++)
		PD_buffers[i]=NULL;

	PD_buffers[PD_TAUSTABUFFER] = SDL_CreateRGBSurface(0, leveys, korkeus, 32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0xFF000000);
	
	PD_texture = SDL_CreateTexture(PD_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		leveys, korkeus);

	SDL_SetTextureBlendMode(PD_texture, SDL_BLENDMODE_BLEND);
	
	PD_palette = SDL_AllocPalette(256);
	//delete PD_palette->colors;


	SDL_ShowCursor(false);
	
	//------

	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,leveys,korkeus);

	//PD_fontbuffer = PisteDraw_Buffer_Uusi(PD_ruudun_leveys,10,true,255);

	return 0;
}

//PisteDraw_Update
void PisteDraw_Paivita_Naytto(bool draw){

	bool isfading;
	isfading  = PD_paletti_fade_pros < 250 && PD_paletti_fade_laskuri > 0;
	isfading |= PD_paletti_fade_pros > 0   && PD_paletti_fade_laskuri < 0;
	
	if (isfading){
		PD_paletti_fade_pros += PD_paletti_fade_laskuri;
		PD_alpha = PD_paletti_fade_pros;
	}

	if(debug) Update_Debugger();
	
	if(draw){
		SDL_UpdateTexture(PD_texture, NULL, PD_buffers[PD_TAUSTABUFFER]->pixels, PD_buffers[PD_TAUSTABUFFER]->pitch);

		if(PD_alpha>0)
			SDL_SetTextureAlphaMod(PD_texture, 255 - PD_alpha);

		SDL_RenderClear(PD_renderer);
		SDL_RenderCopy(PD_renderer, PD_texture, NULL, NULL);
		SDL_RenderPresent(PD_renderer);
	}
}

//PisteDraw_Exit
int PisteDraw_Lopeta(){
  if (!PD_unload) {

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
	
	if(debug == true) SDL_FreePalette(debug_palette);

    PD_unload = true;
  }

  return 0;
}

//PisteDraw_FindFreeBuffer
int PisteDraw_Buffer_Varaa(){
	int i;
	for(i=0;i<MAX_BUFFEREITA;i++)
		if(PD_buffers[i]==NULL){
			PD_buffereita_varattu++;
			return i;
		}
	return -1;
}

//PisteDraw_CreateBuffer
int PisteDraw_Buffer_Uusi(int leveys, int korkeus, bool video_muisti, UCHAR color){

	int index = PisteDraw_Buffer_Varaa();
	int i;
	
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
  if (i < 0 || i > MAX_BUFFEREITA || PD_buffers[i] == NULL)
    return false;
  SDL_Palette* temp = SDL_AllocPalette(1);
  PD_buffers[i]->format->palette = temp;
  SDL_FreeSurface(PD_buffers[i]);
  PD_buffers[i] = NULL;
  PD_buffereita_varattu--;

  return true;
}

// for trying to guess an available filename if requested one wasn't there
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

void PisteDraw_GetBounds(int index, int& w, int& h){
	w = PD_buffers[index]->w;
	h = PD_buffers[index]->h;
}

//TODO - It loads a new buffer and forget de old allocated buffer's pointer
int PisteDraw_Lataa_Kuva(int index, char *filename, bool lataa_paletti){
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
			Color_is_from = index;
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

int PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y){

  //SDL_Rect dest = {x, y, PD_buffers[lahde_index]->w, PD_buffers[lahde_index]->h};
  //SDL_Surface *rotopicture = rotozoomSurfaceXY(PD_buffers[lahde_index], 0, -1.0, 1.0, 1);
  //SDL_BlitSurface(rotopicture, NULL, PD_buffers[kohde_index], &dest);
  //SDL_FreeSurface(rotopicture);
  return 0;
}

int PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y, 
                  int vasen, int yla, int oikea, int ala){
  SDL_Rect src = {vasen, yla, oikea-vasen,ala-yla};//PD_buffers[lahde_index]->w, PD_buffers[lahde_index]->h};
  SDL_Rect dest = {x, y, oikea-vasen, ala-yla};
  SDL_BlitSurface(PD_buffers[lahde_index], &src, PD_buffers[kohde_index], &dest);
  return 0;
}

int PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y){
  SDL_Rect dst = {x, y, PD_buffers[lahde_index]->w, PD_buffers[lahde_index]->h};
  SDL_BlitSurface(PD_buffers[lahde_index], &PD_buffers[lahde_index]->clip_rect, PD_buffers[kohde_index], &dst);
  return 0;
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

int PisteDraw_Buffer_Tayta(int i, UCHAR color){
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

void PisteDraw_Aseta_Klipperi(int i, int vasen, int yla, int oikea, int ala){
  SDL_Rect r = {vasen, yla, oikea-vasen, ala-yla};
  SDL_SetClipRect(PD_buffers[i], &r);
}

void PisteDraw_Aseta_Marginaali_Vasen(int vasen){
  vasen_marginaali = vasen;
}

void PisteDraw_Aseta_Marginaali_Yla(int yla){
  yla_marginaali = yla;
}

// Palettiin liittyvat metodit
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
//  if (FAILED(PD_lpddpal->SetEntries(0,0,255,PD_paletti))) { 
  return 0;
}

int PisteDraw_Paletti_Set(void){
  //if (FAILED(PD_lpddpal->SetEntries(0,0,255,PD_paletti_nyt))) {

  return 0;
}

int PisteDraw_Font_Uusi(int buffer_index, int buffer_x, int buffer_y, int leveys, int korkeus, int lkm){
  int index = 0;
  bool found = false;

  while (index<MAX_FONTTEJA && !found)
  {
    if (PD_fontit[index] == NULL)
    {
      PD_fontit[index] = new PisteFont(korkeus,leveys,lkm);
      //printf("PisteDraw_Font_Uusi %i bx:%i by:%i w:%i h:%i lkm:%i\n", 
      //    buffer_index,buffer_x,buffer_y,leveys,korkeus,lkm);
      
      PD_fontit[index]->Get_bitmap(buffer_x,buffer_y,0/*unused*/,buffer_index);
      found = true;
    }
    else
      index++;
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
