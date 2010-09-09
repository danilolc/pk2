/* 
 PisteEngineSDL - PisteDraw
 <samuli.tuomola@gmail.com>
*/

/* INCLUDES ----------------------------------------------------------------------------------*/

#include "PisteDraw.h"
#include "PisteFont.h"
#include "PisteLog.h"
#include <stdio.h>
#include <fstream>
#include <malloc.h>
#include <memory.h>
#include <sys/stat.h>

#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_gfxPrimitives.h>

/* DEFINES -----------------------------------------------------------------------------------*/

/* TYPE DEFINITIONS --------------------------------------------------------------------------*/

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;
typedef unsigned long  HWND;
typedef unsigned long  HINSTANCE;

/* VARIABLES ---------------------------------------------------------------------------------*/

const int       MAX_BUFFEREITA  = 2000;
const int       MAX_FONTTEJA  = 10;

SDL_Color     PD_paletti[256];          // color palette
SDL_Color     PD_paletti_nyt[256];      // paletin talletusta varten
SDL_Surface     *PD_buffers[MAX_BUFFEREITA]; // taulukko kuvabuffereista

int           PD_ruudun_leveys;
int           PD_ruudun_korkeus;
int           PD_ruudun_bpp;
int           PD_max_varit;
int           PD_buffereita_varattu = 2;

int           vasen_marginaali = 0,
            yla_marginaali = 0;

int           PD_paletti_fade_pros  = 100;
int           PD_paletti_fade_laskuri = 0;

PisteFont       *PD_fontit[MAX_FONTTEJA];
int           PD_fontbuffer;
int           PD_font_korkein = 0;

char          virhe[60];

bool          PD_unload = true;

/* PROTOTYPES --------------------------------------------------------------------------------*/

/* METHODS -----------------------------------------------------------------------------------*/

int PD_Loki_Kirjoita(char *viesti)
{
    return(0);  
  
    int virhe = 0;

    char *filename = "fonttiloki.txt";
      
    FILE *tiedosto;

    if ((tiedosto = fopen(filename, "a")) == NULL)
    {
      return(1);
    }

    char mjono[255];

    //memset(mjono,' ',sizeof(mjono));
    //mjono[60] = '\n';

    strcpy(mjono,viesti);

    fwrite(mjono,   sizeof(UCHAR),  strlen(mjono),  tiedosto);

    fclose(tiedosto);

  return(0);
}


int PisteDraw_Alusta(HWND &main_window_handle, HINSTANCE &hinstance_app, 
           int leveys, int korkeus, int bpp,
           int max_colors)
{
  PD_ruudun_leveys = leveys;
  PD_ruudun_korkeus = korkeus;
  PD_ruudun_bpp = bpp;
  PD_buffers[PD_TAUSTABUFFER] = SDL_SetVideoMode(leveys, korkeus, bpp, SDL_SWSURFACE|SDL_ANYFORMAT|SDL_DOUBLEBUF);
  if(PD_buffers[PD_TAUSTABUFFER] == NULL) {
    fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
    exit(1);
  }
  PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,leveys,korkeus);

  PD_fontbuffer = PisteDraw_Buffer_Uusi(PD_ruudun_leveys,10,true,255);

  return 0;
}

int PisteDraw_Aseta_Videomode(int leveys, int korkeus, int bpp,
           int max_colors)
{
/*
  if (FAILED(PD_lpdd->SetDisplayMode(PD_ruudun_leveys, PD_ruudun_korkeus, PD_ruudun_bpp,0,0)))
  {
    strcpy(virhe,"Unable to change video mode!");
    PisteLog_Kirjoita("[Error] Piste Draw: Unable to change video mode! \n");
    return PD_VIRHE;
  }*/
  return 0;
}

int PisteDraw_Lopeta()
{

  if (!PD_unload) {

    int i;

    for (i=0; i<MAX_FONTTEJA; i++)
    {
      if (PD_fontit[i] != NULL )
        delete PD_fontit[i];
      PD_fontit[i] = NULL;
    }

    for (i=2;i<MAX_BUFFEREITA;i++)    // 0 ja 1 on varattu taustapuskureille
      PisteDraw_Buffer_Tuhoa(i);

    PisteDraw_Buffer_Tuhoa(PD_TAUSTABUFFER);

    PD_unload = true;
  }

  return 0;
}

