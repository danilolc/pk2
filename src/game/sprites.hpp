//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "game/spriteclass.hpp"

#include <list>

extern PrototypeClass* Prototypes_List[MAX_PROTOTYYPPEJA];

extern std::list<SpriteClass*> Sprites_List;
extern std::list<SpriteClass*> bgSprites_List;
extern SpriteClass* Player_Sprite;

void Prototypes_ClearAll();
int  Prototypes_GetAll();

void Sprites_add_bg(int index);
void Sprites_sort_bg();
void Sprites_start_directions();
void Sprites_add(PrototypeClass* protot, int is_Player_Sprite, double x, double y, SpriteClass* emo, bool isbonus);
void Sprites_add_ammo(PrototypeClass* protot, double x, double y, SpriteClass* emo);
void Sprites_clear();

int Update_Sprites();