//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/types.hpp"
#include "engine/PJson.hpp"

#include <vector>
#include <string>

class episode_entry{
public:
    std::string name;
    std::string zipfile;
    std::string path;
    bool is_zip = false;
};

void to_json(nlohmann::json& j,const episode_entry& entry);
void from_json(const nlohmann::json& j, episode_entry& entry);

extern std::vector<episode_entry> episodes;

void Search_Episodes();

#ifdef __ANDROID__
void Android_InstallZipEpisode();

#endif