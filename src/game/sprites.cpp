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

#include <cstring>

SpriteClass* Player_Sprite;

int next_free_prototype = 0;

//TODO - use std::vector
//std::vector<PrototypeClass> Prototypes;
//std::vector<SpriteClass> Sprites;
//std::vector<int> bgSprites;
//
//or
//
//std::vector<PrototypeClass*> Prototypes;
//std::vector<SpriteClass*> Sprites;
//std::vector<SpriteClass*> bgSprites;

PrototypeClass Prototypes_List[MAX_PROTOTYYPPEJA];
SpriteClass Sprites_List[MAX_SPRITEJA];

int bgSprites_List[MAX_SPRITEJA];

void Prototypes_ClearAll() {
	for (int i = 0; i < MAX_PROTOTYYPPEJA; i++) {

		for (int j = 0; j < SPRITE_MAX_SOUNDS; j++)
			if (Prototypes_List[i].aanet[j] > -1)
				PSound::free_sfx(Prototypes_List[i].aanet[j]);
		Prototypes_List[i].Uusi();
		strcpy(Game->map->protot[i],"");

	}

	next_free_prototype = 0;
}

int Prototypes_get(const char* name) {
	
	//Check if have space
	if(next_free_prototype >= MAX_PROTOTYYPPEJA)
		return 2;

	PrototypeClass& protot = Prototypes_List[next_free_prototype];

	PFile::Path path = Episode->Get_Dir(name);
	FindAsset(&path, "sprites" PE_SEP);

	//Check if it can be loaded
	if (protot.Load(path) == 1)
		return 1;

	protot.indeksi = next_free_prototype;

	//Load sounds
	for (int i = 0; i < SPRITE_MAX_SOUNDS; i++) {

		if (strcmp(protot.aanitiedostot[i],"") != 0) {

			path = Episode->Get_Dir(protot.aanitiedostot[i]);

			if (FindAsset(&path, "sprites" PE_SEP)) {

				protot.aanet[i] = PSound::load_sfx(path);

			} else {

				PLog::Write(PLog::ERR, "PK2", "Can't find sound %s", protot.aanitiedostot[i]);

			}
		}
	}

	next_free_prototype++;

	return 0;
}

void Prototypes_get_transformation(int i) {

	int j = 0;
	bool loaded = false;

	if (strcmp(Prototypes_List[i].muutos_sprite,"") != 0) {

		// verify if the transformation is already loaded
		while (j < MAX_PROTOTYYPPEJA && !loaded) {
			if (j!=i) {
				if (strcmp(Prototypes_List[i].muutos_sprite,Prototypes_List[j].tiedosto)==0) {
					Prototypes_List[i].muutos = j;
					loaded = true;
				}
			}
			j++;
		}

		if (!loaded) {

			if (Prototypes_get(Prototypes_List[i].muutos_sprite) == 0)
				Prototypes_List[i].muutos = next_free_prototype - 1;
			
		}
	}
}

void Prototypes_get_bonus(int i) {

	int j = 0;
	bool loaded = false;

	if (strcmp(Prototypes_List[i].bonus_sprite,"")!=0){
		while (j<MAX_PROTOTYYPPEJA && !loaded){
			if (j!=i){
				if (strcmp(Prototypes_List[i].bonus_sprite,Prototypes_List[j].tiedosto)==0){
					Prototypes_List[i].bonus = j;
					loaded = true;
				}
			}

			j++;
		}

		if (!loaded){

			if (Prototypes_get(Prototypes_List[i].bonus_sprite) == 0)
				Prototypes_List[i].bonus = next_free_prototype - 1;
			
		}
	}
}

