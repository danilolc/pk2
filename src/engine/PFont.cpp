//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "PFont.hpp"

#include "PDraw.hpp"
#include "PLang.hpp"
#include "types.hpp"
#include "PFilesystem.hpp"

#include <cmath>
#include <cstring>


void PFont::initCharlist() {

	const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "###" "0123456789.!?:-.+=()/#\\_%🌜";
	this->initCharlist(chars);
}


void PFont::initCharlist(const char* letters){
	this->utf8_charlist.clear();

	int index_counter = 0;
	const char * str = letters;
	PString::UTF8_Char u8c;

	while(*str!='\0'){
		str = u8c.read(str);
		this->utf8_charlist.emplace_back(std::make_pair(index_counter, u8c));

		PString::UTF8_Char lower = PString::lowercase(u8c);
		PString::UTF8_Char upper = PString::uppercase(u8c);
		if(u8c!=lower){
			this->utf8_charlist.emplace_back(std::make_pair(index_counter, lower));
		}
		else if(u8c!=upper){
			this->utf8_charlist.emplace_back(std::make_pair(index_counter, upper));
		}

		++index_counter;
	}
}

int PFont::get_image(int x, int y, int img_source) {

	image_index = PDraw::image_cut(img_source, x, y, char_w * char_count, char_h * char_count);
	return 0;

}

int PFont::load(PFile::Path path) {

	int i = 0;

	PLang param_file = PLang();

	if (!param_file.Read_File(path))
		return -1;

	//i = param_file.Search_Id("image width");
	//int buf_width = atoi(param_file.Get_Text(i));

	i = param_file.Search_Id("image x");
	int buf_x = atoi(param_file.Get_Text(i).c_str());

	i = param_file.Search_Id("image y");
	int buf_y = atoi(param_file.Get_Text(i).c_str());

	i = param_file.Search_Id("letter width");
	this->char_w = atoi(param_file.Get_Text(i).c_str());

	i = param_file.Search_Id("letter height");
	this->char_h = atoi(param_file.Get_Text(i).c_str());

	i = param_file.Search_Id("letters");
	if(i!=-1){
		this->initCharlist(param_file.Get_Text(i).c_str());
	}
	else{
		this->initCharlist();
	}

	i = param_file.Search_Id("line spacing");
	if(i!=-1){
		this->lineSep = param_file.getInteger(i, 0);
	}
	//this->initCharlist();

	i = param_file.Search_Id("image");

	std::optional<PFile::Path> imagePath = PFilesystem::FindVanillaAsset(param_file.Get_Text(i), PFilesystem::FONTS_DIR);
	if (!imagePath.has_value()){
		//TODO Add exception here
		return -1;
	}
		

	int temp_image = PDraw::image_load(*imagePath, true);
	if (temp_image == -1) return -1;

	this->get_image(buf_x, buf_y, temp_image);
	PDraw::image_delete(temp_image);

	

	return 0;

}

int PFont::getCharacterPos(const PString::UTF8_Char& u8c)const{
	for(const std::pair<int, PString::UTF8_Char>& p: this->utf8_charlist){
		if(p.second==u8c){
			return p.first * this->char_w;
		}
	}

	return -1;
}

std::pair<int, int> PFont::getTextSize(const char *text)const {
	
	int textWidth = 0;
	int textHeight = this->char_h;
	int width = 0;

	const char* curr_char = text;
	PString::UTF8_Char u8c;
	while (*curr_char!='\0'){
		if(*curr_char == '\n'){
			++curr_char;
			if(width > textWidth){
				textWidth = width;
			}

			width = 0;
			textHeight += this->lineSep;
			textHeight += this->char_h;
		}
		else{
			curr_char = u8c.read(curr_char);
			width += this->char_w;
		}		
	}

	if(width > textWidth){
		textWidth = width;
	}

	return std::make_pair(textWidth, textHeight);
}

int PFont::write_line(int posx, int posy, const char *text) {
	
	int ox = posx;

	PDraw::RECT srcrect, dstrect;
	srcrect.y = 0;
	srcrect.w = char_w;
	srcrect.h = char_h;
	dstrect.y = posy;

	dstrect.w = char_w;
	dstrect.h = char_h;

	int charsNumber = 0;
	const char* curr_char = text;
	PString::UTF8_Char u8c;
	while (*curr_char!='\0'){
		curr_char = u8c.read(curr_char);
		int ix = this->getCharacterPos(u8c);
		if(ix!=-1){
			srcrect.x = ix;
			dstrect.x = ox;
			PDraw::image_cutclip(image_index,srcrect,dstrect);
		}

		ox += char_w;
		charsNumber+=1;
	}
	return char_w * charsNumber;
}

