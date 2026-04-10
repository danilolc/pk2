#include "screen.hpp"
#include "engine/PInput.hpp"
#include "engine/PSound.hpp"
#include "sfx.hpp"
#include "system.hpp"
#include "gfx/text.hpp"
#include "language.hpp"

bool Screen::closing_game = false;
int Screen::next_screen = 0;

int Screen::bg_screen = -1;
bool Screen::mouse_hidden = false;


void Screen::fadeQuit() {

	if(!closing_game) Fade_out(FADE_FAST);
	closing_game = true;
	PSound::set_musicvolume(0);
	
}

bool Screen::drawMenuText(int id, int x, int y){
	return drawMenuTextS(tekstit->Get_Text(id), x, y);
}


void Screen::drawMouseCursor(){
	if (!mouse_hidden){
		const Point2D mousePos = PInput::InputSystem::instance().getMousePos();
		Draw_Cursor(mousePos.x, mousePos.y);
	}
}


void Screen::clearMouseInput(){
	this->enterPressed = false;
	this->mousePressed = false;
}

void Screen::onKeyPressed(const PInput::Key& k){
	if(k==PInput::Key::MOUSE_LEFT || k==PInput::Key::JOY_STICK_LEFT){
		this->mousePressed = true;
	}
	else if(k==PInput::Key::SPACE ||
		k==PInput::Key::RETURN ||
		k==PInput::Key::JOY_A ||
		k==PInput::Key::JOY_START) {
			this->enterPressed = true;
	}

	if(k.getInputType() == PInput::INPUT_MOUSE_BUTTON){
		mouse_hidden = false;
	}
}

void Screen::onKeyReleased(const PInput::Key& k){

}


bool Screen::drawMenuTextS(const std::string& text, int x, int y) {

	const int TEXT_H = 20; 

	int length = text.size() * 15;

	const Point2D& mousePos = PInput::InputSystem::instance().getMousePos();
	bool mouse_on = mousePos.x > x && mousePos.x < x + length 
		&& mousePos.y > y && mousePos.y < y + TEXT_H
		&& !mouse_hidden;

	if ( mouse_on ) {

		Wavetext_Draw(text.c_str(), fontti3, x, y);//

		if ( (this->mousePressed && mouse_on) || this->enterPressed ) {

			Play_MenuSFX(sfx_global.menu_sound, 100);
			return true;

		}
	} else {
		WavetextSlow_Draw(text.c_str(), fontti2, x, y);	
	}
	return false;
}