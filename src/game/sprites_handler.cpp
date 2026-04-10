#include "sprites_handler.hpp"
#include "spriteclass.hpp"
#include "prototypes_handler.hpp"

#include "game.hpp"

#include "engine/PLog.hpp"
#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"

#include "physics.hpp"
#include "episode/episodeclass.hpp"
#include "settings/settings.hpp"

#include <limits.h>
#include <sstream>
#include "system.hpp"
#include <optional>

void SpritesHandler::clearAll(){
    for (SpriteClass* sprite : Sprites_List) {
		if(sprite!=nullptr){
			delete sprite;
		}
	}

	Sprites_List.clear();
	bgSprites_List.clear();
	fgSprites_List.clear();
}

static int Get_BG_Parallax_Score(SpriteClass* s){

	PrototypeClass*prototype = s->prototype;
	for(const int& ai:prototype->AI_v){
		if(ai==AI_BACKGROUND_BRING_TO_FRONT){
			return INT_MAX;
		}
		else if(ai==AI_BACKGROUND_SEND_TO_BACK){
			return INT_MIN;
		}
	}

	int parallax = prototype->parallax_type;

	return parallax==0 ? INT_MAX - 1 : parallax;
}

static bool Compare_bgSprites(SpriteClass* s1, SpriteClass* s2) {
	return Get_BG_Parallax_Score(s1) < Get_BG_Parallax_Score(s2);
}


void SpritesHandler::sortBg(){
    this->bgSprites_List.sort(Compare_bgSprites);
	this->fgSprites_List.sort(Compare_bgSprites);
}


void SpritesHandler::onSkullBlocksChanged(){
    for(SpriteClass* sprite: Sprites_List){
		if(sprite==nullptr||sprite->energy<=0)continue;

		for(const SpriteAI::AI_Class& ai: sprite->prototype->AI_f){
			if(ai.trigger==AI_TRIGGER_SKULLS_CHANGED){
				ai.func(sprite);
			}
		}
	}
}

void SpritesHandler::onEvent1(){
	for(SpriteClass* sprite: Sprites_List){
		if(sprite==nullptr||sprite->energy<=0)continue;

		for(const SpriteAI::AI_Class& ai: sprite->prototype->AI_f){
			if(ai.trigger==AI_TRIGGER_EVENT1){
				ai.func(sprite);
			}
		}
	}
}

void SpritesHandler::onEvent2(){
	for(SpriteClass* sprite: Sprites_List){
		if(sprite==nullptr||sprite->energy<=0)continue;

		for(const SpriteAI::AI_Class& ai: sprite->prototype->AI_f){
			if(ai.trigger==AI_TRIGGER_EVENT2){
				ai.func(sprite);
			}
		}
	}
}

void SpritesHandler::onOpeningKeylocks(){
	for(SpriteClass* sprite: Sprites_List){
		if(sprite==nullptr||sprite->energy<=0)continue;

		for(const SpriteAI::AI_Class& ai: sprite->prototype->AI_f){
			if(ai.trigger==AI_TRIGGER_KEYLOCKS_OPENED){
				ai.func(sprite);
			}
		}
	}
}

