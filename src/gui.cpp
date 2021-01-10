//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "gui.hpp"

#include "settings.hpp"
#include "game/gifts.hpp"
#include "game/sprites.hpp"

#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"

#include <cmath>

int Gui_pad = 0;

//  0 1 2 3 4
// << < | > >>
int Gui_pad_button = 2;

bool Gui_up = false;
bool Gui_down = false;
bool Gui_egg = false;
bool Gui_doodle = false;
bool Gui_gift = false;

bool Gui_menu = false;
bool Gui_touch = false;
bool Gui_tab = false;


static PDraw::Gui* gui_padbg  = nullptr;
static PDraw::Gui* gui_padbt  = nullptr;

static PDraw::Gui* gui_up     = nullptr;
static PDraw::Gui* gui_down   = nullptr;
static PDraw::Gui* gui_egg    = nullptr;
static PDraw::Gui* gui_doodle = nullptr;
static PDraw::Gui* gui_gift   = nullptr;

static PDraw::Gui* gui_menu   = nullptr;
static PDraw::Gui* gui_touch  = nullptr;
static PDraw::Gui* gui_tab    = nullptr;


const static int PadBt_x = 315;
const static int PadBt_y = 675;

const static int Alpha = 110;

const static int PadBt_a = 20;
const static int PadBt_r = 890;
const static int PadBt_b = PadBt_y + sqrt(PadBt_r*PadBt_r - PadBt_x*PadBt_x);

static int UI_mode = UI_NONE;

static int pad_id = 0;
static bool pad_grab = false;

void Game_GUI(bool set){
return;
	gui_padbg->active = set;
	gui_padbt->active = set;

	gui_up->active = set;
	gui_down->active = set;
	gui_doodle->active = set;
	gui_egg->active = set;
	gui_gift->active = set;

	gui_menu->active = set;
	gui_tab->active = set;

}

void GUI_Change(int ui_mode) {
return;
	UI_mode = ui_mode;

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
return;
	int w = 230 * 0.8;
	int h = 220 * 0.8;

	PFile::Path path("mobile" PE_SEP);

	gui_touch =  PDraw::create_gui(path, 0,  0,1920,1080, Alpha);
	gui_tab =    PDraw::create_gui(path, 0,930, 530, 150, Alpha);

	path.SetFile("menu.png");
	gui_menu =   PDraw::create_gui(path, 50,130,w,h,Alpha);
	
	path.SetFile("padbg.png");
	gui_padbg =  PDraw::create_gui(path, 90,620,641*0.9,362*0.9,Alpha);

	path.SetFile("padbt.png");
	gui_padbt =  PDraw::create_gui(path, PadBt_x, PadBt_y,169*0.9,173*0.9,Alpha);

	int y = 650;
	const int dy = 200;

	path.SetFile("up.png");
	gui_up =     PDraw::create_gui(path, 1630, y,w,h,Alpha);

	y -= dy;
	path.SetFile("doodle.png");
	gui_doodle = PDraw::create_gui(path, 1630, y,w,h,Alpha);

	y -= dy;
	path.SetFile("gift.png");
	gui_gift =   PDraw::create_gui(path, 1630, y,w,h,Alpha);

	y = 720;
	path.SetFile("down.png");
	gui_down =   PDraw::create_gui(path, 1410, y,w,h,Alpha);

	y -= dy;
	path.SetFile("egg.png");
	gui_egg =    PDraw::create_gui(path, 1410, y,w,h,Alpha);

}

static bool read_gui(PDraw::Gui* gui) {
return false;
	if (!gui)
		return false; //error?

	if (!gui->active)
		return false;

	for (PInput::touch_t touch : PInput::touchlist) {

		float touch_x = touch.pos_x * 1920;
		float touch_y = touch.pos_y * 1080;

		if(touch_x > gui->x && touch_x < gui->x + gui->w)
		if(touch_y > gui->y && touch_y < gui->y + gui->h)
		if(!pad_grab || pad_id != touch.id)
			return true;

	}

	return false;

}

