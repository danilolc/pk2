#pragma once

#include "prototype.hpp"
#include "engine/PFile.hpp"

class EpisodeClass;

class PrototypesHandler{

public:
    PrototypesHandler(EpisodeClass* episode, bool isCommandLineTool=false)
    :mEpisode(episode), commandLineTool(isCommandLineTool){
        
    }
    ~PrototypesHandler(){
        clear();
    }
    /**
     * @brief
     * Do not copy objects of this class!
     */
    PrototypesHandler(const PrototypesHandler&)=delete;
    PrototypesHandler& operator=(const PrototypesHandler&)=delete;

    void clear();

    /**
     * @brief 
     * Load sprite prototype by name
     */
    PrototypeClass* loadPrototype(const std::string& filename_in);

    /**
     * @brief 
     * Save prototype to file
     */
    void savePrototype(PrototypeClass*prototype, const std::string& filename)const;

    /**
     * @brief 
     * Get sprite prototype by index for the purpose of debugging.
     * If out of range, nullptr is returned.
     */
    PrototypeClass* get(std::size_t index)const;

    void loadSpriteAssets();
    void unloadSpriteAssets();

    std::size_t size(){
        return this->mPrototypes.size();
    }
private:
    /**
     * @brief 
     * if should load dependecies sprites such as ammo, transformation and bonus
     */
    bool mAssetsLoaded = false;

    

    /**
     * @brief 
     * Episode pointer
     */
    class EpisodeClass*mEpisode = nullptr;
    bool commandLineTool = true;

    std::vector<PrototypeClass*>mPrototypes;
};