void Prototypes_get_ammo1(int i) {

	int j = 0;
	bool loaded = false;

	if (strcmp(Prototypes_List[i].ammus1_sprite,"")!=0){
		while (j<MAX_PROTOTYYPPEJA && !loaded){
			if (j!=i){
				if (strcmp(Prototypes_List[i].ammus1_sprite,Prototypes_List[j].tiedosto)==0){
					Prototypes_List[i].ammus1 = j;
					loaded = true;
				}
			}

			j++;
		}

		if (!loaded){

			if (Prototypes_get(Prototypes_List[i].ammus1_sprite) == 0)
				Prototypes_List[i].ammus1 = next_free_prototype - 1;

		}
	}
}

void Prototypes_get_ammo2(int i) {

	int j = 0;
	bool loaded = false;

	if (strcmp(Prototypes_List[i].ammus2_sprite,"")!=0){
		while (j<MAX_PROTOTYYPPEJA && !loaded){
			if (j!=i){
				if (strcmp(Prototypes_List[i].ammus2_sprite,Prototypes_List[j].tiedosto)==0){
					Prototypes_List[i].ammus2 = j;
					loaded = true; //Prototype has already loaded
				}
			}

			j++;
		}

		if (!loaded) {

			if (Prototypes_get(Prototypes_List[i].ammus2_sprite) == 0)
				Prototypes_List[i].ammus2 = next_free_prototype - 1;

		}
	}
}

int Prototypes_GetAll() {

	int last_prototype = -1;

	for (int i = 0; i < MAX_PROTOTYYPPEJA; i++) {
		if (strcmp(Game->map->protot[i], "") != 0) {

			last_prototype = i;

			PFile::Path path = Episode->Get_Dir(Game->map->protot[i]);

			if (Prototypes_get(Game->map->protot[i]) != 0) {

				PLog::Write(PLog::WARN, "PK2", "Can't load sprite %s. It will not appear", Game->map->protot[i]);
				next_free_prototype++;

			}

		} else {

			next_free_prototype++;

		}
	}

	next_free_prototype = last_prototype + 1;

	for (int i = 0; i < MAX_PROTOTYYPPEJA; i++) {

		Prototypes_get_transformation(i);
		Prototypes_get_bonus(i);
		Prototypes_get_ammo1(i);
		Prototypes_get_ammo2(i);

	}

	return 0;
}

void Sprites_add_bg(int index) {
	for (int i=0;i<MAX_SPRITEJA;i++){
		if (bgSprites_List[i] == -1){
			bgSprites_List[i] = index;
			return;
		}
	}
}

void Sprites_sort_bg() {
	bool lopeta = false;
	int l = 1;
	int vali;

	while (l < MAX_SPRITEJA && lopeta == false)
	{
		lopeta = true;

		for (int i=0;i<MAX_SPRITEJA-1;i++)
		{
			if (bgSprites_List[i] == -1 || bgSprites_List[i+1] == -1)
				i = MAX_SPRITEJA;
			else
			{
				if (Sprites_List[bgSprites_List[i]].tyyppi->pallarx_kerroin > Sprites_List[bgSprites_List[i+1]].tyyppi->pallarx_kerroin)
				{
					vali = bgSprites_List[i];
					bgSprites_List[i]   = bgSprites_List[i+1];
					bgSprites_List[i+1] = vali;
					lopeta = false;
				}
			}
		}
		l++;
	}
}

