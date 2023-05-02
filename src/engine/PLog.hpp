//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/types.hpp"
#include "engine/PFile.hpp"

namespace PLog {

enum {

    DEBUG = 1 << 0,
    INFO  = 1 << 1,
    WARN  = 1 << 2,
    ERR   = 1 << 3,
    FATAL = 1 << 4,
    ALL   = 255,
    OFF   = 0
    
};

void Init(u8 level, PFile::Path file);
void Write(const char* txt);
void Write(u8 level, const char* origin, const char* format, ...);
void Exit();

}