//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include <string>
#include <vector>
#include <optional>

#include "mapstore.hpp"
#include "scores_table.hpp"

#include "engine/types.hpp"
#include "engine/PFile.hpp"
#include "engine/PLang.hpp"
#include "engine/PZip.hpp"

#include "sfx.hpp"

//50

class ProxyLevelEntry{
public:
	u32 weight = 1;
	std::string filename;
};

class LevelEntry{
public:
	std::string fileName;
	std::string levelName;

	int map_x = 0;
	int map_y = 0;

	int icon_id = 0;
	u32 number = 0;
	u8 status = 0;

	int best_score = 0;

	std::optional<std::vector<ProxyLevelEntry>> proxies;

	void loadLevelHeader(PFile::Path levelFile);
	std::string getLevelFilename(bool proxy)const;

};

enum LEVEL_STATUS {

	LEVEL_PASSED    = 0b001,
	LEVEL_ALLAPPLES = 0b010,
	LEVEL_HAS_BIG_APPLES = 0b100
};

class EpisodeClass {
	public:	
		episode_entry entry;
		PZip::PZip source_zip;

		std::string player_name;

		int getPlayerScore()const{
			return this->player_score;
		}

		

		u32 next_level = 1;
		//u32 level_count = 0;

		bool glows = false;
		bool hide_numbers = false;
		bool ignore_collectable = false;
		bool require_all_levels = false;
		bool no_ending = false;
		std::string collectable_name = "big apple";
		bool transformation_offset = false;
		bool visible_wind = false;
		bool supermode_music = true;
		bool legacy_start_offset = false;
		bool legacy_camera_offset = false;
		bool legacy_obstacle_phasing = false;
		int checkpointPenalty = 150;

		ScoresTable scoresTable;

		PLang infos;

		EpisodeClass(const std::string& player_name, episode_entry entry);
		~EpisodeClass();

		void load();

		
		void loadAssets();

		void  saveScores();
		

		SfxHandler sfx;

		std::size_t getLevelsNumber()const{
			return this->levels_list_v.size();
		}

		u8 getLevelStatus(int level_id)const;
		int getLevelBestScore(int level_id)const;


		void updateLevelStatus(int level_id, u8 status);
		void updateLevelBestScore(int level_id, int best_score);

		std::string getLevelFilename(int level_id, bool executeProxies=false)const;
		int findLevelbyFilename(const std::string& levelFilename)const;

		const std::vector<LevelEntry>& getLevelEntries()const{
			return this->levels_list_v;
		}
		
		u32 getHighestLevelNumber()const{
			return this->highestLevelNumber;
		}

		bool isCompleted()const{
			return this->completed;
		}

		bool completed = false;
	private:
		int player_score = 0;

		void loadInfo();
		void loadLevels();
		u32 highestLevelNumber = 0;
		void updateNextLevel();

		std::vector<LevelEntry> levels_list_v;

		std::string getScoresPath()const;		
		void openScores();
};

extern EpisodeClass* Episode;
