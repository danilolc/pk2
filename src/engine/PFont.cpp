//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
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

	for ( uint i = 0; i < 256; i++ )
		charlist[i] = -1;
	
	for ( uint i = 0; i < sizeof(chars); i++)
		charlist[(u8)chars[i]] = i * char_w;
	
	return 0;

}

int PFont::get_image(int x, int y, int img_source) {

	image_index = PDraw::image_cut(img_source, x, y, char_w * char_count, char_h * char_count);
	return 0;

}

int PFont::load(PFile::Path path) {

	int i = 0;
	char chars[256];

	if (!path.Find()) 
		return -1;

	PLang param_file = PLang();

	if (!param_file.Read_File(path))
		return -1;

	//i = param_file.Search_Id("image width");
	//int buf_width = atoi(param_file.Get_Text(i));

	i = param_file.Search_Id("image x");
	int buf_x = atoi(param_file.Get_Text(i));

	i = param_file.Search_Id("image y");
	int buf_y = atoi(param_file.Get_Text(i));

	i = param_file.Search_Id("letters");
	this->char_count = strlen(param_file.Get_Text(i));

	i = param_file.Search_Id("letter width");
	this->char_w = atoi(param_file.Get_Text(i));

	i = param_file.Search_Id("letter height");
	this->char_h = atoi(param_file.Get_Text(i));

	i = param_file.Search_Id("letters");
	strcpy(chars, param_file.Get_Text(i));

	i = param_file.Search_Id("image");
	path.SetFile(param_file.Get_Text(i));

	if (!path.Find())
		return -1;

	int temp_image = PDraw::image_load(path, false);
	if (temp_image == -1) return -1;

	this->get_image(buf_x, buf_y, temp_image);
	PDraw::image_delete(temp_image);

	// TODO
	for ( uint i = 0; i < 256; i++ )
		charlist[i] = -1;
	
	for ( uint i = 0; i < char_count; i++ )
		charlist[(u8)(chars[i]&~' ')] = i * char_w;

	return 0;

}

int PFont::write(int posx, int posy, const char *text) {
	
	int i = 0;
	int ix, ox = posx;
	char curr_char;

	PDraw::RECT srcrect, dstrect;
	srcrect.y = 0;
	srcrect.w = char_w;
	srcrect.h = char_h;
	dstrect.y = posy;

	do {
		curr_char = text[i];
		ix = charlist[(u8)(curr_char&~' ')];
		if (ix > -1) {
			srcrect.x = ix;
			dstrect.x = ox;
			PDraw::image_cutclip(image_index,srcrect,dstrect);
		}
		ox += char_w;
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

	int w, h;
	PDraw::get_buffer_size(&w, &h);

	if (alpha > 100) alpha = 100;
	if (alpha < 0) alpha = 0;
	int a1 = alpha;
	int a2 = 100 - alpha;

	PDraw::drawscreen_start(back_buffer, back_w);
	PDraw::drawimage_start(image_index, txt_buffer, txt_w);

	do {
		curr_char = text[i];
		int ix = charlist[(u8)(curr_char&~' ')];
		if (ix > -1){
			for (uint x = 0; x < char_w; x++) {
				
				int fx = posx + x + i * char_w;
				if(fx < 0 || fx >= w) break;

				for (uint y = 0; y < char_h; y++) {
					
					int fy = posy + y;
					if (fy < 0 || fy >= h) break;


					color1 = txt_buffer[ix + x + y * txt_w];

					if (color1 != 255) {
					
						// Mix colors
						color1 &= (u8)0b00011111;
						color2 = back_buffer[fx + fy * back_w];
						color3 = color2 & (u8)0b11100000;
						color2-= color3;
						color1 = (color1 * a1 + color2 * a2)/100;

						back_buffer[fx + fy * back_w] = color1 + color3;

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
