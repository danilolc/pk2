//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include <vector>
#include <string>

namespace PFile {

struct Zip;
typedef void* RW;

int ReadRW(RW* rw, void* buffer, int len);
int WriteRW(RW* rw, const void* buffer, int len);
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