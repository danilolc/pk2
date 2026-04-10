//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "gfx/text.hpp"

#include "system.hpp"
#include "game/game.hpp"
#include "settings/settings.hpp"

#include "engine/PLog.hpp"
#include "engine/PDraw.hpp"
#include "engine/PFilesystem.hpp"

#include <cstring>
#include <list>
#include <stdexcept>

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


static void mLoadFont(PLang*lang, int& font, const std::string& langID, const std::string& fallbackName){
	int ind_font = lang->Search_Id(langID.c_str());

	const std::string& fontName = ind_font==-1 ? fallbackName: lang->Get_Text(ind_font);

	std::optional<PFile::Path> path = PFilesystem::FindVanillaAsset(fontName, PFilesystem::FONTS_DIR);
	if(!path.has_value()){
		throw std::runtime_error("Font picture: \""+fontName+"\" not found!");
	}

	font = PDraw::font_create(*path);
	if(font == -1){
		throw std::runtime_error("Can't create font from \""+fontName+"\"!");
	}
}



void Load_Fonts(PLang* lang) {
	PDraw::clear_fonts();

	mLoadFont(lang, fontti1, "font small font", "ScandicSmall.txt");
	mLoadFont(lang, fontti2, "font big font normal", "ScandicBig1.txt");
	mLoadFont(lang, fontti3, "font big font hilite", "ScandicBig2.txt");
	mLoadFont(lang, fontti4, "font big font shadow", "ScandicBig3.txt");
}

void CreditsText_Draw(const std::string& text, int font, int x, int y, u32 start, u32 end, u32 time){
	int pros = 100;
	if (time > start && time < end) {

		if (time - start < 100)
			pros = time - end;

		if (end - time < 100)
			pros = end - time;

		if (pros > 0) {
			if (pros < 100)
				PDraw::font_writealpha_s(font,text,x,y,pros);
			else
				PDraw::font_write_line(font,text,x,y);
		}

	}
}

void CreditsText_Draw_Centered(const std::string& text, int font, int y, u32 start, u32 end, u32 time){
	std::pair<int, int> text_size = PDraw::font_get_text_size(font, text);
	int x = 320 - text_size.first/2;
	CreditsText_Draw(text, font, x, y, start, end, time);
}


int WavetextLap_Draw(const char *text, int fontti, int x, int y, float lap) {

	int pos = 0;
	int i = 0;
	PString::UTF8_Char u8c;

	while(*text!='\0'){
		text = u8c.read(text);
		int ys = (int)(sin_table((i+degree)*8))/(7.f/(lap*0.8));
		int xs = (int)(cos_table((i+degree)*8))/(9.f/(lap*2.5));

		PDraw::font_write(fontti4, u8c.c_str(), x+pos-xs+3,y+ys+3);
		pos += PDraw::font_write_line(fontti, u8c.c_str(), x+pos-xs,y+ys);
		++i;
	}

	return pos;

}

int Wavetext_Draw(const char *text, int fontti, int x, int y) {

	int pos = 0;
	int i = 0;
	PString::UTF8_Char u8c;

	while(*text!='\0'){
		text = u8c.read(text);

		int ys = (int)(sin_table((i+degree)*8))/7;
		int xs = (int)(cos_table((i+degree)*8))/9;

		PDraw::font_write(fontti4,u8c.c_str(),x+pos-xs+3,y+ys+3);
		pos += PDraw::font_write_line(fontti,u8c.c_str(),x+pos-xs,y+ys);
		++i;
	}

	return pos;

}

int WavetextSlow_Draw(const char *text, int fontti, int x, int y) {


	int pos = 0;
	int i = 0;
	PString::UTF8_Char u8c;

	while(*text!='\0'){
		text = u8c.read(text);

		int ys = (int)(sin_table((i+degree)*4))/9;
		int xs = (int)(cos_table((i+degree)*4))/11;

		if (Settings.transparent_text) {
			auto[width, _] = PDraw::font_writealpha_s(fontti,u8c.c_str(),x+pos-xs,y+ys,75, 1);
			pos += width;
		
		} else {

			PDraw::font_write(fontti4,u8c.c_str(),x+pos-xs+1,y+ys+1);
			pos += PDraw::font_write_line(fontti,u8c.c_str(),x+pos-xs,y+ys);
		
		}
		++i;
	}

	return pos;

}

int ShadowedText_Draw(const std::string& text, int x, int y) {

	PDraw::font_write(fontti4, text, x + 2, y + 2);
	return PDraw::font_write_line(fontti2, text, x, y);

}

void Fadetext_Init(){

	fadetekstit.clear();

}

void Fadetext_New(int font, const std::string&text , u32 x, u32 y, u32 timer) {

	PK2FADETEXT tt;
	
	tt.fontti = font;
	strncpy(tt.teksti, text.c_str(), 19);
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

			if (pros < 100)
				PDraw::font_writealpha_s(text.fontti, text.teksti, x, y, pros);
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