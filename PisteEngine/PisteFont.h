#ifndef P_FONT
#define P_FONT

#include "D:\Visual Studio\MyProjects\PisteEngine\PisteDraw.h"
#include "D:\Visual Studio\MyProjects\PisteLanguage\PisteLanguage.h"

class PisteFont
{
private:
	int font_table[256];
	int	font_korkeus, font_leveys, font_lkm;
	PisteLanguage *tiedosto;

public:
	UCHAR *font_bitmap;
	int font_buffer;

	int Init_fonts(void);
	int Init_fonts_tiedosto(void);
	int Get_bitmap(int buffer_x, int buffer_y, int ruudun_leveys,UCHAR *buffer);
	
	int Piirra_merkkijono(char *merkkijono, int font_x, int font_y, int kohde_buffer);

	int Piirra_merkkijono(int font_x, int font_y, int lPitch, char *merkkijono, 
		UCHAR *back_buffer, bool loop);
	int Piirra_merkkijono_led(int font_x, int font_y, int lPitch, char *merkkijono, 
		UCHAR *back_buffer);
	int Piirra_merkkijono_lapinakyva(int font_x, int font_y, int lPitch, RECT klipperi, char *merkkijono,
		UCHAR *back_buffer, bool loop, int pros);
	int Piirra_merkkijono_varillinen(int font_x, int font_y, int lPitch, char *merkkijono,
		UCHAR *back_buffer, bool loop, UCHAR color2);
	int Korkeus(void){return font_korkeus;};

	int LataaTiedostosta(char *polku, char *file);

	PisteFont(int korkeus, int leveys, int lkm);
	PisteFont();
	~PisteFont();


};

#endif