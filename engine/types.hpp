//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef P_TYPES
#define P_TYPES

#include <stdint.h>

using namespace std;

typedef uint8_t  BYTE;

typedef uint16_t WORD;

#ifdef _WIN32
    typedef unsigned long DWORD;
#else
    typedef uint32_t DWORD;
#endif

#endif
