#pragma once
#include "prototypes_handler.hpp"
#include "spriteclass.hpp"

#include <list>

class LevelSector;

class SpritesHandler{
public:
    SpritesHandler(LevelSector* sector):mLevelSector(sector){}

    ~SpritesHandler(){
        clearAll();
    };
    void clearAll();
    
    void sortBg();
    void onSkullBlocksChanged();
    void onEvent1();
    void onEvent2();
    void onOpeningKeylocks();

    int onTickUpdate(int camera_x, int camera_y);
    
    /**
     * @brief 
     * Spawning the player
     */
    SpriteClass* addPlayer(PrototypeClass*prototype, double x, double y);

    /**
     * @brief 
     * Spawn a sprite based on the level sprite tiles
     */
    void addLevelSprite(PrototypeClass*prototype, double x, double y);

    /**
     * @brief 
     * Spawn a bonus sprite dropped upon another sprite's death
     */
    void addDroppedBonusSprite(PrototypeClass*prototype, double x, double y);
    /**
     * @brief 
     * Spawn a sprite as a projectile
     */
    void addProjectileSprite(PrototypeClass* prototype, SpriteClass* shooter, const std::optional<Point2D> & offset);
    /**
     * @brief 
     * Spawn a sprite from player's gift
     */
    void addGiftSprite(PrototypeClass* prototype, SpriteClass* player);
    /**
     * @brief 
     * Spawn a sprite from lua
     */

    SpriteClass* addLuaSprite(PrototypeClass* prototype, double x, double y, SpriteClass*parent=nullptr){
        
        return this->mCreateSprite(prototype, 0, x, y, parent);
    }
    
    std::list<SpriteClass*> Sprites_List;
    std::list<SpriteClass*> bgSprites_List;
    std::list<SpriteClass*> fgSprites_List;

    LevelSector* mLevelSector;

    void drawSprites(int camera_x, int camera_y, bool gamePaused, int& debug_drawn_sprites);
    void drawBGsprites(int camera_x, int camera_y, bool gamePaused, int& debug_drawn_sprite);
    void drawFGsprites(int camera_x, int camera_y, bool gamePaused, int& debug_drawn_sprite);


    nlohmann::json toJson()const;
    void fromJSON(const nlohmann::json& j, PrototypesHandler& handler, LevelSector*sector);


    std::size_t size()const{
        return this->Sprites_List.size();
    }
    SpriteClass* getSpriteById(std::size_t id);
    SpriteClass * findPlayer();

    SpriteClass* findNearestTarget(const SpriteClass* agent)const;
private:

    void spriteToJson(nlohmann::json& j, const SpriteClass&s)const;
    void jsonToSprite(const nlohmann::json&j, SpriteClass&s, PrototypesHandler&handler)const;

    SpriteClass * mCreateSprite(PrototypeClass* prototype, int player_c, double x, double y, SpriteClass*parent_sprite=nullptr);

    void mAddBG(SpriteClass* sprite) {
        bgSprites_List.push_back(sprite);
    }

    void mAddFG(SpriteClass* sprite){
        fgSprites_List.push_back(sprite);
    }

    bool spriteDestructed (SpriteClass* sprite);
};