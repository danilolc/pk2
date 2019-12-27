//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "sprite.hpp"

extern PK2Sprite* Player_Sprite;

extern PK2Sprite_Prototyyppi Prototypes_List[MAX_PROTOTYYPPEJA];
extern PK2Sprite Sprites_List[MAX_SPRITEJA];

extern int bgSprites_List[MAX_SPRITEJA];

void Prototypes_ClearAll();
int  Prototypes_GetAll();

void Sprites_add_bg(int index);
void Sprites_sort_bg();
void Sprites_start_directions();
void Sprites_add(int protoype_id, int is_Player_Sprite, double x, double y, int emo, bool isbonus);
void Sprites_add_ammo(int protoype_id, int is_Player_Sprite, double x, double y, int emo);
void Sprites_clear();