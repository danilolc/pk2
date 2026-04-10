//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include <sstream>
#include "game/physics.hpp"

#include "game/game.hpp"

#include "game/gifts.hpp"

#include "settings/settings.hpp"
#include "settings/config_txt.hpp"

#include "gfx/particles.hpp"
#include "gfx/effect.hpp"
#include "gfx/text.hpp"
#include "language.hpp"
#include "sfx.hpp"
#include "system.hpp"
#include "episode/episodeclass.hpp"
#include "gfx/touchscreen.hpp"
#include "exceptions.hpp"


#include "engine/types.hpp"
#include "engine/PInput.hpp"
#include "engine/PSound.hpp"


#include "lua/lua_game_events.hpp"

#include <cstring>

static double sprite_left;
static double sprite_right;
static double sprite_upper;
static double sprite_bottom;

static int sprite_width;
static int sprite_height;

static void Check_SpriteBlock(SpriteClass* sprite, const PK2BLOCK &block) {

	//left and right
	if (sprite_upper < block.bottom && sprite_bottom-1 > block.top){
		if (sprite_right+sprite->a-1 > block.left && sprite_left+sprite->a < block.right){
			// Tutkitaan onko sprite menossa oikeanpuoleisen palikan sis��n.
			if (sprite_right+sprite->a < block.right){
				// Onko block sein�?
				if (block.right_side == BLOCK_WALL){
					sprite->can_move_right = false;
					if (block.id == BLOCK_LIFT_HORI)
						sprite->x = block.left - sprite_width/2;
				}
			}

			if (sprite_left+sprite->a > block.left){
				if (block.left_side == BLOCK_WALL){
					sprite->can_move_left = false;

					if (block.id == BLOCK_LIFT_HORI)
						sprite->x = block.right + sprite_width/2;

				}
			}
		}
	}

	sprite_left = sprite->x-sprite_width/2;
	sprite_right = sprite->x+sprite_width/2;

	//ceil and floor

	if (sprite_left < block.right && sprite_right-1 > block.left){
		if (sprite_bottom+sprite->b-1 >= block.top && sprite_upper+sprite->b <= block.bottom){
			if (sprite_bottom+sprite->b-1 <= block.bottom){
				if (block.bottom_side == BLOCK_WALL){
					sprite->can_move_down = false;

					if (block.id == BLOCK_LIFT_VERT)
						sprite->y = block.top - sprite_height /2;

					if (sprite_bottom-1 >= block.top && sprite->b >= 0)
						if (block.id != BLOCK_LIFT_HORI)
							sprite->y = block.top - sprite_height /2;
				}
			}

			if (sprite_upper+sprite->b > block.top){
				if (block.top_side == BLOCK_WALL){
					sprite->can_move_up = false;

					if (sprite_upper < block.bottom)
						if (block.id != BLOCK_LIFT_HORI)
							sprite->crouched = true;
				}
			}
		}
	}
}

void Check_MapBlock(SpriteClass* sprite, PK2BLOCK block) {

	//If sprite is in the block
	if (sprite->x <= block.right && sprite->x >= block.left && sprite->y <= block.bottom && sprite->y >= block.top){

		/**********************************************************************/
		/* Examine if block is water background                               */
		/**********************************************************************/
		if (block.water)
			sprite->in_water = true;
		else
			sprite->in_water = false;

		/**********************************************************************/
		/* Examine if it touches the fire                                     */
		/**********************************************************************/
		if (block.id == BLOCK_FIRE && Game->button1 == 0 &&
		sprite->damage_timer == 0 && sprite->damage_taken_type>DAMAGE_SELF_DESTRUCTION){

			sprite->damage_taken = 2;
			sprite->damage_taken_type = DAMAGE_FIRE;
		}

		/**********************************************************************/
		/* Examine if block is hideway			                              */
		/**********************************************************************/
		if (block.id == BLOCK_HIDEOUT)
			sprite->hidden = true;
		else
			sprite->hidden = false;
		

		/**********************************************************************/
		/* Examine if block is the exit                                       */
		/**********************************************************************/
		if (block.id == BLOCK_EXIT && (sprite->energy>0 || config_txt.deadLevelFinish)) {

			switch (Game->level.game_mode)
			{
			case GAME_MODE_STANDARD:
				if(sprite == Game->playerSprite){
					Game->finish();
				}
				break;
			case GAME_MODE_LUA_EXIT:
				if(sprite == Game->playerSprite){
					PK2lua::TriggerEventListeners(PK2lua::LUA_EVENT_EXIT_TOUCHED);
				}
				break;
			case GAME_MODE_CHICK:
				if(sprite->HasAI(AI_CHICK)){
					Game->finish();
				}
				break;
			default:
				break;
			}
		}
	}

	//If sprite is thouching the block
	if (sprite_left <= block.right-4 && sprite_right >= block.left+4 && sprite_upper <= block.bottom && sprite_bottom >= block.top+16){
		/**********************************************************************/
		/* Examine if it touches the fire                                     */
		/**********************************************************************/
		if (block.id == BLOCK_FIRE && Game->button1 == 0 && sprite->damage_timer == 0
		&& sprite->damage_taken_type>DAMAGE_SELF_DESTRUCTION){
			sprite->damage_taken = 2;
			sprite->damage_taken_type = DAMAGE_FIRE;
		}
	}

	//Examine if there is a block on bottom
	if ((block.id<80 || block.id>139) && block.id != BLOCK_BARRIER_DOWN && block.id < 150){
		int mask_index = (int)(sprite->x+sprite->a) - block.left;

		if (mask_index < 0)
			mask_index = 0;

		if (mask_index > 31)
			mask_index = 31;

		block.top += sprite->level_sector->tileset1->block_masks[block.id].alas[mask_index];

		if (block.top >= block.bottom-2)
			block.bottom_side = BLOCK_BACKGROUND;

		block.bottom -= sprite->level_sector->tileset1->block_masks[block.id].ylos[mask_index];
	}

	//If sprite is thouching the block (again?)
	if (sprite_left <= block.right+2 && sprite_right >= block.left-2 && sprite_upper <= block.bottom && sprite_bottom >= block.top){
		/**********************************************************************/
		/* Examine if it is a key and touches lock wall                       */
		/**********************************************************************/
		if (block.id == BLOCK_LOCK && sprite->prototype->can_open_locks){
			LevelSector * sector = sprite->level_sector;

			sector->foreground_tiles[block.left/32+(block.top/32)* sector->getWidth()] = 255;
			sector->calculateEdges();

			sprite->removed = true;

			if (!sprite->prototype->indestructible) {
				Game->keys--;
				if (Game->keys < 1)
					Game->openLocks();
			}

			Effect_Explosion(block.left+16, block.top+10, 0);
			Play_GameSFX(Episode->sfx.open_locks_sound,100, (int)sprite->x, (int)sprite->y, SOUND_SAMPLERATE, false);
		}

		/**********************************************************************/
		/* Make wind effects                                                  */
		/**********************************************************************/
		if (block.id == BLOCK_DRIFT_LEFT && sprite->can_move_left)
			sprite->a -= 0.02;

		if (block.id == BLOCK_DRIFT_RIGHT && sprite->can_move_right)
			sprite->a += 0.02;	//0.05

		/*********************************************************************/
		/* Examine if sprite is on the border to fall                        */
		/*********************************************************************/
		if (block.border && sprite->jump_timer <= 0 && sprite->y < block.bottom && sprite->y > block.top){
			/* && sprite_bottom <= block.bottom+2)*/ // onko sprite tullut borderlle
			if (sprite_left > block.left)
				sprite->edge_on_the_left = true;

			if (sprite_right < block.right)
				sprite->edge_on_the_right = true;
		}
	}

	//Examine walls on left and right

	if (sprite_upper < block.bottom && sprite_bottom-1 > block.top) {
		if (sprite_right+sprite->a-1 > block.left && sprite_left+sprite->a < block.right) {
			// Examine whether the sprite going in the right side of the block.
			if (sprite_right+sprite->a < block.right) {
				// Onko block sein�?
				if (block.right_side == BLOCK_WALL) {
					sprite->can_move_right = false;

					if (block.id == BLOCK_LIFT_HORI)
						sprite->x = block.left - sprite_width/2;
				}
			}
			// Examine whether the sprite going in the left side of the block.
			if (sprite_left+sprite->a > block.left) {
				if (block.left_side == BLOCK_WALL) {
					sprite->can_move_left = false;

					if (block.id == BLOCK_LIFT_HORI)
						sprite->x = block.right + sprite_width/2;

				}
			}
		}
	}

	sprite_left = sprite->x - sprite_width/2;
	sprite_right = sprite->x + sprite_width/2;

	//Examine walls on up and down

	if (sprite_left < block.right && sprite_right-1 > block.left) { //Remove the left and right blocks
		if (sprite_bottom+sprite->b-1 >= block.top && sprite_upper+sprite->b <= block.bottom) { //Get the up and down blocks
			if (sprite_bottom+sprite->b-1 <= block.bottom) { //Just in the sprite's foot
				if (block.bottom_side == BLOCK_WALL) { //If it is a wall
					sprite->can_move_down = false;
					if (block.id == BLOCK_LIFT_VERT)
						sprite->y = block.top - sprite_height /2;

					if (sprite_bottom-1 >= block.top && sprite->b >= 0) {
						//sprite->y -= sprite_bottom - block.top;
						if (block.id != BLOCK_LIFT_HORI) {
							sprite->y = block.top - sprite_height /2;
						}
					}

					if (sprite->weight_button >= 1) { // Sprite can press the buttons
						if (block.id == BLOCK_BUTTON1 && Game->button1 == 0) {
							Game->button1 = Game->level.button1_time;
							Game->button_vibration = 64;
							Play_GameSFX(Episode->sfx.switch_sound, 100, (int)sprite->x, (int)sprite->y, SOUND_SAMPLERATE, false);
							Settings.vibrateController(Game->button_vibration);
						}

						if (block.id == BLOCK_BUTTON2 && Game->button2 == 0) {
							Game->button2 = Game->level.button2_time;
							Game->button_vibration = 64;
							Play_GameSFX(Episode->sfx.switch_sound, 100, (int)sprite->x, (int)sprite->y, SOUND_SAMPLERATE, false);
							Settings.vibrateController(Game->button_vibration);
						}

						if (block.id == BLOCK_BUTTON3 && Game->button3 == 0) {
							Game->button3 = Game->level.button3_time;
							Game->button_vibration = 64;
							Play_GameSFX(Episode->sfx.switch_sound, 100, (int)sprite->x, (int)sprite->y, SOUND_SAMPLERATE, false);
							Settings.vibrateController(Game->button_vibration);
						}
					}

				}
			}

			if (sprite_upper+sprite->b > block.top) {
				if (block.top_side == BLOCK_WALL) {
					sprite->can_move_up = false;

					if (sprite_upper < block.bottom) {
						if (block.id == BLOCK_LIFT_VERT && sprite->crouched && !sprite->can_move_down && sprite->damage_taken_type>DAMAGE_SELF_DESTRUCTION) {
							sprite->damage_taken = 2;
							sprite->damage_taken_type = DAMAGE_IMPACT;
						}

						if (block.id != BLOCK_LIFT_HORI) {
							//if (sprite->crouched)
							//	sprite->y = block.bottom + sprite_height /2;

							sprite->crouched = true;
						}
					}
				}
			}
		}
	}
}