DWORD PisteDraw_Videomuistia()
{
/*
  DDCAPS hel_caps, hal_caps;

  DD_INIT_STRUCT(hel_caps);
  DD_INIT_STRUCT(hal_caps); 
  
  if (FAILED(PD_lpdd->GetCaps(&hal_caps,&hel_caps)))  
  {
    strcpy(virhe,"Cannot aquire system information!");
    return PD_VIRHE;
  }
  
  return hal_caps.dwVidMemFree;*/
  return 1;
}

DWORD PisteDraw_Videomuistia_Max()
{
/*
  DDCAPS hel_caps, hal_caps;

  DD_INIT_STRUCT(hel_caps);
  DD_INIT_STRUCT(hal_caps); 
  
  if (FAILED(PD_lpdd->GetCaps(&hal_caps,&hel_caps)))  
  {
    strcpy(virhe,"Cannot aquire system information!");
    PisteLog_Kirjoita("[Error] Piste Draw: Cannot aquire system information! \n");
    return PD_VIRHE;
  }

  return hal_caps.dwVidMemTotal;
*/
}

int PisteDraw_Buffer_Varaa() {
  return ++PD_buffereita_varattu;
}

int PisteDraw_Buffer_Uusi(int leveys, int korkeus, bool video_muisti, UCHAR color)
{
  int i = PisteDraw_Buffer_Varaa();

//  printf("PisteDraw_Buffer_Uusi %i %i\n", leveys, korkeus);

  PD_buffers[i] = SDL_CreateRGBSurface(SDL_SWSURFACE,leveys,korkeus,PD_ruudun_bpp,0,0,0,0);
  if(PD_buffers[i] == NULL) {
    fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
    exit(1);
  }
  // TODO: hmm, separate 8-bit function?
  Uint32 transcolor = SDL_MapRGB(PD_buffers[i]->format, 155,232,224);
  SDL_SetColorKey(PD_buffers[i], SDL_SRCCOLORKEY|SDL_RLEACCEL, transcolor);

  return i;
}

int PisteDraw_Buffer_Uusi(int leveys, int korkeus, int colorfrom)
{
  int i = PisteDraw_Buffer_Varaa();

  PD_buffers[i] = SDL_CreateRGBSurface(SDL_SWSURFACE,leveys,korkeus,PD_ruudun_bpp,0,0,0,0);
  if(PD_buffers[i] == NULL) {
    fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
    exit(1);
  }
  // TODO?
  SDL_Color c = PD_buffers[colorfrom]->format->palette->colors[255];
  Uint32 transcolor = SDL_MapRGB(PD_buffers[i]->format, c.r,c.g,c.b);
  //printf("trans: %i %i,%i,%i\n", transcolor, c.r,c.g,c.b);
  SDL_SetColorKey(PD_buffers[i], SDL_SRCCOLORKEY|SDL_RLEACCEL, transcolor);

  return i;
}


bool PisteDraw_Buffer_Tuhoa(int i)
{
  if (i < 0 || i > MAX_BUFFEREITA)
    return false;

  SDL_FreeSurface(PD_buffers[i]);

  return true;
}

void strtoupper(char *s) {
  if(s == NULL)
    return;
  while(*s != '\0')
    *s++ = toupper(*s);
}

// for trying to guess an available filename if requested one wasn't there
char *PisteDraw_Locate_Kuva(char *filename)
{
  // cut up the path and file base components
  char *dir = strdup(filename);
  char *base = strrchr(dir, '/');
  struct stat st;
  if(base != NULL) {
    *base++ = '\0';
  } else {
    // just filename
    base = dir;
  }
  // expecting it to be 6+3 dos filename, forget the extension
  char *t = strrchr(base, '.');
  if(t != NULL) *t = '\0';

  char altfile[strlen(filename)];
  sprintf(altfile, "%s/%s.png", dir==base?".":dir, base);
  if(stat(altfile, &st) == 0)
    return altfile;
  else
  {
    strtoupper(base);
    sprintf(altfile, "%s/%s.png", dir==base?".":dir, base);
    if(stat(altfile, &st) == 0)
      return altfile;
    else
    {
      sprintf(altfile, "%s/%s.bmp", dir==base?".":dir, base);
      free(dir);
      if(stat(altfile, &st) == 0)
        return altfile;
      else
        return NULL;
    }
  }
  free(dir);
}

