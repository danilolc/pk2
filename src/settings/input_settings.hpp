#pragma once
#include "engine/PInput.hpp"

class InputSettings{
public:
    InputSettings(){
        this->setDefault();
    }

    void setDefault();
    void setDefaultJoystick();

    PInput::Key* getKeyByMenuID(unsigned int menuID);

    //bool setKey(const PInput::Key& key, int id);

    PInput::Key left;
    PInput::Key right;
    PInput::Key up;
    PInput::Key down;

    PInput::Key walk_slow;
	PInput::Key attack1;
	PInput::Key attack2;
	PInput::Key open_gift;

    PInput::Key toggleHUD;
    PInput::Key screenshotKey;
    PInput::Key fullscreenModeSwitch;
    PInput::Key pauseGame;
    PInput::Key commitSuicide;
    PInput::Key cycleGifts;

    PInput::Key dev_ghostMode;
    PInput::Key dev_fly;
    PInput::Key dev_heal;
    
    friend void from_json(const nlohmann::json& j, InputSettings& controls);
	friend void to_json(nlohmann::json& j, const InputSettings& controls);
};