void SpriteOffscreen(SpriteClass* sprite){
	if(sprite->energy>0){
		for(const SpriteAI::AI_Class& ai:sprite->prototype->AI_f){
			if(ai.trigger==AI_TRIGGER_OFFSCREEN){
				ai.func(sprite);
			}
		}
	}
}

void SpriteOnDamage(SpriteClass* sprite){
	sprite->energy -= sprite->damage_taken;
	
	switch (sprite->damage_taken_type)
	{
	case DAMAGE_ELECTRIC:
		sprite->damage_timer = 6 * DAMAGE_TIME;
		break;
	case DAMAGE_INSTANT:
		sprite->damage_timer = 1;
		break;
	default:
		sprite->damage_timer = DAMAGE_TIME;
		break;
	}

	/*sprite->damage_timer = DAMAGE_TIME;

	if (sprite->damage_taken_type == DAMAGE_ELECTRIC)
		sprite->damage_timer *= 6;*/

	Play_GameSFX(sprite->prototype->sounds[SOUND_DAMAGE], 100, (int)sprite->x, (int)sprite->y,
					sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);

	if (sprite->prototype->destruction_effect%100 == FX_DESTRUCT_FEATHERS)
		Effect_By_ID(FX_DESTRUCT_FEATHERS, (u32)sprite->x, (u32)sprite->y);

	if (sprite->prototype->type != TYPE_PROJECTILE){
		Particles_New(PARTICLE_STAR,sprite->x,sprite->y,-1,-1,60,0.01,128);
		Particles_New(PARTICLE_STAR,sprite->x,sprite->y, 0,-1,60,0.01,128);
		Particles_New(PARTICLE_STAR,sprite->x,sprite->y, 1,-1,60,0.01,128);
	}


	for(const SpriteAI::AI_Class& ai: sprite->prototype->AI_f){
		if(!ai.apply_to_creatures)continue;

		if(ai.trigger==AI_TRIGGER_DAMAGE){
			ai.func(sprite);
		}
	}
}

void SpriteOnRespawn(SpriteClass* sprite){
	int effect_id = sprite->prototype->destruction_effect;
	if(effect_id>=100)
		effect_id -= 100;

	Effect_By_ID(effect_id, sprite->x, sprite->y);

	sprite->removed = false;
	sprite->energy = sprite->prototype->energy;
	sprite->charging_timer = 0;
	sprite->damage_timer = 0;
	sprite->attack1_timer = 0;
	sprite->attack2_timer = 0;
}

void SpriteOnDeath(SpriteClass* sprite){
	int destruction_effect = sprite->prototype->destruction_effect;
	LevelSector* sector = sprite->level_sector;

	for(const SpriteAI::AI_Class&ai: sprite->prototype->AI_f){

		if(ai.trigger==AI_TRIGGER_DEATH){
			ai.func(sprite);
		}
	}

	if (sprite->prototype->bonus != nullptr && sprite->prototype->bonuses_number > 0){
		if (sprite->prototype->bonus_always || rand()%4 == 1){

			int bonuses_number = sprite->prototype->bonuses_number;
			PrototypeClass* bonus = sprite->prototype->bonus;

			if(bonuses_number>1){
				for(int i=0;i<bonuses_number;++i){
					sector->sprites.addDroppedBonusSprite(bonus, sprite->x+(10-rand()%21),sprite->y+(10-rand()%21));
				}
			}
			else if(bonuses_number==1){
				sector->sprites.addDroppedBonusSprite(bonus, sprite->x,sprite->y);
			}			
		}
	}

	if (destruction_effect >= FX_DESTRUCT_ANIMATED){
		destruction_effect -= FX_DESTRUCT_ANIMATED;
	}
	else if(sprite->respawn_timer==0)
		sprite->removed = true;

	Effect_By_ID(destruction_effect, (u32)sprite->x, (u32)sprite->y);
	Play_GameSFX(sprite->prototype->sounds[SOUND_DESTRUCTION],100, (int)sprite->x, (int)sprite->y,
					sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);

	if (sprite->prototype->type == TYPE_GAME_CHARACTER && sprite->prototype->score != 0 && !sprite->isPlayer()){
		Fadetext_New(fontti2,std::to_string(sprite->prototype->score),(int)sprite->x-8,(int)sprite->y-8,80);
		Game->score_increment += sprite->prototype->score;
	}


	for(SpriteClass* sprite2: sector->sprites.Sprites_List){
		if(sprite2->parent_sprite==sprite){
			sprite2->parent_sprite=nullptr;
			if(sprite2->HasAI(AI_DIE_WITH_MOTHER_SPPRITE)){
				sprite2->damage_taken = sprite2->energy;
				sprite2->damage_taken_type = DAMAGE_SELF_DESTRUCTION;
			}
		}

		if(sprite2->target_sprite==sprite){

			if(sprite2->prototype->type!=TYPE_CHECKPOINT || !sprite->isPlayer()){
				sprite2->target_sprite=nullptr;
			}
		}
	}

	/**
	 * killed enemies counter
	 * */
	if(sprite->original
	&& sprite->prototype->type == TYPE_GAME_CHARACTER
	&& !sprite->prototype->indestructible //to ignore indestructible
	&& sprite->prototype->damage > 0 //to ignore switches, boxes and so on
	&& sprite->prototype->enemy){
		
		Game->enemies--;
		if(Game->enemies<=0){
			Game->enemies=0;
		}
	}

	if(Game->lastCheckpoint==sprite){
		Game->showInfo("Your checkpoint was destroyed!");
		Game->lastCheckpoint = nullptr;
	}
}

