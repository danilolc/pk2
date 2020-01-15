//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "gui.hpp"

#include "settings.hpp"

#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"

PDraw::Gui* gui_touch  = nullptr;
PDraw::Gui* gui_padbg  = nullptr;
PDraw::Gui* gui_padbt  = nullptr;
PDraw::Gui* gui_up     = nullptr;
PDraw::Gui* gui_down   = nullptr;
PDraw::Gui* gui_egg    = nullptr;
PDraw::Gui* gui_doodle = nullptr;
PDraw::Gui* gui_menu   = nullptr;
PDraw::Gui* gui_gift   = nullptr;
PDraw::Gui* gui_tab    = nullptr;

void Game_GUI(bool set){

	gui_menu->active = set;

	gui_padbg->active = set;
	gui_padbt->active = set;

	gui_up->active = set;
	gui_down->active = set;
	gui_doodle->active = set;
	gui_egg->active = set;
	gui_gift->active = set;
	gui_tab->active = set;

}

void GUI_Change(int ui_mode) {

	switch(ui_mode){
		case UI_TOUCH_TO_START:
			gui_touch->active = true;
			Game_GUI(false);
		break;
		case UI_CURSOR:
			gui_touch->active = false;
			Game_GUI(false);
		break;
		case UI_GAME_BUTTONS:
			gui_touch->active = true;
			Game_GUI(true);
		break;
	}

}

void GUI_Load() {

	static u32 escape = PInput::ESCAPE;
	static u32 tab    = PInput::TAB;
	static u32 enter  = PInput::RETURN;

	int w = 230 * 0.8;
	int h = 220 * 0.8;
	int alpha = 140;

	PFile::Path path("mobile" PE_SEP);

	gui_touch =  PDraw::create_gui(path, 0,0,1920,1080,alpha);

	path.SetFile("menu.png");
	gui_menu =   PDraw::create_gui(path, 50,130,w,h,alpha);
	
	path.SetFile("padbg.png");
	gui_padbg =  PDraw::create_gui(path, 50,620,641*0.9,362*0.9,alpha);

	path.SetFile("padbt.png");
	gui_padbt =  PDraw::create_gui(path, 275,670,169*0.9,173*0.9,180);

	int y = 650;
	const int dy = 200;

	path.SetFile("up.png");
	gui_up =     PDraw::create_gui(path, 1630, y,w,h,alpha);

	y -= dy;
	path.SetFile("doodle.png");
	gui_doodle = PDraw::create_gui(path, 1630, y,w,h,alpha);

	y -= dy;
	path.SetFile("gift.png");
	gui_gift =   PDraw::create_gui(path, 1630, y,w,h,alpha);

	y = 720;
	path.SetFile("down.png");
	gui_down =   PDraw::create_gui(path, 1410, y,w,h,alpha);

	y -= dy;
	path.SetFile("egg.png");
	gui_egg =    PDraw::create_gui(path, 1410, y,w,h,alpha);

	path.SetFile("");
	gui_tab =    PDraw::create_gui(path,    0, 930,530,150, alpha);

}

void GUI_Update() {



	
}