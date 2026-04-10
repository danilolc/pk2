//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
/**
 * @brief 
 * New filesystem utils by SaturninTheAlien to replace obsolete ones from PFile.cpp
 */
#pragma once

#include <string>
#include <vector>
#include <optional>
#include "PFile.hpp"

namespace PFilesystem{

extern const std::string EPISODES_DIR;

extern const std::string GFX_DIR;
extern const std::string TILES_DIR;
extern const std::string SCENERY_DIR;

extern const std::string LANGUAGE_DIR;
extern const std::string FONTS_DIR;

extern const std::string SFX_DIR;
extern const std::string SPRITES_DIR;
extern const std::string MUSIC_DIR;

extern const std::string LUA_DIR;
extern const std::string LIFE_DIR;


void CreateDirectory(const std::string& path);

void SetAssetsPath(const std::string& name);
void SetDataPath(const std::string& name);
void SetPrefDataPath();

void SetDefaultPaths();

std::string GetAssetsPath();
std::string GetDataPath();

PFile::Path GetDataFileW(const std::string& filename);

std::string GetEpisodeDirectory();

std::string GetScreenshotName();

void SetEpisode(const std::string& episodeName, PZip::PZip* zip_file=nullptr);
//bool FindAsset_s(std::string& name, const std::string& default_dir, const std::string& alt_extension);

std::optional<PFile::Path> FindAsset(const std::string& name, const std::string& default_dir, const std::string& alt_extension="");
std::optional<PFile::Path> FindVanillaAsset(const std::string& name, const std::string& default_dir, const std::string& alt_extension="");
std::optional<PFile::Path> FindEpisodeAsset(const std::string& name, const std::string& default_dir, const std::string& alt_extension="");

std::vector<std::string> ScanDirectory_s(const std::string& name, const std::string& filter="");
std::vector<std::string> ScanOriginalAssetsDirectory(const std::string& name, const std::string& filter="");


}