void PotionTransformation(SpriteClass* sprite, PrototypeClass* intended_prototype){
	if (intended_prototype->type == TYPE_GAME_CHARACTER){
		/**
		 * @brief 
		 * Robohead turning into rooster is no longer considered enemy.
		 */
		if(sprite->enemy && !intended_prototype->enemy){
			sprite->enemy = false;
		}

		sprite->prototype = intended_prototype;

		sprite->ammo1 = sprite->prototype->ammo1;
		sprite->ammo2 = sprite->prototype->ammo2;
		sprite->initial_weight = sprite->prototype->weight;

		/**
		 * @brief 
		 * Transformation offset
		 */
		if(Episode!=nullptr && Episode->transformation_offset){
			sprite->y -= sprite->prototype->height/2;
		}


		sprite->updatePhysicsFlags();

		if(sprite==Game->playerSprite){
			int infotext = Episode->infos.Search_Id("pekka transformed");
			if (infotext != -1)
				Game->showInfo(Episode->infos.Get_Text(infotext));
		}

		for(const SpriteAI::AI_Class& ai: sprite->prototype->AI_f){
			if(ai.trigger == AI_TRIGGER_TRANSFORMATION){
				ai.func(sprite);
			}
		}
	}
}

void BonusSpriteCollected(SpriteClass* sprite, SpriteClass* collector){

	if(collector->prototype->type==TYPE_PROJECTILE){
		collector = collector->parent_sprite;
		if(collector==nullptr){
			return;
		}
	}
	if (sprite->prototype->score != 0) {
		if(sprite->prototype->big_apple){
			Game->apples_got++;
		}

		Game->score_increment += sprite->prototype->score;
		
		if (!sprite->HasAI(AI_BONUS_CLOCK)) {
			int font = sprite->prototype->score >= 50? fontti2 : fontti1;
			Fadetext_New(font, std::to_string(sprite->prototype->score), (int)sprite->x-8,(int)sprite->y-8,100);
		}

	}

	if (!sprite->prototype->indestructible)
	{
		bool showed_info = false;

		collector->energy -= sprite->prototype->damage;
		int destruction_effect = sprite->prototype->destruction_effect;

		if (destruction_effect >= FX_DESTRUCT_ANIMATED){
			destruction_effect -= FX_DESTRUCT_ANIMATED;
		}
		else
		{
			if (sprite->prototype->can_open_locks)
			{
				Game->keys--;

				if (Game->keys < 1)
					Game->openLocks();
			}

			sprite->removed = true;
		}


		for(const int& ai: sprite->prototype->AI_v){
			switch (ai)
			{
			case AI_REBORN:{
					sprite->respawn_timer = sprite->prototype->charge_time;
					sprite->energy = sprite->prototype->energy;
					sprite->removed = false;
					sprite->x = sprite->orig_x;
					sprite->y = sprite->orig_y;
			}
			break;
			case AI_BONUS_INVISIBILITY:{
				collector->invisible_timer = sprite->prototype->charge_time;
			}
			break;
			case AI_BONUS_SUPERMODE:{
				collector->super_mode_timer = sprite->prototype->charge_time;
				if(collector == Game->playerSprite && Episode->supermode_music){
					Game->startSupermodeMusic();
				}
				//PSound::play_overlay_music();
				   // the problem is this will most likely overwrite the current music, fixlater
			}
			break;
			case AI_BONUS_CLOCK:{
				int increase_time = sprite->prototype->charge_time * TIME_FPS;
				Game->timeout += increase_time;

				float shown_time = float(increase_time) / 60;
				int minutes = int(shown_time / 60);
				int seconds = int(shown_time) % 60;

				std::ostringstream os;
				os<<minutes<<":";
				if(seconds<10){
					os<<"0";
				}
				os<<seconds;
				
				Fadetext_New(fontti1,os.str(),(int)sprite->x-15,(int)sprite->y-8,100);
			}
			break;
			case AI_INFO_IF_BONUS_COLLECTED:{
				showed_info = true;
				AI_Functions::DisplayInfo(sprite);
			}
			break;

			case AI_EMIT_EVENT1_IF_BONUS_COLLECTED:{
				Game->event1 = true;
			}
			break;

			case AI_EMIT_EVENT2_IF_BONUS_COLLECTED:{
				Game->event2 = true;
			}
			break;

			case AI_CHANGE_SKULLS_IF_BONUS_COLLECTED:{
				Game->change_skulls = true;
			}
			break;


			default:
				break;
			}
		}



		if (sprite->prototype->bonus != nullptr)
			if (Game->gifts.add(sprite->prototype->bonus))
				Game->showInfo(tekstit->Get_Text(PK_txt.game_newitem));

		if (sprite->prototype->transformation != nullptr){
			PotionTransformation(collector, sprite->prototype->transformation);
		}

		if (sprite->prototype->ammo2 != nullptr && collector->ammo2 != sprite->prototype->ammo2){
			collector->ammo2 = sprite->prototype->ammo2;

			if(!showed_info && collector == Game->playerSprite){
				showed_info = true;

				int infotext = Episode->infos.Search_Id("new doodle attack");
				if (infotext != -1)
					Game->showInfo(Episode->infos.Get_Text(infotext));
				else
					Game->showInfo(tekstit->Get_Text(PK_txt.game_newdoodle));
			}
		}

		if (sprite->prototype->ammo1 != nullptr && collector->ammo1 != sprite->prototype->ammo1){
			collector->ammo1 = sprite->prototype->ammo1;

			if(!showed_info && collector == Game->playerSprite){
				showed_info = true;

				int infotext = Episode->infos.Search_Id("new egg attack");
				if (infotext != -1)
					Game->showInfo(Episode->infos.Get_Text(infotext));
				else
					Game->showInfo(tekstit->Get_Text(PK_txt.game_newegg));
			}
		}

		Play_GameSFX(sprite->prototype->sounds[SOUND_DESTRUCTION],100, (int)sprite->x, (int)sprite->y,
						sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);

		Effect_By_ID(destruction_effect, (u32)sprite->x, (u32)sprite->y);
	}
}

