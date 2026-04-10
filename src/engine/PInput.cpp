#include "PInput.hpp"
#include "PInputKey.hpp"

#include "PLog.hpp"
#include "PRender.hpp"
#include "PDraw.hpp"

namespace PInput{


void InputSystem::searchForInputDevices(){

	int start = 0;

	char* cont = getenv("PK2_CONTROLLER");
	if (cont) {
		start = std::max(0, atoi(cont));
	}


	this->closeInputDevices();

	int num = SDL_NumJoysticks();
	for (int i = start; i < num; ++i) {
		if (SDL_IsGameController(i)) {
			this->gController = SDL_GameControllerOpen(i);
			if (this->gController) {
				PLog::Write(PLog::INFO, "PInput", "Controller found: %s", SDL_GameControllerName(gController));
				break;
			}
		}
	}

	if(this->gController == nullptr) {
		PLog::Write(PLog::INFO, "PInput", "No Controller found");
	}

	if (this->gController) {
		SDL_Joystick* joy = SDL_GameControllerGetJoystick(this->gController);
		this->gHaptic = SDL_HapticOpenFromJoystick(joy);

		if (this->gHaptic) {
			if (SDL_HapticRumbleSupported(this->gHaptic) &&
				SDL_HapticRumbleInit(this->gHaptic) == 0) {
				// success
			} else {
				SDL_HapticClose(this->gHaptic);
				this->gHaptic = nullptr;
			}
		}
	}
	else{

		int numHaptics = SDL_NumHaptics();
		for (int i = 0; i < numHaptics; i++) {

			this->gHaptic = SDL_HapticOpen(i);
			if (this->gHaptic == nullptr)
				continue;
			
			if (SDL_HapticRumbleSupported(this->gHaptic)) {
				if (SDL_HapticRumbleInit(this->gHaptic) == 0) {
					break; // ✔ success
				}
			}

			SDL_HapticClose(this->gHaptic);
			this->gHaptic = nullptr;

		}
	}

	if (this->gHaptic == nullptr) {
		PLog::Write(PLog::INFO, "PInput", "No haptic found");
	}
	
}


void InputSystem::closeInputDevices(){
	if(this->gController!=nullptr){
		SDL_GameControllerClose(this->gController);
		this->gController = nullptr;
	}
	if(this->gHaptic!=nullptr){
		SDL_HapticClose(this->gHaptic);
		this->gHaptic = nullptr;
	}
}


void InputSystem::handleEvent(const SDL_Event& event){
	if( (event.type == SDL_KEYDOWN && event.key.repeat == 0) ||
		event.type == SDL_MOUSEBUTTONDOWN ||
		event.type == SDL_CONTROLLERBUTTONDOWN ){

		Key eventKey(event);
		for(const std::function<void(const Key&)> &f: this->keyDownListeners){
			f(eventKey);
		}
	}

	else if(event.type == SDL_KEYUP ||
		event.type == SDL_MOUSEBUTTONUP ||
		event.type == SDL_CONTROLLERBUTTONUP ){

		Key eventKey(event);
		for(const std::function<void(const Key&)> &f: this->keyUpListeners){
			f(eventKey);
		}
	}
	else if(event.type == SDL_TEXTINPUT){
		if(this->textInput){
			this->lastUTF8Char.read(event.text.text);
		}
	}
	else if(event.type == SDL_MOUSEMOTION){
		for(const std::function<void()>& f: this->physicalMouseMotionListeners){
			f();
		}
	}

	else if(event.type == SDL_CONTROLLERDEVICEADDED || event.type == SDL_CONTROLLERDEVICEREMOVED){
		this->searchForInputDevices();
	}
}

bool InputSystem::isKeyPressed(const Key& key){

	switch (key.type)
    {
    case INPUT_KEYBOARD:{
        const Uint8* keymap = SDL_GetKeyboardState(NULL);
        return keymap[(SDL_Scancode)key.code];
    }     
    break;

    case INPUT_MOUSE_BUTTON:{
        int x, y;
        Uint32 buttons = SDL_GetMouseState(&x, &y);
        return (buttons & SDL_BUTTON(key.code)) != 0;
    }
    break;

    case INPUT_GAME_CONTROLLER:{
        if (!gController) return false;

        return SDL_GameControllerGetButton(
            gController,
            (SDL_GameControllerButton)key.code
        ) != 0;
    }

    default:

        break;
    }
    return false;
}

void InputSystem::startTextInput(){

	if(!this->textInput){
		this->textInput = true;
		SDL_StartTextInput();

		this->lastUTF8Char = PString::UTF8_Char();
	}
}

void InputSystem::stopTextInput(){
	if(this->textInput){
		this->textInput = false;
		SDL_StopTextInput();
	}
}

PString::UTF8_Char InputSystem::getLastUTF8(){
	PString::UTF8_Char res = this->lastUTF8Char;
	if(!res.isNull()){
		this->lastUTF8Char = PString::UTF8_Char();
	}

	return res;
}



float InputSystem::getAxis(int axis)const{

	if(this->gController==nullptr)return 0;

	float fac = 1.f/32768;

	if (axis == 0)
		fac *= SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_LEFTX);
	else if (axis == 1)
		fac *= SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_LEFTY);
	else if (axis == 2)
		fac *= SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_RIGHTX);
	else if (axis == 3)
		fac *= SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_RIGHTY);
	else if (axis == 4)
		fac *= SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	else if (axis == 5)
		fac *= SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	else
		return 0.f;
	
	if (abs(fac) < 0.15)
		fac = 0.f;

	return fac;
}

