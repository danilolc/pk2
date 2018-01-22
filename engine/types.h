//#########################
//PisteEngine
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef P_TYPES
#define P_TYPES

#define PATH_SIZE 128

#include <stdint.h>
#include "../../../../../../opt/android-sdk/ndk-bundle/sources/cxx-stl/gnu-libstdc++/4.9/include/cstdint"

typedef uint8_t  BYTE;
typedef uint16_t WORD;
#ifdef _WIN32
typedef unsigned long DWORD;
#else
typedef uint32_t DWORD;
#endif

#endif