void UpdateSprite(SpriteClass* sprite){
	LevelSector * sector = sprite->level_sector;

	sprite_width  = sprite->prototype->width;
	sprite_height = sprite->prototype->height;

	sprite_left = sprite->x - sprite_width  / 2;
	sprite_right = sprite->x + sprite_width  / 2;
	sprite_upper   = sprite->y - sprite_height / 2;
	sprite_bottom   = sprite->y + sprite_height / 2;

	double max_speed = sprite->prototype->max_speed;

	bool in_water = sprite->in_water;

	sprite->crouched = false;

	sprite->edge_on_the_left = false;
	sprite->edge_on_the_right = false;


	/* Pistet��n vauhtia tainnutettuihin spriteihin */
	if (sprite->energy < 1)
		max_speed = 3;

	// Calculate the remainder of the sprite towards

	if (sprite->attack1_timer > 0)
		sprite->attack1_timer--;

	if (sprite->attack2_timer > 0)
		sprite->attack2_timer--;

	if (sprite->charging_timer > 0)	// aika kahden ampumisen (munimisen) v�lill�
		sprite->charging_timer --;

	if (sprite->mutation_timer > 0)	// time of mutation
		sprite->mutation_timer --;

	/*****************************************************************************************/
	/* Player-sprite and its controls                                                        */
	/*****************************************************************************************/

	bool add_speed = true;
	bool gliding = false;

	/*bool max_speed_available = sprite->HasAI(AI_MAX_SPEED_PLAYER) ||
		(sprite->swimming && sprite->HasAI(AI_MAX_SPEED_SWIMMING)) ||
		(sprite->super_mode_timer > 0 && sprite->HasAI(AI_MAX_SPEED_PLAYER_ON_SUPER));*/

	if (sprite->isPlayer() && sprite->energy > 0){

		const InputSettings& input = Settings.getInput();

		/* ATTACK 1 */
		if ((input.attack1.isPressed() || TouchScreenControls.egg) && sprite->charging_timer == 0 && sprite->ammo1 != nullptr)
			sprite->attack1_timer = sprite->prototype->attack1_time;
		/* ATTACK 2 */
		else if ((input.attack2.isPressed() || TouchScreenControls.doodle) && sprite->charging_timer == 0 && sprite->ammo2 != nullptr)
				sprite->attack2_timer = sprite->prototype->attack2_time;
		
		if(sprite->player_c==1){

			/* CROUCH */
			sprite->crouched = false;


			bool axis_couch = Settings.getJoystickAxis(1) > 0.5;
			if ((input.down.isPressed() || TouchScreenControls.down || axis_couch) && !sprite->can_move_down) {
				sprite->crouched = true;
				sprite_upper += sprite_height/1.5;
			}

			/* SLOW WALK */
			if ( input.walk_slow.isPressed() || TouchScreenControls.pad_button == 1 || TouchScreenControls.pad_button == 3)
				add_speed = false;

			/* NAVIGATING*/

			int navigation = Settings.getJoystickAxis(0) * 100;;

			if (TouchScreenControls.pad_button == 0 || TouchScreenControls.pad_button == 1)
				navigation = -100;
			else if (TouchScreenControls.pad_button == 3 || TouchScreenControls.pad_button == 4)
				navigation = 100;

			if (input.right.isPressed())
				navigation = 100;
			
			if (input.left.isPressed())
				navigation = -100;

			double a_lisays = 0.04;//0.08;

			if (add_speed) {
				if (rand()%20 == 1 && sprite->animation_index == ANIMATION_WALKING) // Draw dust
					Particles_New(PARTICLE_DUST_CLOUDS,sprite->x-8,sprite_bottom-8,0.25,-0.25,40,0,0);

				a_lisays += 0.09;//0.05
			}

			if (sprite->can_move_down)
				a_lisays /= 1.5;//2.0

			a_lisays *= double(navigation) / 100;

			if (sprite->max_speed_available)
				a_lisays *= max_speed;
					
			if (navigation > 0)
				sprite->flip_x = false;
			else if (navigation < 0)
				sprite->flip_x = true;

			if (sprite->crouched)	// Slow when couch
				a_lisays /= 10;

			sprite->a += a_lisays;
		}

		if(sprite->player_c < 3){
			/* JUMPING */
			if (sprite->prototype->weight > 0 && !sprite->swimming) {
				if (input.up.isPressed() || TouchScreenControls.up) {
					if (!sprite->crouched) {
						if (sprite->jump_timer == 0)
							Play_GameSFX(Episode->sfx.jump_sound, 100, (int)sprite->x, (int)sprite->y,
										sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);

						if (sprite->jump_timer <= 0)
							sprite->jump_timer = 1; //10;
					}
				} else {
					if (sprite->jump_timer > 0 && sprite->jump_timer < 45)
						sprite->jump_timer = 55;
				}

				/* dripping quietly down */
				bool axis_up = Settings.getJoystickAxis(1) < -0.5;
				if ((input.up.isPressed() || TouchScreenControls.up || axis_up) && sprite->jump_timer >= 150/*90+20*/ &&
					sprite->prototype->can_glide)
					gliding = true;
			}
			/* MOVING UP AND DOWN */
			else { // if the player sprite-weight is 0 - like birds

				double speed = 0.15;
				if (sprite->max_speed_available)
					speed *= max_speed;

				if (input.up.isPressed() || TouchScreenControls.up || Settings.getJoystickAxis(1) < -0.5)
					sprite->b -= speed;

				if (input.down.isPressed() || TouchScreenControls.down || Settings.getJoystickAxis(1) > 0.5)
					sprite->b += speed;

				sprite->jump_timer = 0;
			}			
		}

		/* AI */
		for(std::size_t i=0;i<sprite->prototype->AI_f.size();++i){
			const SpriteAI::AI_Class& ai = sprite->prototype->AI_f[i];
			
			if(sprite->player_c==1){
				if(!ai.apply_to_player)continue;
			}
			else{
				if((!ai.apply_to_player && !ai.apply_to_creatures) || ai.requires_player_sprite)continue;
			}

			if( (sprite->energy>0 && ai.trigger==AI_TRIGGER_ALIVE) || ai.trigger==AI_TRIGGER_ANYWAY){
				ai.func(sprite);
			}
		}


		/*for(const SpriteAI::AI_Class& ai:sprite->prototype->AI_f){
			if(!ai.apply_to_player)continue;

			if( (sprite->energy>0 && ai.trigger==AI_TRIGGER_ALIVE) || ai.trigger==AI_TRIGGER_ANYWAY){
				ai.func(sprite);
			}
		}*/

		/* It is not acceptable that a player is anything other than the game character */
		if (sprite->prototype->type != TYPE_GAME_CHARACTER)
			sprite->energy = 0;
	}


	sprite->can_move_right	 = true,
	sprite->can_move_left	 = true,
	sprite->can_move_up		 = true,
	sprite->can_move_down		 = true;

	/*****************************************************************************************/
	/* Jump                                                                                  */
	/*****************************************************************************************/

	bool hyppy_maximissa = sprite->jump_timer >= 90;

	// Jos ollaan hyp�tty / ilmassa:
	if (sprite->jump_timer > 0) {
		if (sprite->jump_timer < 50-sprite->prototype->max_jump)
			sprite->jump_timer = 50-sprite->prototype->max_jump;

		if (sprite->jump_timer < 10)
			sprite->jump_timer = 10;

		if (!hyppy_maximissa) {
		// sprite->b = (sprite->prototype->max_jump/2 - sprite->jump_timer/2)/-2.0;//-4
		   sprite->b = -sin_table(sprite->jump_timer)/8;//(sprite->prototype->max_jump/2 - sprite->jump_timer/2)/-2.5;
			if (sprite->b > sprite->prototype->max_jump){
				sprite->b = sprite->prototype->max_jump/10.0;
				sprite->jump_timer = 90 - sprite->jump_timer;
			}

		}

		if (sprite->jump_timer < 180)
			sprite->jump_timer += 2;
	}

	if (sprite->jump_timer < 0)
		sprite->jump_timer++;

	if (sprite->b > 0 && !hyppy_maximissa)
		sprite->jump_timer = 90;//sprite->prototype->max_jump*2;

	/*****************************************************************************************/
	/* Hit recovering                                                                        */
	/*****************************************************************************************/

	if (sprite->damage_timer > 0)
		sprite->damage_timer--;

	/*****************************************************************************************/
	/* Timers                                                                                */
	/*****************************************************************************************/

	if (sprite->invisible_timer > 0) {
	
		sprite->invisible_timer--;

	}

	SpriteClass* Player_Sprite = Game->playerSprite;
	
	if (sprite->super_mode_timer > 0) {

		sprite->super_mode_timer--;

		if(sprite==Player_Sprite && sprite->super_mode_timer==0){
			Player_Sprite->level_sector->startMusic();
		}
	}

	/*****************************************************************************************/
	/* Gravity effect                                                                        */
	/*****************************************************************************************/
	
	if (sprite->weight != 0 && (sprite->jump_timer <= 0 || sprite->jump_timer >= 45) && !sprite->swimming)
		sprite->b += sprite->weight/1.25;// + sprite->b/1.5;

	if (gliding && sprite->b > 0) // If gliding
		sprite->b /= 1.3;//1.5;//3

	/*****************************************************************************************/
	/* Speed limits                                                                          */
	/*****************************************************************************************/

	if (sprite->b > 4.0)//4
		sprite->b = 4.0;//4

	if (sprite->b < -4.0)
		sprite->b = -4.0;

	//Limit speed 1
	if (sprite->a > max_speed)
		sprite->a = max_speed;

	if (sprite->a < -max_speed)
		sprite->a = -max_speed;

	/*****************************************************************************************/
	/* Blocks colision -                                                                     */
	/*****************************************************************************************/

	if (sprite->prototype->check_tiles){ //Find the tiles that the sprite occupies

		int palikat_x_lkm = (int)((sprite_width) /32)+4; //Number of blocks
		int palikat_y_lkm = (int)((sprite_height)/32)+4;

		int map_vasen = (int)(sprite_left) / 32; //Position in tile level
		int map_yla   = (int)(sprite_upper)   / 32;

		/*****************************************************************************************/
		/* Going through the blocks around the sprite->                                           */
		/*****************************************************************************************/

		//palikat_lkm = palikat_y_lkm*palikat_x_lkm;
		for (int y = 0; y < palikat_y_lkm; y++){
			for (int x = 0; x < palikat_x_lkm; x++) {
				int p = x + y*palikat_x_lkm;
				if ( p < 300 )
					if (!(sprite == Player_Sprite && dev_mode && Settings.getInput().dev_ghostMode.isPressed())){
						PK2BLOCK block = sector->getBlock(map_vasen+x-1,map_yla+y-1, Game->level.block_types);
						//Block_Get(map_vasen+x-1,map_yla+y-1);
						Check_MapBlock(sprite, block);
					}
			}
		}
	}
	/*****************************************************************************************/
	/* If the sprite is under water                                                          */
	/*****************************************************************************************/

	if (sprite->in_water) {

		if (!sprite->prototype->can_swim || sprite->energy < 1) {

			sprite->b /= 2.0;
			sprite->a /= 1.05;

			if (sprite->jump_timer > 0 && sprite->jump_timer < 90)
				sprite->jump_timer--;
		}

		if (rand()%80 == 1)
			Particles_New(PARTICLE_SPARK,sprite->x-4,sprite->y,0,-0.5-rand()%2,rand()%30+30,0,
			sector->splash_color);
	}

	if (in_water != sprite->in_water&&!sprite->initial_update) { // Sprite comes in or out from water
		Effect_Splash(sprite->x, sprite->y, sector->splash_color);
		Play_GameSFX(Episode->sfx.splash_sound, 100, (int)sprite->x, (int)sprite->y, SOUND_SAMPLERATE, true);
	}

	/*****************************************************************************************/
	/* Sprite weight                                                                         */
	/*****************************************************************************************/

	sprite->weight = sprite->initial_weight;
	sprite->weight_button = sprite->weight;

	if(sprite->energy < 1){
		if(sprite->prototype->dead_weight.has_value()){
			sprite->weight = *sprite->prototype->dead_weight;
		}
		else if(sprite->weight == 0){ // Fall when is death
			sprite->weight = 1;
		}
	}

	/*****************************************************************************************/
	/* Sprite collision with other sprites                                                   */
	/*****************************************************************************************/

	double sprite2_yla; // kyykistymiseen liittyv�
	PK2BLOCK spritepalikka;

	//Compare this sprite with every sprite in the game
	for (SpriteClass* sprite2 : sector->sprites.Sprites_List) {
		if (sprite2 != sprite && sprite2->active && !sprite2->removed) {
			if (sprite2->crouched)
				sprite2_yla = sprite2->prototype->height / 3;//1.5;
			else
				sprite2_yla = 0;

			if (sprite2->prototype->is_wall && sprite->prototype->check_tiles && sprite2->energy>0) { //If there is a block sprite active

				if (sprite->x-sprite_width/2 +sprite->a  <= sprite2->x + sprite2->prototype->width /2 &&
					sprite->x+sprite_width/2 +sprite->a  >= sprite2->x - sprite2->prototype->width /2 &&
					sprite->y-sprite_height/2+sprite->b <= sprite2->y + sprite2->prototype->height/2 &&
					sprite->y+sprite_height/2+sprite->b >= sprite2->y - sprite2->prototype->height/2)
				{
					spritepalikka.id = 0;
					spritepalikka.bottom   = (int)sprite2->y + sprite2->prototype->height/2;
					spritepalikka.right = (int)sprite2->x + sprite2->prototype->width/2;
					spritepalikka.left = (int)sprite2->x - sprite2->prototype->width/2;
					spritepalikka.top   = (int)sprite2->y - sprite2->prototype->height/2;

					spritepalikka.water  = false;

					spritepalikka.right_side   = BLOCK_WALL;
					spritepalikka.left_side = BLOCK_WALL;
					spritepalikka.top_side       = BLOCK_WALL;
					spritepalikka.bottom_side       = BLOCK_WALL;

					if (!sprite->prototype->is_wall){
						if (!sprite2->prototype->is_wall_down)
							spritepalikka.bottom_side = BLOCK_BACKGROUND;
						if (!sprite2->prototype->is_wall_up)
							spritepalikka.top_side = BLOCK_BACKGROUND;
						if (!sprite2->prototype->is_wall_right)
							spritepalikka.right_side = BLOCK_BACKGROUND;
						if (!sprite2->prototype->is_wall_left)
							spritepalikka.left_side = BLOCK_BACKGROUND;
					}

					if(Episode->legacy_obstacle_phasing){
						if(sprite2->a > 0){
							spritepalikka.id = BLOCK_LIFT_HORI;
						}
					}
					else if (sprite2->a != 0){
						spritepalikka.id = BLOCK_LIFT_HORI;
					}						

					if (sprite2->b > 0)
						spritepalikka.id = BLOCK_LIFT_VERT;

					if (!(sprite == Player_Sprite && dev_mode && Settings.getInput().dev_ghostMode.isPressed()))
						Check_SpriteBlock(sprite, spritepalikka); //Colision sprite and sprite block
				}
			}

			if (sprite->x <= sprite2->x + sprite2->prototype->width /2 &&
				sprite->x >= sprite2->x - sprite2->prototype->width /2 &&
				sprite->y/*top*/ <= sprite2->y + sprite2->prototype->height/2 &&
				sprite->y/*bottom*/ >= sprite2->y - sprite2->prototype->height/2 + sprite2_yla)
			{
				// sprites with same index change directions when touch
				if (sprite->prototype == sprite2->prototype &&
					sprite2->energy > 0/* && sprite->isPlayer() == 0*/)
				{
					if (sprite->x < sprite2->x)
						sprite->can_move_right = false;
					if (sprite->x > sprite2->x)
						sprite->can_move_left = false;
					if (sprite->y < sprite2->y)
						sprite->can_move_down = false;
					if (sprite->y > sprite2->y)
						sprite->can_move_up = false;
				}

				if (sprite->HasAI(AI_ARROW_BARRIER)) {

					if (sprite2->HasAI(AI_ARROW_RIGHT)) {
						sprite->a = sprite->prototype->max_speed / 3.5;
						sprite->b = 0;
					}
					else if (sprite2->HasAI(AI_ARROW_LEFT)) {
						sprite->a = sprite->prototype->max_speed / -3.5;
						sprite->b = 0;
					}

					if (sprite2->HasAI(AI_ARROW_UP)) {
						sprite->b = sprite->prototype->max_speed / -3.5;
						sprite->a = 0;
					}
					else if (sprite2->HasAI(AI_ARROW_DOWN)) {
						sprite->b = sprite->prototype->max_speed / 3.5;
						sprite->a = 0;
					}
				}

				if(sprite2->prototype->type==TYPE_BLACK_HOLE){
					sprite->energy = 0;
					sprite->removed = true;

					if (sprite->weight_button >= 1){
						Game->vibrate(50);
					}			
				}

				// If two sprites from different teams touch each other
				else if ( (sprite->enemy != sprite2->enemy
				|| sprite->prototype->hostile_to_everyone
				|| sprite2->prototype->hostile_to_everyone)

				&& sprite->parent_sprite != sprite2 &&
				!sprite->prototype->ambient && !sprite2->prototype->ambient) {
					
					if (sprite2->damage_timer == 0 &&
						sprite->damage_timer == 0 &&
						sprite2->energy > 0 &&
						sprite->energy > 0 &&
						sprite2->damage_taken < 1)
					{

						if (sprite->super_mode_timer > 0 && sprite2->super_mode_timer == 0) {
							sprite2->damage_taken = sprite2->energy;
							sprite2->damage_taken_type = DAMAGE_SUPERMODE;
						}
						else if (sprite2->super_mode_timer > 0 && sprite->super_mode_timer == 0) {
							sprite->damage_taken = sprite->energy;
							sprite->damage_taken_type = DAMAGE_SUPERMODE;
						}
						
						//Bounce on the sprite head
						else{
							if (sprite2->b > 2 && sprite2->weight >= 0.5 &&
								sprite2->y < sprite->y && !sprite->prototype->is_wall &&
								sprite2->canDamageOnCollision(sprite))
							{
								sprite->damage_taken = (int)(sprite2->weight+sprite2->b/4);
								sprite->damage_taken_type = DAMAGE_DROP;
								sprite2->jump_timer = 1;
								if (sprite2->HasAI(AI_EGG2)) // Egg bounced, then crack
									sprite2->damage_taken = sprite2->prototype->energy;
							}

							// If there is another sprite damaging
							if (sprite->prototype->damage > 0 &&
							sprite->canDamageOnCollision(sprite2)) {
								
								sprite2->damage_taken        = sprite->prototype->damage;
								sprite2->damage_taken_type = sprite->prototype->damage_type;
							
								if(!sprite->HasAI(AI_NO_CONTACT_ATTACK_ANIMATION)){
									sprite->attack1_timer = sprite->prototype->attack1_time;
								}

								if (sprite2->prototype->type == TYPE_PROJECTILE && sprite2->canDamageOnCollision(sprite)) {
									sprite->damage_taken = sprite2->prototype->damage;
									if(sprite->damage_taken==0){
										sprite->damage_taken = 1;
									}
									
									sprite->damage_taken_type = sprite2->prototype->damage_type;
								}

								// The projectiles are shattered by shock
								if (sprite->prototype->type == TYPE_PROJECTILE) {
									sprite->damage_taken = 1;//sprite2->prototype->damage;
									sprite->damage_taken_type = sprite2->prototype->damage_type;
								}
							}

						}
					}
				}

				// Calculate cumulative weight to determine if the sprite can push buttons.
				if (sprite->weight > 0)
					sprite->weight_button += sprite2->prototype->weight;

			}
		}
	}

	/*****************************************************************************************/
	/* If the sprite has suffered damage                                                     */
	/*****************************************************************************************/
	if (sprite->damage_taken != 0 && sprite->energy > 0){

		if(sprite->damage_taken_type == DAMAGE_SELF_DESTRUCTION){
			SpriteOnDamage(sprite);
		}
		else if (
			!sprite->prototype->indestructible
			&& (sprite->prototype->immunity_type != sprite->damage_taken_type
			|| sprite->prototype->immunity_type == DAMAGE_NONE)
			&& sprite->super_mode_timer==0){
				
			SpriteOnDamage(sprite);
		}

		sprite->damage_taken = 0;
		sprite->damage_taken_type = DAMAGE_NONE;


		/*****************************************************************************************/
		/* If the sprite is destroyed                                                            */
		/*****************************************************************************************/

		if (sprite->energy < 1) {
			SpriteOnDeath(sprite);
		}
	}

	if (sprite->damage_timer == 0)
		sprite->damage_taken_type = DAMAGE_NONE;


	/*****************************************************************************************/
	/* Revisions                                                                             */
	/*****************************************************************************************/

	if (!sprite->can_move_right)
		if (sprite->a > 0)
			sprite->a = 0;

	if (!sprite->can_move_left)
		if (sprite->a < 0)
			sprite->a = 0;

	if (!sprite->can_move_up){
		if (sprite->b < 0)
			sprite->b = 0;

		if (!hyppy_maximissa)
			sprite->jump_timer = 95;//sprite->prototype->max_jump * 2;
	}

	if (!sprite->can_move_down)
		if (sprite->b >= 0 ){ //If sprite is falling
			if (sprite->jump_timer > 0){
				if (sprite->jump_timer >= 90+10){

					if(sprite->weight>0){
						Play_GameSFX(Episode->sfx.pump_sound,30,(int)sprite->x, (int)sprite->y,
				                  int(25050-sprite->weight*3000),true);

						//Particles_New(	PARTICLE_DUST_CLOUDS,sprite->x+rand()%5-rand()%5-10,sprite_bottom+rand()%3-rand()%3,
						//			  0,-0.2,rand()%50+20,0,0);

						if (rand()%7 == 1) {
							Particles_New(PARTICLE_SMOKE,sprite->x+rand()%5-rand()%5-10,sprite_bottom+rand()%3-rand()%3,
											0.3,-0.1,450,0,0);
							Particles_New(PARTICLE_SMOKE,sprite->x+rand()%5-rand()%5-10,sprite_bottom+rand()%3-rand()%3,
											-0.3,-0.1,450,0,0);
						}
					}

					if (sprite->weight > 1) {
						Game->vibrate(34 + int(sprite->weight * 20));
					}
				}

				sprite->jump_timer = 0;
			}

			sprite->b = 0;
		}

	/*****************************************************************************************/
	/* Set correct values                                                                    */
	/*****************************************************************************************/

	if (sprite->b > 4.0)
		sprite->b = 4.0;

	if (sprite->b < -4.0)
		sprite->b = -4.0;

	//Limit speed 2
	if (sprite->a > max_speed)
		sprite->a = max_speed;

	if (sprite->a < -max_speed)
		sprite->a = -max_speed;

	if (sprite->energy < 0)
		sprite->energy = 0;

	if (sprite->damage_timer == 0 || sprite->isPlayer()) {
		sprite->x += sprite->a;
		sprite->y += sprite->b;
	}

	if (sprite->player_c == 1 || sprite->energy < 1) {
		if (!sprite->can_move_down){
			sprite->a /= sector->slipperiness_factor;
		}			
		else{
			sprite->a /= 1.03;//1.02//1.05
		}			

		sprite->b /= 1.25;
	}


	if (sprite->jump_timer < 0)
		sprite->can_move_down = false;

	/*****************************************************************************************/
	/* Set game area to sprite                                                               */
	/*****************************************************************************************/

	if (sprite->x < 0){
		sprite->x = 0;
		sprite->can_move_left = false;
	}		

	if (sprite->y < -sprite_height){
		sprite->y = -sprite_height;
		sprite->can_move_up = false;
	}		

	if (sprite->x > sector->getWidth()*32){
		sprite->can_move_right = false;
		sprite->x = sector->getWidth()*32;
	}
		

	// If the sprite falls under the lower edge of the level
	if (sprite->y > sector->getHeight()*32 + sprite_height) {

		sprite->y = sector->getHeight() *32 + sprite_height;
		sprite->energy = 0;
		sprite->damage_taken_type = DAMAGE_SELF_DESTRUCTION;
		sprite->removed = true;

		if (sprite->weight_button >= 1){
			Game->vibrate(50);
		}
	}


	/*****************************************************************************************/
	/* AI                                                                                    */
	/*****************************************************************************************/
	
	if (!sprite->isPlayer()) {

		for(std::size_t i=0; i<sprite->prototype->AI_f.size(); ++i){
			const SpriteAI::AI_Class&ai = sprite->prototype->AI_f[i];
			if(!ai.apply_to_creatures)continue;
			
			if(ai.trigger==AI_TRIGGER_ANYWAY){
				ai.func(sprite);
			}
			else if(sprite->energy>0 && ai.trigger==AI_TRIGGER_ALIVE){
				ai.func(sprite);
			}
		}

		/*for(SpriteAI::AI_Class& ai : sprite->prototype->AI_f){
			if(!ai.apply_to_creatures)continue;
			
			if(ai.trigger==AI_TRIGGER_ANYWAY){
				ai.func(sprite);
			}
			else if(sprite->energy>0 && ai.trigger==AI_TRIGGER_ALIVE){
				ai.func(sprite);
			}
		}*/
	}

	if (sprite->energy > sprite->prototype->energy)
		sprite->energy = sprite->prototype->energy;

	//if (kaiku == 1 && sprite->prototype->prototype == TYPE_PROJECTILE && sprite->prototype->damage_type == DAMAGE_NOISE &&
	//	sprite->prototype->sounds[SOUND_ATTACK1] > -1)
	//	Play_GameSFX(sprite->prototype->sounds[SOUND_ATTACK1],20, (int)sprite->x, (int)sprite->y,
	//				  sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);


	

	if (sprite->a > max_speed)
		sprite->a = max_speed;

	if (sprite->a < -max_speed)
		sprite->a = -max_speed;


	/*****************************************************************************************/
	/* Attacks 1 and 2                                                                       */
	/*****************************************************************************************/

	/**
	 * @brief 
	 * Some old projectile spri
	 */

	if(sprite->self_destruction && sprite->prototype->legacy_projectile){

		if(sprite->ammo1!=nullptr){
			Play_GameSFX(sprite->prototype->sounds[SOUND_ATTACK1],100, (int)sprite->x, (int)sprite->y,
						  sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);
			
			sector->sprites.addProjectileSprite(sprite->ammo1,sprite, sprite->prototype->ammo1_offset);

			/**
			 * To prevent duplication of legacy projectiles
			 */

			sprite->ammo1 = nullptr;
			SpriteOnDeath(sprite);
		}
		if(sprite->ammo2!=nullptr){
			Play_GameSFX(sprite->prototype->sounds[SOUND_ATTACK2],100, (int)sprite->x, (int)sprite->y,
						  sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);
			
			sector->sprites.addProjectileSprite(sprite->ammo2,sprite, sprite->prototype->ammo2_offset);

			/**
			 * To prevent duplication of legacy projectiles
			 */
			sprite->ammo2 = nullptr;
			SpriteOnDeath(sprite);
		}
	}

	/**
	 * @brief 
	 * Normal attack1, attack2
	 */
	// If the sprite is ready and isn't crouching
	else if (sprite->charging_timer == 0  && !sprite->crouched ) {
		// the attack has just started
		if (sprite->attack1_timer == sprite->prototype->attack1_time) {

			if(sprite->energy>0){
				// provides recovery time, after which the sprite can attack again
				sprite->charging_timer = sprite->prototype->charge_time;
				if(sprite->charging_timer == 0) sprite->charging_timer = 5;
			}
			else{
				sprite->charging_timer = -1;
			}			
			// if you don't have your own charging time ...
			if (sprite->ammo1 != nullptr && sprite->prototype->charge_time == 0)
			// ... and the projectile has, take charge_time from the projectile
				if (sprite->ammo1->projectile_charge_time > 0)
					sprite->charging_timer = sprite->ammo1->projectile_charge_time;

			// attack sound
			Play_GameSFX(sprite->prototype->sounds[SOUND_ATTACK1],100, (int)sprite->x, (int)sprite->y,
						  sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);

			if (sprite->ammo1 != nullptr) {
				sector->sprites.addProjectileSprite(sprite->ammo1, sprite, sprite->prototype->ammo1_offset);
		//		if (Level_Prototypes_List[sprite->ammo1].sounds[SOUND_ATTACK1] > -1)
		//			Play_GameSFX(Level_Prototypes_List[sprite->ammo1].sounds[SOUND_ATTACK1],100, (int)sprite->x, (int)sprite->y,
		//						  sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);
			}
		}

		// Same as attack 1
		if (sprite->attack2_timer == sprite->prototype->attack2_time) {
			
			if(sprite->energy>0){
				// provides recovery time, after which the sprite can attack again
				sprite->charging_timer = sprite->prototype->charge_time;
				if(sprite->charging_timer == 0) sprite->charging_timer = 5;
			}
			else{
				sprite->charging_timer = -1;
			}

			if (sprite->ammo2 != nullptr && sprite->prototype->charge_time == 0)
				if (sprite->ammo2->projectile_charge_time > 0)
					sprite->charging_timer = sprite->ammo2->projectile_charge_time;

			Play_GameSFX(sprite->prototype->sounds[SOUND_ATTACK2],100,(int)sprite->x, (int)sprite->y,
						  sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);

			if (sprite->ammo2 != nullptr) {
				sector->sprites.addProjectileSprite(sprite->ammo2, sprite, sprite->prototype->ammo2_offset);

		//		if (Level_Prototypes_List[sprite->ammo2].sounds[SOUND_ATTACK1] > -1)
		//			Play_GameSFX(Level_Prototypes_List[sprite->ammo2].sounds[SOUND_ATTACK1],100, (int)sprite->x, (int)sprite->y,
		//						  sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);

			}
		}
	}

	// Random sounds
	if (sprite->prototype->sounds[SOUND_RANDOM] != -1 && rand()%200 == 1 && sprite->energy > 0)
		Play_GameSFX(sprite->prototype->sounds[SOUND_RANDOM],80,(int)sprite->x, (int)sprite->y,
					  sprite->prototype->sound_frequency, sprite->prototype->random_sound_frequency);


	/**
	 * If the checkpoint touches player
	 */
	if(sprite->prototype->type==TYPE_CHECKPOINT && Game->lastCheckpoint != sprite && sprite->energy>0){

		double sprite2_yla = Player_Sprite->crouched ? Player_Sprite->prototype->height /3 : 0;


		if (sprite->x <= Player_Sprite->x + Player_Sprite->prototype->width /2 &&
				sprite->x >= Player_Sprite->x - Player_Sprite->prototype->width /2 &&
				sprite->y/*top*/ <= Player_Sprite->y + Player_Sprite->prototype->height/2 &&
				sprite->y/*bottom*/ >= Player_Sprite->y - Player_Sprite->prototype->height/2 + sprite2_yla){


			if(Game->score > Episode->checkpointPenalty){
				sprite->target_sprite = Player_Sprite;
				Game->lastCheckpoint = sprite;
				sprite->attack1_timer = sprite->prototype->attack1_time;

				Game->saveGameState();
				Game->showInfo("A new checkpoint!");
			}
			else{
				Game->showInfo("Too low score!");
			}			
		}
	}
	sprite->initial_update = false;
}

