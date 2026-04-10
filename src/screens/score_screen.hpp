//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once
#include "screen.hpp"

class ScoreScreen: public Screen{
public:
    ScoreScreen();
    ~ScoreScreen();
    void Init()override;
    void Loop()override;

private:
    int LevelScore_Compare(int level, int score, int apples, int time);
    int EpisodeScore_Compare(int score);
    int Draw_ScoreCount();


    bool going_to_map = false;

    int counting_phase = 0;
    int counting_delay = 0;

    //float apples_xoffset = 0.f;
    u32 apples_counted = 0;
    u32 apples_not_counted = 0;

    int total_score = 0;
    int bonus_score = 0;
    int time_score = 0;
    int energy_score = 0;
    int gifts_score = 0;

    bool map_new_record = false;
    bool map_new_time_record = false;
    bool episode_new_record = false;
};

