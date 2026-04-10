//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "game/spriteclass.hpp"

#include "sfx.hpp"
#include "episode/episodeclass.hpp"

#include "system.hpp"
#include "gfx/effect.hpp"

#include "engine/PDraw.hpp"
#include "engine/PLog.hpp"
#include "engine/types.hpp"
#include "exceptions.hpp"

#include <cstring>
#include <algorithm>
#include <sstream>

std::size_t SpriteClass::ID_COUNTER=0;


/* -------- SpriteClass  ------------------------------------------------------------------ */

SpriteClass::SpriteClass(){}
SpriteClass::SpriteClass(PrototypeClass *prototype, int player_c,
 double x,
 double y,
 LevelSector*sector,
 SpriteClass*parent){
	this->id = ID_COUNTER;
	++ID_COUNTER;


	if (prototype) {

		this->prototype         = prototype;
		this->player_c        = player_c;

		this->can_collect_bonuses = this->isPlayer();

		this->active     = true;
		this->removed        = false;

		this->x              = x;
		this->y              = y;
		this->level_sector = sector;
		
		this->orig_x         = x;
		this->orig_y         = y;
		
		this->energy        = prototype->energy;
		this->initial_weight = prototype->weight;
		this->weight         = prototype->weight;
		
		this->enemy     = prototype->enemy;
		this->ammo1         = prototype->ammo1;
		this->ammo2         = prototype->ammo2;

		this->parent_sprite = parent;
	
	}
}

SpriteClass::~SpriteClass() {}

void SpriteClass::SetAnimation(int anim_i, bool reset){
	if (anim_i != animation_index){
		if (reset)
			current_sequence = 0;

		animation_index = anim_i;
	}
}
int SpriteClass::Animoi(){

	if(this->prototype->type==TYPE_CHECKPOINT){
		Animation_Checkpoint();
	}
	else{
		switch (prototype->first_ai()) {
			case AI_ROOSTER:           Animation_Rooster();  break;
			case AI_LITTLE_CHICKEN: Animation_Rooster();  break;
			case AI_BONUS:          Animation_Bonus(); break;
			case AI_EGG:            Animation_Egg();   break;
			case AI_EGG2:           Animation_Egg();   break;
			case AI_PROJECTILE:          Animation_Projectile(); break;
			case AI_JUMPER:         Animation_Rooster();  break;
			case AI_BASIC:          Animation_Basic(); break;
			case AI_TELEPORT:       Animation_Basic(); break;
			default: break;
		}
	}

	const SpriteAnimation& anim = prototype->animations[animation_index];

	/*if (current_sequence >= animaatio.frames_number)
		current_sequence = 0;*/

	if(current_sequence >= anim.sequence.size()){
		current_sequence = 0;
		return 0;
	}

	int frame = anim.sequence[current_sequence] - 1;

	// Calculate how much of the currently valid frame is still displayed
	if (frame_timer < prototype->frame_rate)
		frame_timer++;
	// If the time has elapsed, the next frame of the current animation is changed
	else {
		frame_timer = 0;

		// Are there more frames in the animation?
		if (current_sequence + 1 < anim.sequence.size())
			current_sequence++;
		// If not and the animation is set to loop, the animation is started from the beginning.
		else if(anim.loop) {
			current_sequence = anim.intro;//0;
		}
	}

	if (frame < 0)
		frame = 0;
	
	if (frame >= prototype->frames_number)
		frame = prototype->frames_number - 1;

	return frame;
}
void SpriteClass::draw(int kamera_x, int kamera_y){
	// Tehdaan apumuuttujia
	int	l = (int)prototype->picture_frame_width/2,//width
		h = (int)prototype->picture_frame_height/2,
		x = (int)this->x-(kamera_x),
		y = (int)this->y-(kamera_y);

	int frame = this->Animoi();

	if (prototype->vibrates){
		x += rand()%2 - rand()%2;
		y += rand()%2 - rand()%2;
	}
	
	if (flip_x) {
		
		if(this->invisible_timer){
			if(this->isPlayer() || !this->enemy){
				PDraw::image_cliptransparent(prototype->frames_mirror[frame], x-l-1, y-h, 40, COLOR_GRAY);
			}
		}			
		else{

			switch (prototype->blend_mode)
			{
			case 0:
				PDraw::image_clip(prototype->frames_mirror[frame], x-l-1, y-h);
				break;

			case 1:
				PDraw::image_cliptransparent(prototype->frames_mirror[frame], x-l-1, y-h, prototype->blend_alpha, COLOR_NORMAL);
				break;
			case 13:
				PDraw::image_clip_mirror(prototype->frames_mirror[frame], x-l-1, y-h);
				break;

			default:
				break;
			}

			
		}
			

	} else {

		if(this->invisible_timer){
			if(this->isPlayer() || !this->enemy){
				PDraw::image_cliptransparent(prototype->frames[frame], x-l-1, y-h, 40, COLOR_GRAY);
			}
		}
		else{
			switch (prototype->blend_mode)
			{
			case 0:
				PDraw::image_clip(prototype->frames[frame], x-l-1, y-h);
				break;

			case 1:
				PDraw::image_cliptransparent(prototype->frames[frame], x-l-1, y-h, prototype->blend_alpha, COLOR_NORMAL);
				break;

			case 13:
				PDraw::image_clip_mirror(prototype->frames[frame], x-l-1, y-h);
				break;

			default:
				break;
			}
		}	
	}
}

