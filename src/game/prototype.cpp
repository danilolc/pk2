#include "prototype.hpp"
#include "spriteclass_legacy.hpp"

#include "engine/PDraw.hpp"
#include "engine/PSound.hpp"
#include "engine/PLog.hpp"
#include "engine/PFilesystem.hpp"

#include "settings/config_txt.hpp"
#include "gfx/missing_texture.hpp"

#include "exceptions.hpp"
#include <sstream>

SpriteAnimation::SpriteAnimation(const LegacySprAnimation& anim){
	this->loop = anim.loop;
	this->sequence = std::vector<int>(anim.sequence, anim.sequence + anim.frames);
}

class UnsupportedSpriteVersionException: public std::exception{
public:
	UnsupportedSpriteVersionException(const std::string& version);
	const char* what() const noexcept{
        return message.c_str();
    }
private:
	std::string message;
};

UnsupportedSpriteVersionException::UnsupportedSpriteVersionException(const std::string& version){
	std::ostringstream os;
	os<<"Unsupported sprite version: "<<version;
	this->message = os.str();
}


void to_json(nlohmann::json& j, const SpriteAnimation& a){
	j["loop"] = a.loop;
	j["sequence"] = a.sequence;

	if(a.intro > 0){
		j["intro"] = a.intro;
	}
}

void from_json(const nlohmann::json&j, SpriteAnimation& a){
	a.loop = j["loop"].get<bool>();
	a.sequence = j["sequence"].get<std::vector<int>>();

	if(j.contains("intro")){
		a.intro = j["intro"].get<unsigned int>();
		if(a.intro >= a.sequence.size()){
			a.intro = 0;
		}
	}
}

/**
 * @brief 
 * To prevent segfault while reading malformed sprites in the legacy formats.
 */
inline std::string ReadLegacyString(char*arr, std::size_t n){
	arr[n-1] = '\0';
	return std::string(arr);
}

/* -------- SpriteClass Prototyyppi ------------------------------------------------------------------ */

PrototypeClass::PrototypeClass(){}

PrototypeClass::~PrototypeClass(){
	if(this->mAssetsLoaded){
		this->unloadAssets();
	}
}