void UpdateBonusSprite(SpriteClass* sprite){

	LevelSector * sector = sprite->level_sector;

	sprite_width  = sprite->prototype->width;
	sprite_height = sprite->prototype->height;

	sprite_left = sprite->x - sprite_width  / 2;
	sprite_right = sprite->x + sprite_width  / 2;
	sprite_upper   = sprite->y - sprite_height / 2;
	sprite_bottom   = sprite->y + sprite_height / 2;

	sprite->can_move_right	= true,
	sprite->can_move_left	= true,
	sprite->can_move_up		= true,
	sprite->can_move_down		= true;

	if (sprite->x < 0){
		sprite->x = 0;
		sprite->can_move_left = false;
	}		

	if (sprite->y < -sprite_height){
		sprite->y = -sprite_height;
		sprite->can_move_up = false;
	}		

	if (sprite->x > sector->getWidth()*32){
		sprite->can_move_right = false;
		sprite->x = sector->getWidth()*32;
	}
		

	// If the sprite falls under the lower edge of the level
	if (sprite->y > sector->getHeight()*32 + sprite_height) {

		sprite->y = sector->getHeight()*32 + sprite_height;
		sprite->energy = 0;
		sprite->removed = true;
	}

	bool in_water = sprite->in_water;
	if (sprite->damage_timer > 0)
		sprite->damage_timer--;

	if (sprite->charging_timer > 0)
		sprite->charging_timer--;

	if (sprite->mutation_timer > 0)	// aika muutokseen
		sprite->mutation_timer --;

	// Hyppyyn liittyv�t seikat

	if (Game->button_vibration + Game->getVibration() > 0 && sprite->jump_timer == 0)
		sprite->jump_timer = sprite->prototype->max_jump / 2;

	if (sprite->jump_timer > 0 && sprite->jump_timer < sprite->prototype->max_jump)
	{
		sprite->jump_timer ++;
		sprite->b = (sprite->prototype->max_jump - sprite->jump_timer)/-4.0;//-2
	}

	if (sprite->b > 0)
		sprite->jump_timer = sprite->prototype->max_jump;



	if (sprite->weight != 0 || sprite->prototype->dead_weight.has_value())	// jos bonuksella on weight, tutkitaan ymp�rist�
	{
		// o
		//
		// |  Gravity
		// V

		sprite->b += sprite->weight + sprite->b/1.25;

		if (sprite->in_water)
		{
			if (sprite->b > 0)
				sprite->b /= 2.0;

			if (rand()%80 == 1)
				Particles_New(PARTICLE_SPARK,sprite->x-4,sprite->y,0,-0.5-rand()%2,rand()%30+30,0,
				sector->splash_color);
		}

		sprite->in_water = false;

		sprite->weight_button = sprite->weight;

		/* TOISET SPRITET */

		PK2BLOCK spritepalikka; 

		for (SpriteClass* sprite2 : sector->sprites.Sprites_List) {
			if (sprite2 != sprite && !sprite2->removed) {
				if (sprite2->prototype->is_wall && sprite->prototype->check_tiles && sprite2->energy>0) {
					if (sprite->x-sprite_width/2 +sprite->a <= sprite2->x + sprite2->prototype->width /2 &&
						sprite->x+sprite_width/2 +sprite->a >= sprite2->x - sprite2->prototype->width /2 &&
						sprite->y-sprite_height/2+sprite->b <= sprite2->y + sprite2->prototype->height/2 &&
						sprite->y+sprite_height/2+sprite->b >= sprite2->y - sprite2->prototype->height/2)
					{
						spritepalikka.id = 0;
						spritepalikka.bottom   = (int)sprite2->y + sprite2->prototype->height/2;
						spritepalikka.right = (int)sprite2->x + sprite2->prototype->width/2;
						spritepalikka.left = (int)sprite2->x - sprite2->prototype->width/2;
						spritepalikka.top   = (int)sprite2->y - sprite2->prototype->height/2;

						spritepalikka.bottom_side       = BLOCK_WALL;
						spritepalikka.top_side       = BLOCK_WALL;
						spritepalikka.right_side   = BLOCK_WALL;
						spritepalikka.left_side = BLOCK_WALL;

						if (!sprite2->prototype->is_wall_down)
							spritepalikka.bottom_side		 = BLOCK_BACKGROUND;
						if (!sprite2->prototype->is_wall_up)
							spritepalikka.top_side		 = BLOCK_BACKGROUND;
						if (!sprite2->prototype->is_wall_right)
							spritepalikka.right_side   = BLOCK_BACKGROUND;
						if (!sprite2->prototype->is_wall_left)
							spritepalikka.left_side = BLOCK_BACKGROUND;


						spritepalikka.water  = false;

						// Bonus accepts just walls?
						/*spritepalikka.right_side   = BLOCK_WALL;
						spritepalikka.left_side = BLOCK_WALL;
						spritepalikka.top_side       = BLOCK_WALL;
						spritepalikka.bottom_side       = BLOCK_WALL;*/

						Check_SpriteBlock(sprite, spritepalikka); //Colision bonus and sprite block
					}
				}

				if (sprite->x < sprite2->x + sprite2->prototype->width/2 &&
					sprite->x > sprite2->x - sprite2->prototype->width/2 &&
					sprite->y < sprite2->y + sprite2->prototype->height/2 &&
					sprite->y > sprite2->y - sprite2->prototype->height/2 &&
					sprite->damage_timer == 0)
				{

					if(sprite2->prototype->type==TYPE_BLACK_HOLE){
						sprite->removed = true;
						sprite->energy = 0;
					}


					else if(sprite2->can_collect_bonuses &&
					!sprite->prototype->indestructible &&
					sprite2->energy > 0 &&
					sprite->energy > 0){
						BonusSpriteCollected(sprite, sprite2);
					}

					//Character sprite pushing a bonus
					else if (sprite2->prototype->type != TYPE_BONUS && (!sprite2->prototype->ambient || sprite2->player_c!=0) ){
						sprite->a += sprite2->a*(rand()%4);
					}

					// lis�t��n spriten painoon sit� koskettavan toisen spriten weight
					sprite->weight_button += sprite2->prototype->weight;

					// samanmerkkiset spritet vaihtavat suuntaa t�rm�tess��n
					if (sprite->prototype == sprite2->prototype &&
						sprite2->energy > 0)
					{
						if (sprite->x < sprite2->x) {
							sprite2->a += sprite->a / 3.0;
							sprite->can_move_right = false;
						}
						if (sprite->x > sprite2->x) {
							sprite2->a += sprite->a / 3.0;
							sprite->can_move_left = false;
						}
					}

				}
			}
		}

		// Tarkistetaan ettei menn� mihink��n suuntaan liian kovaa.

		if (sprite->b > 4)
			sprite->b = 4;

		if (sprite->b < -4)
			sprite->b = -4;

		if (sprite->a > 3)
			sprite->a = 3;

		if (sprite->a < -3)
			sprite->a = -3;

		// Lasketaan

		if (sprite->prototype->check_tiles)
		{

			int palikat_x_lkm = (int)((sprite_width) /32)+4;
			int palikat_y_lkm = (int)((sprite_height)/32)+4;

			int map_vasen = (int)(sprite_left)/32;
			int map_yla   = (int)(sprite_upper)/32;

			// Tutkitaan t�rm��k� palikkaan

			for (int y = 0; y < palikat_y_lkm; y++)
				for (int x = 0; x < palikat_x_lkm; x++){
					PK2BLOCK block = sector->getBlock(map_vasen+x-1,map_yla+y-1, Game->level.block_types);
					//Block_Get(map_vasen+x-1,map_yla+y-1);
					Check_MapBlock(sprite, block);
				}

		}

		if (in_water != sprite->in_water && !sprite->initial_update) {
			Effect_Splash((int)sprite->x,(int)sprite->y, sector->splash_color);
			Play_GameSFX(Episode->sfx.splash_sound, 100, (int)sprite->x, (int)sprite->y, SOUND_SAMPLERATE, true);
		}


		if (!sprite->can_move_right)
		{
			if (sprite->a > 0)
				sprite->a = -sprite->a/1.5;
		}

		if (!sprite->can_move_left)
		{
			if (sprite->a < 0)
				sprite->a = -sprite->a/1.5;
		}

		if (!sprite->can_move_up)
		{
			if (sprite->b < 0)
				sprite->b = 0;

			sprite->jump_timer = sprite->prototype->max_jump;
		}

		if (!sprite->can_move_down)
		{
			if (sprite->b >= 0)
			{
				if (sprite->jump_timer > 0)
				{
					sprite->jump_timer = 0;
				//	if (/*sprite->b == 4*/!maassa)
				//		Play_GameSFX(pump_sound,20,(int)sprite->x, (int)sprite->y,
				//				      int(25050-sprite->prototype->weight*4000),true);
				}

				if (sprite->b > 2)
					sprite->b = -sprite->b/(3+rand()%2);
				else
					sprite->b = 0;
			}
			//sprite->a /= kitka;
			sprite->a /= 1.07;
		}
		else
		{
			sprite->a /= 1.02;
		}

		sprite->b /= 1.5;

		if (sprite->b > 4)
			sprite->b = 4;

		if (sprite->b < -4)
			sprite->b = -4;

		if (sprite->a > 4)
			sprite->a = 4;

		if (sprite->a < -4)
			sprite->a = -4;

		sprite->x += sprite->a;
		sprite->y += sprite->b;

	}
	else	// jos spriten weight on nolla, tehd��n spritest� "kelluva"
	{
		sprite->y = sprite->orig_y + cos_table(degree+(sprite->orig_x+sprite->orig_y)) / 3.0;
		// Test if player touches bonus

		if(sprite->energy>0 &&
		!sprite->prototype->indestructible &&
		sprite->damage_timer == 0){
			for(SpriteClass* sprite2: sector->sprites.Sprites_List){
				if (sprite2 != sprite &&
					sprite2->can_collect_bonuses &&
					sprite2->energy > 0 &&
					sprite->x < sprite2->x + sprite2->prototype->width/2 &&
					sprite->x > sprite2->x - sprite2->prototype->width/2 &&
					sprite->y < sprite2->y + sprite2->prototype->height/2 &&
					sprite->y > sprite2->y - sprite2->prototype->height/2){
						BonusSpriteCollected(sprite, sprite2);
					}
			}


		}
	}
	sprite->weight = sprite->initial_weight;
	
	for(const SpriteAI::AI_Class& ai: sprite->prototype->AI_f){

		if(!ai.apply_to_bonuses)continue;

		if((sprite->energy>0 && ai.trigger==AI_TRIGGER_ALIVE)
		||  ai.trigger==AI_TRIGGER_ANYWAY){
			ai.func(sprite);
		}
	}

	/**
	 * @brief 
	 * To allow self destruction
	 */
	if(sprite->damage_taken_type == DAMAGE_SELF_DESTRUCTION){
		sprite->damage_taken = 0;
		sprite->damage_taken_type = DAMAGE_NONE;
		sprite->energy = 0;
		SpriteOnDeath(sprite);
	}

	/* The energy doesn't matter that the player is a bonus item */
	if (sprite->isPlayer())
		sprite->energy = 0;

	sprite->initial_update = false;
}

