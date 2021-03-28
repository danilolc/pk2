//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "types.hpp"

#include <vector>
#include <string>

namespace PFile {

struct Zip;

struct RW {

    size_t size();
    size_t to_buffer(void** buffer);

    int read(void* val, size_t size);
    int read(bool& val);

    int read(std::string& str);

    // Read the value always in little endian
    int read(u8& val);
    int read(s8& val);
    int read(u16& val);
    int read(s16& val);
    int read(u32& val);
    int read(s32& val);
    int read(u64& val);
    int read(s64& val);

    int write(const void* val, size_t size);
    int write(bool val);

    int write(std::string& str);

    // Write the value always in little endian
    int write(u8 val);
    int write(s8 val);
    int write(u16 val);
    int write(s16 val);
    int write(u32 val);
    int write(s32 val);
    int write(u64 val);
    int write(s64 val);

    int close();

};

class Path {

    public: 

    Path(std::string path);
    Path(Zip* zip_file, std::string path);
    Path(Path path, std::string file);
    ~Path();

    bool operator ==(Path path);
    const char* c_str();

    //type:
    // ""  - all files and directories
    // "/" - directory
    // ".exe" - *.exe
    std::vector<std::string> scandir(const char* type);

    bool NoCaseFind();
    bool Find();

    bool Is_Zip();

    int SetFile(std::string file);
    int SetPath(std::string path);
    void FixSep();

    std::string GetDirectory();
    std::string GetFileName();

    RW* GetRW(const char* mode);

    private:
    
    std::string path;
    Zip* zip_file;

};

Zip* OpenZip(std::string path);
void CloseZip(Zip* zp);

}