/* 
PisteEngine - PisteFont 1.1	
09.12.2001	Janne Kivilahti / Piste Gamez

Muutokset:
11.04.2004  Fontin parametrien lataus tiedostosta - LataaTiedostosta(char *file)
*/

/* INCLUDES ----------------------------------------------------------------------------------*/

#include <windows.h> 
#include "PisteFont.h"
#include <windowsx.h> 
#include <mmsystem.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <io.h>
#include <fstream.h>
#include <iostream.h>
#include <string.h>
/* DEFINES -----------------------------------------------------------------------------------*/

/* TYPE DEFINITIONS --------------------------------------------------------------------------*/

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

/* VARIABLES ---------------------------------------------------------------------------------*/

/* PROTOTYPES --------------------------------------------------------------------------------*/

/* METHODS -----------------------------------------------------------------------------------*/

int Loki_Kirjoita(char *viesti)
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

PisteFont::PisteFont(int korkeus, int leveys, int lkm)
{
	tiedosto = NULL;

	font_korkeus = korkeus;
	font_leveys  = leveys;
	font_lkm	 = lkm;
	font_bitmap  = (UCHAR *)malloc(korkeus * leveys * lkm);
	font_buffer	 = PisteDraw_Buffer_Uusi(leveys*lkm,korkeus,true,255);

	this->Init_fonts();
}

PisteFont::PisteFont()
{
	tiedosto = NULL;
	Loki_Kirjoita("PisteFont()\n");

	font_korkeus = 0;
	font_leveys  = 0;
	font_lkm	 = 0;
}

int PisteFont::LataaTiedostosta(char *polku, char *file)
{
	Loki_Kirjoita("LataaTiedostosta(char *polku, char *file)\n");
	Loki_Kirjoita(polku);
	Loki_Kirjoita(file);
	Loki_Kirjoita("\n");
	
	tiedosto = new PisteLanguage();
	Loki_Kirjoita("tiedosto = new PisteLanguage();");

	char xfile[_MAX_PATH];
	strcpy(xfile,polku);
	strcat(xfile,file);

	Loki_Kirjoita("1.\n");

	tiedosto = new PisteLanguage();

	Loki_Kirjoita("2.\n");

	if (!tiedosto->Lue_Tiedosto(xfile)) 
		return -1;

	Loki_Kirjoita("3.\n");

	int ind = 0;
	
	//leveys
	ind = tiedosto->Hae_Indeksi("letter width");
	if (ind != -1)
		font_leveys = atoi(tiedosto->Hae_Teksti(ind));
	else
		return -1;

	Loki_Kirjoita("4.\n");

	//korkeus
	ind = tiedosto->Hae_Indeksi("letter height");
	if (ind != -1)
		font_korkeus = atoi(tiedosto->Hae_Teksti(ind));
	else
		return -1;

	Loki_Kirjoita("5.\n");

	//lkm
	ind = tiedosto->Hae_Indeksi("letters");
	if (ind != -1)
		font_lkm = strlen(tiedosto->Hae_Teksti(ind));
	else
		return -1;

	Loki_Kirjoita("6.\n");

	int buf_x, buf_y, buf_width;

	//kuvatiedoston leveys
	ind = tiedosto->Hae_Indeksi("image width");
	if (ind != -1)
		buf_width = atoi(tiedosto->Hae_Teksti(ind));
	else
		buf_width = 640;

	//buffer x
	ind = tiedosto->Hae_Indeksi("image x");
	if (ind != -1)
		buf_x = atoi(tiedosto->Hae_Teksti(ind));
	else
		return -1;	

	Loki_Kirjoita("7.\n");

	//buffer y
	ind = tiedosto->Hae_Indeksi("image y");
	if (ind != -1)
		buf_y = atoi(tiedosto->Hae_Teksti(ind));
	else
		return -1;

	Loki_Kirjoita("8.\n");

	if (buf_x < 0 || buf_x > 640 || buf_y < 0 || buf_y > 480)
		return -1;

	Loki_Kirjoita(tiedosto->Hae_Teksti(tiedosto->Hae_Indeksi("letter width")));
	Loki_Kirjoita(" ");
	Loki_Kirjoita(tiedosto->Hae_Teksti(tiedosto->Hae_Indeksi("letter height")));
	Loki_Kirjoita(" ");
	Loki_Kirjoita(tiedosto->Hae_Teksti(tiedosto->Hae_Indeksi("letters")));
	Loki_Kirjoita(" ");
	Loki_Kirjoita(tiedosto->Hae_Teksti(tiedosto->Hae_Indeksi("image x")));
	Loki_Kirjoita(" ");
	Loki_Kirjoita(tiedosto->Hae_Teksti(tiedosto->Hae_Indeksi("image y")));
	Loki_Kirjoita(" ");
	char teksti[255];
	itoa(font_lkm,teksti,10);
	Loki_Kirjoita(teksti);
	Loki_Kirjoita("\n");

	//kuvatiedosto
	ind = tiedosto->Hae_Indeksi("image");

	if (ind != -1) {

		Loki_Kirjoita("9.\n");
		Loki_Kirjoita(tiedosto->Hae_Teksti(tiedosto->Hae_Indeksi("image")));
		Loki_Kirjoita(" ");

		int temp_kuva = PisteDraw_Buffer_Uusi(buf_width,480,true,255);

		Loki_Kirjoita("10.\n");

		strcpy(xfile,polku);
		strcat(xfile,tiedosto->Hae_Teksti(ind));

		Loki_Kirjoita("11. ");Loki_Kirjoita(xfile);

		if (PisteDraw_Lataa_Kuva(temp_kuva,xfile, true) == PD_VIRHE) {
			PisteDraw_Buffer_Tuhoa(temp_kuva);
			return -1;
		}

		Loki_Kirjoita("12.\n");

		font_bitmap  = (UCHAR *)malloc(font_korkeus * font_leveys * font_lkm);
		font_buffer	 = PisteDraw_Buffer_Uusi(font_leveys * font_lkm,font_korkeus,true,255);

		Loki_Kirjoita("13.\n");

		UCHAR *buffer = NULL;
		DWORD ruudun_leveys;			
			
		if (PisteDraw_Piirto_Aloita(temp_kuva, *&buffer, (DWORD &) ruudun_leveys) == PD_VIRHE)
			return -1;

		Loki_Kirjoita("14. ruudun leveys");
		itoa(ruudun_leveys,teksti,10);
		Loki_Kirjoita(teksti);

		Loki_Kirjoita("\n15.");
		Get_bitmap(buf_x,buf_y,ruudun_leveys,buffer);

		Loki_Kirjoita("16.\n");

		PisteDraw_Piirto_Lopeta(temp_kuva);

		Loki_Kirjoita("17.");

		PisteDraw_Buffer_Tuhoa(temp_kuva);

		Loki_Kirjoita("18.");
	}
	else
		return -1;

	//this->Init_fonts_tiedosto();
	this->Init_fonts_tiedosto();

	return 0;
}