void SpriteClass::HandleEffects() {

	if (super_mode_timer || prototype->effect == EFFECT_STARS)
		Effect_Super(x, y, prototype->width, prototype->height);
		
	if (prototype->effect == EFFECT_SMOKE)
		Effect_Points(x - 5, y - 5, prototype->width, prototype->height, prototype->color);

	if (prototype->effect == EFFECT_THUNDER && prototype->charge_time > 0)
		if (rand() % prototype->charge_time == 0){
			this->StartThunder();
		}
}

bool SpriteClass::canDamageOnCollision(const SpriteClass* target)const{

	if(target->prototype->indestructible || target->prototype->type==TYPE_BONUS
	||target->damage_taken_type == DAMAGE_SELF_DESTRUCTION){
		return false;
	}
	else if(target->invisible_timer>0){
		int damage_type = this->prototype->damage_type;
		if(this->prototype->is_wall && (damage_type==DAMAGE_COMPRESSION||damage_type==DAMAGE_DROP)){
			return true;
		}
		else{
			return false;
		}
	}

	return true;
}

void SpriteClass::AI_Move_X(double liike){
	if (energy > 0)
		this->x = this->orig_x + liike;
}
void SpriteClass::AI_Move_Y(double liike){
	if (energy > 0)
		this->y = this->orig_y + liike;
}
void SpriteClass::Animation_Basic(){

	int uusi_animaatio = -1;
	bool alusta = false;

	if (energy < 1 && !can_move_down)
	{
		uusi_animaatio = ANIMATION_DEATH;
		alusta = true;
	}
	else
	{

		if (a > -0.2 && a < 0.2 && b == 0 && jump_timer <= 0)
		{
			uusi_animaatio = ANIMATION_IDLE;
			alusta = true;
		}

		if ((a < -0.2 || a > 0.2) && jump_timer <= 0)
		{
			uusi_animaatio = ANIMATION_WALKING;
			alusta = false;
		}

		if (b < 0)//-0.3
		{
			uusi_animaatio = ANIMATION_JUMP_UP;
			alusta = false;
		}

		if ((jump_timer > prototype->max_jump || b > 1.5) && can_move_down)
		{
			uusi_animaatio = ANIMATION_JUMP_DOWN;
			alusta = false;
		}

		if (crouched)
		{
			uusi_animaatio = ANIMATION_SQUAT;
			alusta = true;
		}

		if (attack1_timer > 0)
		{
			uusi_animaatio = ANIMATION_ATTACK1;
			alusta = true;
		}

		if (attack2_timer > 0)
		{
			uusi_animaatio = ANIMATION_ATTACK2;
			alusta = true;
		}

		if (damage_timer > 0)
		{
			uusi_animaatio = ANIMATION_DAMAGE;
			alusta = false;
		}

	}

	if (uusi_animaatio != -1)
		SetAnimation(uusi_animaatio,alusta);

}