// Hold pad on digital pad mode
static float hold_pad(float pos_x, int* button) {
return 0;
	//pos_x varies from 0 to 1
	//x varies from 0 to 3.2
	float x = 3.2 * pos_x;

	float a =  3.98;
	float b = 25.00;

	//Make the middle poits more close
	if (x < 1.00) {
		x = x + a*(x-0.00)*(x-0.50)*(x-1.00);
		*button = 0;
	
	} else if (x < 1.40) {
		x = x + b*(x-1.00)*(x-1.20)*(x-1.40);
		*button = 1;
	
	} else if (x < 1.80) {
		x = x + b*(x-1.40)*(x-1.60)*(x-1.80);
		*button = 2;
	
	} else if (x < 2.20) {
		x = x + b*(x-1.80)*(x-2.00)*(x-2.20);
		*button = 3;
	
	} else if (x < 3.2) {
		x = x + a*(x-2.20)*(x-2.70)*(x-3.20);
		*button = 4;
	
	} else {
		x = 3.2;
		*button = 4;
	}

	if (x < 0.00)
		x = 0.00;
	if (x > 3.20)
		x = 3.20;
	
	return x / 3.20;

}


static int get_pad() {
return 0;
	int button = 2;

	if (!pad_grab) {
		for (PInput::touch_t touch : PInput::touchlist) {
			
			float touch_x = touch.pos_x * 1920;
			float touch_y = touch.pos_y * 1080;

			if(touch_x > gui_padbg->x && touch_x < gui_padbg->x + gui_padbg->w &&
				touch_y > gui_padbg->y && touch_y < gui_padbg->y + gui_padbg->h) {

				pad_grab = true;
				pad_id = touch.id;
				break;

			}

		}
	
	}
	
	if (pad_grab) {

		PInput::touch_t* last_touch = nullptr;
		for (PInput::touch_t& touch : PInput::touchlist) {

			if(touch.id == pad_id)
				last_touch = &touch;
			
		}

		if (!last_touch) {

			pad_grab = false;
			gui_padbt->x = PadBt_x;
			gui_padbt->y = PadBt_y;

		} else {

			float touch = (last_touch->pos_x * 1920 - gui_padbg->x) / gui_padbg->w;
			float hold_touch = hold_pad(touch, &button);

			gui_padbt->x = gui_padbg->x + hold_touch * gui_padbg->w;
			
			gui_padbt->x -= gui_padbt->w / 2;

			if (gui_padbt->x < gui_padbg->x)
				gui_padbt->x = gui_padbg->x;

			if (gui_padbt->x > gui_padbg->x + gui_padbg->w - gui_padbt->w)
				gui_padbt->x = gui_padbg->x + gui_padbg->w - gui_padbt->w;
			
			int x = gui_padbt->x - PadBt_a;
			gui_padbt->y = PadBt_b - sqrt(PadBt_r*PadBt_r - x*x);

			/*if (button == 0) {
				ret = 100;
			} else if (button == 1) {
				ret = 20;
			} else if (button == 2) {
				ret = 0;
			} else if (button == 3) {
				ret = -20;
			} else if (button == 4) {
				ret = -100;
			}*/

			/*ret = gui_padbg->x + (gui_padbg->w - gui_padbt->w) / 2;
			ret -= gui_padbt->x;
			ret /= float(gui_padbg->w - gui_padbt->w) / 2;
			ret *= 100;*/

		}

	}

	return button;

}

void GUI_Update() {
return;
	if (UI_mode == UI_GAME_BUTTONS) {

		gui_doodle->active = Player_Sprite->ammus2 != -1;
		gui_egg->active = Player_Sprite->ammus1 != -1;
		gui_gift->active = Gifts_Count() > 0;
	
		Gui_pad_button = get_pad();

		Gui_up = read_gui(gui_up);
		Gui_down = read_gui(gui_down);
		Gui_egg = read_gui(gui_egg);
		Gui_doodle = read_gui(gui_doodle);
		Gui_gift = read_gui(gui_gift);

		Gui_menu = read_gui(gui_menu);
		Gui_tab = read_gui(gui_tab);

	} else if (UI_mode == UI_TOUCH_TO_START) {

		Gui_touch = read_gui(gui_touch);
	
	}

}

void GUI_Exit() {
return;
	PDraw::remove_gui(gui_touch);
	PDraw::remove_gui(gui_tab);

	PDraw::remove_gui(gui_menu);
	PDraw::remove_gui(gui_padbg);
	PDraw::remove_gui(gui_padbt);
	PDraw::remove_gui(gui_up);
	PDraw::remove_gui(gui_doodle);
	PDraw::remove_gui(gui_gift);
	PDraw::remove_gui(gui_down);
	PDraw::remove_gui(gui_egg);

}