#include "input_settings.hpp"

#include "engine/PInput.hpp"


void InputSettings::setDefault(){

    this->left = PInput::Key(SDL_SCANCODE_LEFT);
    this->right = PInput::Key(SDL_SCANCODE_RIGHT);
	this->up = PInput::Key(SDL_SCANCODE_UP);
	this->down = PInput::Key(SDL_SCANCODE_DOWN);
	this->walk_slow = PInput::Key(SDL_SCANCODE_LALT);
	this->attack1 = PInput::Key(SDL_SCANCODE_LCTRL);
	this->attack2 = PInput::Key(SDL_SCANCODE_LSHIFT);
	this->open_gift = PInput::Key(SDL_SCANCODE_SPACE);

	this->toggleHUD = PInput::Key(SDL_SCANCODE_F1);
	this->screenshotKey = PInput::Key(SDL_SCANCODE_F2);
	this->fullscreenModeSwitch = PInput::Key(SDL_SCANCODE_F11);
	this->pauseGame = PInput::Key(SDL_SCANCODE_P);
	this->commitSuicide = PInput::Key(SDL_SCANCODE_DELETE);
	this->cycleGifts = PInput::Key(SDL_SCANCODE_TAB);

	this->dev_fly = PInput::Key(SDL_SCANCODE_U);
	this->dev_heal = PInput::Key(SDL_SCANCODE_E);
	this->dev_ghostMode = PInput::Key(SDL_SCANCODE_Y);

}

void InputSettings::setDefaultJoystick(){
	this->left = PInput::Key(SDL_CONTROLLER_BUTTON_DPAD_LEFT, PInput::INPUT_GAME_CONTROLLER);
	this->right = PInput::Key(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, PInput::INPUT_GAME_CONTROLLER);
	this->up = PInput::Key(SDL_CONTROLLER_BUTTON_DPAD_UP, PInput::INPUT_GAME_CONTROLLER);
	this->down = PInput::Key(SDL_CONTROLLER_BUTTON_DPAD_DOWN, PInput::INPUT_GAME_CONTROLLER);

	this->walk_slow = PInput::Key(SDL_CONTROLLER_BUTTON_Y, PInput::INPUT_GAME_CONTROLLER);
	this->attack1 = PInput::Key(SDL_CONTROLLER_BUTTON_A, PInput::INPUT_GAME_CONTROLLER);
	this->attack2 = PInput::Key(SDL_CONTROLLER_BUTTON_B, PInput::INPUT_GAME_CONTROLLER);
	this->open_gift = PInput::Key(SDL_CONTROLLER_BUTTON_LEFTSHOULDER, PInput::INPUT_GAME_CONTROLLER);
}


PInput::Key* InputSettings::getKeyByMenuID(unsigned int menuId){
	switch(menuId){
		case 1 : return &this->left;
		case 2 : return &this->right;
		case 3 : return &this->up;
		case 4 : return &this->down;
		case 5 : return &this->walk_slow;
		case 6 : return &this->attack2;
		case 7 : return &this->attack1;
		case 8 : return &this->open_gift;
	}


	return nullptr;
}


void from_json(const nlohmann::json& j, InputSettings& controls){
    j.at("left").get_to(controls.left);
	j.at("right").get_to(controls.right);
	j.at("up").get_to(controls.up);
	j.at("down").get_to(controls.down);

	j.at("walk_slow").get_to(controls.walk_slow);
	j.at("attack1").get_to(controls.attack1);
	j.at("attack2").get_to(controls.attack2);
	j.at("open_gift").get_to(controls.open_gift);

}

void to_json(nlohmann::json& j, const InputSettings& controls){
    j["left"] = controls.left;
	j["right"] = controls.right;
	j["up"] = controls.up;
	j["down"] = controls.down;

	j["walk_slow"] = controls.walk_slow;
	j["attack1"] = controls.attack1;
	j["attack2"] = controls.attack2;

	j["open_gift"] = controls.open_gift;

}