// Platform defs
#pragma once

#include "types.hpp"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>
#include <fstream>

#include <cmath> //sin cos
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#ifdef _WIN32
    #include <direct.h>
#elif __linux__ || __APPLE__
    #include <unistd.h> //getcwd chdir
#endif

#ifdef __ANDROID__
    #include <android/log.h>
    #undef printf
    #define printf(...) __android_log_print(ANDROID_LOG_DEBUG, "PK2", __VA_ARGS__)

    #ifndef main
        extern "C" int SDL_main(int argc, char *argv[]);
        #define main SDL_main
    #endif
#endif

#ifdef _WIN32
    #define PE_SEP "\\"
#else
    #define PE_SEP "/"
#endif

#define PE_PATH_SIZE 128

// Win32 redefinitions and stuff to make MSVC happy
#if defined (_WIN32) && defined (_MSC_VER)

// silence macro definition warnings (C4005)
#pragma warning(disable: 4005)

// redefinitions to silence error C4996 (The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name)
#ifndef itoa
#define itoa _itoa
#endif

#ifndef ltoa
#define ltoa _ltoa
#endif

#ifndef strdup
#define strdup _strdup
#endif


// require #include <direct.h>
#ifndef getcwd
#define getcwd _getcwd
#endif

#ifndef chdir
#define chdir _chdir
#endif

#endif // _WIN32 && _MSC_VER
