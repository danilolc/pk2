#pragma once

#include "PJson.hpp"

#include <SDL.h>
#include <string>

namespace PInput{

class InputSystem;

typedef enum{
    INPUT_UNKNOWN = 0,
    INPUT_KEYBOARD = 1,
    INPUT_GAME_CONTROLLER = 2,
    INPUT_MOUSE_BUTTON = 3
}InputType;

class Key{
public:
    Key() = default;

    Key(int scancode, InputType type=INPUT_KEYBOARD):
        type(type), code(scancode){}

    Key(const SDL_Event& event);

    bool operator==(const Key&other)const{
        return this->type == other.type && this->code == other.code;
    }
    bool operator!=(const Key&other)const{
        return !this->operator==(other);
    }
    InputType getInputType()const{
        return this->type;
    }

    bool isPressed()const;
    bool accept(const SDL_Event& event)const;
    std::string getName()const; 
    std::optional<int> getNumericValue()const;


    static const Key ESCAPE;
    static const Key RETURN;
    static const Key DELETE;
    static const Key BACKSPACE;
    static const Key SPACE;
    
    static const Key LEFT;
    static const Key RIGHT;
    static const Key UP;
    static const Key DOWN;
    static const Key LALT;

    static const Key MOUSE_LEFT;
    static const Key MOUSE_RIGHT;
    
    static const Key JOY_A;
    static const Key JOY_B;
    static const Key JOY_X;
    static const Key JOY_Y;

    static const Key JOY_UP;
    static const Key JOY_DOWN;
    static const Key JOY_LEFT;
    static const Key JOY_RIGHT;

    static const Key JOY_START;
    static const Key JOY_STICK_LEFT;
    static const Key JOY_STICK_RIGHT;

    static const Key JOY_GUIDE;
    
private:
    friend class InputSystem;
    InputType type = INPUT_UNKNOWN;
    int code = 0;

    friend void to_json(nlohmann::json& j, const Key& key);
    friend void from_json(const nlohmann::json& j, Key& key);
};

}

