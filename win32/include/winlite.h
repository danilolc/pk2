#ifndef WINLITE_H
#define WINLITE_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define NOWINRES
#define NOSERVICE
#define NOMCX
#define NOIME

#include <windows.h>

#endif
#endif
