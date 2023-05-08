//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "game/physics.hpp"

#include "game/game.hpp"
#include "game/sprites.hpp"
#include "game/gifts.hpp"
#include "settings.hpp"
#include "gfx/particles.hpp"
#include "gfx/effect.hpp"
#include "gfx/text.hpp"
#include "language.hpp"
#include "sfx.hpp"
#include "system.hpp"
#include "episode/episodeclass.hpp"
#include "gui.hpp"

#include "engine/types.hpp"
#include "engine/PInput.hpp"
#include "engine/PSound.hpp"

#include <cstring>

static double sprite_x;
static double sprite_y;
static double sprite_a;
static double sprite_b;

static double sprite_vasen;
static double sprite_oikea;
static double sprite_yla;
static double sprite_ala;

static int sprite_leveys;
static int sprite_korkeus;

static bool oikealle;
static bool vasemmalle;
static bool ylos;
static bool alas;

static bool vedessa;

static double max_nopeus;

static PK2BLOCK Block_Get(u32 x, u32 y) {

	PK2BLOCK block;
	//memset(&block, 0, sizeof(block));

	// Outside the screen
	if (x >= PK2MAP_MAP_WIDTH || y >= PK2MAP_MAP_HEIGHT) {
		
		block.koodi  = 255;
		block.tausta = true;
		block.vasen  = x*32;
		block.oikea  = x*32 + 32;
		block.yla    = y*32;
		block.ala    = y*32 + 32;
		block.water  = false;
		block.border = true;

		block.vasemmalle = 0;
		block.oikealle = 0;
		block.ylos = 0;
		block.alas = 0;

		return block;

	}

	u8 i = Game->map.seinat[x+y*PK2MAP_MAP_WIDTH];

	if (i<150) { //If it is ground

		block        = Game->lasketut_palikat[i];
		block.vasen  = x*32+Game->lasketut_palikat[i].vasen;
		block.oikea  = x*32+32+Game->lasketut_palikat[i].oikea;
		block.yla    = y*32+Game->lasketut_palikat[i].yla;
		block.ala    = y*32+32+Game->lasketut_palikat[i].ala;

	} else { //If it is sky - Need to reset
	
		block.koodi  = 255;
		block.tausta = true;
		block.vasen  = x*32;
		block.oikea  = x*32 + 32;
		block.yla    = y*32;
		block.ala    = y*32 + 32;
		block.water  = false;

		block.vasemmalle = 0;
		block.oikealle = 0;
		block.ylos = 0;
		block.alas = 0;
	
	}

	i = Game->map.taustat[x+y*PK2MAP_MAP_WIDTH];

	if (i > 131 && i < 140)
		block.water = true;

	block.border = Game->map.reunat[x+y*PK2MAP_MAP_WIDTH];

	return block;
}

static void Check_SpriteBlock(SpriteClass* sprite, PK2BLOCK &palikka) {

	//left and right
	if (sprite_yla < palikka.ala && sprite_ala-1 > palikka.yla){
		if (sprite_oikea+sprite_a-1 > palikka.vasen && sprite_vasen+sprite_a < palikka.oikea){
			// Tutkitaan onko sprite menossa oikeanpuoleisen palikan sis��n.
			if (sprite_oikea+sprite_a < palikka.oikea){
				// Onko palikka sein�?
				if (palikka.oikealle == BLOCK_WALL){
					oikealle = false;
					if (palikka.koodi == BLOCK_LIFT_HORI)
						sprite_x = palikka.vasen - sprite_leveys/2;
				}
			}

			if (sprite_vasen+sprite_a > palikka.vasen){
				if (palikka.vasemmalle == BLOCK_WALL){
					vasemmalle = false;

					if (palikka.koodi == BLOCK_LIFT_HORI)
						sprite_x = palikka.oikea + sprite_leveys/2;

				}
			}
		}
	}

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;

	//ceil and floor

	if (sprite_vasen < palikka.oikea && sprite_oikea-1 > palikka.vasen){
		if (sprite_ala+sprite_b-1 >= palikka.yla && sprite_yla+sprite_b <= palikka.ala){
			if (sprite_ala+sprite_b-1 <= palikka.ala){
				if (palikka.alas == BLOCK_WALL){
					alas = false;

					if (palikka.koodi == BLOCK_LIFT_VERT)
						sprite_y = palikka.yla - sprite_korkeus /2;

					if (sprite_ala-1 >= palikka.yla && sprite_b >= 0)
						if (palikka.koodi != BLOCK_LIFT_HORI)
							sprite_y = palikka.yla - sprite_korkeus /2;
				}
			}

			if (sprite_yla+sprite_b > palikka.yla){
				if (palikka.ylos == BLOCK_WALL){
					ylos = false;

					if (sprite_yla < palikka.ala)
						if (palikka.koodi != BLOCK_LIFT_HORI)
							sprite->crouched = true;
				}
			}
		}
	}
}

static void Check_MapBlock(SpriteClass* sprite, PK2BLOCK &palikka) {

	//If sprite is in the block
	if (sprite_x <= palikka.oikea && sprite_x >= palikka.vasen && sprite_y <= palikka.ala && sprite_y >= palikka.yla){

		/**********************************************************************/
		/* Examine if block is water background                               */
		/**********************************************************************/
		if (palikka.water)
			sprite->vedessa = true;
		else
			sprite->vedessa = false;

		/**********************************************************************/
		/* Examine if it touches the fire                                     */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_FIRE && Game->button1 == 0 && sprite->damage_timer == 0){
			sprite->saatu_vahinko = 2;
			sprite->saatu_vahinko_tyyppi = DAMAGE_FIRE;
		}

		/**********************************************************************/
		/* Examine if block is hideway (unused)                               */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_HIDEOUT)
			sprite->piilossa = true;
		else
			sprite->piilossa = false;
		

		/**********************************************************************/
		/* Examine if block is the exit                                       */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_EXIT) {
			if ((!Game->chick_mode && sprite->pelaaja != 0) || sprite->Onko_AI(AI_CHICK))
				Game->Finnish();
		}
	}

	//If sprite is thouching the block
	if (sprite_vasen <= palikka.oikea-4 && sprite_oikea >= palikka.vasen+4 && sprite_yla <= palikka.ala && sprite_ala >= palikka.yla+16){
		/**********************************************************************/
		/* Examine if it touches the fire                                     */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_FIRE && Game->button1 == 0 && sprite->damage_timer == 0){
			sprite->saatu_vahinko = 2;
			sprite->saatu_vahinko_tyyppi = DAMAGE_FIRE;
		}
	}

	//Examine if there is a block on bottom
	if ((palikka.koodi<80 || palikka.koodi>139) && palikka.koodi != BLOCK_BARRIER_DOWN && palikka.koodi < 150){
		int mask_index = (int)(sprite_x+sprite_a) - palikka.vasen;

		if (mask_index < 0)
			mask_index = 0;

		if (mask_index > 31)
			mask_index = 31;

		palikka.yla += Game->palikkamaskit[palikka.koodi].alas[mask_index];

		if (palikka.yla >= palikka.ala-2)
			palikka.alas = BLOCK_BACKGROUND;

		palikka.ala -= Game->palikkamaskit[palikka.koodi].ylos[mask_index];
	}

	//If sprite is thouching the block (again?)
	if (sprite_vasen <= palikka.oikea+2 && sprite_oikea >= palikka.vasen-2 && sprite_yla <= palikka.ala && sprite_ala >= palikka.yla){
		/**********************************************************************/
		/* Examine if it is a key and touches lock wall                       */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_LOCK && sprite->tyyppi->avain){
			Game->map.seinat[palikka.vasen/32+(palikka.yla/32)*PK2MAP_MAP_WIDTH] = 255;
			Game->map.Calculate_Edges();

			sprite->piilota = true;

			if (sprite->tyyppi->tuhoutuminen != FX_DESTRUCT_EI_TUHOUDU) {
				Game->keys--;
				if (Game->keys < 1)
					Game->Open_Locks();
			}

			Effect_Explosion(palikka.vasen+16, palikka.yla+10, 0);
			Play_GameSFX(open_locks_sound,100, (int)sprite_x, (int)sprite_y, SOUND_SAMPLERATE, false);
		}

		/**********************************************************************/
		/* Make wind effects                                                  */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_DRIFT_LEFT && vasemmalle)
			sprite_a -= 0.02;

		if (palikka.koodi == BLOCK_DRIFT_RIGHT && oikealle)
			sprite_a += 0.02;	//0.05

		/*********************************************************************/
		/* Examine if sprite is on the border to fall                        */
		/*********************************************************************/
		if (palikka.border && sprite->jump_timer <= 0 && sprite_y < palikka.ala && sprite_y > palikka.yla){
			/* && sprite_ala <= palikka.ala+2)*/ // onko sprite tullut borderlle
			if (sprite_vasen > palikka.vasen)
				sprite->reuna_vasemmalla = true;

			if (sprite_oikea < palikka.oikea)
				sprite->reuna_oikealla = true;
		}
	}

	//Examine walls on left and right

	if (sprite_yla < palikka.ala && sprite_ala-1 > palikka.yla) {
		if (sprite_oikea+sprite_a-1 > palikka.vasen && sprite_vasen+sprite_a < palikka.oikea) {
			// Examine whether the sprite going in the right side of the block.
			if (sprite_oikea+sprite_a < palikka.oikea) {
				// Onko palikka sein�?
				if (palikka.oikealle == BLOCK_WALL) {
					oikealle = false;

					if (palikka.koodi == BLOCK_LIFT_HORI)
						sprite_x = palikka.vasen - sprite_leveys/2;
				}
			}
			// Examine whether the sprite going in the left side of the block.
			if (sprite_vasen+sprite_a > palikka.vasen) {
				if (palikka.vasemmalle == BLOCK_WALL) {
					vasemmalle = false;

					if (palikka.koodi == BLOCK_LIFT_HORI)
						sprite_x = palikka.oikea + sprite_leveys/2;

				}
			}
		}
	}

	sprite_vasen = sprite_x - sprite_leveys/2;
	sprite_oikea = sprite_x + sprite_leveys/2;

	//Examine walls on up and down

	if (sprite_vasen < palikka.oikea && sprite_oikea-1 > palikka.vasen) { //Remove the left and right blocks
		if (sprite_ala+sprite_b-1 >= palikka.yla && sprite_yla+sprite_b <= palikka.ala) { //Get the up and down blocks
			if (sprite_ala+sprite_b-1 <= palikka.ala) { //Just in the sprite's foot
				if (palikka.alas == BLOCK_WALL) { //If it is a wall
					alas = false;
					if (palikka.koodi == BLOCK_LIFT_VERT)
						sprite_y = palikka.yla - sprite_korkeus /2;

					if (sprite_ala-1 >= palikka.yla && sprite_b >= 0) {
						//sprite_y -= sprite_ala - palikka.yla;
						if (palikka.koodi != BLOCK_LIFT_HORI) {
							sprite_y = palikka.yla - sprite_korkeus /2;
						}
					}

					if (sprite->kytkinpaino >= 1) { // Sprite can press the buttons
						if (palikka.koodi == BLOCK_BUTTON1 && Game->button1 == 0) {
							Game->button1 = Game->map.button1_time;
							Game->button_vibration = 64;
							Play_GameSFX(switch_sound, 100, (int)sprite_x, (int)sprite_y, SOUND_SAMPLERATE, false);
							PInput::Vibrate(1000);
						}

						if (palikka.koodi == BLOCK_BUTTON2 && Game->button2 == 0) {
							Game->button2 = Game->map.button2_time;
							Game->button_vibration = 64;
							Play_GameSFX(switch_sound, 100, (int)sprite_x, (int)sprite_y, SOUND_SAMPLERATE, false);
							PInput::Vibrate(1000);
						}

						if (palikka.koodi == BLOCK_BUTTON3 && Game->button3 == 0) {
							Game->button3 = Game->map.button3_time;
							Game->button_vibration = 64;
							Play_GameSFX(switch_sound, 100, (int)sprite_x, (int)sprite_y, SOUND_SAMPLERATE, false);
							PInput::Vibrate(1000);
						}
					}

				}
			}

			if (sprite_yla+sprite_b > palikka.yla) {
				if (palikka.ylos == BLOCK_WALL) {
					ylos = false;

					if (sprite_yla < palikka.ala) {
						if (palikka.koodi == BLOCK_LIFT_VERT && sprite->crouched) {
							sprite->saatu_vahinko = 2;
							sprite->saatu_vahinko_tyyppi = DAMAGE_IMPACT;
						}

						if (palikka.koodi != BLOCK_LIFT_HORI) {
							//if (sprite->crouched)
							//	sprite_y = palikka.ala + sprite_korkeus /2;

							sprite->crouched = true;
						}
					}
				}
			}
		}
	}
}