int SpritesHandler::onTickUpdate(int camera_x, int camera_y){
	const int ACTIVE_BORDER_X = 320;
	const int ACTIVE_BORDER_y = 240;

	int active_sprites = 0;

	//Activate sprite if it is next to the screen
	for (SpriteClass* sprite : Sprites_List) {
		if(sprite->prototype->always_active){
			sprite->active=true;
		}
		else if (sprite->x < camera_x + 640 + ACTIVE_BORDER_X &&
			sprite->x > camera_x - ACTIVE_BORDER_X &&
			sprite->y < camera_y + 480 + ACTIVE_BORDER_y &&
			sprite->y > camera_y - ACTIVE_BORDER_y){
				sprite->active = true;
			}
			
		else if(sprite->active){
			sprite->active = false;
			SpriteOffscreen(sprite);
		}	
	}

	for (SpriteClass* sprite : Sprites_List) {
		if(sprite->active){
			if(sprite->respawn_timer>0){
				--sprite->respawn_timer;

				if(sprite->respawn_timer==0){
					SpriteOnRespawn(sprite);
				}
			}

			// Update bonus first to get energy change
			if (sprite->active && !sprite->removed) {
				if (sprite->prototype->type == TYPE_BONUS && sprite->respawn_timer==0) {
					UpdateBonusSprite(sprite);
					active_sprites++;
				}
			}
		}
	}

	for (SpriteClass* sprite : Sprites_List) {
		if (sprite->active && !sprite->removed) {

			switch (sprite->prototype->type)
			{
			case TYPE_NOTHING:
			case TYPE_GAME_CHARACTER:
			case TYPE_PROJECTILE:
			case TYPE_TELEPORT:
			case TYPE_CHECKPOINT:
				
				if(sprite->respawn_timer==0 || sprite->prototype->destruction_effect>=100){
					UpdateSprite(sprite);
					active_sprites++;
				}
				break;
			
			default:
				break;
			}

			/*if (sprite->prototype->type <= TYPE_TELEPORT && sprite->prototype->type != TYPE_BONUS) {
			}*/
		}
	}

	// Clean destructed sprites
	fgSprites_List.remove_if([](SpriteClass*s){return s->removed || s->prototype->type!=TYPE_FOREGROUND;});
	bgSprites_List.remove_if([](SpriteClass*s){return s->removed || s->prototype->type!=TYPE_BACKGROUND;});
	Sprites_List.remove_if([&](SpriteClass*s){return this->spriteDestructed(s);});


	return active_sprites;
}

SpriteClass* SpritesHandler::mCreateSprite(PrototypeClass* prototype, int player_c, double x, double y, SpriteClass*parent_sprite){
	if(prototype==nullptr){
		throw std::runtime_error("Cannot create a sprite from a null prototype");
	}
	
	SpriteClass* sprite = new SpriteClass(prototype, player_c, x, y, this->mLevelSector, parent_sprite);
	this->Sprites_List.push_back(sprite);

	sprite->updatePhysicsFlags();

	for(const SpriteAI::AI_Class& ai: sprite->prototype->AI_f){
		if(ai.trigger==AI_TRIGGER_SPAWN || ai.trigger==AI_TRIGGER_TRANSFORMATION){
			ai.func(sprite);
		}
	}

	if (prototype->type == TYPE_BACKGROUND){
		this->mAddBG(sprite);
	}
	else if(prototype->type == TYPE_FOREGROUND){
		this->mAddFG(sprite);
	}

	return sprite;
}

SpriteClass* SpritesHandler::addPlayer(PrototypeClass*prototype, double x, double y){
	SpriteClass* sprite = this->mCreateSprite(prototype, 1, x, y, nullptr);
	sprite->initial_update = true;
	sprite->original = true;

	AI_Functions::player_invisible = sprite;
	if(sprite->invisible_timer==0){
		AI_Functions::player = sprite;
	}
	else{
		AI_Functions::player = nullptr;
	}

	sprite->orig_x = sprite->x;
	sprite->orig_y = sprite->y;

	return sprite;
}

void SpritesHandler::addLevelSprite(PrototypeClass*prototype, double x, double y){
	SpriteClass* sprite  = this->mCreateSprite(prototype, 0, x, y, nullptr);
	sprite->initial_update = true;
	sprite->original = true;

	/**
	 * @brief 
	 * Legacy feature
	 * Why does it exist ?
	 */
	sprite->x = x + 16 + 1;
	sprite->y += sprite->prototype->height/2;

	sprite->orig_x = sprite->x;
	sprite->orig_y = sprite->y;
}

void SpritesHandler::addDroppedBonusSprite(PrototypeClass*prototype, double x, double y){
	SpriteClass* sprite  = this->mCreateSprite(prototype, 0, x, y, nullptr);

	if(sprite->weight>0)sprite->jump_timer = 1;
	
	sprite->damage_timer = 35;//25

	sprite->a = 3 - rand()%7;
}

void SpritesHandler::addGiftSprite(PrototypeClass* prototype, SpriteClass* player){
	/**
	 * @brief 
	 * Fix spyrooster green bee bug.
	 */
	SpriteClass* parent = nullptr;
	if(player!=nullptr && player->enemy){
		parent = player;
	}
	
	SpriteClass* sprite  = this->mCreateSprite(prototype,
		0,
		player->x,
		player->y, parent);

	sprite->y += sprite->prototype->height/2;
}

