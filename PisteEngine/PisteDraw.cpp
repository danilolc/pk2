/* 
PisteEngine - PisteDraw 1.5	
16.06.2002	Janne Kivilahti / Piste Gamez

Versio 1.2
----------

	Maksimimäärä buffereita korotettu 200 -> 2000.

	PisteDraw_Buffer_Tayta(int i, int vasen, int yla, int oikea, int ala, UCHAR color);
	Klipperi lisätty.

	PisteDraw_Font_Kirjoita_Lapinakyva(int font_index, char *merkkijono, int buffer_index, int x, int y, int pros)
	Lisätty. Toimii vain 256-värisessä, 32:n osiin pilkotussa paletissa.

	PisteDraw_Piirto_Lopeta(int i);
	Lisätty kuormitettu versio, jossa on int muotoinen parametri. 
	Mahdollistaa useiden pintojen lukitsemisen yhtäaikaa. Vanha versio toimii myös.

	PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y);
	Lisätty kuormitettu versio. 
	Mahdollistaa bittikartan piirron, joka on peilattu sivusuunnassa ja/tai ylösalaisin.

Versio 1.3
----------

	char *PisteDraw_Virheilmoitus() lisätty. Helpottaa virheen etsintää.

	PisteDraw_Lataa_Kuva(int index, char *filename, bool lataa_paletti)
	Eri kokoisten kuvien lataus on nyt mahdollista.

	int PisteDraw_Paletti_Pyorita(int eka_vari, int vika_vari)
	mahdollistaa värien pyörittämisen

Versio 1.4
----------

	PCX-kuvien lataus PisteDraw_Lataa_Kuva() -aliohjelmalla. Aliohjelma tutkii onko tiedoston
	pääte bmp vai pcx ja lataa kuvan sen perusteella. Virhettä pukkaa, jos tiedosto ei ole
	kumpaakaan tyyppiä.


	DWORD PisteDraw_Videomuistia_Max()

    PisteDraw_Aseta_Marginaali_Vasen(int vasen);
	PisteDraw_Aseta_Marginaali_Yla(int yla);

Versio 1.5
----------

	Pintojen palautus.

*/

/* INCLUDES ----------------------------------------------------------------------------------*/

#include "PisteDraw.h"
#include "PisteFont.h"
#include "PisteLog.h"
#include <io.h>
#include <stdio.h>
#include <fstream.h>
#include <mmsystem.h>
#include <malloc.h>
#include <memory.h>

/* DEFINES -----------------------------------------------------------------------------------*/

#define DD_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }
#define BITMAP_ID            0x4D42

/* TYPE DEFINITIONS --------------------------------------------------------------------------*/

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

typedef struct BITMAP_FILE_TAG
{
	BITMAPFILEHEADER bitmapfileheader;  // this contains the bitmapfile header
	BITMAPINFOHEADER bitmapinfoheader;  // this is all the info including the palette
	PALETTEENTRY     paletti[256];      // we will store the palette here
	UCHAR            *buffer;           // this is a pointer to the data

} BITMAP_FILE, *BITMAP_FILE_PTR;

struct BITMAP_BUFFER
{
	LPDIRECTDRAWSURFACE4	lpdds;
	RECT					klipperi;
	bool					video_muisti;
	int						leveys;
	int						korkeus;
	bool					lukittu;
};

struct RECTA {
   DWORD left;
   DWORD top;
   DWORD right;
   DWORD bottom;
};


/* VARIABLES ---------------------------------------------------------------------------------*/

const int				MAX_BUFFEREITA	= 2000;
const int				MAX_FONTTEJA	= 10;

HWND					PD_main_window_handle	= NULL; // globally track main window
HINSTANCE				PD_hinstance_app		= NULL; // globally track hinstance
HDC						PD_global_dc			= NULL; // tracks a global dc
					

LPDIRECTDRAW4			PD_lpdd         = NULL;   // dd object
LPDIRECTDRAWSURFACE4	PD_lpddsprimary = NULL;   // dd primary surface
PALETTEENTRY			PD_paletti[256];          // color palette
PALETTEENTRY			PD_paletti_nyt[256];      // paletin talletusta varten
LPDIRECTDRAWPALETTE		PD_lpddpal      = NULL;   // a pointer to the created dd palette
DDSURFACEDESC2			PD_ddsd;                  // a direct draw surface description struct
DDBLTFX					PD_ddbltfx;               // used to fill
DDSCAPS2				PD_ddscaps;				  // a direct draw surface capabilities struct
BITMAP_BUFFER			PD_buffers[MAX_BUFFEREITA]; // taulukko kuvabuffereista

int						PD_ruudun_leveys;
int						PD_ruudun_korkeus;
int						PD_ruudun_bpp;
int						PD_max_varit;
int						PD_buffereita_varattu = 2;

int						vasen_marginaali = 0,
						yla_marginaali = 0;

int						PD_paletti_fade_pros	= 100;
int						PD_paletti_fade_laskuri	= 0;

PisteFont				*PD_fontit[MAX_FONTTEJA];
int						PD_fontbuffer;
int						PD_font_korkein = 0;

char					virhe[60];

bool					PD_unload = true;

/* PROTOTYPES --------------------------------------------------------------------------------*/

LPDIRECTDRAWSURFACE4 PisteDraw_Create_Surface(int width, int height, int mem_flags, UCHAR color);

int PisteDraw_Load_Bitmap(BITMAP_FILE_PTR bitmap, char *filename);

int PisteDraw_Flip_Bitmap(UCHAR *image, int bytes_per_line, int height);

int PisteDraw_Unload_Bitmap_File(BITMAP_FILE_PTR bitmap);

int PisteDraw_Lataa_PCX(char *filename, int buffer_index, bool lataa_paletti);