int Sprite_Movement(SpriteClass* sprite){
	
	if (!sprite->tyyppi)
		return -1;

	// Save values
	sprite_x = sprite->x;
	sprite_y = sprite->y;
	sprite_a = sprite->a;
	sprite_b = sprite->b;

	sprite_leveys  = sprite->tyyppi->leveys;
	sprite_korkeus = sprite->tyyppi->korkeus;

	sprite_vasen = sprite_x - sprite_leveys  / 2;
	sprite_oikea = sprite_x + sprite_leveys  / 2;
	sprite_yla   = sprite_y - sprite_korkeus / 2;
	sprite_ala   = sprite_y + sprite_korkeus / 2;

	max_nopeus = sprite->tyyppi->max_nopeus;

	vedessa = sprite->vedessa;

	oikealle	 = true,
	vasemmalle	 = true,
	ylos		 = true,
	alas		 = true;

	sprite->crouched = false;

	sprite->reuna_vasemmalla = false;
	sprite->reuna_oikealla = false;


	/* Pistet��n vauhtia tainnutettuihin spriteihin */
	if (sprite->energia < 1)
		max_nopeus = 3;

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

	bool swimming = sprite->vedessa && (sprite->Onko_AI(AI_SWIMMING) || sprite->Onko_AI(AI_MAX_SPEED_SWIMMING));
	bool max_speed = sprite->Onko_AI(AI_MAX_SPEED_PLAYER) ||
		(swimming && sprite->Onko_AI(AI_MAX_SPEED_SWIMMING)) ||
		(sprite->super_mode_timer > 0 && sprite->Onko_AI(AI_MAX_SPEED_PLAYER_ON_SUPER));
	
	/*swimming = sprite->vedessa;
	max_speed = sprite->vedessa;
	sprite->tyyppi->can_swim = true;*/

	if (sprite->pelaaja != 0 && sprite->energia > 0){
		/* SLOW WALK */
		if (PInput::Keydown(Input->walk_slow)
		|| Gui_pad_button == 1 || Gui_pad_button == 3)
			add_speed = false;

		/* ATTACK 1 */
		if ((PInput::Keydown(Input->attack1) || Gui_egg) && sprite->charging_timer == 0 && sprite->ammus1 != nullptr)
			sprite->attack1_timer = sprite->tyyppi->attack1_time;
		/* ATTACK 2 */
		else if ((PInput::Keydown(Input->attack2) || Gui_doodle) && sprite->charging_timer == 0 && sprite->ammus2 != nullptr)
				sprite->attack2_timer = sprite->tyyppi->attack2_time;

		/* CROUCH */
		sprite->crouched = false;
		bool axis_couch = (Input == &Settings.joystick) && (PInput::GetAxis(1) > 0.5);
		if ((PInput::Keydown(Input->down) || Gui_down || axis_couch) && !sprite->alas) {
			sprite->crouched = true;
			sprite_yla += sprite_korkeus/1.5;
		}

		/* NAVIGATING*/
		int navigation = 0;

		if (Input == &Settings.joystick)
			navigation = PInput::GetAxis(0) * 100;

		if (Gui_pad_button == 0 || Gui_pad_button == 1)
			navigation = -100;
		else if (Gui_pad_button == 3 || Gui_pad_button == 4)
			navigation = 100;

		if (PInput::Keydown(Input->right))
			navigation = 100;
		
		if (PInput::Keydown(Input->left))
			navigation = -100;

		double a_lisays = 0.04;//0.08;

		if (add_speed) {
			if (rand()%20 == 1 && sprite->animation_index == ANIMATION_WALKING) // Draw dust
				Particles_New(PARTICLE_DUST_CLOUDS,sprite_x-8,sprite_ala-8,0.25,-0.25,40,0,0);

			a_lisays += 0.09;//0.05
		}

		if (sprite->alas)
			a_lisays /= 1.5;//2.0

		a_lisays *= double(navigation) / 100;

		if (max_speed)
			a_lisays *= max_nopeus;
				
		if (navigation > 0)
			sprite->flip_x = false;
		else if (navigation < 0)
			sprite->flip_x = true;

		if (sprite->crouched)	// Slow when couch
			a_lisays /= 10;

		sprite_a += a_lisays;

		/* JUMPING */
		if (sprite->tyyppi->weight > 0 && !swimming) {
			if (PInput::Keydown(Input->jump) || Gui_up) {
				if (!sprite->crouched) {
					if (sprite->jump_timer == 0)
						Play_GameSFX(jump_sound, 100, (int)sprite_x, (int)sprite_y,
									  sprite->tyyppi->aani_frq, sprite->tyyppi->random_frq);

					if (sprite->jump_timer <= 0)
						sprite->jump_timer = 1; //10;
				}
			} else {
				if (sprite->jump_timer > 0 && sprite->jump_timer < 45)
					sprite->jump_timer = 55;
			}

			/* dripping quietly down */
			bool axis_up = (Input == &Settings.joystick) && (PInput::GetAxis(1) < -0.5);
			if ((PInput::Keydown(Input->jump) || Gui_up || axis_up) && sprite->jump_timer >= 150/*90+20*/ &&
				sprite->tyyppi->can_glide)
				gliding = true;
		}
		/* MOVING UP AND DOWN */
		else { // if the player sprite-weight is 0 - like birds

			double speed = 0.15;
			if (max_speed)
				speed *= max_nopeus;

			if (PInput::Keydown(Input->jump) || Gui_up)
				sprite_b -= speed;

			if (PInput::Keydown(Input->down) || Gui_down)
				sprite_b += speed;

			sprite->jump_timer = 0;
		}

		/* AI */
		for (int ai=0;ai < SPRITE_MAX_AI;ai++)
			switch (sprite->tyyppi->AI[ai]){
			
			case AI_CHANGE_WHEN_ENERGY_UNDER_2:
				if (sprite->tyyppi->muutos != nullptr)
					sprite->AI_Change_When_Energy_Under_2(sprite->tyyppi->muutos);
			break;
			
			case AI_CHANGE_WHEN_ENERGY_OVER_1:
			if (sprite->tyyppi->muutos != nullptr)
				if (sprite->AI_Change_When_Energy_Over_1(sprite->tyyppi->muutos)==1)
					Effect_Destruction(FX_DESTRUCT_SAVU_HARMAA, (u32)sprite->x, (u32)sprite->y);
			break;
			
			case AI_MUUTOS_AJASTIN:
				if (sprite->tyyppi->muutos != nullptr)
					sprite->AI_Muutos_Ajastin(sprite->tyyppi->muutos);
			break;
			
			case AI_DAMAGED_BY_WATER:
				sprite->AI_Damaged_by_Water();
			break;
			
			case AI_MUUTOS_JOS_OSUTTU:
				if (sprite->tyyppi->muutos != nullptr)
					sprite->AI_Muutos_Jos_Osuttu(sprite->tyyppi->muutos);
			break;

			default: break;
			}

		/* It is not acceptable that a player is anything other than the game character */
		if (sprite->tyyppi->type != TYPE_GAME_CHARACTER)
			sprite->energia = 0;
	}

	/*****************************************************************************************/
	/* Jump                                                                                  */
	/*****************************************************************************************/

	bool hyppy_maximissa = sprite->jump_timer >= 90;

	// Jos ollaan hyp�tty / ilmassa:
	if (sprite->jump_timer > 0) {
		if (sprite->jump_timer < 50-sprite->tyyppi->max_hyppy)
			sprite->jump_timer = 50-sprite->tyyppi->max_hyppy;

		if (sprite->jump_timer < 10)
			sprite->jump_timer = 10;

		if (!hyppy_maximissa) {
		// sprite_b = (sprite->tyyppi->max_hyppy/2 - sprite->jump_timer/2)/-2.0;//-4
		   sprite_b = -sin_table(sprite->jump_timer)/8;//(sprite->tyyppi->max_hyppy/2 - sprite->jump_timer/2)/-2.5;
			if (sprite_b > sprite->tyyppi->max_hyppy){
				sprite_b = sprite->tyyppi->max_hyppy/10.0;
				sprite->jump_timer = 90 - sprite->jump_timer;
			}

		}

		if (sprite->jump_timer < 180)
			sprite->jump_timer += 2;
	}

	if (sprite->jump_timer < 0)
		sprite->jump_timer++;

	if (sprite_b > 0 && !hyppy_maximissa)
		sprite->jump_timer = 90;//sprite->tyyppi->max_hyppy*2;

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
	
	if (sprite->super_mode_timer > 0) {

		sprite->super_mode_timer--;
		if (Player_Sprite->super_mode_timer == 0)
			PSound::resume_music();
	
	}

	/*****************************************************************************************/
	/* Gravity effect                                                                        */
	/*****************************************************************************************/
	
	if (sprite->weight != 0 && (sprite->jump_timer <= 0 || sprite->jump_timer >= 45) && !swimming)
		sprite_b += sprite->weight/1.25;// + sprite_b/1.5;

	if (gliding && sprite_b > 0) // If gliding
		sprite_b /= 1.3;//1.5;//3

	/*****************************************************************************************/
	/* Speed limits                                                                          */
	/*****************************************************************************************/

	if (sprite_b > 4.0)//4
		sprite_b = 4.0;//4

	if (sprite_b < -4.0)
		sprite_b = -4.0;

	//Limit speed 1
	if (sprite_a > max_nopeus)
		sprite_a = max_nopeus;

	if (sprite_a < -max_nopeus)
		sprite_a = -max_nopeus;

	/*****************************************************************************************/
	/* Blocks colision -                                                                     */
	/*****************************************************************************************/

	if (sprite->tyyppi->tiletarkistus){ //Find the tiles that the sprite occupies

		int palikat_x_lkm = (int)((sprite_leveys) /32)+4; //Number of blocks
		int palikat_y_lkm = (int)((sprite_korkeus)/32)+4;

		PK2BLOCK near_blocks[palikat_x_lkm * palikat_y_lkm];

		int map_vasen = (int)(sprite_vasen) / 32; //Position in tile map
		int map_yla   = (int)(sprite_yla)   / 32;

		for (int y = 0; y < palikat_y_lkm; y++)
			for (int x = 0; x < palikat_x_lkm; x++) //For each block, create a array of blocks around the sprite
				near_blocks[x+(y*palikat_x_lkm)] = Block_Get(map_vasen+x-1,map_yla+y-1); //x = 0, y = 0

		/*****************************************************************************************/
		/* Going through the blocks around the sprite->                                           */
		/*****************************************************************************************/

		//palikat_lkm = palikat_y_lkm*palikat_x_lkm;
		for (int y = 0; y < palikat_y_lkm; y++){
			for (int x = 0; x < palikat_x_lkm; x++) {
				int p = x + y*palikat_x_lkm;
				if ( p < 300 )
					if (!(sprite == Player_Sprite && dev_mode && PInput::Keydown(PInput::Y)))
						Check_MapBlock(sprite, near_blocks[p]);
			}
		}
	}
	/*****************************************************************************************/
	/* If the sprite is under water                                                          */
	/*****************************************************************************************/

	if (sprite->vedessa) {

		if (!sprite->tyyppi->can_swim || sprite->energia < 1) {

			sprite_b /= 2.0;
			sprite_a /= 1.05;

			if (sprite->jump_timer > 0 && sprite->jump_timer < 90)
				sprite->jump_timer--;
		}

		if (rand()%80 == 1)
			Particles_New(PARTICLE_SPARK,sprite_x-4,sprite_y,0,-0.5-rand()%2,rand()%30+30,0,32);
	}

	if (vedessa != sprite->vedessa) { // Sprite comes in or out from water
		Effect_Splash(sprite_x, sprite_y, 32);
		Play_GameSFX(splash_sound, 100, (int)sprite_x, (int)sprite_y, SOUND_SAMPLERATE, true);
	}

	/*****************************************************************************************/
	/* Sprite weight                                                                         */
	/*****************************************************************************************/

	sprite->weight = sprite->initial_weight;
	sprite->kytkinpaino = sprite->weight;

	if (sprite->energia < 1 && sprite->weight == 0) // Fall when is death
		sprite->weight = 1;

	/*****************************************************************************************/
	/* Sprite collision with other sprites                                                   */
	/*****************************************************************************************/

	int tuhoutuminen;
	double sprite2_yla; // kyykistymiseen liittyv�
	PK2BLOCK spritepalikka;

	//Compare this sprite with every sprite in the game
	for (SpriteClass* sprite2 : Sprites_List) {
		if (sprite2 != sprite && sprite2->aktiivinen && !sprite2->piilota) {
			if (sprite2->crouched)
				sprite2_yla = sprite2->tyyppi->korkeus / 3;//1.5;
			else
				sprite2_yla = 0;

			if (sprite2->tyyppi->este && sprite->tyyppi->tiletarkistus) { //If there is a block sprite active

				if (sprite_x-sprite_leveys/2 +sprite_a  <= sprite2->x + sprite2->tyyppi->leveys /2 &&
					sprite_x+sprite_leveys/2 +sprite_a  >= sprite2->x - sprite2->tyyppi->leveys /2 &&
					sprite_y-sprite_korkeus/2+sprite_b <= sprite2->y + sprite2->tyyppi->korkeus/2 &&
					sprite_y+sprite_korkeus/2+sprite_b >= sprite2->y - sprite2->tyyppi->korkeus/2)
				{
					spritepalikka.koodi = 0;
					spritepalikka.ala   = (int)sprite2->y + sprite2->tyyppi->korkeus/2;
					spritepalikka.oikea = (int)sprite2->x + sprite2->tyyppi->leveys/2;
					spritepalikka.vasen = (int)sprite2->x - sprite2->tyyppi->leveys/2;
					spritepalikka.yla   = (int)sprite2->y - sprite2->tyyppi->korkeus/2;

					spritepalikka.water  = false;

					spritepalikka.tausta = false;

					spritepalikka.oikealle   = BLOCK_WALL;
					spritepalikka.vasemmalle = BLOCK_WALL;
					spritepalikka.ylos       = BLOCK_WALL;
					spritepalikka.alas       = BLOCK_WALL;

					if (!sprite->tyyppi->este){
						if (!sprite2->tyyppi->este_alas)
							spritepalikka.alas = BLOCK_BACKGROUND;
						if (!sprite2->tyyppi->este_ylos)
							spritepalikka.ylos = BLOCK_BACKGROUND;
						if (!sprite2->tyyppi->este_oikealle)
							spritepalikka.oikealle = BLOCK_BACKGROUND;
						if (!sprite2->tyyppi->este_vasemmalle)
							spritepalikka.vasemmalle = BLOCK_BACKGROUND;
					}

					if (sprite2->a > 0)
						spritepalikka.koodi = BLOCK_LIFT_HORI;

					if (sprite2->b > 0)
						spritepalikka.koodi = BLOCK_LIFT_VERT;

					if (!(sprite == Player_Sprite && dev_mode && PInput::Keydown(PInput::Y)))
						Check_SpriteBlock(sprite, spritepalikka); //Colision sprite and sprite block
				}
			}

			if (sprite_x <= sprite2->x + sprite2->tyyppi->leveys /2 &&
				sprite_x >= sprite2->x - sprite2->tyyppi->leveys /2 &&
				sprite_y/*yla*/ <= sprite2->y + sprite2->tyyppi->korkeus/2 &&
				sprite_y/*ala*/ >= sprite2->y - sprite2->tyyppi->korkeus/2 + sprite2_yla)
			{
				// sprites with same index change directions when touch
				if (sprite->tyyppi == sprite2->tyyppi &&
					sprite2->energia > 0/* && sprite->pelaaja == 0*/)
				{
					if (sprite->x < sprite2->x)
						oikealle = false;
					if (sprite->x > sprite2->x)
						vasemmalle = false;
					if (sprite->y < sprite2->y)
						alas = false;
					if (sprite->y > sprite2->y)
						ylos = false;
				}

				if (sprite->Onko_AI(AI_NUOLET_VAIKUTTAVAT)) {

					if (sprite2->Onko_AI(AI_NUOLI_RIGHT)) {
						sprite_a = sprite->tyyppi->max_nopeus / 3.5;
						sprite_b = 0;
					}
					else if (sprite2->Onko_AI(AI_NUOLI_LEFT)) {
						sprite_a = sprite->tyyppi->max_nopeus / -3.5;
						sprite_b = 0;
					}

					if (sprite2->Onko_AI(AI_NUOLI_UP)) {
						sprite_b = sprite->tyyppi->max_nopeus / -3.5;
						sprite_a = 0;
					}
					else if (sprite2->Onko_AI(AI_NUOLI_DOWN)) {
						sprite_b = sprite->tyyppi->max_nopeus / 3.5;
						sprite_a = 0;
					}
				}

				/* sprites can exchange information about a player's whereabouts */ //TODO - test this sometime
	/*			if (sprite->seen_player_x != -1 && sprite2->seen_player_x == -1)
				{
					sprite2->seen_player_x = sprite->seen_player_x + rand()%30 - rand()%30;
					sprite->seen_player_x = -1;
				} */

				// If two sprites from different teams touch each other
				if (sprite->vihollinen != sprite2->vihollinen && sprite->emosprite != sprite2) {
					if (sprite2->tyyppi->type != TYPE_BACKGROUND &&
						sprite->tyyppi->type   != TYPE_BACKGROUND &&
						sprite2->tyyppi->type != TYPE_TELEPORT &&
						sprite2->damage_timer == 0 &&
						sprite->damage_timer == 0 &&
						sprite2->energia > 0 &&
						sprite->energia > 0 &&
						sprite2->saatu_vahinko < 1)
					{

						if (sprite->super_mode_timer > 0 && sprite2->super_mode_timer == 0) {
							sprite2->saatu_vahinko = 500;
							sprite2->saatu_vahinko_tyyppi = DAMAGE_ALL;
						}
						if (sprite2->super_mode_timer > 0 && sprite->super_mode_timer == 0) {
							sprite->saatu_vahinko = 500;
							sprite->saatu_vahinko_tyyppi = DAMAGE_ALL;
						}
						
						//Bounce on the sprite head
						if (sprite2->b > 2 && sprite2->weight >= 0.5 &&
							sprite2->y < sprite_y && !sprite->tyyppi->este &&
							sprite->tyyppi->tuhoutuminen != FX_DESTRUCT_EI_TUHOUDU)
						{
							if (sprite2->super_mode_timer)
								sprite->saatu_vahinko = 500;
							else
								sprite->saatu_vahinko = (int)(sprite2->weight+sprite2->b/4);
							sprite->saatu_vahinko_tyyppi = DAMAGE_DROP;
							sprite2->jump_timer = 1;
							if (sprite2->Onko_AI(AI_EGG2)) // Egg bounced, then crack
								sprite2->saatu_vahinko = sprite2->tyyppi->energia;
						}

						// If there is another sprite damaging
						if (sprite->tyyppi->vahinko > 0 && sprite2->tyyppi->type != TYPE_BONUS) {
							
							sprite2->saatu_vahinko        = sprite->tyyppi->vahinko;
							sprite2->saatu_vahinko_tyyppi = sprite->tyyppi->vahinko_tyyppi;
							
							if ( !(sprite2->pelaaja && sprite2->invisible_timer) ) //If sprite2 isn't a invisible player
								sprite->attack1_timer = sprite->tyyppi->attack1_time; //Then sprite attack??

							// The projectiles are shattered by shock
							if (sprite2->tyyppi->type == TYPE_PROJECTILE) {
								sprite->saatu_vahinko = 1;//sprite2->tyyppi->vahinko;
								sprite->saatu_vahinko_tyyppi = sprite2->tyyppi->vahinko_tyyppi;
							}

							if (sprite->tyyppi->type == TYPE_PROJECTILE) {
								sprite->saatu_vahinko = 1;//sprite2->tyyppi->vahinko;
								sprite->saatu_vahinko_tyyppi = sprite2->tyyppi->vahinko_tyyppi;
							}
						}
					}
				}

				// lis�t��n spriten painoon sit� koskettavan toisen spriten weight
				if (sprite->weight > 0)
					sprite->kytkinpaino += sprite2->tyyppi->weight;

			}
		}
	}

	/*****************************************************************************************/
	/* If the sprite has suffered damage                                                     */
	/*****************************************************************************************/
	if (sprite->saatu_vahinko != 0 && sprite->super_mode_timer != 0) {
		sprite->saatu_vahinko = 0;
		sprite->saatu_vahinko_tyyppi = DAMAGE_NONE;
	}

	// If it is invisible, just these damages can injury it
	if (sprite->saatu_vahinko != 0 && sprite->invisible_timer != 0 && 
		sprite->saatu_vahinko_tyyppi != DAMAGE_FIRE &&
		sprite->saatu_vahinko_tyyppi != DAMAGE_COMPRESSION &&
		sprite->saatu_vahinko_tyyppi != DAMAGE_DROP &&
		sprite->saatu_vahinko_tyyppi != DAMAGE_ALL) {
		
		sprite->saatu_vahinko = 0;
		sprite->saatu_vahinko_tyyppi = DAMAGE_NONE;
	}

	if (sprite->saatu_vahinko != 0 && sprite->energia > 0 && sprite->tyyppi->tuhoutuminen != FX_DESTRUCT_EI_TUHOUDU){
		if (sprite->tyyppi->suojaus != sprite->saatu_vahinko_tyyppi || sprite->tyyppi->suojaus == DAMAGE_NONE){
			sprite->energia -= sprite->saatu_vahinko;
			sprite->damage_timer = DAMAGE_TIME;

			if (sprite->saatu_vahinko_tyyppi == DAMAGE_ELECTRIC)
				sprite->damage_timer *= 6;

			Play_GameSFX(sprite->tyyppi->aanet[SOUND_DAMAGE], 100, (int)sprite->x, (int)sprite->y,
						  sprite->tyyppi->aani_frq, sprite->tyyppi->random_frq);

			if (sprite->tyyppi->tuhoutuminen%100 == FX_DESTRUCT_HOYHENET)
				Effect_Destruction(FX_DESTRUCT_HOYHENET, (u32)sprite->x, (u32)sprite->y);

			if (sprite->tyyppi->type != TYPE_PROJECTILE){
				Particles_New(PARTICLE_STAR,sprite_x,sprite_y,-1,-1,60,0.01,128);
				Particles_New(PARTICLE_STAR,sprite_x,sprite_y, 0,-1,60,0.01,128);
				Particles_New(PARTICLE_STAR,sprite_x,sprite_y, 1,-1,60,0.01,128);
			}

			if (sprite->Onko_AI(AI_VAIHDA_KALLOT_JOS_OSUTTU))
				Game->Change_SkullBlocks();

			if (sprite->Onko_AI(AI_ATTACK_1_JOS_OSUTTU)){
				sprite->attack1_timer = sprite->tyyppi->attack1_time;
				sprite->charging_timer = 0;
			}

			if (sprite->Onko_AI(AI_ATTACK_2_JOS_OSUTTU)){
				sprite->attack2_timer = sprite->tyyppi->attack2_time;
				sprite->charging_timer = 0;
			}

		}

		sprite->saatu_vahinko = 0;
		sprite->saatu_vahinko_tyyppi = DAMAGE_NONE;


		/*****************************************************************************************/
		/* If the sprite is destroyed                                                            */
		/*****************************************************************************************/

		if (sprite->energia < 1) {
			tuhoutuminen = sprite->tyyppi->tuhoutuminen;

			if (tuhoutuminen != FX_DESTRUCT_EI_TUHOUDU) {

				if (sprite->Onko_AI(AI_CHICK)) // Killed the chick
					Game->game_over = true;

				if (sprite->tyyppi->bonus != nullptr && sprite->tyyppi->bonusten_lkm > 0)
					if (sprite->tyyppi->bonus_always || rand()%4 == 1)
						for (int bi=0; bi<sprite->tyyppi->bonusten_lkm; bi++)
							Sprites_add(sprite->tyyppi->bonus,0,sprite_x-11+(10-rand()%20),
											  sprite_ala-16-(10+rand()%20), sprite, true);

				if (sprite->Onko_AI(AI_VAIHDA_KALLOT_JOS_TYRMATTY) && !sprite->Onko_AI(AI_VAIHDA_KALLOT_JOS_OSUTTU))
					Game->Change_SkullBlocks();

				if (tuhoutuminen >= FX_DESTRUCT_ANIMAATIO)
					tuhoutuminen -= FX_DESTRUCT_ANIMAATIO;
				else
					sprite->piilota = true;

				Effect_Destruction(tuhoutuminen, (u32)sprite->x, (u32)sprite->y);
				Play_GameSFX(sprite->tyyppi->aanet[SOUND_DESTRUCTION],100, (int)sprite->x, (int)sprite->y,
							  sprite->tyyppi->aani_frq, sprite->tyyppi->random_frq);

				if (sprite->Onko_AI(AI_UUSI_JOS_TUHOUTUU)) {
					Sprites_add(sprite->tyyppi, 0, 
						sprite->alku_x - sprite->tyyppi->leveys,
						sprite->alku_y - sprite->tyyppi->korkeus,
						sprite, false);
				}

				if (sprite->tyyppi->type == TYPE_GAME_CHARACTER && sprite->tyyppi->pisteet != 0){
					char luku[10];
					sprintf(luku, "%i", sprite->tyyppi->pisteet);
					Fadetext_New(fontti2,luku,(int)sprite->x-8,(int)sprite->y-8,80);
					Game->score_increment += sprite->tyyppi->pisteet;
				}
			} else
				sprite->energia = 1;
		}
	}

	if (sprite->damage_timer == 0)
		sprite->saatu_vahinko_tyyppi = DAMAGE_NONE;


	/*****************************************************************************************/
	/* Revisions                                                                             */
	/*****************************************************************************************/

	/*if (&sprite == Player_Sprite && dev_mode) {

		oikealle   = true;
		vasemmalle = true;
		ylos       = true;
		alas       = true;
		printf("%f\n", sprite_b);

	}*/

	if (!oikealle)
		if (sprite_a > 0)
			sprite_a = 0;

	if (!vasemmalle)
		if (sprite_a < 0)
			sprite_a = 0;

	if (!ylos){
		if (sprite_b < 0)
			sprite_b = 0;

		if (!hyppy_maximissa)
			sprite->jump_timer = 95;//sprite->tyyppi->max_hyppy * 2;
	}

	if (!alas)
		if (sprite_b >= 0){ //If sprite is falling
			if (sprite->jump_timer > 0){
				if (sprite->jump_timer >= 90+10){
					Play_GameSFX(pump_sound,30,(int)sprite_x, (int)sprite_y,
				                  int(25050-sprite->weight*3000),true);

					//Particles_New(	PARTICLE_DUST_CLOUDS,sprite_x+rand()%5-rand()%5-10,sprite_ala+rand()%3-rand()%3,
					//			  0,-0.2,rand()%50+20,0,0);

					if (rand()%7 == 1) {
						Particles_New(PARTICLE_SMOKE,sprite_x+rand()%5-rand()%5-10,sprite_ala+rand()%3-rand()%3,
									  	   0.3,-0.1,450,0,0);
						Particles_New(PARTICLE_SMOKE,sprite_x+rand()%5-rand()%5-10,sprite_ala+rand()%3-rand()%3,
									  	   -0.3,-0.1,450,0,0);
					}

					if (sprite->weight > 1) {
						Game->vibration = 34 + int(sprite->weight * 20);
						PInput::Vibrate(500);
					}
				}

				sprite->jump_timer = 0;
			}

			sprite_b = 0;
		}

	/*****************************************************************************************/
	/* Set correct values                                                                    */
	/*****************************************************************************************/

	if (sprite_b > 4.0)
		sprite_b = 4.0;

	if (sprite_b < -4.0)
		sprite_b = -4.0;

	//Limit speed 2
	if (sprite_a > max_nopeus)
		sprite_a = max_nopeus;

	if (sprite_a < -max_nopeus)
		sprite_a = -max_nopeus;

	if (sprite->energia < 0)
		sprite->energia = 0;

	if (sprite->energia > sprite->tyyppi->energia)
		sprite->energia = sprite->tyyppi->energia;

	if (sprite->damage_timer == 0 || sprite->pelaaja == 1) {
		sprite_x += sprite_a;
		sprite_y += sprite_b;
	}

	if (sprite == Player_Sprite || sprite->energia < 1) {
		double kitka = 1.04;

		if (Game->map.ilma == WEATHER_RAIN || Game->map.ilma == WEATHER_RAIN_LEAVES)
			kitka = 1.03; // Slippery ground in the rain

		if (Game->map.ilma == WEATHER_SNOW)
			kitka = 1.01; // And even more on snow

		if (!alas)
			sprite_a /= kitka;
		else
			sprite_a /= 1.03;//1.02//1.05

		sprite_b /= 1.25;
	}

	sprite->x = sprite_x;
	sprite->y = sprite_y;
	sprite->a = sprite_a;
	sprite->b = sprite_b;

	sprite->oikealle = oikealle;
	sprite->vasemmalle = vasemmalle;
	sprite->alas = alas;
	sprite->ylos = ylos;

	/*
	sprite->weight = sprite->tyyppi->weight;

	if (sprite->energia < 1 && sprite->weight == 0)
		sprite->weight = 1;*/

	if (sprite->jump_timer < 0)
		sprite->alas = false;

	//sprite->crouched   = false;

	/*****************************************************************************************/
	/* AI                                                                                    */
	/*****************************************************************************************/

	//TODO run sprite lua script
	
	if (sprite->pelaaja == 0) {
		for (int ai=0;ai < SPRITE_MAX_AI; ai++) {
			switch (sprite->tyyppi->AI[ai]) {
				case AI_NONE:							ai = SPRITE_MAX_AI; // lopetetaan
													break;
				case AI_KANA:						sprite->AI_Kana();
													break;
				case AI_LITTLE_CHICKEN:					sprite->AI_Kana();
													break;
				case AI_SAMMAKKO1:					sprite->AI_Sammakko1();
													break;
				case AI_SAMMAKKO2:					sprite->AI_Sammakko2();
													break;
				case AI_BONUS:						sprite->AI_Bonus();
													break;
				case AI_EGG:						sprite->AI_Egg();
													break;
				case AI_EGG2:						sprite->AI_Egg2();
													break;
				case AI_AMMUS:						sprite->AI_Ammus();
													break;
				case AI_JUMPER:					sprite->AI_Jumper();
													break;
				case AI_BASIC:						sprite->AI_Basic();
													break;
				case AI_NONSTOP:					sprite->AI_NonStop();
													break;
				case AI_TURNING_HORIZONTALLY:		sprite->AI_Kaantyy_Esteesta_Hori();
													break;
				case AI_KAANTYY_ESTEESTA_VERT:		sprite->AI_Kaantyy_Esteesta_Vert();
													break;
				case AI_LOOK_FOR_CLIFFS:				sprite->AI_Varoo_Kuoppaa();
													break;
				case AI_RANDOM_CHANGE_DIRECTION_H:	sprite->AI_Random_Suunnanvaihto_Hori();
													break;
				case AI_RANDOM_KAANTYMINEN:			sprite->AI_Random_Kaantyminen();
													break;
				case AI_RANDOM_JUMP:				sprite->AI_Random_Hyppy();
													break;
				case AI_FOLLOW_PLAYER:			if (Player_Sprite->invisible_timer == 0)
														sprite->AI_Seuraa_Pelaajaa(*Player_Sprite);
													break;
				case AI_FOLLOW_PLAYER_IF_IN_FRONT:	if (Player_Sprite->invisible_timer == 0)
														sprite->AI_Seuraa_Pelaajaa_Jos_Nakee(*Player_Sprite);
													break;
				case AI_FOLLOW_PLAYER_VERT_HORI:	if (Player_Sprite->invisible_timer == 0)
														sprite->AI_Seuraa_Pelaajaa_Vert_Hori(*Player_Sprite);
													break;
				case AI_FOLLOW_PLAYER_IF_IN_FRONT_VERT_HORI:
													if (Player_Sprite->invisible_timer == 0)
														sprite->AI_Seuraa_Pelaajaa_Jos_Nakee_Vert_Hori(*Player_Sprite);
													break;
				case AI_PAKENEE_PELAAJAA_JOS_NAKEE:	if (Player_Sprite->invisible_timer == 0)
														sprite->AI_Pakenee_Pelaajaa_Jos_Nakee(*Player_Sprite);
													break;
				case AI_POMMI:						sprite->AI_Pommi();
													break;
				case AI_ATTACK_1_JOS_OSUTTU:		sprite->AI_Attack_1_Jos_Osuttu();
													break;
				case AI_ATTACK_2_JOS_OSUTTU:		sprite->AI_Attack_2_Jos_Osuttu();
													break;
				case AI_ATTACK_1_NONSTOP:			sprite->AI_Attack_1_Nonstop();
													break;
				case AI_ATTACK_2_NONSTOP:			sprite->AI_Attack_2_Nonstop();
													break;
				case AI_ATTACK_1_IF_PLAYER_IN_FRONT:
													if (Player_Sprite->invisible_timer == 0)
														sprite->AI_Attack_1_if_Player_in_Front(*Player_Sprite);
													break;
				case AI_ATTACK_2_IF_PLAYER_IN_FRONT:
													if (Player_Sprite->invisible_timer == 0)
														sprite->AI_Attack_2_if_Player_in_Front(*Player_Sprite);
													break;
				case AI_ATTACK_1_IF_PLAYER_BELLOW:
													if (Player_Sprite->invisible_timer == 0)
														sprite->AI_Attack_1_if_Player_Bellow(*Player_Sprite);
													break;
				case AI_HYPPY_JOS_PELAAJA_YLAPUOLELLA:
													if (Player_Sprite->invisible_timer == 0)
														sprite->AI_Hyppy_Jos_Pelaaja_Ylapuolella(*Player_Sprite);
													break;
				case AI_DAMAGED_BY_WATER:		sprite->AI_Damaged_by_Water();
													break;
				case AI_KILL_EVERYONE:				sprite->AI_Kill_Everyone();
													break;
				case AI_KITKA_VAIKUTTAA:			sprite->AI_Kitka_Vaikuttaa();
													break;
				case AI_PIILOUTUU:					sprite->AI_Piiloutuu();
													break;
				case AI_PALAA_ALKUUN_X:				sprite->AI_Palaa_Alkuun_X();
													break;
				case AI_PALAA_ALKUUN_Y:				sprite->AI_Palaa_Alkuun_Y();
													break;
				case AI_LIIKKUU_X_COS:				sprite->AI_Liikkuu_X(cos_table(degree));
													break;
				case AI_LIIKKUU_Y_COS:				sprite->AI_Liikkuu_Y(cos_table(degree));
													break;
				case AI_LIIKKUU_X_SIN:				sprite->AI_Liikkuu_X(sin_table(degree));
													break;
				case AI_LIIKKUU_Y_SIN:				sprite->AI_Liikkuu_Y(sin_table(degree));
													break;
				case AI_LIIKKUU_X_COS_NOPEA:		sprite->AI_Liikkuu_X(cos_table(degree*2));
													break;
				case AI_LIIKKUU_Y_SIN_NOPEA:		sprite->AI_Liikkuu_Y(sin_table(degree*2));
													break;
				case AI_LIIKKUU_X_COS_HIDAS:		sprite->AI_Liikkuu_X(cos_table(degree/2));
													break;
				case AI_LIIKKUU_Y_SIN_HIDAS:		sprite->AI_Liikkuu_Y(sin_table(degree/2));
													break;
				case AI_LIIKKUU_Y_SIN_VAPAA:		sprite->AI_Liikkuu_Y(sin_table(sprite->action_timer/2));
													break;
				case AI_CHANGE_WHEN_ENERGY_UNDER_2:	if (sprite->tyyppi->muutos != nullptr)
														sprite->AI_Change_When_Energy_Under_2(sprite->tyyppi->muutos);
													break;
				case AI_CHANGE_WHEN_ENERGY_OVER_1:	if (sprite->tyyppi->muutos != nullptr)
														if (sprite->AI_Change_When_Energy_Over_1(sprite->tyyppi->muutos)==1)
															Effect_Destruction(FX_DESTRUCT_SAVU_HARMAA, (u32)sprite->x, (u32)sprite->y);
													break;
				case AI_MUUTOS_AJASTIN:				if (sprite->tyyppi->muutos != nullptr) {
														sprite->AI_Muutos_Ajastin(sprite->tyyppi->muutos);
													}
													break;
				case AI_MUUTOS_JOS_OSUTTU:			if (sprite->tyyppi->muutos != nullptr) {
														sprite->AI_Muutos_Jos_Osuttu(sprite->tyyppi->muutos);
													}
													break;
				case AI_TELEPORT:					if (sprite->AI_Teleportti(Sprites_List, *Player_Sprite)==1)
													{

														Game->camera_x = (int)Player_Sprite->x;
														Game->camera_y = (int)Player_Sprite->y;
														Game->dcamera_x = Game->camera_x-screen_width/2;
														Game->dcamera_y = Game->camera_y-screen_height/2;
														Fade_in(FADE_NORMAL);
														if (sprite->tyyppi->aanet[SOUND_ATTACK2] != -1)
															Play_MenuSFX(sprite->tyyppi->aanet[SOUND_ATTACK2], 100);
															//Play_GameSFX(, 100, Game->camera_x, Game->camera_y, SOUND_SAMPLERATE, false);


													}
													break;
				case AI_KIIPEILIJA:					sprite->AI_Kiipeilija();
													break;
				case AI_KIIPEILIJA2:				sprite->AI_Kiipeilija2();
													break;
				case AI_TUHOUTUU_JOS_EMO_TUHOUTUU:	sprite->AI_Tuhoutuu_Jos_Emo_Tuhoutuu();
													break;

				case AI_TIPPUU_TARINASTA:			sprite->AI_Tippuu_Tarinasta(Game->vibration + Game->button_vibration);
													break;
				case AI_LIIKKUU_DOWN_JOS_KYTKIN1_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(Game->button1,0,1);
													break;
				case AI_LIIKKUU_UP_JOS_KYTKIN1_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(Game->button1,0,-1);
													break;
				case AI_LIIKKUU_LEFT_JOS_KYTKIN1_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(Game->button1,-1,0);
													break;
				case AI_LIIKKUU_RIGHT_JOS_KYTKIN1_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(Game->button1,1,0);
													break;
				case AI_LIIKKUU_DOWN_JOS_KYTKIN2_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(Game->button2,0,1);
													break;
				case AI_LIIKKUU_UP_JOS_KYTKIN2_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(Game->button2,0,-1);
													break;
				case AI_LIIKKUU_LEFT_JOS_KYTKIN2_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(Game->button2,-1,0);
													break;
				case AI_LIIKKUU_RIGHT_JOS_KYTKIN2_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(Game->button2,1,0);
													break;
				case AI_LIIKKUU_DOWN_JOS_KYTKIN3_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(Game->button3,0,1);
													break;
				case AI_LIIKKUU_UP_JOS_KYTKIN3_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(Game->button3,0,-1);
													break;
				case AI_LIIKKUU_LEFT_JOS_KYTKIN3_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(Game->button3,-1,0);
													break;
				case AI_LIIKKUU_RIGHT_JOS_KYTKIN3_PAINETTU: sprite->AI_Liikkuu_Jos_Kytkin_Painettu(Game->button3,1,0);
													break;
				case AI_TIPPUU_JOS_KYTKIN1_PAINETTU: sprite->AI_Tippuu_Jos_Kytkin_Painettu(Game->button1);
													break;
				case AI_TIPPUU_JOS_KYTKIN2_PAINETTU: sprite->AI_Tippuu_Jos_Kytkin_Painettu(Game->button2);
													break;
				case AI_TIPPUU_JOS_KYTKIN3_PAINETTU: sprite->AI_Tippuu_Jos_Kytkin_Painettu(Game->button3);
													break;
				case AI_RANDOM_LIIKAHDUS_VERT_HORI:	sprite->AI_Random_Liikahdus_Vert_Hori();
													break;
				case AI_KAANTYY_JOS_OSUTTU:			sprite->AI_Kaantyy_Jos_Osuttu();
													break;
				case AI_EVIL_ONE:					if (sprite->energia < 1) 
													{
														PSound::set_musicvolume(0);
														Game->music_stopped = true;
													}
													break;
				
				case AI_DESTRUCTED_NEXT_TO_PLAYER:	sprite->AI_Destructed_Next_To_Player(*Player_Sprite);
													break;

				default:

				if (sprite->tyyppi->AI[ai] >= AI_INFOS_BEGIN && sprite->tyyppi->AI[ai] <= AI_INFOS_END)
					if (sprite->AI_Info(*Player_Sprite)) {

						int info = sprite->tyyppi->AI[ai] - AI_INFOS_BEGIN + 1;
						
						std::string sinfo = "info";
						if (info < 10) sinfo += '0';
						sinfo += std::to_string(info);

						int index = Episode->infos.Search_Id(sinfo.c_str());
						if (index != -1)
							Game->Show_Info(Episode->infos.Get_Text(index));
						else
							Game->Show_Info(tekstit->Get_Text(PK_txt.infos[info]));

					}
			}



		}
	}

	//if (kaiku == 1 && sprite->tyyppi->tyyppi == TYPE_PROJECTILE && sprite->tyyppi->vahinko_tyyppi == DAMAGE_NOISE &&
	//	sprite->tyyppi->aanet[SOUND_ATTACK1] > -1)
	//	Play_GameSFX(sprite->tyyppi->aanet[SOUND_ATTACK1],20, (int)sprite_x, (int)sprite_y,
	//				  sprite->tyyppi->aani_frq, sprite->tyyppi->random_frq);


	/*****************************************************************************************/
	/* Set game area to sprite                                                               */
	/*****************************************************************************************/

	if (sprite->x < 0)
		sprite->x = 0;

	if (sprite->y < -sprite_korkeus)
		sprite->y = -sprite_korkeus;

	if (sprite->x > PK2MAP_MAP_WIDTH*32)
		sprite->x = PK2MAP_MAP_WIDTH*32;

	// If the sprite falls under the lower edge of the map
	if (sprite->y > PK2MAP_MAP_HEIGHT*32 + sprite_korkeus) {

		sprite->y = PK2MAP_MAP_HEIGHT*32 + sprite_korkeus;
		sprite->energia = 0;
		sprite->piilota = true;

		if (sprite->kytkinpaino >= 1)
			Game->vibration = 50;
	}

	if (sprite->a > max_nopeus)
		sprite->a = max_nopeus;

	if (sprite->a < -max_nopeus)
		sprite->a = -max_nopeus;


	/*****************************************************************************************/
	/* Attacks 1 and 2                                                                       */
	/*****************************************************************************************/

	// If the sprite is ready and isn't crouching
	if (sprite->charging_timer == 0 && !sprite->crouched) {
		// the attack has just started
		if (sprite->attack1_timer == sprite->tyyppi->attack1_time) {
			// provides recovery time, after which the sprite can attack again
			sprite->charging_timer = sprite->tyyppi->charge_time;
			if(sprite->charging_timer == 0) sprite->charging_timer = 5;
			// if you don't have your own charging time ...
			if (sprite->ammus1 != nullptr && sprite->tyyppi->charge_time == 0)
			// ... and the projectile has, take charge_time from the projectile
				if (sprite->ammus1->tulitauko > 0)
					sprite->charging_timer = sprite->ammus1->tulitauko;

			// attack sound
			Play_GameSFX(sprite->tyyppi->aanet[SOUND_ATTACK1],100, (int)sprite_x, (int)sprite_y,
						  sprite->tyyppi->aani_frq, sprite->tyyppi->random_frq);

			if (sprite->ammus1 != nullptr) {
				Sprites_add_ammo(sprite->ammus1,sprite_x, sprite_y, sprite);

		//		if (Prototypes_List[sprite->ammus1].aanet[SOUND_ATTACK1] > -1)
		//			Play_GameSFX(Prototypes_List[sprite->ammus1].aanet[SOUND_ATTACK1],100, (int)sprite_x, (int)sprite_y,
		//						  sprite->tyyppi->aani_frq, sprite->tyyppi->random_frq);
			}
		}

		// Same as attack 1
		if (sprite->attack2_timer == sprite->tyyppi->attack2_time) {
			sprite->charging_timer = sprite->tyyppi->charge_time;
			if(sprite->charging_timer == 0) sprite->charging_timer = 5;
			if (sprite->ammus2 != nullptr && sprite->tyyppi->charge_time == 0)
				if (sprite->ammus2->tulitauko > 0)
					sprite->charging_timer = sprite->ammus2->tulitauko;

			Play_GameSFX(sprite->tyyppi->aanet[SOUND_ATTACK2],100,(int)sprite_x, (int)sprite_y,
						  sprite->tyyppi->aani_frq, sprite->tyyppi->random_frq);

			if (sprite->ammus2 != nullptr) {
				Sprites_add_ammo(sprite->ammus2,sprite_x, sprite_y, sprite);

		//		if (Prototypes_List[sprite->ammus2].aanet[SOUND_ATTACK1] > -1)
		//			Play_GameSFX(Prototypes_List[sprite->ammus2].aanet[SOUND_ATTACK1],100, (int)sprite_x, (int)sprite_y,
		//						  sprite->tyyppi->aani_frq, sprite->tyyppi->random_frq);

			}
		}
	}

	// Random sounds
	if (sprite->tyyppi->aanet[SOUND_RANDOM] != -1 && rand()%200 == 1 && sprite->energia > 0)
		Play_GameSFX(sprite->tyyppi->aanet[SOUND_RANDOM],80,(int)sprite_x, (int)sprite_y,
					  sprite->tyyppi->aani_frq, sprite->tyyppi->random_frq);

	return 0;

}

