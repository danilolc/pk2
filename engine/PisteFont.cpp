//#########################
//PisteEngine - PisteFont
//by Janne Kivilahti from Piste Gamez
//#########################

#include <cctype>
#include <iostream>
#include <cstring>
#include <cstdio>

#include <fstream>
#include <iostream>

#include "platform.hpp"
#include "PisteFont.hpp"
#include "PisteDraw.hpp"
#include "PisteLanguage.hpp"

int PisteFont2::InitCharList(){
	int font_index[256], i;

	for (i=0;i<256;i++) charList[i] = -1;
	for (i=0;i<char_count;i++) font_index[i] = i * char_w; //The x position of the char

	charList['A'] = font_index[0];
	charList['B'] = font_index[1];
	charList['C'] = font_index[2];
	charList['D'] = font_index[3];
	charList['E'] = font_index[4];
	charList['F'] = font_index[5];
	charList['G'] = font_index[6];
	charList['H'] = font_index[7];
	charList['I'] = font_index[8];
	charList['J'] = font_index[9];
	charList['K'] = font_index[10];
	charList['L'] = font_index[11];
	charList['M'] = font_index[12];
	charList['N'] = font_index[13];
	charList['O'] = font_index[14];
	charList['P'] = font_index[15];
	charList['Q'] = font_index[16];
	charList['R'] = font_index[17];
	charList['S'] = font_index[18];
	charList['T'] = font_index[19];
	charList['U'] = font_index[20];
	charList['V'] = font_index[21];
	charList['W'] = font_index[22];
	charList['X'] = font_index[23];
	charList['Y'] = font_index[24];
	charList['Z'] = font_index[25];
	charList[(BYTE)197] = font_index[26];
	charList[(BYTE)196] = font_index[27];
	charList[(BYTE)214] = font_index[28];
	charList['0'] = font_index[29];
	charList['1'] = font_index[30];
	charList['2'] = font_index[31];
	charList['3'] = font_index[32];
	charList['4'] = font_index[33];
	charList['5'] = font_index[34];
	charList['6'] = font_index[35];
	charList['7'] = font_index[36];
	charList['8'] = font_index[37];
	charList['9'] = font_index[38];
	charList['.'] = font_index[39];
	charList['!'] = font_index[40];
	charList['?'] = font_index[41];
	charList[':'] = font_index[42];
	charList['-'] = font_index[43];
	charList[','] = font_index[44];
	charList['+'] = font_index[45];
	charList['='] = font_index[46];
	charList['('] = font_index[47];
	charList[')'] = font_index[48];
	charList['/'] = font_index[49];
	charList['#'] = font_index[50];
	charList['\\']= font_index[51];
	charList['_'] = font_index[52];
	charList['%'] = font_index[53];
	charList['\''] = font_index[54];

	return 0;
}
int PisteFont2::GetImage(int x, int y, int img_source){
	ImageIndex = PisteDraw2_Image_Cut(img_source, x, y, char_w*char_count, char_h*char_count);
	return 0;
}
int PisteFont2::LoadFile(const char* file_path, const char* file){
	char path[128];
	int i = 0;
	int temp_image;
	int buf_x, buf_y, buf_width;
	int font_index[256];
	char chars[256];

	PisteLanguage* param_file = new PisteLanguage();

	strcpy(path,file_path);
	strcat(path,file);

	if (!param_file->Read_File(path)){
		delete param_file;
		return -1;
	}

	i = param_file->Hae_Indeksi("image width");
	buf_width = atoi(param_file->Hae_Teksti(i));

	i = param_file->Hae_Indeksi("image x");
	buf_x = atoi(param_file->Hae_Teksti(i));

	i = param_file->Hae_Indeksi("image y");
	buf_y = atoi(param_file->Hae_Teksti(i));

	i = param_file->Hae_Indeksi("letters");
	char_count = strlen(param_file->Hae_Teksti(i));

	i = param_file->Hae_Indeksi("letter width");
	char_w = atoi(param_file->Hae_Teksti(i));

	i = param_file->Hae_Indeksi("letter height");
	char_h = atoi(param_file->Hae_Teksti(i));

	i = param_file->Hae_Indeksi("letters");
	strcpy(chars,param_file->Hae_Teksti(i));

	i = param_file->Hae_Indeksi("image");
	strcpy(path,file_path);
	strcat(path,param_file->Hae_Teksti(i));

	delete param_file;

	temp_image = PisteDraw2_Image_Load(path,false);
	if (temp_image == -1) return -1;

	this->GetImage(buf_x,buf_y,temp_image);
	PisteDraw2_Image_Delete(temp_image);

	for (i=0;i<256;i++)
		charList[i]=-1;
	for (i=0;i<char_count;i++)
		font_index[i] = i * char_w;
	for (i=0;i<char_count;i++)
		charList[(BYTE) toupper(chars[i])] = font_index[i];

	return 0;
}

int PisteFont2::Write_Text(int posx, int posy, const char *text){
	int i = 0, i2;
	int ix;
	char curr_char;

	PD_RECT srcrect, dstrect;
	srcrect.y = 0;
	srcrect.w = char_w;
	srcrect.h = char_h;
	dstrect.y = posy;

	do {
		curr_char = text[i];
		ix = charList[(BYTE)toupper(curr_char)];
		i2 = i * char_w + posx;
		if (ix > -1) {
			srcrect.x = ix;
			dstrect.x = i2;
			PisteDraw2_Image_CutClip(ImageIndex,srcrect,dstrect);
		}
		i++;
	} while(curr_char != '\0');

	return((i-1)*char_w);
}
int PisteFont2::Write_TextTrasparent(int posx, int posy, const char* text, int alpha){

	BYTE *back_buffer, *txt_buffer;
	DWORD back_w, txt_w;

	int i = 0;
	int x, y, ix, fx, fy, a1, a2;
	BYTE color1, color2, color3;
	char curr_char;

	if (alpha > 100) alpha = 100;
	a1 = alpha;
	a2 = 100 - alpha;

	PisteDraw2_DrawScreen_Start(*&back_buffer, (DWORD &)back_w);
	PisteDraw2_DrawImage_Start(ImageIndex, *&txt_buffer, (DWORD &)txt_w);

	do {
		curr_char = text[i];
		ix = charList[(BYTE)toupper(curr_char)];
		if (ix > -1){
			for (x = 0; x < char_w; x++){
				fx = posx + x + i * char_w;
				if(fx >= back_w) break; //Screen weight

				for (y=0;y<char_h;y++){
					color1 = txt_buffer[ix+x+y*txt_w];
					if (color1!=255){
						fy = posy + y;
						fy *= back_w;
						fy += fx;

						color1 &= (BYTE)0b00011111;
						color2 = back_buffer[fy];
						color3 = color2 & (BYTE)0b11100000;
						color2-= color3;
						color1 = (color1 * a1 + color2 * a2)/100;
						back_buffer[fy] = color1 + color3;
					}
				}
			}
		}
		i++;
	} while(curr_char != '\0');

	PisteDraw2_DrawScreen_End();
	PisteDraw2_DrawImage_End(ImageIndex);

	return((i-1)*char_w);
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
	if(ImageIndex != -1)
		PisteDraw2_Image_Delete(ImageIndex);
}