void PrototypeClass::setProto10(PrototypeClass10 &proto){
	this->picture_filename = ReadLegacyString(proto.picture, 13);
	this->name = ReadLegacyString(proto.name, 30);

	this->transformation_str = ReadLegacyString(proto.transformation_str, 13);
	this->bonus_str = ReadLegacyString(proto.bonus_str, 13);
	this->ammo1_str = ReadLegacyString(proto.ammo1_str, 13);
	this->ammo2_str = ReadLegacyString(proto.ammo2_str, 13);

	for (int i=0;i<SPRITE_SOUNDS_NUMBER_LEGACY;i++) {
		this->sound_files[i] = ReadLegacyString(proto.sound_files[i], 13);
		this->sounds[i] = proto.sounds[i];
	}

	//animations_number		= proto.animations_number;
	can_open_locks				= proto.can_open_locks;
	energy				= proto.energy;
	is_wall				= proto.is_wall;
	frames_number				= proto.frames_number;
	frame_rate			= proto.frame_rate;
	picture_frame_x				= proto.picture_frame_x;
	picture_frame_y				= proto.picture_frame_y;
	picture_frame_width	= proto.picture_frame_width;
	picture_frame_height	= proto.picture_frame_height;
	height				= proto.height;
	charge_time			= proto.charge_time;
	width				= proto.width;
	max_jump			= proto.max_jump;
	max_speed			= proto.max_speed;
	weight				= proto.weight;
	score				= proto.score;
	immunity_type				= proto.immunity_type;
	
	destruction_effect		= proto.how_destroyed;
	if(destruction_effect==FX_DESTRUCT_NO_EFFECT){
		indestructible = true;
	}

	type				= proto.sprite_type;
	damage				= proto.damage;
	color				= proto.color;
	enemy			= proto.enemy;


	for (int i=0;i<5;i++){
		if(proto.AI[i]!=0){
			this->AI_v.push_back(proto.AI[i]);
		}
	}

	for(int i=0;i<SPRITE_ANIMATIONS_NUMBER;++i){
		this->animations[i] = proto.animaatiot[i];
	}
}
void PrototypeClass::setProto11(PrototypeClass11 &proto){
	this->picture_filename = ReadLegacyString(proto.picture, 13);
	this->name = ReadLegacyString(proto.name, 30);

	this->transformation_str = ReadLegacyString(proto.transformation_str, 13);
	this->bonus_str = ReadLegacyString(proto.bonus_str, 13);
	this->ammo1_str = ReadLegacyString(proto.ammo1_str, 13);
	this->ammo2_str = ReadLegacyString(proto.ammo2_str, 13);

	for (int i=0;i<SPRITE_SOUNDS_NUMBER_LEGACY;i++) {
		this->sound_files[i] = ReadLegacyString(proto.sound_files[i], 13);
		this->sounds[i] = proto.sounds[i];
	}

	//animations_number		= proto.animations_number;
	can_open_locks				= proto.can_open_locks;
	bonuses_number        = proto.bonuses_number;
	energy				= proto.energy;
	is_wall				= proto.is_wall;
	frames_number				= proto.frames_number;
	frame_rate			= proto.frame_rate;
	attack1_time		= proto.attack1_time;
	attack2_time		= proto.attack2_time;
	picture_frame_x				= proto.picture_frame_x;
	picture_frame_y				= proto.picture_frame_y;
	picture_frame_width	= proto.picture_frame_width;
	picture_frame_height	= proto.picture_frame_height;
	height				= proto.height;
	charge_time			= proto.charge_time;
	width				= proto.width;
	max_jump			= proto.max_jump;
	max_speed			= proto.max_speed;
	weight				= proto.weight;
	parallax_type		= proto.parallax_type;
	score				= proto.score;
	immunity_type				= proto.immunity_type;
	vibrates				= proto.vibrates;
	
	destruction_effect		= proto.how_destroyed;
	if(destruction_effect==FX_DESTRUCT_NO_EFFECT){
		indestructible = true;
	}

	type				= proto.sprite_type;
	damage				= proto.damage;
	damage_type		= proto.damage_type;
	color				= proto.color;
	enemy			= proto.enemy;

	for (int i=0;i<5;i++){
		if(proto.AI[i]!=0){
			this->AI_v.push_back(proto.AI[i]);
		}
	}

	/*for (int i=0;i<SPRITE_MAX_ANIMAATIOITA;i++) {
		for (int j=0;j<ANIMATION_SEQUENCE_SIZE;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];

		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frames_number = proto.animaatiot[i].frames_number;
	}*/
	for(int i=0;i<SPRITE_ANIMATIONS_NUMBER;++i){
		this->animations[i] = proto.animaatiot[i];
	}
}
void PrototypeClass::setProto12(PrototypeClass12 &proto){

	this->picture_filename = ReadLegacyString(proto.picture, 13);
	this->name = ReadLegacyString(proto.name, 30);

	this->transformation_str = ReadLegacyString(proto.transformation_str, 13);
	this->bonus_str = ReadLegacyString(proto.bonus_str, 13);
	this->ammo1_str = ReadLegacyString(proto.ammo1_str, 13);
	this->ammo2_str = ReadLegacyString(proto.ammo2_str, 13);

	for (int i=0;i<SPRITE_SOUNDS_NUMBER_LEGACY;i++) {
		this->sound_files[i] = ReadLegacyString(proto.sound_files[i], 13);
		this->sounds[i] = proto.sounds[i];
	}

	sound_frequency			= proto.sound_frequency;
	//animations_number		= proto.animations_number;
	can_open_locks				= proto.can_open_locks;
	bonuses_number        = proto.bonuses_number;
	energy				= proto.energy;
	is_wall				= proto.is_wall;
	is_wall_up			= proto.is_wall_up;
	is_wall_down			= proto.is_wall_down;
	is_wall_right		= proto.is_wall_right;
	is_wall_left		= proto.is_wall_left;
	frames_number				= proto.frames_number;
	frame_rate			= proto.frame_rate;
	attack1_time		= proto.attack1_time;
	attack2_time		= proto.attack2_time;
	picture_frame_x				= proto.picture_frame_x;
	picture_frame_y				= proto.picture_frame_y;
	picture_frame_width	= proto.picture_frame_width;
	picture_frame_height	= proto.picture_frame_height;
	height				= proto.height;
	charge_time			= proto.charge_time;
	width				= proto.width;
	max_jump			= proto.max_jump;
	max_speed			= proto.max_speed;
	weight				= proto.weight;
	parallax_type		= proto.parallax_type;
	score				= proto.score;
	random_sound_frequency			= proto.random_sound_frequency;
	immunity_type				= proto.immunity_type;
	vibrates				= proto.vibrates;
	check_tiles		= proto.check_tiles;
	
	destruction_effect		= proto.how_destroyed;
	if(destruction_effect==FX_DESTRUCT_NO_EFFECT){
		indestructible = true;
	}

	type				= proto.sprite_type;
	damage				= proto.damage;
	damage_type		= proto.damage_type;
	color				= proto.color;
	enemy			= proto.enemy;

	for (int i=0;i<5;i++){
		if(proto.AI[i]!=0){
			this->AI_v.push_back(proto.AI[i]);
		}
	}

	/*for (int i=0;i<SPRITE_MAX_ANIMAATIOITA;i++) {
		for (int j=0;j<ANIMATION_SEQUENCE_SIZE;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];

		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frames_number = proto.animaatiot[i].frames_number;
	}*/

	for(int i=0;i<SPRITE_ANIMATIONS_NUMBER;++i){
		this->animations[i] = proto.animaatiot[i];
	}
}



