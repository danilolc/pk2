//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "episode/episodeclass.hpp"

#include "episode/mapstore.hpp"
#include "game/levelclass.hpp"
#include "language.hpp"
#include "system.hpp"


#include "settings/config_txt.hpp"
#include "save.hpp"
#include "save_slots.hpp"

#include "engine/PLog.hpp"
#include "engine/PFile.hpp"
#include "engine/PFilesystem.hpp"

#include "engine/PDraw.hpp"

#include "lua/pk2_lua.hpp"

#include <algorithm>
#include <cstring>
#include <string>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;


void LevelEntry::loadLevelHeader(PFile::Path levelFile){
	LevelClass temp;

	temp.load(levelFile, true);

	this->levelName = temp.name;
	this->map_x = temp.icon_x;
	this->map_y = temp.icon_y;
	this->number = temp.level_number;
	this->icon_id = temp.icon_id;
}

std::string LevelEntry::getLevelFilename(bool proxy)const{
	if(proxy && this->proxies.has_value()){
		const std::vector<ProxyLevelEntry>& proxies = *this->proxies;
		
		u32 total_probability = 0;
		for(const ProxyLevelEntry&p : proxies){
			total_probability += p.weight;
		}

		int r = rand() % total_probability;
		for(const ProxyLevelEntry&p : proxies){
			r-= (int)p.weight;
			if(r<0){
				return p.filename;
			}
		}
	}

	return this->fileName;
}

EpisodeClass* Episode = nullptr;


EpisodeClass::~EpisodeClass(){
	PFilesystem::SetEpisode("", nullptr);
}

std::string EpisodeClass::getScoresPath()const{
	return (fs::path(PFilesystem::GetDataPath()) / "scores" / (this->entry.name + ".dat")).string();
}

void EpisodeClass::openScores() {
	try{
		this->scoresTable.load(this->getScoresPath());
		/**
		 * @brief 
		 * Fix missing level filenames, for example, in old score files
		 */
		int i = 0;
		for(const LevelEntry& entry:this->levels_list_v){
			LevelScore * score = this->scoresTable.getScoreByLevelNumber(i);
			if(score!=nullptr && score->levelFileName.empty()){
				score->levelFileName = entry.fileName;
			}
			++i;
		}
	}
	catch(const std::exception&e){
		PLog::Write(PLog::WARN, "PK2", e.what());
		PLog::Write(PLog::INFO, "PK2", "Can't load scores files");

	}
}

// Version 1.1
void EpisodeClass::saveScores() {
	PFile::Path path(this->getScoresPath());

	try{
		scoresTable.save(path);
	}
	catch(const std::exception& e){
		PLog::Write(PLog::ERR, "PK2", e.what());
		PLog::Write(PLog::ERR, "PK2", "Can't save scores");
	}
}

//TODO - Load info from different languages
void EpisodeClass::loadInfo() {

	std::optional<PFile::Path> infofile = PFilesystem::FindEpisodeAsset("infosign.txt", "");
	if(infofile.has_value()){
		if (this->infos.Read_File(*infofile)){
			PLog::Write(PLog::DEBUG, "PK2", "%s loaded", infofile->c_str());
		}
		else{
			PLog::Write(PLog::ERR, "PK2", "Cannot load %s", infofile->c_str());
		}		
	}
}

//TODO - don't load the same image again
void EpisodeClass::loadAssets() {

	std::optional<PFile::Path> path = PFilesystem::FindAsset("pk2stuff.png",
		PFilesystem::GFX_DIR, ".bmp");

	if(!path.has_value()){
		PLog::Write(PLog::ERR, "PK2", "Can't load pk2stuff"); //"Can't load map bg"
	}
	else{
		PDraw::image_load(global_gfx_texture, *path, true);
	}

	path = PFilesystem::FindAsset("pk2stuff2.png",
		PFilesystem::GFX_DIR, ".bmp");

	if(!path.has_value()){
		PLog::Write(PLog::ERR, "PK2", "Can't load pk2stuff2"); //"Can't load map bg"
	}
	else{
		PDraw::image_load(global_gfx_texture2, *path, true);
	}
}


void from_json(const nlohmann::json& j, ProxyLevelEntry& proxy){

	proxy.filename = j["file"].get<std::string>();
	PJson::jsonReadU32(j,"weight", proxy.weight);
}


