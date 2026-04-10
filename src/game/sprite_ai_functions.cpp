//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "sprite_ai_functions.hpp"
#include "spriteclass.hpp"
#include "gfx/effect.hpp"
#include "game.hpp"
#include "episode/episodeclass.hpp"
#include "system.hpp"
#include "sfx.hpp"
#include "engine/PSound.hpp"
#include "language.hpp"

namespace AI_Functions{

SpriteClass*player = nullptr;
SpriteClass*player_invisible = nullptr;


void Rooster(SpriteClass*s){
    if (rand()%50 == 10 && s->a != 0)
        s->a /= 1.1;

    if (rand()%150 == 10 && s->b == 0 && s->jump_timer == 0 && s->can_move_up)
    {
        s->jump_timer = 1;
        while (s->a == 0)
            s->a = rand()%2 - rand()%2;
    }

    if (rand()%20 == 1 && s->b == 0 && s->jump_timer == 0 && !s->can_move_right && !s->flip_x)
    {
        s->jump_timer = 1;
        while (s->a == 0)
            s->a = rand()%2;
    }

    if (rand()%20 == 1 && s->b == 0 && s->jump_timer == 0 && !s->can_move_left && s->flip_x)
    {
        s->jump_timer = 1;
        while (s->a == 0)
            s->a = rand()%2 * -1;
    }

    if (rand()%200 == 10)
        s->a = rand()%2 - rand()%2;

    if (s->jump_timer == s->prototype->max_jump && s->a == 0)
    {
        while (s->a == 0)
            s->a = rand()%2 - rand()%2;
    }

    if (s->a < 0)
        s->flip_x = true;

    if (s->a > 0)
        s->flip_x = false;

}

void BlueFrog(SpriteClass*s){
    if (s->action_timer%100 == 0 && s->jump_timer == 0 && s->can_move_up)
    {
        s->jump_timer = 1;
    }
}

void RedFrog(SpriteClass*s){
    if (s->action_timer%100 == 0 && s->can_move_up)
    {
        s->jump_timer = 1;

    }

    if (s->jump_timer > 0)
    {
        if (!s->flip_x)
            s->a = s->prototype->max_speed / 3.5;
        else
            s->a = s->prototype->max_speed / -3.5;
    }
}

void Egg(SpriteClass*s){
    if (!s->can_move_down)
		s->energy = 0;

	//a /= 1.01;

	if (s->energy == 0 && s->charging_timer == 0)
		s->charging_timer = s->prototype->charge_time;

	if (s->charging_timer == 1)
		s->removed = true;
}

void Egg2(SpriteClass*s){
    if (!s->can_move_down)
		s->damage_taken = s->prototype->energy;

	//a /= 1.01;

	if (s->energy == 0 && s->charging_timer == 0)
		s->charging_timer = s->prototype->charge_time;

	if (s->charging_timer == 1)
		s->removed = true;
}


void Projectile(SpriteClass*s){
    if (s->a < 0)
		s->flip_x = true;

	if (s->a > 0)
		s->flip_x = false;

	if (s->charging_timer == 0)
		s->charging_timer = s->prototype->charge_time;

	if (s->charging_timer == 1)
	{
		if(!s->prototype->indestructible){
			s->damage_taken = s->prototype->energy;
			s->damage_taken_type = DAMAGE_WEAK_SELF_DESTRUCTION;
			s->self_destruction = true;
		}
		else{
			s->legacy_indestructible_ammo = true;
		}
	}

	if (s->energy < 1)
		s->removed = true;
}

void Jumper(SpriteClass*s){
    if (!s->can_move_down && s->b==0 && s->jump_timer == 0)
	{
		s->jump_timer = 1;
	}

	if (s->a < 0)
		s->flip_x = true;

	if (s->a > 0)
		s->flip_x = false;
}

void Basic(SpriteClass*s){
    /*if (s->x < 10)
	{
		s->x = 10;
		s->can_move_left = false;
	}

	else if (s->x > 8192)
	{
		s->x = 8192;
		s->can_move_right = false;
	}

	if (s->y > 9920)
	{
		s->y = 9920;
	}

	else if (s->y < -32)
	{
		s->y = -32;
	}*/

	if (s->a < 0)
		s->flip_x = true;

	else if (s->a > 0)
		s->flip_x = false;

	s->action_timer++;

	if (s->action_timer > 31320) // divisible by 360
		s->action_timer = 0;
}

void NonStop(SpriteClass*s){
    double max = s->prototype->max_speed / 3.5;

    if (s->flip_x)
    {
        if (s->a > -max)
            s->a -= 0.1;
    }
    else
    {
        if (s->a < max)
            s->a += 0.1;
    }
}

void NonStopVertical(SpriteClass*s){
	double max = s->prototype->max_speed / 3.5;

    if (s->b <= 0)
    {
        if (s->b > -max)
            s->b -= 0.1;
    }
    else
    {
        if (s->b < max)
            s->b += 0.1;
    }
}

void NonStop2(SpriteClass*s){
	double max = s->prototype->max_speed;
    if (s->flip_x)
    {
        if (s->a > -max)
            s->a -= 1;
    }
    else
    {
        if (s->a < max)
            s->a += 1;
    }
}

void Turning_Horizontally(SpriteClass*s){

    if (!s->can_move_right)
    {
        s->a = s->prototype->max_speed / -3.5;
    }

    if (!s->can_move_left)
    {
        s->a = s->prototype->max_speed / 3.5;
    }
}
void Turning_Vertically(SpriteClass*s){
    if (!s->can_move_down)
    {
        s->b = s->prototype->max_speed / -3.5;
    }

    if (!s->can_move_up)
    {
        s->b = s->prototype->max_speed / 3.5;
    }
}

void Look_For_Cliffs(SpriteClass*s){
    double max = s->prototype->max_speed / 3.5;

    if (s->edge_on_the_right && s->a > -max){
        s->a -= 0.13;
    }

    if (s->edge_on_the_left && s->a < max){
        s->a += 0.13;
    }

    /*
    if (this->edge_on_the_right && this->a > 0)
    {
        this->a = this->a * -1;
        flip_x = true;
    }

    if (this->edge_on_the_left && this->a < 0)
    {
        this->a = this->a * -1;
        flip_x = false;
    }
	*/
}

void Random_Change_Dir_H(SpriteClass*s){
    if (rand()%150 == 1)
    {
        int max = (int)(s->prototype->max_speed / 4);

        while (s->a == 0 && max > 0)
            s->a = rand()%max+1 - rand()%max+1;
    }
}

void Random_Turning(SpriteClass*s){
    if (s->action_timer%400 == 1 && s->a == 0)
    {
        s->flip_x = !s->flip_x;
    }
}


void Random_Jump(SpriteClass*s){
    if (rand()%150 == 10 && s->b == 0 && s->jump_timer == 0 && s->can_move_up){
        s->jump_timer = 1;
    }
}

void Follow_Player(SpriteClass*s){
	if (player!=nullptr)
	{
		double max = s->prototype->max_speed / 3.5;

		if (s->a > -max && s->x > player->x)
		{
			s->a -= 0.1;
		}

		if (s->a < max && s->x < player->x)
		{
			s->a += 0.1;
		}

		/*s->seen_player_x = (int)(player->x+player->a);
		s->seen_player_y = (int)(player->y+player->b);*/

		if (s->prototype->max_speed == 0)
		{
			if (player->x < s->x)
				s->flip_x = true;
			else
				s->flip_x = false;
		}
	}
}

void Follow_Player_If_Seen(SpriteClass*s){
	if (player!=nullptr){

		double max = s->prototype->max_speed / 3.5;

		if (s->seen_player_x != -1){
			if (s->a > -max && s->x > s->seen_player_x)
				s->a -= 0.1;

			if (s->a < max && s->x < s->seen_player_x)
				s->a += 0.1;
		}

		if ((player->x < s->x && s->flip_x) || (player->x > s->x && ! s->flip_x)){
			if ((player->x - s->x < 300 && player->x - s->x > -300) &&
				(player->y - s->y < s->prototype->height && player->y - s->y > -s->prototype->height)){
				s->seen_player_x = (int)(player->x+player->a);
				s->seen_player_y = (int)(player->y+player->b);
			}
			else{
				s->seen_player_x = -1;
				s->seen_player_y = -1;
			}
		}
	}
}


void Follow_Player_Vert_Hori(SpriteClass*s){
	if (player!=nullptr)
	{
		double max = s->prototype->max_speed / 3.5;

		if (s->a > -max && s->x > player->x)
		{
			s->a -= 0.1;
		}

		if (s->a < max && s->x < player->x)
		{
			s->a += 0.1;
		}

		if (s->b > -max && s->y > player->y)
		{
			s->b -= 0.4;
		}

		if (s->b < max && s->y < player->y)
		{
			s->b += 0.4;
		}

		s->seen_player_x = (int)(player->x+player->a);
		s->seen_player_y = (int)(player->y+player->b);

		if (s->prototype->max_speed == 0)
		{
			if (player->x < s->x)
				s->flip_x = true;
			else
				s->flip_x = false;
		}
	}
}


void Follow_Player_If_Seen_Vert_Hori(SpriteClass*s){
	if (player!=nullptr){
		double max = s->prototype->max_speed / 3.5;

		if (s->seen_player_x != -1){
			if (s->a > -max && s->x > s->seen_player_x)
				s->a -= 0.1;

			if (s->a < max && s->x < s->seen_player_x)
				s->a += 0.1;

			if (s->b > -max && s->y > s->seen_player_y)
				s->b -= 0.4;

			if (s->b < max && s->y < s->seen_player_y)
				s->b += 0.4;
		}

		if ((player->x < s->x && s->flip_x) || (player->x > s->x && ! s->flip_x)){
			if ((player->x - s->x < 300 && player->x - s->x > -300) &&
				(player->y - s->y < 80 && player->y - s->y > -80)){
				s->seen_player_x = (int)(player->x+player->a);
				s->seen_player_y = (int)(player->y+player->b);
			}
			else{
				s->seen_player_x = -1;
				s->seen_player_y = -1;
			}
		}
	}
}

void Follow_Player_Diagonally(SpriteClass*s){
	if(player!=nullptr){
		s->flyToWaypointXY(player->x, player->y);
	}
}

void Look_at_Player(SpriteClass*s){
	if(player!=nullptr){
		s->flip_x = player->x < s->x;
	}
}

void Run_Away_From_Player(SpriteClass*s){
	if (player!=nullptr)
	{
		if ((player->x < s->x && s->flip_x && !player->flip_x) || (player->x > s->x && !s->flip_x && player->flip_x))
			if ((player->x - s->x < 300 && player->x - s->x > -300) &&
				(player->y - s->y < s->prototype->height && player->y - s->y > -s->prototype->height))
			{
				double max = s->prototype->max_speed / 2.5;

				if (s->x > player->x) {
					s->a = max;
					s->flip_x = false;
				}

				if (s->x < player->x) {
					s->a = max * -1;
					s->flip_x = true;
				}
			}
	}
}

void Run_Away_From_Supermode_Player(SpriteClass*s){
	if(player_invisible!=nullptr && player_invisible->super_mode_timer!=0){		
		if ((player_invisible->x - s->x < 300 && player_invisible->x - s->x > -300) &&
			(player_invisible->y - s->y < s->prototype->height && player_invisible->y - s->y > -s->prototype->height))
		{
			double max = s->prototype->max_speed / 2.5;

			if (s->x > player_invisible->x) {
				s->a = max;
				s->flip_x = false;
			}

			if (s->x < player_invisible->x) {
				s->a = max * -1;
				s->flip_x = true;
			}
		}
	}
}

void SelfDestruction(SpriteClass*s){
	if (s->charging_timer == 0)
		s->charging_timer = s->prototype->charge_time;

	if (s->charging_timer == 1)
	{
		s->damage_taken = s->energy;
		s->damage_taken_type = DAMAGE_SELF_DESTRUCTION;
		s->self_destruction = true;
	}
}

void Attack_1_Nonstop(SpriteClass*s){
	if (s->charging_timer == 0)
	{
		s->attack1_timer = s->prototype->attack1_time;
	}
}
void Attack_2_Nonstop(SpriteClass*s){
	if (s->charging_timer == 0)
	{
		s->attack2_timer = s->prototype->attack2_time;
	}
}


void Attack_1_if_Player_in_Front(SpriteClass*s){
	if (s->damage_timer == 0 && player!=nullptr)
	{

		double detection = s->prototype->player_detection.x;

		if ((player->x - s->x < detection && player->x - s->x > -detection) &&
			(player->y - s->y < s->prototype->height && player->y - s->y > -s->prototype->height))
		{
			if ((player->x < s->x && s->flip_x) || (player->x > s->x && !s->flip_x))
			{
				s->attack1_timer = s->prototype->attack1_time;
			}
		}
	}
}
void Attack_2_if_Player_in_Front(SpriteClass*s){
	if (s->damage_timer == 0 && player!=nullptr)
	{
		double detection = s->prototype->player_detection.x;

		if ((player->x - s->x < detection && player->x - s->x > -detection) &&
			(player->y - s->y < s->prototype->height && player->y - s->y > -s->prototype->height))
		{
			if ((player->x < s->x && s->flip_x) || (player->x > s->x && !s->flip_x))
			{
				s->attack2_timer = s->prototype->attack2_time;
			}
		}
	}
}
void Attack_1_if_Player_Below(SpriteClass*s){
	if (s->damage_timer == 0 && player!=nullptr)
	{

		double detection = s->prototype->player_detection.y;

		if ((player->x - s->x < s->prototype->width && player->x - s->x > -s->prototype->width) &&
			(player->y > s->y && player->y - s->y < detection))
		{
			s->attack1_timer = s->prototype->attack2_time;
		}
	}
}
/**
 * @brief 
 * 
 * "Greta Engine", new AI
 */
void Attack_1_If_Player_Above(SpriteClass*s){
	if (s->damage_timer == 0 && player!=nullptr)
	{

		double detection = s->prototype->player_detection.y;

		if ((player->x - s->x < s->prototype->width && player->x - s->x > -s->prototype->width) &&
			(player->y < s->y && s->y - player->y < detection))
		{
			s->attack1_timer = s->prototype->attack1_time;
		}
	}
}

/**
 * @brief 
 * 
 * "Greta Engine", new AI
 */
void Attack_2_If_Player_Above(SpriteClass*s){
	if (s->damage_timer == 0 && player!=nullptr)
	{
		double detection = s->prototype->player_detection.y;

		if ((player->x - s->x < s->prototype->width && player->x - s->x > -s->prototype->width) &&
			(player->y < s->y && s->y - player->y < detection))
		{
			s->attack2_timer = s->prototype->attack2_time;
		}
	}
}

/**
 * @brief 
 * 
 * "Greta Engine", new AI
 */
void Transform_If_Player_Above(SpriteClass*s){
	if(player!=nullptr){

		double detection = s->prototype->player_detection.y;

		if ((player->x - s->x < s->prototype->width && player->x - s->x > -s->prototype->width) &&
			(player->y < s->y && s->y - player->y < detection))
		{
			if(s->prototype->charge_time==0){
				s->transform();
			}
			else{
				Self_Transformation(s);
			}
		}
	}
}

void Transform_If_Player_Below(SpriteClass*s){
	if(player!=nullptr){

		double detection = s->prototype->player_detection.y;

		if ((player->x - s->x < s->prototype->width && player->x - s->x > -s->prototype->width) &&
			(player->y > s->y && player->y - s->y < detection))
		{
			if(s->prototype->charge_time==0){
				s->transform();
			}
			else{
				Self_Transformation(s);
			}
		}
	}
}

void Transform_If_Player_in_Front(SpriteClass* s){

	if(player!=nullptr){
		double detection = s->prototype->player_detection.x;
		
		if ((player->x - s->x < detection && player->x - s->x > -detection) &&
			(player->y - s->y < s->prototype->height && player->y - s->y > -s->prototype->height)){

			if(s->prototype->charge_time==0){
				s->transform();
			}
			else{
				Self_Transformation(s);
			}
		}
	}	
}
///////


void Jump_If_Player_Above(SpriteClass*s){
	if (s->jump_timer == 0 && player!=nullptr){

		double detection = s->prototype->player_detection.y;

		if ((player->x - s->x < s->prototype->width && player->x - s->x > -s->prototype->width) &&
			(player->y < s->y && s->y - player->y < detection)){

			s->jump_timer = 1;
		}
	}
}


void Jump_If_Player_in_Front(SpriteClass*s){
	if (s->jump_timer == 0 && player!=nullptr){
		double detection = s->prototype->player_detection.x;
		
		if ((player->x - s->x < detection && player->x - s->x > -detection) &&
			(player->y - s->y < s->prototype->height && player->y - s->y > -s->prototype->height)){

			if ((player->x < s->x && s->flip_x) || (player->x > s->x && !s->flip_x)){

				s->jump_timer = 1;
			}
		}
	}
}

static bool IsPlayerNearby(SpriteClass*s){
	if(player!=nullptr){
		double k = s->prototype->player_detection.x;
		double l = s->prototype->player_detection.y;
		double x = s->x - player->x;
		double y = s->y - player->y;

		k = k*k;
		l = l*l;
		x = x*x;
		y = y*y;

		return x*l + y*k < k*l;
	}

	return false;
}


void Attack_1_if_Player_Nearby(SpriteClass*s){
	if(IsPlayerNearby(s)){
		s->attack1_timer = s->prototype->attack1_time;
	}
}

void Attack_2_if_Player_Nearby(SpriteClass*s){
	if(IsPlayerNearby(s)){
		s->attack2_timer = s->prototype->attack2_time;
	}
}


void Transform_if_Player_Nearby(SpriteClass*s){
	if(IsPlayerNearby(s)){
		s->transform();
	}
}

void Die_if_Player_Nearby(SpriteClass*s){
	if(IsPlayerNearby(s)){
		s->die();
	}
}

void Transform_if_Player_Away(SpriteClass*s){
	if(!IsPlayerNearby(s)){
		s->transform();
	}
}

void Die_if_Player_Away(SpriteClass*s){
	if(!IsPlayerNearby(s)){
		s->die();
	}
}



void Damaged_by_Water(SpriteClass*s){
	if (s->in_water && s->damage_taken_type > DAMAGE_SELF_DESTRUCTION){
		s->damage_taken++;
		s->damage_taken_type = DAMAGE_WATER;
	}
}

void Friction_Effect(SpriteClass*s){
	if (!s->can_move_down)
		s->a /= 1.07;
	else
		s->a /= 1.02;
}

void Hiding(SpriteClass*s){
	if (s->hidden)
	{
		s->a /= 1.02;
		s->crouched = true;
	}
}

void Return_To_Orig_X(SpriteClass*s){

	if(s->seen_player_x == -1){
		double max = s->prototype->max_speed / 3.5;

		if (s->x < s->orig_x-16 && s->a < max)
			s->a += 0.05;

		if (s->x > s->orig_x+16 && s->a > -max)
			s->a -= 0.05;
	}	
}
void Return_To_Orig_Y(SpriteClass*s){
	if(s->seen_player_y==-1){
		double max = s->prototype->max_speed / 3.5;

		if (s->y < s->orig_y-16 && s->b < max)
			s->b += 0.04;

		if (s->y > s->orig_y+16 && s->b > -max)
			s->b -= 0.04;
	}
}


void Follow_Commands(SpriteClass* s) {
	if(s->prototype->commands.size()==0)return;

	if(s->current_command >= s->prototype->commands.size()){
		s->current_command = 0;
	}

	SpriteCommands::Command* c = s->prototype->commands[s->current_command];
	if(c->execute(s)){
		// next command
		s->current_command++;
	}
}

void Transform_When_Energy_Under_2(SpriteClass* s){

	PrototypeClass* transformation = s->prototype->transformation;

	if (transformation!=nullptr&& !s->removed && s->energy < 2 && transformation != s->prototype) {
		if(s->energy == 1 || !s->HasAI(AI_ROOSTER)){

			/**
			 * @brief 
			 * For Yamano's lily pads
			 */
			if(transformation->legacy_projectile){
				s->self_destruction = true;
			}

			s->prototype = transformation;
			s->initial_weight = s->prototype->weight;
		}
	}
}


void Transform_When_Energy_Over_1(SpriteClass* s){

	PrototypeClass* transformation = s->prototype->transformation;
	if (transformation!=nullptr && s->energy > 1 && transformation != s->prototype) {
		s->prototype = transformation;
		s->initial_weight = s->prototype->weight;
		Effect_By_ID(FX_DESTRUCT_SMOKE_GRAY, (u32)s->x, (u32)s->y);
	}
}

void Self_Transformation(SpriteClass* s){

	PrototypeClass* transformation = s->prototype->transformation;
	if (transformation!=nullptr && transformation != s->prototype)
	{
		if (s->mutation_timer/*charging_timer*/ == 0)
			s->mutation_timer/*charging_timer*/ = s->prototype->charge_time;

		if (s->mutation_timer/*charging_timer*/ == 1)
		{
			s->transform();

			s->animation_index = -1;
			s->SetAnimation(ANIMATION_IDLE,true);
		}
	}
}

void Self_Transformation_Random_Prototype(SpriteClass* s){

	int charge_time = s->prototype->charge_time;
	if(charge_time>0){
		if (s->mutation_timer/*charging_timer*/ == 0)
			s->mutation_timer/*charging_timer*/ = s->prototype->charge_time;
		
		if (s->mutation_timer/*charging_timer*/ == 1)
		{
			PrototypeClass* transformation = rand() % 2 == 0 ? s->prototype->transformation : s->prototype->ammo2;
			if(s->transformTo(transformation)){
				s->charging_timer = 0;
				s->animation_index = -1;
				s->SetAnimation(ANIMATION_IDLE,true);
			}
		}
	}

	else{
		PrototypeClass* transformation = rand() % 2 == 0 ? s->prototype->transformation : s->prototype->ammo2;
		if(s->transformTo(transformation)){
			s->charging_timer = 0;
			s->animation_index = -1;
			s->SetAnimation(ANIMATION_IDLE,true);
		}

	}
}

void Random_Move_Vert_Hori(SpriteClass* s){

	if (rand()%150 == 1 || s->action_timer == 1)
	if ((int)s->a == 0 || (int)s->b == 0)
	{
		int max = (int)s->prototype->max_speed;

		if (max != 0)
		{
			while (s->a == 0)
				s->a = rand()%(max+1) - rand()%(max+1);

			while (s->b == 0)
				s->b = rand()%(max+1) - rand()%(max+1);

			//a /= 3.0;
			//b /= 3.0;
		}

	}
}

void Destructed_Next_To_Player(SpriteClass* s) {

	if(player_invisible==nullptr)return;

	double dx = s->x - player_invisible->x;
	double dy = s->y - player_invisible->y;

	int dist = s->prototype->energy * 32;

	if (s->energy > 0 && dx*dx + dy*dy < dist*dist) {

		s->damage_taken = s->prototype->energy;
		s->damage_taken_type = DAMAGE_SELF_DESTRUCTION;		
	}
}


void Climber(SpriteClass*s){

	if (!s->can_move_down && s->can_move_left)
	{
		s->b = 0;
		s->a = s->prototype->max_speed / -3.5;
		//return 1;
	}

	if (!s->can_move_up && s->can_move_right)
	{
		s->b = 0;
		s->a = s->prototype->max_speed / 3.5;
		//b = this->prototype->max_speed / 3.5;
		//return 1;
	}

	if (!s->can_move_right && s->can_move_down)
	{
		s->a = 0;
		s->b = s->prototype->max_speed / 3.5;
		//return 1;
	}

	if (!s->can_move_left && s->can_move_up)
	{
		s->a = 0;
		s->b = s->prototype->max_speed / -3.5;
		//return 1;
	}
}
void Climber2(SpriteClass*s){

	if (s->can_move_left && s->can_move_right && s->can_move_up && s->can_move_down) {

		if (s->a < 0) {
			s->b = s->prototype->max_speed / 3.5;
			//a = 0;
		}
		else if (s->a > 0) {
			s->b = s->prototype->max_speed / -3.5;
			//a = 0;
		}
		else if (s->b < 0) {
			s->a = s->prototype->max_speed / -3.5;
			//b = 0;
		}
		else if (s->b > 0) {
			s->a = s->prototype->max_speed / 3.5;
			//b = 0;
		}
		if (s->b != 0)
			s->a = 0;
	}

}

void Fall_When_Shaken(SpriteClass*s){

	int tarina = Game->getVibration() + Game->button_vibration;

	if (tarina > 0)
	{
		s->initial_weight = 0.5;
	}
}


void Move_If_Switch_Pressed(SpriteClass*s, int game_button, int ak, int bk){
	if (game_button > 0)
	{
		if (s->a == 0 && ak != 0)
		{
			s->a = s->prototype->max_speed / 3.5 * ak; // ak = -1 / 1
		}

		if (s->b == 0 && bk != 0)
			s->b = s->prototype->max_speed / 3.5 * bk; // bk = -1 / 1
	}

	s->flip_x = false;
}


void Teleporter(SpriteClass*s){

	if (player_invisible!=nullptr && s->charging_timer == 0 && s->attack1_timer == 0)
	{
		if (player_invisible->x <= s->x + s->prototype->width /2 && player_invisible->x >= s->x - s->prototype->width /2 &&
			player_invisible->y <= s->y + s->prototype->height/2 && player_invisible->y >= s->y - s->prototype->height/2 )
		{

			PrototypeClass*exitPrototype = s->prototype;
			if(exitPrototype->ammo1!=nullptr){
				exitPrototype = exitPrototype->ammo1;
			}

			SpriteClass* dst = Game->selectTeleporter(s, exitPrototype);
			if(dst==nullptr){
				/**
				 * To prevent the game lagging if there's only one teleporter.
				*/
				s->attack1_timer = s->prototype->attack1_time;

				return;
			}

			dst->attack1_timer = dst->prototype->attack1_time;
			dst->charging_timer = 0;
			s->charging_timer = 0;

			Game->teleportPlayer(dst->x, dst->y, dst->level_sector);

			if (s->prototype->sounds[SOUND_ATTACK2] != -1)
				Play_MenuSFX(s->prototype->sounds[SOUND_ATTACK2], 100);
		}
	}
}


void Turn_Back_If_Damaged(SpriteClass*s) {
	if (s->damage_timer == 1 && s->energy > 0){
		if (s->a != 0) s->a = -s->a;
		s->flip_x = !s->flip_x;
	}
}


void Return_To_Orig_X_Constant(SpriteClass*s){

	if(s->seen_player_x == -1){
		double max_speed = s->prototype->max_speed / 3.5;
		double dx = s->x - s->orig_x;

		if(dx*dx <= max_speed*max_speed){
			s->a = 0;
			s->x = s->orig_x;
		}
		else if(s->x > s->orig_x){
			s->flip_x = true;
			s->a = -max_speed;
		}
		else{
			s->flip_x = false;
			s->a = max_speed;
		}
	}	
}
void Return_To_Orig_Y_Constant(SpriteClass*s){
	if(s->seen_player_y==-1){
		double max_speed = s->prototype->max_speed / 3.5;
		double dy = s->y - s->orig_y;
		if(dy*dy <= max_speed*max_speed){
			s->b = 0;
			s->y = s->orig_y;
		}
		else if(s->y > s->orig_y){
			s->b = -max_speed;
		}
		else{
			s->b = max_speed;
		}
	}
}

void SwimInWater(SpriteClass*s){
	s->swimming = s->in_water && s->energy > 0;
}

void SwimInWaterMaxSpeed(SpriteClass*s){
	s->swimming = s->in_water && s->energy > 0;
	s->max_speed_available = s->swimming;
}


void UncontrollablePlayer(SpriteClass*s){
	if(s->isPlayer()){
		s->player_c = 2;
	}
}

void UncontrollablePlayer2(SpriteClass*s){
	if(s->isPlayer()){
		s->player_c = 3;
	}
}

void MaxSpeedPlayer(SpriteClass*s){
	s->max_speed_available = true;
}

void MaxSpeedOnSuper(SpriteClass*s){
	s->max_speed_available = s->super_mode_timer>0;
}

void NpcCollectBonuses(SpriteClass*s){
	s->can_collect_bonuses = true;
}


void DieIfTouchesWall(SpriteClass*s){

	if( !s->can_move_right || !s->can_move_left ||
		(!s->can_move_down && s->b >= 0) ||
		(!s->can_move_up && s->b <= 0 )) {
		s->damage_taken = s->energy;
		s->damage_taken_type = DAMAGE_SELF_DESTRUCTION;
	}
}

void TransformIfTouchesFloor(SpriteClass*s){
	if(!s->can_move_down && s->b >= 0){
		s->transform();
	}
}

void TransformIfTouchesWall(SpriteClass*s){
	if( !s->can_move_right || !s->can_move_left ||
		(!s->can_move_down && s->b >= 0) ||
		(!s->can_move_up && s->b <= 0 )) {
			
		s->transform();
	}
}

void CannotBePushed(SpriteClass*s){
	s->a = 0;
	s->x = s->orig_x;
}


void Follow_Enemy_X(SpriteClass*s){
	SpriteClass*target = s->level_sector->sprites.findNearestTarget(s);
	if(target!=nullptr){

		double max = s->prototype->max_speed / 3.5;

		if (s->a > -max && s->x > target->x)
		{
			s->a -= 0.1;
		}

		if (s->a < max && s->x < target->x)
		{
			s->a += 0.1;
		}

		if (s->prototype->max_speed == 0)
		{
			if (target->x < s->x)
				s->flip_x = true;
			else
				s->flip_x = false;
		}
	}
}


void Follow_Enemy_Y(SpriteClass*s){

	SpriteClass*target = s->level_sector->sprites.findNearestTarget(s);
	if(target!=nullptr){
		double max = s->prototype->max_speed / 3.5;
		if (s->b > -max && s->y > target->y)
		{
			s->b -= 0.4;
		}

		if (s->b < max && s->y < target->y)
		{
			s->b += 0.4;
		}
	}
}

void Follow_Enemy_Diagonally(SpriteClass*s){
	SpriteClass*target = s->level_sector->sprites.findNearestTarget(s);
	if(target!=nullptr){
		s->flyToWaypointXY(target->x, target->y);		
	}
}

/**
 * @brief 
 * AIs triggered on death
 */

void EvilOne(SpriteClass*s){
	PSound::set_musicvolume(0);
	Game->music_stopped = true;	
}

void Chick(SpriteClass*s){
	Game->game_over = true;
}

void Reborn(SpriteClass*s){
	s->respawn_timer = s->prototype->charge_time;
	if(s->prototype->charge_time==0){		
		s->energy = s->prototype->energy;
		s->removed = false;
	}
	else{
		s->energy = 0;
		s->removed = false;
	}
}


/**
 * @brief 
 * AIs triggered on damage
 * 
 */

void Attack_1_If_Damaged(SpriteClass*s){

	if(!s->prototype->legacy_projectile){
		s->attack1_timer = s->prototype->attack1_time;
		s->charging_timer = 0;
	}
}


void Attack_2_If_Damaged(SpriteClass*s){
	if(!s->prototype->legacy_projectile){
		s->attack2_timer = s->prototype->attack2_time;
		s->charging_timer = 0;
	}
}


void InfiniteEnergy(SpriteClass*s){
	if(s->damage_taken_type >= 0){
		s->damage_taken = 0;
		s->energy = s->prototype->energy;
	}
}



void RandomStartDirection(SpriteClass*sprite){

	sprite->a = ((rand() % 2 )*2 - 1) * sprite->prototype->max_speed / 3.5;
	
	/*while (sprite->a == 0) {
		sprite->a = ((rand()%2 - rand()%2) * sprite->prototype->max_speed) / 3.5;//2;
	}*/
}

void RandomStartDirectionVert(SpriteClass*sprite){
	sprite->b = ((rand() % 2 )*2 - 1) * sprite->prototype->max_speed / 3.5;
	
	/*while (sprite->b == 0) {
		sprite->b = ((rand()%2 - rand()%2) * sprite->prototype->max_speed) / 3.5;//2;
	}*/
}

void StartFacingThePlayer(SpriteClass*sprite){

	if(player==nullptr) return;

	if(sprite->prototype->max_speed==0){
		sprite->flip_x = sprite->x > player->x;
	}
	else{
		if (sprite->x < player->x)
		sprite->a = sprite->prototype->max_speed / 3.5;

		if (sprite->x > player->x)
			sprite->a = (sprite->prototype->max_speed * -1) / 3.5;
	}
}

void StartFacingThePlayerVert(SpriteClass*sprite){

	if(player==nullptr) return;

	if (sprite->y < player->y)
		sprite->b = sprite->prototype->max_speed / -3.5;

	if (sprite->y > player->y)
		sprite->b = sprite->prototype->max_speed / 3.5;

}


void StartDown(SpriteClass*s){
	s->b = s->prototype->max_speed / 3.5;
}

void StartUp(SpriteClass*s){
	s->b = -s->prototype->max_speed / 3.5;
}

void StartLeft(SpriteClass*s){

	if(s->prototype->max_speed==0){
		s->flip_x = true;
	}
	else{
		s->a = -s->prototype->max_speed / 3.5;
	}	
}
void StartRight(SpriteClass*s){
	if(s->prototype->max_speed==0){
		s->flip_x = false;
	}
	else{
		s->a = s->prototype->max_speed / 3.5;
	}
}

void DieIfSkullBlocksChanged(SpriteClass*sprite){
	sprite->damage_taken = sprite->energy + 1;
	sprite->damage_taken_type = DAMAGE_SELF_DESTRUCTION;
	sprite->self_destruction = true;
}

/**
 * @brief 
 * 
 */
void ThrowableWeapon(SpriteClass*sprite, SpriteClass*shooter){
	if ((int)shooter->a == 0){
		// If the "shooter" is a player or the speed of the projectile is zero
		if (shooter->isPlayer() || sprite->prototype->max_speed == 0){
			if (!shooter->flip_x)
				sprite->a = sprite->prototype->max_speed;
			else
				sprite->a = -sprite->prototype->max_speed;
		}
		else { // or, in the case of an enemy
			if (!shooter->flip_x)
				sprite->a = 1 + rand()%(int)sprite->prototype->max_speed;
			else
				sprite->a = -1 - rand()%-(int)sprite->prototype->max_speed;
		}
	}
	else{
		/**
		 * @brief 
		 * Adding the shooter speed has no effect in this case,
		 * beacuse the projectile speed is decreased to max_speed in physics.cpp
		 */
		if (!shooter->flip_x)
			sprite->a = sprite->prototype->max_speed + shooter->a;
		else
			sprite->a = -sprite->prototype->max_speed + shooter->a;

		//sprite->a = emo->a * 1.5;

	}

	sprite->jump_timer = 1;
}

void ThrowableWeapon2(SpriteClass*sprite, SpriteClass*shooter){

	double speed = sprite->prototype->max_speed/3.5;

	if (!shooter->flip_x){
		sprite->a = speed;
	}
	else{
		sprite->a = -speed;
	}
	
	sprite->a += shooter->a;
	sprite->jump_timer = 1;
}

void ProjectileEgg(SpriteClass*sprite, SpriteClass*shooter){
	if(!shooter->prototype->ammo1_offset.has_value()){
		sprite->y = shooter->y+10;
	}
	sprite->a = shooter->a / 1.5;
}

void StaticProjectile(SpriteClass*sprite, SpriteClass*shooter){
	sprite->a = 0;
	sprite->b = 0;
}


void ProjectileAimRandomly(SpriteClass*sprite, SpriteClass*){
	double angle = (double(rand())/RAND_MAX) * 2 * M_PI;
	double v = sprite->prototype->max_speed;

	sprite->a = v * cos(angle);
	sprite->b = v * sin(angle);
}

void ProjectileAimAtPlayer(SpriteClass*sprite, SpriteClass*shooter){
	if(player==nullptr){
		ProjectileAimRandomly(sprite, shooter);
	}
	else{
		double v = sprite->prototype->max_speed;

		double dx = player->x - sprite->x;
		double dy = player->y - sprite->y;

		double eps2 = dx*dx + dy*dy; 
		
		if(eps2 <= v*v){
			ProjectileAimRandomly(sprite, shooter);
		}
		else{
			double z = sqrt(eps2);
			sprite->a = v * dx / z;
			sprite->b = v * dy / z;
		}
	}
}


void DisplayInfo(SpriteClass* sprite){
	std::string sinfo = "info";

	int info = sprite->prototype->info_id;
	if(info <= 0)return;

	if (info < 10) sinfo += '0';
	sinfo += std::to_string(info);

	int index = Episode->infos.Search_Id(sinfo.c_str());
	if (index != -1)
		Game->showInfo(Episode->infos.Get_Text(index));
	else if(info<(int)PK_txt.infos.size())
		Game->showInfo(tekstit->Get_Text(PK_txt.infos[info]));
}

void DisplayInfoIfTouchesPlayer(SpriteClass* sprite){
    if (player_invisible!=nullptr &&
    (player_invisible->x - sprite->x < 10
    && player_invisible->x - sprite->x > -10) &&
    (player_invisible->y - sprite->y < sprite->prototype->height
    && player_invisible->y - sprite->y > -sprite->prototype->height)){

		DisplayInfo(sprite);
    }
}


void DisplayMissingSpriteWarning(SpriteClass* sprite){
	if (player_invisible!=nullptr &&
		(player_invisible->x - sprite->x < 10
		&& player_invisible->x - sprite->x > -10) &&
		(player_invisible->y - sprite->y < sprite->prototype->height
		&& player_invisible->y - sprite->y > -sprite->prototype->height)){
	
		Game->showInfo(std::string("Missing sprite!\n") + sprite->prototype->filename + ".spr/.spr2");
	}
}

void DisplayMissingTextureWarning(SpriteClass* sprite){
	if (player_invisible!=nullptr &&
		(player_invisible->x - sprite->x < 10
		&& player_invisible->x - sprite->x > -10) &&
		(player_invisible->y - sprite->y < sprite->prototype->height
		&& player_invisible->y - sprite->y > -sprite->prototype->height)){
	
		Game->showInfo(std::string("Missing texture!\n") + sprite->prototype->picture_filename);
	}
}


}
