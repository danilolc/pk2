#include "engine/PFile.hpp"

#include "engine/PLog.hpp"
#include "engine/PUtils.hpp"
#include "engine/platform.hpp"

#include <cstring>
#include <sys/stat.h>

#include <string>
#include <vector>
#include <algorithm>

#include <SDL.h>

#ifndef NO_ZIP
#include <zip.h>
#endif

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

namespace PFile {

struct Zip {

	#ifndef NO_ZIP
	std::string name;
	zip_t *zip;
	zip_source* src;
	#endif

};

void CloseZip(Zip* zp) {

	#ifndef NO_ZIP
	if (zp) {
	
		//zip_close(zp->zip);
		zip_discard(zp->zip);
		
		zip_source_close(zp->src);
	
		delete zp;
		
	}
	#endif

}

Zip* OpenZip(std::string path) {

	#ifndef NO_ZIP
	SDL_RWops* rw = SDL_RWFromFile(path.c_str(), "rb");
	if (rw == NULL) {

        PLog::Write(PLog::ERR, "PFile", "Can't open %s", path.c_str());
		return nullptr;

    }

	int size = SDL_RWsize(rw);

	void* buffer = malloc(size);
	SDL_RWread(rw, buffer, size, 1);
	SDL_RWclose(rw);

	zip_error err;
	zip_source_t* src = zip_source_buffer_create(buffer, size, 1, &err);
    
    zip_t* zip = zip_open_from_source(src, ZIP_RDONLY, &err);
	

	std::string name = path.substr(path.find_last_of(PE_SEP) + 1);

	Zip* ret = new Zip;
	ret->name = name;
	
	ret->src = src;
	ret->zip = zip;

    return ret;

	#else

	return nullptr;

	#endif

}

Path::Path(std::string path) {

	path = path.substr(0, path.find_last_not_of(" ") + 1);
    this->assign(path);
    this->is_zip = false;

}

Path::Path(Zip* zip_file, std::string path) {

	path = path.substr(0, path.find_last_not_of(" ") + 1);
    this->assign(path);
	this->is_zip = true;
    this->zip_file = zip_file;

}

Path::Path(Path path, std::string file) {

	file = file.substr(0, file.find_last_not_of(" ") + 1);
	*this = path;
	*this += file;

}

Path::~Path() {

}

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

bool is_type(const char* file, const char* type) {

    if(type[0] == '\0') {

        return true;

    } else if(type[0] == '/' && strstr(file, ".") == NULL) {

        return true;

    } else {

        const char* ext = Get_Extension(file);
        if(strcmp(ext, type) == 0) {

            return true;

        }
    }

    return false;

}

#ifndef NO_ZIP

std::vector<std::string> scan_zip(Zip* zip_file, const char* path, const char* type) {

    std::vector<std::string> result;

    int path_size = strlen(path);

    struct zip_stat st;
    zip_stat_init(&st);
    
    int sz = zip_get_num_entries(zip_file->zip, 0);
    for (int i = 0; i < sz; i++) {
        
        zip_stat_index(zip_file->zip, i, 0, &st);

		if( strstr(st.name, path) == st.name ) {

			std::string filename(st.name + path_size);
			filename = filename.substr(0, filename.find("/")); //PE_SEP?

			if(filename.size() == 0)
				continue;
			
            if(is_type(filename.c_str(), type)) {

				bool has_found = false;
				for (std::string st : result)
					if (st == filename) {
						has_found = true;
						break;
					}
				
				if (!has_found)
                	result.push_back(filename);

            }

        }

    }

    PLog::Write(PLog::DEBUG, "PFile", "Scanned zip \"%s\" on \"%s\" for \"%s\". Found %i matches", zip_file->name.c_str(), path, type, (int)result.size());
    return result;

}

#endif

//Scans directory to find file based on case
bool Path::NoCaseFind() {

	std::string filename = this->GetFileName();
	this->SetFile("");

	std::vector<std::string> list = this->scandir("");

	int sz = list.size();
	for(int i = 0; i < sz; i++) {
		
		std::string name = list[i];
		
		if(PUtils::NoCaseCompare(name.c_str(), filename.c_str())) {

			this->SetFile(name);
			PLog::Write(PLog::DEBUG, "PFile", "Found on %s", this->c_str());

			return true;
		}

	}

	this->SetFile(filename);
	PLog::Write(PLog::INFO, "PFile", "%s not found", this->c_str());

	return false;

}

#ifdef __ANDROID__

std::vector<std::vector<std::string>> apk_results;
std::vector<std::string> apk_entries;

std::vector<std::string> scan_apk(const char* dir, const char* type) {

	std::string entry(dir);
	entry += "$";
	entry += type;

	int sz = apk_entries.size();
	for (int i = 0; i < sz; i++) {

		if (apk_entries[i] == entry) {

			PLog::Write(PLog::DEBUG, "PFile", "Got apk cache on \"%s\" for \"%s\"", dir, type);
			return apk_results[i];

		}

	}

	JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
	jobject activity = (jobject)SDL_AndroidGetActivity();
	jclass clazz(env->GetObjectClass(activity));
	jmethodID method_id = env->GetMethodID(clazz, "listDir", "(Ljava/lang/String;)[Ljava/lang/String;");

	jstring param = env->NewStringUTF(dir);
	jobjectArray array = (jobjectArray)env->CallObjectMethod(activity, method_id, param);

	jsize size = env->GetArrayLength(array);
	
	std::vector<std::string> result;

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

    PLog::Write(PLog::DEBUG, "PFile", "Scanned APK on \"%s\" for \"%s\". Found %i matches", dir, type, (int)result.size());

	apk_entries.push_back(entry);
	apk_results.push_back(result);
	return result;

}

#endif

std::vector<std::vector<std::string>> scan_results;
std::vector<std::string> scan_entries;

#ifdef _WIN32

std::vector<std::string> scan_file(const char* dir, const char* type) {
	
	std::string entry(dir);
	entry += "$";
	entry += type;

	int sz = scan_entries.size();
	for (int i = 0; i < sz; i++) {

		if (scan_entries[i] == entry) {

			PLog::Write(PLog::DEBUG, "PFile", "Got cache on \"%s\" for \"%s\"", dir, type);
			return scan_results[i];

		}

	}

	std::vector<std::string> result;
    struct _finddata_t map_file;

	std::string buffer(dir);

	if (type[0] == '/' || type[0] == '\\') {

        buffer += "\\*";
	
    } else if (type[0] == '\0') {

        buffer += "\\*";
	
    } else {
    
        buffer += "\\*";
        buffer += type;
    
    }

	long hFile = _findfirst(buffer.c_str(), &map_file);
	if (hFile == -1L) {

		scan_entries.push_back(entry);
		scan_results.push_back(result);
		return result;

	} else {

		if (map_file.name[0] != '.') //No hidden files
			result.push_back(map_file.name); //TODO - dirs?
	
	}

	while (1) {

		if( _findnext( hFile, &map_file ) != 0 ) //TODO - test if works
			break;

		if (map_file.name[0] != '.')
			result.push_back(map_file.name);

	}

	_findclose( hFile );

    PLog::Write(PLog::DEBUG, "PFile", "Scanned on \"%s\" for \"%s\". Found %i matches", dir, type, (int)result.size());

	scan_entries.push_back(entry);
	scan_results.push_back(result);
	return result;

}

bool Path::Find() {

	if (this->is_zip)
		return this->NoCaseFind();

	const char* cstr = this->c_str();
	PLog::Write(PLog::DEBUG, "PFile", "Find %s", cstr);

	/*
	int sz = strlen(filename);
	for (int i = 0; i < sz; i++)
		if (filename[i] == '/')
			filename[i] = '\\';*/


	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(cstr) && GetLastError() == ERROR_FILE_NOT_FOUND) {

		PLog::Write(PLog::INFO, "PFile", "%s not found", cstr);
		return false;

	}

