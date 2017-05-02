//#########################
//PisteEngine - PisteFont
//by Janne Kivilahti from Piste Gamez
//#########################

#include "PisteFont.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>

int PisteFont2::InitCharList(){
	int font_index[256], i;

	for (i=0;i<256;i++)
		charList[i]=-1;
	for (i=0;i<char_count;i++)
		font_index[i] = i * char_w;

	charList[(UCHAR)'A'] = font_index[0];
	charList[(UCHAR)'B'] = font_index[1];
	charList[(UCHAR)'C'] = font_index[2];
	charList[(UCHAR)'D'] = font_index[3];
	charList[(UCHAR)'E'] = font_index[4];
	charList[(UCHAR)'F'] = font_index[5];
	charList[(UCHAR)'G'] = font_index[6];
	charList[(UCHAR)'H'] = font_index[7];
	charList[(UCHAR)'I'] = font_index[8];
	charList[(UCHAR)'J'] = font_index[9];
	charList[(UCHAR)'K'] = font_index[10];
	charList[(UCHAR)'L'] = font_index[11];
	charList[(UCHAR)'M'] = font_index[12];
	charList[(UCHAR)'N'] = font_index[13];
	charList[(UCHAR)'O'] = font_index[14];
	charList[(UCHAR)'P'] = font_index[15];
	charList[(UCHAR)'Q'] = font_index[16];
	charList[(UCHAR)'R'] = font_index[17];
	charList[(UCHAR)'S'] = font_index[18];
	charList[(UCHAR)'T'] = font_index[19];
	charList[(UCHAR)'U'] = font_index[20];
	charList[(UCHAR)'V'] = font_index[21];
	charList[(UCHAR)'W'] = font_index[22];
	charList[(UCHAR)'X'] = font_index[23];
	charList[(UCHAR)'Y'] = font_index[24];
	charList[(UCHAR)'Z'] = font_index[25];
	charList[(UCHAR)toupper('�')] = font_index[26];
	charList[(UCHAR)toupper('�')] = font_index[27];
	charList[(UCHAR)toupper('�')] = font_index[28];
	charList[(UCHAR)'0'] = font_index[29];
	charList[(UCHAR)'1'] = font_index[30];
	charList[(UCHAR)'2'] = font_index[31];
	charList[(UCHAR)'3'] = font_index[32];
	charList[(UCHAR)'4'] = font_index[33];
	charList[(UCHAR)'5'] = font_index[34];
	charList[(UCHAR)'6'] = font_index[35];
	charList[(UCHAR)'7'] = font_index[36];
	charList[(UCHAR)'8'] = font_index[37];
	charList[(UCHAR)'9'] = font_index[38];
	charList[(UCHAR)'.'] = font_index[39];
	charList[(UCHAR)'!'] = font_index[40];
	charList[(UCHAR)'?'] = font_index[41];
	charList[(UCHAR)':'] = font_index[42];
	charList[(UCHAR)'-'] = font_index[43];
	charList[(UCHAR)','] = font_index[44];
	charList[(UCHAR)'+'] = font_index[45];
	charList[(UCHAR)'='] = font_index[46];
	charList[(UCHAR)'('] = font_index[47];
	charList[(UCHAR)')'] = font_index[48];
	charList[(UCHAR)'/'] = font_index[49];
	charList[(UCHAR)'#'] = font_index[50];
	charList[(UCHAR)'\\']= font_index[51];
	charList[(UCHAR)'_']= font_index[52];
	charList[(UCHAR)'%']= font_index[53];

	return 0;
}
int PisteFont2::GetImage(int x, int y, int img_source){
	ImageIndex = PisteDraw2_Image_Cut(img_source, x, y, char_w*char_count, char_h*char_count);
}
int PisteFont2::LoadFile(const char* path, const char* file){
	//TODO
}

int PisteFont2::Write_Text(int posx, int posy, const char *text){
	int i = 0, i2;
	int ix;
	char character;

	PD_RECT srcrect, dstrect;
	srcrect.y = 0;
	srcrect.w = char_w;
	srcrect.h = char_h;
	dstrect.y = posy;

	while (character != '\0'){
		character = text[i];
		ix = charList[(UCHAR)toupper(character)];
		i2 = i * char_w;

		if (ix > -1){
			srcrect.x = ix;
			dstrect.x = i2;
			PisteDraw2_Image_CutClip(ImageIndex,srcrect,dstrect);
		}
		i++;
	}

	return((i-1)*char_w);
}
int PisteFont2::Write_TextTrasparent(int posx, int posy, const char* text, int alpha){
	//TODO
}

