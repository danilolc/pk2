#pragma once

#include <string>

//#include <zip.h>

namespace PFile {

class Path {

    public: 

    std::string path; // Path inside zip if is_zip
    
    bool is_zip;
    //zip_t* zip;

    Path(std::string path);
    //Path(zip_t* zip, std::string path);

    ~Path();

};




}