	PLog::Write(PLog::DEBUG, "PFile", "Found on %s", cstr);
	return true;

}

#else

std::vector<std::string> scan_file(const char* dir, const char* type) {

	
	std::string entry(dir);
	entry += "$";
	entry += type;

	int sz = scan_entries.size();
	for (int i = 0; i < sz; i++) {

		if (scan_entries[i] == entry) {

			PLog::Write(PLog::DEBUG, "PFile", "Got cache on \"%s\" for \"%s\"", dir, type);
			return scan_results[i];

		}

	}

	std::vector<std::string> result;
	struct dirent **namelist;

	int numb = scandir(dir, &namelist, 0, alphasort);
	
	if (numb == -1) {

		PLog::Write(PLog::WARN, "PFile", "Can't scan \"%s\"", dir);
		
		scan_entries.push_back(entry);
		scan_results.push_back(result);
		return result;
	
	}

	for( int i = 0; i < numb; i++ ) {

		if( namelist[i]->d_name[0] != '.' ) {

			if(type[0] == '/' && namelist[i]->d_type == DT_DIR) {

				result.push_back(namelist[i]->d_name);
				continue;

			} else if(type[0] == '\0') {
			
				result.push_back(namelist[i]->d_name);
				continue;
			
			} else {

				const char* ext = Get_Extension(namelist[i]->d_name);
				if(strcmp(ext, type) == 0) {

					result.push_back(namelist[i]->d_name);
					continue;

				}

			}
			
		}

		free(namelist[i]);

	}
	free(namelist);

	PLog::Write(PLog::DEBUG, "PFile", "Scanned on \"%s\" for \"%s\". Found %i matches", dir, type, (int)result.size());
	
	scan_entries.push_back(entry);
	scan_results.push_back(result);
	return result;
    
}