PisteFont2::PisteFont2(int img_source, int x, int y, int width, int height, int count){
	char_w = width;
	char_h = height;
	char_count = count;

	this->GetImage(x, y, img_source);
	this->InitCharList();
}
PisteFont2::PisteFont2(){
	char_w = 0;
	char_h = 0;
	char_count = 0;
}
PisteFont2::~PisteFont2(){
	PisteDraw2_Image_Delete(ImageIndex);
}











PisteFont::PisteFont(int korkeus, int leveys, int lkm){
	param_file = NULL;

	font_korkeus = korkeus;
	font_leveys  = leveys;
	font_lkm	 = lkm;
	font_bitmap  = new UCHAR[korkeus * leveys * lkm];//(UCHAR *)malloc(korkeus * leveys * lkm);

	font_buffer	 = PisteDraw_Buffer_Uusi(leveys*lkm,korkeus,true,255);

	//PisteDraw_Lataa_Kuva(font_buffer,"wormz2.bmp", true);

	this->Init_fonts();
}
PisteFont::PisteFont(){
	param_file = NULL;
	font_korkeus = 0;
	font_leveys  = 0;
	font_lkm	 = 0;
}
PisteFont::~PisteFont(){
	delete font_bitmap;
	PisteDraw_Buffer_Tuhoa(font_buffer);

	if (param_file != NULL)
		delete param_file;
}

