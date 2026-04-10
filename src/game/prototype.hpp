#pragma once

#include "engine/PFile.hpp"
#include "engine/types.hpp"

#include <list>
#include <vector>
#include <string>
#include <map>
#include <array>
#include <optional>
#include <functional>

#include "spriteclass_constants.hpp"
#include "sprite_ai_commands.hpp"
#include "sprite_ai_table.hpp"

/**
 * @brief 
 * Legacy classes declaration
 */
struct LegacySprAnimation;
struct PrototypeClass10;
struct PrototypeClass11;
struct PrototypeClass12;
struct PrototypeClass13;


class EpisodeClass;

class SpriteAnimation{
public:
    SpriteAnimation(){};
    SpriteAnimation(const LegacySprAnimation& anim);
    bool loop = false;
    u32 intro = 0;

    std::vector<int> sequence;

    friend void to_json(nlohmann::json& j, const SpriteAnimation& a);
    friend void from_json(const nlohmann::json&j, SpriteAnimation& a);
};

class PrototypeClass{
public:

    static const std::map<std::string, int> SoundTypesDict;
    static const std::map<std::string, int> AnimationsDict;
    static const std::map<std::string, u8> ColorsDict;

    std::string version = "2.0";

    std::string filename;
    std::string picture_filename;
    
    int     type = TYPE_NOTHING;

    std::array<std::string, SPRITE_SOUNDS_NUMBER> sound_files = {""};
    std::array<int, SPRITE_SOUNDS_NUMBER> sounds = {-1, -1, -1,  -1, -1, -1,  -1};

    //int     framet[SPRITE_MAX_FRAMEJA] = {0};
    std::vector<int>frames;
    std::vector<int>frames_mirror;
    //int     framet_peilikuva[SPRITE_MAX_FRAMEJA] = {0};
    int      frames_number            = 0;

    std::array<SpriteAnimation, SPRITE_ANIMATIONS_NUMBER> animations;
    //PK2SPRITE_ANIMAATIO animaatiot[SPRITE_MAX_ANIMAATIOITA] = {};
    //u8      animations_number       = 0;
    int      frame_rate         = 0;
    int     picture_frame_x             = 0;
    int     picture_frame_y             = 0;
    int     picture_frame_width  = 0;
    int     picture_frame_height = 0;

    std::string name;
    int     width    = 0;
    int     height   = 0;
    double  weight    = 0;

    bool    enemy     = false;

    /**
     * @brief 
     * If true, the sprite is enemy towards any other sprite
     */
    bool    hostile_to_everyone   = false;

    int     energy        = 0;
    int     damage        = 0;
    int      damage_type = DAMAGE_IMPACT;
    int      immunity_type        = DAMAGE_NONE;
    int     score        = 0;

    std::vector<int> AI_v;
    std::vector<SpriteAI::AI_Class> AI_f;
    std::vector<SpriteAI::ProjectileAIClass> AI_p;

    int first_ai()const{
        return AI_v.empty()? 0 : AI_v[0];
    }

    int      max_jump    = 0;
    double  max_speed   = 3;
    int     charge_time  = 0;
    u8      color         = COLOR_NORMAL;
    bool    is_wall         = false;
    
    int     destruction_effect = FX_DESTRUCT_NO_EFFECT;
    bool    indestructible = false; //if true only DAMAGE_ALL can really hurt the sprite


    //int     how_destroyed = FX_DESTRUCT_ANIMATED;
    
    
    bool    can_open_locks        = false;
    bool    vibrates      = false;
    int      bonuses_number = 1;
    int     attack1_time = 60;
    int     attack2_time = 60;

    int     parallax_type = 0;

    std::string transformation_str;
    std::string bonus_str;
    std::string ammo1_str;
    std::string ammo2_str;


    PrototypeClass* transformation     = nullptr;
    PrototypeClass* bonus      = nullptr;
    PrototypeClass* ammo1     = nullptr;
    PrototypeClass* ammo2     = nullptr;

    bool    check_tiles = true;
    int     sound_frequency      = 22050;
    bool    random_sound_frequency    = true;

    bool    is_wall_up       = true;
    bool    is_wall_down       = true;
    bool    is_wall_right   = true;
    bool    is_wall_left = true;

    u8      effect       = EFFECT_NONE;
    bool    is_transparent       = false;
    int     projectile_charge_time    = 0;
    bool    can_glide    = false;
    //bool    boss         = false; //unused
    bool    bonus_always = false;
    bool    can_swim     = false;


    /**
     * @brief
     * Transparency / experimental
     */
    int blend_mode = 0;
    int blend_alpha = 50;

    /**
     * @brief 
     * Field for the purpose of counting.
     * If Episode->ignore_collectable==false, this field will be always false.
     */
    bool    big_apple     = false;   //
    /**
     * @brief 
     * The ID of displayed text in the "shoutbox"
     */
    int     info_id = 0;

    /**
     * @brief 
     * If true a sprite will be active even far from the camera
     */
    bool    always_active = false;   // 

    /**
     * @brief 
     * If the creature sprite is ambient (like white butterflies or chain),
     * it should not consume attacks or interact with other sprites 
     */
    bool    ambient = false;

    std::optional<bool> can_push_bonuses = {};


    /**
     * @brief 
     * Weight when KO'd variable
     * If has_dead_weight set to false, to legacy behaviour
     * (only sprites with weight 0 fall) 
     */

    std::optional<double> dead_weight = {};

    /**
     * @brief
     * Ammo offesets
     */
    std::optional<Point2D> ammo1_offset = {};
    std::optional<Point2D> ammo2_offset = {};

    /**
     * @brief 
     * Player detection range
     */

    Point2D player_detection = Point2D(200, 350);

    /**
     * @brief
     * To fix the bomb and legacy projectiles bugs with the new sprite system
     */
    bool    legacy_projectile = false;

    PrototypeClass();
    ~PrototypeClass();

    /**
     * @brief 
     * Load JSON sprite prototype
     */
    void     loadPrototypeJSON(PFile::Path path,
        std::function<PrototypeClass*(const std::string&)> fn_loadPrototype=nullptr);
    /**
     * @brief 
     * Load legacy sprite prototype
     */
    void     loadPrototypeLegacy(PFile::Path path);

    /**
     * @brief 
     * Load sprite assets like texture, sounds, etc.
     */
    void     loadAssets();

    /**
     * @brief 
     * Unload sprite assets
     */
    void    unloadAssets();

    /**
     * @brief 
     * Missing sprite placeholder
     */

    void    initMissingPlaceholder();

    
    void     draw(int x, int y, int frame)const;
    bool    hasAI(int AI)const;

    std::vector<SpriteCommands::Command*>commands;
    nlohmann::json commands_json;

    friend void to_json(nlohmann::json& j, const PrototypeClass& c);
private:
    void    setProto10(PrototypeClass10 &proto);
    void    setProto11(PrototypeClass11 &proto);
    void    setProto12(PrototypeClass12 &proto);
    void    setProto13(PrototypeClass13 &proto);

    void    setProto20(const nlohmann::json& j);

    bool    mAssetsLoaded = false;
};