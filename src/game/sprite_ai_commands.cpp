//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
/**
 * @brief 
 * Experimental commands/waypoints AI by SaturninTheAlien
 */

#include "sprite_ai_commands.hpp"
#include "sprite_ai_table.hpp"

#include <sstream>

#include "engine/PLog.hpp"
#include "exceptions.hpp"
#include "sfx.hpp"
#include "spriteclass.hpp"
#include "gfx/effect.hpp"
#include "game/game.hpp"
#include "3rd_party/sol.hpp"

#include "lua/lua_sprite_commands.hpp"

namespace SpriteCommands{

class WaypointX:public Command{
public:
    WaypointX(double target_x):target_x(target_x){};
    ~WaypointX()=default;
    bool execute(SpriteClass*sprite)override;
private:
    double target_x;
};

bool WaypointX::execute(SpriteClass*sprite){
    return sprite->flyToWaypointX(this->target_x);
}


class WaypointRX:public Command{
public:
    WaypointRX(double target_x):target_x(target_x){};
    ~WaypointRX()=default;
    bool execute(SpriteClass*sprite)override;
private:
    double target_x;
};

bool WaypointRX::execute(SpriteClass*sprite){
    return sprite->flyToWaypointX(this->target_x + sprite->orig_x);
}

class WaypointY:public Command{
public:
    WaypointY(double target_y):target_y(target_y){};
    ~WaypointY()=default;
    bool execute(SpriteClass*sprite)override;
private:
    double target_y;
};

bool WaypointY::execute(SpriteClass*sprite){
    return sprite->flyToWaypointY(this->target_y);
}


class WaypointRY:public Command{
public:
    WaypointRY(double target_y):target_y(target_y){};
    ~WaypointRY()=default;
    bool execute(SpriteClass*sprite)override;
private:
    double target_y;
};

bool WaypointRY::execute(SpriteClass*sprite){
    return sprite->flyToWaypointY(this->target_y + sprite->orig_y);
}


class Waypoint: public Command{
public:
    Waypoint(double target_x, double target_y):target_x(target_x), target_y(target_y){};
    ~Waypoint()=default;
    bool execute(SpriteClass*sprite)override;
private:
    double target_x, target_y;
};

bool Waypoint::execute(SpriteClass*sprite){
    return sprite->flyToWaypointXY(this->target_x, this->target_y);
}


class WaypointR: public Command{
public:
    WaypointR(double target_x, double target_y):target_x(target_x), target_y(target_y){};
    ~WaypointR()=default;
    bool execute(SpriteClass*sprite)override;
private:
    double target_x, target_y;
};

bool WaypointR::execute(SpriteClass*sprite){
    return sprite->flyToWaypointXY(this->target_x + sprite->orig_x, this->target_y + sprite->orig_y);
}


class WaypointSeenPlayer: public Command{
public:
    bool execute(SpriteClass*sprite)override;
};

bool WaypointSeenPlayer::execute(SpriteClass*sprite){
    bool success = true;
    if(AI_Functions::player!=nullptr){
        if(sprite->seen_player_x==-1){
            sprite->seen_player_x = AI_Functions::player->x;
            sprite->seen_player_y = AI_Functions::player->y;
        }

        success = sprite->flyToWaypointXY(sprite->seen_player_x, sprite->seen_player_y);
    }

    if(success){
        sprite->seen_player_x = -1;
        sprite->seen_player_y = -1;
    }

    return success;
}


class WaypointOrigXY: public Command{
public:
    bool execute(SpriteClass*sprite)override;
};


bool WaypointOrigXY::execute(SpriteClass*sprite){
    return sprite->flyToWaypointXY(sprite->orig_x, sprite->orig_y);
}


class TransformationCommand: public Command{
public:
    bool execute(SpriteClass*sprite)override;
};


bool TransformationCommand::execute(SpriteClass*sprite){
    return !sprite->transform();
};

class SelfDestructionCommand: public Command{
public:
    bool execute(SpriteClass*sprite)override;
};

bool SelfDestructionCommand::execute(SpriteClass*sprite){
    sprite->damage_taken = sprite->energy;
	sprite->damage_taken_type = DAMAGE_SELF_DESTRUCTION;
    sprite->self_destruction = true;
    return false;
}


class MakeSoundCommand: public Command{
public:
    MakeSoundCommand(int sound_index):sound_index(sound_index){}
    bool execute(SpriteClass*sprite)override;
private:
    int sound_index = -1;
};

bool MakeSoundCommand::execute(SpriteClass*sprite){
    Play_GameSFX(sprite->prototype->sounds[this->sound_index],100, (int)sprite->x, (int)sprite->y,
    sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);
    return true;
}


class WaitCommand: public Command{
public:
    WaitCommand(int wait_time): wait_time(wait_time){};
    bool execute(SpriteClass*sprite)override;
private:
    int wait_time = 0;
};

bool WaitCommand::execute(SpriteClass*sprite){
    if(sprite->command_timer==-1){
        sprite->command_timer = this->wait_time;
    }

    if(sprite->command_timer==0){
        sprite->command_timer=-1;
        return true;
    }
    else{
        --sprite->command_timer;
        return false;
    }
}

class WaitRandomCommand: public Command{
public:
    WaitRandomCommand(int timer_min, int timer_max):
        timer_min(timer_min), timer_max(timer_max){

        }
    bool execute(SpriteClass*sprite)override;
private:
    int timer_min;
    int timer_max;
};

bool WaitRandomCommand::execute(SpriteClass*sprite){
    if(sprite->command_timer==-1){
        int t = this->timer_max+1 - this->timer_min;
        sprite->command_timer = rand()%t + this->timer_min;
    }

    if(sprite->command_timer==0){
        sprite->command_timer=-1;
        return true;
    }
    else{
        --sprite->command_timer;
        return false;
    }    
}

double getCommandXPos(const nlohmann::json& j, int prototypeWidth){
    return j.get<double>() * 32 + ((double)prototypeWidth)/2;
}

double getCommandYPos(const nlohmann::json& j, int prototypeHeight){
    return j.get<double>() * 32 + ((double)prototypeHeight)/2;
}

class ThunderCommand:public Command{
public:
    ThunderCommand()=default;
    bool execute(SpriteClass*sprite)override;
};


bool ThunderCommand::execute(SpriteClass*sprite){
    sprite->StartThunder();
    return true;
};


class LuaCommand: public Command{
public:
    LuaCommand(const std::string & funcName);
    bool execute(SpriteClass*sprite)override;

private:
    sol::protected_function l_function;
};

LuaCommand::LuaCommand(const std::string & funcName){
    if(Game->lua == nullptr){
        throw std::runtime_error("Lua is disabled in this episode, cannot execute \"lua\" command");
    }

    this->l_function = PK2lua::GetCommandByName(funcName);
}

bool LuaCommand::execute(SpriteClass*sprite){
    sol::protected_function_result res = this->l_function(sprite);
    if(res.valid()){
        return res;
    }
    else{
        throw res.get<sol::error>(); // sol::error(res.status);
    }
}
/**
 * @brief 
 *
 */
class ChasePlayerCommand: public Command{
public:
    ChasePlayerCommand(int timer):mTimer(timer){}
    bool execute(SpriteClass*sprite)override;
private:
    int mTimer;
};

bool ChasePlayerCommand::execute(SpriteClass*sprite){

    if(AI_Functions::player!=nullptr){

        sprite->flyToWaypointXY(AI_Functions::player->x, AI_Functions::player->y);

        if(sprite->command_timer==-1){
            sprite->command_timer = this->mTimer;
        }

        if(sprite->command_timer==0){
            sprite->command_timer=-1;
            return true;
        }
        else{
            --sprite->command_timer;
            return false;
        }

    }
    else{
        sprite->command_timer = 0;
        return true;
    }
}


class EffectCommand: public Command{
public:
    EffectCommand(int effect):mEffect(effect){}
    bool execute(SpriteClass*sprite)override;
private:
    int mEffect;
};


bool EffectCommand::execute(SpriteClass*sprite){
    Effect_By_ID(this->mEffect, sprite->x, sprite->y);
    return true;
}


class WeatherCommand: public Command{
public:
    WeatherCommand(int weatherId):weatherId(weatherId){}
    bool execute(SpriteClass*sprite)override;
private:
    int weatherId=0;
};


bool WeatherCommand::execute(SpriteClass*sprite){

    LevelSector*sector = sprite->level_sector;
    if(sector->weather != this->weatherId){
        sector->weather = this->weatherId;
        BG_Particles::Init(this->weatherId, sector->rain_color);
    }

    return true;
}

void Parse_Commands(const nlohmann::json& j_in, std::vector<Command*>& commands_v, int prototypeWidth, int prototypeHeight){
    if(!j_in.is_array()){
        throw PExcept::PException("Commands field has to be an array!");
    }

    int state = 0;
    int timer1 = 0;
    double target_x = 0., target_y = 0.;
    for(const nlohmann::json& j: j_in){
        switch (state)
        {
        case 0:
            if(j.is_string()){
                std::string command_name = j.get<std::string>();
                if(command_name=="waypoint_x"){
                    state = 1;
                }
                else if(command_name=="waypoint_y"){
                    state = 2;
                }
                else if(command_name=="waypoint_xy"){
                    state = 3;
                }
                else if(command_name=="waypoint_orig_xy"){
                    commands_v.push_back(new WaypointOrigXY());
                }
                else if(command_name=="waypoint_seen_player"){
                    commands_v.push_back(new WaypointSeenPlayer());
                }
                else if(command_name=="make_sound"){
                    state = 5;
                }
                else if(command_name=="die" || command_name=="self_destruction"){
                    commands_v.push_back(new SelfDestructionCommand());
                }
                else if(command_name=="transform"){
                    commands_v.push_back(new TransformationCommand());
                }
                else if(command_name=="wait"){
                    state = 6;
                }
                else if(command_name=="wait_random"){
                    state = 7;
                }
                else if(command_name=="thunder"){
                    commands_v.push_back(new ThunderCommand());
                }

                else if(command_name=="waypoint_rx"){
                    state = 9;
                }
                else if(command_name=="waypoint_ry"){
                    state = 10;
                }
                else if(command_name=="waypoint_rxy"){
                    state = 11;
                }
                else if(command_name=="lua"){
                    state = 13;
                }
                else if(command_name=="chase_player"){
                    state = 14;
                }
                else if(command_name=="effect"){
                    state = 15;
                }
                else if(command_name=="weather"){
                    state = 16;
                }
            }
            break;
        case 1:
            if(j.is_number()){
                target_x = getCommandXPos(j, prototypeWidth);
                commands_v.push_back(new WaypointX(target_x));
            }
            state = 0;
            break;
        case 2:
            if(j.is_number()){
                target_y = getCommandYPos(j, prototypeHeight);
                commands_v.push_back(new WaypointY(target_y));
            }
            state = 0;
            break;
        
        case 3:
            if(j.is_number()){
                target_x = getCommandXPos(j, prototypeWidth);
                state = 4;
            }
            else{
                state = 0;
            }
            break;
        case 4:
            if(j.is_number()){
                target_y = getCommandYPos(j, prototypeHeight);
                commands_v.push_back(new Waypoint(target_x, target_y));
            }
            state=0;
            break;

        case 5:
            if(j.is_string()){
                auto it = PrototypeClass::SoundTypesDict.find(j.get<std::string>());
                if(it!=PrototypeClass::SoundTypesDict.end()){
                    if(it->second>=SPRITE_SOUNDS_NUMBER){
                        std::ostringstream os;
                        os<<"Sprite sound index: "<<it->second<<" out of array";
                        throw PExcept::PException(os.str());
                    }
                    commands_v.push_back(new MakeSoundCommand(it->second));
                }
            }

            state=0;
            break;
        case 6:
            if(j.is_number_integer()){
                int wait_time = j.get<int>();
                commands_v.push_back(new WaitCommand(wait_time));
            }
            state=0;
            break;

        case 7:
            if(j.is_number_integer()){
                timer1 = j.get<int>();
                if(timer1<0){
                    throw PExcept::PException("\"wait_random\", negative waiting time is not allowed!");
                }
                state = 8;
            }
            else{
                state = 0;
            }
            break;
        case 8:
            if(j.is_number_integer()){
                int timer2 = j.get<int>();
                if(timer2<timer1){
                    throw PExcept::PException("\"wait_random\", timer2 has to be greater than timer1!");
                }
                commands_v.push_back(new WaitRandomCommand(timer1,timer2));
            }
            state = 0;
            break;

        case 9:
            if(j.is_number()){
                target_x = j.get<double>() * 32;
                commands_v.push_back(new WaypointRX(target_x));
            }
            state=0;
            break;
        
        case 10:
            if(j.is_number()){
                target_y = j.get<double>() * 32;
                commands_v.push_back(new WaypointRY(target_y));
            }
            state=0;
            break;
        
        case 11:
            if(j.is_number()){
                target_x = j.get<double>() * 32;
                state = 12;   
            }
            else{
                state = 0;
            }
            break;
        case 12:
            if(j.is_number()){
                target_y = j.get<double>() * 32;
                commands_v.push_back(new WaypointR(target_x, target_y));
            }
            state = 0;
            break;

        case 13:
            commands_v.push_back(new LuaCommand(j.get<std::string>()));
            state = 0;
            break;

        case 14:
            if(j.is_number_integer()){
                int chasing_time = j.get<int>();
                commands_v.push_back(new ChasePlayerCommand(chasing_time));
            }

            state = 0;
            break;

        case 15:
            if(j.is_number_integer()){
                int effect_id = j.get<int>();
                commands_v.push_back(new EffectCommand(effect_id));
            }

            state=0;
            break;

        case 16:{

            int weather_id = 0;

            if(j.is_number_integer()){
                weather_id = j.get<int>();
                commands_v.push_back(new WeatherCommand(weather_id));
            }
            else if(j.is_string()){
                std::string weather = j.get<std::string>();
                if(weather=="normal"){
                    weather_id = WEATHER_NORMAL;
                }
                else if(weather=="rain"){
                    weather_id = WEATHER_RAIN;
                }
                else if(weather=="leaves"){
                    weather_id = WEATHER_LEAVES;
                }
                else if(weather=="rain_and_leaves"){
                    weather_id = WEATHER_RAIN_LEAVES;
                }
                else if(weather=="snow"){
                    weather_id = WEATHER_SNOW;
                }
                else if(weather=="dandelions"){
                    weather_id = WEATHER_DANDELIONS;
                }

                else{
                    std::ostringstream os;
                    os<<"Unknown game weather: "<<weather;
                    throw std::runtime_error(os.str());
                }

                commands_v.push_back(new WeatherCommand(weather_id));
            }

            state=0;

        }

            
            break;

        default:
            break;
        }
    }
}

}