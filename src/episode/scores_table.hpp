//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PFile.hpp"
#include "engine/PJson.hpp"

#include <string>
#include <vector>

class LevelScore{
public:
    std::string levelFileName;
    int levelNumber = 0;

    //bool hasScore = true;
    int bestScore = 0;
    std::string topPlayer;
    int maxApples = 0;
    bool hasTime = false;
    int bestTime = 0;
    std::string fastestPlayer;

    friend void to_json(nlohmann::json& j,const LevelScore& score);
    friend void from_json(const nlohmann::json& j, LevelScore& score);
};


class ScoresTable{
private:
    std::vector<LevelScore> scores;
public:
    bool indexedByLevelName = false;
    int  episodeTopScore = 0;
    std::string episodeTopPlayer;
    

    void load(PFile::Path path);
    void save(PFile::Path path)const;

    friend void to_json(nlohmann::json& j,const ScoresTable& st);
    friend void from_json(const nlohmann::json& j, ScoresTable& st);
    LevelScore* getScoreByLevelNumber(int levelNumber);
    void addScore(const LevelScore& score){
        this->scores.emplace_back(score);
    }

    LevelScore* getScoreByLevelName(const std::string& levelName);
};