void InputSystem::vibrate(int duration, float strength)const{

	u32 duration_ms = (u32)(duration * 5 / 3); //convert game ticks to ms

	if(this->gController!=nullptr){
		u16 rumble = (u16)(0xFFFF * strength);
		SDL_GameControllerRumble(this->gController, rumble, rumble, duration_ms);
	}

	if(this->gHaptic!=nullptr){
		SDL_HapticRumblePlay(this->gHaptic, strength, duration_ms);
	}
}


void InputSystem::updateMouse(){
	static int last_x, last_y;
	static bool ignore_mouse = false;

	int sw, sh;
	PRender::get_window_size(&sw, &sh);

	int bw, bh;
	PDraw::get_buffer_size(&bw, &bh);

	int off_x, off_y;
	PDraw::get_offset(&off_x, &off_y);


	//if(!Settings.touchscreen_mode){

		if (!PRender::is_fullscreen()) {
			SDL_SetRelativeMouseMode(SDL_FALSE);

			int tmpx, tmpy;
			SDL_GetMouseState(&tmpx, &tmpy);

			//Problem with fitScreen
			tmpx *= float(bw) / sw;
			tmpy *= float(bh) / sh;

			tmpx -= off_x;
			tmpy -= off_y;

			// Mouse moved
			if (abs(last_x - tmpx) > 0 || abs(last_y - tmpy) > 0)
				ignore_mouse = false;

			last_x = tmpx;
			last_y = tmpy;
			
			if (!ignore_mouse) {
				mousePos.x = float(tmpx);
				mousePos.y = float(tmpy);
			}
	
		} else {
			SDL_SetRelativeMouseMode(SDL_TRUE);

			ignore_mouse = false;

			int delta_x, delta_y;
			SDL_GetRelativeMouseState(&delta_x, &delta_y);

			mousePos.x += 0.4 * delta_x;
			mousePos.y += 0.4 * delta_y;
		}
	//}

	if(this->mouseKeysEnabled) {

		float delta_x = 0;
		float delta_y = 0;

		delta_x += getAxis(0) * 3;
		delta_y += getAxis(1) * 3;

		if (isKeyPressed(Key::LEFT) || isKeyPressed(Key::JOY_LEFT) ){
			delta_x += -3;
		}  
		if (isKeyPressed(Key::RIGHT) || isKeyPressed(Key::JOY_RIGHT) ){
			delta_x += +3;
		}
		if (isKeyPressed(Key::UP) || isKeyPressed(Key::JOY_UP) ){
			delta_y += -3;
		}
		if (isKeyPressed(Key::DOWN) || isKeyPressed(Key::JOY_DOWN) ){
			delta_y += +3;
		}

		if (delta_x > 0.1 || delta_x < -0.1 || 
		    delta_y > 0.1 || delta_y < -0.1)
			ignore_mouse = true;

		mousePos.x += delta_x;
		mousePos.y += delta_y;
		
	}

	// set limits
	if (mousePos.x < -off_x) mousePos.x = -off_x;
	if (mousePos.x > bw - off_x - 19) mousePos.x = bw - off_x - 19;
	if (mousePos.y < -off_y) mousePos.y = -off_y;
	if (mousePos.y > bh - off_y - 19) mousePos.y = bh - off_y - 19;
}


void InputSystem::updateTouch(){
	this->mTouchlist.clear();
	
	int bw, bh;
	PDraw::get_buffer_size(&bw, &bh);

	int off_x, off_y;
	PDraw::get_offset(&off_x, &off_y);

	int devicesNumber = SDL_GetNumTouchDevices();

	bool mouseSet = false;
	for (int i = 0; i < devicesNumber; i++) {
		
		SDL_TouchID id = SDL_GetTouchDevice(i);

		int fingers = SDL_GetNumTouchFingers(id);

		for(int j = 0; j < fingers; j++){

			SDL_Finger* finger = SDL_GetTouchFinger(id, j);
			if(finger!=nullptr) {

				Touch touch(finger->x, finger->y, finger->id);
				this->mTouchlist.emplace_back(touch);

				int tmpx = finger->x * bw - off_x;
				int tmpy = finger->y * bh - off_y;
				if(!mouseSet){
					mouseSet = true;
					this->mousePos.x = tmpx;
					this->mousePos.y = tmpy;
				}
			}

				
		}

	}
}


}