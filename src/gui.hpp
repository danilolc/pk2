//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

extern int Gui_pad;

extern bool Gui_up;
extern bool Gui_down;
extern bool Gui_egg;
extern bool Gui_doodle;
extern bool Gui_gift;
extern bool Gui_tab;

extern bool Gui_menu;
extern bool Gui_touch;
extern bool Gui_tab;

enum UI_MODE {
	UI_TOUCH_TO_START,
	UI_CURSOR,
	UI_GAME_BUTTONS
};

void GUI_Change(int ui_mode);
void GUI_Load();

void GUI_Update();
void GUI_Exit();