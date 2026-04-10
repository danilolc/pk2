//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
/**
 * @brief
 * String utils by SaturninTheAlien
 */
#include "PString.hpp"
#include <algorithm>
#include <iomanip>
#include <bitset>
#include <sstream>

namespace PString{

std::string lowercase(const std::string& src){

    std::string str = src;

    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    return str;
}

std::string rtrim(const std::string & src){
    std::size_t i = src.size();
    while(i>0){
        --i;
        if(!std::isspace(src[i]))break;
    }

    ++i;
    return src.substr(0, i);
}

std::string unwindowsPath(const std::string& path){
    std::string res = path;
    std::size_t n = path.size();
    for(std::size_t i=0;i<n;++i){
        if(res[i]=='\\'){
            res[i]='/';
        }
    }
    return res;
}



bool startsWith(const std::string& str, const std::string& prefix){

    std::size_t n = prefix.size();
    if(str.size() < n)return false;

    return str.substr(0, n)==prefix;
}


bool endsWith(const std::string& str, const std::string& suffix){
    std::size_t n = suffix.size();
    if(str.size() < n) return false;
    return str.substr(str.size() - n, n)==suffix;
}


std::string removeSuffix(const std::string& str, const std::string& suffix){
    if(endsWith(str, suffix)){
        return str.substr(0, str.size() - suffix.size());
    }
    else{
        return str;
    }
}


static int getBytesNumber(char c){

    //ASCII ch
    if((c & 0x80)==0){
        return 1;
    }
    // 11110000

    //3 following bytes
    else if((c & 0xF0)==0xF0){
        return 4;
    }

    //2 following bytes
    else if((c & 0xE0)==0xE0){
        return 3;
    }

    //1 following bytes
    else if((c & 0xC0)==0xC0){
        return 2;
    }


    std::ostringstream os;
    os<<"Not allowed UTF-8 leading byte: "<<std::bitset<8>(c)<<std::endl;
    throw std::runtime_error(os.str());
}

const char* UTF8_Char::read(const char *str){
	for(int i=0;i<4;++i){
		this->data[i] = '\0';
	}

	int bytes = getBytesNumber(*str);
    char* data_ptr = this->data;
    if(bytes==1){
        *data_ptr = *str;
        ++str;
    }
    else{
        for(int i=0;i<bytes;++i){
            //if(*str=='\0')return str;
            // Not a correct following byte, it also works when *str=='\0'
            if( ((*str) & 0x80) == 0)return str;

            *data_ptr = *str;

            ++data_ptr;
            ++str;
        }
    }

	return str;
}

UTF8_Char lowercase(UTF8_Char src){
	if(getBytesNumber(*src.data)==1){
		*src.data = std::tolower(*src.data);
	}
    //TODO lowercase other charcters
	return src;
}

UTF8_Char uppercase(UTF8_Char src){
	if(getBytesNumber(*src.data)==1){
		*src.data = std::toupper(*src.data);
	}
    //TODO lowercase other charcters
	return src;
}

}