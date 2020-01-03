#include "engine/PFile.hpp"

namespace PFile {

Path::Path(std::string path) {

    this->path = path;
    this->is_zip = false;

}

/*Path::Path(zip_t* zip, std::string path) {

    this->path = path;

    this->zip = zip;
    this->is_zip = true;

}*/

Path::~Path() {

}

};