void PrototypeClass::setProto13(PrototypeClass13 &proto){
	this->picture_filename = ReadLegacyString(proto.picture, 100); //proto.picture;
	this->name = ReadLegacyString(proto.name, 30); //proto.name;

	this->transformation_str = ReadLegacyString(proto.transformation_str, 100); //proto.transformation_str;
	this->bonus_str = ReadLegacyString(proto.bonus_str, 100);  //proto.bonus_str;
	this->ammo1_str = ReadLegacyString(proto.ammo1_str, 100); //proto.ammo1_str;
	this->ammo2_str = ReadLegacyString(proto.ammo2_str, 100);  //proto.ammo2_str;

	for (int i=0;i<SPRITE_SOUNDS_NUMBER_LEGACY;i++) {
		this->sound_files[i] = ReadLegacyString(proto.sound_files[i], 100);
		this->sounds[i] = proto.sounds[i];
	}

	sound_frequency			= proto.sound_frequency;
	//animations_number		= proto.animations_number;
	can_open_locks				= proto.can_open_locks;
	bonuses_number        = proto.bonuses_number;
	energy				= proto.energy;
	is_wall				= proto.is_wall;
	is_wall_up			= proto.is_wall_up;
	is_wall_down			= proto.is_wall_down;
	is_wall_right		= proto.is_wall_right;
	is_wall_left		= proto.is_wall_left;
	frames_number				= proto.frames_number;
	frame_rate			= proto.frame_rate;
	attack1_time		= proto.attack1_time;
	attack2_time		= proto.attack2_time;
	picture_frame_x				= proto.picture_frame_x;
	picture_frame_y				= proto.picture_frame_y;
	picture_frame_width	= proto.picture_frame_width;
	picture_frame_height	= proto.picture_frame_height;
	height				= proto.height;
	charge_time			= proto.charge_time;
	width				= proto.width;
	max_jump			= proto.max_jump;
	max_speed			= proto.max_speed;
	weight				= proto.weight;
	parallax_type		= proto.parallax_type;
	score				= proto.score;
	random_sound_frequency			= proto.random_sound_frequency;
	immunity_type				= proto.immunity_type;
	vibrates				= proto.vibrates;
	check_tiles		= proto.check_tiles;
	
	destruction_effect		= proto.how_destroyed;
	if(destruction_effect==FX_DESTRUCT_NO_EFFECT){
		indestructible = true;
	}

	type				= proto.sprite_type;
	damage				= proto.damage;
	damage_type		= proto.damage_type;
	color				= proto.color;
	enemy			= proto.enemy;

	effect		= proto.effect;
	is_transparent				= proto.is_transparent;
	projectile_charge_time			= proto.projectile_charge_time;
	can_glide			= proto.can_glide;
	//boss				= proto.boss;
	bonus_always			= proto.bonus_always;
	can_swim			= proto.can_swim;

	for (int i=0;i<10;i++){
		if(proto.AI[i]!=0){
			this->AI_v.push_back(proto.AI[i]);
		}
	}

	/*for (int i=0;i<SPRITE_MAX_ANIMAATIOITA;i++) {
		for (int j=0;j<ANIMATION_SEQUENCE_SIZE;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];

		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frames_number = proto.animaatiot[i].frames_number;
	}*/

	for(int i=0;i<SPRITE_ANIMATIONS_NUMBER;++i){
		this->animations[i] = proto.animaatiot[i];
	}
}