PisteFont::~PisteFont()
{
	free(font_bitmap);
	PisteDraw_Buffer_Tuhoa(font_buffer);

	if (tiedosto != NULL)
		delete tiedosto;
}

int PisteFont::Init_fonts_tiedosto(void)
{
	Loki_Kirjoita("\nInit_fonts_tiedosto(void)\n");
	
	int font_index[256];
	char kirjaimet[MAX_TEKSTIN_PITUUS];
	int i;
	
	for (i=0;i<256;i++)
		font_table[i]=-1;

	Loki_Kirjoita("\n1\n");

	strcpy(kirjaimet,tiedosto->Hae_Teksti(tiedosto->Hae_Indeksi("letters")));

	Loki_Kirjoita("\n2\n");
	
	for (i=0;i<font_lkm;i++){
		font_index[i] = i * font_leveys;
	}

	Loki_Kirjoita("\n3\n");

	for (i=0;i<font_lkm;i++){
		font_table[(UCHAR)toupper(kirjaimet[i])] = font_index[i];
	}
	
	Loki_Kirjoita("\n4\n");

	return(0);
}

int PisteFont::Init_fonts(void)
{
	Loki_Kirjoita("\nInit_fonts(void)\n");

	int font_index[256];
	int i;
	
	for (i=0;i<256;i++)
		font_table[i]=-1;
	
	for (i=0;i<font_lkm;i++)
		font_index[i] = i * font_leveys;

	font_table[(UCHAR)'A'] = font_index[0];
	font_table[(UCHAR)'B'] = font_index[1];
	font_table[(UCHAR)'C'] = font_index[2];
	font_table[(UCHAR)'D'] = font_index[3];
	font_table[(UCHAR)'E'] = font_index[4];
	font_table[(UCHAR)'F'] = font_index[5];
	font_table[(UCHAR)'G'] = font_index[6]; 
	font_table[(UCHAR)'H'] = font_index[7];
	font_table[(UCHAR)'I'] = font_index[8];
	font_table[(UCHAR)'J'] = font_index[9];
	font_table[(UCHAR)'K'] = font_index[10];
	font_table[(UCHAR)'L'] = font_index[11];
	font_table[(UCHAR)'M'] = font_index[12];
	font_table[(UCHAR)'N'] = font_index[13];
	font_table[(UCHAR)'O'] = font_index[14];
	font_table[(UCHAR)'P'] = font_index[15];
	font_table[(UCHAR)'Q'] = font_index[16];
	font_table[(UCHAR)'R'] = font_index[17];
	font_table[(UCHAR)'S'] = font_index[18];
	font_table[(UCHAR)'T'] = font_index[19];
	font_table[(UCHAR)'U'] = font_index[20];
	font_table[(UCHAR)'V'] = font_index[21];
	font_table[(UCHAR)'W'] = font_index[22];
	font_table[(UCHAR)'X'] = font_index[23];
	font_table[(UCHAR)'Y'] = font_index[24];
	font_table[(UCHAR)'Z'] = font_index[25];
	font_table[(UCHAR)toupper('å')] = font_index[26];
	font_table[(UCHAR)toupper('ä')] = font_index[27];
	font_table[(UCHAR)toupper('ö')] = font_index[28];
	font_table[(UCHAR)'0'] = font_index[29];
	font_table[(UCHAR)'1'] = font_index[30];
	font_table[(UCHAR)'2'] = font_index[31];
	font_table[(UCHAR)'3'] = font_index[32];
	font_table[(UCHAR)'4'] = font_index[33];
	font_table[(UCHAR)'5'] = font_index[34];
	font_table[(UCHAR)'6'] = font_index[35];
	font_table[(UCHAR)'7'] = font_index[36];
	font_table[(UCHAR)'8'] = font_index[37];
	font_table[(UCHAR)'9'] = font_index[38];
	font_table[(UCHAR)'.'] = font_index[39];
	font_table[(UCHAR)'!'] = font_index[40];
	font_table[(UCHAR)'?'] = font_index[41];
	font_table[(UCHAR)':'] = font_index[42];
	font_table[(UCHAR)'-'] = font_index[43];
	font_table[(UCHAR)','] = font_index[44];
	font_table[(UCHAR)'+'] = font_index[45];
	font_table[(UCHAR)'='] = font_index[46];
	font_table[(UCHAR)'('] = font_index[47];
	font_table[(UCHAR)')'] = font_index[48];
	font_table[(UCHAR)'/'] = font_index[49];
	font_table[(UCHAR)'#'] = font_index[50];
	font_table[(UCHAR)'\\']= font_index[51];
	font_table[(UCHAR)'_']= font_index[52];
	font_table[(UCHAR)'%']= font_index[53];
	return(0);
}

