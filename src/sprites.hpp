#pragma once

#include "sprite.hpp"

extern PK2Sprite* Player_Sprite;

extern PK2Sprite Sprites_List[MAX_SPRITEJA];
extern PK2Sprite_Prototyyppi Prototypes_List[MAX_PROTOTYYPPEJA];

extern int bgSprites_List[MAX_SPRITEJA];

void Prototypes_clear_all();
int  Prototypes_get_sound(char *polku, char *tiedosto);
int  Prototypes_get(char *polku, char *tiedosto);
void Prototypes_get_transformation(int i);
void Prototypes_get_bonus(int i);
void Prototypes_get_ammo1(int i);
void Prototypes_get_ammo2(int i);
int  Prototypes_get_all();

void Sprites_add_bg(int index);
void Sprites_sort_bg();
void Sprites_start_directions();
void Sprites_add(int protoype_id, int is_Player_Sprite, double x, double y, int emo, bool isbonus);
void Sprites_add_ammo(int protoype_id, int is_Player_Sprite, double x, double y, int emo);
void Sprites_clear();