const std::map<std::string,int> PrototypeClass::AnimationsDict = {
	{"idle", ANIMATION_IDLE},
    {"walking", ANIMATION_WALKING},
    {"jump_up", ANIMATION_JUMP_UP},
    {"jump_down", ANIMATION_JUMP_DOWN},
    {"squat", ANIMATION_SQUAT},
    {"damage", ANIMATION_DAMAGE},
    {"death", ANIMATION_DEATH},
    {"attack1", ANIMATION_ATTACK1},
    {"attack2", ANIMATION_ATTACK2}
};

const std::map<std::string,int> PrototypeClass::SoundTypesDict={
    {"damage", SOUND_DAMAGE},
    {"destruction", SOUND_DESTRUCTION},
    {"attack1", SOUND_ATTACK1},
    {"attack2", SOUND_ATTACK2},
    {"random", SOUND_RANDOM},
    {"special1", SOUND_SPECIAL1},
    {"special2", SOUND_SPECIAL2}
};

const std::map<std::string,u8> PrototypeClass::ColorsDict={
	{"gray", COLOR_GRAY},
	{"blue", COLOR_BLUE},
	{"red", COLOR_RED},
	{"green", COLOR_GREEN},
	{"orange", COLOR_ORANGE},
	{"violet", COLOR_VIOLET},
	{"turquoise", COLOR_TURQUOISE},
	{"normal", COLOR_NORMAL}
};

