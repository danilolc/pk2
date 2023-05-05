//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "game/sprites.hpp"

#include "engine/PSound.hpp"
#include "engine/PUtils.hpp"
#include "engine/PLog.hpp"

#include "game/game.hpp"
#include "episode/episodeclass.hpp"
#include "physics.hpp"
#include "system.hpp"

#include <algorithm>
#include <cstring>
#include <vector>

PrototypeClass* Prototypes_List[MAX_PROTOTYYPPEJA] = {nullptr};

std::list<SpriteClass*> Sprites_List;
std::list<SpriteClass*> bgSprites_List;
SpriteClass* Player_Sprite = nullptr;

void Prototypes_ClearAll() {

	for (int i = 0; i < MAX_PROTOTYYPPEJA; i++) {
		if (Prototypes_List[i] != nullptr) {
			delete Prototypes_List[i];
			Prototypes_List[i] = nullptr;
		}

	}

}

PrototypeClass* Prototypes_get(const char* name, int id = -1) {

	PFile::Path path = Episode->Get_Dir(name);
	if (!FindAsset(&path, "sprites" PE_SEP)) {
		PLog::Write(PLog::ERR, "PK2", "Couldn't find %s", name);
		return nullptr;
	}

	if (id >= MAX_PROTOTYYPPEJA) {
		PLog::Write(PLog::ERR, "PK2", "Invalid prototype id");
		return nullptr;
	}

	if (id != -1 && Prototypes_List[id] != nullptr) {
		delete Prototypes_List[id];
		Prototypes_List[id] = nullptr;
	}

	if (id == -1) {
		for (int i = 0; i < MAX_PROTOTYYPPEJA; i++) {
			if (Prototypes_List[i] == nullptr) {
				id = i;
				break;
			}
		}
	}

	if (id == -1) {
		PLog::Write(PLog::ERR, "PK2", "Not enough space for prototypes");
		return nullptr;
	}

	PrototypeClass* protot = new PrototypeClass();

	//Check if it can be loaded
	if (protot->Load(path) != 0) {
		PLog::Write(PLog::ERR, "PK2", "Couldn't load %s", name);
		delete protot;
		return nullptr;
	}

	Prototypes_List[id] = protot;

	return protot;
}

void Prototypes_get_transformation(int i) {

	if (strcmp(Prototypes_List[i]->muutos_sprite, "") != 0) {

		// verify if the transformation is already loaded
		for (int j = 0; j < MAX_PROTOTYYPPEJA; j++) {
			if (Prototypes_List[j] != nullptr) {
				if (strcmp(Prototypes_List[i]->muutos_sprite, Prototypes_List[j]->tiedosto) == 0) {
					Prototypes_List[i]->muutos = Prototypes_List[j];
					return;
				}
			}
		}

		Prototypes_List[i]->muutos = Prototypes_get(Prototypes_List[i]->muutos_sprite);

	}
}

void Prototypes_get_bonus(int i) {

	if (strcmp(Prototypes_List[i]->bonus_sprite, "") != 0) {

		// verify if the transformation is already loaded
		for (int j = 0; j < MAX_PROTOTYYPPEJA; j++) {
			if (Prototypes_List[j] != nullptr) {
				if (strcmp(Prototypes_List[i]->bonus_sprite, Prototypes_List[j]->tiedosto) == 0) {
					Prototypes_List[i]->bonus = Prototypes_List[j];
					return;
				}
			}
		}

		Prototypes_List[i]->bonus = Prototypes_get(Prototypes_List[i]->bonus_sprite);

	}
}

void Prototypes_get_ammo1(int i) {

	if (strcmp(Prototypes_List[i]->ammus1_sprite, "") != 0) {

		// verify if the transformation is already loaded
		for (int j = 0; j < MAX_PROTOTYYPPEJA; j++) {
			if (Prototypes_List[j] != nullptr) {
				if (strcmp(Prototypes_List[i]->ammus1_sprite, Prototypes_List[j]->tiedosto) == 0) {
					Prototypes_List[i]->ammus1 = Prototypes_List[j];
					return;
				}
			}
		}

		Prototypes_List[i]->ammus1 = Prototypes_get(Prototypes_List[i]->ammus1_sprite);

	}
}

