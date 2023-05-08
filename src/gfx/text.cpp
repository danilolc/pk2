//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "gfx/text.hpp"

#include "system.hpp"
#include "game/game.hpp"
#include "settings.hpp"

#include "engine/PLog.hpp"
#include "engine/PDraw.hpp"

#include <cstring>
#include <iostream>
#include <list>

struct PK2FADETEXT {
	char teksti[20];
	int fontti;
	int x,y,timer;
};

std::list<PK2FADETEXT> fadetekstit;

//Fonts
int fontti1 = -1;
int fontti2 = -1;
int fontti3 = -1;
int fontti4 = -1;
int fontti5 = -1;

int Load_Fonts(PLang* lang) {

	PDraw::clear_fonts();

	PFile::Path fonts_path("language" PE_SEP "fonts" PE_SEP);
	PFile::Path index_path("");

	int ind_path = lang->Search_Id("font path");
	if (ind_path != -1) {

		index_path = PFile::Path(lang->Get_Text(ind_path));
		PLog::Write(PLog::DEBUG, "PK2", "Fonts on %s", index_path.c_str());

	}

	int ind_font = lang->Search_Id("font small font");
	if (ind_path == -1 || ind_font == -1) {

		fonts_path.SetFile("ScandicSmall.txt");
        fontti1 = PDraw::font_create(fonts_path);
		if (fontti1 == -1) {

			PLog::Write(PLog::ERR, "PK2", "Can't create font 1 from ScandicSmall.txt");
			PDraw::clear_fonts();
			return -1;

		}
		
	} else {

		index_path.SetFile(lang->Get_Text(ind_font));
        fontti1 = PDraw::font_create(index_path);
		if (fontti1 == -1) {

			PLog::Write(PLog::ERR, "PK2", "Can't create font 1");
			PDraw::clear_fonts();
			return -1;

		}
	
	}

	ind_font = lang->Search_Id("font big font normal");
	if (ind_path == -1 || ind_font == -1) {

		fonts_path.SetFile("ScandicBig1.txt");
        fontti2 = PDraw::font_create(fonts_path);
		if (fontti2 == -1) {

			PLog::Write(PLog::ERR, "PK2", "Can't create font 2 from ScandicBig1.txt");
			PDraw::clear_fonts();
			return -1;

		}
		
	} else {

		index_path.SetFile(lang->Get_Text(ind_font));
        fontti2 = PDraw::font_create(index_path);
		if (fontti2 == -1) {

			PLog::Write(PLog::ERR, "PK2", "Can't create font 2");
			PDraw::clear_fonts();
			return -1;

		}
	}

	ind_font = lang->Search_Id("font big font hilite");
	if (ind_path == -1 || ind_font == -1) {

		fonts_path.SetFile("ScandicBig2.txt");
        fontti3 = PDraw::font_create(fonts_path);
		if (fontti3 == -1) {

			PLog::Write(PLog::ERR, "PK2", "Can't create font 3 from ScandicBig2.txt");
			PDraw::clear_fonts();
			return -1;

		}

	} else {

		index_path.SetFile(lang->Get_Text(ind_font));
        fontti3 = PDraw::font_create(index_path);
		if (fontti3 == -1) {

			PLog::Write(PLog::ERR, "PK2", "Can't create font 3");
			PDraw::clear_fonts();
			return -1;

		}
	}

	ind_font = lang->Search_Id("font big font shadow");
	if (ind_path == -1 || ind_font == -1) {

		fonts_path.SetFile("ScandicBig3.txt");
        fontti4 = PDraw::font_create(fonts_path);
		if (fontti4 == -1) {

			PLog::Write(PLog::ERR, "PK2", "Can't create font 4 from ScandicBig3.txt");
			PDraw::clear_fonts();
			return -1;

		}
	
	} else {

		index_path.SetFile(lang->Get_Text(ind_font));
        fontti4 = PDraw::font_create(index_path);
		if (fontti4 == -1) {

			PLog::Write(PLog::ERR, "PK2", "Can't create font 4");
			PDraw::clear_fonts();
			return -1;
			
		}

	}

	return 0;
}