void PrototypeClass::setProto20(const nlohmann::json& j){
	using namespace PJson;

	if(j.contains("ai")){
		this->AI_v.clear();
		this->AI_v = j["ai"].get<std::vector<int>>();
	}

	jsonReadString(j, "ammo1", this->ammo1_str);


	jsonReadString(j, "ammo2", this->ammo2_str);

	if(j.contains("animations")){
		const nlohmann::json& j_animations = j["animations"]; 
		for(std::pair<std::string, int> p: AnimationsDict){
			if(j_animations.contains(p.first)){
				this->animations[p.second] = j_animations[p.first].get<SpriteAnimation>();
			}
		}
	}

	jsonReadBool(j, "always_active", this->always_active);

	jsonReadInt(j, "attack1_time", this->attack1_time);

	jsonReadInt(j, "attack2_time", this->attack2_time);

	jsonReadBool(j, "bonus_always", this->bonus_always);

	jsonReadString(j, "bonus", this->bonus_str);

	jsonReadInt(j, "bonuses_number", this->bonuses_number);

	//jsonReadBool(j, "boss", this->boss);

	jsonReadBool(j, "can_glide", this->can_glide);

	jsonReadBool(j, "can_open_locks", this->can_open_locks);

	jsonReadBool(j, "can_swim", this->can_swim);

	jsonReadInt(j, "charge_time", this->charge_time);

	jsonReadEnumU8(j, "color", this->color, ColorsDict);

	jsonReadInt(j, "damage", this->damage);

	jsonReadInt(j, "damage_type", this->damage_type);

	jsonReadEnumU8(j, "effect", this->effect);

	jsonReadBool(j, "enemy", this->enemy);

	jsonReadInt(j, "energy", this->energy);

	if(j.contains("frame")){
		const nlohmann::json& j_frame = j["frame"];
		this->picture_frame_x = j_frame["pos_x"].get<int>();
		this->picture_frame_y = j_frame["pos_y"].get<int>();
		this->picture_frame_width  = j_frame["width"].get<int>();
		this->picture_frame_height = j_frame["height"].get<int>();
	}

	jsonReadInt(j, "frame_rate", this->frame_rate);

	jsonReadInt(j, "frames_number", this->frames_number);

	/**
	 * @brief 
	 * Obsolete field
	 */
	if(j.contains("how_destroyed")){
		int how_destroyed = j["how_destroyed"].get<int>();

		this->destruction_effect = how_destroyed;
		if(this->destruction_effect == FX_DESTRUCT_NO_EFFECT){
			this->indestructible = true;
		}
	}

	jsonReadInt(j, "destruction_effect", this->destruction_effect);
	jsonReadBool(j, "indestructible", this->indestructible);

	jsonReadInt(j, "immunity_type", this->immunity_type);

	jsonReadBool(j, "is_transparent", this->is_transparent);

	jsonReadBool(j, "is_wall", this->is_wall);

	jsonReadBool(j, "is_wall_down", this->is_wall_down);

	jsonReadBool(j, "is_wall_left", this->is_wall_left);

	jsonReadBool(j, "is_wall_right", this->is_wall_right);

	jsonReadBool(j, "is_wall_up", this->is_wall_up);

	jsonReadBool(j, "check_tiles", this->check_tiles);

	jsonReadInt(j, "max_jump", this->max_jump);

	jsonReadDouble(j, "max_speed", this->max_speed);

	jsonReadString(j, "name", this->name);

	jsonReadInt(j, "parallax_type", this->parallax_type);

	jsonReadString(j, "picture", this->picture_filename);

	jsonReadInt(j, "projectile_charge_time", this->projectile_charge_time);

	jsonReadBool(j, "random_sound_frequency", this->random_sound_frequency);

	jsonReadInt(j, "score", this->score);

	if(j.contains("size")){
		const nlohmann::json& size = j["size"];
		this->width = size["width"].get<int>();
		this->height = size["height"].get<int>();
	}

	jsonReadInt(j, "sound_frequency", this->sound_frequency);

	if(j.contains("sounds")){
		const nlohmann::json& j_sounds = j["sounds"];
		for(std::pair<std::string, int> p: SoundTypesDict){
			this->sounds[p.second] = -1;
			jsonReadString(j_sounds, p.first, this->sound_files[p.second]);
		}
	}

	jsonReadString(j, "transformation", this->transformation_str);

	jsonReadInt(j, "type", this->type);

	jsonReadBool(j, "vibrates", this->vibrates);

	jsonReadDouble(j, "weight", this->weight);

	jsonReadInt(j, "info_id", this->info_id);

	if(j.contains("commands")){
		this->commands_json = j["commands"];
	}

	if(j.contains("dead_weight") && j["dead_weight"].is_number()){
		this->dead_weight = j["dead_weight"].get<double>();
	}

	if(j.contains("attack1_offset")){
		this->ammo1_offset = j["attack1_offset"].get<Point2D>();
	}

	if(j.contains("attack2_offset")){
		this->ammo2_offset = j["attack2_offset"].get<Point2D>();
	}

	if(j.contains("player_detection")){
		this->player_detection = j["player_detection"].get<Point2D>();
	}


	if(j.contains("can_push_bonuses")){
		this->can_push_bonuses = j["can_push_bonuses"].get<bool>();
	}

	/**
	 * Experimental
	 */
	jsonReadInt(j, "blend_mode", this->blend_mode);
	jsonReadInt(j, "blend_alpha", this->blend_alpha);
}