// TODO: repurposed lataa_paletti, now meaning enable_transparency
int PisteDraw_Lataa_Kuva(int index, char *filename, bool lataa_paletti)
{
  PD_buffers[index] = IMG_Load(filename);
  if(!PD_buffers[index])
  {
    char *altfile = PisteDraw_Locate_Kuva(filename);
    if(altfile != NULL)
      PD_buffers[index] = IMG_Load(altfile);
    if(!PD_buffers[index])
    {
       printf("IMG_Load: %s\n", IMG_GetError());
       return PD_VIRHE;
    }
  }

  if(lataa_paletti) {
    if(PD_buffers[index]->format->BitsPerPixel != 8)
      printf("BPP %i! %s\n",PD_buffers[index]->format->BitsPerPixel, filename);
    SDL_SetColorKey(PD_buffers[index], SDL_SRCCOLORKEY|SDL_RLEACCEL, 255);
  }

  return 0;
}

int PisteDraw_Buffer_Flip(int i)
{
  SDL_BlitSurface(PD_buffers[i], NULL, PD_buffers[PD_TAUSTABUFFER], NULL);
  return 0;
}

int PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y)
{
  SDL_Rect dest = {x, y, PD_buffers[lahde_index]->w, PD_buffers[lahde_index]->h};
  SDL_Surface *rotopicture = rotozoomSurfaceXY(PD_buffers[lahde_index], 0, -1.0, 1.0, 1);
  SDL_BlitSurface(rotopicture, NULL, PD_buffers[kohde_index], &dest);
  SDL_FreeSurface(rotopicture);
  return 0;
}

int PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y, 
                  int vasen, int yla, int oikea, int ala)
{
  SDL_Rect src = {vasen, yla, oikea-vasen,ala-yla};//PD_buffers[lahde_index]->w, PD_buffers[lahde_index]->h};
  SDL_Rect dest = {x, y, oikea-vasen, ala-yla};
  SDL_BlitSurface(PD_buffers[lahde_index], &src, PD_buffers[kohde_index], &dest);
  return 0;
}

int PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y)
{
  SDL_Rect dst = {x, y, PD_buffers[lahde_index]->w, PD_buffers[lahde_index]->h};
  SDL_BlitSurface(PD_buffers[lahde_index], &PD_buffers[lahde_index]->clip_rect, PD_buffers[kohde_index], &dst);
  return 0;
}

int PisteDraw_Piirto_Aloita(int i, UCHAR *&back_buffer, DWORD &lPitch)
{
  back_buffer = (UCHAR*)PD_buffers[i]->pixels;
  // should it be (y*pitch) + x*bpp 
  lPitch = PD_buffers[i]->format->BytesPerPixel;
  return SDL_LockSurface(PD_buffers[i]);
}

int PisteDraw_Piirto_Lopeta(void)
{
  int i=0;
  while (i<MAX_BUFFEREITA /*PD_buffereita_varattu*/)
  {
    if (PD_buffers[i]->pixels != NULL)
      SDL_UnlockSurface(PD_buffers[i]);
    i++;
  }
  return 0;
}

int PisteDraw_Piirto_Lopeta(int i)
{
  SDL_UnlockSurface(PD_buffers[i]);
  return 0;
}

int PisteDraw_Buffer_Tayta(int i, UCHAR color)
{
  return PisteDraw_Buffer_Tayta(i, 0, 0, PD_buffers[i]->w, PD_buffers[i]->h, color);
}

int PisteDraw_Buffer_Tayta(int i, int vasen, int yla, int oikea, int ala, UCHAR color)
{
  SDL_Rect r = {vasen, yla, oikea-vasen, ala-yla};
  return SDL_FillRect(PD_buffers[i], &r, color);
}

void PisteDraw_Aseta_Klipperi(int i, int vasen, int yla, int oikea, int ala)
{
  SDL_Rect r = {vasen, yla, oikea-vasen, ala-yla};
  SDL_SetClipRect(PD_buffers[i], &r);
}

