//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "types.hpp"
#include "PFile.hpp"

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

void Init(u8 level, bool _print_to_stdout, bool _print_to_file);
void Write(const char* format, ...);                                    // generic logger for use by developers, with default level of 'DEBUG' and origin "PLog"
void Write(u8 level, const char* format, ...);                           // custom level, default origin "PLog"
void Write(const char* origin, const char* format, ...);                 // custom origin, default level 'DEBUG'
void Write(u8 level, const char* origin, const char* format, ...);      // fully customizable logger
void Exit();

}