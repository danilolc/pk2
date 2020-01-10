#include "engine/PLog.hpp"

#include <ctime>
#include <cstdio>
#include <SDL_rwops.h>

#ifdef __ANDROID__
#include <android/log.h>
#include <string>
#endif

#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_FATAL   "\x1b[41m"
#define ANSI_COLOR_RESET   "\x1b[0m"

namespace PLog {

static SDL_RWops* log_file = NULL;
static u8 log_level = 0;

void Init(u8 level, const char* file) {

    if (file != NULL)
        log_file = SDL_RWFromFile(file, "w+");
    
    log_level = level;

}

void Write(u8 level, const char* origin, const char* format, ...) {

    if (!(log_level & level))
        return;

    /*time_t rawtime;
    time (&rawtime);
    struct tm* timeinfo = localtime(&rawtime);*/
    
    va_list args;
    va_start(args, format);

    #ifdef __ANDROID__

    int android_level;

    switch (level) {

        case DEBUG:
            android_level = ANDROID_LOG_DEBUG; break;
        case INFO:
            android_level = ANDROID_LOG_INFO; break;
        case WARN:
            android_level = ANDROID_LOG_WARN; break;
        case ERROR:
            android_level = ANDROID_LOG_ERROR; break;
        case FATAL:
            android_level = ANDROID_LOG_FATAL; break;
        default:
            android_level = ANDROID_LOG_UNKNOWN; break;

    }

    __android_log_vprint(android_level, origin, format, args);

    #else
    
    const char* level_name;

    switch (level) {

        case DEBUG:
            level_name = ANSI_COLOR_BLUE "[DEBUG]" ANSI_COLOR_RESET; break;
        case INFO:
            level_name = ANSI_COLOR_GREEN "[INFO]" ANSI_COLOR_RESET; break;
        case WARN:
            level_name = ANSI_COLOR_YELLOW "[WARN]" ANSI_COLOR_RESET; break;
        case ERROR:
            level_name = ANSI_COLOR_RED "[ERROR]" ANSI_COLOR_RESET; break;
        case FATAL:
            level_name = ANSI_COLOR_FATAL "[FATAL]" ANSI_COLOR_RESET; break;
        default:
            level_name = "[UNKNOWN]"; break;

    }

    printf("%s\t%s\t- ", level_name, origin);
    vprintf(format, args);
    printf("\n");

    #endif

    va_end(args);

}

void Exit() {

    Write(DEBUG, "PLog", "PisteLog exited.");
    
    if (log_file != NULL)
        SDL_RWclose(log_file);

    log_file = NULL;
    log_level = 0;

}

}