void PisteDraw_Aseta_Marginaali_Vasen(int vasen)
{
  vasen_marginaali = vasen;
}

void PisteDraw_Aseta_Marginaali_Yla(int yla)
{
  yla_marginaali = yla;
}

// Palettiin liittyvät metodit

int PisteDraw_Fade_Paletti_Do(int pros)
{
  UCHAR i;
  //SDL_SetAlpha(PD_buffers[PD_TAUSTABUFFER], SDL_SRCALPHA, pros);
  boxRGBA(PD_buffers[PD_TAUSTABUFFER], 0,0, PD_buffers[PD_TAUSTABUFFER]->w, PD_buffers[PD_TAUSTABUFFER]->h, 0,0,0, 200-pros);
  
  return 0;
}

int PisteDraw_Paletti_Pyorita(UCHAR eka_vari, UCHAR vika_vari)
{
  UCHAR i;
  
//  if (FAILED(PD_lpddpal->GetEntries(0,0,255,PD_paletti_nyt))) {

  SDL_Color temp_vari;
    
  temp_vari.r   = PD_paletti_nyt[vika_vari].r;
  temp_vari.g = PD_paletti_nyt[vika_vari].g;
  temp_vari.b  = PD_paletti_nyt[vika_vari].b;

  for (i=vika_vari;i>eka_vari;i--)
  {
    PD_paletti_nyt[i].r   = PD_paletti_nyt[i-1].r;
    PD_paletti_nyt[i].g = PD_paletti_nyt[i-1].g;
    PD_paletti_nyt[i].b  = PD_paletti_nyt[i-1].b;
  }

  PD_paletti_nyt[eka_vari].r   = temp_vari.r;
  PD_paletti_nyt[eka_vari].g = temp_vari.g;
  PD_paletti_nyt[eka_vari].b  = temp_vari.b;

  // SDL_SetPalette
//  if (FAILED(PD_lpddpal->SetEntries(0,0,255,PD_paletti_nyt))) {
  
  return 0;
}
/*

int PisteDraw_Paletti_Pyorita(UCHAR eka_vari, UCHAR vika_vari)
{
  UCHAR i;
  
  if (FAILED(PD_lpddpal->GetEntries(0,0,255,PD_paletti_nyt)))
    return PD_VIRHE;

  SDL_Color temp_vari;
    
  temp_vari.r   = PD_paletti_nyt[eka_vari].r;
  temp_vari.g = PD_paletti_nyt[eka_vari].g;
  temp_vari.b  = PD_paletti_nyt[eka_vari].b;

  for (i=eka_vari;i<vika_vari;i++)
  {
    PD_paletti_nyt[i].r   = PD_paletti_nyt[i+1].r;
    PD_paletti_nyt[i].g = PD_paletti_nyt[i+1].g;
    PD_paletti_nyt[i].b  = PD_paletti_nyt[i+1].b;
  }

  PD_paletti_nyt[vika_vari].r   = temp_vari.r;
  PD_paletti_nyt[vika_vari].g = temp_vari.g;
  PD_paletti_nyt[vika_vari].b  = temp_vari.b;

  if (FAILED(PD_lpddpal->SetEntries(0,0,255,PD_paletti_nyt)))
    return PD_VIRHE;  
  
  return 0;
}
*/
int PisteDraw_Fade_Paletti(void)
{
  if ((PD_paletti_fade_pros < 100 && PD_paletti_fade_laskuri > 0) ||
    (PD_paletti_fade_pros > 0 && PD_paletti_fade_laskuri < 0))
  {
    PD_paletti_fade_pros += PD_paletti_fade_laskuri;
    if (PisteDraw_Fade_Paletti_Do(PD_paletti_fade_pros)==PD_VIRHE) {
      PisteLog_Kirjoita("[Error] Piste Draw: Palette fade failed!\n");  
      return PD_VIRHE;
    }
  } 

  return 0;
}

void PisteDraw_Fade_Paletti_Set(int laskuri)
{
  PD_paletti_fade_laskuri = laskuri;
}

void PisteDraw_Fade_Paletti_In(int laskuri)
{
  PD_paletti_fade_pros    =  0;
  PD_paletti_fade_laskuri =  laskuri; 
}

