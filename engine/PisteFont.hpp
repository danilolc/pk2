//#########################
//PisteEngine - PisteFont
//by Janne Kivilahti from Piste Gamez
//#########################

#pragma once

#include "platform.hpp"
//#include "PisteDraw.hpp"

namespace Piste {

class Font{
private:
	int char_list[256];
	int char_w, char_h, char_count;
	int image_index = -1;
	int init_char_list();
	int get_image(int x,int y,int img_source);

public:
	int write(int posx, int posy, const char *text);
	int write_trasparent(int posx, int posy, const char* text, int alpha);
	
	Font(int img_source, int x, int y, int width, int height, int count);
	Font();
	~Font();

	int load(const char* file_path, const char* file);
};


}
class PisteFont2{
private:
	int charList[256];
	int char_w, char_h, char_count;
	int ImageIndex = -1;
	int InitCharList();
	int GetImage(int x,int y,int img_source);

public:
	int Write_Text(int posx, int posy, const char *text);
	int Write_TextTrasparent(int posx, int posy, const char* text, int alpha);
	int LoadFile(const char* file_path, const char* file);

	PisteFont2(int img_source, int x, int y, int width, int height, int count);
	PisteFont2();
	~PisteFont2();
};