std::pair<int,int> PFont::write(int posx, int posy, const char *text)const {
	
	int ox = posx;

	PDraw::RECT srcrect, dstrect;
	srcrect.y = 0;
	srcrect.w = char_w;
	srcrect.h = char_h;
	dstrect.y = posy;

	dstrect.w = char_w;
	dstrect.h = char_h;


	int textWidth = 0;
	int textHeight = this->char_h;
	int width = 0;

	PString::UTF8_Char u8c;
	while (*text!='\0'){

		if(*text=='\n'){
			++text;

			/**
			 * @brief 
			 * carriage return 
			 */
			if(width > textWidth){
				textWidth = width;
			}
			width=0;
			ox = posx;

			/**
			 * @brief 
			 * New line
			 */
			dstrect.y += this->lineSep;
			dstrect.y += this->char_h;

			textHeight += this->lineSep;
			textHeight += this->char_h;
		}
		else{
			text = u8c.read(text);
			int ix = this->getCharacterPos(u8c);
			if(ix!=-1){
				srcrect.x = ix;
				dstrect.x = ox;
				PDraw::image_cutclip(image_index,srcrect,dstrect);
			}

			ox += this->char_w;
			width += this->char_w;
		}
	}
	if(width > textWidth){
		textWidth = width;
	}
	
	return std::make_pair(textWidth, textHeight);
}

std::pair<int,int> PFont::write_trasparent(int posx, int posy, const char* text, int alpha, int blendMode)const {

	u8 *back_buffer, *txt_buffer;
	u32 back_w, txt_w;

	u8 color1, color2, color3;

	int w, h;
	PDraw::get_buffer_size(&w, &h);

	if (alpha > 100) alpha = 100;
	if (alpha < 0) alpha = 0;
	int a1 = alpha;
	int a2 = 100 - alpha;

	PDraw::drawscreen_start(back_buffer, back_w);
	PDraw::drawimage_start(image_index, txt_buffer, txt_w);


	PString::UTF8_Char u8c;

	int textWidth = 0;
	int textHeight = this->char_h;

	int width = 0;

	while (*text!='\0'){
		if(*text=='\n'){
			++text;

			/**
			 * @brief 
			 * carriage return 
			 */
			if(width > textWidth){
				textWidth = width;
			}
			width=0;
			//ox = posx;

			/**
			 * @brief 
			 * New line
			 */
			posy += this->lineSep;
			posy += this->char_h;
			textHeight += this->lineSep;
			textHeight += this->char_h;		
		}
		else{

			text = u8c.read(text);
			int ix = this->getCharacterPos(u8c);
			if(ix!=-1){

				for (uint x = 0; x < char_w; x++) {
					
					int fx = posx + x + width;
					if(fx < 0 || fx >= w) break;

					for (uint y = 0; y < char_h; y++) {
						
						int fy = posy + y;
						if (fy < 0 || fy >= h) break;


						color1 = txt_buffer[ix + x + y * txt_w];

						if (color1 != 255) {

							if(blendMode==0){
								// Mix colors
								color1 &= (u8)0b00011111;
								color2 = back_buffer[fx + fy * back_w];
								color3 = color2 & (u8)0b11100000;
								color2-= color3;
								color1 = (color1 * a1 + color2 * a2)/100;

								back_buffer[fx + fy * back_w] = color1 + color3;
							}
							else if(blendMode==1){

								if((color1 >> 5) == 2){
									color1 -= 2;
								}

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
			}
			width+=this->char_w;
		}
	}
	
	PDraw::drawscreen_end();
	PDraw::drawimage_end(image_index);

	if(width > textWidth){
		textWidth = width;
	}

	return std::make_pair(textWidth, textHeight);
}

PFont::PFont(int img_source, int x, int y, int width, int height, int count) {

	char_w = width;
	char_h = height;
	char_count = count;

	this->get_image(x, y, img_source);
	//this->initCharlist();

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


bool PFont::acceptChar(PString::UTF8_Char u8c)const{

	for(const std::pair<int, PString::UTF8_Char>&p:this->utf8_charlist){
		if(u8c==p.second)return true;
	}

	return false;
}