int PisteDraw_Lataa_PCX_Paletti(char *filename);

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

		fwrite(mjono,		sizeof(CHAR),	strlen(mjono),  tiedosto);

		fclose(tiedosto);

	return(0);
}


int	PisteDraw_Alusta(HWND &main_window_handle, HINSTANCE &hinstance_app, 
					 int leveys, int korkeus, int bpp,
					 int max_colors)
{

	if (PD_unload) {

		strcpy(virhe,"Uh, oh, I think we have a bug...");

		PD_main_window_handle	= (HWND &)main_window_handle;
		PD_hinstance_app		= (HINSTANCE &)hinstance_app;
		PD_ruudun_leveys		= leveys;
		PD_ruudun_korkeus		= korkeus;
		PD_ruudun_bpp			= bpp;
		PD_max_varit			= max_colors;
		
		LPDIRECTDRAW temp = NULL;				 // väliaikainen rajapinta jolla haetaan uusin versio
		int i;

		if (FAILED(DirectDrawCreate(NULL, &temp, NULL)))	// luo rajapintaosoitin versioon 1.0
		{
			strcpy(virhe,"Cannot initialize DirectDraw!");
			PisteLog_Kirjoita("[Error] Piste Draw: Cannot initialize DirectDraw! \n");
			return PD_VIRHE;
		}

		if (FAILED(temp->QueryInterface(IID_IDirectDraw4,(LPVOID *)&PD_lpdd))) // osoitin v 4.0
		{
			strcpy(virhe,"Cannot initialize DirectDraw4!");
			PisteLog_Kirjoita("[Error] Piste Draw: Cannot initialize DirectDraw4! \n");
			return PD_VIRHE;
		}

		temp->Release();	// tuhotaan väliaikainen rajapinta
		temp = NULL;
		
		if (FAILED(PD_lpdd->SetCooperativeLevel(PD_main_window_handle, // Yhteistyö Windowsin kanssa..
											  DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX |
											  DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)))
		{
			strcpy(virhe,"Failed to cooperate with Windows!");
			PisteLog_Kirjoita("[Error] Piste Draw: Failed to cooperate with Windows! \n");
			return PD_VIRHE;
		}
		
		if (FAILED(PD_lpdd->SetDisplayMode(PD_ruudun_leveys, PD_ruudun_korkeus, PD_ruudun_bpp,0,0)))
		{
			strcpy(virhe,"Unable to change video mode!");
			PisteLog_Kirjoita("[Error] Piste Draw: Unable to change video mode! \n");
			return PD_VIRHE;
		}

		DD_INIT_STRUCT(PD_ddsd);

		PD_ddsd.dwFlags				=	DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		PD_ddsd.dwBackBufferCount	=	2;				//Kolmoispuskurointi = primary + 2 taustapuskuria
		PD_ddsd.ddsCaps.dwCaps		=	DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
		
		if (FAILED(PD_lpdd->CreateSurface(&PD_ddsd, &PD_lpddsprimary, NULL)))
		{
			strcpy(virhe,"Cannot create primary surface!");
			PisteLog_Kirjoita("[Error] Piste Draw: Cannot create primary surface! \n");
			return PD_VIRHE;
		}
		
		PD_buffers[PD_TAUSTABUFFER].leveys		= leveys;
		PD_buffers[PD_TAUSTABUFFER].korkeus		= korkeus;
		PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,0,0,leveys,korkeus);
		PD_ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
		
		if (FAILED(PD_lpddsprimary->GetAttachedSurface(&PD_ddsd.ddsCaps, &PD_buffers[PD_TAUSTABUFFER].lpdds)))
		{
			strcpy(virhe,"Cannot attach back buffer to primary surface!");
			PisteLog_Kirjoita("[Error] Piste Draw: Cannot attach back buffer to primary surface! \n");
			return PD_VIRHE;
		}

		PD_buffers[PD_TAUSTABUFFER2].leveys		= leveys;
		PD_buffers[PD_TAUSTABUFFER2].korkeus	= korkeus;
		PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER2,0,0,leveys,korkeus);

		for (i=1;i<255;i++)							//Luodaan 8-bittinen paletti
		{
			PD_paletti[i].peRed		=	0;
			PD_paletti[i].peGreen	=	0;
			PD_paletti[i].peBlue	=	0;
			PD_paletti[i].peFlags	=	PC_NOCOLLAPSE;
		}

		if (FAILED(PD_lpdd->CreatePalette(DDPCAPS_8BIT |
										DDPCAPS_ALLOW256 |
										DDPCAPS_INITIALIZE,
										PD_paletti,
										&PD_lpddpal,
										NULL)))
		{
			PisteLog_Kirjoita("[Error] Piste Draw: Cannot create 8-bit palette! \n");
			strcpy(virhe,"Cannot create 8-bit palette!");
			return PD_VIRHE;
		}

		if (FAILED(PD_lpddsprimary->SetPalette(PD_lpddpal)))
		{
			PisteLog_Kirjoita("[Error] Piste Draw: Cannot set palette! \n");
			strcpy(virhe,"Cannot set palette!");
			return PD_VIRHE;
		}

		/*LIITETÄÄN KLIPPERI KUVAN REUNOJEN YLIKIRJOITUSTA ESTÄMÄÄN*/

		PD_fontbuffer = PisteDraw_Buffer_Uusi(PD_ruudun_leveys,10,true,255);

		for (i=2;i<MAX_BUFFEREITA;i++)		// alustetaan kuvabufferi taulukko.  
			PD_buffers[i].lpdds = NULL;				// 0 ja 1 on varattu taustapuskureille

		vasen_marginaali = 0;
		yla_marginaali = 0;
	
		PD_unload = false;
	}

	return 0;
}