void SpritesHandler::addProjectileSprite(PrototypeClass* prototype, SpriteClass* shooter, const std::optional<Point2D>& offset){
	if(shooter==nullptr){
		PLog::Write(PLog::WARN, "PK2", "Trying to add ammo sprite with null shooter!");
		return;
	}


	double x = shooter->x;
	double y = shooter->y;

	if(offset.has_value()){

		double off_x = -shooter->prototype->width/2 + offset->x;
		double off_y = -shooter->prototype->height/2 + offset->y;

		if(shooter->flip_x){
			off_x = -off_x;
		}
		if(shooter->flip_y){
			off_y = -off_y;
		}

		x += off_x;
		y += off_y;
	}

	SpriteClass* sprite  = this->mCreateSprite(prototype, 0, x, y, shooter);

	/**
	 * @brief 
	 * To fix the beehive bug
	 */
	if(prototype->type==TYPE_GAME_CHARACTER && shooter->HasAI(AI_BONUS)){
		sprite->damage_timer = 2;
	}

	sprite->parent_sprite = shooter;
	sprite->enemy = shooter->enemy;

	if(prototype->AI_p.empty()){ // No other projectile AIs, default behaviour
		if (!shooter->flip_x)
			sprite->a = sprite->prototype->max_speed;
		else
			sprite->a = -sprite->prototype->max_speed;

		/**
		 * @brief 
		 * To fix spikeballs from "The Revenant - Pekka’s Prison" 
		 */
		if(shooter->prototype->is_wall && shooter->prototype->is_wall_up
		&& !shooter->self_destruction && prototype->check_tiles){
			sprite->y -= shooter->prototype->height/2;
			sprite->y -= prototype->height/2;

			sprite->a /= 3.5;
		}
	}
	else{
		for(const SpriteAI::ProjectileAIClass& ai: prototype->AI_p){
			ai.func(sprite, shooter);
		}
	}

}

bool SpritesHandler::spriteDestructed(SpriteClass* sprite) { 

	if (sprite->isPlayer()) // Never remove the player
		return false;
	
	if (sprite->removed) {
		delete sprite;
		return true;
	}

	return false;
}


static bool isSpriteVisible(const SpriteClass* sprite, int camera_x, int camera_y){
	if(sprite->respawn_timer>0 && sprite->prototype->destruction_effect < 100) return false;

	return (sprite->x - sprite->prototype->picture_frame_width/2  < camera_x + screen_width &&
			sprite->x + sprite->prototype->picture_frame_width/2  > camera_x &&
			sprite->y - sprite->prototype->picture_frame_height/2 < camera_y + screen_height &&
			sprite->y + sprite->prototype->picture_frame_height/2 > camera_y);
}

void SpritesHandler::drawBGsprites(int camera_x, int camera_y, bool gamePaused, int& debug_drawn_sprites){
	for (SpriteClass* sprite : this->bgSprites_List) {

		double orig_x = sprite->orig_x;
		double orig_y = sprite->orig_y;

		double xl, yl;

		if (sprite->prototype->parallax_type != 0) {

			double parallax = double(sprite->prototype->parallax_type);

			xl =  orig_x - camera_x-screen_width/2 - sprite->prototype->width/2;
			xl /= parallax;
			yl =  orig_y - camera_y-screen_height/2 - sprite->prototype->height/2;
			yl /= parallax;
		}
		else {

			xl = yl = 0;

		}

		UpdateBackgroundSprite(sprite, yl);

		sprite->x = orig_x-xl;
		sprite->y = orig_y-yl;

		if (isSpriteVisible(sprite, camera_x, camera_y)) {
			sprite->draw(camera_x,camera_y);

			if (!gamePaused)
				sprite->HandleEffects();

			sprite->hidden = false;
			debug_drawn_sprites++;
		} else {
			if (!gamePaused)
				sprite->Animoi();
			sprite->hidden = true;
		}
	}
}