void to_json(nlohmann::json& j, const PrototypeClass& c){
	using json = nlohmann::json;

    j["version"] = PK2SPRITE_CURRENT_VERSION;

    j["type"] = c.type;
    j["picture"] = c.picture_filename;

    json sounds;
    for(std::pair<std::string, int> p: PrototypeClass::SoundTypesDict){
        sounds[p.first] = c.sound_files[p.second];        
    }

    j["sounds"] = sounds;
    j["frames_number"] = c.frames_number;
    j["frame_rate"] = c.frame_rate;

    json animations;
    for(std::pair<std::string, int> p :PrototypeClass::AnimationsDict){
        animations[p.first] = c.animations[p.second];
    }

    j["animations"] = animations;


    json frame;
    frame["pos_x"] = c.picture_frame_x;
    frame["pos_y"] = c.picture_frame_y;
    frame["width"] = c.picture_frame_width;
    frame["height"] = c.picture_frame_height;

    j["frame"] = frame;

    j["name"] = c.name;
    json size;
	size["width"]  = c.width;
	size["height"] = c.height;

	j["size"] = size;
    j["weight"] = c.weight;
    j["enemy"] = c.enemy;
    j["energy"] = c.energy;
    j["damage"] = c.damage;
    j["damage_type"] = c.damage_type;
    j["immunity_type"] = c.immunity_type;
    j["score"] = c.score;

    j["ai"] = c.AI_v;
    j["max_jump"] = c.max_jump;
    j["max_speed"] = c.max_speed;
    j["charge_time"] = c.charge_time;

	j["color"] = c.color;

	for(std::pair<std::string, u8> p:PrototypeClass::ColorsDict){
		if(p.second==c.color){
			j["color"] = p.first;
			break;
		}
	}

    j["is_wall"] = c.is_wall;

	j["destruction_effect"] = c.destruction_effect;
	j["indestructible"] = c.indestructible;

    j["can_open_locks"] = c.can_open_locks;
    j["vibrates"] = c.vibrates;
    j["bonuses_number"] = c.bonuses_number;
    j["attack1_time"] = c.attack1_time;
    j["attack2_time"] = c.attack2_time;
    j["parallax_type"] = c.parallax_type;
    j["transformation"] = c.transformation_str;
    j["bonus"] = c.bonus_str;
    j["ammo1"] = c.ammo1_str;
    j["ammo2"] = c.ammo2_str;
    j["check_tiles"] = c.check_tiles;
    j["sound_frequency"] = c.sound_frequency;
    j["random_sound_frequency"] = c.random_sound_frequency;
    j["is_wall_up"] = c.is_wall_up;
    j["is_wall_down"] = c.is_wall_down;
    j["is_wall_right"] = c.is_wall_right;
    j["is_wall_left"] = c.is_wall_left;

    j["effect"] = c.effect;
    j["is_transparent"] = c.is_transparent;

    j["projectile_charge_time"] = c.projectile_charge_time;

    j["can_glide"] = c.can_glide;
    //j["boss"] = c.boss;
    j["bonus_always"] = c.bonus_always;
    j["can_swim"] = c.can_swim;

	j["always_active"] = c.always_active;

	j["info_id"] = c.info_id;

	if(!c.commands_json.is_null()){
		j["commands"] = c.commands_json;
	}
	if(c.dead_weight.has_value()){
		j["dead_weight"] = *c.dead_weight;
	}

	if(c.ammo1_offset.has_value()){
		j["attack1_offset"] = *c.ammo1_offset;
	}

	if(c.ammo2_offset.has_value()){
		j["attack2_offset"] = *c.ammo2_offset;
	}

	j["player_detection"] = c.player_detection;

	if(c.blend_mode!=0){
		j["blend_mode"] = c.blend_mode;
		j["blend_alpha"] = c.blend_alpha;
	}
}

