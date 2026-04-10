#pragma once

#include "types.hpp"
#include "PInputKey.hpp"
#include "PString.hpp"

#include <SDL.h>
#include <vector>
#include <functional>


namespace PInput{


class Touch{
public:
    Touch(float x, float y, int id):
    x(x), y(y), id(id){

    }

    float x = 0;
    float y = 0;
    int id = 0;
};

class InputSystem{
public:
    static InputSystem& instance() {
        static InputSystem instance;
        return instance;
    }


    void addKeyDownListener(std::function<void(const Key&)> f){
        this->keyDownListeners.emplace_back(f);
    }
    void addKeyUpListener(std::function<void(const Key&)> f){
        this->keyUpListeners.emplace_back(f);
    }

    void addPhysicalMouseMotionListener(std::function<void()> f){
        this->physicalMouseMotionListeners.emplace_back(f);
    }


    bool isJoystickAvailable(){
        return this->gController!=nullptr;
    }

    bool isTextInputActive()const{
        return this->textInput;
    }

    const Point2D& getMousePos(){
        return this->mousePos;
    }


    void searchForInputDevices();
    void closeInputDevices();


    void handleEvent(const SDL_Event& event);
    float getAxis(int axis)const;
    void vibrate(int duration, float strength)const;

    bool isKeyPressed(const Key& key);


    void startTextInput();
    void stopTextInput();
    PString::UTF8_Char getLastUTF8();
    
    
    void updateMouse();
    void updateTouch();

    bool mouseKeysEnabled = false;

    const std::vector<Touch>& getTouches()const{
        return this->mTouchlist;
    }
private:
    std::vector<Touch> mTouchlist;

    InputSystem() = default;

    Point2D mousePos;

    bool textInput = false;
    PString::UTF8_Char lastUTF8Char;

    SDL_GameController *gController = nullptr;
    SDL_Haptic * gHaptic = nullptr;

    std::vector<std::function<void(const Key& key)>> keyDownListeners;
    std::vector<std::function<void(const Key& key)>> keyUpListeners;
    std::vector<std::function<void()>> physicalMouseMotionListeners;
};
    
}