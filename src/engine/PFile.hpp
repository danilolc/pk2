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

typedef void RW;

size_t RWToBuffer(RW* rw, void** buffer);

int ReadRW(RW* rw, void* val, size_t size);
int ReadRW(RW* rw, bool& val);
int ReadRW(RW* rw, u8& val);
int ReadRW(RW* rw, u16& val);
int ReadRW(RW* rw, u32& val);
int ReadRW(RW* rw, u64& val);

int WriteRW(RW* rw, const void* val, size_t size);
int WriteRW(RW* rw, bool val);
int WriteRW(RW* rw, u8 val);
int WriteRW(RW* rw, u16 val);
int WriteRW(RW* rw, u32 val);
int WriteRW(RW* rw, u64 val);

int CloseRW(RW* rw);

class Path : public std::string {

    public: 

    Path(std::string path);
    Path(Zip* zip_file, std::string path);
    Path(Path path, std::string file);
    ~Path();

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
    std::string GetFileName();

    RW* GetRW(const char* mode);

    private:
    
    bool is_zip;
    Zip* zip_file;

};

Zip* OpenZip(std::string path);
void CloseZip(Zip* zp);

}