void EpisodeClass::loadLevels(){
	std::string dir = PFilesystem::GetEpisodeDirectory();


	std::vector<PFile::Path> proxyLevelFiles;
	std::vector<std::string> proxyLevelNames;

	if(entry.is_zip){
		std::vector<PZip::PZipEntry> v = this->source_zip.scanDirectory(
			std::string("episodes/")+this->entry.name, ".proxy");
		for(const PZip::PZipEntry& en: v){
			proxyLevelFiles.emplace_back(PFile::Path(&this->source_zip, en));
			proxyLevelNames.emplace_back(en.name);
		}
	}
	else{

		proxyLevelNames = PFilesystem::ScanOriginalAssetsDirectory(dir, ".proxy");
		for(const std::string& name: proxyLevelNames){
			proxyLevelFiles.emplace_back(PFile::Path((fs::path(dir) / name).string()));
		}
	}

	std::vector<PFile::Path> realLevelFiles;
	std::vector<std::string> realLevelNames;


	if(entry.is_zip){
		std::vector<PZip::PZipEntry> v = this->source_zip.scanDirectory(
			std::string("episodes/")+this->entry.name, ".map");
		for(const PZip::PZipEntry& en: v){
			realLevelFiles.emplace_back(PFile::Path(&this->source_zip, en));
			realLevelNames.emplace_back(en.name);
		}
	}
	else{

		realLevelNames = PFilesystem::ScanOriginalAssetsDirectory(dir, ".map");
		for(const std::string& name: realLevelNames){
			realLevelFiles.emplace_back(PFile::Path((fs::path(dir) / name).string()));
		}
	}


	std::vector<std::string> hiddenLevels;

	std::size_t n = proxyLevelNames.size();
	for(std::size_t i=0;i<n;++i){
		try{
			LevelEntry levelEntry;
			levelEntry.fileName = proxyLevelNames[i];
			nlohmann::json j = proxyLevelFiles[i].GetJSON();

			PJson::jsonReadString(j, "name", levelEntry.levelName);
			PJson::jsonReadU32(j, "number", levelEntry.number);
			PJson::jsonReadInt(j, "map_x", levelEntry.map_x);
			PJson::jsonReadInt(j, "map_y", levelEntry.map_y);
			PJson::jsonReadInt(j, "icon_id", levelEntry.icon_id);

			std::vector<ProxyLevelEntry> proxies = j["levels"].get<std::vector<ProxyLevelEntry>>();

			for(const ProxyLevelEntry& p: proxies){

				/**
				 * @brief 
				 * Check if level from proxy exist
				 */

				if(std::find(realLevelNames.begin(),
				realLevelNames.end(), p.filename)==realLevelNames.end()){
					std::ostringstream os;
					os<<"Level \""<<p.filename<<"\" not found!";
					throw std::runtime_error(os.str());
				}

				/**
				 * @brief 
				 * Hide levels used by proxies
				 */
				if(std::find(hiddenLevels.begin(), hiddenLevels.end(), p.filename)== hiddenLevels.end()){
					hiddenLevels.push_back(p.filename);
				}
			}
			
			levelEntry.proxies = proxies;
			this->levels_list_v.push_back(levelEntry);
		}
		catch(const std::exception& e){
			PLog::Write(PLog::ERR, "%s", e.what());
		}
	}

	std::size_t levelsNumber = realLevelNames.size();

	// Read levels plain data
	for (u32 i = 0; i < levelsNumber; i++) {

		try{
			LevelEntry levelEntry;
			levelEntry.fileName = realLevelNames[i];

			/**
			 * @brief 
			 * Hidden levels shouldn't appear on the map screen
			 */
			if(std::find(hiddenLevels.begin(), hiddenLevels.end(), levelEntry.fileName)!=hiddenLevels.end()){
				continue;
			}

			levelEntry.loadLevelHeader(realLevelFiles[i]);
			if(levelEntry.number > this->highestLevelNumber){
				this->highestLevelNumber = levelEntry.number;
			}
			this->levels_list_v.emplace_back(levelEntry);
		}
		catch(const std::exception& e){
			PLog::Write(PLog::ERR, "PK2 level", e.what());
		}
		
	}

	// Sort levels
	std::stable_sort(this->levels_list_v.begin(), this->levels_list_v.end(),
	[](const LevelEntry& a, const LevelEntry& b) {
		return a.number < b.number;
	});

	// Set positions
	int i = 0;
	for(LevelEntry& entry: this->levels_list_v){
		if (entry.map_x == 0){
			entry.map_x = 172 + i*30;
		}
		if (entry.map_y == 0){
			entry.map_y = 270;
		}
		++i;
	}
}

