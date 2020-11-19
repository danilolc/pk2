//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#define PK2_NAME "Pekka Kana 2"

#define PK2_VERSION "1.3.5"
#define PK2_VERNUM 0x0306
#define PK2_VER_MAJOR 1
#define PK2_VER_MINOR 3
#define PK2_VER_REVISION 5

#define PK2_VERSION_NAME " (r3)"

#ifdef __ANDROID__
#define PK2_PLATFORM " (Android)"
#elif _WIN32
#define PK2_PLATFORM " (Windows)"
#elif __linux
#define PK2_PLATFORM " (Linux)"
#else
#define PK2_PLATFORM " (Unknown)"
#endif

#ifdef PK2_USE_ZIP
#define PK2_ZIP_STR " (zip)"
#else
#define PK2_ZIP_STR " (no-zip)"
#endif

#ifdef PK2_PORTABLE
#define PK2_PORTABLE_STR " (portable)"
#else
#define PK2_PORTABLE_STR ""
#endif

#ifdef PK2_NO_THREAD
#define PK2_NO_THREAD_STR " (no thread)"
#else
#define PK2_NO_THREAD_STR ""
#endif

#define PK2_VERSION_STR PK2_VERSION PK2_VERSION_NAME PK2_PLATFORM PK2_ZIP_STR PK2_PORTABLE_STR PK2_NO_THREAD_STR