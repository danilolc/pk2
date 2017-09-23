//#########################
//PisteEngine - PisteFont
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef P_FONT
#define P_FONT

#include "PisteDraw.h"
#include "PisteLanguage.h"

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
	int LoadFile(const char* path, const char* file);

	PisteFont2(int img_source, int x, int y, int width, int height, int count);
	PisteFont2();
	~PisteFont2();
};









class PisteFont
{
	private:
		int font_table[256];
		int	font_korkeus, font_leveys, font_lkm;
		PisteLanguage* param_file;

	public:
		BYTE *font_bitmap;
		int font_buffer;

		int Init_fonts(void);
		int Init_fonts_tiedosto(void);
		int Get_bitmap(int buffer_x, int buffer_y, int ruudun_leveys, int buffer_index);
		int Get_bitmap(int buffer_x, int buffer_y, int ruudun_leveys, BYTE *buffer);

		int Piirra_merkkijono(char *merkkijono, int font_x, int font_y, int kohde_buffer);

		int Piirra_merkkijono(int font_x, int font_y, int lPitch, char *merkkijono,
			BYTE *back_buffer, bool loop);
		int Piirra_merkkijono_led(int font_x, int font_y, int lPitch, char *merkkijono,
			BYTE *back_buffer);
		int Piirra_merkkijono_lapinakyva(char *merkkijono, int font_x, int font_y, int kohde_buffer, int alpha);
		int Piirra_merkkijono_varillinen(int font_x, int font_y, int lPitch, char *merkkijono,
			BYTE *back_buffer, bool loop, BYTE color2);
		int Korkeus(void){return font_korkeus;};

		int LataaTiedostosta(char *polku, char *file);

		PisteFont(int korkeus, int leveys, int lkm);
		PisteFont();
		~PisteFont();
};

#endif