int BonusSprite_Movement(SpriteClass* sprite){

	sprite_x = sprite->x;
	sprite_y = sprite->y;
	sprite_a = sprite->a;
	sprite_b = sprite->b;

	sprite_leveys  = sprite->tyyppi->leveys;
	sprite_korkeus = sprite->tyyppi->korkeus;

	sprite_vasen = sprite_x - sprite_leveys  / 2;
	sprite_oikea = sprite_x + sprite_leveys  / 2;
	sprite_yla   = sprite_y - sprite_korkeus / 2;
	sprite_ala   = sprite_y + sprite_korkeus / 2;

	oikealle	= true,
	vasemmalle	= true,
	ylos		= true,
	alas		= true;

	vedessa = sprite->vedessa;

	max_nopeus = sprite->tyyppi->max_nopeus;

	if (sprite->damage_timer > 0)
		sprite->damage_timer--;

	if (sprite->charging_timer > 0)
		sprite->charging_timer--;

	if (sprite->mutation_timer > 0)	// aika muutokseen
		sprite->mutation_timer --;

	// Hyppyyn liittyv�t seikat

	if (Game->button_vibration + Game->vibration > 0 && sprite->jump_timer == 0)
		sprite->jump_timer = sprite->tyyppi->max_hyppy / 2;

	if (sprite->jump_timer > 0 && sprite->jump_timer < sprite->tyyppi->max_hyppy)
	{
		sprite->jump_timer ++;
		sprite_b = (sprite->tyyppi->max_hyppy - sprite->jump_timer)/-4.0;//-2
	}

	if (sprite_b > 0)
		sprite->jump_timer = sprite->tyyppi->max_hyppy;



	if (sprite->weight != 0)	// jos bonuksella on weight, tutkitaan ymp�rist�
	{
		// o
		//
		// |  Gravity
		// V

		sprite_b += sprite->weight + sprite_b/1.25;

		if (sprite->vedessa)
		{
			if (sprite_b > 0)
				sprite_b /= 2.0;

			if (rand()%80 == 1)
				Particles_New(PARTICLE_SPARK,sprite_x-4,sprite_y,0,-0.5-rand()%2,rand()%30+30,0,32);
		}

		sprite->vedessa = false;

		sprite->kytkinpaino = sprite->weight;

		/* TOISET SPRITET */

		PK2BLOCK spritepalikka; 

		for (SpriteClass* sprite2 : Sprites_List) {
			if (sprite2 != sprite && !sprite2->piilota) {
				if (sprite2->tyyppi->este && sprite->tyyppi->tiletarkistus) {
					if (sprite_x-sprite_leveys/2 +sprite_a <= sprite2->x + sprite2->tyyppi->leveys /2 &&
						sprite_x+sprite_leveys/2 +sprite_a >= sprite2->x - sprite2->tyyppi->leveys /2 &&
						sprite_y-sprite_korkeus/2+sprite_b <= sprite2->y + sprite2->tyyppi->korkeus/2 &&
						sprite_y+sprite_korkeus/2+sprite_b >= sprite2->y - sprite2->tyyppi->korkeus/2)
					{
						spritepalikka.koodi = 0;
						spritepalikka.ala   = (int)sprite2->y + sprite2->tyyppi->korkeus/2;
						spritepalikka.oikea = (int)sprite2->x + sprite2->tyyppi->leveys/2;
						spritepalikka.vasen = (int)sprite2->x - sprite2->tyyppi->leveys/2;
						spritepalikka.yla   = (int)sprite2->y - sprite2->tyyppi->korkeus/2;

						spritepalikka.alas       = BLOCK_WALL;
						spritepalikka.ylos       = BLOCK_WALL;
						spritepalikka.oikealle   = BLOCK_WALL;
						spritepalikka.vasemmalle = BLOCK_WALL;

						if (!sprite2->tyyppi->este_alas)
							spritepalikka.alas		 = BLOCK_BACKGROUND;
						if (!sprite2->tyyppi->este_ylos)
							spritepalikka.ylos		 = BLOCK_BACKGROUND;
						if (!sprite2->tyyppi->este_oikealle)
							spritepalikka.oikealle   = BLOCK_BACKGROUND;
						if (!sprite2->tyyppi->este_vasemmalle)
							spritepalikka.vasemmalle = BLOCK_BACKGROUND;


						spritepalikka.water  = false;

						spritepalikka.tausta = false;

						// Bonus accepts just walls?
						/*spritepalikka.oikealle   = BLOCK_WALL;
						spritepalikka.vasemmalle = BLOCK_WALL;
						spritepalikka.ylos       = BLOCK_WALL;
						spritepalikka.alas       = BLOCK_WALL;*/

						Check_SpriteBlock(sprite, spritepalikka); //Colision bonus and sprite block
					}
				}

				if (sprite_x < sprite2->x + sprite2->tyyppi->leveys/2 &&
					sprite_x > sprite2->x - sprite2->tyyppi->leveys/2 &&
					sprite_y < sprite2->y + sprite2->tyyppi->korkeus/2 &&
					sprite_y > sprite2->y - sprite2->tyyppi->korkeus/2 &&
					sprite->damage_timer == 0)
				{
					if (sprite2->tyyppi->type != TYPE_BONUS &&
						!(sprite2 == Player_Sprite && sprite->tyyppi->tuhoutuminen != FX_DESTRUCT_EI_TUHOUDU))
						sprite_a += sprite2->a*(rand()%4);

					// lis�t��n spriten painoon sit� koskettavan toisen spriten weight
					sprite->kytkinpaino += sprite2->tyyppi->weight;

					// samanmerkkiset spritet vaihtavat suuntaa t�rm�tess��n
					if (sprite->tyyppi == sprite2->tyyppi &&
						sprite2->energia > 0)
					{
						if (sprite->x < sprite2->x) {
							sprite2->a += sprite->a / 3.0;
							oikealle = false;
						}
						if (sprite->x > sprite2->x) {
							sprite2->a += sprite->a / 3.0;
							vasemmalle = false;
						}
						/*
						if (sprite->y < spritet[sprite_index].y)
							alas = false;
						if (sprite->y > spritet[sprite_index].y)
							ylos = false;*/
					}

				}
			}
		}

		// Tarkistetaan ettei menn� mihink��n suuntaan liian kovaa.

		if (sprite_b > 4)
			sprite_b = 4;

		if (sprite_b < -4)
			sprite_b = -4;

		if (sprite_a > 3)
			sprite_a = 3;

		if (sprite_a < -3)
			sprite_a = -3;

		// Lasketaan

		if (sprite->tyyppi->tiletarkistus)
		{

			int palikat_x_lkm = (int)((sprite_leveys) /32)+4;
			int palikat_y_lkm = (int)((sprite_korkeus)/32)+4;

			PK2BLOCK near_blocks[palikat_x_lkm * palikat_y_lkm];

			int map_vasen = (int)(sprite_vasen)/32;
			int map_yla   = (int)(sprite_yla)/32;

			for (int y = 0; y < palikat_y_lkm; y++)
				for (int x = 0; x < palikat_x_lkm; x++)
					near_blocks[x+y*palikat_x_lkm] = Block_Get(map_vasen+x-1,map_yla+y-1);

			// Tutkitaan t�rm��k� palikkaan

			for (int y = 0; y < palikat_y_lkm; y++)
				for (int x = 0; x < palikat_x_lkm; x++)
					Check_MapBlock(sprite, near_blocks[x+y*palikat_x_lkm]);

		}

		if (vedessa != sprite->vedessa) {
			Effect_Splash((int)sprite_x,(int)sprite_y,32);
			Play_GameSFX(splash_sound, 100, (int)sprite_x, (int)sprite_y, SOUND_SAMPLERATE, true);
		}


		if (!oikealle)
		{
			if (sprite_a > 0)
				sprite_a = -sprite_a/1.5;
		}

		if (!vasemmalle)
		{
			if (sprite_a < 0)
				sprite_a = -sprite_a/1.5;
		}

		if (!ylos)
		{
			if (sprite_b < 0)
				sprite_b = 0;

			sprite->jump_timer = sprite->tyyppi->max_hyppy;
		}

		if (!alas)
		{
			if (sprite_b >= 0)
			{
				if (sprite->jump_timer > 0)
				{
					sprite->jump_timer = 0;
				//	if (/*sprite_b == 4*/!maassa)
				//		Play_GameSFX(pump_sound,20,(int)sprite_x, (int)sprite_y,
				//				      int(25050-sprite->tyyppi->weight*4000),true);
				}

				if (sprite_b > 2)
					sprite_b = -sprite_b/(3+rand()%2);
				else
					sprite_b = 0;
			}
			//sprite_a /= kitka;
			sprite_a /= 1.07;
		}
		else
		{
			sprite_a /= 1.02;
		}

		sprite_b /= 1.5;

		if (sprite_b > 4)
			sprite_b = 4;

		if (sprite_b < -4)
			sprite_b = -4;

		if (sprite_a > 4)
			sprite_a = 4;

		if (sprite_a < -4)
			sprite_a = -4;

		sprite_x += sprite_a;
		sprite_y += sprite_b;

		sprite->x = sprite_x;
		sprite->y = sprite_y;
		sprite->a = sprite_a;
		sprite->b = sprite_b;

		sprite->oikealle = oikealle;
		sprite->vasemmalle = vasemmalle;
		sprite->alas = alas;
		sprite->ylos = ylos;
	}
	else	// jos spriten weight on nolla, tehd��n spritest� "kelluva"
	{
		sprite->y = sprite->alku_y + cos_table(degree+(sprite->alku_x+sprite->alku_y)) / 3.0;
		sprite_y = sprite->y;
	}

	sprite->weight = sprite->initial_weight;

	int tuhoutuminen;

	// Test if player touches bonus
	if (sprite_x < Player_Sprite->x + Player_Sprite->tyyppi->leveys/2 &&
		sprite_x > Player_Sprite->x - Player_Sprite->tyyppi->leveys/2 &&
		sprite_y < Player_Sprite->y + Player_Sprite->tyyppi->korkeus/2 &&
		sprite_y > Player_Sprite->y - Player_Sprite->tyyppi->korkeus/2 &&
		sprite->damage_timer == 0)
	{
		if (sprite->energia > 0 && Player_Sprite->energia > 0)
		{
			if (sprite->tyyppi->pisteet != 0) {

				char* name = sprite->tyyppi->nimi;
				if (!Episode->ignore_collectable && strncmp(name, Episode->collectable_name.c_str(), Episode->collectable_name.size()) == 0)
					Game->apples_got++;

				Game->score_increment += sprite->tyyppi->pisteet;
				
				if (!sprite->Onko_AI(AI_BONUS_AIKA)) {

					char luku[10];
					sprintf(luku, "%i", sprite->tyyppi->pisteet);
					
					if (sprite->tyyppi->pisteet >= 50)
						Fadetext_New(fontti2,luku,(int)sprite->x-8,(int)sprite->y-8,100);
					else
						Fadetext_New(fontti1,luku,(int)sprite->x-8,(int)sprite->y-8,100);

				}

			}

			if (sprite->Onko_AI(AI_BONUS_AIKA)) {
				
				int increase_time = sprite->tyyppi->charge_time * TIME_FPS;
				Game->timeout += increase_time;

				float shown_time = float(increase_time) / 60;
				int min = int(shown_time / 60);
				int sek = int(shown_time) % 60;
				
				char min_c[8], sek_c[8];
				sprintf(min_c, "%i", min);
				sprintf(sek_c, "%i", sek);

				char luku[8];
				strcpy(luku, min_c);
				strcat(luku, ":");
                if (sek < 10)
                    strcat(luku, "0");
				strcat(luku, sek_c);
				Fadetext_New(fontti1,luku,(int)sprite->x-15,(int)sprite->y-8,100);
				
			}

			if (sprite->Onko_AI(AI_BONUS_NAKYMATTOMYYS))
				Player_Sprite->invisible_timer = sprite->tyyppi->charge_time;

			if (sprite->Onko_AI(AI_BONUS_SUPERMODE)) {
				Player_Sprite->super_mode_timer = sprite->tyyppi->charge_time;
				PSound::play_overlay_music();
			}

			//Game->map.spritet[(int)(sprite->alku_x/32) + (int)(sprite->alku_y/32)*PK2MAP_MAP_WIDTH] = 255;

			if (sprite->tyyppi->tuhoutuminen != FX_DESTRUCT_EI_TUHOUDU)
				Player_Sprite->energia -= sprite->tyyppi->vahinko;

			tuhoutuminen = sprite->tyyppi->tuhoutuminen;

			if (tuhoutuminen != FX_DESTRUCT_EI_TUHOUDU)
			{
				if (tuhoutuminen >= FX_DESTRUCT_ANIMAATIO)
					tuhoutuminen -= FX_DESTRUCT_ANIMAATIO;
				else
				{
					if (sprite->tyyppi->avain)
					{
						Game->keys--;

						if (Game->keys < 1)
							Game->Open_Locks();
					}

					sprite->piilota = true;
				}

				if (sprite->Onko_AI(AI_UUSI_JOS_TUHOUTUU)) {
					double ax, ay;
					ax = sprite->alku_x;//-sprite->tyyppi->leveys;
					ay = sprite->alku_y-sprite->tyyppi->korkeus/2.0;
					Sprites_add(sprite->tyyppi, 0, ax-17, ay, sprite, false);
					for (int r=1;r<6;r++)
						Particles_New(PARTICLE_SPARK,ax+rand()%10-rand()%10, ay+rand()%10-rand()%10,0,0,rand()%100,0.1,32);

				}

				if (sprite->tyyppi->bonus != nullptr)
					if (Gifts_Add(sprite->tyyppi->bonus))
						Game->Show_Info(tekstit->Get_Text(PK_txt.game_newitem));

				if (sprite->tyyppi->muutos != nullptr)
				{
					if (sprite->tyyppi->muutos->AI[0] != AI_BONUS)
					{
						Player_Sprite->tyyppi = sprite->tyyppi->muutos;
						Player_Sprite->ammus1 = Player_Sprite->tyyppi->ammus1;
						Player_Sprite->ammus2 = Player_Sprite->tyyppi->ammus2;
						Player_Sprite->initial_weight = Player_Sprite->tyyppi->weight;
						Player_Sprite->y -= Player_Sprite->tyyppi->korkeus/2;
						
						int infotext = Episode->infos.Search_Id("pekka transformed");
						if (infotext != -1)
							Game->Show_Info(Episode->infos.Get_Text(infotext));
						//Game->Show_Info("pekka has been transformed!");
					}
				}

				if (sprite->tyyppi->ammus1 != nullptr) {
					Player_Sprite->ammus1 = sprite->tyyppi->ammus1;

					int infotext = Episode->infos.Search_Id("new egg attack");
					if (infotext != -1)
						Game->Show_Info(Episode->infos.Get_Text(infotext));
					else
						Game->Show_Info(tekstit->Get_Text(PK_txt.game_newegg));
				}

				if (sprite->tyyppi->ammus2 != nullptr) {
					Player_Sprite->ammus2 = sprite->tyyppi->ammus2;

					int infotext = Episode->infos.Search_Id("new doodle attack");
					if (infotext != -1)
						Game->Show_Info(Episode->infos.Get_Text(infotext));
					else
						Game->Show_Info(tekstit->Get_Text(PK_txt.game_newdoodle));
				}

				Play_GameSFX(sprite->tyyppi->aanet[SOUND_DESTRUCTION],100, (int)sprite->x, (int)sprite->y,
							  sprite->tyyppi->aani_frq, sprite->tyyppi->random_frq);

				Effect_Destruction(tuhoutuminen, (u32)sprite_x, (u32)sprite_y);
			}
		}
	}

	for (int i = 0; i < SPRITE_MAX_AI; i++) {

		if (sprite->tyyppi->AI[i] == AI_NONE)
			break;

		switch (sprite->tyyppi->AI[i]) {
		
		case AI_BONUS:				sprite->AI_Bonus(); break;

		case AI_BASIC:				sprite->AI_Basic(); break;

		case AI_MUUTOS_AJASTIN:		if (sprite->tyyppi->muutos != nullptr)
										sprite->AI_Muutos_Ajastin(sprite->tyyppi->muutos);
									break;

		case AI_TIPPUU_TARINASTA:	sprite->AI_Tippuu_Tarinasta(Game->vibration + Game->button_vibration);
									break;

		default:					break;
		
		}
	}

	/* The energy doesn't matter that the player is a bonus item */
	if (sprite->pelaaja != 0)
		sprite->energia = 0;

	return 0;
}

