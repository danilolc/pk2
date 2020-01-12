#include "engine/PFile.hpp"

#include "engine/PLog.hpp"
#include "engine/platform.hpp"

#include <cstring>
#include <sys/stat.h>

#include <SDL.h>

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

namespace PFile {

struct Zip {
	zip_t *zip;
	zip_source* src;
	SDL_RWops* rw;
};

void CloseZip(Zip* zp) {

	//zip_close(zp->zip);
	//SDL_RWclose(zp->rw);
	zip_source_close(zp->src);

}

Zip* OpenZip(std::string path) {

	Zip* ret = new Zip;
	
    zip_error err;
    ret->rw = SDL_RWFromFile(path.c_str(), "r");
    if (ret->rw == NULL) {

        PLog::Write(PLog::ERROR, "PFile", "Can't open %s", path.c_str());
		delete ret;
		return nullptr;

    }

    FILE* rw_file = ret->rw->hidden.stdio.fp;
    ret->src = zip_source_filep_create(rw_file, 0, -1, &err);
    ret->zip = zip_open_from_source(ret->src, ZIP_RDONLY, &err);

    return ret;

}

Path::Path(std::string path) {

    this->path = path;
    this->is_zip = false;

}

Path::Path(Zip* zip_file, std::string path) {

    this->path = path;

    this->zip_file = zip_file;
    this->is_zip = true;

}

Path::Path(Path path, std::string file) {

    *this = path;

	this->Add(file);

}

Path::~Path() {

}

void Path::Add(std::string path) {

	this->path += path;

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
			filename = filename.substr(0, filename.find("/"));

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

    PLog::Write(PLog::DEBUG, "PFile", "Scanned ZIP on \"%s\" for \"%s\". Found %i matches", path, type, (int)result.size());
    return result;

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
	return result;

}

#else

std::vector<std::string> scan_file(const char* dir, const char* type) {

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
	return result;
    
}

#endif

std::vector<std::string> Path::scandir(const char* type) {
    
    if (this->is_zip)
        return scan_zip(this->zip_file, this->path.c_str(), type);
    
    #ifdef __ANDROID__

    if (this->path[0] != '/')
        return scan_apk(this->path.c_str(), type);

    #endif

    return scan_file(this->path.c_str(), type);

}

};