//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PFile.hpp"

#include "engine/PLog.hpp"
#include "engine/PUtils.hpp"
#include "engine/platform.hpp"

#include <cstring>
#include <sys/stat.h>

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

#include <SDL.h>

#ifdef PK2_USE_ZIP
#include <zip.h>
#endif

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

namespace PFile {

struct Zip {

	#ifdef PK2_USE_ZIP
	std::string name;
	zip_t *zip;
	zip_source* src;
	#endif

};

void CloseZip(Zip* zp) {

	#ifdef PK2_USE_ZIP
	if (zp) {
	
		//zip_close(zp->zip);
		zip_source_close(zp->src);
		zip_discard(zp->zip);
		

	
		delete zp;
		
	}
	#endif

}

Zip* OpenZip(std::string path) {

	#ifdef PK2_USE_ZIP
	
	SDL_RWops* rw = SDL_RWFromFile(path.c_str(), "r");
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

void Path::FixSep() {

	const char* nosep = PE_NOSEP;
	const char* sep = PE_SEP;
	std::replace(this->path.begin(), this->path.end(), nosep[0], sep[0]);

}

Path::Path(std::string path) {

	path = path.substr(0, path.find_last_not_of(" ") + 1);
    this->path = path;
	
	this->zip_file = nullptr;

	this->FixSep();

}

Path::Path(Zip* zip_file, std::string path) {

	if(zip_file == nullptr) {

		PLog::Write(PLog::ERR, "PFile", "No zip file for %s\n", path.c_str());

	}

	path = path.substr(0, path.find_last_not_of(" ") + 1);
    this->path = path;
	
    this->zip_file = zip_file;

	this->FixSep();

}

Path::Path(Path path, std::string file) {

	*this = path;
	
	file = file.substr(0, file.find_last_not_of(" ") + 1);
	this->path += file;
	
	this->FixSep();

}

Path::~Path() {

}

bool Path::operator==(Path path) { //needed?

	bool a = this->zip_file == path.zip_file; //compare with zip_file name?
	bool b = this->path == path.path;

	return a && b;

}

const char* Path::c_str() {

	return path.c_str();
	
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

#ifdef PK2_USE_ZIP

static int zip_get_index(zip_t* z, const char* filename, int* size) {

	struct zip_stat st;
    zip_stat_init(&st);

	int sz = zip_get_num_entries(z, 0);
    for (int i = 0; i < sz; i++) {

		zip_stat_index(z, i, 0, &st);

		if ( PUtils::NoCaseCompare(st.name, filename) ) {
			
			*size = st.size;
			return i;

		}

	}

	return -1;

}

static bool pathcomp(const char* path, const char* entry) {

	while(*path != '\0') {

		char a = *path;
		char b = *entry;

		if (a == '\\') a = '/';
		if (b == '\\') b = '/';

		if ( (a | ' ') != (b | ' ') )
			return false;
		
		path++;
		entry++;

	}

	return true;

}

std::vector<std::string> scan_zip(Zip* zip_file, const char* path, const char* type) {

    std::vector<std::string> result;

    int path_size = strlen(path);

    struct zip_stat st;
    zip_stat_init(&st);
    
    int sz = zip_get_num_entries(zip_file->zip, 0);
    for (int i = 0; i < sz; i++) {
        
        zip_stat_index(zip_file->zip, i, 0, &st);

		if( pathcomp(path, st.name) ) {

			std::string filename(st.name + path_size + 1);
			filename = filename.substr(0, filename.find("/")); //PE_SEP?

			if(filename.size() == 0)
				continue;
			
            if(is_type(filename.c_str(), type)) {

				//needed?
				bool repeated = false;
				for (std::string st : result)
					if (st == filename) {
						repeated = true;
						break;
					}
				
				if (!repeated)
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
			PLog::Write(PLog::DEBUG, "PFile", "Found on %s", this->path.c_str());

			return true;
		}

	}

	this->SetFile(filename);
	PLog::Write(PLog::INFO, "PFile", "%s not found", this->path.c_str());

	return false;

}

#ifdef __ANDROID__

std::vector<std::string> scan_apk(const char* dir, const char* type) {

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

	return result;

}

#endif

#ifdef _WIN32

std::vector<std::string> scan_file(const char* dir, const char* type) {
	
	std::vector<std::string> result;
    struct _finddata_t map_file;
	bool searching_folder = false;

	std::string buffer(dir);

	if (type[0] == '/' || type[0] == '\\') {

        buffer += "\\*";
		searching_folder = true;
	
    } else if (type[0] == '\0') {

        buffer += "\\*";
	
    } else {
    
        buffer += "\\*";
        buffer += type;
    
    }

	long hFile = _findfirst(buffer.c_str(), &map_file);
	while (hFile != -1) {

		if (map_file.name[0] != '.') {

			bool is_folder = map_file.attrib & _A_SUBDIR;
			if (is_folder == searching_folder) {

				result.push_back(map_file.name);

			}

		}

		if( _findnext( hFile, &map_file ) != 0 )
			break;

	}

	_findclose( hFile );

    PLog::Write(PLog::DEBUG, "PFile", "Scanned on \"%s\" for \"%s\". Found %i matches", dir, type, (int)result.size());

	return result;

}

bool Path::Find() {

	if (this->zip_file != nullptr)
		return this->NoCaseFind();

	const char* cstr = this->c_str();
	PLog::Write(PLog::DEBUG, "PFile", "Find %s", cstr);

	/*
	int sz = strlen(filename);
	for (int i = 0; i < sz; i++)
		if (filename[i] == '/')
			filename[i] = '\\';*/

	std::ifstream ifile(cstr);
	if (!(bool)ifile
		/*INVALID_FILE_ATTRIBUTES == GetFileAttributes(cstr) && GetLastError() == ERROR_FILE_NOT_FOUND*/) {

		PLog::Write(PLog::INFO, "PFile", "%s not found", cstr);
		return false;

	}

	PLog::Write(PLog::DEBUG, "PFile", "Found on %s", cstr);
	return true;

}

#else

std::vector<std::string> scan_file(const char* dir, const char* type) {

	std::vector<std::string> result;
	struct dirent **namelist;

	int numb = scandir(dir, &namelist, 0, alphasort);
	
	if (numb == -1) {

		PLog::Write(PLog::WARN, "PFile", "Couldn't scan on \"%s\"", dir);
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

	}

	for( int i = 0; i < numb; i++ ) 
		free(namelist[i]);

	free(namelist);

	PLog::Write(PLog::DEBUG, "PFile", "Scanned on \"%s\" for \"%s\". Found %i matches", dir, type, (int)result.size());
	
	return result;
    
}

bool Path::Find() {

	// Scan dir on ZIP
	if (this->zip_file != nullptr)
		return this->NoCaseFind();
	
	#ifdef __ANDROID__

	// Scan dir on APK
	if (c_str()[0] != '/')
		return this->NoCaseFind();

	#endif

	const char* cstr = this->path.c_str();

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

bool Path::Is_Zip() {

	return this->zip_file != nullptr;

}

int Path::SetFile(std::string file) {

	int dif = this->path.find_last_of(PE_SEP);

	file = file.substr(0, file.find_last_not_of(" ") + 1);

	this->path = this->path.substr(0, dif + 1) + file;

	return 0;

}

int Path::SetPath(std::string path) {

	int s = path.size();
	if (s > 0)
		if (path[s-1] != PE_SEP[0] && path[s-1] != PE_NOSEP[0])
			path += PE_SEP;

	this->path = path + this->GetFileName();
	this->FixSep();

	return 0;

}

std::string Path::GetDirectory() {

	int dif = this->path.find_last_of(PE_SEP);
	return this->path.substr(0, dif);

}

std::string Path::GetFileName() {

	int dif = this->path.find_last_of(PE_SEP);
	return this->path.substr(dif + 1);

}

//SDL_RW interface --------
extern "C" {
static int SDLCALL pfile_mem_close(SDL_RWops* context) {

	if (context) {

		SDL_free(context->hidden.mem.base);
		SDL_FreeRW(context);

	}

	return 0;

}

#ifdef PK2_USE_ZIP
static Sint64 SDLCALL pfile_zip_size(SDL_RWops* context) {

	return (Sint64)context->hidden.unknown.data2;

}

static Sint64 SDLCALL pfile_zip_seek(SDL_RWops* context, Sint64 offset, int whence) {

	zip_file_t* zfile = (zip_file_t*)context->hidden.unknown.data1;

	switch (whence) {
		case RW_SEEK_SET:
			zip_fseek(zfile, offset, SEEK_SET);
		case RW_SEEK_CUR:
			zip_fseek(zfile, offset, SEEK_CUR);
		case RW_SEEK_END:
			zip_fseek(zfile, offset, SEEK_END);
		default:
			return SDL_SetError("Unknown value for 'whence'");
    }

	return (Sint64)zip_ftell(zfile);

}

static size_t SDLCALL pfile_zip_read(SDL_RWops* context, void *ptr, size_t size, size_t maxnum) {

	zip_file_t* zfile = (zip_file_t*)context->hidden.unknown.data1;

	return zip_fread(zfile, ptr, size * maxnum);

}

static size_t SDLCALL pfile_zip_write(SDL_RWops* context, const void *ptr, size_t size, size_t num) {

	SDL_SetError("Can't write zip RW");

	return 0;

}

static int SDLCALL pfile_zip_close(SDL_RWops* context) {

	zip_file_t* zfile = (zip_file_t*)context->hidden.unknown.data1;
	zip_fclose(zfile);
	SDL_FreeRW(context);

	return 0;

}
#endif
}
//-------------------------

RW* Path::GetRW(const char* mode) {

	SDL_RWops* ret;

	const char* cstr = this->path.c_str();

	if (this->zip_file != nullptr) {

		#ifdef PK2_USE_ZIP
		
		int size;
		int index = zip_get_index(this->zip_file->zip, cstr, &size);
		if (index < 0) {

			PLog::Write(PLog::ERR, "PFile", "Can't get RW from zip \"%s\", file \"%s\"", this->zip_file->name.c_str(), cstr);
			return nullptr;

		}

		zip_file_t* zfile = zip_fopen_index(this->zip_file->zip, index, 0);
		if (!zfile) {

			PLog::Write(PLog::ERR, "PFile", "RW from zip \"%s\", file \"%s\" is NULL", this->zip_file->name.c_str(), cstr);
			return nullptr;

		}

		/*ret = SDL_AllocRW(); // TODO - why isn't it working? (maybe I can't open two files from the same zip)
		if (ret == nullptr) {

			PLog::Write(PLog::ERR, "PFile", "Can't allocate RW");
			return nullptr;

		}

		ret->size = pfile_zip_size;
		ret->seek = pfile_zip_seek;
		ret->read = pfile_zip_read;
		ret->write = pfile_zip_write;
		ret->close = pfile_zip_close;
		ret->hidden.unknown.data1 = zfile;
		ret->hidden.unknown.data2 = (void*)size;
		ret->type = 0;
				
		return (RW*)ret;*/

		void* buffer = SDL_malloc(size);
		zip_fread(zfile, buffer, size);
		zip_fclose(zfile);

		ret = SDL_RWFromConstMem(buffer, size);
		if (!ret) {

			return nullptr;
		
		}

		ret->close = pfile_mem_close;
		return (RW*)ret;

		#else

		return nullptr;
		
		#endif
		
	}
	
	#ifdef __ANDROID__
	if (cstr[0] != '/') { //From APK --TODO why?

		SDL_RWops* temp = SDL_RWFromFile(cstr, mode);
		if (!temp) {

			PLog::Write(PLog::ERR, "PFile", "Can't get RW from apk file \"%s\"", cstr);
			return nullptr;

		}

		int size = SDL_RWsize(temp);
		void* buffer = SDL_malloc(size);

		SDL_RWread(temp, buffer, size, 1);
		SDL_RWclose(temp);

		ret = SDL_RWFromConstMem(buffer, size);
		if (!ret) {

			return nullptr;
		
		}

		ret->close = pfile_mem_close;
		return (RW*)ret;
	
	}
	#endif

	ret = SDL_RWFromFile(cstr, mode);
	if (!ret) {

		PLog::Write(PLog::ERR, "PFile", "Can't get RW from file \"%s\"", cstr);
		return nullptr;

	}

	return (RW*)ret;

}

int RW::read(std::string& str) {

	str.clear();

	while(1) {
		u8 c = SDL_ReadU8((SDL_RWops*)this);

		if (c == '\0')
			return str.size();
		
		str += c;
	
	}

}

int RW::read(void* val, size_t size) {

	return SDL_RWread((SDL_RWops*)this, val, 1, size);

}
int RW::read(bool& val) {

	u8 v = SDL_ReadU8((SDL_RWops*)this);
	
	if (v == 0) val = false;
	else val = true;

	return 1;

}
int RW::read(u8& val) {

	val = SDL_ReadU8((SDL_RWops*)this);
	return 1;

}
int RW::read(s8& val) {

	val = SDL_ReadU8((SDL_RWops*)this);
	return 1;

}
int RW::read(u16& val) {

	val = SDL_ReadLE16((SDL_RWops*)this);
	return 1;

}
int RW::read(s16& val) {

	val = SDL_ReadLE16((SDL_RWops*)this);
	return 1;

}
int RW::read(u32& val) {

	val = SDL_ReadLE32((SDL_RWops*)this);
	return 1;

}
int RW::read(s32& val) {

	val = SDL_ReadLE32((SDL_RWops*)this);
	return 1;

}
int RW::read(u64& val) {

	val = SDL_ReadLE64((SDL_RWops*)this);
	return 1;

}
int RW::read(s64& val) {

	val = SDL_ReadLE64((SDL_RWops*)this);
	return 1;

}

int RW::write(std::string& str) {

	return SDL_RWwrite((SDL_RWops*)this, str.c_str(), 1, str.size() + 1);

}

int RW::write(const void* val, size_t size) {

	return SDL_RWwrite((SDL_RWops*)this, val, size, 1);

}
int RW::write(bool val) {

	return SDL_WriteU8((SDL_RWops*)this, val);

}
int RW::write(u8 val) {

	return SDL_WriteU8((SDL_RWops*)this, val);

}
int RW::write(s8 val) {

	return SDL_WriteU8((SDL_RWops*)this, val);

}
int RW::write(u16 val) {

	return SDL_WriteLE16((SDL_RWops*)this, val);

}
int RW::write(s16 val) {

	return SDL_WriteLE16((SDL_RWops*)this, val);

}
int RW::write(u32 val) {

	return SDL_WriteLE32((SDL_RWops*)this, val);

}
int RW::write(s32 val) {

	return SDL_WriteLE32((SDL_RWops*)this, val);

}
int RW::write(u64 val) {

	return SDL_WriteLE64((SDL_RWops*)this, val);

}
int RW::write(s64 val) {

	return SDL_WriteLE64((SDL_RWops*)this, val);

}

size_t RW::size() {

	SDL_RWops* rwops = (SDL_RWops*) this;

	return SDL_RWsize(rwops);

}

size_t RW::to_buffer(void** buffer) {

	SDL_RWops* rwops = (SDL_RWops*) this;
	
	size_t size = SDL_RWsize(rwops);
	
	if (size == 0) {

		PLog::Write(PLog::ERR, "PFile", "RW size = 0");
		return size;

	}

	*buffer = SDL_malloc(size);

	if (*buffer) {

		this->read(*buffer, size);
		return size;

	} else {

		PLog::Write(PLog::ERR, "PFile", "Could not alloc memory");
		return 0;

	}

}

int RW::close() {

	if (this == nullptr) {

		PLog::Write(PLog::ERR, "PFile", "Tried to close a null rw");
		return -1;

	}

	SDL_RWops* rwops = (SDL_RWops*) this;
	
	int ret = SDL_RWclose(rwops);
	if (ret != 0) {
	
		PLog::Write(PLog::ERR, "PFile", "Error freeing rw");
	
	}

	return ret;

}

std::unordered_map<std::string, std::vector<std::string>> scan_cache;
//std::unordered_map<std::string, std::vector<std::string>> scan_cache_zip; //TODO

std::vector<std::string> Path::scandir(const char* type) {
    
	std::vector<std::string> ret;

	std::string dir = this->path.substr(0, this->path.find_last_of(PE_SEP));
	std::string cache_entry(dir + type);
	
	const char* cstr = dir.c_str();
	
	if (this->zip_file != nullptr) {

    	#ifdef PK2_USE_ZIP
        
		return scan_zip(this->zip_file, cstr, type);
		
		#else
		
		return ret;
		
		#endif

	}
	
	// Look the cache	
	auto it = scan_cache.find(cache_entry);
	if (it != scan_cache.end()) {

		PLog::Write(PLog::DEBUG, "PFile", "Got cache on \"%s\" for \"%s\"", cstr, type);
		return it->second;

	}

    #ifdef __ANDROID__

    if (cstr[0] != '/') {

		ret = scan_apk(cstr, type);
		scan_cache[cache_entry] = ret;
        return ret;

	}

    #endif
	
	ret = scan_file(cstr, type);
	scan_cache[cache_entry] = ret;
	return ret;

}

};