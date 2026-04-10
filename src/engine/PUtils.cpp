//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "PUtils.hpp"

#include "PLog.hpp"
#include "PFile.hpp"

#include <SDL.h>
#include <filesystem>
#include <cstring>
#include <string>
#include <locale>
#include <sys/stat.h>

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

namespace PUtils {

void GetLanguage(char* lang) {

	#if (SDL_COMPILEDVERSION < 2014)
		#warning SDL version must be at least 2.0.14 to support locale
		const char* locale = "en";
	#else
		// Abdullah: should've considered that SDL_GetPrefferedLocales() returns NULL on error (such as if a system doesn't have a locale, like WSL)
		char locale[5] = "en";
		SDL_Locale* locales = SDL_GetPreferredLocales();
		if (locales)
		{
			locale[0] = locales[0].language[0]; // there should be a better way to do this but meh,
			locale[1] = locales[0].language[1]; 
			// heh there was a memory leak here interesting, good thing this function is called only once
		}
		SDL_free(locales); 
	#endif
	lang[0] = SDL_tolower(locale[0]);
	lang[1] = SDL_tolower(locale[1]);     // there should be a better way to do this but meh,
	lang[2] = '\0';
}

}