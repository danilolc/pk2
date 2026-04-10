//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
/**
 * @brief
 * String utils by SaturninTheAlien
 */

#pragma once
#include <string>
#include "types.hpp"

namespace PString{
/**
 * @brief 
 * Convert a string to lowercase
 */
std::string lowercase(const std::string& src);
/**
 * @brief 
 * Remove white characters at the end of a string
 */
std::string rtrim(const std::string & src);

/**
 * @brief 
 * Replace '\\' with '/'
 */
std::string unwindowsPath(const std::string& path);


bool endsWith(const std::string& str, const std::string& suffix);
bool startsWith(const std::string& str, const std::string& prefix);

std::string removeSuffix(const std::string& str, const std::string& suffix);


class UTF8_Char{
private:
    char data[5] = {'\0'};
public:
    UTF8_Char()=default;
    UTF8_Char(char c){
        this->data[0] = c;
    }

    const char* c_str()const{
        return data;
    }

    bool operator==(const UTF8_Char& other)const{
        return *reinterpret_cast<const u32*>(this->data) == *reinterpret_cast<const u32*>(other.data);
    }

    bool operator!=(const UTF8_Char& other)const{
        return *reinterpret_cast<const u32*>(this->data) != *reinterpret_cast<const u32*>(other.data);
    }    
    const char* read(const char*str);

    bool isNull()const{
        return this->data[0]=='\0';
    }

    friend UTF8_Char lowercase(UTF8_Char u8c);
    friend UTF8_Char uppercase(UTF8_Char src);
};

UTF8_Char lowercase(UTF8_Char u8c);
UTF8_Char uppercase(UTF8_Char src);

}