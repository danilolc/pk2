//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief 
 * New filesystem utils by SaturninTheAlien to replace obsolete ones from PFile.cpp
 */

#include "PFilesystem.hpp"
#include "PString.hpp"
#include "PLog.hpp"

#include <SDL.h>
#include <algorithm>
#include <filesystem>
#include <sstream>


#ifdef __ANDROID__
#include <jni.h>
#endif

namespace fs = std::filesystem;
namespace PFilesystem{

const std::string EPISODES_DIR = "episodes";
const std::string GFX_DIR = "gfx";

const std::string TILES_DIR = (fs::path(GFX_DIR) / "tiles").string();
const std::string SCENERY_DIR = (fs::path(GFX_DIR) / "scenery").string();

const std::string LANGUAGE_DIR = "language";
const std::string FONTS_DIR = (fs::path(LANGUAGE_DIR) / "fonts").string();

const std::string SFX_DIR = "sfx";
const std::string SPRITES_DIR = "sprites";
const std::string MUSIC_DIR = "music";

const std::string LUA_DIR = "lua";
const std::string LIFE_DIR = "rle";

static fs::path mAssetsPath;
static fs::path mDataPath;
static fs::path mEpisodePath;

//static std::string mEpisodeName;
static PZip::PZip* mEpisodeZip;


static bool mAssetsPathSet = false;
static bool mDataPathSet = false;

void CreateDirectory(const std::string& path_in){
    fs::path p(path_in);

    /*if(!p.is_absolute()){
        p = mAssetsPath / p;
    }*/

	if(!std::filesystem::exists(p) || !std::filesystem::is_directory(p)){
		std::filesystem::create_directory(p);
	}
}




void SetAssetsPath(const std::string& name){

    mAssetsPathSet = true;
    fs::path p = name;

    fs::path p1 = p / "gfx" / "pk2stuff.bmp";

    if(fs::exists(p1)){
        mAssetsPath = p;   
    } else {

        fs::path p2 = p / "res" / "gfx" / "pk2stuff.bmp";
        if(fs::exists(p2)){
            mAssetsPath = p / "res";
        }
        else{
            std::ostringstream os;
            os<<"Incorrect assets path: \""<<name<<"\"";

            throw PFile::PFileException(os.str());
        }
    }
}

void SetDataPath(const std::string& name){
    mDataPath = name;
    mDataPathSet = true;

    /**
     * @brief 
     * Create the directories if they don't exist.
     */

    CreateDirectory(mDataPath.string());
    CreateDirectory( (mDataPath / "scores").string());
    CreateDirectory( (mDataPath / "mapstore").string());
    CreateDirectory( (mDataPath / "saves").string());
    CreateDirectory( (mDataPath / "screenshots").string());
    CreateDirectory( (mDataPath / "checkpoint").string());
    CreateDirectory( (mDataPath / "episodes").string());


    //TODO
    //throw exception if the directory is not writeable

}

void SetPrefDataPath(){
    char* data_path_p = SDL_GetPrefPath("piste-gamez", "pekka-kana-2");
    if(data_path_p==nullptr){
        std::ostringstream os;
        os<<"SDL_GetPrefPath failed: "<<SDL_GetError();
        throw PFile::PFileException(os.str());
    }

    SetDataPath(data_path_p);
    SDL_free(data_path_p);
}

fs::path getBasePath(){
    char* c_path = SDL_GetBasePath();
	if(c_path==nullptr){

        std::ostringstream os;

        os<<"Cannot get the base path\n";
        os<<SDL_GetError();

        throw PFile::PFileException(os.str());
	}

    fs::path result = c_path;
    SDL_free(c_path);

    return result;
}

void SetDefaultPaths(){
    
#ifdef __ANDROID__
 /**
  * Android
  */
    if(!mAssetsPathSet){
        mAssetsPath = "";
        mAssetsPathSet = true;
    }

    if(!mDataPathSet){
        SetDataPath(SDL_AndroidGetInternalStoragePath());
    }

#else

    bool portable = true;

#ifdef _WIN32
    /**
     * Windows
     */

    if(!mAssetsPathSet){
        SetAssetsPath(getBasePath().string());
    }

    #ifdef PK2_PORTABLE
    portable = true;
    #else
    portable = false;
    #endif

#else
    /**
     * Linux and other
     */
    if(!mAssetsPathSet){

        fs::path executable_dir = getBasePath().parent_path();

        /**
         * The game has been installed in a fixed directory
         */

        // Installed in  /usr/games/pekka-kana-2

        if(PString::startsWith(executable_dir.string(), "/usr/games") || 
        PString::startsWith(executable_dir.string(), "/usr/bin")){
            portable = false;
            SetAssetsPath("/usr/share/games/pekka-kana-2");
        }

        // Installed in  /usr/local/bin/pekka-kana-2
        else if(PString::startsWith(executable_dir.string(), "/usr/local")){

            portable = false;
            SetAssetsPath("/usr/local/share/games/pekka-kana-2");
        }

        // Installed in  /opt/piste-gamez/pekka-kana-2
        else if(PString::startsWith(executable_dir.string(), "/opt/piste-gamez/pekka-kana-2")){
            portable = false;
            SetAssetsPath( (executable_dir.parent_path() / "res").string());
        }

        /**
         * The game is being run locally
         */
        else if(executable_dir.filename().string() == "bin"){
            SetAssetsPath( (executable_dir.parent_path() / "res").string());
        }
        else{
            SetAssetsPath(executable_dir.string());
        }
    }
#endif

    //Linux and Windows but not Android
    if(!mDataPathSet){

        if(portable){
            SetDataPath((mAssetsPath / "data").string());
        }
        else{
            SetPrefDataPath();
        }
    }

#endif
}


std::string GetAssetsPath(){
    return mAssetsPath.string();
}

std::string GetDataPath(){
    return mDataPath.string();
}

PFile::Path GetDataFileW(const std::string& filename){
    return (mDataPath / filename).string();
}

std::string GetEpisodeDirectory(){
    if(!mEpisodePath.empty()){
        return mEpisodePath.string();
    }

    return mAssetsPath.string();
}


void SetEpisode(const std::string& episodeName, PZip::PZip* zip_file){
    //mEpisodeName = episodeName;
    mEpisodeZip = zip_file;
    if(zip_file==nullptr){
        mEpisodePath = episodeName;
        if(!mEpisodePath.is_absolute()){
            mEpisodePath = mAssetsPath / "episodes" / mEpisodePath;
        }
    }
    else{
        mEpisodePath = fs::path("episodes") / episodeName;
    }
}

/**
 * @brief 
 * Finding files, cAsE insensitive
 */
static std::optional<std::string> FindFile(const fs::path& dir, const std::string& cAsE,  const std::string& alt_extension){
    if(!fs::exists(dir) || !fs::is_directory(dir))return {};
    std::string name_lowercase = PString::rtrim(PString::lowercase(cAsE));

    std::string name_lowercase_alt = "";
    if(!alt_extension.empty()){
        name_lowercase_alt = fs::path(name_lowercase).replace_extension(alt_extension).string();
    }

    std::optional<std::string> alt_res = {};


    for (const auto & entry : fs::directory_iterator(dir)){
        if(!entry.is_directory()){
            fs::path filename = entry.path().filename();

            std::string s1 = PString::lowercase(filename.string());
            
            if(name_lowercase == s1){

                return (dir / filename).string();
            }
            else if(!alt_extension.empty() && name_lowercase_alt == s1){
                alt_res = (dir / filename).string();

            }
        }
    }

    return alt_res;
}






std::optional<PFile::Path> FindVanillaAsset(const std::string& name, const std::string& default_dir, const std::string& alt_extension){
    std::string filename = fs::path(name).filename().string();

#ifdef __ANDROID__
    /**
     * apk://assets/sprites/pig.spr2
     */

    JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
	jobject activity = (jobject)SDL_AndroidGetActivity();

    jclass clazz(env->GetObjectClass(activity));
    jmethodID method_id = env->GetMethodID(clazz, "findPK2Asset", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    if(method_id==0){
        throw std::runtime_error("JNI: Method \"findPK2Asset\" not found!");
    }

    jstring param1 = env->NewStringUTF(name.c_str());
    jstring param2 = env->NewStringUTF(default_dir.c_str());
    jstring param3 = alt_extension.empty() ? nullptr : env->NewStringUTF(alt_extension.c_str());

    jstring res = (jstring)env->CallObjectMethod(activity, method_id, param1, param2, param3);
    env->DeleteLocalRef(param1);
    env->DeleteLocalRef(param2);  

    if(param3!=nullptr){
        env->DeleteLocalRef(param3);
    }

    if(res!=nullptr){
        const char* utf = env->GetStringUTFChars(res, nullptr);

        PFile::Path p((fs::path(default_dir) / utf).string());

        env->ReleaseStringUTFChars(res, utf);
        env->DeleteLocalRef(res);

        p.insideAndroidAPK = true;

        return p;
    }

#else
    /**
     * @brief 
     * sprites/pig.spr2
     */
    std::optional<std::string> op = FindFile(mAssetsPath / default_dir, filename, alt_extension);
    if(op.has_value()){
        return PFile::Path(*op);
    }

#endif

    return {};
}



std::optional<PFile::Path> FindEpisodeAsset(const std::string& name, const std::string& default_dir, const std::string& alt_extension){
    std::string filename = fs::path(name).filename().string();
    if(filename.empty()) return {};

    if(mEpisodeZip!=nullptr){

        std::optional<PZip::PZipEntry> entry;
        /**
         * @brief 
         * zip:/episodes/"episode"/pig.spr2
         */

        entry = mEpisodeZip->getEntry( (mEpisodePath / filename).string(), alt_extension);
        if(entry.has_value())return PFile::Path(mEpisodeZip, *entry);


        /**
         * @brief 
         * zip:/episodes/"episode"/sprites/pig.spr2
         */
        if(!default_dir.empty()){
            entry = mEpisodeZip->getEntry((mEpisodePath/default_dir/filename).string(), alt_extension);
            if(entry.has_value())return PFile::Path(mEpisodeZip, *entry);
        }
        
        /**
         * @brief 
         * zip:/sprites/pig.spr2
         */
        if(!default_dir.empty()){
            entry = mEpisodeZip->getEntry((fs::path(default_dir)/filename).string(), alt_extension);
            if(entry.has_value())return PFile::Path(mEpisodeZip, *entry);
        }        
    }
    
    else if(!mEpisodePath.empty()){

        #ifndef __ANDROID__

        /**
         * @brief 
         * episodes/"episode"/pig.spr2
         */
        std::optional<std::string> op = FindFile(mEpisodePath, filename, alt_extension);
        if(op.has_value()){
            return PFile::Path(*op);
        }

        /**
         * @brief 
         * episodes/"episode"/sprites/pig.spr2
         */
        if(!default_dir.empty()){
            op = FindFile(mEpisodePath / default_dir, filename, alt_extension);
            if(op.has_value()){
                return PFile::Path(*op);
            }
        }

        #else
        /**
         * @brief 
         * apk://episodes/"episode"/pig.spr2
         */
        std::optional<PFile::Path> op = FindVanillaAsset(
            filename, mEpisodePath.string(), alt_extension );
        if(op.has_value()){
            return op;
        }
        
        #endif
    }

    return {};
}

std::optional<PFile::Path> FindAsset(const std::string& name, const std::string& default_dir, const std::string& alt_extension){
    if(name.empty())return {};

    /**
     * 1. /full_path/pig.spr2
     */    
    fs::path p(name);
    if(p.is_absolute() && fs::exists(p) && !fs::is_directory(p))return PFile::Path(name);

    std::optional<PFile::Path> op = FindEpisodeAsset(name, default_dir, alt_extension);
    if(op.has_value())return op;

    op = FindVanillaAsset(name, default_dir, alt_extension);
    
    if(!op.has_value()){
        PLog::Write(PLog::WARN, "PFilesystem", "File \"%s\" not found!", name.c_str());
    }

    return op;
}

std::vector<std::string> ScanDirectory_s(const std::string& name, const std::string& filter){
    fs::path dir(name);
    if(!dir.is_absolute()){
        dir = mAssetsPath / dir;
    }
    std::vector<std::string> result;
    if(!fs::exists(dir) || !fs::is_directory(dir)){
        PLog::Write(PLog::WARN, "PFile", "Directory \"%s\" not found, cannot scan it", name.c_str());
        return result;
    }


    for (const auto & entry : fs::directory_iterator(dir)){

        if(filter.empty()){
            result.push_back(entry.path().filename().string());
        }
        else if(filter=="/"){
            if(entry.is_directory()){
                result.push_back(entry.path().filename().string());
            }
        }
        else{
            auto filename = entry.path().filename();
            std::string extension = PString::lowercase(filename.extension().string());
            if(extension==filter){
                result.push_back(filename.string());
            }
        }
    }

    std::sort(result.begin(), result.end());
    
    return result;
}


std::vector<std::string> ScanOriginalAssetsDirectory(const std::string& name, const std::string& filter){

#ifndef __ANDROID__
    return ScanDirectory_s(name, filter);
#else
    JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
	jobject activity = (jobject)SDL_AndroidGetActivity();

    jclass clazz(env->GetObjectClass(activity));
    jmethodID method_id = env->GetMethodID(clazz, "listPK2Assets", "(Ljava/lang/String;)[Ljava/lang/String;");
    if(method_id==0){
        throw std::runtime_error("JNI: Method listPK2Assets not found!");
    }

    jstring param = env->NewStringUTF(name.c_str());
    jobjectArray strings_array = (jobjectArray)env->CallObjectMethod(activity, method_id, param);
    env->DeleteLocalRef(param);
    

    if(strings_array==nullptr){
        throw std::runtime_error(std::string("Cannot scan the APK directory: ")+name);
    }


    int len = env->GetArrayLength(strings_array);
    std::vector<std::string> result;

    for (int i = 0; i < len; i++) {

		jstring js = (jstring)env->GetObjectArrayElement(strings_array, i);
		const char* utf = env->GetStringUTFChars(js, nullptr);

        std::string s = utf;

        env->ReleaseStringUTFChars(js, utf);
        env->DeleteLocalRef(js);


        if(filter.empty()){
            result.emplace_back(s);
        }
        else if(filter=="/"){
            if(s.find(".")==std::string::npos){
                result.emplace_back(s);
            }
        }
        else{
            std::string extension = PString::lowercase(fs::path(s).extension().string());
            if(extension==filter){
                result.emplace_back(s);
            }
        }
	}

    env->DeleteLocalRef(strings_array);
	
	return result;
#endif
}


std::string GetScreenshotName(){

    fs::path sp = mDataPath /"screenshots";
    int i = 0;
    fs::path res;
    do{
        res = sp / (std::string("pk2_screenshot_")+std::to_string(i)+".png");
        ++i;
    }while (fs::exists(res));

    return res.string();
}

}