int PisteFont::Get_bitmap(int buffer_x, int buffer_y, int ruudun_leveys, UCHAR *buffer)
{
	int font_bitmap_leveys = font_lkm * font_leveys;

	Loki_Kirjoita("\nGet_bitmap\n");

	UCHAR *fbuffer = NULL;
	DWORD f_leveys;
	UCHAR vari;

	Loki_Kirjoita("\n1\n");

	if (PisteDraw_Piirto_Aloita(font_buffer, *&fbuffer, (DWORD &) f_leveys) == PD_VIRHE)
		return PD_VIRHE;

	Loki_Kirjoita("\n2\n");

	for (int y=0;y<this->font_korkeus;y++)
		for (int x=0;x<font_bitmap_leveys-1;x++)
		{
			vari = buffer[buffer_x+x+(buffer_y+y)*ruudun_leveys];
			fbuffer[x+y*f_leveys] =	vari;
			if (vari != 255)
				vari %= 32;
			font_bitmap[x+y*font_bitmap_leveys] = vari;
			
		}

	Loki_Kirjoita("\n3\n");

	if (PisteDraw_Piirto_Lopeta(font_buffer) == PD_VIRHE)
		return PD_VIRHE;

	Loki_Kirjoita("\n4\n");

	return(0);
}

int PisteFont::Piirra_merkkijono(int font_x, int font_y, int lPitch, char *merkkijono,
								 UCHAR *back_buffer, bool loop)
{
	int i = 0, i2;
	int x, y, ix, fx, fy, x2;
	UCHAR color;
	char merkki = '0';
	int font_bitmap_leveys = font_lkm * font_leveys;
	UCHAR *buffer;

	while (merkki != '\0')
	{
		merkki = merkkijono[i];
		ix = font_table[(UCHAR)toupper(merkki)];
		i2 = i * font_leveys;
		
		if (ix > -1)
		{
			for (x=0;x<font_leveys;x++)
			{
				fx = font_x + x + i2;
				x2 = ix+x;
				buffer = back_buffer + fx;

				for (y=0;y<font_korkeus;y++)
				{
					if ((color = font_bitmap[x2 + y * font_bitmap_leveys]) != 255)
					{
						fy = font_y + y;
						buffer[fy * lPitch] = color;
					}
				}
			}
		}
		i++;
	}
	return((i-1)*font_leveys);
}

