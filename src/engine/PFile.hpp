#pragma once

#include <string>

namespace PFile {

class Path {

    public: 

    std::string path; // Path inside zip if is_zip
    bool is_zip;
    std::string zipfile;

    Path(std::string path);
    Path(std::string zip, std::string path);
    ~Path();

};




}