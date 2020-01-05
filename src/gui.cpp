//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "gui.hpp"

#include "settings.hpp"

#include "engine/PGui.hpp"
#include "engine/PInput.hpp"

int gui_touch  = -1;
int gui_padbg  = -1;
int gui_padbt  = -1;
int gui_up     = -1;
int gui_down   = -1;
int gui_egg    = -1;
int gui_doodle = -1;
int gui_menu   = -1;
int gui_gift   = -1;
int gui_tab    = -1;

void Game_GUI(bool set){

	PGui::activate(gui_menu, set);

	PGui::activate(gui_padbg, set);
	PGui::activate(gui_padbt, set);

	PGui::activate(gui_up, set);
	PGui::activate(gui_down, set);
	PGui::activate(gui_doodle, set);
	PGui::activate(gui_egg, set);
	PGui::activate(gui_gift, set);
	PGui::activate(gui_tab, set);

}

void GUI_Change(int ui_mode) {

	switch(ui_mode){
		case UI_TOUCH_TO_START:
			PGui::activate(gui_touch, true);
			Game_GUI(false);
		break;
		case UI_CURSOR:
			PGui::activate(gui_touch, false);
			Game_GUI(false);
		break;
		case UI_GAME_BUTTONS:
			PGui::activate(gui_touch, true);
			Game_GUI(true);
		break;
	}

}

void GUI_Load() {

	static u32 escape = PInput::ESCAPE;
	static u32 tab    = PInput::TAB;
	static u32 enter  = PInput::RETURN;

	int w = 255 * 0.8;
	int h = 214 * 0.8;
	int alpha = 140;

	gui_touch = PGui::create(0,0,1920,1080,alpha,"", &enter);

	gui_menu = PGui::create(50,130,w,h,alpha,"mobile/menu.png", &escape);
	
	gui_padbg = PGui::create(50,620,641*0.9,362*0.9,alpha,"mobile/padbg.png", nullptr);
	gui_padbt = PGui::create(275,670,169*0.9,173*0.9,180,"mobile/padbt.png", nullptr);

	int y = 670;
	const int dy = 190;

	gui_up =     PGui::create(1630, y,w,h,alpha,"mobile/up.png", &Settings.control_jump);
	y -= dy;
	gui_doodle = PGui::create(1630, y,w,h,alpha,"mobile/doodle.png", &Settings.control_attack2);
	y -= dy;
	gui_gift =   PGui::create(1630, y,w,h,alpha,"mobile/gift.png", &Settings.control_open_gift);
	
	y = 720;
	gui_down =   PGui::create(1400, y,w,h,alpha,"mobile/down.png", &Settings.control_down);
	y -= dy;
	gui_egg =    PGui::create(1400, y,w,h,alpha,"mobile/egg.png", &Settings.control_attack1);

	gui_tab =    PGui::create(   0, 930,530,150,alpha,"", &tab);

}

void GUI_Update() {



	
}