void PisteDraw_Fade_Paletti_Out(int laskuri)
{
  PD_paletti_fade_pros    =  200;
  PD_paletti_fade_laskuri = -laskuri; 
}

bool PisteDraw_Fade_Paletti_Valmis(void)
{
  if (PD_paletti_fade_pros > 0   && PD_paletti_fade_laskuri < 0)
    return false;

  if (PD_paletti_fade_pros < 100 && PD_paletti_fade_laskuri > 0)
    return false;

  return true;
}

int PisteDraw_Reset_Paletti(void)
{
//  if (FAILED(PD_lpddpal->SetEntries(0,0,255,PD_paletti))) {
  
  return 0;
}

int PisteDraw_Paletti_Get(SDL_Color *&paletti)
{
  //if (FAILED(PD_lpddpal->GetEntries(0,0,255,PD_paletti_nyt))) {
  //paletti = (SDL_Color *)PD_paletti_nyt;

  return 0;
}

int PisteDraw_Paletti_Set(void)
{
  //if (FAILED(PD_lpddpal->SetEntries(0,0,255,PD_paletti_nyt))) {

  return 0;
}

void PisteDraw_Paivita_Naytto()
{
  PisteDraw_Fade_Paletti();
//  while (FAILED(PD_lpddsprimary->Flip(NULL, DDFLIP_WAIT)));
  SDL_Flip(PD_buffers[PD_TAUSTABUFFER]);
}

int PisteDraw_Font_Uusi(int buffer_index, int buffer_x, int buffer_y, int leveys, int korkeus, int lkm)
{
  int index = 0;
  bool found = false;

  while (index<MAX_FONTTEJA && !found)
  {
    if (PD_fontit[index] == NULL)
    {
      PD_fontit[index] = new PisteFont(korkeus,leveys,lkm);
      printf("PisteDraw_Font_Uusi %i bx:%i by:%i w:%i h:%i lkm:%i\n",
          buffer_index,buffer_x,buffer_y,leveys,korkeus,lkm);
      
      UCHAR *buffer = NULL;
      DWORD ruudun_leveys;      
      /*
      PisteDraw_Piirto_Aloita(buffer_index, *&buffer, (DWORD &) ruudun_leveys);
      PD_fontit[index]->Get_bitmap(buffer_x,buffer_y,ruudun_leveys,buffer);
      PisteDraw_Piirto_Lopeta(buffer_index);
      */
      PD_fontit[index]->Get_bitmap(buffer_x,buffer_y,ruudun_leveys,buffer_index);
      found = true;
      
      if (korkeus > PD_font_korkein)
      {
        PD_font_korkein = korkeus;
        PisteDraw_Buffer_Tuhoa(PD_fontbuffer);
        PD_fontbuffer = PisteDraw_Buffer_Uusi(PD_ruudun_leveys,korkeus,true,255);
      }

    }
    else
      index++;
  }
  
  if (!found)
  {
    strcpy(virhe,"PisteEngine has run out of free fonts!");
    PisteLog_Kirjoita("[Error] Piste Draw: PisteDraw has run out of free fonts!\n");
    index = PD_VIRHE;
  }

  return index;
}

int PisteDraw_Font_Uusi(char *polku, char *tiedosto)
{
  int index = 0;
  bool found = false;

  PD_Loki_Kirjoita("PisteDraw_Font_Uusi\n");
  PD_Loki_Kirjoita(polku);
  PD_Loki_Kirjoita(tiedosto);

  PisteLog_Kirjoita("- PisteDraw: Loading a font from file: ");
  PisteLog_Kirjoita(polku);PisteLog_Kirjoita(tiedosto);
  PisteLog_Kirjoita("\n");

  while (index<MAX_FONTTEJA && !found)
  {
    if (PD_fontit[index] == NULL)
    {
      PD_Loki_Kirjoita("Tyhja fontti\n");
      
      PD_fontit[index] = new PisteFont();
      
      if (PD_fontit[index]->LataaTiedostosta(polku,tiedosto) == -1){
        strcpy(virhe,"PisteEngine can't load a font from file!");
        printf("fon? %s %s\n", polku, tiedosto);
        PisteLog_Kirjoita("[Error] Piste Draw: Loading font failed!\n");
        
        PD_Loki_Kirjoita("Lataus epäonnistui\n");

        return PD_VIRHE;
      }

      PD_Loki_Kirjoita("Lataus onnistui\n");

      found = true;
      
      if (PD_fontit[index]->Korkeus() > PD_font_korkein)
      {
        PD_font_korkein = PD_fontit[index]->Korkeus();
        PisteDraw_Buffer_Tuhoa(PD_fontbuffer);
        PD_fontbuffer = PisteDraw_Buffer_Uusi(PD_ruudun_leveys,PD_font_korkein,true,255);
      }

    }
    else
      index++;
  }
  
  if (!found)
  {
    strcpy(virhe,"PisteEngine has run out of free fonts!");
    PisteLog_Kirjoita("[Error] Piste Draw: PisteDraw has run out of free fonts!\n");
    index = PD_VIRHE;
  }

  PD_Loki_Kirjoita("Kaikki ok.\n");

  return index;
}

