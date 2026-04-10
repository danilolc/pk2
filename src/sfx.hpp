//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once
#include <vector>
#include <string>
#include <map>

const int SOUND_SAMPLERATE = 22050;

class EpisodeClass;

class SfxHandler{
public:
    static const std::map<std::string, int SfxHandler::*> soundFilenames;

    SfxHandler()=default;
    ~SfxHandler(){
        free();
    }

    /**
     * @brief Construct a new Sfx Handler object
     * Do not copy objects!
     */
    SfxHandler(const SfxHandler& src)=delete;
    SfxHandler(SfxHandler&& src)=delete;
    SfxHandler& operator=(const SfxHandler& src)=delete;    
    SfxHandler& operator=(SfxHandler&&src)=delete;


    int switch_sound = -1;
    int jump_sound = -1;
    int splash_sound = -1;
    int open_locks_sound = -1;
    int menu_sound = -1;
    int moo_sound = -1;
    int doodle_sound = -1;
    int pump_sound = -1;
    int score_sound = -1;
    int apple_sound = -1;
    int thunder_sound = -1;

    void loadAll();
    void loadAllForEpisode(const SfxHandler& src, EpisodeClass*episode=nullptr);
    void free();
private:
    int mLoadSound(const std::string& name);
    int mLoadSoundEpisode(int prev, const std::string&name, EpisodeClass*episode);
    std::vector<int> mSounds;
};

extern SfxHandler sfx_global;

void Update_GameSFX();
void Play_GameSFX(int aani, int voimakkuus, int x, int y, int freq, bool random_freq);
void Play_MenuSFX(int aani, int voimakkuus);