void SpritesHandler::drawFGsprites(int camera_x, int camera_y, bool gamePaused, int& debug_drawn_sprites){
	for(SpriteClass* sprite : this->fgSprites_List){

		double orig_x = sprite->orig_x;
		double orig_y = sprite->orig_y;

		double xl, yl;

		if (sprite->prototype->parallax_type != 0) {

			double parallax = double(-sprite->prototype->parallax_type);

			xl =  orig_x - camera_x-screen_width/2 - sprite->prototype->width/2;
			xl /= parallax;
			
			yl =  orig_y - camera_y-screen_height/2 - sprite->prototype->height/2;
			yl /= parallax;
		}
		else {

			xl = yl = 0;

		}

		UpdateBackgroundSprite(sprite, yl);

		sprite->x = orig_x-xl;
		sprite->y = orig_y-yl;


		if (isSpriteVisible(sprite, camera_x, camera_y)) {
			sprite->draw(camera_x,camera_y);

			if (!gamePaused)
				sprite->HandleEffects();

			sprite->hidden = false;
			debug_drawn_sprites++;
		} else {
			if (!gamePaused)
				sprite->Animoi();
			sprite->hidden = true;
		}
	}
}

void SpritesHandler::drawSprites(int camera_x, int camera_y, bool gamePaused, int& debug_drawn_sprites){
	for (SpriteClass* sprite : this->Sprites_List) {
		if (sprite->prototype->type == TYPE_BACKGROUND || sprite->prototype->type == TYPE_FOREGROUND)
			continue;
		
		if (sprite->removed)
			continue;

		if (isSpriteVisible(sprite, camera_x, camera_y)) {

			// Draw impact circle
			if (sprite->damage_timer > 0 && sprite->prototype->type != TYPE_BONUS && sprite->energy < 1){
				int framex = ((degree%12)/3) * 58;
				u32 hit_x = sprite->x-8;
				u32 hit_y = sprite->y-8;
				PDraw::image_cutclip(Game->gfxTexture,hit_x-camera_x-28+8, hit_y-camera_y-27+8,1+framex,83,1+57+framex,83+55);
			}

			bool blinking = dev_mode && sprite->isPlayer() && Settings.getInput().dev_ghostMode.isPressed();
			if(!blinking || degree % 2 == 0){
				sprite->draw(camera_x, camera_y);
			}

			// Draw stars on dead sprite
			if (sprite->energy < 1 && sprite->prototype->type != TYPE_PROJECTILE){
				int sx = (int)sprite->x;
				for (int stars=0; stars<3; stars++){
					double star_x = sprite->x - 8  + sin_table((stars*120+degree)*2)      / 3;
					double star_y = sprite->y - 18 + cos_table((stars*120+degree)*2 + sx) / 8;
					PDraw::image_cutclip(Game->gfxTexture,star_x-camera_x, star_y-camera_y,1,1,11,11);
				}
			}

			if (!gamePaused)
				sprite->HandleEffects();

			debug_drawn_sprites++;

		} else {

			if (!gamePaused)
				sprite->Animoi();

			// Delete death body
			if (sprite->energy < 1 && sprite->respawn_timer==0)
				sprite->removed = true;
			
		}
	}
}

SpriteClass* SpritesHandler::getSpriteById(std::size_t id){
	for(SpriteClass* sprite: this->Sprites_List){
		if(sprite->id==id){
			return sprite;
		}
	}

	return nullptr;
}


SpriteClass* SpritesHandler::findNearestTarget(const SpriteClass* agent)const{
	if(agent==nullptr) return nullptr;
		
	SpriteClass * result = nullptr;
	double result_d2 = std::numeric_limits<double>::infinity();

	for(SpriteClass* sprite: this->Sprites_List){

		if(sprite->active &&
		sprite!=agent &&
		sprite->energy > 0 &&
		!sprite->prototype->ambient &&
		(sprite->enemy != agent->enemy || sprite->prototype->hostile_to_everyone || agent->prototype->hostile_to_everyone) &&
		agent->canDamageOnCollision(sprite)){

			double dx = sprite->x - agent->x;
			double dy = sprite->y - agent->y;

			double d2 = dx*dx + dy*dy;
			
			if(d2 < result_d2){
				result = sprite;
				result_d2 = d2;
			}			
		}
	}
	
	return result;
}


nlohmann::json SpritesHandler::toJson()const{
	std::vector<nlohmann::json> vec;
	for(SpriteClass *sprite: this->Sprites_List){
		if(sprite->removed)continue;

		nlohmann::json j;
		this->spriteToJson(j, *sprite);
		vec.emplace_back(j);
	}

	return nlohmann::json(vec);
}

