// Platform defs
#ifndef PLATFORM_H
#define PLATFORM_H

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

#ifndef gcvt
#define gcvt _gcvt
#endif

#ifndef chdir
#define chdir _chdir
#endif

#endif // _WIN32 && _MSC_VER


#ifndef _MAX_PATH
#define _MAX_PATH 128
#endif

#endif // PLATFORM_H