bool Path::Find() {

	// Scan dir on ZIP
	if (this->is_zip)
		return this->NoCaseFind();
	
	#ifdef __ANDROID__

	// Scan dir on APK
	if (c_str()[0] != '/')
		return this->NoCaseFind();

	#endif

	const char* cstr = this->c_str();

	PLog::Write(PLog::DEBUG, "PFile", "Find %s", cstr);

	struct stat buffer;
	if(stat(cstr, &buffer) == 0) {

		PLog::Write(PLog::DEBUG, "PFile", "Found on %s", cstr);
		return true;

	}

	PLog::Write(PLog::INFO, "PFile", "%s not found, trying different cAsE", cstr);
	
	return this->NoCaseFind();
	
}

#endif

int Path::SetFile(std::string file) {

	int dif = this->find_last_of(PE_SEP);

	file = file.substr(0, file.find_last_not_of(" ") + 1);

	this->assign(this->substr(0, dif + 1) + file);

	return 0;

}

int Path::SetPath(std::string path) {

	this->assign(path + this->GetFileName());

	return 0;

}

std::string Path::GetFileName() {

	int dif = this->find_last_of(PE_SEP);
	return this->substr(dif + 1);

}

RW* Path::GetRW(const char* mode) {

	SDL_RWops* ret;

	const char* cstr = this->c_str();

	if (this->is_zip) {

		#ifndef NO_ZIP
		struct zip_stat st;
		zip_stat_init(&st);
		if (zip_stat(this->zip_file->zip, cstr, 0, &st) == -1) {

			PLog::Write(PLog::ERR, "PFile", "Can't get RW from zip \"%s\", file \"%s\"", this->zip_file->name.c_str(), cstr);
			return nullptr;

		}

		zip_file_t* zfile = zip_fopen(this->zip_file->zip, cstr, 0);

		if (!zfile) {

			PLog::Write(PLog::ERR, "PFile", "RW from zip \"%s\", file \"%s\" is NULL", this->zip_file->name.c_str(), cstr);
			return nullptr;

		}

		void* buffer = SDL_malloc(st.size);
		zip_fread(zfile, buffer, st.size);
		zip_fclose(zfile);

		ret = SDL_RWFromConstMem(buffer, st.size);
		ret->hidden.unknown.data1 = buffer;

		return (RW*)ret;

		#else

		return nullptr;
		
		#endif
		
	}
	
	#ifdef __ANDROID__
	if (cstr[0] != '/') { //From APK

		SDL_RWops* temp = SDL_RWFromFile(cstr, mode);
		if (!temp) {

			PLog::Write(PLog::ERR, "PFile", "Can't get RW from file \"%s\"", cstr);
			return nullptr;

		}

		int size = SDL_RWsize(temp);
		void* buffer = SDL_malloc(size);

		SDL_RWread(temp, buffer, size, 1);
		SDL_RWclose(temp);

		ret = SDL_RWFromConstMem(buffer, size);
		ret->hidden.unknown.data1 = buffer;

		return (RW*)ret;
	
	}
	#endif

	ret = SDL_RWFromFile(cstr, mode);
	
	if (!ret) {

		PLog::Write(PLog::ERR, "PFile", "Can't get RW from file \"%s\"", cstr);
		return nullptr;

	}

	ret->hidden.unknown.data1 = nullptr;
	return (RW*)ret;

}

int WriteRW(RW* rw, const void* buffer, int len) {

	SDL_RWops* rwops = (SDL_RWops*) rw;

	if (rwops->hidden.unknown.data1 != nullptr) {
	
		// This log may call this function again, doing a infinite loop
		PLog::Write(PLog::ERR, "PFile", "Can't write const RW");
		return 0;
	
	}

	return SDL_RWwrite(rwops, buffer, len, 1);

}

int ReadRW(RW* rw, void* buffer, int len) {

	SDL_RWops* rwops = (SDL_RWops*) rw;
	return SDL_RWread(rwops, buffer, len, 1);

}

int CloseRW(RW* rw) {

	if (!rw) {

		PLog::Write(PLog::ERR, "PFile", "Tried to close a NULL rw");
		return -1;

	}

	SDL_RWops* rwops = (SDL_RWops*) rw;

	if (rwops->hidden.unknown.data1 != nullptr)
		SDL_free(rwops->hidden.unknown.data1);
	
	int ret = SDL_RWclose(rwops);

	if (ret != 0) {

		PLog::Write(PLog::ERR, "PFile", "Error freeing rw");

	}

	return ret;

}

std::vector<std::string> Path::scandir(const char* type) {
    
	const char* cstr = this->c_str();
	
	if (this->is_zip) {

    	#ifndef NO_ZIP
        
		return scan_zip(this->zip_file, cstr, type);
		
		#else
		
		std::vector<std::string> ret;
		return ret;
		
		#endif

	}
	
    #ifdef __ANDROID__

    if (cstr[0] != '/')
        return scan_apk(cstr, type);

    #endif

    return scan_file(cstr, type);

}

};