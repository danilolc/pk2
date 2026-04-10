//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once
#include <string>
#include <map>
#include <vector>
#include <functional>
#include "spriteclass_constants.hpp"


class SpriteClass;
class PrototypeClass;

enum AI_TRIGGER{  
    AI_TRIGGER_NONE,
    AI_TRIGGER_ANYWAY, //on each game tick 
    AI_TRIGGER_ALIVE, // on each game tick if sprite->energy>0


    AI_TRIGGER_DEATH,  //if the sprite dies
    AI_TRIGGER_DAMAGE, //if the sprite is damaged

    AI_TRIGGER_OFFSCREEN, //if the sprite goes offscreen

    AI_TRIGGER_SPAWN,  //spawn
    AI_TRIGGER_TRANSFORMATION,  //spawn and transformation,

    AI_TRIGGER_SKULLS_CHANGED, //if skull blocks are changed
    AI_TRIGGER_EVENT1, //if event1
    AI_TRIGGER_EVENT2, //if event2

    AI_TRIGGER_KEYLOCKS_OPENED, //when player has collected all the keys


    AI_TRIGGER_BONUS_COLLECTED, //if bonus sprite is collected

    //considering them
    AI_TRIGGER_PLAYER_ABOVE,
    AI_TRIGGER_PLAYER_BELOW,
    AI_TRIGGER_PLAYER_IN_FRONT,
};

namespace AI_Functions{
    extern SpriteClass* player;
    extern SpriteClass* player_invisible;
    void DisplayInfo(SpriteClass* sprite);
}

namespace SpriteAI{


class AI_Class{
public:
    AI_Class() = default;
    int id = AI_NONE;
    int trigger = AI_TRIGGER_NONE;
    void (*func)(SpriteClass*sprite) = nullptr;

    bool apply_to_player = false;
    bool apply_to_creatures = false;
    bool apply_to_bonuses = false;
    bool apply_to_backgrounds = false;

    /**
     * @brief 
     * If the player sprite is required to execute the AI (for example Attak if player below)
     */
    bool requires_player_sprite = false;
};

/**
 * @brief 
 * AI functions for projectiles
 */

class ProjectileAIClass{
public:
    ProjectileAIClass() = default;
    void (*func)(SpriteClass*sprite, SpriteClass*shooter) = nullptr;
    int id = AI_NONE;
};

/*
AI Table, singleton
*/
class AI_Table{
public:
    static AI_Table INSTANCE;

    /*void InitSpriteAIs(std::vector<AI_Class>& ai_vec, const std::vector<int>& ai_indices)const;
    void InitSpriteProjectileAIs(std::vector<ProjectileAIClass>& ai_vec, const std::vector<int>& ai_indices)const;*/
    void InitSpritePrototypeAIs(PrototypeClass* sprite_prototype)const;

    AI_Table(const AI_Table& src) = delete;
    AI_Table& operator=(const AI_Table& src) = delete;
    

    AI_Class missing_sprite_AI;
    AI_Class missing_texture_AI;

private:
    AI_Table();
    void Init_AI(int id,
        int trigger,
        void (*func)(SpriteClass*),
        bool creatures=true,
        bool player=false,
        bool bonuses=false,
        bool backgrounds=false,
        bool requires_player_sprite = false);

    void Init_AI_Projectile(int id, void (*func)(SpriteClass*, SpriteClass*));


    std::map<int, AI_Class> mAIsDict;
    std::map<int, ProjectileAIClass> mProjectileAIsDict;
};

}