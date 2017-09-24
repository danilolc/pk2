//#########################
//PisteEngine - PisteFont
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef P_FONT
#define P_FONT

#include "PisteDraw.h"

class PisteFont2{
private:
	int charList[256];
	int char_w, char_h, char_count;
	int ImageIndex;
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

#endif
