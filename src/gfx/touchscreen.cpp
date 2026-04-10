//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "touchscreen.hpp"

#include "system.hpp"
#include "settings/settings.hpp"
#include "game/gifts.hpp"
#include "game/game.hpp"
#include "engine/PRender.hpp"
#include "engine/PInput.hpp"
#include "engine/PFilesystem.hpp"

#include <cmath>


const static int PadBt_x = 315;
const static int PadBt_y = 675;

const static int TS_ALPHA_MAX = 110;

const static int PadBt_a = 20;
const static int PadBt_r = 890;
const static int PadBt_b = PadBt_y + sqrt(PadBt_r*PadBt_r - PadBt_x*PadBt_x);

PK2TouchScreenButton::PK2TouchScreenButton(PRender::RECT src, PRender::RECT dst, u8 alpha):
src(src), dst(dst), alpha(alpha){

}

void PK2TouchScreenButton::draw()const{

	if (this->src.w == 0 || this->src.h == 0)return;

	PRender::FRECT dst;
	dst.x = float(this->dst.x) / 1920;
	dst.y = float(this->dst.y) / 1080;
	dst.w = float(this->dst.w) / 1920;
	dst.h = float(this->dst.h) / 1080;

	PRender::FRECT src;
	src.x = float(this->src.x) / 1024;
	src.y = float(this->src.y) / 1024;
	src.w = float(this->src.w) / 1024;
	src.h = float(this->src.h) / 1024;

	PRender::render_ui(src, dst, float(this->alpha) / 255);
}

void PK2TouchScreenControls::change(int ui_mode) {

	this->UI_mode = ui_mode;

	// UI_mode = ui_mode;

	// switch(ui_mode){
	// 	case UI_TOUCH_TO_START:
	// 		gui_touch.active = true;
	// 		this->set(false);
	// 	break;
	// 	case UI_CURSOR:
	// 		gui_touch.active = false;
	// 		this->set(false);
	// 	break;
	// 	case UI_GAME_BUTTONS:
	// 		gui_touch.active = true;
	// 		this->set(true);
	// 	break;
	// }

}

void PK2TouchScreenControls::load() {
	if(loaded)return;
	this->loaded = true;

	int button_w = 230, button_h = 220;

	int w = button_w * 0.8;
	int h = button_h * 0.8;

	std::optional<PFile::Path> path = PFilesystem::FindVanillaAsset("touchscreen.png", PFilesystem::GFX_DIR);

	if(!path.has_value()){
		throw std::runtime_error("\"touchscreen.png\" not found!");
	}

	PRender::load_ui_texture(*path);

	PRender::RECT src, dst;

	src.x = 0, src.y = 0, src.w = 0,    src.h = 0;
	dst.x = 0, dst.y = 0, dst.w = 1920, dst.h = 1080;
	gui_touch = PK2TouchScreenButton(src, dst, TS_ALPHA_MAX);


	src.x = 0, src.y = 0,   src.w = 0,   src.h = 0;
	dst.x = 0, dst.y = 930, dst.w = 530, dst.h = 150;
	gui_tab = PK2TouchScreenButton(src, dst, TS_ALPHA_MAX);

	src.x = button_w * 1, src.y = button_h * 1, src.w = button_w, src.h = button_h;
	dst.x = 50, dst.y = 130, dst.w = w, dst.h = h;
	gui_menu = PK2TouchScreenButton(src, dst, TS_ALPHA_MAX);
	
	src.x = button_w * 0, src.y = button_h * 3, src.w = 640, src.h = 360;
	dst.x = 90, dst.y = 610, dst.w = 577, dst.h = 353;
	gui_padbg = PK2TouchScreenButton(src, dst, TS_ALPHA_MAX);

	src.x = button_w * 0, src.y = button_h * 2, src.w = 175, src.h = 175;
	dst.x = PadBt_x, dst.y = PadBt_y, dst.w = 152, dst.h = 156;
	gui_padbt = PK2TouchScreenButton(src, dst, TS_ALPHA_MAX);

	int y = 650;
	const int dy = 200;

	src.x = button_w * 2, src.y = button_h * 0, src.w = button_w, src.h = button_h;
	dst.x = 1630, dst.y = y, dst.w = w, dst.h = h;
	gui_up = PK2TouchScreenButton(src, dst, TS_ALPHA_MAX);

	y -= dy;
	src.x = button_w * 0, src.y = button_h * 0, src.w = button_w, src.h = button_h;
	dst.x = 1630, dst.y = y, dst.w = w, dst.h = h;
	gui_doodle = PK2TouchScreenButton(src, dst, TS_ALPHA_MAX);

	y -= dy;
	src.x = button_w * 2, src.y = button_h * 1, src.w = button_w, src.h = button_h;
	dst.x = 1630, dst.y = y, dst.w = w, dst.h = h;
	gui_gift = PK2TouchScreenButton(src, dst, TS_ALPHA_MAX);

	y = 720;
	src.x = button_w * 1, src.y = button_h * 0, src.w = button_w, src.h = button_h;
	dst.x = 1410, dst.y = y, dst.w = w, dst.h = h;
	gui_down = PK2TouchScreenButton(src, dst, TS_ALPHA_MAX);

	y -= dy;
	src.x = button_w * 0, src.y = button_h * 1, src.w = button_w, src.h = button_h;
	dst.x = 1410, dst.y = y, dst.w = w, dst.h = h;
	gui_egg = PK2TouchScreenButton(src, dst, TS_ALPHA_MAX);

}