void UpdateBackgroundSprite(SpriteClass* sprite, double &yl){
	
	for(const SpriteAI::AI_Class& ai: sprite->prototype->AI_f){
		if(!ai.apply_to_backgrounds)continue;

		if((ai.trigger == AI_TRIGGER_ALIVE && sprite->energy>0)||ai.trigger == AI_TRIGGER_ANYWAY){

			if(ai.func==nullptr){
				switch (ai.id)
				{
				case AI_BACKGROUND_MOON:
					yl += screen_height/3+50;
					break;
				case AI_BACKGROUND_HORIZONTAL_PARALLAX:
					yl = 0;
					break;
				default:
					break;
				}
			}
			else{
				ai.func(sprite);
			}
		}
	}
	
	if (sprite->attack1_timer > 0) // for BG teleporters
		sprite->attack1_timer--;

	if (sprite->charging_timer > 0)
		sprite->charging_timer--;

	if (sprite->mutation_timer > 0)	// aika muutokseen
		sprite->mutation_timer --;

	/**
	 * @brief 
	 * To allow self destruction
	 */
	if(sprite->damage_taken_type == DAMAGE_SELF_DESTRUCTION){
		sprite->damage_taken = 0;
		sprite->damage_taken_type = DAMAGE_NONE;
		sprite->energy = 0;
		SpriteOnDeath(sprite);
	}
}




