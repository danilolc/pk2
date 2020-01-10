//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "engine/PUtils.hpp"

#include "engine/PLog.hpp"

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
	
	if (path != NULL) {
		
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

std::vector<std::string> Scandir(const char* type, const char* dir) {
	
	std::vector<std::string>* res = get_result(type, dir);
	if (res != nullptr) {
		
		PLog::Write(PLog::DEBUG, "PUtils", "Got backup from \"%s\", \"%s\"", type, dir);
		return *res;

	}

	std::vector<std::string> result;
    struct _finddata_t map_file;

	char buffer[PE_PATH_SIZE];
	if (type[0] == '/' || type[0] == '\\')
		_snprintf(buffer, sizeof(buffer), "%s\\*", dir);
	else if (type[0] == '\0')
		_snprintf(buffer, sizeof(buffer), "%s\\*", dir);
	else
		_snprintf(buffer, sizeof(buffer), "%s\\*%s", dir, type);

	long hFile = _findfirst(buffer, &map_file);
	if (hFile == -1L) {

		save_result(type, dir, &result);
		return result;

	}
	else {

		if (map_file.name[0] != '.') //No hidden files
			result.push_back(map_file.name);
	
	}

	while (1) {

		if( _findnext( hFile, &map_file ) != 0 ) //TODO - test if works
			break;

		if (map_file.name[0] != '.')
			result.push_back(map_file.name);

	}

	_findclose( hFile );

	save_result(type, dir, &result);
	return result;

}

int CreateDir(const char *path, const char* dir){
	
	std::string complete_path(path);
	if (dir)
		complete_path += dir;
	
	return CreateDirectory(complete_path.c_str(), NULL);
	
}

#else

const char* Get_Extension(const char* string) {

	int len = strlen(string);
	const char* end = string + len;
	
	for( int i = 0; i < len; i++ ) {

		if (*(end - i) == '.' 
			|| *(end - i) == '/'
			|| *(end - i) == '\\') {

			return end - i;

		}

	}

	return string;

}

char* Get_Extension(char* string) {

	int len = strlen(string);
	char* end = string + len;
	
	for( int i = 0; i < len; i++ ) {

		if (*(end - i) == '.' 
			|| *(end - i) == '/'
			|| *(end - i) == '\\') {

			return end - i;

		}

	}

	return string;

}

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

std::vector<std::string> Scandir(const char* type, const char* dir) {

	std::vector<std::string>* res = get_result(type, dir);
	if (res != nullptr) {
		
		PLog::Write(PLog::DEBUG, "PUtils", "Got backup from \"%s\", \"%s\"", type, dir);
		return *res;

	}

	JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
	jobject activity = (jobject)SDL_AndroidGetActivity();
	jclass clazz(env->GetObjectClass(activity));
	jmethodID method_id = env->GetMethodID(clazz, "listDir", "(Ljava/lang/String;)[Ljava/lang/String;");

	jstring param = env->NewStringUTF(dir);
	jobjectArray array = (jobjectArray)env->CallObjectMethod(activity, method_id, param);

	jsize size = env->GetArrayLength(array);
	
	std::vector<std::string> result;

	PLog::Write(PLog::DEBUG, "PUtils", "Scanned %s, %i files", dir, size);

	for (int i = 0; i < size; i++) {

		jstring filename = (jstring)env->GetObjectArrayElement(array, i);
		jboolean isCopy;
		const char* file = env->GetStringUTFChars(filename, &isCopy);

		if( file[0] != '.' ) {

			if(type[0] == '/' && strstr(file, ".") == NULL) { //provisory way - consider file without '.' a directory

				result.push_back(file);
				continue;

			} else if(type[0] == '\0') {
			
				result.push_back(file);
				continue;
			
			} else {

				const char* ext = Get_Extension(file);
				if(strcmp(ext, type) == 0) {

					result.push_back(file);
					continue;

				}
			}
		}

		if (isCopy == JNI_TRUE) {
    		env->ReleaseStringUTFChars(filename, file);
		}
	}


	env->DeleteLocalRef(activity);
	env->DeleteLocalRef(clazz);

	PLog::Write(PLog::DEBUG, "PUtils", "Scanned \"%s\" for \"%s\" and found %i matches", dir, type, (int)result.size());

	save_result(type, dir, &result);
	return result;

}

#else

void GetLanguage(char* lang) {

	const char* locale = std::locale("").name().c_str();

	lang[0] = locale[0] | ' '; // lower
	lang[1] = locale[1] | ' '; 
	lang[2] = '\0';

}

std::vector<std::string> Scandir(const char* type, const char* dir) {

	std::vector<std::string>* res = get_result(type, dir);
	if (res != nullptr) {
		
		PLog::Write(PLog::DEBUG, "PUtils", "Got backup from \"%s\", \"%s\"", type, dir);
		return *res;

	}

	std::vector<std::string> result;
	struct dirent **namelist;

	int numb = scandir(dir, &namelist, 0, alphasort);
	
	if (numb == -1)
		return result;

	for( int i = 0; i < numb; i++ ) {

		if( namelist[i]->d_name[0] != '.' ) {

			if(type[0] == '/' && namelist[i]->d_type == DT_DIR) {

				result.push_back(namelist[i]->d_name);
				continue;

			} else if(type[0] == '\0') {
			
				result.push_back(namelist[i]->d_name);
				continue;
			
			} else {

				char* ext = Get_Extension(namelist[i]->d_name);
				if(strcmp(ext, type) == 0) {

					result.push_back(namelist[i]->d_name);
					continue;

				}

			}
			
		}

		free(namelist[i]);

	}
	free(namelist);

	PLog::Write(PLog::DEBUG, "PUtils", "Scanned \"%s\" for \"%s\" and found %i matches", dir, type, (int)result.size());

	save_result(type, dir, &result);
	return result;

}

#endif

int CreateDir(const char *path, const char* dir) {

	std::string complete_path(path);

	if (dir)
		complete_path += dir;
	
	return mkdir(complete_path.c_str(), 0700);
	
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

	std::vector<std::string> list = Scandir("", dir);

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