void PrototypeClass::loadPrototypeJSON(PFile::Path path,
	std::function<PrototypeClass*(const std::string&)> fn_loadPrototype){

	const nlohmann::json proto = path.GetJSON();
	if(!proto.contains("version") || !proto["version"].is_string()){
		throw PExcept::PException("Incorrect JSON, no string field \"version\"");
	}
	std::string version = proto["version"].get<std::string>();
	if(version=="2.0"){

		if(proto.contains("parent")&&proto["parent"].is_string()){

			if(fn_loadPrototype==nullptr){
				throw PExcept::PException("The parent field is not supported while loading prototypes this way!");
			}
			PrototypeClass* parentPrototype = fn_loadPrototype(proto["parent"]);
			
			if(parentPrototype!=nullptr){
				if(!parentPrototype->commands.empty()){
					throw PExcept::PException("The parent prototype cannot contain commands!");
				}

				*this = *parentPrototype;
			}
		}

		this->setProto20(proto);
	}
	else{
		throw UnsupportedSpriteVersionException(version);
	}
}

void PrototypeClass::loadPrototypeLegacy(PFile::Path path){

	try{
		PFile::RW file = path.GetRW2("r");
		char versio[4];
		file.read(versio, 4);

		if (strcmp(versio,"1.0") == 0){
			PrototypeClass10 proto;
			file.read(&proto, sizeof(proto));
			this->setProto10(proto);
		}
		else if (strcmp(versio,"1.1") == 0){
			PrototypeClass11 proto;
			file.read(&proto, sizeof(proto));
			this->setProto11(proto);
		}
		else if (strcmp(versio,"1.2") == 0){
			PrototypeClass12 proto;
			file.read(&proto, sizeof(proto));
			this->setProto12(proto);
		}
		else if (strcmp(versio,"1.3") == 0){
			PrototypeClass13 proto;
			file.read(&proto, sizeof(proto));
			this->setProto13(proto);
		}
		else {
			file.close();
			throw UnsupportedSpriteVersionException(versio);
		}
		this->version = versio;
		file.close();


	}
	catch(const std::exception& e){
		PLog::Write(PLog::ERR, "PK2 Sprites", e.what());
		throw PExcept::FileNotFoundException(path.c_str(), PExcept::MISSING_SPRITE_PROTOTYPE);
	}	
}


