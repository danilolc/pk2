//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include <string>
#include <vector>
#include <optional>

namespace PZip{


class PZipException:public std::exception{
public:
    PZipException(const std::string& message):message(message){}
    const char* what() const noexcept{
        return message.c_str();
    }
private:
    std::string message;
};

class PZipEntry{
public:
    PZipEntry()=default;

    bool operator==(const PZipEntry& second)const{
        return this->index == second.index;
    }

    PZipEntry(int index, int size):
    index(index), size(size){

    }

    PZipEntry(const std::string& name, int index, int size):
    name(name), index(index), size(size){

    }

    bool good()const{
        return index >= 0 && size > 0;
    }

    std::string name;
    int index=-1;
    int size=0;
};


class PZip{
public:
    PZip() = default;

    PZip(const std::string& path){
        this->open(path);
    }


    PZip(const PZip& pzip)=delete;
    PZip(PZip&& pzip);

    PZip& operator=(const PZip& pzip)=delete;

    ~PZip(){
        this->close();
    }

    const std::string& getName()const{
        return this->name;
    }
    
    void open(const std::string& path);
    void close();

    void read(const PZipEntry& entry, void* buffer);

    std::optional<PZipEntry> getEntry(const std::string& cAsE_path, const std::string& alt_extension="");
    std::vector<std::string> findSubdirectories(const std::string& dirname_cAsE);
    
    std::vector<PZipEntry> scanDirectory(const std::string& filename_cAsE, const std::string& filter=""); 

private:
    std::string name;
    void * zip = nullptr;
    void * src = nullptr;
};

}