void SpriteClass::Animation_Checkpoint(){
	int uusi_animaatio = -1;
	bool alusta = false;

	if (energy < 1 && !can_move_down)
	{
		uusi_animaatio = ANIMATION_DEATH;
		alusta = true;
	}
	else
	{

		if (a > -0.2 && a < 0.2 && b == 0 && jump_timer <= 0)
		{
			uusi_animaatio = ANIMATION_IDLE;
			alusta = true;
		}

		if ((a < -0.2 || a > 0.2) && jump_timer <= 0)
		{
			uusi_animaatio = ANIMATION_WALKING;
			alusta = false;
		}

		if (b < 0)//-0.3
		{
			uusi_animaatio = ANIMATION_JUMP_UP;
			alusta = false;
		}

		if ((jump_timer > prototype->max_jump || b > 1.5) && can_move_down)
		{
			uusi_animaatio = ANIMATION_JUMP_DOWN;
			alusta = false;
		}

		if (crouched)
		{
			uusi_animaatio = ANIMATION_SQUAT;
			alusta = true;
		}

		

		if (attack2_timer > 0)
		{
			uusi_animaatio = ANIMATION_ATTACK2;
			alusta = true;
		}

		else if (attack1_timer > 0)
		{
			uusi_animaatio = ANIMATION_ATTACK1;
			alusta = true;
		}

		else if (damage_timer > 0)
		{
			uusi_animaatio = ANIMATION_DAMAGE;
			alusta = false;
		}

		else if(this->target_sprite!=nullptr && this->prototype->max_speed==0){

			uusi_animaatio = ANIMATION_WALKING;
			alusta = true;
		}

	}

	if (uusi_animaatio != -1)
		SetAnimation(uusi_animaatio,alusta);
}

void SpriteClass::Animation_Rooster(){

	int uusi_animaatio = -1;
	bool alusta = false;

	if (energy < 1 && !can_move_down) {
	
		uusi_animaatio = ANIMATION_DEATH;
		alusta = true;
	
	} else {

		if (a > -0.2 && a < 0.2 && b == 0 && jump_timer <= 0) {

			uusi_animaatio = ANIMATION_IDLE;
			alusta = true;
		
		}

		if ((a < -0.2 || a > 0.2) && jump_timer <= 0) {

			uusi_animaatio = ANIMATION_WALKING;
			alusta = false;
		
		}

		if (b < 0) {

			uusi_animaatio = ANIMATION_JUMP_UP;
			alusta = false;

		}

		if ((jump_timer > 90+10/*prototype->max_jump || b > 1.5*/) && can_move_down) {
		
			uusi_animaatio = ANIMATION_JUMP_DOWN;
			alusta = false;
		
		}

		if (attack1_timer > 0) {

			uusi_animaatio = ANIMATION_ATTACK1;
			alusta = true;

		}

		if (attack2_timer > 0) {

			uusi_animaatio = ANIMATION_ATTACK2;
			alusta = true;

		}

		if (crouched) {

			uusi_animaatio = ANIMATION_SQUAT;
			alusta = true;

		}

		if (damage_timer > 0) {

			uusi_animaatio = ANIMATION_DAMAGE;
			alusta = false;

		}

	}

	if (uusi_animaatio != -1)
		SetAnimation(uusi_animaatio,alusta);
}

void SpriteClass::Animation_Bonus() {
	SetAnimation(ANIMATION_IDLE, true);
}
void SpriteClass::Animation_Projectile() {
	SetAnimation(ANIMATION_IDLE, true);
}