int PisteDraw_Aseta_Videomode(int leveys, int korkeus, int bpp,
					 int max_colors)
{
	if (FAILED(PD_lpdd->SetDisplayMode(PD_ruudun_leveys, PD_ruudun_korkeus, PD_ruudun_bpp,0,0)))
	{
		strcpy(virhe,"Unable to change video mode!");
		PisteLog_Kirjoita("[Error] Piste Draw: Unable to change video mode! \n");
		return PD_VIRHE;
	}
	return 0;
}

int	PisteDraw_Lopeta()
{

	if (!PD_unload) {

		int i;

		for (i=0; i<MAX_FONTTEJA; i++)
		{
			if (PD_fontit[i] != NULL )
				delete PD_fontit[i];
			PD_fontit[i] = NULL;
		}

		for (i=2;i<MAX_BUFFEREITA;i++)		// 0 ja 1 on varattu taustapuskureille
			PisteDraw_Buffer_Tuhoa(i);

		if (PD_lpddpal)
		{
			PD_lpddpal->Release();
		}

		PisteDraw_Buffer_Tuhoa(PD_TAUSTABUFFER);

		PisteDraw_Buffer_Tuhoa(PD_TAUSTABUFFER2);

		if (PD_lpddsprimary)
		{
			PD_lpddsprimary->Release();
		} 

		if (PD_lpdd)
		{
			PD_lpdd->Release();
		}

		PD_unload = true;
	}

	return 0;
}

DWORD PisteDraw_Videomuistia()
{
	DDCAPS hel_caps, hal_caps;

	DD_INIT_STRUCT(hel_caps);
	DD_INIT_STRUCT(hal_caps);	
	
	if (FAILED(PD_lpdd->GetCaps(&hal_caps,&hel_caps)))	
	{
		strcpy(virhe,"Cannot aquire system information!");
		return PD_VIRHE;
	}
	
	return hal_caps.dwVidMemFree;
}

DWORD PisteDraw_Videomuistia_Max()
{
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
}

int PisteDraw_Buffer_Uusi(int leveys, int korkeus, bool video_muisti, UCHAR color)
{
	int i = 0, 
		varaus;
	bool find = false;

	DDCAPS hel_caps, hal_caps;

	DD_INIT_STRUCT(hel_caps);
	DD_INIT_STRUCT(hal_caps);

	if (FAILED(PD_lpdd->GetCaps(&hal_caps,&hel_caps)))	
	{
		strcpy(virhe,"Cannot aquire system information!");
		PisteLog_Kirjoita("[Error] Piste Draw: Cannot aquire system information to create a new buffer! \n");
		return PD_VIRHE;
	}

	if ((unsigned long)(leveys*korkeus) > hal_caps.dwVidMemFree) {		//tarkastetaan riittääkö videomuistia
		video_muisti = false;
		PisteLog_Kirjoita("[Warning] Piste Draw: Out of video memory - creating a new buffer in system memory! \n");
	}

	if (video_muisti)
	{
		varaus = DDSCAPS_VIDEOMEMORY;
	}
	else
	{
		varaus = DDSCAPS_SYSTEMMEMORY;
	}

	while ((i < MAX_BUFFEREITA) && !find)
	{
		if (PD_buffers[i].lpdds == NULL)	// Onko puskurin pinta jo varattu?
		{
			if ((PD_buffers[i].lpdds = PisteDraw_Create_Surface(leveys, korkeus, varaus, color)) != NULL)
			{
				PD_buffers[i].leveys			= leveys;
				PD_buffers[i].korkeus			= korkeus;
				PD_buffers[i].klipperi.left		= 0;
				PD_buffers[i].klipperi.top		= 0;
				PD_buffers[i].klipperi.right	= leveys;
				PD_buffers[i].klipperi.bottom	= korkeus;
				PD_buffers[i].video_muisti		= video_muisti;
				PD_buffers[i].lukittu			= false;
				find = true;
				PD_buffereita_varattu++;
			}
			else
			{
				i = PD_VIRHE;
				strcpy(virhe,"Cannot create a new DirectDraw surface!");
				PisteLog_Kirjoita("[Error] Piste Draw: Cannot create a new DirectDraw surface! \n");
			}
		}
		else
		{
			i++;
		}
	}
	
	if (!find)
	{
		strcpy(virhe,"PisteEngine has run out of buffers!");
		PisteLog_Kirjoita("[Error] Piste Draw: PisteDraw has run out of buffers! \n");
		i = PD_VIRHE;
	}

	return i;
}

bool PisteDraw_Buffer_Tuhoa(int i)
{
	if (i < 0 || i > MAX_BUFFEREITA)
		return false;

	if (PD_buffers[i].lpdds != NULL)
	{
		PD_buffers[i].lpdds->Release();
		PD_buffers[i].lpdds = NULL;
		PD_buffereita_varattu--;
	}
	else
		return false;

	return true;
}