void Prototypes_get_ammo2(int i) {

	if (strcmp(Prototypes_List[i]->ammus2_sprite, "") != 0) {

		// verify if the transformation is already loaded
		for (int j = 0; j < MAX_PROTOTYYPPEJA; j++) {
			if (Prototypes_List[j] != nullptr) {
				if (strcmp(Prototypes_List[i]->ammus2_sprite, Prototypes_List[j]->tiedosto) == 0) {
					Prototypes_List[i]->ammus2 = Prototypes_List[j];
					return;
				}
			}
		}

		Prototypes_List[i]->ammus2 = Prototypes_get(Prototypes_List[i]->ammus2_sprite);

	}
}

int Prototypes_GetAll() {

	for (u32 i = 0; i < PK2MAP_MAP_MAX_PROTOTYPES; i++) {
		if (strcmp(Game->map.protot[i], "") != 0) {

			PFile::Path path = Episode->Get_Dir(Game->map.protot[i]);

			if (Prototypes_get(Game->map.protot[i], i) == nullptr) {
				
				PLog::Write(PLog::WARN, "PK2", "Can't load sprite %s. It will not appear", Game->map.protot[i]);

			}

		}
	}

	for (int i = 0; i < MAX_PROTOTYYPPEJA; i++) {
		if (Prototypes_List[i] != nullptr) {

			Prototypes_get_transformation(i);
			Prototypes_get_bonus(i);
			Prototypes_get_ammo1(i);
			Prototypes_get_ammo2(i);

		}
	}

	return 0;
}

void Sprites_add_bg(SpriteClass* sprite) {

	bgSprites_List.push_back(sprite);

}

bool Compare_bgSprites(SpriteClass* s1, SpriteClass* s2) {

	return (s1->tyyppi->pallarx_kerroin < s2->tyyppi->pallarx_kerroin); 
	
}

void Sprites_sort_bg() {
	
	bgSprites_List.sort(Compare_bgSprites);

}

void Sprites_start_directions() {
	for (SpriteClass* sprite : Sprites_List) {
		sprite->a = 0;

		if (sprite->Onko_AI(AI_RANDOM_START_DIRECTION)){
			while (sprite->a == 0) {
				sprite->a = ((rand()%2 - rand()%2) * sprite->tyyppi->max_nopeus) / 3.5;//2;
			}
		}

		if (sprite->Onko_AI(AI_RANDOM_ALOITUSSUUNTA_VERT)){
			while (sprite->b == 0) {
				sprite->b = ((rand()%2 - rand()%2) * sprite->tyyppi->max_nopeus) / 3.5;//2;
			}
		}

		if (sprite->Onko_AI(AI_START_DIRECTIONS_TOWARDS_PLAYER)){

			if (sprite->x < Player_Sprite->x)
				sprite->a = sprite->tyyppi->max_nopeus / 3.5;

			if (sprite->x > Player_Sprite->x)
				sprite->a = (sprite->tyyppi->max_nopeus * -1) / 3.5;
		}

		if (sprite->Onko_AI(AI_START_DIRECTIONS_TOWARDS_PLAYER_VERT)){

			if (sprite->y < Player_Sprite->y)
				sprite->b = sprite->tyyppi->max_nopeus / -3.5;

			if (sprite->y > Player_Sprite->y)
				sprite->b = sprite->tyyppi->max_nopeus / 3.5;
		}
	}
}

void Sprites_add(PrototypeClass* protot, int is_Player_Sprite, double x, double y, SpriteClass* emo, bool isbonus) {

	SpriteClass* sprite = new SpriteClass(protot, is_Player_Sprite, x, y);
	Sprites_List.push_back(sprite);

	if (is_Player_Sprite) Player_Sprite = sprite;

	if(isbonus) { //If it is a bonus dropped by enemy

		sprite->x += sprite->tyyppi->leveys;
		sprite->y += sprite->tyyppi->korkeus/2;
		sprite->alku_x = sprite->x;
		sprite->alku_y = sprite->y;
		sprite->jump_timer = 1;
		sprite->a = rand()%2 - rand()%4;
		sprite->damage_timer = 35;//25

	} else {

		sprite->x = x + 16 + 1;
		sprite->y += sprite->tyyppi->korkeus/2;
		sprite->alku_x = sprite->x;
		sprite->alku_y = sprite->y;
		
	}

	if (protot->type == TYPE_BACKGROUND)
		Sprites_add_bg(sprite);

	if (emo != nullptr)
		sprite->emosprite = emo;
	else
		sprite->emosprite = sprite;
	
}