// void PlayerRespawnOnCheckpoint(SpriteClass* player, SpriteClass* checkPoint){
// 	player->energy = player->prototype->energy;

// 	if(player->energy<=0){
// 		player->energy = 10;
// 	}

// 	player->removed = false;
// 	double x = checkPoint->x;
// 	double y = checkPoint->y;
// 	std::optional<Point2D>& offset = checkPoint->prototype->ammo1_offset;

// 	if(offset.has_value()){

// 		double off_x = -checkPoint -> prototype->width/2 + offset->x;
// 		double off_y = -checkPoint -> prototype->height/2 + offset->y;

// 		if(checkPoint->flip_x){
// 			off_x = -off_x;
// 		}
// 		if(checkPoint->flip_y){
// 			off_y = -off_y;
// 		}

// 		x += off_x;
// 		y += off_y;
// 	}

// 	Game->teleportPlayer(checkPoint->x, checkPoint->y, checkPoint->level_sector);

// 	checkPoint->attack2_timer = checkPoint->prototype->attack2_time;

// 	checkPoint->target_sprite = player;

// 	/**
// 	 * Score penalty for respawning on checkpoint
// 	 */

// 	int score = checkPoint->prototype->score;
// 	if (score != 0){
// 		if(score>0){
// 			score = -score;
// 		}
// 		Fadetext_New(fontti2,std::to_string(score),(int)checkPoint->x-8,(int)checkPoint->y-8,80);
// 		Game->score += score;

		
// 	}
// }