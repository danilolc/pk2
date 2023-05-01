//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "gui.hpp"

#include "system.hpp"
#include "settings.hpp"
#include "game/gifts.hpp"
#include "game/sprites.hpp"

#include "engine/PRender.hpp"
#include "engine/PGui.hpp"
#include "engine/PInput.hpp"

#include <cmath>

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


static PGui::Gui* gui_padbg  = nullptr;
static PGui::Gui* gui_padbt  = nullptr;

static PGui::Gui* gui_up     = nullptr;
static PGui::Gui* gui_down   = nullptr;
static PGui::Gui* gui_egg    = nullptr;
static PGui::Gui* gui_doodle = nullptr;
static PGui::Gui* gui_gift   = nullptr;

static PGui::Gui* gui_menu   = nullptr;
static PGui::Gui* gui_touch  = nullptr;
static PGui::Gui* gui_tab    = nullptr;


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

	int button_w = 230, button_h = 220;

	int w = button_w * 0.8;
	int h = button_h * 0.8;

	PFile::Path path("mobile" PE_SEP "all.png");
	PRender::load_ui_texture(path);
	PRender::RECT src, dst;

	src.x = 0, src.y = 0, src.w = 0,    src.h = 0;
	dst.x = 0, dst.y = 0, dst.w = 1920, dst.h = 1080;
	gui_touch = PGui::create_gui(src, dst, Alpha);


	src.x = 0, src.y = 0,   src.w = 0,   src.h = 0;
	dst.x = 0, dst.y = 930, dst.w = 530, dst.h = 150;
	gui_tab = PGui::create_gui(src, dst, Alpha);

	src.x = button_w * 1, src.y = button_h * 1, src.w = button_w, src.h = button_h;
	dst.x = 50, dst.y = 130, dst.w = w, dst.h = h;
	gui_menu = PGui::create_gui(src, dst, Alpha);
	
	src.x = button_w * 0, src.y = button_h * 3, src.w = 640, src.h = 360;
	dst.x = 90, dst.y = 610, dst.w = 577, dst.h = 353;
	gui_padbg = PGui::create_gui(src, dst, Alpha);

	src.x = button_w * 0, src.y = button_h * 2, src.w = 175, src.h = 175;
	dst.x = PadBt_x, dst.y = PadBt_y, dst.w = 152, dst.h = 156;
	gui_padbt = PGui::create_gui(src, dst, Alpha);

	int y = 650;
	const int dy = 200;

	src.x = button_w * 2, src.y = button_h * 0, src.w = button_w, src.h = button_h;
	dst.x = 1630, dst.y = y, dst.w = w, dst.h = h;
	gui_up = PGui::create_gui(src, dst, Alpha);

	y -= dy;
	src.x = button_w * 0, src.y = button_h * 0, src.w = button_w, src.h = button_h;
	dst.x = 1630, dst.y = y, dst.w = w, dst.h = h;
	gui_doodle = PGui::create_gui(src, dst, Alpha);

	y -= dy;
	src.x = button_w * 2, src.y = button_h * 1, src.w = button_w, src.h = button_h;
	dst.x = 1630, dst.y = y, dst.w = w, dst.h = h;
	gui_gift = PGui::create_gui(src, dst, Alpha);

	y = 720;
	src.x = button_w * 1, src.y = button_h * 0, src.w = button_w, src.h = button_h;
	dst.x = 1410, dst.y = y, dst.w = w, dst.h = h;
	gui_down = PGui::create_gui(src, dst, Alpha);

	y -= dy;
	src.x = button_w * 0, src.y = button_h * 1, src.w = button_w, src.h = button_h;
	dst.x = 1410, dst.y = y, dst.w = w, dst.h = h;
	gui_egg = PGui::create_gui(src, dst, Alpha);

}

static bool read_gui(PGui::Gui* gui) {

	if (!gui)
		return false; //error?

	if (!gui->active)
		return false;

	for (PInput::touch_t touch : PInput::touchlist) {

		float touch_x = touch.pos_x * 1920;
		float touch_y = touch.pos_y * 1080;

		if(touch_x > gui->dst.x && touch_x < gui->dst.x + gui->dst.w)
		if(touch_y > gui->dst.y && touch_y < gui->dst.y + gui->dst.h)
		if(!pad_grab || pad_id != touch.id)
			return true;

	}

	return false;

}

