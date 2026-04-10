//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "screen.hpp"

class IntroScreen:public Screen{
public:
    void Init()override;
    void Loop()override;
    void Draw();

    void onKeyPressed(const PInput::Key& key)override;
private:
    uint intro_counter = 0;
    bool closing_intro = false;
};