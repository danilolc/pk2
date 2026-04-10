//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "PZip.hpp"
#include "PFile.hpp"
#include "PString.hpp"

#include <SDL.h>
#include <zip.h>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace PZip{

PZip::PZip(PZip&& pzip)
:name(std::move(pzip.name)), zip(pzip.zip), src(pzip.src){
    pzip.zip = nullptr;
    pzip.src = nullptr;
}

void PZip::open(const std::string& path){
	SDL_RWops* rw = SDL_RWFromFile(path.c_str(), "r");
	if (rw == NULL) {
        std::ostringstream os;
        os<<"Can't open \""<<path<<"\"";       
        throw PZipException(os.str());
    }

	int size = SDL_RWsize(rw);

	void* buffer = malloc(size);

	SDL_RWread(rw, buffer, size, 1);
	SDL_RWclose(rw);

	zip_error err;

    /**
     * @brief 
     * There's no need to free(buffer) because zip_source_buffer_create does it.
     */
	zip_source_t* src = zip_source_buffer_create(buffer, size, 1, &err);

    zip_t* zip = zip_open_from_source(src, ZIP_RDONLY, &err);

    this->name = std::filesystem::path(path).filename().string();


    this->src = src;
    this->zip = zip;
}

void PZip::close(){
    if(this->src!=nullptr){
        zip_source_close((zip_source_t*)this->src);
        this->src = nullptr;
    }

	if(this->zip!=nullptr){
        zip_discard((zip_t*)this->zip);
        this->zip = nullptr;
    }

    this->name = "";
}

std::optional<PZipEntry> PZip::getEntry(const std::string& cAsE_path, const std::string& alt_extension){

	std::string lower_path = PString::unwindowsPath(PString::lowercase(PString::rtrim(cAsE_path)));

	std::string lower_path_alt="";
	if(!alt_extension.empty()){
		lower_path_alt= PString::unwindowsPath(
			fs::path(lower_path).replace_extension(alt_extension).string()
		);
	}

	std::optional<PZipEntry> alt_res = {};

	struct zip_stat st;
	zip_stat_init(&st);

	int sz = zip_get_num_entries((zip_t*)this->zip , 0);
	for (int i = 0; i < sz; ++i) {
		zip_stat_index((zip_t*)this->zip, i, 0, &st);
		if(st.name==nullptr)continue;

		std::string st_name = st.name;

		std::string st_name_lower = PString::lowercase(st_name);

		if(lower_path==st_name_lower){
			return PZipEntry(
				fs::path(st_name).filename().string(),
				i,
				st.size
			);
		}
		else if(!lower_path_alt.empty() && lower_path_alt==st_name_lower){
			alt_res = PZipEntry(
				fs::path(st_name).filename().string(),
				i,
				st.size
			);
		}
	}

	return alt_res;
}

void PZip::read(const PZipEntry& entry, void* buffer){
	zip_file_t* zfile = zip_fopen_index((zip_t*)this->zip, entry.index, 0);

	if (!zfile) {

		std::ostringstream os;
		os<<"Zfile from zip \""<<this->name<<"\", file \""<<
		entry.name<<"\" is NULL";
		throw PZipException(os.str());
	}

	zip_fread(zfile, buffer, entry.size);
	zip_fclose(zfile);
}


std::vector<std::string> PZip::findSubdirectories(const std::string& dirname_cAsE){
	std::string dirname = PString::unwindowsPath(PString::rtrim(PString::lowercase(dirname_cAsE)));

	std::vector<std::string> result;
	
	struct zip_stat st;
	zip_stat_init(&st);

	int sz = zip_get_num_entries((zip_t*)this->zip , 0);
	
	for (int i = 0; i < sz; ++i) {
		zip_stat_index((zip_t*)this->zip, i, 0, &st);
		if(st.name==nullptr)continue;

		std::string st_name = st.name;

		if(PString::lowercase(st_name).compare(0, dirname.size(), dirname)!=0)continue;

		/**
		 * @brief 
		 * To skip the directory itself
		 */
		if(st_name.size() == dirname.size() + 1)continue;

		/**
		 * @brief 
		 * Remove directory prefix
		 */
		st_name = st_name.substr(dirname.size() + 1, st_name.size());

		auto pos = st_name.find("/");
		if(pos==std::string::npos)continue;

		st_name = st_name.substr(0, pos);
		bool duplicate = false;
		for(const std::string& s: result){
			if(s == st_name){
				duplicate = true;
				break;
			}
		}

		if(!duplicate){
			result.push_back(st_name);
		}		
	}
	return result;
}


std::vector<PZipEntry> PZip::scanDirectory(const std::string& filename_cAsE, const std::string& filter){

	std::string filename = PString::unwindowsPath(PString::rtrim(PString::lowercase(filename_cAsE)));

	std::vector<PZipEntry> result;

	struct zip_stat st;
	zip_stat_init(&st);

	int sz = zip_get_num_entries((zip_t*)this->zip , 0);
	for (int i = 0; i < sz; ++i) {
		zip_stat_index((zip_t*)this->zip, i, 0, &st);
		if(st.name==nullptr)continue;

		std::string st_name = st.name;
		
		if(!filename.empty() && PString::lowercase(st_name).compare(0, filename.size(), filename)!=0)continue;

		/**
		 * @brief 
		 * To skip the directory itself
		 */
		if(st_name.size() == filename.size() + 1)continue;

		st_name = st_name.substr(filename.size() + 1, st_name.size());

		if(filter=="/"){
			if(st_name.size() > 0 && st_name[st_name.size() - 1]=='/'){
				st_name = st_name.substr(0, st_name.size()-1);
			}
			else{
				continue;
			}
		}
		else if(!filter.empty()){

			std::string extension = PString::lowercase(fs::path(st_name).extension().string());
			if(extension!=filter){
				continue;
			}
		}

		/**
		 * @brief 
		 * To skip subdirectories
		 */
		if(st_name.find("/")!=std::string::npos)continue;

		result.push_back( PZipEntry(st_name, i, st.size));
	}

	return result;
}


//
}