//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PLog.hpp"

#include "PFilesystem.hpp"

#include <stdexcept>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <cstdarg>

#ifdef __ANDROID__
#include <android/log.h>
#include <string>
#endif

#include <SDL_mutex.h>

#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_FATAL   "\x1b[41m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#ifdef _WIN32
#define END_LINE "\r\n"
#include <windows.h>
#else
#define END_LINE "\n"
#endif

namespace PLog {

static PFile::RW* log_file = nullptr;
static u8 log_level = 0;

static bool print_to_stdout = false;
static bool print_to_file = false;

/**
 * @brief 
 * Is it really necessary to have a mutex for only one thread?
 */
static SDL_mutex* mutex = nullptr;

void Init(u8 level, bool _print_to_stdout, bool _print_to_file) {

    print_to_file = _print_to_file;
    print_to_stdout = _print_to_stdout;

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    if (mutex == nullptr){
        mutex = SDL_CreateMutex();
    }

    log_level = level;

    if(log_file!=nullptr){
        throw std::runtime_error("Log file already opened!");
    }

    if(print_to_file){
        PFile::Path file = PFilesystem::GetDataFileW("log.txt");
        log_file = new PFile::RW(file.GetRW2("w"));
    }
}

static void write_on_file(const char* level_name, const char* origin, const char* format, va_list* args) {

    const int BUFFER_SIZE = 1024;

    char buffer[BUFFER_SIZE];
    int size;

    size = snprintf(buffer, BUFFER_SIZE, "%s", level_name + 5);
    log_file->write(buffer, size - 4);

    size = snprintf(buffer, BUFFER_SIZE, "\t%s\t- ", origin);
    log_file->write(buffer, size);

    size = vsnprintf(buffer, BUFFER_SIZE, format, *args);
    log_file->write(buffer, size);
    
    log_file->write(END_LINE, sizeof(END_LINE) - 1);

}

void va_Write(u8 level, const char* origin, const char* format, va_list args)
{
    if (!(log_level & level))
        return;

    va_list args_file;   // remains uninitialized!! dont touch! because  
    if (log_file != nullptr) 
        va_copy(args_file, args);  // for use in write_on_file, this call is potentially expensive
    
    SDL_LockMutex(mutex);

    /*time_t rawtime;
    time (&rawtime);
    struct tm* timeinfo = localtime(&rawtime);*/
    const char* level_name;
    switch (level) 
    {
        case DEBUG:
            level_name = ANSI_COLOR_BLUE "[DEBUG]" ANSI_COLOR_RESET; break;
        case INFO:
            level_name = ANSI_COLOR_GREEN "[INFO]" ANSI_COLOR_RESET; break;
        case WARN:
            level_name = ANSI_COLOR_YELLOW "[WARN]" ANSI_COLOR_RESET; break;
        case ERR:
            level_name = ANSI_COLOR_RED "[ERR]" ANSI_COLOR_RESET; break;
        case FATAL:
            level_name = ANSI_COLOR_FATAL "[FATAL]" ANSI_COLOR_RESET; break;
        default:
            level_name = "[UNKNOWN]"; break;
    }

    if (print_to_stdout) 
    {    
        #ifdef __ANDROID__

        int android_level;

        switch (level) {

            case DEBUG:
                android_level = ANDROID_LOG_DEBUG; break;
            case INFO:
                android_level = ANDROID_LOG_INFO; break;
            case WARN:
                android_level = ANDROID_LOG_WARN; break;
            case ERR:
                android_level = ANDROID_LOG_ERROR; break;
            case FATAL:
                android_level = ANDROID_LOG_FATAL; break;
            default:
                android_level = ANDROID_LOG_UNKNOWN; break;

        }

        __android_log_vprint(android_level, origin, format, args);

        #else

        printf("%s\t%s\t- ", level_name, origin);
        vprintf(format, args);
        printf("\n");

        #endif

    }

    if (log_file != nullptr) 
    {
        write_on_file(level_name, origin, format, &args_file);
        
        va_end(args_file); // va_list should be ended by the function that called va_start
    }
    SDL_UnlockMutex(mutex);
}


void Write(u8 level, const char* origin, const char* format, ...) 
{    
    va_list args;
    va_start(args, format);

    va_Write(level, origin, format, args);

    va_end(args);
}

void Write(const char* format, ...)
{
    u8 level = PLog::DEBUG;
    const char *origin = "PLog";
    va_list args;
    va_start(args, format);

    va_Write(level, origin, format, args);

    va_end(args);
}

void Write(u8 level, const char* format, ...)
{
    const char *origin = "PLog";
    va_list args;
    va_start(args, format);

    va_Write(level, origin, format, args);

    va_end(args);

}

void Write(const char *origin, const char* format, ...)
{
    u8 level = PLog::DEBUG;
    va_list args;
    va_start(args, format);

    va_Write(level, origin, format, args);

    va_end(args);

}

void Exit() {

    Write(DEBUG, "PLog", "Terminated");

    SDL_DestroyMutex(mutex);
    mutex = nullptr;
    
    if (log_file != nullptr){
        delete log_file;
        log_file = nullptr;
    }
    log_level = 0;
}

}