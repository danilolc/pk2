#include "engine/PFile.hpp"

namespace PFile {

Path::Path(std::string path) {

    this->path = path;
    this->is_zip = false;

}

Path::Path(std::string zip, std::string path) {

    this->path = path;
    this->zipfile = zip;
    this->is_zip = true;

}

Path::~Path() {

}

};