//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

//TODO
//Move it to Mapstore or EpisodeClass
#include "save_slots.hpp"

#include "episode/mapstore.hpp"
#include "system.hpp"

#include "engine/PLog.hpp"
#include "engine/PFilesystem.hpp"
#include "engine/PString.hpp"

#include "exceptions.hpp"

#include <cstring>
#include <string>
#include <sstream>
#include <algorithm>

namespace PK2save{

void to_json(nlohmann::json& j,const PK2SaveLevelEntry& entry){
    j["n"] = entry.level_name;
    j["s"] = entry.status;
	if(entry.level_id >=0 ){
		j["i"] = entry.level_id;
	}

	j["p"] = entry.best_score;
}

void from_json(const nlohmann::json& j, PK2SaveLevelEntry& entry){
    j.at("n").get_to(entry.level_name);
    j.at("s").get_to(entry.status);

	if(j.contains("i")){
		j["i"].get_to(entry.level_id);
	}
	else{
		entry.level_id = -1;
	}

	if(j.contains("p")){
		j["p"].get_to(entry.best_score);
	}
}


void to_json(nlohmann::json& j,const PK2SaveSlot& slot){
	j["empty"] = slot.empty;
	if(!slot.empty){
		j["next"] = slot.next_level;
		j["completed"] = slot.completed;
		j["episode"] = slot.episode;
		j["score"] = slot.score;
		j["levels"] = slot.levels;
		j["player"] = slot.player;
	}
}

void from_json(const nlohmann::json& j, PK2SaveSlot& slot){
	j.at("empty").get_to(slot.empty);
	if(!slot.empty){
		j.at("next").get_to(slot.next_level);
		j.at("completed").get_to(slot.completed);
		j.at("episode").get_to(slot.episode);
		j.at("score").get_to(slot.score);
		j.at("levels").get_to(slot.levels);
		j.at("player").get_to(slot.player);
	}
}


const int LEGACY_PE_PATH_SIZE = 128;
const int LEGACY_SAVE_SLOTS_NUMBER = 11;
const int SAVE_SLOTS_NUMBER = 11;

const int EPISODI_MAX_LEVELS = 100;

const char* SAVES_FILE = "saves.dat";


//Legacy save slots format


/**
 * @brief 
 * Legacy PK2
 */
struct PK2SAVE_V1{
	s32   next_level;
	char  episode[260]; //260, 256, 128?
	char  name[20];
	bool  not_empty;
	bool  levels_passed[100]; // 100, 50?
	bool  _[3]; // ?
	s32   score;
};

/**
 * @brief 
 * Danilo's era
 */
struct PK2SAVE_V3 {

