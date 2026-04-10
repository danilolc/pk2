//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief
 * Lua utils by SaturninTheAlien
 */

#include "lua_sprite_class.hpp"
#include "game/prototype.hpp"
#include "game/spriteclass.hpp"
#include "game/levelsector.hpp"
#include "sfx.hpp"
namespace PK2lua{

void ExposePrototypeClass(sol::state& lua){

    /**
     * @brief 
     * Prototypes should be read-only not to change them during the gameplay.
     */

    lua.new_usertype<PrototypeClass>(
        "Prototype",
        sol::no_constructor,
        "name", sol::readonly(&PrototypeClass::name),
        "ammo1", sol::readonly(&PrototypeClass::ammo1),
        "ammo2", sol::readonly(&PrototypeClass::ammo2),
        "bonus", sol::readonly(&PrototypeClass::bonus),
        "transformation", sol::readonly(&PrototypeClass::transformation),
        "width", sol::readonly(&PrototypeClass::width),
        "height", sol::readonly(&PrototypeClass::height),
        "weight", sol::readonly(&PrototypeClass::weight),
        "enemy", sol::readonly(&PrototypeClass::enemy),
        "hostileToEveryone", sol::readonly(&PrototypeClass::hostile_to_everyone),
        "indestructible", sol::readonly(&PrototypeClass::indestructible),

        "color", sol::readonly(&PrototypeClass::color),
        "destructionEffect", sol::readonly(&PrototypeClass::destruction_effect),
        "energy", sol::readonly(&PrototypeClass::energy),
        "maxSpeed", sol::readonly(&PrototypeClass::max_speed));
}

void SpriteMakeSound(SpriteClass*sprite, int soundIndex){
    if(sprite!=nullptr&&soundIndex>=0 && soundIndex<(int)sprite->prototype->sounds.size()){
        Play_GameSFX(sprite->prototype->sounds[soundIndex],100, (int)sprite->x, (int)sprite->y,
        sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);
    }
}

SpriteClass* SpriteFindNearestTarget(SpriteClass* agent){
    if(agent==nullptr)return nullptr;
    return agent->level_sector->sprites.findNearestTarget(agent);
}

void ExposeSpriteClass(sol::state& lua){
    lua.new_usertype<SpriteClass>(
        "SpriteClass",
        sol::no_constructor,

        "id", &SpriteClass::id,

        /**
         * @brief 
         * Functions
         */

        "transform", &SpriteClass::transform,
        "transformTo", &SpriteClass::transformTo,

        "die", &SpriteClass::die,
        "flyToWaypointX", &SpriteClass::flyToWaypointX,
        "flyToWaypointY", &SpriteClass::flyToWaypointY,
        "flyToWaypointXY", &SpriteClass::flyToWaypointXY,
        "findNearestTarget", &SpriteFindNearestTarget,
        "makeSound", &SpriteMakeSound,

        /**
         * @brief 
         * Writeable fields
         */

        "active", &SpriteClass::active,
        "prototype", &SpriteClass::prototype,
        
        "x", &SpriteClass::x,
        "y", &SpriteClass::y,

        "a", &SpriteClass::a, // x velocity
        "b", &SpriteClass::b, // y velocity

        "flipX", &SpriteClass::flip_x,
        "flipY", &SpriteClass::flip_y,

        "energy", &SpriteClass::energy,
        "weight", &SpriteClass::weight,
        "crouched", &SpriteClass::crouched,

        "damageTimer", &SpriteClass::damage_timer,
        "invisibleTimer", &SpriteClass::invisible_timer,
        "superModeTimer", &SpriteClass::super_mode_timer,
        "attack1Timer", &SpriteClass::attack1_timer,
        "attack2Timer", &SpriteClass::attack2_timer,

        "enemy", &SpriteClass::enemy,
        "ammo1", &SpriteClass::ammo1,
        "ammo2", &SpriteClass::ammo2,

        /**
         * @brief 
         * Read-only fields
         * Shouldn't be changed in scripts because they are handled by physics.cpp and so on
         */

        //"player", sol::readonly(&SpriteClass::player),
        "player", &SpriteClass::player_c,

        "origX", sol::readonly(&SpriteClass::orig_x),
        "origY", sol::readonly(&SpriteClass::orig_y),

        "inWater", sol::readonly(&SpriteClass::in_water),
        "swimming", sol::readonly(&SpriteClass::swimming),

        "hidden", sol::readonly(&SpriteClass::hidden),
        "initialWeight", sol::readonly(&SpriteClass::initial_weight),
        "jumpTimer", &SpriteClass::jump_timer,

        "seenPlayerX", sol::readonly(&SpriteClass::seen_player_x),
        "seenPlayerY", sol::readonly(&SpriteClass::seen_player_y),
        

        "canMoveLeft", sol::readonly(&SpriteClass::can_move_left),
        "canMoveRight", sol::readonly(&SpriteClass::can_move_right),
        "canMoveUp", sol::readonly(&SpriteClass::can_move_up),
        "canMoveDown", sol::readonly(&SpriteClass::can_move_down),

        "levelSector", sol::readonly(&SpriteClass::level_sector),
        "parent", sol::readonly(&SpriteClass::parent_sprite),
        "target", &SpriteClass::target_sprite,
        "canCollectBonuses", &SpriteClass::can_collect_bonuses,
        "canPushBonuses", &SpriteClass::can_push_bonuses
        
        );
}

}