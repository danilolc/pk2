#pragma once

#include <vector>
#include <string>

#include <cstdio>
#include <zip.h>

#include <SDL_rwops.h>

namespace PFile {

struct Zip;

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

    SDL_RWops* GetRW(const char* mode);

    private:
    
    bool is_zip;
    Zip* zip_file;

};

Zip* OpenZip(std::string path);
void CloseZip(Zip* zp);

}