	bool  empty;
	u32   next_level;
	char  episode[LEGACY_PE_PATH_SIZE];
	char  name[20];
	u32   score;
	u8    level_status[EPISODI_MAX_LEVELS];
	
};

std::vector<PK2SaveSlot> saveSlots;

void ClearSlots() {
	saveSlots.clear();
	saveSlots.resize(SAVE_SLOTS_NUMBER);
}


static std::string parseLegacyName(const std::string& s){
	std::string res = PString::lowercase(PString::rtrim(s));
	std::replace(res.begin(), res.end(), '-', ' ');
	return res;
}

static episode_entry LegacyFindEpisode(const std::string& episodeName){

	bool set = false;
	episode_entry res;

	for(const episode_entry& episode: episodes){
		if(episode.name.compare(episodeName) == 0){
			if (set)
				PLog::Write(PLog::WARN, "PK2", "Episode conflict on %s, choosing the first one", episodeName.c_str());
			else {
				res = episode;
				set = true;
			}
		}		
	}

	//Not found
	if(!set){
		/**
		 * On some old releases the character '-' was used instead of space
		 */

		std::string name_legacy = parseLegacyName(episodeName);
		for(const episode_entry& episode: episodes){
			if( parseLegacyName(episode.name).compare(name_legacy) == 0){
				if (set)
					PLog::Write(PLog::WARN, "PK2", "Episode conflict on %s, choosing the first one", episodeName.c_str());
				else {
					res = episode;
					set = true;
				}
			}		
		}
	}

	//falback
	if(!set){
		res.name = episodeName;
		res.is_zip = false;
	}

	return res;
}


void Save_All_Records() {

	PFile::Path path = PFilesystem::GetDataFileW(SAVES_FILE);
	try{
		PFile::RW file = path.GetRW2("w");
		file.write("4", 2);
		nlohmann::json j = saveSlots;
		file.writeCBOR(j);
		file.close();
	}
	catch(const PFile::PFileException& e){
		PLog::Write(PLog::ERR, "PK2", e.what());
		throw PExcept::PException("Can't save records");
	}
}


void LoadSaveSlots() {
	char version[2];

	PFile::Path path = PFilesystem::GetDataFileW(SAVES_FILE);
	if(!path.exists()){
		ClearSlots();
		Save_All_Records();
		return;
	}

	try
	{
		PFile::RW file = path.GetRW2("r");
		file.read(version, 2);
		version[1] = '\0';
		if(strncmp(version, "4", 2) == 0){
			PLog::Write(PLog::INFO, "PK2", "Loading save version 4");

			nlohmann::json j = file.readCBOR();
			saveSlots = j.get<std::vector<PK2SaveSlot>>();
		}
		else if (strncmp(version, "3", 2) == 0) {
			PLog::Write(PLog::INFO, "PK2", "Loading save version 3");

			//saveSlots.resize(LEGACY_SAVE_SLOTS_NUMBER);

			char count_c[8];
			int count = 0;

			file.read(count_c, sizeof(count_c));
			count_c[7] = '\0';
			count = atoi(count_c);
			if (count > LEGACY_SAVE_SLOTS_NUMBER)
				count = LEGACY_SAVE_SLOTS_NUMBER;

			for (int i = 0; i < count; i++) {
				PK2SAVE_V3 slot_v3;

				file.read(slot_v3.empty);
				file.read(slot_v3.next_level);
				file.read(slot_v3.episode, LEGACY_PE_PATH_SIZE);
				slot_v3.episode[LEGACY_PE_PATH_SIZE - 1 ] = '\0';

				file.read(slot_v3.name, 20);
				slot_v3.name[19] = '\0';

				file.read(slot_v3.score);
				file.read(slot_v3.level_status, EPISODI_MAX_LEVELS);

				PK2SaveSlot slot;
				slot.empty = slot_v3.empty;
				slot.completed = slot_v3.next_level == UINT32_MAX;
				slot.episode = LegacyFindEpisode(slot_v3.episode); 
				slot.player = slot_v3.name;
				slot.score = slot_v3.score;
				slot.next_level = slot_v3.next_level;

				for(int i=0;i<EPISODI_MAX_LEVELS;++i){
					PK2SaveLevelEntry entry;
					entry.level_id = i;
					entry.status = slot_v3.level_status[i];

					slot.levels.emplace_back(entry);
				}

				saveSlots.emplace_back(slot);
			}		
		}
		else if (strncmp(version, "1", 2) == 0) {

			PLog::Write(PLog::INFO, "PK2", "Loading save version 1");

			char count_c[8];
			int count = 0;

			file.read(count_c, sizeof(count_c));
			count_c[7] = '\0';
			count = atoi(count_c);
			if (count > LEGACY_SAVE_SLOTS_NUMBER)
				count = LEGACY_SAVE_SLOTS_NUMBER;

			
			int path_size = 0;
			int jakso_lapaisty_size = 0;
			int align_size = 0;

			int s = file.size();
			int save_size = (s - 10) / count;
			int char_field = save_size - 8;
			PLog::Write(PLog::INFO, "PK2", "Save char size = %i", char_field);

			switch(char_field) {

				case 384:
					path_size = 260;
					jakso_lapaisty_size = 100;
					align_size = 3;
					break;
				
				case 332:
					path_size = 260;
					jakso_lapaisty_size = 50;
					align_size = 1;
					break;

				case 380:
					path_size = 256;
					jakso_lapaisty_size = 100;
					align_size = 3;
					break;
				
				case 328:
					path_size = 256;
					jakso_lapaisty_size = 50;
					align_size = 1;
					break;

				case 252:
					path_size = 128;
					jakso_lapaisty_size = 100;
					align_size = 3;
					break;
				
				case 200:
					path_size = 128;
					jakso_lapaisty_size = 50;
					align_size = 1;
					break;

				default:
					throw std::runtime_error("Couldn't load save file, it's malfomed!");

			}

			for (int i = 0; i < count; i++) {

				PK2SAVE_V1 slot_v1;
				file.read(&slot_v1.next_level, 4);
				file.read(&slot_v1.episode, path_size);
				slot_v1.episode[path_size - 1] = '\0';

				file.read(&slot_v1.name, 20);
				slot_v1.name[19] = '\0';
				
				file.read(&slot_v1.not_empty, 1);
				file.read(&slot_v1.levels_passed, jakso_lapaisty_size);
				
				file.read(&slot_v1._, align_size);
				file.read(&slot_v1.score, 4);

				PK2SaveSlot slot;
				slot.empty = !slot_v1.not_empty;
				slot.completed = (u32)slot_v1.next_level == UINT32_MAX;
				slot.episode = LegacyFindEpisode(slot_v1.episode); 
				slot.player = slot_v1.name;
				slot.score = slot_v1.score;
				slot.next_level = slot_v1.next_level;

				for(int i=0; i<99; ++i){
					PK2SaveLevelEntry entry;
					entry.level_id = i;
					entry.status = slot_v1.levels_passed[i+1] ? LEVEL_PASSED : 0;


					slot.levels.emplace_back(entry);
				}

				saveSlots.emplace_back(slot);
			}

		} else {
			std::ostringstream os;
			os<<"Can't read this save version: \""<<version<<"\"";
			throw std::runtime_error(os.str());
		}

		file.close();

		/* code */
	}
	catch(const PFile::PFileException& e){
		PLog::Write(PLog::DEBUG, "PK2", e.what());
		PLog::Write(PLog::INFO, "PK2", "No save files found.");
		ClearSlots();
		Save_All_Records();
	}
	catch(const std::exception& e){
		PLog::Write(PLog::ERR, "PK2", "Cannot load save slots!");
		PLog::Write(PLog::ERR, "PK2", e.what());
		ClearSlots();
		Save_All_Records();
	}

	if(saveSlots.size() < SAVE_SLOTS_NUMBER){
		saveSlots.resize(SAVE_SLOTS_NUMBER);
	}
}


void LoadSlot(EpisodeClass* e, int slot_index){
	if(slot_index<0 || slot_index>=(int)saveSlots.size()){
		std::ostringstream os;
		os<<"Not allowed save slot index: "<<slot_index<<std::endl;
		throw std::runtime_error(os.str());
	}

	const PK2SaveSlot& slot = saveSlots[slot_index];
	if(slot.empty){
		std::ostringstream os;
		os<<"The save slot: "<<slot_index<<" is empty!"<< std::endl;
		throw std::runtime_error(os.str());
	}

	
	e->completed = slot.completed;

	e->next_level = slot.next_level;
	e->player_name = slot.player;

	int level_id = 0;
    for(const LevelEntry& levelEntry: e->getLevelEntries()){

        for(const PK2SaveLevelEntry& saveEntry: slot.levels){
            if(saveEntry.level_name==levelEntry.fileName){
                e->updateLevelStatus(level_id, saveEntry.status);
				//e->player_score += saveEntry.best_score;
				e->updateLevelBestScore(level_id, saveEntry.best_score);
                break;
            }
			//For older saves
			else if( level_id == saveEntry.level_id ){
				e->updateLevelStatus(level_id, saveEntry.status);
				// best scores for levels weren't saved
                break;
			}
        }

        ++level_id;
    }
}

void SaveSlot(EpisodeClass* e, int slot_index) {

	if(slot_index<0 || slot_index>=(int)saveSlots.size()){
		std::ostringstream os;
		os<<"Not allowed save slot index: "<<slot_index<<std::endl;
		throw std::runtime_error(os.str());
	}

	PK2SaveSlot slot;

	slot.empty = false;

	slot.episode = e->entry;	
	slot.completed = e->completed;
	slot.score = e->getPlayerScore();
	slot.next_level = e->next_level;
	slot.player = e->player_name;

	int level_id = 0;
	for(const LevelEntry& levelEntry: e->getLevelEntries()){
        PK2SaveLevelEntry saveEntry;
        saveEntry.level_name = levelEntry.fileName;
        saveEntry.status = e->getLevelStatus(level_id);
		saveEntry.best_score = e->getLevelBestScore(level_id);
        ++level_id;

        slot.levels.emplace_back(saveEntry);
    }

	saveSlots[slot_index] = slot;
	Save_All_Records();
}


}