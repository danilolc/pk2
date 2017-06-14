// Win32 redefinitions and stuff to make MSVC happy
// Pretty lazy huh? :) Need to do this in a better way

// this silences error C4996 (eg. 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead.)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

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

#ifndef getcwd
#define getcwd _getcwd
#endif

#ifndef gcvt
#define gcvt _gcvt
#endif

#ifndef chdir
#define chdir _chdir
#endif