void SpritesHandler::fromJSON(const nlohmann::json& j, PrototypesHandler& handler, LevelSector*sector){
	this->clearAll();
	

	for(const nlohmann::json& j2: j){
		SpriteClass* sprite = new SpriteClass();
		this->jsonToSprite(j2, *sprite, handler);
		sprite->level_sector = sector;
		sprite->removed = false;
		this->Sprites_List.push_back(sprite);
	}

	for(SpriteClass* sprite: this->Sprites_List){
		if(sprite->parent_sprite_id.has_value()){
			sprite->parent_sprite = this->getSpriteById(*sprite->parent_sprite_id);
		}

		if(sprite->target_sprite_id.has_value()){
			sprite->target_sprite = this->getSpriteById(*sprite->target_sprite_id);
		}

		switch (sprite->prototype->type)
		{
		case TYPE_BACKGROUND:
			this->bgSprites_List.push_back(sprite);
			break;

		case TYPE_FOREGROUND:
			this->fgSprites_List.push_back(sprite);
		
		default:
			break;
		}
	}

	this->sortBg();
}

SpriteClass* SpritesHandler::findPlayer(){
	for(SpriteClass* sprite: this->Sprites_List){
		if(sprite->isPlayer()){
			return sprite;
		}
	}
	return nullptr;
}

void SpritesHandler::spriteToJson(nlohmann::json&j, const SpriteClass&s)const{
	j["id"] = s.id;
	j["active"] = s.active;
	j["prototype"] = s.prototype->filename;

	/**
	 * the 'removed' field intentionally skipped
	 */

	j["orig_x"] = s.orig_x;
	j["orig_y"] = s.orig_y;
	j["x"] = s.x;
	j["y"] = s.y;
	j["a"] = s.a;
	j["b"] = s.b;

	//level sector ?
	j["flip_x"] = s.flip_x;
	j["flip_y"] = s.flip_y;
	j["jump_timer"] = s.jump_timer;

	j["can_move_up"] = s.can_move_up;
	j["can_move_down"] = s.can_move_down;
	j["can_move_right"] = s.can_move_right;
	j["can_move_left"] = s.can_move_left;

	j["edge_on_the_left"] = s.edge_on_the_left;
	j["edge_on_the_right"] = s.edge_on_the_right;
	
	j["energy"] = s.energy;

	if(s.parent_sprite!=nullptr){
		j["parent_id"] = s.parent_sprite->id;
	}
	else{
		j["parent_id"] = nullptr;
	}

	if(s.target_sprite!=nullptr){
		j["target_id"] = s.target_sprite->id;
	}
	else{
		j["target_id"] = nullptr;
	}

	j["weight"] = s.weight;
	j["weight_button"] = s.weight_button;

	j["crouched"] = s.crouched;
	j["damage_timer"] = s.damage_timer;

	j["invisible_timer"] = s.invisible_timer;
	j["super_mode_timer"] = s.super_mode_timer;
	j["charging_timer"] = s.charging_timer;
	j["attack1_timer"] = s.attack1_timer;
	j["attack2_timer"] = s.attack2_timer;
	j["in_water"] = s.in_water;
	j["swimming"] = s.swimming;
	j["max_speed_available"] = s.max_speed_available;
	j["hidden"] = s.hidden;
	j["initial_weight"] = s.initial_weight;
	j["damage_taken"] = s.damage_taken;
	j["damage_taken_type"] = s.damage_taken_type;
	j["enemy"] = s.enemy;

	if(s.ammo1!=nullptr){
		j["ammo1"] = s.ammo1->filename;
	}
	else{
		j["ammo1"] = nullptr;
	}

	if(s.ammo2!=nullptr){
		j["ammo2"] = s.ammo2->filename;
	}
	else{
		j["ammo2"] = nullptr;
	}

	j["seen_player_x"] = s.seen_player_x;
	j["seen_player_y"] = s.seen_player_y;

	j["action_timer"] = s.action_timer;
	j["animation_index"] = s.animation_index;
	j["current_sequence"] = s.current_sequence;
	j["frame_timer"] = s.frame_timer;
	j["mutation_timer"] = s.mutation_timer;
	j["respawn_timer"] = s.respawn_timer;
	j["current_command"] = s.current_command;
	j["command_timer"] = s.command_timer;

	j["self_destruction"] = s.self_destruction;
	j["initial_update"] = s.initial_update;
	j["legacy_indestructible_ammo"] = s.legacy_indestructible_ammo;
	j["can_collect_bonuses"] = s.can_collect_bonuses;
	j["original"] = s.original;
	j["player_c"] = s.player_c;
}