bool PK2TouchScreenControls::readGui(const PK2TouchScreenButton& gui) {
	gui.draw();
	for (const PInput::Touch& touch: PInput::InputSystem::instance().getTouches()) {

		float touch_x = touch.x * 1920;
		float touch_y = touch.y * 1080;

		if(touch_x > gui.dst.x && touch_x < gui.dst.x + gui.dst.w)
		if(touch_y > gui.dst.y && touch_y < gui.dst.y + gui.dst.h)
		if(!pad_grab || pad_id != touch.id)
			return true;
	}
	return false;
}

// Hold pad on digital pad mode
float PK2TouchScreenControls::holdPad(float pos_x, int* button) {

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


int PK2TouchScreenControls::getPad() {

	int button = 2;

	if (!pad_grab) {
		for (const PInput::Touch& touch : PInput::InputSystem::instance().getTouches()) {
			
			float touch_x = touch.x * 1920;
			float touch_y = touch.y * 1080;

			if(touch_x > gui_padbg.dst.x && touch_x < gui_padbg.dst.x + gui_padbg.dst.w &&
				touch_y > gui_padbg.dst.y && touch_y < gui_padbg.dst.y + gui_padbg.dst.h) {

				pad_grab = true;
				pad_id = touch.id;
				break;

			}

		}
	
	}
	
	if (pad_grab) {

		const PInput::Touch* last_touch = nullptr;
		for (const PInput::Touch& touch :  PInput::InputSystem::instance().getTouches()) {

			if(touch.id == pad_id)
				last_touch = &touch;
			
		}

		if (!last_touch) {

			pad_grab = false;
			gui_padbt.dst.x = PadBt_x;
			gui_padbt.dst.y = PadBt_y;

		} else {

			float touch = (last_touch->x * 1920 - gui_padbg.dst.x) / gui_padbg.dst.w;
			float hold_touch = this->holdPad(touch, &button);

			gui_padbt.dst.x = gui_padbg.dst.x + hold_touch * gui_padbg.dst.w;
			
			gui_padbt.dst.x -= gui_padbt.dst.w / 2;

			if (gui_padbt.dst.x < gui_padbg.dst.x)
				gui_padbt.dst.x = gui_padbg.dst.x;

			if (gui_padbt.dst.x > gui_padbg.dst.x + gui_padbg.dst.w - gui_padbt.dst.w)
				gui_padbt.dst.x = gui_padbg.dst.x + gui_padbg.dst.w - gui_padbt.dst.w;
			
			int x = gui_padbt.dst.x - PadBt_a;
			gui_padbt.dst.y = PadBt_b - sqrt(PadBt_r*PadBt_r - x*x);

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

			/*ret = gui_padbg.x + (gui_padbg.w - gui_padbt.w) / 2;
			ret -= gui_padbt.x;
			ret /= float(gui_padbg.w - gui_padbt.w) / 2;
			ret *= 100;*/

		}

	}

	return button;

}

void PK2TouchScreenControls::reset(){
	//doodle_alpha = 0; egg_alpha = 0; gift_alpha = 0;

	this->gui_doodle.alpha_counter = 0;
	this->gui_doodle.active = false;

	this->gui_egg.alpha_counter = 0;
	this->gui_egg.active = false;


	this->gui_gift.alpha_counter = 0;
	this->gui_gift.active = false;


	this->gui_menu.alpha_counter = TS_ALPHA_MAX;
	this->gui_menu.active = true;
		
	this->gui_up.active = true;
	this->gui_down.alpha_counter = TS_ALPHA_MAX;

	this->gui_padbg.active = true;
	this->gui_padbg.alpha_counter = TS_ALPHA_MAX;

	this->gui_padbt.active = true;
	this->gui_padbt.alpha_counter = TS_ALPHA_MAX;

	this->gui_tab.active = true;
}


void PK2TouchScreenButton::update(){

	float screen_alpha = Screen_Alpha();

	const int DELTA = 5;
	if(this->active){
		this->alpha_counter+=DELTA;
		if (this->alpha_counter > TS_ALPHA_MAX) this->alpha_counter = TS_ALPHA_MAX;
	}
	else {
		this->alpha_counter -= DELTA;
		if (this->alpha_counter < 0) this->alpha_counter = 0;
	}

	this->alpha = static_cast<u8>(screen_alpha * alpha_counter);
}

void PK2TouchScreenControls::update() {

	if(!this->loaded){
		this->load();
	}

	

	this->pad_button = 2;

	this->up     = false;
	this->down   = false;
	this->egg    = false;
	this->doodle = false;
	this->gift   = false;

	this->menu  = false;
	this->touch = false;
	this->tab   = false;

	if (UI_mode == UI_GAME_BUTTONS) {

		{

			SpriteClass* Player_Sprite = Game->playerSprite;

			this->gui_doodle.active = Player_Sprite->ammo2 != nullptr;
			this->gui_egg.active =  Player_Sprite->ammo1 != nullptr;
			this->gui_gift.active =  Game->gifts.count() > 0;

			this->gui_up.active = Player_Sprite->player_c < 3;
			this->gui_down.active = (Player_Sprite->player_c == 1 || (this->gui_up.active && Player_Sprite->weight==0));

			

			this->gui_padbg.active = Player_Sprite->player_c == 1;
			this->gui_padbt.active = this->gui_padbg.active;


			this->gui_doodle.update();
			this->gui_egg.update();
			this->gui_gift.update();

		
			this->gui_up.update();
			this->gui_down.update();

			this->gui_padbg.update();
			this->gui_padbt.update();
		}


		this->pad_button = this->getPad();

		this->up = this->readGui(gui_up);
		this->down = this->readGui(gui_down);
		this->egg = this->readGui(gui_egg);
		this->doodle = this->readGui(gui_doodle);
		this->gift = this->readGui(gui_gift);

		this->menu = this->readGui(gui_menu);
		this->tab = this->readGui(gui_tab);

		this->gui_padbg.draw();
		this->gui_padbt.draw();

	} else if (UI_mode == UI_TOUCH_TO_START) {
		this->touch = this->readGui(gui_touch);
	}

	this->any = egg || doodle || gift || up || down || menu || tab;
}

PK2TouchScreenControls TouchScreenControls;