int PisteDraw_Lataa_Kuva(int index, char *filename, bool lataa_paletti)
{
	BITMAP_FILE		bitmap;

	bool ok = false;

	char paate[4];

	for (int i=0;i<4;i++)
		paate[i] = toupper(filename[strlen(filename)-3+i]);
	paate[4] = '\0';

	if (strcmp(paate,"BMP")==0)
	{
		ok = true;
		if (PD_buffers[index].lpdds)
		{
			if (PisteDraw_Load_Bitmap(&bitmap, filename) != 0)
			{
				strcpy(virhe,"Could not load picture ");
				strcat(virhe,filename);
				strcat(virhe,"!");
				PisteLog_Kirjoita("[Error] Piste Draw: ");
				PisteLog_Kirjoita(virhe);
				PisteLog_Kirjoita("\n");
				return PD_VIRHE;
			}

			DD_INIT_STRUCT(PD_ddsd);
			
			if (FAILED(PD_buffers[index].lpdds->Lock(NULL,&PD_ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL)))
			{
				strcpy(virhe,"Could not lock surface after loading a picture!");
				PisteLog_Kirjoita("[Error] Piste Draw: Could not lock surface after loading a picture! \n");
				return PD_VIRHE;
			}

			UCHAR *back_buffer = (UCHAR *)PD_ddsd.lpSurface; 

			int x;
			int y;
			int leveys, korkeus;

			if (bitmap.bitmapinfoheader.biWidth > PD_buffers[index].leveys)
				leveys = PD_buffers[index].leveys;
			else
				leveys = bitmap.bitmapinfoheader.biWidth;

			if (bitmap.bitmapinfoheader.biHeight > PD_buffers[index].korkeus)
				korkeus = PD_buffers[index].korkeus;
			else
				korkeus = bitmap.bitmapinfoheader.biHeight;

			for (y=0; y<korkeus; y++)
			{
				for (x=0; x<leveys; x++)
				{
					back_buffer[x+y*PD_ddsd.lPitch] = bitmap.buffer[x+y*bitmap.bitmapinfoheader.biWidth];
				}
			}

			if (FAILED(PD_buffers[index].lpdds->Unlock(NULL)))
			{
				strcpy(virhe,"Could not unlock surface after loading a picture!");
				PisteLog_Kirjoita("[Error] Piste Draw: Could not unlock surface after loading a picture! \n");
				return PD_VIRHE;
			}


			PisteDraw_Unload_Bitmap_File(&bitmap);
			
			if (lataa_paletti)
			{
			
				if (FAILED(PD_lpddpal->SetEntries(0,0,PD_max_varit, bitmap.paletti)))
				{
					strcpy(virhe,"Could not set palette after loading a picture!");
					PisteLog_Kirjoita("[Error] Piste Draw: Could not set palette after loading a picture! \n");
					return PD_VIRHE;
				}

				for (int pi=0;pi<256;pi++)
				{
					PD_paletti[pi].peBlue  = bitmap.paletti[pi].peBlue;
					PD_paletti[pi].peRed   = bitmap.paletti[pi].peRed;
					PD_paletti[pi].peGreen = bitmap.paletti[pi].peGreen;
					PD_paletti[pi].peFlags = bitmap.paletti[pi].peFlags;
				}

			}

		}
		else
			return PD_VIRHE;
	}

	if (strcmp(paate,"PCX")==0)
	{
		ok = true;
		if (PisteDraw_Lataa_PCX(filename, index, lataa_paletti) == PD_VIRHE) {
			strcpy(virhe,"Could not load picture ");
			strcat(virhe,filename);
			strcat(virhe,"!");
			PisteLog_Kirjoita("[Error] Piste Draw: ");
			PisteLog_Kirjoita(virhe);
			PisteLog_Kirjoita("\n");
			return PD_VIRHE;
		}
	}

	if (!ok)
	{
		strcpy(virhe,"Could not load bitmap file of type ");
		strcat(virhe,paate);
		return PD_VIRHE;
	}

	return 0;
}

int PisteDraw_Buffer_Flip(int i)
{
	DD_INIT_STRUCT(PD_ddsd);
	DD_INIT_STRUCT(PD_ddbltfx);

	RECT dest_rect;
	dest_rect.left	 =	0;
	dest_rect.right	 =	PD_ruudun_leveys;
	dest_rect.top	 =	0;
	dest_rect.bottom =	PD_ruudun_korkeus;

	if (FAILED(PD_buffers[PD_TAUSTABUFFER].lpdds->Blt(&dest_rect, 
										PD_buffers[i].lpdds,
										&dest_rect, 
										(DDBLT_WAIT | DDBLT_KEYSRC),
										NULL)))
	{
		strcpy(virhe,"Unable to flip surface!");
		PisteLog_Kirjoita("[Error] Piste Draw: Unable to flip surface!\n");
		return PD_VIRHE;	
	}
	return 0;
}

int PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y)
{
	DD_INIT_STRUCT(PD_ddsd);				//Alustetaan DirectX pintakuvaus
	DD_INIT_STRUCT(PD_ddbltfx);				//Alustetaan Blittaus tehosteet
	int leveys;								//Apumuuttuja
	int korkeus;							//Apumuuttuja
	RECT dest_rect;							//Alue kohdebufferista josta kopioidaan lähdebufferiin
	RECT source_rect;						//Alue lähdebufferista josta kopioidaan kohdebufferiin
	RECT *klipperi_kohde;					//Pointteri kohdebufferin leikkuriin 
	RECT *klipperi_lahde;					//Pointteri lähdebufferin leikkuriin

	x += vasen_marginaali;
	y += yla_marginaali;

	if (peilaa_x)
		PD_ddbltfx.dwDDFX = DDBLTFX_MIRRORLEFTRIGHT;

	if (peilaa_y)
		PD_ddbltfx.dwDDFX = DDBLTFX_MIRRORUPDOWN;

	if (peilaa_x && peilaa_y)
		PD_ddbltfx.dwDDFX = DDBLTFX_MIRRORLEFTRIGHT | DDBLTFX_MIRRORUPDOWN;

	klipperi_kohde = &PD_buffers[kohde_index].klipperi;	
	klipperi_lahde = &PD_buffers[lahde_index].klipperi;

	int vasen	= klipperi_lahde->left;
	int oikea	= klipperi_lahde->right;
	int yla		= klipperi_lahde->top;
	int ala		= klipperi_lahde->bottom;

	leveys  = oikea - vasen;	// lasketaan apuja
	korkeus = ala - yla; 

	if (x+leveys > klipperi_kohde->right)			//Varmistetaan että ei piirretä kohdebufferin
	{												//rajojen ulkopuolelle
		oikea = vasen + klipperi_kohde->right - x;
	}

	if (x < klipperi_kohde->left)
	{
		vasen = vasen - x + klipperi_kohde->left;
	}

	if (y+korkeus > klipperi_kohde->bottom)
	{
		ala = yla + klipperi_kohde->bottom - y;
	}

	if (y < klipperi_kohde->top)
	{
		yla = yla - y + klipperi_kohde->top;
	}

	if (oikea>vasen && ala>yla)
	{
				
		dest_rect.left		=	vasen+x;
		dest_rect.right		=	oikea+x;
		dest_rect.top		=	yla+y;
		dest_rect.bottom	=	ala+y;		

		source_rect.left	=	vasen;
		source_rect.right	=	oikea;
		source_rect.top		=	yla;
		source_rect.bottom	=	ala;

		if (peilaa_x)
		{
			if (x<klipperi_kohde->left)
			{
				source_rect.left  = klipperi_lahde->left;
				source_rect.right = klipperi_lahde->right - (vasen - klipperi_lahde->left);		
			}
			else
			{
				source_rect.left  = klipperi_lahde->left + (klipperi_lahde->right - oikea);
				source_rect.right = klipperi_lahde->right;
			}
		}

		if (peilaa_y)
		{
			if (y < klipperi_kohde->top)
			{
				source_rect.top    = klipperi_lahde->top;
				source_rect.bottom = klipperi_lahde->bottom - (yla - klipperi_lahde->top);		
			}
			else
			{
				source_rect.top    = klipperi_lahde->top + (klipperi_lahde->bottom - ala);
				source_rect.bottom = klipperi_lahde->bottom;
			}		
		}

		if (FAILED(PD_buffers[kohde_index].lpdds->Blt(&dest_rect, PD_buffers[lahde_index].lpdds, &source_rect,
			(DDBLT_WAIT | DDBLT_KEYSRC | DDBLT_DDFX), &PD_ddbltfx))){
			PisteLog_Kirjoita("[Error] Piste Draw: Unable to flip surface!\n");
			return PD_VIRHE; 
		}
	}

	return 0;
}

int PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y, 
							    int vasen, int yla, int oikea, int ala)
{
	DD_INIT_STRUCT(PD_ddsd);				//Alustetaan DirectX pintakuvaus
	DD_INIT_STRUCT(PD_ddbltfx);				//Alustetaan Blittaus tehosteet
	int leveys;								//Apumuuttuja
	int korkeus;							//Apumuuttuja
	RECT source_rect;						//Alue lähdebufferista josta kopioidaan kohdebufferiin
	RECT *klipperi_kohde;					//Pointteri kohdebufferin leikkuriin 
	RECT *klipperi_lahde;

	x += vasen_marginaali;
	y += yla_marginaali;

	klipperi_kohde = &PD_buffers[kohde_index].klipperi;	
	klipperi_lahde = &PD_buffers[lahde_index].klipperi;

	if (vasen < PD_buffers[lahde_index].klipperi.left)	//Tarkistetaan että dataa ei oteta lähdebufferin
		vasen = PD_buffers[lahde_index].klipperi.left;	//rajojen ulkopuolelta

	if (oikea > PD_buffers[lahde_index].klipperi.right)
		oikea = PD_buffers[lahde_index].klipperi.right;

	if (yla < PD_buffers[lahde_index].klipperi.top)
		yla = PD_buffers[lahde_index].klipperi.top;

	if (ala > PD_buffers[lahde_index].klipperi.bottom)
		ala = PD_buffers[lahde_index].klipperi.bottom;

	leveys  = oikea - vasen;	// lasketaan apuja
	korkeus = ala - yla; 

	if (x+leveys > klipperi_kohde->right)			//Varmistetaan että ei piirretä kohdebufferin
	{												//rajojen ulkopuolelle
		oikea = vasen + klipperi_kohde->right - x;
	}

	if (x < klipperi_kohde->left)
	{
		vasen = vasen - x + klipperi_kohde->left;
		x = klipperi_kohde->left;
	}

	if (y+korkeus > klipperi_kohde->bottom)
	{
		ala = yla + klipperi_kohde->bottom - y;
	}

	if (y < klipperi_kohde->top)
	{
		yla = yla - y + klipperi_kohde->top;
		y = klipperi_kohde->top;
	}

	if (oikea>vasen && ala>yla)
	{
		source_rect.left	=	vasen;
		source_rect.right	=	oikea;
		source_rect.top		=	yla;
		source_rect.bottom	=	ala;

		if (FAILED(PD_buffers[kohde_index].lpdds->BltFast(x, y, PD_buffers[lahde_index].lpdds, &source_rect,
			(DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))) {
			PisteLog_Kirjoita("[Error] Piste Draw: Unable to flip surface!\n");
			return PD_VIRHE; // blitataan kaikki paitsi läpinäkyvyysväri
		}
	}

	return 0;
}

int PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y)
{
	DD_INIT_STRUCT(PD_ddsd);				//Alustetaan DirectX pintakuvaus
	DD_INIT_STRUCT(PD_ddbltfx);				//Alustetaan Blittaus tehosteet
	int leveys;								//Apumuuttuja
	int korkeus;							//Apumuuttuja
	RECT source_rect;						//Alue lähdebufferista josta kopioidaan kohdebufferiin
	RECT *klipperi_kohde;					//Pointteri kohdebufferin leikkuriin 
	RECT *klipperi_lahde;					//Pointteri lähdebufferin leikkuriin

	x += vasen_marginaali;
	y += yla_marginaali;

	klipperi_kohde = &PD_buffers[kohde_index].klipperi;	
	klipperi_lahde = &PD_buffers[lahde_index].klipperi;

	int vasen	= klipperi_lahde->left;
	int oikea	= klipperi_lahde->right;
	int yla		= klipperi_lahde->top;
	int ala		= klipperi_lahde->bottom;

	leveys  = oikea - vasen;	// lasketaan apuja
	korkeus = ala - yla; 

	if (x+leveys > klipperi_kohde->right)			//Varmistetaan että ei piirretä kohdebufferin
	{												//rajojen ulkopuolelle
		oikea = vasen + klipperi_kohde->right - x;
	}

	if (x < klipperi_kohde->left)
	{
		vasen = vasen - x + klipperi_kohde->left;
		x = klipperi_kohde->left;
	}

	if (y+korkeus > klipperi_kohde->bottom)
	{
		ala = yla + klipperi_kohde->bottom - y;
	}

	if (y < klipperi_kohde->top)
	{
		yla = yla - y + klipperi_kohde->top;
		y = klipperi_kohde->top;
	}

	if (oikea>vasen && ala>yla)
	{
		source_rect.left	=	vasen;
		source_rect.right	=	oikea;
		source_rect.top		=	yla;
		source_rect.bottom	=	ala;

		if (FAILED(PD_buffers[kohde_index].lpdds->BltFast(x, y, PD_buffers[lahde_index].lpdds, &source_rect,
			(DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))) {			
			PisteLog_Kirjoita("[Error] Piste Draw: BlitFast failed!\n");
			return PD_VIRHE; // blitataan kaikki paitsi läpinäkyvyysväri
		}
	}

	return 0;
}

int PisteDraw_Piirto_Aloita(int i, UCHAR *&back_buffer, DWORD &lPitch)
{
	if (PD_buffers[i].lukittu)
		if (PisteDraw_Piirto_Lopeta(i)==PD_VIRHE) {
			PisteLog_Kirjoita("[Error] Piste Draw: Can't start free draw: Surface unlock failed!\n");
			return PD_VIRHE;
		}
	
	DD_INIT_STRUCT(PD_ddsd); 
	//PD_piirto_buffer_index = i;

	if (FAILED(PD_buffers[i].lpdds->Lock(NULL, &PD_ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
	{
		PisteLog_Kirjoita("[Error] Piste Draw: Can't start free draw: Surface lock failed!\n");
		return PD_VIRHE;
	}	

	back_buffer = (UCHAR *)PD_ddsd.lpSurface;
	lPitch		= (DWORD &)PD_ddsd.lPitch;
		
	PD_buffers[i].lukittu = true;
	
	return 0;
}

int PisteDraw_Piirto_Lopeta(void)
{
	int i = 0;

	while (i<MAX_BUFFEREITA /*PD_buffereita_varattu*/)
	{
		if (PD_buffers[i].lpdds != NULL)
			if (PD_buffers[i].lukittu)
			{	
				if (FAILED(PD_buffers[i].lpdds->Unlock(NULL)))
				{
					PisteLog_Kirjoita("[Error] Piste Draw: Free draw - surface unlock failed!\n");
					return PD_VIRHE;
				}

				PD_buffers[i].lukittu = false;
			}
	}
	return 0;
}

int PisteDraw_Piirto_Lopeta(int i)
{
	if (PD_buffers[i].lukittu)
	{	
		if (FAILED(PD_buffers[i].lpdds->Unlock(NULL)))
		{
			PisteLog_Kirjoita("[Error] Piste Draw: Free draw - surface unlock failed!\n");
			return PD_VIRHE;
		}

		PD_buffers[i].lukittu = false;
	}
	return 0;
}

int PisteDraw_Buffer_Tayta(int i, UCHAR color)
{
	DD_INIT_STRUCT(PD_ddbltfx);
	PD_ddbltfx.dwFillColor = color;	
	IDirectDrawSurface4 *buffer;

	if (i >= MAX_BUFFEREITA)
		return PD_VIRHE;

	if (PD_buffers[i].lpdds == NULL)
		return PD_VIRHE;

	buffer = PD_buffers[i].lpdds;
	
	RECT dest_rect;
	dest_rect.left	 =	0;
	dest_rect.right	 =	PD_buffers[i].leveys;
	dest_rect.top	 =	0;
	dest_rect.bottom =	PD_buffers[i].korkeus;	

	if (FAILED(buffer->Blt(&dest_rect, 
					 	   NULL, 
						   NULL, 
						   DDBLT_COLORFILL | DDBLT_WAIT, 
						   &PD_ddbltfx))) {
		PisteLog_Kirjoita("[Error] Piste Draw: Buffer fill failed!\n");
		return PD_VIRHE;
	}

	return 0;
}

int PisteDraw_Buffer_Tayta(int i, int vasen, int yla, int oikea, int ala, UCHAR color)
{

	DD_INIT_STRUCT(PD_ddbltfx);
	PD_ddbltfx.dwFillColor = color;	
	IDirectDrawSurface4 *buffer;

	if (i >= MAX_BUFFEREITA)
		return PD_VIRHE;

	if (PD_buffers[i].lpdds == NULL)
		return PD_VIRHE;

	buffer = PD_buffers[i].lpdds;

	vasen += vasen_marginaali;
	oikea += vasen_marginaali;
	yla += yla_marginaali;
	ala += yla_marginaali;

	RECT dest_rect;

	dest_rect.left	 =	(long)vasen;
	dest_rect.right	 =	(long)oikea;
	dest_rect.top	 =	(long)yla;
	dest_rect.bottom =	(long)ala;
	
	if (dest_rect.left < PD_buffers[i].klipperi.left)
		dest_rect.left = PD_buffers[i].klipperi.left;

	if (dest_rect.right > PD_buffers[i].klipperi.right)
		dest_rect.right = PD_buffers[i].klipperi.right;

	if (dest_rect.top < PD_buffers[i].klipperi.top)
		dest_rect.top = PD_buffers[i].klipperi.top;

	if (dest_rect.bottom > PD_buffers[i].klipperi.bottom)
		dest_rect.bottom = PD_buffers[i].klipperi.bottom;

	if (FAILED(buffer->Blt(&dest_rect, 
					  	   NULL, 
						   NULL, 
						   DDBLT_COLORFILL | DDBLT_WAIT, 
						   &PD_ddbltfx))) {
		PisteLog_Kirjoita("[Error] Piste Draw: Buffer fill area failed!\n");
		return PD_VIRHE;
	}

	return 0;
}

void PisteDraw_Aseta_Klipperi(int i, int vasen, int yla, int oikea, int ala)
{
	BITMAP_BUFFER *buffer;
	
	buffer = &PD_buffers[i];
	
	buffer->klipperi.left	= vasen;	
	buffer->klipperi.right	= oikea;
	buffer->klipperi.top	= yla;
	buffer->klipperi.bottom	= ala;	
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
	
	if (FAILED(PD_lpddpal->GetEntries(0,0,255,PD_paletti_nyt))) {
		PisteLog_Kirjoita("[Error] Piste Draw: Palette get entries failed!\n");
		return PD_VIRHE;
	}

	for (i=0;i<255;i++)
	{
		PD_paletti_nyt[i].peRed   = (PD_paletti[i].peRed   * pros / 100);
		PD_paletti_nyt[i].peGreen = (PD_paletti[i].peGreen * pros / 100);
		PD_paletti_nyt[i].peBlue  = (PD_paletti[i].peBlue  * pros / 100);
	}

	if (FAILED(PD_lpddpal->SetEntries(0,0,255,PD_paletti_nyt))) {
		PisteLog_Kirjoita("[Error] Piste Draw: Palette set entries failed!\n");
		return PD_VIRHE;
	}
	
	return 0;
}

int PisteDraw_Paletti_Pyorita(UCHAR eka_vari, UCHAR vika_vari)
{
	UCHAR i;
	
	if (FAILED(PD_lpddpal->GetEntries(0,0,255,PD_paletti_nyt))) {
		PisteLog_Kirjoita("[Error] Piste Draw: Palette get entries failed!\n");		
		return PD_VIRHE;
	}

	PALETTEENTRY temp_vari;
		
	temp_vari.peRed   = PD_paletti_nyt[vika_vari].peRed;
	temp_vari.peGreen = PD_paletti_nyt[vika_vari].peGreen;
	temp_vari.peBlue  = PD_paletti_nyt[vika_vari].peBlue;

	for (i=vika_vari;i>eka_vari;i--)
	{
		PD_paletti_nyt[i].peRed   = PD_paletti_nyt[i-1].peRed;
		PD_paletti_nyt[i].peGreen = PD_paletti_nyt[i-1].peGreen;
		PD_paletti_nyt[i].peBlue  = PD_paletti_nyt[i-1].peBlue;
	}

	PD_paletti_nyt[eka_vari].peRed   = temp_vari.peRed;
	PD_paletti_nyt[eka_vari].peGreen = temp_vari.peGreen;
	PD_paletti_nyt[eka_vari].peBlue  = temp_vari.peBlue;

	if (FAILED(PD_lpddpal->SetEntries(0,0,255,PD_paletti_nyt))) {
		PisteLog_Kirjoita("[Error] Piste Draw: Palette set entries failed!\n");	
		return PD_VIRHE;	
	}
	
	return 0;
}
/*

int PisteDraw_Paletti_Pyorita(UCHAR eka_vari, UCHAR vika_vari)
{
	UCHAR i;
	
	if (FAILED(PD_lpddpal->GetEntries(0,0,255,PD_paletti_nyt)))
		return PD_VIRHE;

	PALETTEENTRY temp_vari;
		
	temp_vari.peRed   = PD_paletti_nyt[eka_vari].peRed;
	temp_vari.peGreen = PD_paletti_nyt[eka_vari].peGreen;
	temp_vari.peBlue  = PD_paletti_nyt[eka_vari].peBlue;

	for (i=eka_vari;i<vika_vari;i++)
	{
		PD_paletti_nyt[i].peRed   = PD_paletti_nyt[i+1].peRed;
		PD_paletti_nyt[i].peGreen = PD_paletti_nyt[i+1].peGreen;
		PD_paletti_nyt[i].peBlue  = PD_paletti_nyt[i+1].peBlue;
	}

	PD_paletti_nyt[vika_vari].peRed   = temp_vari.peRed;
	PD_paletti_nyt[vika_vari].peGreen = temp_vari.peGreen;
	PD_paletti_nyt[vika_vari].peBlue  = temp_vari.peBlue;

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
	PD_paletti_fade_pros    =  100;
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
	if (FAILED(PD_lpddpal->SetEntries(0,0,255,PD_paletti))) {
		PisteLog_Kirjoita("[Error] Piste Draw: Palette reset failed!\n");
		return PD_VIRHE;
	}
	
	return 0;
}

int PisteDraw_Paletti_Get(PALETTEENTRY *&paletti)
{
	if (FAILED(PD_lpddpal->GetEntries(0,0,255,PD_paletti_nyt))) {
		PisteLog_Kirjoita("[Error] Piste Draw: Palette get failed!\n");
		return PD_VIRHE;
	}

	paletti = (PALETTEENTRY *)PD_paletti_nyt;

	return 0;
}

int PisteDraw_Paletti_Set(void)
{
	if (FAILED(PD_lpddpal->SetEntries(0,0,255,PD_paletti_nyt))) {
		PisteLog_Kirjoita("[Error] Piste Draw: Palette set failed!\n");
		return PD_VIRHE;	
	}

	return 0;
}

void PisteDraw_Paivita_Naytto()
{
	PisteDraw_Fade_Paletti();
	while (FAILED(PD_lpddsprimary->Flip(NULL, DDFLIP_WAIT)));
}

int PisteDraw_Palauta_Pinnat()
{
	if (FAILED(PD_lpdd->RestoreAllSurfaces()))	
		return PD_VIRHE;

	return 0;
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
			
			UCHAR *buffer = NULL;
			DWORD ruudun_leveys;			
			
			PisteDraw_Piirto_Aloita(buffer_index, *&buffer, (DWORD &) ruudun_leveys);
			PD_fontit[index]->Get_bitmap(buffer_x,buffer_y,ruudun_leveys,buffer);
			PisteDraw_Piirto_Lopeta(buffer_index);
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

	tekstin_leveys = PD_fontit[font_index]->Piirra_merkkijono_lapinakyva(x, y, ruudun_leveys, PD_buffers[buffer_index].klipperi, merkkijono, buffer, false, pros);

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
///////// BMP-KUVATIEDOSTON LATAUSRUTIINIT //////////////////////////

int PisteDraw_Load_Bitmap(BITMAP_FILE_PTR bitmap, char *filename)
{
	int file_handle,  
		index;				

	UCHAR   *temp_buffer = NULL; 
	OFSTRUCT file_data; 

	if ((file_handle = OpenFile(filename,&file_data,OF_READ)) == -1)
		return 1; 

	_lread(file_handle, &bitmap->bitmapfileheader,sizeof(BITMAPFILEHEADER));
	
	if (bitmap->bitmapfileheader.bfType!=BITMAP_ID)
	{
		_lclose(file_handle);
		return 2;
	}

	_lread(file_handle, &bitmap->bitmapinfoheader,sizeof(BITMAPINFOHEADER));

	if (bitmap->bitmapinfoheader.biBitCount == 8)
	{
		_lread(file_handle, &bitmap->paletti, PD_max_varit*sizeof(PALETTEENTRY));

		for (index=0; index < PD_max_varit; index++)
		{
			// reverse the red and green fields
			int temp_color                = bitmap->paletti[index].peRed;
			bitmap->paletti[index].peRed  = bitmap->paletti[index].peBlue;
			bitmap->paletti[index].peBlue = temp_color;
       
			bitmap->paletti[index].peFlags = PC_NOCOLLAPSE;
		} 

    } 

	_lseek(file_handle,-(int)(bitmap->bitmapinfoheader.biSizeImage),SEEK_END);

	if (bitmap->bitmapinfoheader.biBitCount==8 || bitmap->bitmapinfoheader.biBitCount==16 || 
		bitmap->bitmapinfoheader.biBitCount==24)
	{

		if (!(bitmap->buffer = (UCHAR *)malloc(bitmap->bitmapinfoheader.biSizeImage)))
		{
			_lclose(file_handle);

			return 3;
		} 

		_lread(file_handle,bitmap->buffer,bitmap->bitmapinfoheader.biSizeImage);

	}
	else
		return 4;

	_lclose(file_handle);

	PisteDraw_Flip_Bitmap(bitmap->buffer, 
				bitmap->bitmapinfoheader.biWidth*(bitmap->bitmapinfoheader.biBitCount/8), 
				bitmap->bitmapinfoheader.biHeight);
	return 0;
} 


int PisteDraw_Unload_Bitmap_File(BITMAP_FILE_PTR bitmap)
{
	if (bitmap->buffer)
	{
		free(bitmap->buffer);

		bitmap->buffer = NULL;
	} 
	return 0;
} 


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

// DirectDraw pinnan luonti

LPDIRECTDRAWSURFACE4 PisteDraw_Create_Surface(int width, int height, int mem_flags, UCHAR color)
{
	LPDIRECTDRAWSURFACE4	lpdds;

	DD_INIT_STRUCT(PD_ddsd);

	PD_ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

	PD_ddsd.dwWidth		=	width;
	PD_ddsd.dwHeight	=	height;

	PD_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | mem_flags;

	if (FAILED(PD_lpdd->CreateSurface(&PD_ddsd, &lpdds, NULL)))
		return(NULL);

	DDCOLORKEY color_key;
	color_key.dwColorSpaceLowValue	= color;
	color_key.dwColorSpaceHighValue = color;
	
	if(FAILED(lpdds->SetColorKey(DDCKEY_SRCBLT, &color_key)))
		return(NULL);

	return(lpdds);
}

// PCX-Kuvan lataus ///////////////////////////////////////////////////////////7

int PisteDraw_Lataa_PCX(char *filename, int buffer_index, bool lataa_paletti) 
{
    int xsize, ysize, tavu1, tavu2, position=0;
    FILE *handle=fopen(filename, "rb");

    if(handle==NULL) 
	{
		return PD_VIRHE; 
    }

	UCHAR *buffer = NULL;
	DWORD ruudun_leveys;

	if (PisteDraw_Piirto_Aloita(buffer_index, *&buffer, (DWORD &) ruudun_leveys) == PD_VIRHE)
		return PD_VIRHE;    

	fseek(handle, 8, SEEK_SET);
    
	xsize=fgetc(handle)+(fgetc(handle)<<8)+1;
    
	ysize=fgetc(handle)+(fgetc(handle)<<8)+1;
    
	fseek(handle, 128, SEEK_SET);
    
	while(position<xsize*ysize) 
	{
        tavu1=fgetc(handle);
        
		if(tavu1>192) 
		{
            tavu2=fgetc(handle);
        
			for(; tavu1>192; tavu1--)
                buffer[position++]=tavu2;

        } 
		else 
			buffer[position++]=tavu1;
    }
    fclose(handle);

	if (PisteDraw_Piirto_Lopeta(buffer_index) == PD_VIRHE)
		return PD_VIRHE;

	if (lataa_paletti)
		if (PisteDraw_Lataa_PCX_Paletti(filename) == PD_VIRHE)
			return PD_VIRHE;

	return 0;
}

int PisteDraw_Lataa_PCX_Paletti(char *filename) 
{
    FILE *handle=fopen(filename, "rb");
    char paletti[3*256];
	int c;

    if(handle==NULL) 
	{
		return PD_VIRHE;
    }

    fseek(handle,-768,SEEK_END);
    
	for(c=0; c<256*3; c++)
        paletti[c] =fgetc(handle)/4;
    
	fclose(handle);

	return 0;
}