void PrototypeClass::loadAssets(){

	if(this->mAssetsLoaded){
		return;
		/*std::ostringstream os;
		os<<"Assets for sprite: \""<<this->filename<<"\" already loaded!";
		throw PExcept::PException(os.str());*/
	}

	this->mAssetsLoaded=true;

	std::optional<PFile::Path> imagePath = PFilesystem::FindAsset(this->picture_filename,
	PFilesystem::SPRITES_DIR);

	if(!imagePath.has_value()){
		if(config_txt.panic_when_missing_assets){
			throw PExcept::FileNotFoundException(this->picture_filename, PExcept::MISSING_SPRITE_TEXTURE);
		}
		else{
			PLog::Write(PLog::ERR, "PK2 Sprites", "Texture \"%s\" not found!", this->picture_filename.c_str());		
			this->frames_number = 1;
			this->picture_frame_x = 0;
			this->picture_frame_y = 0;
			this->frames.push_back(new_missing_texture_placeholder(173, 0, this->picture_frame_width, this->picture_frame_height));
			this->frames_mirror.push_back(new_missing_texture_placeholder(0, 173, this->picture_frame_width, this->picture_frame_height));
			this->AI_f.push_back(SpriteAI::AI_Table::INSTANCE.missing_texture_AI);
		}
	}
	else{
		if(this->frames_number>0){
			this->frames.resize(this->frames_number);
			this->frames_mirror.resize(this->frames_number);
		}
		else{
			throw std::runtime_error("The 0 value of frames_number is not allowed");
		}

		int texture = PDraw::image_load(*imagePath, false);
		if (texture == -1) {
			std::ostringstream os;
			os<<"Unable to load sprite texture: \""<<this->picture_filename<<"\"";
			throw PExcept::PException(os.str());
		}

		//Change sprite colors
		if (this->color != COLOR_NORMAL){ 

			int w, h;
			u8 color;
			PDraw::image_getsize(texture,w,h);

			u8 *buffer = NULL;
			u32 width;
			PDraw::drawimage_start(texture, buffer, width);

			for (int x = 0; x < w; x++)
				for (int y = 0; y < h; y++)
					if ((color = buffer[x+y*width]) != 255) {
						color %= 32;
						color += this->color;
						buffer[x+y*width] = color;
					}

			PDraw::drawimage_end(texture);
		}

		int frame_x = picture_frame_x;
		int frame_y = picture_frame_y;

		int texture_width = 0;
		int texture_height = 0;

		PDraw::image_getsize(texture, texture_width, texture_height);

		//Get each frame
		for (int frame_i = 0; frame_i < frames_number; frame_i++) {

			if (frame_x + picture_frame_width > texture_width) {
				frame_y += this->picture_frame_height + 3;
				frame_x = picture_frame_x;
			}

			this->frames[frame_i] = PDraw::image_cut(texture,frame_x,frame_y,picture_frame_width,picture_frame_height); //frames
			this->frames_mirror[frame_i] = PDraw::image_cut(texture,frame_x,frame_y,picture_frame_width,picture_frame_height); //flipped frames
			PDraw::image_fliphori(this->frames_mirror[frame_i]);

			frame_x += this->picture_frame_width + 3;
		
		}

		PDraw::image_delete(texture);
	}

	for (int i = 0; i < SPRITE_SOUNDS_NUMBER; i++) {

		if(!this->sound_files[i].empty()){

			std::optional<PFile::Path> soundPath = PFilesystem::FindAsset(this->sound_files[i],
			PFilesystem::SPRITES_DIR);

			if (soundPath.has_value()) {

				this->sounds[i] = PSound::load_sfx(*soundPath);

			} else {

				PLog::Write(PLog::ERR, "PK2 Sprites", "Can't find sound %s", this->sound_files[i].c_str());
			}
		}
	}

	SpriteAI::AI_Table::INSTANCE.InitSpritePrototypeAIs(this);

	if(this->damage_type==DAMAGE_SELF_DESTRUCTION){
		throw PExcept::PException("\"Self Destruction\" damage type is not allowed for the sprite damage type!");
	}

	/*SpriteAI::AI_Table::INSTANCE.InitSpriteAIs(this->AI_f, this->AI_v);
	SpriteAI::AI_Table::INSTANCE.InitSpriteProjectileAIs(this->AI_p, this->AI_v);*/

	if(!this->commands_json.is_null()){
		SpriteCommands::Parse_Commands(this->commands_json, this->commands, this->width, this->height);
	}
}

void PrototypeClass::unloadAssets(){
	for(int& frame_index: this->frames){
		if(frame_index>0){
			PDraw::image_delete(frame_index);
		}
	}

	for(int& frame_index: this->frames_mirror){
		if(frame_index>0){
			PDraw::image_delete(frame_index);
		}
	}

	for(const int& sound_index: this->sounds){
		if(sound_index!=-1){
			PSound::free_sfx(sound_index);
		}
	}
	for(SpriteCommands::Command* & command: this->commands){
		if(command!=nullptr){
			delete command;
			command = nullptr;
		}
	}

	this->mAssetsLoaded=false;
}

void PrototypeClass::initMissingPlaceholder(){
	this->mAssetsLoaded = true;

	this->type = TYPE_FOREGROUND;
	this->energy = 1;
	this->indestructible = true;

	this->weight = 0;

	this->frames_number = 1;
	this->picture_frame_x = 0;
	this->picture_frame_y = 0;
	this->picture_frame_width = 32;
	this->picture_frame_height = 32;

	this->width = 32;
	this->height = 32;	

	this->frames.push_back(new_missing_texture_placeholder(78, 0, this->picture_frame_width, this->picture_frame_height));
	this->frames_mirror.push_back(new_missing_texture_placeholder(0, 78, this->picture_frame_width, this->picture_frame_height));
	this->AI_f.push_back(SpriteAI::AI_Table::INSTANCE.missing_sprite_AI);
}


void PrototypeClass::draw(int x, int y, int frame)const{
	PDraw::image_clip(this->frames[frame], x, y);
}
bool PrototypeClass::hasAI(int ai)const{
	return std::find(this->AI_v.begin(), this->AI_v.end(), ai) != this->AI_v.end();
}