int PisteDraw_Font_Kirjoita(int font_index, char *merkkijono, int buffer_index, int x, int y)
{
  int tekstin_leveys;

//  PisteDraw_Buffer_Flip_Nopea(PD_fontit[font_index]->font_buffer, PD_TAUSTABUFFER, x,y);
  tekstin_leveys = PD_fontit[font_index]->Piirra_merkkijono(merkkijono, x, y, buffer_index);

  return tekstin_leveys;
}

int PisteDraw_Font_Kirjoita_Lapinakyva(int font_index, char *merkkijono, int buffer_index, int x, int y, int pros)
{
  UCHAR *buffer = NULL;
  DWORD ruudun_leveys;
  int tekstin_leveys;

  x += vasen_marginaali;
  y += yla_marginaali;

  if (PisteDraw_Piirto_Aloita(buffer_index, *&buffer, (DWORD &) ruudun_leveys) == PD_VIRHE)
    return PD_VIRHE;

  tekstin_leveys = PD_fontit[font_index]->Piirra_merkkijono_lapinakyva(x, y, ruudun_leveys, PD_buffers[buffer_index]->clip_rect, merkkijono, buffer, false, pros);

  if (PisteDraw_Piirto_Lopeta(buffer_index) == PD_VIRHE)
    return PD_VIRHE;

  return tekstin_leveys;
}

char *PisteDraw_Virheilmoitus()
{
  return virhe;
}

/*
int PisteDraw_Font_Kirjoita_Lapinakyva(int font_index, char *merkkijono, int buffer_index, int x, int y, int pros)
{
  UCHAR *buffer = NULL;
  DWORD ruudun_leveys;
  int tekstin_leveys;

  if (PisteDraw_Buffer_Tayta(PD_fontbuffer,255) == PD_VIRHE)
    return PD_VIRHE;

  PisteDraw_Buffer_Flip_Nopea(buffer_index, PD_fontbuffer,0,0,x,y,x+640,y+PD_font_korkein);

  if (PisteDraw_Piirto_Aloita(PD_fontbuffer, *&buffer, (DWORD &) ruudun_leveys) == PD_VIRHE)
    return PD_VIRHE;

  tekstin_leveys = PD_fontit[font_index]->Piirra_merkkijono_lapinakyva(x, y, ruudun_leveys, merkkijono, buffer, false, pros);

  if (PisteDraw_Piirto_Lopeta(PD_fontbuffer) == PD_VIRHE)
    return PD_VIRHE;
  
  if (PisteDraw_Buffer_Flip_Nopea(PD_fontbuffer, buffer_index, x, y, 0, 0, tekstin_leveys,
                PD_fontit[font_index]->Korkeus()) == PD_VIRHE)
    return PD_VIRHE;

  return tekstin_leveys;
}*/
/*
int PisteDraw_Flip_Bitmap(UCHAR *image, int bytes_per_line, int height)
{
  UCHAR *buffer; 
  int index;     

  if (!(buffer = (UCHAR *)malloc(bytes_per_line*height)))
    return 1;

  memcpy(buffer,image,bytes_per_line*height);

  for (index=0; index < height; index++)
     memcpy(&image[((height-1) - index)*bytes_per_line],
         &buffer[index*bytes_per_line], bytes_per_line);

  free(buffer);

  return 0;
} 
*/