void Sprites_add_ammo(PrototypeClass* protot, double x, double y, SpriteClass* emo) {

	//SpriteClass(proto, is_Player_Sprite,false,x-proto->leveys/2,y);
	SpriteClass* sprite = new SpriteClass(protot, false, x, y);
	Sprites_List.push_back(sprite);

	//sprite->x += sprite->tyyppi->leveys;
	//sprite->y += sprite->tyyppi->korkeus/2;

	if (protot->Onko_AI(AI_HEITTOASE)){
		if ((int)emo->a == 0){
			// If the "shooter" is a player or the speed of the projectile is zero
			if (emo->pelaaja == 1 || sprite->tyyppi->max_nopeus == 0){
				if (!emo->flip_x)
					sprite->a = sprite->tyyppi->max_nopeus;
				else
					sprite->a = -sprite->tyyppi->max_nopeus;
			}
			else { // or, in the case of an enemy
				if (!emo->flip_x)
					sprite->a = 1 + rand()%(int)sprite->tyyppi->max_nopeus;
				else
					sprite->a = -1 - rand()%-(int)sprite->tyyppi->max_nopeus;
			}
		}
		else{
			if (!emo->flip_x)
				sprite->a = sprite->tyyppi->max_nopeus + emo->a;
			else
				sprite->a = -sprite->tyyppi->max_nopeus + emo->a;

			//sprite->a = emo->a * 1.5;

		}

		sprite->jump_timer = 1;
	}
	else
	if (protot->Onko_AI(AI_EGG) || protot->Onko_AI(AI_EGG2)){
		sprite->y = emo->y+10;
		sprite->a = emo->a / 1.5;
	}
	else{
		if (!emo->flip_x)
			sprite->a = sprite->tyyppi->max_nopeus;
		else
			sprite->a = -sprite->tyyppi->max_nopeus;
	}

	if (emo != nullptr){
		sprite->emosprite = emo;
		sprite->vihollinen = emo->vihollinen;
	}
	else{
		sprite->emosprite = sprite;
	}

	if (protot->type == TYPE_BACKGROUND)
		Sprites_add_bg(sprite);

}

bool Sprite_Destructed (SpriteClass* sprite) { 

	if (sprite == Player_Sprite) // Never remove the player
		return false;
	
	if (sprite->piilota) {
		if (sprite->Onko_AI(AI_CHICK)) // Killed the chick
			Game->game_over = true;
		
		delete sprite;
		return true;
	}

	return false;
	
}

bool bgSprite_Destructed (SpriteClass* sprite) { 
	
	if (sprite->piilota)
		return true;

	return false;

}

int Update_Sprites() {
	
	const int ACTIVE_BORDER_X = 320;
	const int ACTIVE_BORDER_y = 240;

	int active_sprites = 0;

	//Activate sprite if it is next to the screen
	for (SpriteClass* sprite : Sprites_List) {
		
		if (sprite->x < Game->camera_x + 640 + ACTIVE_BORDER_X &&
			sprite->x > Game->camera_x - ACTIVE_BORDER_X &&
			sprite->y < Game->camera_y + 480 + ACTIVE_BORDER_y &&
			sprite->y > Game->camera_y - ACTIVE_BORDER_y)
			sprite->aktiivinen = true;
		else
			sprite->aktiivinen = false;
	
	}

	// Update bonus first to get energy change
	for (SpriteClass* sprite : Sprites_List) {
		if (sprite->aktiivinen && !sprite->piilota) {
			if (sprite->tyyppi->type == TYPE_BONUS) {
				BonusSprite_Movement(sprite);
				active_sprites++;
			}
		}
	}

	for (SpriteClass* sprite : Sprites_List) {
		if (sprite->aktiivinen && !sprite->piilota) {
			if (sprite->tyyppi->type != TYPE_BONUS && sprite->tyyppi->type != TYPE_BACKGROUND) {
				Sprite_Movement(sprite);
				active_sprites++;
			}
		}
	}

	// Clean destructed sprites
	bgSprites_List.remove_if(bgSprite_Destructed);
	Sprites_List.remove_if(Sprite_Destructed);

	/*int count = 0;
	for (SpriteClass* sprite : Sprites_List) {
		if (sprite->energia > 0) {
			if (sprite->tyyppi->type == TYPE_BONUS)
				count++;
			if (sprite->tyyppi->bonus > -1 && sprite->tyyppi->bonusten_lkm > 0)
				count += sprite->tyyppi->bonusten_lkm;

		}
	}

	PLog::Write(PLog::DEBUG, "PK2", "%i", count);
	*/
	return active_sprites;

}

void Sprites_clear() {

	for (SpriteClass* sprite : Sprites_List) {
		delete sprite;
	}

	Sprites_List.clear();
	bgSprites_List.clear();
	Player_Sprite = nullptr;
}