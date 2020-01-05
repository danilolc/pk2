//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "engine/PFont.hpp"

#include "engine/PDraw.hpp"
#include "engine/PLang.hpp"
#include "engine/platform.hpp"

#include <cmath>
#include <cstring>

int PFont::init_charlist(){
	int font_index[256], i;

	for (i=0;i<256;i++) charlist[i] = -1;
	for (i=0;i<char_count;i++) font_index[i] = i * char_w; //The x position of the char

	//TODO - create a {'A', 'B' ..} array
	charlist['A'] = font_index[0];
	charlist['B'] = font_index[1];
	charlist['C'] = font_index[2];
	charlist['D'] = font_index[3];
	charlist['E'] = font_index[4];
	charlist['F'] = font_index[5];
	charlist['G'] = font_index[6];
	charlist['H'] = font_index[7];
	charlist['I'] = font_index[8];
	charlist['J'] = font_index[9];
	charlist['K'] = font_index[10];
	charlist['L'] = font_index[11];
	charlist['M'] = font_index[12];
	charlist['N'] = font_index[13];
	charlist['O'] = font_index[14];
	charlist['P'] = font_index[15];
	charlist['Q'] = font_index[16];
	charlist['R'] = font_index[17];
	charlist['S'] = font_index[18];
	charlist['T'] = font_index[19];
	charlist['U'] = font_index[20];
	charlist['V'] = font_index[21];
	charlist['W'] = font_index[22];
	charlist['X'] = font_index[23];
	charlist['Y'] = font_index[24];
	charlist['Z'] = font_index[25];
	charlist[197] = font_index[26];
	charlist[196] = font_index[27];
	charlist[214] = font_index[28];
	charlist['0'] = font_index[29];
	charlist['1'] = font_index[30];
	charlist['2'] = font_index[31];
	charlist['3'] = font_index[32];
	charlist['4'] = font_index[33];
	charlist['5'] = font_index[34];
	charlist['6'] = font_index[35];
	charlist['7'] = font_index[36];
	charlist['8'] = font_index[37];
	charlist['9'] = font_index[38];
	charlist['.'] = font_index[39];
	charlist['!'] = font_index[40];
	charlist['?'] = font_index[41];
	charlist[':'] = font_index[42];
	charlist['-'] = font_index[43];
	charlist[','] = font_index[44];
	charlist['+'] = font_index[45];
	charlist['='] = font_index[46];
	charlist['('] = font_index[47];
	charlist[')'] = font_index[48];
	charlist['/'] = font_index[49];
	charlist['#'] = font_index[50];
	charlist['\\']= font_index[51];
	charlist['_'] = font_index[52];
	charlist['%'] = font_index[53];

	return 0;
}
int PFont::get_image(int x, int y, int img_source){
	image_index = PDraw::image_cut(img_source, x, y, char_w * char_count, char_h * char_count);
	return 0;
}
int PFont::load(const char* file_path, const char* file) {

	char path[128];
	int i = 0;
	int font_index[256];
	char chars[256];

	PLang* param_file = new PLang();

	strcpy(path,file_path);
	strcat(path,file);

	if (!param_file->Read_File(path)){
		delete param_file;
		return -1;
	}

	//i = param_file->Hae_Indeksi("image width");
	//int buf_width = atoi(param_file->Get_Text(i));

	i = param_file->Hae_Indeksi("image x");
	int buf_x = atoi(param_file->Get_Text(i));

	i = param_file->Hae_Indeksi("image y");
	int buf_y = atoi(param_file->Get_Text(i));

	i = param_file->Hae_Indeksi("letters");
	this->char_count = strlen(param_file->Get_Text(i));

	i = param_file->Hae_Indeksi("letter width");
	this->char_w = atoi(param_file->Get_Text(i));

	i = param_file->Hae_Indeksi("letter height");
	this->char_h = atoi(param_file->Get_Text(i));

	i = param_file->Hae_Indeksi("letters");
	strcpy(chars, param_file->Get_Text(i));

	i = param_file->Hae_Indeksi("image");
	strcpy(path,file_path);
	strcat(path,param_file->Get_Text(i));

	delete param_file;

	int temp_image = PDraw::image_load(path,false);
	if (temp_image == -1) return -1;

	this->get_image(buf_x, buf_y, temp_image);
	PDraw::image_delete(temp_image);

	for ( uint i = 0; i < 256; i++ )
		charlist[i] = -1;
	
	for ( uint i = 0; i < char_count; i++ )
		font_index[i] = i * char_w;
	
	for ( uint i = 0; i < char_count; i++ )
		charlist[(u8)(chars[i]&~' ')] = font_index[i];

	return 0;
}

int PFont::write(int posx, int posy, const char *text){
	int i = 0, i2;
	int ix;
	char curr_char;

	PDraw::RECT srcrect, dstrect;
	srcrect.y = 0;
	srcrect.w = char_w;
	srcrect.h = char_h;
	dstrect.y = posy;

	do {
		curr_char = text[i];
		ix = charlist[(u8)(curr_char&~' ')];
		i2 = i * char_w + posx;
		if (ix > -1) {
			srcrect.x = ix;
			dstrect.x = i2;
			PDraw::image_cutclip(image_index,srcrect,dstrect);
		}
		i++;
	} while(curr_char != '\0');

	return((i-1)*char_w);
}
int PFont::write_trasparent(int posx, int posy, const char* text, int alpha){

	u8 *back_buffer, *txt_buffer;
	u32 back_w, txt_w;

	uint i = 0;
	u8 color1, color2, color3;
	char curr_char;

	if (alpha > 100) alpha = 100;
	int a1 = alpha;
	int a2 = 100 - alpha;

	PDraw::drawscreen_start(*&back_buffer, (u32 &)back_w);
	PDraw::drawimage_start(image_index, *&txt_buffer, (u32 &)txt_w);

	do {
		curr_char = text[i];
		int ix = charlist[(u8)(curr_char&~' ')];
		if (ix > -1){
			for (uint x = 0; x < char_w; x++) {
				
				int fx = posx + x + i * char_w;
				if(fx >= back_w) break; //Screen weight

				for (uint y = 0; y < char_h; y++) {

					color1 = txt_buffer[ix+x+y*txt_w];

					if (color1 != 255) {
						
						int fy = posy + y;
						fy *= back_w;
						fy += fx;

						// Mix colors
						color1 &= (u8)0b00011111;
						color2 = back_buffer[fy];
						color3 = color2 & (u8)0b11100000;
						color2-= color3;
						color1 = (color1 * a1 + color2 * a2)/100;

						back_buffer[fy] = color1 + color3;

					}
				}
			}
		}
		i++;
	
	} while(curr_char != '\0');

	PDraw::drawscreen_end();
	PDraw::drawimage_end(image_index);

	return( (i-1) * char_w );
}

PFont::PFont(int img_source, int x, int y, int width, int height, int count){
	char_w = width;
	char_h = height;
	char_count = count;

	this->get_image(x, y, img_source);
	this->init_charlist();
}
PFont::PFont(){
	char_w = 0;
	char_h = 0;
	char_count = 0;
}
PFont::~PFont(){
	if(image_index != -1)
		PDraw::image_delete(image_index);
}