void Sprites_start_directions() {
	for (int i = 0; i < MAX_SPRITEJA; i++){
		if (/*pelaaja_index >= 0 && pelaaja_index < MAX_SPRITEJA && */!Sprites_List[i].piilota){
			Sprites_List[i].a = 0;

			if (Sprites_List[i].tyyppi->Onko_AI(AI_RANDOM_START_DIRECTION)){
				while (Sprites_List[i].a == 0) {
					Sprites_List[i].a = ((rand()%2 - rand()%2) * Sprites_List[i].tyyppi->max_nopeus) / 3.5;//2;
				}
			}

			if (Sprites_List[i].tyyppi->Onko_AI(AI_RANDOM_ALOITUSSUUNTA_VERT)){
				while (Sprites_List[i].b == 0) {
					Sprites_List[i].b = ((rand()%2 - rand()%2) * Sprites_List[i].tyyppi->max_nopeus) / 3.5;//2;
				}
			}

			if (Sprites_List[i].tyyppi->Onko_AI(AI_START_DIRECTIONS_TOWARDS_PLAYER)){

				if (Sprites_List[i].x < Player_Sprite->x)
					Sprites_List[i].a = Sprites_List[i].tyyppi->max_nopeus / 3.5;

				if (Sprites_List[i].x > Player_Sprite->x)
					Sprites_List[i].a = (Sprites_List[i].tyyppi->max_nopeus * -1) / 3.5;
			}

			if (Sprites_List[i].tyyppi->Onko_AI(AI_START_DIRECTIONS_TOWARDS_PLAYER_VERT)){

				if (Sprites_List[i].y < Player_Sprite->y)
					Sprites_List[i].b = Sprites_List[i].tyyppi->max_nopeus / -3.5;

				if (Sprites_List[i].y > Player_Sprite->y)
					Sprites_List[i].b = Sprites_List[i].tyyppi->max_nopeus / 3.5;
			}
		}
	}
}

void Sprites_add(int protoype_id, int is_Player_Sprite, double x, double y, int emo, bool isbonus) {
	
	PrototypeClass& proto = Prototypes_List[protoype_id];
	bool added = false;
	int i = 0;

	while (!added && i < MAX_SPRITEJA){
		if (Sprites_List[i].piilota && &Sprites_List[i] != Player_Sprite){
			
			Sprites_List[i] = SpriteClass(&proto,is_Player_Sprite,false,x,y);

			if (is_Player_Sprite) Player_Sprite = &Sprites_List[i];

			if(isbonus) { //If it is a bonus dropped by enemy

				Sprites_List[i].x += Sprites_List[i].tyyppi->leveys;
				Sprites_List[i].y += Sprites_List[i].tyyppi->korkeus/2;
				Sprites_List[i].alku_x = Sprites_List[i].x;
				Sprites_List[i].alku_y = Sprites_List[i].y;
				Sprites_List[i].jump_timer = 1;
				Sprites_List[i].a = rand()%2 - rand()%4;
				Sprites_List[i].damage_timer = 35;//25

			} else {

				Sprites_List[i].x = x + 16 + 1;
				Sprites_List[i].y += Sprites_List[i].tyyppi->korkeus/2;
				Sprites_List[i].alku_x = Sprites_List[i].x;
				Sprites_List[i].alku_y = Sprites_List[i].y;
				
			}

			if (proto.tyyppi == TYPE_BACKGROUND)
				Sprites_add_bg(i);

			if (emo != MAX_SPRITEJA)
				Sprites_List[i].emosprite = emo;
			else
				Sprites_List[i].emosprite = i;

			added = true;

		}
		else
			i++;
	}
}