int PisteFont::Piirra_merkkijono(char *merkkijono, int font_x, int font_y, int kohde_buffer)
{
	int i = 0, i2;
	int ix;
	char merkki = '0';

	while (merkki != '\0')
	{
		merkki = merkkijono[i];
		ix = font_table[(UCHAR)toupper(merkki)];
		i2 = i * font_leveys;
		
		if (ix > -1)
			PisteDraw_Buffer_Flip_Nopea(font_buffer,kohde_buffer,font_x+i2,font_y,ix,0,ix+font_leveys,font_korkeus);

		i++;
	}

	return((i-1)*font_leveys);
}

int PisteFont::Piirra_merkkijono_led(int font_x, int font_y, int lPitch, char *merkkijono,//[], 
									 UCHAR *back_buffer)
{
	int i = 0, i2;
	int x, y, ix, fx, fy, x2;
	UCHAR color;
	char merkki = '0';
	int font_bitmap_leveys = font_lkm * font_leveys;

	while (merkki != '\0')
	{
		merkki = merkkijono[i];
		ix = font_table[(UCHAR)toupper(merkki)];
		i2 = i * font_leveys;
		if (ix > -1)
		{
			for (x=0;x<font_leveys;x++)
			{
				fx = font_x + x;
				x2 = ix+x;
				for (y=0;y<font_korkeus;y++)
				{
					color = font_bitmap[x2+y*font_bitmap_leveys];
					if (color == 11 || color == 31)
					{
						fy = font_y + y;
						fy %= 470;
						color++;
						back_buffer[fx + i2 + fy * lPitch] = color;					
					}
				}
			}
		}
		i++;
	}
	return((i-1)*font_leveys);
}

int PisteFont::Piirra_merkkijono_lapinakyva(int font_x, int font_y, int lPitch, RECT klipperi, char *merkkijono,
							  UCHAR *back_buffer, bool loop, int pros)
{
	int i = 0;
	int x, y, ix, fx, fy, p1, p2;
	UCHAR color, color2, color3;
	char merkki = '0';

	int font_bitmap_leveys = font_lkm * font_leveys;

	if (pros>100)
		pros = 100;
	
	p1 = pros;
	p2 = 100 - pros;

	while (merkki != '\0')
	{
		merkki = merkkijono[i];
		ix = font_table[(UCHAR)toupper(merkki)];
		if (ix > -1)
		{
			for (x=0;x<font_leveys;x++)
			{
				fx = font_x + x + i * font_leveys;
				//if (loop) fx %= 480;
				//x2 = ix + x;

				if (fx >= klipperi.left && fx <= klipperi.right)
					for (y=0;y<font_korkeus;y++)
					{
						color = font_bitmap[ix+x+y*font_bitmap_leveys];
						
						if (color!=255)
						{
							fy = font_y + y;
							
							if (fy >= klipperi.top && fy < klipperi.bottom)
							{
								//fy %= 470;
								fy *= lPitch;
								fy += fx;
								color2  = back_buffer[fy];
								/*
								color3  = color2;
								color2 %= 32;
								color3 -= color2;
		*/
								color3  = (color2>>5)<<5;
								color2 -= color3;

								color   = (color * p1 + color2 * p2)/100;
								back_buffer[fy] = color + color3;
							}
						}
					}
			}
		}
		i++;
	}
	return((i-1)*font_leveys);
}

int PisteFont::Piirra_merkkijono_varillinen(int font_x, int font_y, int lPitch, char *merkkijono,
							  UCHAR *back_buffer, bool loop, UCHAR color2)
{
	int i = 0, i2;
	int x, y, ix, fx, fy, x2;
	UCHAR color;
	char merkki = '0';
	int font_bitmap_leveys = font_lkm * font_leveys;

	while (merkki != '\0')
	{
		merkki = merkkijono[i];
		ix = font_table[(UCHAR)toupper(merkki)];
		i2 = i * font_leveys;
		if (ix > -1)
		{
			for (x=0;x<font_leveys;x++)
			{
				fx = font_x + x + i2;
				if (loop) fx %= 480;
				x2 = ix+x;
				for (y=0;y<font_korkeus;y++)
				{
					color = font_bitmap[x2+y*font_bitmap_leveys];
					
					if (color!=255)
					{
						fy = font_y + y;
						fy %= 470;
						color /= 2;
						back_buffer[fx + fy * lPitch] = color + color2;
					}
				}
			}
		}
		i++;
	}
	return((i-1)*font_leveys);
}

