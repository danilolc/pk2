//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PUtils.hpp"

#include "engine/PLog.hpp"
#include "engine/PFile.hpp"

#include <SDL.h>

#include <cstring>
#include <string>
#include <locale>
#include <sys/stat.h>

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

namespace PUtils {

bool force_mobile = false;

void Setcwd() {

	#ifdef __ANDROID__
	return;
	#else

	char* path = SDL_GetBasePath();
	if (path) {
		
		chdir(path);
		SDL_free(path);
	
	}

	#ifndef _WIN32
	chdir(".." PE_SEP "res");
	#endif

	path = getcwd(NULL, 0);
	if (path) {
		
		PLog::Write(PLog::DEBUG, "PUtils", "Working directory: %s", path);
		free(path);

	}

	#endif

}



void Lower(char* string) {

	for(; *string != '\0'; string++)
		*string |= ' ';
	
}

//Remove spaces at the end of string
void RemoveSpace(char* string) {

	int len = strlen(string);

	while(string[len-2] == ' '){
		string[len-2] = '\0';
		len--;
	}

}

bool NoCaseCompare(const char* a, const char* b) {

	for (int i = 0;; i++) {

		char ac = a[i];
		char bc = b[i];

		// This is used to compare paths
		if (ac == '\\') ac = '/';
		if (bc == '\\') bc = '/';

		if ((ac | ' ') != (bc | ' '))
			return false;
		if (ac == '\0')
			return true;

	}

}

int Alphabetical_Compare(const char *a, const char *b) {
	
	int a_size = strlen(a);
	int b_size = strlen(b);
	
	int min_size = a_size < b_size? a_size : b_size;

	for (int i = 0; i < min_size; i++) {
		char ac = a[i] | ' '; //lower case
		char bc = b[i] | ' ';
		if (ac > bc) 
			return 2;
		if (ac < bc)
			return 1;
	}

	if (a_size > b_size)
		return 1;

	if (a_size < b_size)
		return 2;

	return 0;
}

#ifdef _WIN32

int CreateDir(std::string path){
	
	return CreateDirectory(path.c_str(), NULL);
	
}

int RemoveDir(std::string path) {

	std::string command = "rmdir /s /q " + path;
	return system(command.c_str());

}

int RenameDir(std::string old_path, std::string new_path) {

	std::string command = "ren " + old_path + " " + new_path;
	return system(command.c_str());

}

#else

int CreateDir(std::string path) {

	return mkdir(path.c_str(), 0700);
	
}

int RemoveDir(std::string path) {

	// TODO - find a better way
	std::string command = "rm -rf " + path;
	return system(command.c_str());

}

int RenameDir(std::string old_path, std::string new_path) {

	// TODO - find a better way
	std::string command = "mv " + old_path + " " + new_path;
	return system(command.c_str());

}

#endif

void GetLanguage(char* lang) {

	#if (SDL_COMPILEDVERSION < 2014)
		#warning SDL version must be at least 2.0.14 to support locale
		const char* locale = "en";
	#else
		const char* locale = SDL_GetPreferredLocales()->language;
	#endif

	lang[0] = SDL_tolower(locale[0]);
	lang[1] = SDL_tolower(locale[1]); 
	lang[2] = '\0';

}

void Show_Error(const char* txt) {
	
	// TODO
	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERR, "Error", txt, NULL);

}

void Force_Mobile() {

	force_mobile = true;

}

bool Is_Mobile() {

	#ifdef __ANDROID__
		return true;
	#else
		return force_mobile;
	#endif

}

#ifdef __ANDROID__
bool ExternalWriteable() {

	JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
	jobject activity = (jobject)SDL_AndroidGetActivity();
	jclass clazz(env->GetObjectClass(activity));
	jmethodID method_id = env->GetMethodID(clazz, "externalPermitted", "()Z");

	jboolean retval = env->CallBooleanMethod(activity, method_id);

	env->DeleteLocalRef(activity);
	env->DeleteLocalRef(clazz);

    return (retval == JNI_TRUE);

}
#endif

}