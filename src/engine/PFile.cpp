//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include <sstream>

#include <algorithm>
#include <filesystem>
#include <cstring>
#include <fstream>
#include <SDL.h>
#include <stdexcept>

#ifdef __ANDROID__
#include <jni.h>
#include <functional>
#endif

#include "PFile.hpp"

#include "PLog.hpp"
#include "types.hpp"
#include "PString.hpp"



namespace fs = std::filesystem;

namespace PFile {

#ifdef __ANDROID__

static void getAndroidAsset(const std::string& name, const std::function<void(jbyte*, int)>& func){
    JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
	jobject activity = (jobject)SDL_AndroidGetActivity();

    jclass clazz(env->GetObjectClass(activity));
    jmethodID method_id = env->GetMethodID(clazz, "getPK2Asset", "(Ljava/lang/String;)[B");
    /*if(method_id==nullptr){
        throw std::runtime_error("JNI: Method \"getPK2Asset\" not found!");
    }*/

    jstring param = env->NewStringUTF(name.c_str());
    jbyteArray byteArray = (jbyteArray)env->CallObjectMethod(activity, method_id, param);
    
    /**
     * No longer needed
     */
    env->DeleteLocalRef(param);
    //env->DeleteLocalRef(clazz);
    //env->DeleteLocalRef(activity);
    

    if(byteArray==nullptr){
        std::ostringstream os;
        os<<"APK asset \""<<name<<"\" not found!";
        throw std::runtime_error(os.str());
    }

    int len = env->GetArrayLength(byteArray);
    jbyte* data = env->GetByteArrayElements(byteArray, NULL);
    if(data==nullptr){
        env->DeleteLocalRef(byteArray);
        throw std::runtime_error("JNI: env->GetByteArrayElements failed!");
    }

    func(data, len);
    env->ReleaseByteArrayElements(byteArray, data, JNI_ABORT);
    env->DeleteLocalRef(byteArray);
}

#endif


Path::Path(std::string path) {

	path = path.substr(0, path.find_last_not_of(" ") + 1);
    this->path = path;
	
	this->zip_file = nullptr;
}

Path::Path(PZip::PZip* zip_file, const PZip::PZipEntry&e):
zip_file(zip_file), zip_entry(e) {
	this->path = zip_entry.name;
}

Path::Path(Path path, std::string file) {

	*this = path;
	
	file = file.substr(0, file.find_last_not_of(" ") + 1);
	this->path += file;
}

Path::~Path() {

}

bool Path::operator==(const Path& second)const {
	if(this->zip_file!=nullptr || second.zip_file!=nullptr){
		return this->zip_file == second.zip_file && this->zip_entry == second.zip_entry;
	}
	else{
		return this->path == second.path;
	}
}

bool Path::exists()const{
	if(this->zip_file!=nullptr){
		throw std::runtime_error("Unimplemented Path::exists (zip)");
	}
	else{
		return fs::exists(this->path);
	}
}

RW Path::GetRW2(std::string mode)const {


	SDL_RWops* ret = nullptr;

#ifdef __ANDROID__
	if(this->insideAndroidAPK){
		void * buffer = nullptr;
		int size = 0;

		getAndroidAsset(this->path, [&](jbyte* data_j, int size_j){
			buffer = SDL_malloc(size_j);
			memcpy(buffer, data_j, size_j);
			size = size_j;
		});

		ret = SDL_RWFromConstMem(buffer, size);
		return RW(ret, buffer);
	}
#endif

	if (this->zip_file != nullptr && this->zip_entry.good()) {
		void * buffer = SDL_malloc(this->zip_entry.size);
		this->zip_file->read(this->zip_entry, buffer);
		ret = SDL_RWFromConstMem(buffer, this->zip_entry.size);
		return RW(ret, buffer);
	}
	else{
		if(!PString::endsWith(mode, "b")){
			mode+="b";
		}

		ret = SDL_RWFromFile(this->path.c_str(), mode.c_str());
		if (!ret) {

			std::ostringstream os;
			os<<"Can't get RW from the file: \""<<this->path<<"\"";
			std::string s = os.str();
			throw PFileException(s);
		}

		return RW(ret, nullptr);
	}
}


nlohmann::json Path::GetJSON()const{

#ifdef __ANDROID__
	if(this->insideAndroidAPK){
		char * buffer = nullptr;

		getAndroidAsset(this->path, [&](jbyte* data_j, int size){
			buffer = new char[size + 1];
			memcpy(buffer, data_j, size);
			buffer[size] = '\0';
		});

		nlohmann::json res = nlohmann::json::parse(buffer);
		delete[] buffer;
		return res;
	}
#endif
	
	if(this->zip_file!=nullptr && this->zip_entry.good()){

		char * buffer = new char[this->zip_entry.size + 1];
		buffer[this->zip_entry.size] = '\0';

		this->zip_file->read(this->zip_entry, buffer);

		nlohmann::json res = nlohmann::json::parse(buffer);
		delete[] buffer;
		return res;

	}else{
		std::ifstream in(this->path.c_str());
		nlohmann::json res = nlohmann::json::parse(in);
		return res;
	}
}

std::string Path::GetContentAsString()const{

#ifdef __ANDROID__
	if(this->insideAndroidAPK){
		char * buffer = nullptr;

		getAndroidAsset(this->path, [&](jbyte* data_j, int size){
			buffer = new char[size + 1];
			memcpy(buffer, data_j, size);
			buffer[size] = '\0';
		});

		std::string res = buffer;
		delete[] buffer;
		return res;
	}
#endif


	if(this->zip_file!=nullptr){
		char * buffer = new char[this->zip_entry.size + 1];
		buffer[this->zip_entry.size] = '\0';

		this->zip_file->read(this->zip_entry, buffer);

		std::string res = buffer;
		delete[] buffer;
		return res;
	}
	else{
		std::ifstream in(this->path.c_str());
		return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	}
}

RW::RW(RW&& source){
	this->_rwops = source._rwops;
	this->_mem_buffer = source._mem_buffer;

	source._rwops = nullptr;
	source._mem_buffer = nullptr;
}

int RW::read(void* val, size_t size) {

	return SDL_RWread((SDL_RWops*)(this->_rwops), val, 1, size);

}
void RW::read(bool& val) {

	u8 v = SDL_ReadU8((SDL_RWops*)(this->_rwops));
	
	if (v == 0) val = false;
	else val = true;
}
void RW::read(u8& val) {

	val = SDL_ReadU8((SDL_RWops*)(this->_rwops));

}
void RW::read(s8& val) {

	val = SDL_ReadU8((SDL_RWops*)(this->_rwops));
}
void RW::read(u16& val) {

	val = SDL_ReadLE16((SDL_RWops*)(this->_rwops));
}
void RW::read(s16& val) {

	val = SDL_ReadLE16((SDL_RWops*)(this->_rwops));
}
void RW::read(u32& val) {

	val = SDL_ReadLE32((SDL_RWops*)(this->_rwops));

}
void RW::read(s32& val) {

	val = SDL_ReadLE32((SDL_RWops*)(this->_rwops));
}
void RW::read(u64& val) {
	val = SDL_ReadLE64((SDL_RWops*)(this->_rwops));
}
void RW::read(s64& val) {
	val = SDL_ReadLE64((SDL_RWops*)(this->_rwops));
}

void RW::readLegacyStrInt(int&val){
	char buffer[8];
	this->read(buffer, sizeof(buffer));
	buffer[7] = '\0';

	val = atoi(buffer);
}

void RW::readLegacyStrU32(u32& val){
	char buffer[8];
	this->read(buffer, sizeof(buffer));
	buffer[7] = '\0';

	val = (u32)atol(buffer);
}

void RW::readLegacyStr13Chars(std::string & val){
	char buffer[13];
	this->read(buffer, sizeof(buffer));
	buffer[12] = '\0';
	val = buffer;
}

void RW::readLegacyStr40Chars(std::string & val){
	char buffer[40];
	this->read(buffer, sizeof(buffer));
	buffer[39] = '\0';
	val = buffer;
}

/*
int RW::write(std::string& str) {

	return SDL_RWwrite((SDL_RWops*)(this->_rwops), str.c_str(), 1, str.size() + 1);

}*/

int RW::write(const void* val, size_t size) {

	return SDL_RWwrite((SDL_RWops*)(this->_rwops), val, size, 1);

}
void RW::write(bool val) {
	SDL_WriteU8((SDL_RWops*)(this->_rwops), val);

}
void RW::write(u8 val) {
	SDL_WriteU8((SDL_RWops*)(this->_rwops), val);

}
void RW::write(s8 val) {
	SDL_WriteU8((SDL_RWops*)(this->_rwops), val);

}
void RW::write(u16 val) {
	SDL_WriteLE16((SDL_RWops*)(this->_rwops), val);

}
void RW::write(s16 val) {
	SDL_WriteLE16((SDL_RWops*)(this->_rwops), val);

}
void RW::write(u32 val) {
	SDL_WriteLE32((SDL_RWops*)(this->_rwops), val);

}
void RW::write(s32 val) {
	SDL_WriteLE32((SDL_RWops*)(this->_rwops), val);

}
void RW::write(u64 val) {
	SDL_WriteLE64((SDL_RWops*)(this->_rwops), val);

}
void RW::write(s64 val) {
	SDL_WriteLE64((SDL_RWops*)(this->_rwops), val);

}

void RW::writeCBOR(const nlohmann::json& j){
	std::vector<std::uint8_t> v_cbor = nlohmann::json::to_cbor(j);
	u32 size = (u32)v_cbor.size();
	this->write(size);
	this->write(v_cbor.data(), size);
}

nlohmann::json RW::readCBOR(){
	u32 size;
	this->read(size);

	std::vector<std::uint8_t> v_cbor;
	v_cbor.resize(size);

	this->read(v_cbor.data(), size);

	return nlohmann::json::from_cbor(v_cbor);
}

size_t RW::size() {

	SDL_RWops* rwops = (SDL_RWops*)(this->_rwops);

	return SDL_RWsize(rwops);

}

void RW::close() {

	if(this->_rwops!=nullptr){
		SDL_RWops* rwops = (SDL_RWops*)(this->_rwops);
	
		int ret = SDL_RWclose(rwops);
		if (ret != 0) {
		
			PLog::Write(PLog::ERR, "PFile", "Error freeing rw");
		}

		this->_rwops = nullptr;
	}

	if(this->_mem_buffer!=nullptr){
		SDL_free(this->_mem_buffer);
		this->_mem_buffer = nullptr;
	}

}


};