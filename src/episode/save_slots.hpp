//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

//TODO
//Move it to Mapstore or EpisodeClass

#include "episode/episodeclass.hpp"
#include "engine/types.hpp"
#include "engine/PJson.hpp"
#include "mapstore.hpp"

#include <string>
#include <array>
#include <vector>

namespace PK2save{

class PK2SaveLevelEntry{
public:
    std::string level_name;
    u8 status = 0;

	/**
	 * It should be -1 for save slots V4
	 * It's only for backwards compatibility with old saves.
	*/
	int level_id = -1; 
	int best_score = 0;
};

void to_json(nlohmann::json& j,const PK2SaveLevelEntry& entry);
void from_json(const nlohmann::json& j, PK2SaveLevelEntry& entry);

class PK2SaveSlot{
public:
	bool empty = true;
	episode_entry episode;

	std::string player;

	bool completed = false;
	int score = 0;
	u32 next_level = 1;

	std::vector<PK2SaveLevelEntry> levels;
};

void to_json(nlohmann::json& j,const PK2SaveSlot& slot);
void from_json(const nlohmann::json& j, PK2SaveSlot& slot);

extern std::vector<PK2SaveSlot> saveSlots;
//extern std::array<PK2SaveSlot, 11> saveSlots;

void LoadSaveSlots();

void LoadSlot(EpisodeClass* episode, int slot_index);
void SaveSlot(EpisodeClass* episode, int slot_index);

}