// Hold pad on digital pad mode
static float hold_pad(float pos_x, int* button) {

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

	int button = 2;

	if (!pad_grab) {
		for (PInput::touch_t touch : PInput::touchlist) {
			
			float touch_x = touch.pos_x * 1920;
			float touch_y = touch.pos_y * 1080;

			if(touch_x > gui_padbg->dst.x && touch_x < gui_padbg->dst.x + gui_padbg->dst.w &&
				touch_y > gui_padbg->dst.y && touch_y < gui_padbg->dst.y + gui_padbg->dst.h) {

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
			gui_padbt->dst.x = PadBt_x;
			gui_padbt->dst.y = PadBt_y;

		} else {

			float touch = (last_touch->pos_x * 1920 - gui_padbg->dst.x) / gui_padbg->dst.w;
			float hold_touch = hold_pad(touch, &button);

			gui_padbt->dst.x = gui_padbg->dst.x + hold_touch * gui_padbg->dst.w;
			
			gui_padbt->dst.x -= gui_padbt->dst.w / 2;

			if (gui_padbt->dst.x < gui_padbg->dst.x)
				gui_padbt->dst.x = gui_padbg->dst.x;

			if (gui_padbt->dst.x > gui_padbg->dst.x + gui_padbg->dst.w - gui_padbt->dst.w)
				gui_padbt->dst.x = gui_padbg->dst.x + gui_padbg->dst.w - gui_padbt->dst.w;
			
			int x = gui_padbt->dst.x - PadBt_a;
			gui_padbt->dst.y = PadBt_b - sqrt(PadBt_r*PadBt_r - x*x);

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

static int doodle_alpha = 0, egg_alpha = 0, gift_alpha = 0; 
static bool doodle_active = false, egg_active = false, gift_active = false;

void GUI_Reset() {

	doodle_alpha = 0; egg_alpha = 0; gift_alpha = 0; 
	doodle_active = false; egg_active = false; gift_active = false;

}

void GUI_Update() {

	const int DELTA = 5;

	Gui_pad_button = 2;

	Gui_up     = false;
	Gui_down   = false;
	Gui_egg    = false;
	Gui_doodle = false;
	Gui_gift   = false;

	Gui_menu  = false;
	Gui_touch = false;
	Gui_tab   = false;

	if (UI_mode == UI_GAME_BUTTONS) {

		float alpha = Screen_Alpha();

		{
			doodle_active = Player_Sprite->ammus2 != nullptr;
			egg_active = Player_Sprite->ammus1 != nullptr;
			gift_active = Gifts_Count() > 0;

			if (doodle_active) {
				doodle_alpha += DELTA;
				if (doodle_alpha > Alpha) doodle_alpha = Alpha;
			} else {
				doodle_alpha -= DELTA;
				if (doodle_alpha < 0) doodle_alpha = 0;
			}

			if (egg_active) {
				egg_alpha += DELTA;
				if (egg_alpha > Alpha) egg_alpha = Alpha;
			} else {
				egg_alpha -= DELTA;
				if (egg_alpha < 0) egg_alpha = 0;
			}

			if (gift_active) {
				gift_alpha += DELTA;
				if (gift_alpha > Alpha) gift_alpha = Alpha;
			} else {
				gift_alpha -= DELTA;
				if (gift_alpha < 0) gift_alpha = 0;
			}

			gui_doodle->alpha = doodle_alpha * alpha;
			gui_egg->alpha = egg_alpha * alpha;
			gui_gift->alpha = gift_alpha * alpha;

			gui_doodle->active = doodle_alpha != 0;
			gui_egg->active = egg_alpha != 0;
			gui_gift->active = gift_alpha != 0;
		}

		gui_up->alpha = Alpha * alpha;
		gui_down->alpha = Alpha * alpha;
		gui_padbg->alpha = Alpha * alpha;
		gui_padbt->alpha = Alpha * alpha;
		gui_menu->alpha = Alpha * alpha;

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

	PGui::remove_gui(gui_touch);
	PGui::remove_gui(gui_tab);

	PGui::remove_gui(gui_menu);
	PGui::remove_gui(gui_padbg);
	PGui::remove_gui(gui_padbt);
	PGui::remove_gui(gui_up);
	PGui::remove_gui(gui_doodle);
	PGui::remove_gui(gui_gift);
	PGui::remove_gui(gui_down);
	PGui::remove_gui(gui_egg);

}