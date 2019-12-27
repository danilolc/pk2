//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "gui.hpp"

#include "settings.hpp"

#include "PisteGui.hpp"
#include "PisteInput.hpp"

int gui_touch  = -1;
int gui_egg    = -1;
int gui_doodle = -1;
int gui_arr    = -1;
int gui_up     = -1;
int gui_down   = -1;
int gui_left   = -1;
int gui_right  = -1;
int gui_menu   = -1;
int gui_gift   = -1;
int gui_tab    = -1;

void GUI_Activate(bool set){

	PGui::activate(gui_menu, set);
	PGui::activate(gui_arr, set);
	PGui::activate(gui_left, set);
	PGui::activate(gui_right, set);
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
			GUI_Activate(false);
		break;
		case UI_CURSOR:
			PGui::activate(gui_touch, false);
			GUI_Activate(false);
		break;
		case UI_GAME_BUTTONS:
			PGui::activate(gui_touch, true);
			GUI_Activate(true);
		break;
	}
}

void GUI_Load() {

	static DWORD escape = PI_ESCAPE;
	static DWORD tab    = PI_TAB;
	static DWORD enter  = PI_RETURN;

	int circ_size = 200;
	int alpha = 170;

	gui_touch = PGui::create(0,0,1920,1080,alpha,"", &enter);

	gui_menu = PGui::create(50,130,circ_size,circ_size,alpha,"mobile/menu.png", &escape);
	gui_arr = PGui::create(50,650,388,256,alpha,"mobile/arrow.png", nullptr);
	gui_left = PGui::create(50,650,388/2,256,alpha,"", &Settings.control_left);
	gui_right = PGui::create(50+388/2,650,388/2,256,alpha,"",  &Settings.control_right);
	gui_up = PGui::create(1630,650,circ_size,circ_size,alpha,"mobile/up.png", &Settings.control_jump);
	gui_down = PGui::create(1420,700,circ_size,circ_size,alpha,"mobile/down.png", &Settings.control_down);
	gui_doodle = PGui::create(1630,450,circ_size,circ_size,alpha,"mobile/doodle.png", &Settings.control_attack2);
	gui_egg = PGui::create(1420,500,circ_size,circ_size,alpha,"mobile/egg.png", &Settings.control_attack1);
	gui_gift = PGui::create(1630,250,circ_size,circ_size,alpha,"mobile/gift.png", &Settings.control_open_gift);
	gui_tab = PGui::create(0,930,530,150,alpha,"", &tab);

}