void SpritesHandler::jsonToSprite(const nlohmann::json&j, SpriteClass&s, PrototypesHandler&handler)const{
    j.at("id").get_to(s.id);
    j.at("active").get_to(s.active);
    s.prototype = handler.loadPrototype(j.at("prototype").get<std::string>());

    j.at("orig_x").get_to(s.orig_x);
    j.at("orig_y").get_to(s.orig_y);
    j.at("x").get_to(s.x);
    j.at("y").get_to(s.y);
    j.at("a").get_to(s.a);
    j.at("b").get_to(s.b);

    j.at("flip_x").get_to(s.flip_x);
    j.at("flip_y").get_to(s.flip_y);
    j.at("jump_timer").get_to(s.jump_timer);

    j.at("can_move_up").get_to(s.can_move_up);
    j.at("can_move_down").get_to(s.can_move_down);
    j.at("can_move_right").get_to(s.can_move_right);
    j.at("can_move_left").get_to(s.can_move_left);

    j.at("edge_on_the_left").get_to(s.edge_on_the_left);
    j.at("edge_on_the_right").get_to(s.edge_on_the_right);

    j.at("energy").get_to(s.energy);

    if (!j.at("parent_id").is_null())
        s.parent_sprite_id = j.at("parent_id").get<std::size_t>();

    if (!j.at("target_id").is_null())
        s.target_sprite_id = j.at("target_id").get<std::size_t>();


    j.at("weight").get_to(s.weight);
    j.at("weight_button").get_to(s.weight_button);

    j.at("crouched").get_to(s.crouched);
    j.at("damage_timer").get_to(s.damage_timer);

    j.at("invisible_timer").get_to(s.invisible_timer);
    j.at("super_mode_timer").get_to(s.super_mode_timer);
    j.at("charging_timer").get_to(s.charging_timer);
    j.at("attack1_timer").get_to(s.attack1_timer);
    j.at("attack2_timer").get_to(s.attack2_timer);
    j.at("in_water").get_to(s.in_water);
    j.at("swimming").get_to(s.swimming);
    j.at("max_speed_available").get_to(s.max_speed_available);
    j.at("hidden").get_to(s.hidden);
    j.at("initial_weight").get_to(s.initial_weight);
    j.at("damage_taken").get_to(s.damage_taken);
    j.at("damage_taken_type").get_to(s.damage_taken_type);
    j.at("enemy").get_to(s.enemy);

	if(!j.at("ammo1").is_null()){
		s.ammo1 = handler.loadPrototype(j.at("ammo1").get<std::string>());
	}

	if(!j.at("ammo2").is_null()){
		s.ammo2 = handler.loadPrototype(j.at("ammo2").get<std::string>());
	}

    j.at("seen_player_x").get_to(s.seen_player_x);
    j.at("seen_player_y").get_to(s.seen_player_y);

    j.at("action_timer").get_to(s.action_timer);
    j.at("animation_index").get_to(s.animation_index);
    j.at("current_sequence").get_to(s.current_sequence);
    j.at("frame_timer").get_to(s.frame_timer);
    j.at("mutation_timer").get_to(s.mutation_timer);
    j.at("respawn_timer").get_to(s.respawn_timer);
    j.at("current_command").get_to(s.current_command);
    j.at("command_timer").get_to(s.command_timer);

    j.at("self_destruction").get_to(s.self_destruction);
    j.at("initial_update").get_to(s.initial_update);
    j.at("legacy_indestructible_ammo").get_to(s.legacy_indestructible_ammo);
    j.at("can_collect_bonuses").get_to(s.can_collect_bonuses);
    j.at("original").get_to(s.original);
	j.at("player_c").get_to(s.player_c);
}