void EpisodeClass::load() {
	
	if (entry.is_zip){
		this->source_zip.open( (fs::path(PFilesystem::GetDataPath())/"mapstore"/entry.zipfile).string());
		PFilesystem::SetEpisode(entry.name, &this->source_zip);
	}
	else{

		if(entry.path.empty()){
			PFilesystem::SetEpisode(entry.name, nullptr);
		} else {
			PFilesystem::SetEpisode(entry.path, nullptr);
		}		
	}

	if(!test_level){
		this->loadLevels();
		this->openScores();

		if(config_txt.save_slots){
			this->updateNextLevel();
		}
		else{
			PK2save::LoadModern(this);
		}		
	}

	std::optional<PFile::Path> config_path = PFilesystem::FindEpisodeAsset("config.txt", "");
	if(config_path.has_value()){

		PLang config(*config_path);
		if (config.loaded) {

			int id = config.Search_Id("glow_effect");
			if (id != -1) {
				PLog::Write(PLog::INFO, "PK2", "Episode glow is ON");
				this->glows = true;
			}

			id = config.Search_Id("hide_numbers");
			if (id != -1) {
				PLog::Write(PLog::INFO, "PK2", "Episode hide numbers is ON");
				this->hide_numbers = true;
			}

			id = config.Search_Id("ignore_collectable");
			if (id != -1) {
				PLog::Write(PLog::INFO, "PK2", "Episode ignore apples is ON");
				this->ignore_collectable = true;
			}

			id = config.Search_Id("collectable_name");
			if (id != -1) {
				this->collectable_name = config.Get_Text(id);
				PLog::Write(PLog::INFO, "PK2", "Collectable name:");
				PLog::Write(PLog::INFO, "PK2", this->collectable_name.c_str());
			}

			id = config.Search_Id("require_all_levels");
			if (id != -1) {
				PLog::Write(PLog::INFO, "PK2", "Episode require all levels is ON");
				this->require_all_levels = true;
			}

			id = config.Search_Id("no_ending");
			if (id != -1) {
				PLog::Write(PLog::INFO, "PK2", "Episode no ending is ON");
				this->no_ending = true;
			}

			this->transformation_offset = config.getBoolean("potion_transformation_offset", false);
			this->visible_wind = config.getBoolean("visible_wind", false);
			this->supermode_music = config.getBoolean("supermode_music", true);
			this->legacy_start_offset = config.getBoolean("legacy_start_offset", false);
			this->legacy_camera_offset = config.getBoolean("legacy_camera_offset", false);
			this->legacy_obstacle_phasing = config.getBoolean("legacy_obstacle_phasing", false);
			this->checkpointPenalty = config.getInteger("checkpoint_penalty", 150);
		}
		else{
			PLog::Write(PLog::ERR, "PK2", "Cannot open episode config file.");
		}
	}
		
	this->sfx.loadAllForEpisode(sfx_global, this);
	
	this->loadInfo();
}

/*
EpisodeClass::EpisodeClass(int save) {

	//Search the id
	int sz = episodes.size();
	bool set = false;
	for (int i = 0; i < sz; i++) {

		if (episodes[i].name.compare(PK2save::saves_slots[save].episode) == 0) {
			if (set)
				PLog::Write(PLog::WARN, "PK2", "Episode conflict on %s, choosing the first one", PK2save::saves_slots[save].episode);
			else {
				this->entry = episodes[i];
				set = true;
			}
		}
	}
	PK2save::saves_slots[save].name[19] = '\0';
	this->player_name = PK2save::saves_slots[save].name;
	this->next_level = PK2save::saves_slots[save].next_level;
	this->player_score = PK2save::saves_slots[save].score;

	

	this->load();

	int size = this->levels_list_v.size();
	if(size > EPISODI_MAX_LEVELS){
		size = EPISODI_MAX_LEVELS;
	}

	for (int j = 0; j < size; j++) {

		this->levels_list_v[j].status = PK2save::saves_slots[save].level_status[j];
	}

}*/

u8 EpisodeClass::getLevelStatus(int level_id)const{
	if(level_id >= 0 && level_id < (int)this->levels_list_v.size()){
		return this->levels_list_v[level_id].status;
	}
	else{
		return 0;
	}		
}

void EpisodeClass::updateLevelStatus(int level_id, u8 status){
	if(level_id >= 0 && level_id < (int)this->levels_list_v.size()){
		this->levels_list_v[level_id].status = status;
		this->updateNextLevel();
	}
}

int EpisodeClass::getLevelBestScore(int level_id)const{
	if(level_id >= 0 && level_id < (int)this->levels_list_v.size()){
		return this->levels_list_v[level_id].best_score;
	}
	else{
		return 0;
	}
}

void EpisodeClass::updateLevelBestScore(int level_id, int best_score){
	if(level_id >= 0 && level_id < (int)this->levels_list_v.size()){
		this->levels_list_v[level_id].best_score = best_score;

		//Count player score for an episode
		this->player_score = 0;
		for(const LevelEntry& entry: this->levels_list_v){
			this->player_score += entry.best_score;
		}		
	}
}

std::string EpisodeClass::getLevelFilename(int level_id,bool executeProxies)const{
	if(level_id >= 0 && level_id < (int)this->levels_list_v.size()){
		return this->levels_list_v[level_id].getLevelFilename(executeProxies);
	}
	throw std::runtime_error(std::string("Level with id=")+std::to_string(level_id)+" not found!");
}

int EpisodeClass::findLevelbyFilename(const std::string& levelFilename)const{

	for(std::size_t i=0; i < this->levels_list_v.size();++i){
		if(levelFilename == this->levels_list_v[i].fileName){
			return (int)i;
		}
	}
	
	return -1;
}

EpisodeClass::EpisodeClass(const std::string& player_name, episode_entry entry):
entry(entry), player_name(player_name){	
	this->load();
}

void EpisodeClass::updateNextLevel() {

	u32 passedLevelsCounter = 0;

	for(const LevelEntry& entry: this->levels_list_v){
		u8 status = entry.status;

		if((status & LEVEL_PASSED) != 0){

			if(entry.number + 1 > next_level){
				this->next_level = entry.number + 1;
			}

			++passedLevelsCounter;
		}
	}


	if(require_all_levels){
		this->completed = passedLevelsCounter == this->levels_list_v.size();
	}
	else{
		this->completed = this->next_level > this->getHighestLevelNumber();
	}
}