int CreditsText_Draw(const char *text, int font, int x, int y, u32 start, u32 end, u32 time){
	int pros = 100;
	if (time > start && time < end) {

		if (time - start < 100)
			pros = time - end;

		if (end - time < 100)
			pros = end - time;

		if (pros > 0) {
			if (pros < 100)
				PDraw::font_writealpha(font,text,x,y,pros);
			else
				PDraw::font_write(font,text,x,y);
		}

	}
	return 0;
}

int WavetextLap_Draw(const char *text, int fontti, int x, int y, float lap, char end) {

	int pos = 0;
	char kirjain[2] = " ";

	for (int i = 0; text[i] != '\0' && text[i] != end; i++) {

		int ys = (int)(sin_table((i+degree)*8))/(7.f/(lap*0.8));
		int xs = (int)(cos_table((i+degree)*8))/(9.f/(lap*2.5));
		kirjain[0] = text[i];
		
		PDraw::font_write(fontti4,kirjain,x+pos-xs+3,y+ys+3);
		pos += PDraw::font_write(fontti,kirjain,x+pos-xs,y+ys);
	
	}

	return pos;

}

int Wavetext_Draw(const char *text, int fontti, int x, int y, char end) {

	int pos = 0;
	char kirjain[2] = " ";

	for (int i = 0; text[i] != '\0' && text[i] != end; i++) {

		int ys = (int)(sin_table((i+degree)*8))/7;
		int xs = (int)(cos_table((i+degree)*8))/9;
		kirjain[0] = text[i];
		
		PDraw::font_write(fontti4,kirjain,x+pos-xs+3,y+ys+3);
		pos += PDraw::font_write(fontti,kirjain,x+pos-xs,y+ys);
	
	}

	return pos;

}

int WavetextSlow_Draw(const char *text, int fontti, int x, int y, char end) {
	
	int pos = 0;
	char kirjain[2] = " ";

	for (int i = 0; text[i] != '\0' && text[i] != end; i++) {
		
		int ys = (int)(sin_table((i+degree)*4))/9;
		int xs = (int)(cos_table((i+degree)*4))/11;
		kirjain[0] = text[i];

		if (Settings.transparent_text) {
		
			pos += PDraw::font_writealpha(fontti,kirjain,x+pos-xs,y+ys,75);
		
		} else {

			PDraw::font_write(fontti4,kirjain,x+pos-xs+1,y+ys+1);
			pos += PDraw::font_write(fontti,kirjain,x+pos-xs,y+ys);
		
		}
	}

	return pos;

}

int ShadowedText_Draw(const char* text, int x, int y) {

	PDraw::font_write(fontti4, text, x + 2, y + 2);
	return PDraw::font_write(fontti2, text, x, y);

}

void Fadetext_Init(){

	fadetekstit.clear();

}

void Fadetext_New(int fontti, char *teksti, u32 x, u32 y, u32 timer) {

	PK2FADETEXT tt;
	
	tt.fontti = fontti;
	strcpy(tt.teksti,teksti);
	tt.x = x;
	tt.y = y;
	tt.timer = timer;
	
	fadetekstit.push_back(tt);

}

int Fadetext_Draw(){
	int pros;

	for (PK2FADETEXT& text : fadetekstit) {
		if (text.timer > 0) {
			if (text.timer > 50)
				pros = 100;
			else
				pros = text.timer * 2;

			int x = text.x - Game->camera_x;
			int y = text.y - Game->camera_y;

			if (Settings.draw_transparent && pros < 100)
				PDraw::font_writealpha(text.fontti, text.teksti, x, y, pros);
			else
				PDraw::font_write(text.fontti, text.teksti, x, y);

		}
	}
	
	return 0;
}

static bool update_done (PK2FADETEXT& t) { 

	if (t.timer > 0){
		t.timer--;

		if (t.timer%2 == 0)
			t.y--;
	}
	return t.timer == 0; 

}

void Fadetext_Update() {

	fadetekstit.remove_if(update_done);

}