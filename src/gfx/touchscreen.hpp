//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once
#include <vector>
#include "engine/PRender.hpp"

enum UI_MODE {
	UI_NONE,
	UI_TOUCH_TO_START,
	UI_CURSOR,
	UI_GAME_BUTTONS
};

class PK2TouchScreenButton{
public:
	PK2TouchScreenButton()=default;
	PK2TouchScreenButton(PRender::RECT src, PRender::RECT dst, u8 alpha);
	void draw()const;
    PRender::RECT src, dst;
    u8 alpha = 0;


	bool active = false;
	int alpha_counter = 0;

	void update();

};

class PK2TouchScreenControls{
public:
	int pad_button = 2;
	
	bool up = false;
	bool down = false;
	bool egg = false;
	bool doodle = false;
	bool gift = false;
	bool menu = false;
	bool touch = false;
	bool tab = false;

	bool any = false;

	void change(int ui_mode);
	void load();
	void reset();
	void update();
private:
	bool readGui(const PK2TouchScreenButton& gui);
	float holdPad(float pos_x, int* button);
	int getPad();

	PK2TouchScreenButton gui_padbg;
	PK2TouchScreenButton gui_padbt;

	PK2TouchScreenButton gui_up;
	PK2TouchScreenButton gui_down;
	PK2TouchScreenButton gui_egg;
	PK2TouchScreenButton gui_doodle;
	PK2TouchScreenButton gui_gift;

	PK2TouchScreenButton gui_menu;
	PK2TouchScreenButton gui_touch;
	PK2TouchScreenButton gui_tab;

	bool loaded = false;

	int UI_mode = UI_NONE;

	int pad_id = -1;
	bool pad_grab = false;

	//int doodle_alpha = 0, egg_alpha = 0, gift_alpha = 0; 
    //bool doodle_active = false, egg_active = false, gift_active = false;
};

extern PK2TouchScreenControls TouchScreenControls;