void SpriteClass::Animation_Egg() {

	int uusi_animaatio = ANIMATION_IDLE;
	bool alusta = true;

	if (energy < prototype->energy)
		uusi_animaatio = ANIMATION_DEATH;
	
	SetAnimation(uusi_animaatio, alusta);
}

bool SpriteClass::transformTo(PrototypeClass * transformation){

	if(transformation!=nullptr && transformation!=this->prototype){
		this->prototype = transformation;
		this->initial_weight = transformation->weight;
		this->animation_index = -1;
		this->ammo1 = transformation->ammo1;
		this->ammo2 = transformation->ammo2;

		this->current_command = 0;

		this->updatePhysicsFlags();

		this->seen_player_x = -1;
		this->seen_player_y = -1;

		for(const SpriteAI::AI_Class& ai: this->prototype->AI_f){
			if(ai.trigger == AI_TRIGGER_TRANSFORMATION){
				ai.func(this);
			}
		}

		/**
		 * To fix some legacy indestructible projectiles
		*/
		if(this->legacy_indestructible_ammo){
			this->legacy_indestructible_ammo = false;

			if(!this->prototype->indestructible){
				this->damage_taken = this->prototype->energy;
				this->damage_taken_type = DAMAGE_SELF_DESTRUCTION;
				this->self_destruction = true;
			}
		}
		
		return true;
	}

	return false;
}


void SpriteClass::updatePhysicsFlags(){
	this->swimming = false;
	this->max_speed_available = false;
	this->can_collect_bonuses = this->isPlayer();


	if(this->prototype->can_push_bonuses.has_value()){
		this->can_push_bonuses = this->prototype->can_push_bonuses.value();
	}
	else if(this->prototype->type==TYPE_BONUS){
		this->can_push_bonuses = false;
	}
	else if(this->prototype->ambient){
		this->can_push_bonuses = this->isPlayer() || this->prototype->weight > 0;
	}
	else{
		this->can_push_bonuses = true;
	}

	/**
	 * @brief 
	 * Set the default player type
	 */
	if(this->isPlayer()){
		this->player_c = 1;
	}
}

void SpriteClass::die(){
	this->damage_taken = this->energy + 1;
	this->damage_taken_type = DAMAGE_SELF_DESTRUCTION;
}

void SpriteClass::StartThunder(){
	StartLightningEffect();
	Play_GameSFX(Episode->sfx.thunder_sound, 100, (int)this->x, (int)this->y, SOUND_SAMPLERATE, true);
}


bool SpriteClass::flyToWaypointX(double target_x){
	double max_speed = this->prototype->max_speed / 3.5;
    double dx = this->x - target_x;
    
    
    if(dx*dx <= max_speed*max_speed){
        //Waypoint reached, align
        this->a = 0;
        this->x = target_x;
        return true;
    }
    else if(this->x > target_x){
        this->a = -max_speed;
    }
    else{
        this->a = max_speed;
    }
    return false;
}

bool SpriteClass::flyToWaypointY(double target_y){
	double max_speed = this->prototype->max_speed / 3.5;
    double dy = this->y - target_y;

    if(dy*dy <= max_speed*max_speed){
        //Waypoint reached, align
        this->b = 0;
        this->y = target_y;
        return true;
    }
    else if(this->y > target_y){
        this->b = -max_speed;
    }
    else{
        this->b = max_speed;
    }


    return false;
}

bool SpriteClass::flyToWaypointXY(double target_x, double target_y){
	double velocity = this->prototype->max_speed / 3.5;
    double dx = this->x - target_x;
    double dy = this->y - target_y;

    double eps2 = dx*dx + dy*dy; 

    if(eps2 <= velocity*velocity){
        //Waypoint reached, align
        this->a = 0;
        this->b = 0;
        this->x = target_x;
        this->y = target_y;
        return true;
    }
    else{
        double z = sqrt(eps2);
        this->a = -velocity * dx / z;
        this->b = -velocity * dy / z;
    }

    return false;
}