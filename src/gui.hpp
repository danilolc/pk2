//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

extern int Gui_pad_button;

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
	UI_NONE,
	UI_TOUCH_TO_START,
	UI_CURSOR,
	UI_GAME_BUTTONS
};

void GUI_Change(int ui_mode);
void GUI_Load();

void GUI_Reset();
void GUI_Update();
void GUI_Exit();