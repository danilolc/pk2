//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "engine/PFont.hpp"

#include "engine/PUtils.hpp"
#include "engine/PDraw.hpp"
#include "engine/PLang.hpp"
#include "engine/platform.hpp"

#include <cmath>
#include <cstring>

int PFont::init_charlist() {

	const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "\xc5\xc4\xd6" "0123456789.!?:-.+=()/#\\_%";

	for ( int i = 0; i < 256; i++ )
		charlist[i] = -1;
	
	for ( int i = 0; i < sizeof(chars); i++)
		charlist[(u8)chars[i]] = i * char_w;
	
	return 0;

}

int PFont::get_image(int x, int y, int img_source) {

	image_index = PDraw::image_cut(img_source, x, y, char_w * char_count, char_h * char_count);
	return 0;

}

int PFont::load(const char* file_path, const char* file) {

	char path[128];
	int i = 0;
	int font_index[256];
	char chars[256];

	strcpy(path,file_path);
	strcat(path,file);
	if (!PUtils::Find(path)) {
	
		return -1;
	
	}

	PLang* param_file = new PLang();

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

	if (!PUtils::Find(path)) {
	
		return -1;
	
	}

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

int PFont::write(int posx, int posy, const char *text) {
	
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

int PFont::write_trasparent(int posx, int posy, const char* text, int alpha) {

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

PFont::PFont(int img_source, int x, int y, int width, int height, int count) {

	char_w = width;
	char_h = height;
	char_count = count;

	this->get_image(x, y, img_source);
	this->init_charlist();

}

PFont::PFont() {

	char_w = 0;
	char_h = 0;
	char_count = 0;

}

PFont::~PFont() {

	if(image_index != -1)
		PDraw::image_delete(image_index);

}
