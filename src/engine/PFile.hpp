#pragma once

#include <vector>
#include <string>

#include <cstdio>
#include <zip.h>

namespace PFile {

struct Zip;

class Path {

    public: 

    Path(std::string path);
    Path(Zip* zip_file, std::string path);

    //type:
    // ""  - all files and directories
    // "/" - directory
    // ".exe" - *.exe
    std::vector<std::string> scandir(const char* type);

    ~Path();

    private:

    std::string path; // Path inside zip if is_zip
    
    bool is_zip;
    Zip* zip_file;

};

Zip* OpenZip(std::string path);
void CloseZip(Zip* zp);

}