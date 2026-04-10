//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "sfx.hpp"

#include "exceptions.hpp"

#include "system.hpp"
#include "settings/settings.hpp"
#include "game/game.hpp"

#include "engine/PLog.hpp"
#include "engine/PSound.hpp"
#include "engine/PFilesystem.hpp"

#include <cmath>
#include <sstream>

struct GameSFX {

    bool used;
    u32 x, y; // TODO - Update sprite position
    int volume;

};

GameSFX sfx_list[PSound::CHANNELS];

const std::map<std::string, int SfxHandler::*> SfxHandler::soundFilenames = {
    {"switch3.wav", &SfxHandler::switch_sound},
    {"jump4.wav", &SfxHandler::jump_sound},
    {"splash.wav", &SfxHandler::splash_sound},
    {"openlock.wav", &SfxHandler::open_locks_sound},
    {"menu2.wav", &SfxHandler::menu_sound},
    {"moo.wav", &SfxHandler::moo_sound},
    {"doodle.wav", &SfxHandler::doodle_sound},
    {"pump.wav", &SfxHandler::pump_sound},
    {"counter.wav", &SfxHandler::score_sound},
    {"app_bite.wav", &SfxHandler::apple_sound},
    {"thunder.wav", &SfxHandler::thunder_sound}
};


int SfxHandler::mLoadSound(const std::string& name){

    namespace fs = std::filesystem;

    std::optional<PFile::Path> path = PFilesystem::FindVanillaAsset(name, PFilesystem::SFX_DIR);
    if(!path.has_value()){
        throw PExcept::FileNotFoundException(name, PExcept::MISSING_SFX);
    }

    int result = PSound::load_sfx(*path);
    if(result==-1){
        throw PExcept::FileNotFoundException(name, PExcept::MISSING_SFX);
    }
    
    this->mSounds.push_back(result);
    return result;
}

int SfxHandler::mLoadSoundEpisode(int prev, const std::string&name, EpisodeClass*episode){
    std::optional<PFile::Path> path = PFilesystem::FindEpisodeAsset(name, PFilesystem::SFX_DIR);
    if(path.has_value()){
        int res = PSound::load_sfx(*path);
        if(res!=-1){
            this->mSounds.push_back(res);
            return res;
        }
        else{
            std::ostringstream os;
            os<<"Unable to load SFX \""<<name<<"\" from ZIP episode";
            PLog::Write(PLog::ERR, "PK2", os.str().c_str());                
        }
    }
    return prev;
}

void SfxHandler::loadAll(){
    for(auto p: SfxHandler::soundFilenames){
        this->*p.second = this->mLoadSound(p.first);
    }

    for (int i = 0; i < PSound::CHANNELS; i++)
        sfx_list[i].used = false;
}

void SfxHandler::loadAllForEpisode(const SfxHandler& src, EpisodeClass*episode){
    for(auto p: SfxHandler::soundFilenames){
        this->*p.second = this->mLoadSoundEpisode(src.*p.second, p.first, episode);
    }
}

void SfxHandler::free(){
    for(int index:this->mSounds){
        PSound::free_sfx(index);
    }
    this->mSounds.clear();

    for(auto p: SfxHandler::soundFilenames){
        this->*p.second = -1;
    }
}

SfxHandler sfx_global;

int get_pan(int x, int y) {

    int pan = Game->camera_x - x + (screen_width / 2);
    return pan * 2;

}

int attenuate_volume(int volume, int x, int y) {

    const int max_dist = 50;

    float mult = 1;

    int sta_x = Game->camera_x;
    int sta_y = Game->camera_y;
    int end_x = Game->camera_x + screen_width;
    int end_y = Game->camera_y + screen_height;

    if (x < sta_x)
        mult /= float(sta_x - x + max_dist) / max_dist;

    if (x > end_x)
        mult /= float(x - end_x + max_dist) / max_dist;
    
    if (y < sta_y)
        mult /= float(sta_y - y + max_dist) / max_dist;
    
    if (y > end_y)
        mult /= float(y - end_y + max_dist) / max_dist;
    
    mult *= float(Settings.sfx_max_volume) / 100;

    if (mult < 0)
        mult = 0;
    if (mult > 1)
        mult = 1;

    return int(mult*mult*volume);

}

// Set panning and volume based on x, y and volume
void update_channel(int channel) {

    int x = sfx_list[channel].x;
    int y = sfx_list[channel].y;
    int volume = sfx_list[channel].volume;

    int pan = get_pan(x, y);
    int vol = attenuate_volume(volume, x, y);

    PSound::set_channel(channel, pan, vol);

}

void Update_GameSFX() {

    for (int i = 0; i < PSound::CHANNELS; i++)
        if (sfx_list[i].used) {
            if (!PSound::is_playing(i))
                sfx_list[i].used = false;
            else
                update_channel(i);
        }
    
}

void Play_GameSFX(int sound, int volume, int x, int y, int freq, bool random_freq){
	if (sound > -1 && Settings.sfx_max_volume > 0 && volume > 0) {

        if (random_freq)
			freq = freq + rand()%4000 - rand()%2000;

        int pan = get_pan(x, y);
        int vol = attenuate_volume(volume, x, y);

        int channel = PSound::play_sfx(sound, vol, pan, freq);
        if (channel == -1) {

            //PLog::Write(PLog::ERR, "PK2", "Can't play sound");
            return;

        }

        sfx_list[channel].used = true;
        sfx_list[channel].x = x;
        sfx_list[channel].y = y;
        sfx_list[channel].volume = volume;
			
	}
}

void Play_MenuSFX(int sound, int volume){
	if (sound > -1 && Settings.sfx_max_volume > 0 && volume > 0){
		volume = volume / (100 / Settings.sfx_max_volume);

		if (volume > 100)
			volume = 100;

		if (volume < 0)
			volume = 0;

		int freq = 22050 + rand()%5000 - rand()%5000;

		int channel = PSound::play_sfx(sound, Settings.sfx_max_volume, 0, freq);
        
        if (channel == -1) {
        
            PLog::Write(PLog::ERR, "PK2", "Can't play menu sound");
            return;
        
        }
        
        sfx_list[channel].used = false;

	}

}