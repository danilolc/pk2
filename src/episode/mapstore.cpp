//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "mapstore.hpp"

#include "system.hpp"

#include "engine/PUtils.hpp"
#include "engine/PFile.hpp"
#include "engine/PLog.hpp"
#include "engine/PZip.hpp"
#include "engine/PFilesystem.hpp"
#include "engine/PString.hpp"

#include <cstring>
#include <algorithm>
#include <filesystem>
#include <unordered_set>

#ifdef __ANDROID__
#include <jni.h>
#include <optional>
#include <SDL.h>
#endif


namespace fs = std::filesystem;


void to_json(nlohmann::json& j,const episode_entry& entry){
	j["n"] = entry.name;
	if(entry.is_zip){
		j["z"] = entry.zipfile;
	}	
}
void from_json(const nlohmann::json& j, episode_entry& entry){
	j["n"].get_to(entry.name);
	if(j.contains("z")){
		entry.is_zip = true;
		j["z"].get_to(entry.zipfile);
	}
	else{
		entry.is_zip = false;
	}
}


std::vector<episode_entry> episodes;

static void sortEpisodes(){
	if (episodes.size() > 1){
		std::stable_sort(episodes.begin(), episodes.end(),
		[](const episode_entry& a, const episode_entry& b) {

			std::string name1 = PString::lowercase(a.name);
			std::string name2 = PString::lowercase(b.name);

			/**
			 * @brief 
			 * Position "Rooster islands" at the top of the list"
			 */
			if(name1.compare(0, 14, "rooster island")==0){

				if(name2.compare(0, 14, "rooster island")==0){
					return name1 < name2;
				}
				else{
					return true;
				}
			}
			else if(name2.compare(0, 14, "rooster island")==0){
				return false;
			}

			return name1 < name2;
		});
	}
}



void Search_Episodes() {
	episodes.clear();
	
	std::unordered_set<std::string> seen_episodes;

	/**
	 * Searching for user loose episodes (to improve the integration with the level editor)
	 */
	{
		std::filesystem::path p = std::filesystem::path(PFilesystem::GetDataPath()) / "episodes";
		std::vector<std::string> list_user = PFilesystem::ScanDirectory_s(p.string(), "/");
		for(std::string ep: list_user){
			seen_episodes.emplace(ep);

			episode_entry e;
			e.name = ep;
			e.path = (p / ep).string();
			e.is_zip = false;
			episodes.push_back(e);
		}
	}

	/**
	 * Searching for vanilla episodes. The directory may be read-only
	 */
	{
		std::vector<std::string> list_assets = PFilesystem::ScanOriginalAssetsDirectory(PFilesystem::EPISODES_DIR, "/");

		for (std::string ep : list_assets) {
			if(seen_episodes.find(ep)!=seen_episodes.end()){
				continue;
			}

			episode_entry e;
			e.name = ep;
			e.is_zip = false;
			episodes.push_back(e);
		}
	}


	std::string mapstore_path=(fs::path(PFilesystem::GetDataPath())/"mapstore").string();
	std::vector<std::string> list_zip = PFilesystem::ScanDirectory_s(mapstore_path, ".zip");
	for (std::string zip : list_zip) {
		try{

			PZip::PZip zp((fs::path(mapstore_path)/zip).string());
			std::vector<std::string> zip_list = zp.findSubdirectories("episodes");

			for (std::string ep : zip_list) {
				episode_entry e;
				e.name = ep;
				e.is_zip = true;
				e.zipfile = zip;
				episodes.push_back(e);
			}
		}
		catch(const std::exception& e){
			PLog::Write(PLog::ERR, "PK2", e.what());
		}
	}
	PLog::Write(PLog::DEBUG, "PK2", "Found %i episodes", (int)episodes.size());

	sortEpisodes();
}

#ifdef __ANDROID__


static std::optional<std::string> installZip(const std::string& targetPath){

    JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
	jobject activity = (jobject)SDL_AndroidGetActivity();

    jclass clazz(env->GetObjectClass(activity));
    jmethodID method_id = env->GetMethodID(clazz, "installZip", "(Ljava/lang/String;)Ljava/lang/String;");
    if(method_id==0){
        throw std::runtime_error("JNI: Method \"installZip\" not found!");
    }

    jstring param = targetPath.empty()? nullptr : env->NewStringUTF(targetPath.c_str());
    jstring res = (jstring)env->CallObjectMethod(activity, method_id, param);
    if(param!=nullptr){
        env->DeleteLocalRef(param);
    }

    std::optional<std::string> result;
    if(res!=nullptr){
        const char* utf = env->GetStringUTFChars(res, nullptr);
        result = std::string(utf);
        env->ReleaseStringUTFChars(res, utf);
        env->DeleteLocalRef(res);
    }


    return result;
}

void Android_InstallZipEpisode(){
	std::optional<std::string> res = installZip(
		(fs::path(PFilesystem::GetDataPath())/"mapstore").string());

	if(res.has_value()){
		PLog::Write(PLog::INFO, "PK2", "Installed a new zip episode: %s", res->c_str());

		try{
			PZip::PZip zp(*res);
			std::vector<std::string> zip_list = zp.findSubdirectories("episodes");

			for (std::string ep : zip_list) {
				episode_entry e;
				e.name = ep;
				e.is_zip = true;
				e.zipfile = fs::path(*res).filename().string();
				episodes.push_back(e);
			}


			sortEpisodes();
		}
		catch(const std::exception& e){
			PLog::Write(PLog::ERR, "PK2", e.what());
		}

	}
	else{
		PLog::Write(PLog::WARN, "PK2", "Episode not installed!");
	}
}

#endif


