//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################
#pragma once

#include "platform.hpp"

#include "PisteDraw.hpp"
#include "PisteInput.hpp"
#include "PisteSound.hpp"
#include "PisteUtils.hpp"
#include "PisteLanguage.hpp"

namespace Piste {

class Game {

public:
    
    Game(int width, int height, const char* name, const char* icon);
    ~Game();

    void loop(int (*GameLogic)());
    void stop();
    float get_fps();

    void set_debug(bool set);
    void ignore_frame();
    bool is_ready();
    
private:
    
    bool ready = false;
    bool running = false;

    float avrg_fps = 0;

    bool debug = false;
    bool draw = true;
    
    void logic();
};

}
