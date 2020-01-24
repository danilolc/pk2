#include "engine/PLog.hpp"

#include <ctime>
#include <cstdio>
#include <cstring>
#include <cstdarg>

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

#ifdef _WIN32
#define END_LINE "\r\n"
#else
#define END_LINE "\n"
#endif

namespace PLog {

static PFile::RW* log_file = NULL;
static u8 log_level = 0;

void Init(u8 level, PFile::Path file) {

    log_level = level;

    if (log_file != NULL)
        PFile::CloseRW(log_file);
    
    log_file = NULL;

    if (file.size() > 0)
        log_file = file.GetRW("wb");

}

static void write_on_file(const char* level_name, const char* origin, const char* format, va_list* args) {

    const int BUFFER_SIZE = 1024;

    char buffer[BUFFER_SIZE];
    int size;

    size = snprintf(buffer, BUFFER_SIZE, "%s", level_name + 5);
    PFile::WriteRW(log_file, buffer, size - 4);

    size = snprintf(buffer, BUFFER_SIZE, "\t%s\t- ", origin);
    PFile::WriteRW(log_file, buffer, size);

    size = vsnprintf(buffer, BUFFER_SIZE, format, *args);
    PFile::WriteRW(log_file, buffer, size);
    
    PFile::WriteRW(log_file, END_LINE, sizeof(END_LINE) - 1);

}

void Write(const char* txt) {

    Write(PLog::DEBUG, "PLog", txt);

}

void Write(u8 level, const char* origin, const char* format, ...) {

    if (!(log_level & level))
        return;

    /*time_t rawtime;
    time (&rawtime);
    struct tm* timeinfo = localtime(&rawtime);*/
    
    va_list args;
    va_start(args, format);

    const char* level_name;

    switch (level) {

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

    if (log_file != NULL) {
        
        va_end(args);
        va_start(args, format);
        write_on_file(level_name, origin, format, &args);

    }
    
    va_end(args);

}

void Exit() {

    Write(DEBUG, "PLog", "PisteLog exited");
    
    if (log_file != NULL)
        PFile::CloseRW(log_file);

    log_file = NULL;
    log_level = 0;

}

}