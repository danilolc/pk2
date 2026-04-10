//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "scores_table.hpp"
#include "engine/PLog.hpp"
#include <iostream>

static constexpr int LEGACY_MAX_LEVELS_NUMBER = 100;

void to_json(nlohmann::json& j,const LevelScore& score){
    j["level"] = score.levelFileName;
    j["number"] = score.levelNumber;
    j["best"] = score.bestScore;
    j["top_player"] = score.topPlayer;
    j["max_apples"] = score.maxApples;
    j["has_time"] = score.hasTime;
    if(score.hasTime){
        j["best_time"] = score.bestTime;
        j["fastest_player"] = score.fastestPlayer;
    }    
}


void from_json(const nlohmann::json& j, LevelScore& score){
    j.at("level").get_to(score.levelFileName);
    j.at("number").get_to(score.levelNumber);
    j.at("best").get_to(score.bestScore);

    j.at("top_player").get_to(score.topPlayer);
    j.at("max_apples").get_to(score.maxApples);
    j.at("has_time").get_to(score.hasTime);
    if(score.hasTime){
        j.at("best_time").get_to(score.bestTime);
        j.at("fastest_player").get_to(score.fastestPlayer);
    }
}


void to_json(nlohmann::json& j,const ScoresTable& st){
    j["top_player"] = st.episodeTopPlayer;
    j["top_score"] = st.episodeTopScore;
    j["scores"] = st.scores;
}

void from_json(const nlohmann::json& j, ScoresTable& st){
    j.at("top_player").get_to(st.episodeTopPlayer);
    j.at("top_score").get_to(st.episodeTopScore);
    j.at("scores").get_to(st.scores);
}

//Scores 1.0
struct PK2EPISODESCORES10 {

	u32  best_score[LEGACY_MAX_LEVELS_NUMBER];         // the best score of each level in episode
	char top_player[LEGACY_MAX_LEVELS_NUMBER][20];     // the name of the player with more score in each level on episode
	u32  best_time[LEGACY_MAX_LEVELS_NUMBER];          // the best time in (dec)conds
	char fastest_player[LEGACY_MAX_LEVELS_NUMBER][20]; // the name of the fastest player in each level

	u32  episode_top_score;
	char episode_top_player[20];
	
};



void ScoresTable::load(PFile::Path path){
    if(!path.exists())return;
    char version[4];

    PFile::RW file = path.GetRW2("r");
    file.read(version, 4);
    version[3] = '\0';

    if (strncmp(version, "1.2", 4) == 0){
        PLog::Write(PLog::INFO, "PK2", "Loading scores v1.2");

        nlohmann::json j = file.readCBOR();
        file.close();
        
        from_json(j, *this);
    }
    else if (strncmp(version, "1.1", 4) == 0) {

        PLog::Write(PLog::INFO, "PK2", "Loading scores v1.1");
        
        u32 count;
        file.read(count);

        if (count > LEGACY_MAX_LEVELS_NUMBER)
            count = LEGACY_MAX_LEVELS_NUMBER;

        for (u32 i = 0; i < count; ++i) {

            u8 has_score = 0;
            file.read(has_score);

            u32  best_score = 0;
            file.read(best_score);

            char top_player[20];
            file.read(top_player, 20);
            top_player[19] = '\0';

            u32  max_apples = 0;
            file.read(max_apples);

            u8   has_time = 0;
            file.read(has_time);

            s32  best_time = 0;
            file.read(best_time);

            char fastest_player[20];
            file.read(fastest_player, 20);
            fastest_player[19] = '\0';

            if(!has_score)continue;

            LevelScore score;
            score.bestScore = best_score;
            score.topPlayer = top_player;
            score.maxApples = max_apples;
            score.hasTime = has_time;
            score.bestTime = best_time;
            score.fastestPlayer = fastest_player;
            
            score.levelNumber = i;

            this->scores.emplace_back(score);
        }

        u32  episode_top_score = 0;
        file.read(episode_top_score);

        this->episodeTopScore = episode_top_score;

        char episode_top_player[20];
        file.read(episode_top_player, 20);
        episode_top_player[19] = '\0';

        this->episodeTopPlayer = episode_top_player;

        file.close();

    } else if (strncmp(version, "1.0", 4) == 0) {

        PLog::Write(PLog::INFO, "PK2", "Loading scores v1.0");

        PK2EPISODESCORES10 temp;
        file.read(&temp, sizeof(temp));
        file.close();

        for (int i = 0; i < LEGACY_MAX_LEVELS_NUMBER; i++) {

            if(temp.best_score[i] == 0)continue;

            LevelScore score;
            score.bestScore = temp.best_score[i];

            temp.top_player[i][19] = '\0';
            score.topPlayer = temp.top_player[i];

            score.hasTime = (temp.best_time[i] != 0);
            score.maxApples = 0;

            score.bestTime = temp.best_time[i] * 100;

            temp.fastest_player[i][19] = '\0';

            score.fastestPlayer = temp.fastest_player[i];
        }

        this->episodeTopScore = temp.episode_top_score;

        temp.episode_top_player[19] = '\0';
        this->episodeTopPlayer = temp.episode_top_player;
    } else {
        PLog::Write(PLog::INFO, "PK2", "Can't read this scores version: \"%s\"", version);
        file.close();
    }
}

void ScoresTable::save(PFile::Path path)const{
    PFile::RW file = path.GetRW2("w");
    char version[4] = "1.2";
    file.write(version, 4);

    nlohmann::json j = *this;

    file.writeCBOR(j);

    file.close();
}


LevelScore* ScoresTable::getScoreByLevelNumber(int levelNumber){
    for(LevelScore& ls: this->scores){
        if(ls.levelNumber==levelNumber){
            return &ls;
        }
    }

    return nullptr;
}

LevelScore* ScoresTable::getScoreByLevelName(const std::string& levelName){
    for(LevelScore& ls: this->scores){
        if(ls.levelFileName==levelName){
            return &ls;
        }
    }

    return nullptr;
}