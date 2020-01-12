//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
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

int Setcwd() {

	#ifdef __ANDROID__

	return 0;
	
	#else

	char* path = SDL_GetBasePath();
	
	if (path) {
		
		chdir(path);
		SDL_free(path);
	
	}

	return chdir("../res");

	#endif

}



void Lower(char* string) {

	for(; *string != '\0'; string++)
		*string |= ' ';
	
}

void RemoveSpace(char* string) {

	int len = strlen(string);

	while(string[len-2] == ' '){
		string[len-2] = '\0';
		len--;
	}

}

bool NoCaseCompare(const char* a, const char* b) {

	for (int i = 0;; i++) {

		if ((a[i] | ' ') != (b[i] | ' '))
			return false;
		if (a[i] == '\0')
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

std::vector<std::string> result_indexes;
std::vector<std::vector<std::string>> results;

std::vector<std::string>* get_result(const char* type, const char* dir) {

	std::string index = type;
	index += "$";
	index += dir;

	int size = result_indexes.size();

	for (int i = 0; i < size; i++) {
		if (result_indexes[i] == index)
			return &results[i];
	}
	return nullptr;

}

void save_result(const char* type, const char* dir, std::vector<std::string>* result) {

	std::string index = type;
	index += "$";
	index += dir;

	result_indexes.push_back(index);
	results.push_back(*result);

}


#ifdef _WIN32

void GetLanguage(char* lang) {

	lang[0] = '\0';
	//WCHAR wcBuffer[16];
	//LANGID lid = GetUserDefaultUILanguage();

}

bool Find(char* filename) {

	PLog::Write(PLog::DEBUG, "PUtils", "Find %s", filename);

	int sz = strlen(filename);

	for (int i = 0; i < sz; i++)
		if (filename[i] == '/')
			filename[i] = '\\';


	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(filename) && GetLastError() == ERROR_FILE_NOT_FOUND) {

		PLog::Write(PLog::INFO, "PUtils", "%s not found", filename);
		return false;

	}

	PLog::Write(PLog::DEBUG, "PUtils", "Found on %s", filename);
	return true;

}

int CreateDir(std::string path){
	
	return CreateDirectory(path.c_str(), NULL);
	
}

#else

#ifdef __ANDROID__

void GetLanguage(char* lang) {

	JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
	jobject activity = (jobject)SDL_AndroidGetActivity();
	jclass clazz(env->GetObjectClass(activity));
	jmethodID method_id = env->GetMethodID(clazz, "getLocale", "()Ljava/lang/String;");

	jstring language = (jstring)env->CallObjectMethod(activity, method_id);

	jboolean isCopy;
	const char* clang = env->GetStringUTFChars(language, &isCopy);

	lang[0] = clang[0] | ' '; // lower
	lang[1] = clang[1] | ' '; 
	lang[2] = '\0';

	if (isCopy == JNI_TRUE) {
    	env->ReleaseStringUTFChars(language, clang);
	}
	env->DeleteLocalRef(activity);
	env->DeleteLocalRef(clazz);

}

#else

void GetLanguage(char* lang) {

	const char* locale = std::locale("").name().c_str();

	lang[0] = locale[0] | ' '; // lower
	lang[1] = locale[1] | ' '; 
	lang[2] = '\0';

}

#endif

int CreateDir(std::string path) {

	return mkdir(path.c_str(), 0700);
	
}

//Scans directory to find file based on case
bool NoCaseFind(char *filename) {

	char dir[PE_PATH_SIZE];
	char file[PE_PATH_SIZE];

	int last = 0;
	for (int i = 0; filename[i] != '\0'; i++) {

		if (filename[i] == '\\')
			filename[i] = '/';
		if (filename[i] == '/')
			last = i;

	}

	strcpy(dir, filename);
	dir[last+1] = '\0';
	
	strcpy(file, &filename[last+1]);

	std::vector<std::string> list = PFile::Path(dir).scandir("");

	int sz = list.size();
	for(int i = 0; i < sz; i++) {
		
		const char* name = list[i].c_str();
		
		if(NoCaseCompare(name, file)) {

			strcpy(filename, dir);
			strcat(filename, name);
			PLog::Write(PLog::DEBUG, "PUtils", "Found on %s", filename);

			return true;
		}

	}

	PLog::Write(PLog::INFO, "PUtils", "%s not found", filename);

	return false;

}

bool Find(char *filename) {

	PLog::Write(PLog::DEBUG, "PUtils", "Find %s", filename);

	struct stat buffer;
	if(stat(filename, &buffer) == 0) {

		PLog::Write(PLog::DEBUG, "PUtils", "Found on %s", filename);
		return true;

	}

	PLog::Write(PLog::INFO, "PUtils", "%s not found, trying different cAsE", filename);
	
	return NoCaseFind(filename);
	
}


#endif

void Show_Error(const char* txt) {
	
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", txt, NULL);

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

}