void Sprites_add_ammo(int protoype_id, int is_Player_Sprite, double x, double y, int emo) {
	PrototypeClass& proto = Prototypes_List[protoype_id];
	bool lisatty = false;
	int i = 0;

	while (!lisatty && i < MAX_SPRITEJA){
		if (Sprites_List[i].piilota && &Sprites_List[i] != Player_Sprite){ //Don't replace player sprite

			Sprites_List[i] = SpriteClass(&proto,is_Player_Sprite,false,x/*-proto.leveys/2*/,y);

			//Sprites_List[i].x += Sprites_List[i].tyyppi->leveys;
			//Sprites_List[i].y += Sprites_List[i].tyyppi->korkeus/2;

			if (proto.Onko_AI(AI_HEITTOASE)){
				if ((int)Sprites_List[emo].a == 0){
					// Jos "ampuja" on pelaaja tai ammuksen nopeus on nolla
					if (Sprites_List[emo].pelaaja == 1 || Sprites_List[i].tyyppi->max_nopeus == 0){
						if (!Sprites_List[emo].flip_x)
							Sprites_List[i].a = Sprites_List[i].tyyppi->max_nopeus;
						else
							Sprites_List[i].a = -Sprites_List[i].tyyppi->max_nopeus;
					}
					else{ // tai jos kyseess� on vihollinen
						if (!Sprites_List[emo].flip_x)
							Sprites_List[i].a = 1 + rand()%(int)Sprites_List[i].tyyppi->max_nopeus;
						else
							Sprites_List[i].a = -1 - rand()%-(int)Sprites_List[i].tyyppi->max_nopeus;
					}
				}
				else{
					if (!Sprites_List[emo].flip_x)
						Sprites_List[i].a = Sprites_List[i].tyyppi->max_nopeus + Sprites_List[emo].a;
					else
						Sprites_List[i].a = -Sprites_List[i].tyyppi->max_nopeus + Sprites_List[emo].a;

					//Sprites_List[i].a = Sprites_List[emo].a * 1.5;

				}

				Sprites_List[i].jump_timer = 1;
			}
			else
			if (proto.Onko_AI(AI_EGG)){
				Sprites_List[i].y = Sprites_List[emo].y+10;
				Sprites_List[i].a = Sprites_List[emo].a / 1.5;
			}
			else{
				if (!Sprites_List[emo].flip_x)
					Sprites_List[i].a = Sprites_List[i].tyyppi->max_nopeus;
				else
					Sprites_List[i].a = -Sprites_List[i].tyyppi->max_nopeus;
			}

			if (emo != MAX_SPRITEJA){
				Sprites_List[i].emosprite = emo;
				Sprites_List[i].vihollinen = Sprites_List[emo].vihollinen;
			}
			else{
				Sprites_List[i].emosprite = i;
			}

			if (proto.tyyppi == TYPE_BACKGROUND)
				Sprites_add_bg(i);

			lisatty = true;
		}
		else
			i++;
	}
}

int Update_Sprites() {
	
	const int ACTIVE_BORDER_X = 320;
	const int ACTIVE_BORDER_y = 240;

	int active_sprites = 0;
	SpriteClass* sprite;

	for (int i = 0; i < MAX_SPRITEJA; i++){ //Activate sprite if it is on screen
		
		sprite = &Sprites_List[i];
		
		if (sprite->x < Game->camera_x + 640 + ACTIVE_BORDER_X &&
			sprite->x > Game->camera_x - ACTIVE_BORDER_X &&
			sprite->y < Game->camera_y + 480 + ACTIVE_BORDER_y &&
			sprite->y > Game->camera_y - ACTIVE_BORDER_y)
			sprite->aktiivinen = true;
		else
			sprite->aktiivinen = false;

		if (sprite->piilota == true)
			sprite->aktiivinen = false;
	
	}

	// Update bonus first to get energy change
	for (int i = 0; i < MAX_SPRITEJA; i++) {
		sprite = &Sprites_List[i];
		if (sprite->aktiivinen && sprite->tyyppi->tyyppi != TYPE_BACKGROUND){
			if (sprite->tyyppi->tyyppi == TYPE_BONUS) {
				BonusSprite_Movement(i);
				active_sprites++;
			}
		}
	}

	for (int i = 0; i < MAX_SPRITEJA; i++) {
		sprite = &Sprites_List[i];
		if (sprite->aktiivinen && sprite->tyyppi->tyyppi != TYPE_BACKGROUND){
			if (sprite->tyyppi->tyyppi != TYPE_BONUS) {
				Sprite_Movement(i);
				active_sprites++;
			}
		}
	}

	return active_sprites;
}

void Sprites_clear() {
	int i = 0;

	while (i < MAX_SPRITEJA){
		Sprites_List[i] = SpriteClass();
		bgSprites_List[i] = -1;
		i++;
	}

	Player_Sprite = NULL;
}