#pragma once
#include "engine/PInputKey.hpp"


namespace PK2gui{

namespace KeyNav{

typedef enum{
    NONE,

    LEFT,
    RIGHT,
    UP,
    DOWN,

    ESCAPE,
    DELETE,
    BACKSPACE,
    ACCEPT
}Nav;


Nav readKeyNav();

void injectKey(const PInput::Key& k);

}

}