int PisteFont::LataaTiedostosta(char *polku, char *file){
	char xfile[128];
	int ind = 0;
	int temp_kuva;
	int buf_x, buf_y, buf_width;

	param_file = new PisteLanguage();

	strcpy(xfile,polku);
	strcat(xfile,file);

	if (!param_file->Read_File(xfile)){
		return -1;
	}

	ind = param_file->Hae_Indeksi("image width");
	buf_width = atoi(param_file->Hae_Teksti(ind));

	ind = param_file->Hae_Indeksi("image x");
	buf_x = atoi(param_file->Hae_Teksti(ind));

	ind = param_file->Hae_Indeksi("image y");
	buf_y = atoi(param_file->Hae_Teksti(ind));

	ind = param_file->Hae_Indeksi("letters");
	font_lkm = strlen(param_file->Hae_Teksti(ind));

	ind = param_file->Hae_Indeksi("letter width");
	font_leveys = atoi(param_file->Hae_Teksti(ind));

	ind = param_file->Hae_Indeksi("letter height");
	font_korkeus = atoi(param_file->Hae_Teksti(ind));


	ind = param_file->Hae_Indeksi("image");

	temp_kuva = PisteDraw_Buffer_Uusi(buf_width,480,true,255);

	strcpy(xfile,polku);
	strcat(xfile,param_file->Hae_Teksti(ind));

	if (PisteDraw_Lataa_Kuva(temp_kuva,xfile, false) == PD_VIRHE) {
		PisteDraw_Buffer_Tuhoa(temp_kuva);
		return -1;
	}

	font_bitmap  = new UCHAR[font_korkeus * font_leveys * font_lkm];//(UCHAR *)malloc(font_korkeus * font_leveys * font_lkm);
	font_buffer	 = PisteDraw_Buffer_Uusi(font_leveys * font_lkm,font_korkeus,true,255);

	UCHAR *buffer = NULL;
	DWORD ruudun_leveys;

	Get_bitmap(buf_x,buf_y,ruudun_leveys,temp_kuva);

	PisteDraw_Buffer_Tuhoa(temp_kuva);


	this->Init_fonts_tiedosto();

	return 0;
}
int PisteFont::Init_fonts_tiedosto(){

	int font_index[256];
	char kirjaimet[MAX_TEXT_LENGTH];
	int i;

	for (i=0;i<256;i++)
		font_table[i]=-1;

	strcpy(kirjaimet,param_file->Hae_Teksti(param_file->Hae_Indeksi("letters")));

	for (i=0;i<font_lkm;i++)
		font_index[i] = i * font_leveys;

	for (i=0;i<font_lkm;i++)
		font_table[(UCHAR)toupper(kirjaimet[i])] = font_index[i];

	return(0);
}
int PisteFont::Init_fonts(){

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
	font_table[(UCHAR)toupper('�')] = font_index[26];
	font_table[(UCHAR)toupper('�')] = font_index[27];
	font_table[(UCHAR)toupper('�')] = font_index[28];
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
int PisteFont::Get_bitmap(int buffer_x, int buffer_y, int ruudun_leveys, int buffer_index){
	int x = buffer_x,
		y = buffer_y,
		w = font_lkm * font_leveys,
		h = font_korkeus;

	PisteDraw_Buffer_Flip_Nopea(
		buffer_index,
		font_buffer,
		0, 0,
		x, y,
		x+w, y+h);
}

int PisteFont::Piirra_merkkijono(int font_x, int font_y, int lPitch, char *merkkijono,
								 UCHAR *back_buffer, bool loop){
	int i = 0, i2;
	int x, y, ix, fx, fy, x2;
	UCHAR color;
	char merkki = '0';
	int font_bitmap_leveys = font_lkm * font_leveys;
	UCHAR *buffer;

	while (merkki != '\0'){
		merkki = merkkijono[i];
		ix = font_table[(UCHAR)toupper(merkki)];
		i2 = i * font_leveys;
		if (ix > -1){
			for (x=0;x<font_leveys;x++){
				fx = font_x + x + i2;
				x2 = ix+x;
				buffer = back_buffer + fx;

				for (y=0;y<font_korkeus;y++){
					if ((color = font_bitmap[x2 + y * font_bitmap_leveys]) != 255){
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
int PisteFont::Piirra_merkkijono(char *merkkijono, int font_x, int font_y, int kohde_buffer){
	int i = 0, i2;
	int ix;
	char merkki = '0';

	while (merkki != '\0'){
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
									 UCHAR *back_buffer){
	int i = 0, i2;
	int x, y, ix, fx, fy, x2;
	UCHAR color;
	char merkki = '0';
	int font_bitmap_leveys = font_lkm * font_leveys;

	while (merkki != '\0'){
		merkki = merkkijono[i];
		ix = font_table[(UCHAR)toupper(merkki)];
		i2 = i * font_leveys;
		if (ix > -1){
			for (x=0;x<font_leveys;x++){
				fx = font_x + x;
				x2 = ix+x;
				for (y=0;y<font_korkeus;y++){
					color = font_bitmap[x2+y*font_bitmap_leveys];
					if (color == 11 || color == 31){
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
int PisteFont::Piirra_merkkijono_lapinakyva(char *merkkijono, int font_x, int font_y, int kohde_buffer, int alpha){

	int i = 0;
	int x, y, ix, fx, fy;
	UCHAR color;
	char merkki = '0';

	int font_bitmap_leveys = font_lkm * font_leveys;

	UCHAR* buffer;
	DWORD w;
	PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer, (DWORD &)w);

	while (merkki != '\0'){
		merkki = merkkijono[i];
		ix = font_table[(UCHAR)toupper(merkki)];
		if (ix > -1){
			for (x=0;x<font_leveys;x++){
				for (y=0;y<font_korkeus;y++){
					color = font_bitmap[ix+x+y*font_bitmap_leveys]; //TODO - Alpha bug
					if (color!=255){
						fx = font_x + x + i * font_leveys;
						fy = font_y + y;
						PisteDraw_SetColor(color, fx, fy, alpha);
					}
				}
			}
		}
		i++;
	}

	PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER);

	return((i-1)*font_leveys);
}
int PisteFont::Piirra_merkkijono_varillinen(int font_x, int font_y, int lPitch, char *merkkijono,
							  UCHAR *back_buffer, bool loop, UCHAR color2){
	int i = 0, i2;
	int x, y, ix, fx, fy, x2;
	UCHAR color;
	char merkki = '0';
	int font_bitmap_leveys = font_lkm * font_leveys;

	while (merkki != '\0'){
		merkki = merkkijono[i];
		ix = font_table[(UCHAR)toupper(merkki)];
		i2 = i * font_leveys;
		if (ix > -1){
			for (x=0;x<font_leveys;x++){
				fx = font_x + x + i2;
				if (loop) fx %= 480;
				x2 = ix+x;
				for (y=0;y<font_korkeus;y++){
					color = font_bitmap[x2+y*font_bitmap_leveys];

					if (color!=255){
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
